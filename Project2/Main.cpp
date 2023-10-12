#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "csv_util.h"
#include <vector>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "opencv2/opencv.hpp"


using namespace cv;
using namespace std;


void match(string im, int n);
void histogram_match(string im, int n);
void multi_histogram_match(string im, int n);
void texture_match(string im, int n);
void custom(string im, int n);


int main(int argc, char* argv[])
{
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);


    if (m == 1) {
        match(argv[3], n);
    }
    else if (m == 2) {
        histogram_match(argv[3], n);
    }
    else if (m == 3) {
        multi_histogram_match(argv[3], n);
    }
    else if (m == 4) {
        texture_match(argv[3], n);
    }
    else if (m == 5) {
        custom(argv[3], n);
    }
    

	return 0;
}



//function to perform baseline matching
void match(string im, int n) {

    map<float, Mat> m;
    
    //reads all the files in the directory
    vector<cv::String> fn;
    glob("C:\\Users\\karth\\Desktop\\olympus\\*.jpg", fn, false);
    vector<Mat> images;
    size_t count = fn.size(); 
    cout << "total images: " << count << endl;
    for (size_t i = 0; i < count; i++)
        images.push_back(imread(fn[i]));

    
    Mat img = cv::imread(im);



    int x = ((img.rows)/ 2) - 4;
    int y = ((img.cols)/ 2) - 4;


    vector<float> v1;




    int c = 0;

    //gets the feature vector
    for (int i = x; i < x + 9; i++) {
        for (int j = y; j < y + 9; j++) {

            c++;
            Vec3b p = img.at<Vec3b>(i,j);
            v1.push_back(p[0]);
            v1.push_back(p[1]);
            v1.push_back(p[2]);

        }
        
    }


    Mat best;
    float bestValue = INT_MAX;

    //compares the feature vector with every image
    for (Mat img2 : images) {
        int x2 = ((img2.rows) / 2) - 4;
        int y2 = ((img2.cols) / 2) - 4;

        vector<float> v2;

        int c2 = 0;


        for (int i = x2; i < x2 + 9; i++) {
            for (int j = y2; j < y2 + 9; j++) {

                c2++;
                Vec3b p2 = img2.at<Vec3b>(i, j);
                v2.push_back(p2[0]);
                v2.push_back(p2[1]);
                v2.push_back(p2[2]);

            }

        }

        float sum = 0;
        for (int i = 0; i < 243; i++) {
            float s1 = v1[i] - v2[i];
            float s2 = s1 * s1;
            sum = sum + s2;
        }

        int v = sum / 243;
        cout << "c: " << v << endl;


        //stores the n closest images
        if (m.size() < n && v != 0) {
            m.insert({v,img2});

        }
        else if (m.rbegin()->first > v  && v !=0 ) {
            m.erase(std::prev(m.end()));
            m.insert({ v,img2 });

        }
    
    }

    cout << "msize: " << m.size() << endl;
    //outputs the images
    for (auto i : m) {
    
        imshow("img", i.second);
        for (;;) {
            char key = waitKey();
            if (key == 'q') {
                break;
            }
        }
    
    }



}



//function for histogram matching which uses rg Chromaticity
void histogram_match(string im, int n) {

    map<float, Mat> m;


    vector<cv::String> fn;
    glob("C:\\Users\\karth\\Desktop\\olympus\\*.jpg", fn, false);
    vector<Mat> images;
    size_t count = fn.size();
    cout << "total images: " << count << endl;
    for (size_t i = 0; i < count; i++)
        images.push_back(imread(fn[i]));


    Mat img = cv::imread(im);

    const int Hsize = 16;

    cv::Mat hist;

    int dim[2] = { Hsize,Hsize };

    hist = cv::Mat::zeros(2,dim, CV_32S );

    //calculating the feature of the target image
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
        
            Vec3b rgb = img.at<Vec3b>(i, j);


            float  r = (float)(rgb[0]) / (float)(rgb[0] + rgb[1] + rgb[2] + 1);
            float  g = (float)(rgb[1]) / (float)(rgb[0] + rgb[1] + rgb[2] + 1);
            

            int r1 = r * 16;
            int c1 = g * 16;

            int val = hist.at<int>(r1,c1); 
            hist.at<int>(r1,c1) = val + 1;

        }
    }


    for (Mat img2 : images) {
        Mat hist2;

        hist2 = Mat::zeros(2, dim, CV_32S);

        for (int i = 0; i < img2.rows; i++) {
            for (int j = 0; j < img2.cols; j++) {

                Vec3b rgb = img2.at<Vec3b>(i, j);

                float  r = (float)(rgb[0]) / (float)(rgb[0] + rgb[1] + rgb[2] + 1);
                float  g = (float)(rgb[1]) / (float)(rgb[0] + rgb[1] + rgb[2] + 1);


                int r1 = r * 16;
                int c1 = g * 16;

                hist2.at<int>(r1, c1) = hist2.at<int>(r1, c1) + 1;

            }

        }

        int sim = 0;
        //performing histogram intersection
        for (int i = 0; i < hist.rows; i++) {
            for (int j = 0; j < hist.cols; j++) {

                if (hist.at<int>(i, j) < hist2.at<int>(i, j)) {

                    sim = sim + hist.at<int>(i, j);
                }
                else {

                    sim = sim + hist2.at<int>(i, j);
                }


            }

        }


        if (m.size() < n ) {
            m.insert({ sim,img2 });

        }
        else if (m.begin()->first < sim ) {
           m.erase(m.begin()->first);
            m.insert({ sim,img2 });
        }
    
    }


    for (auto i : m) {

        imshow("img", i.second);
        
        for (;;) {
            char key = waitKey();
            if (key == 'q') {
                break;
            }
        }

    }
    
}

//Divides the image into to and uses rg chromacity histogram
void multi_histogram_match(string im, int n) {


    map<float, Mat> m;


    vector<cv::String> fn;
    glob("C:\\Users\\karth\\Desktop\\olympus\\*.jpg", fn, false);
    vector<Mat> images;
    size_t count = fn.size();
    cout << "total images: " << count << endl;
    for (size_t i = 0; i < count; i++)
        images.push_back(imread(fn[i]));


    Mat img = cv::imread(im);

    //splitting the image
    Mat img1 = img(Range(0,img.rows/2),Range(0,img.cols));
    Mat img11 = img(Range(img.rows / 2, img.rows), Range(0, img.cols));

    const int Hsize = 16;

    cv::Mat hist, hist1;

    int dim[2] = { Hsize,Hsize };

    hist = cv::Mat::zeros(2, dim, CV_32S);


    for (int i = 0; i < img1.rows; i++) {
        for (int j = 0; j < img1.cols; j++) {

            Vec3b rgb = img1.at<Vec3b>(i, j);


            float  r = (float)(rgb[0]) / (float)(rgb[0] + rgb[1] + rgb[2] + 1);
            float  g = (float)(rgb[1]) / (float)(rgb[0] + rgb[1] + rgb[2] + 1);


            int r1 = r * 16;
            int c1 = g * 16;

            int val = hist.at<int>(r1, c1);
            hist.at<int>(r1, c1) = val + 1;

        }

    }


    hist1 = cv::Mat::zeros(2, dim, CV_32S);


    for (int i = 0; i < img11.rows; i++) {
        for (int j = 0; j < img11.cols; j++) {

            Vec3b rgb = img11.at<Vec3b>(i, j);


            float  r = (float)(rgb[0]) / (float)(rgb[0] + rgb[1] + rgb[2] + 1);
            float  g = (float)(rgb[1]) / (float)(rgb[0] + rgb[1] + rgb[2] + 1);


            int r1 = r * 16;
            int c1 = g * 16;

            int val = hist1.at<int>(r1, c1);
            hist1.at<int>(r1, c1) = val + 1;

        }



    }



    for (Mat img2 : images) {
        Mat hist2, hist21;

        hist2 = Mat::zeros(2, dim, CV_32S);

        hist21 = Mat::zeros(2, dim, CV_32S);

        Mat img21 = img2(Range(0, img2.rows / 2), Range(0, img2.cols));
        Mat img22 = img2(Range(img2.rows / 2, img2.rows), Range(0, img2.cols));

        for (int i = 0; i < img21.rows; i++) {
            for (int j = 0; j < img21.cols; j++) {

                Vec3b rgb = img21.at<Vec3b>(i, j);

                float  r = (float)(rgb[0]) / (float)(rgb[0] + rgb[1] + rgb[2] + 1);
                float  g = (float)(rgb[1]) / (float)(rgb[0] + rgb[1] + rgb[2] + 1);


                int r1 = r * 16;
                int c1 = g * 16;


                hist2.at<int>(r1, c1) = hist2.at<int>(r1, c1) + 1;

            }


        }



        for (int i = 0; i < img22.rows; i++) {
            for (int j = 0; j < img22.cols; j++) {

                Vec3b rgb = img22.at<Vec3b>(i, j);

                float  r = (float)(rgb[0]) / (float)(rgb[0] + rgb[1] + rgb[2] + 1);
                float  g = (float)(rgb[1]) / (float)(rgb[0] + rgb[1] + rgb[2] + 1);


                int r1 = r * 16;
                int c1 = g * 16;


                hist21.at<int>(r1, c1) = hist21.at<int>(r1, c1) + 1;

            }


        }



        int sim1 = 0;

        for (int i = 0; i < hist.rows; i++) {
            for (int j = 0; j < hist.cols; j++) {

                if (hist.at<int>(i, j) < hist2.at<int>(i, j)) {


                    sim1 = sim1 + hist.at<int>(i, j);
                }
                else {

                    sim1 = sim1 + hist2.at<int>(i, j);
                }


            }


        }

        int sim2 = 0;

        for (int i = 0; i < hist1.rows; i++) {
            for (int j = 0; j < hist1.cols; j++) {

                if (hist1.at<int>(i, j) < hist21.at<int>(i, j)) {


                    sim2 = sim2 + hist1.at<int>(i, j);
                }
                else {

                    sim2 = sim2 + hist21.at<int>(i, j);
                }


            }


        }


        int total = sim1 + sim2;

        cout << total << endl;

        if (m.size() < n) {
            m.insert({ total,img2 });

        }
        else if (m.begin()->first < total) {
            m.erase(m.begin()->first);
            m.insert({ total,img2 });
        }


    }



    for (auto i : m) {

        imshow("img", i.second);

        for (;;) {
            char key = waitKey();
            if (key == 'q') {
                break;
            }
        }

    }


}


//Uses the gradient magnitude as the texture feature ad rg chromacity as the colour feature
void texture_match(string im, int n) {


    map<float, Mat> m;


    vector<cv::String> fn;
    glob("C:\\Users\\karth\\Desktop\\olympus\\*.jpg", fn, false);
    vector<Mat> images;
    size_t count = fn.size();
    cout << "total images: " << count << endl;
    for (size_t i = 0; i < count; i++)
        images.push_back(imread(fn[i]));


    Mat img = cv::imread(im);

    const int Hsize = 16;

    cv::Mat hist, hist1;

    int dim[2] = { Hsize,Hsize };

    hist = cv::Mat::zeros(2, dim, CV_32S);

    hist1 = cv::Mat::zeros(1, 16, CV_32S);


    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {

            Vec3b rgb = img.at<Vec3b>(i, j);


            float  r = (float)(rgb[0]) / (float)(rgb[0] + rgb[1] + rgb[2] + 1);
            float  g = (float)(rgb[1]) / (float)(rgb[0] + rgb[1] + rgb[2] + 1);


            int r1 = r * 16;
            int c1 = g * 16;

            int val = hist.at<int>(r1, c1);
            hist.at<int>(r1, c1) = val + 1;

        }
    }


    //gradient magnitude
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            
            Vec3b p = img.at<Vec3b>(i, j);
            int avg = (p[0] + p[1] + p[2]) / 3;
            int position = avg / 16;
            
            int val = hist1.at<int>(position);
            hist1.at<int>(position) = val + 1;

        }
    }


    for (Mat img2 : images) {


        cv::Mat hist2, hist21;

        hist2 = cv::Mat::zeros(2, dim, CV_32S);

        hist21 = cv::Mat::zeros(1, 16, CV_32S);

        for (int i = 0; i < img2.rows; i++) {
            for (int j = 0; j < img2.cols; j++) {

                Vec3b rgb = img2.at<Vec3b>(i, j);


                float  r = (float)(rgb[0]) / (float)(rgb[0] + rgb[1] + rgb[2] + 1);
                float  g = (float)(rgb[1]) / (float)(rgb[0] + rgb[1] + rgb[2] + 1);


                int r1 = r * 16;
                int c1 = g * 16;

                int val = hist2.at<int>(r1, c1);
                hist2.at<int>(r1, c1) = val + 1;

            }
        }

        for (int i = 0; i < img2.rows; i++) {
            for (int j = 0; j < img2.cols; j++) {

                Vec3b p = img2.at<Vec3b>(i, j);
                int avg = (p[0] + p[1] + p[2])/3;
                int position = avg / 16;

                int val = hist21.at<int>(position);
                hist21.at<int>(position) = val + 1;

            }
        }


        float s1 = 0;


        for (int i = 0; i < hist.rows; i++) {
            for (int j = 0; j < hist.cols; j++) {


                if (hist.at<int>(i, j) < hist2.at<int>(i, j)) {


                    s1 = s1 + hist.at<int>(i, j);
                }
                else {

                    s1 = s1 + hist2.at<int>(i, j);
                }

            }


        }

        int s2 = 0;

        for (int i = 0; i < hist1.rows; i++) {


            if (hist1.at<int>(i) < hist21.at<int>(i)) {


                s2 = s2 + hist1.at<int>(i);
            }
            else {

                s2 = s2 + hist21.at<int>(i);
            }

        }

        int total = s1 + s2;

        cout << total << endl;

        if (m.size() < n) {
            m.insert({ total,img2 });

        }
        else if (m.begin()->first < total) {
            m.erase(m.begin()->first);
            m.insert({ total,img2 });
        }
    
    }

    for (auto i : m) {

        imshow("img", i.second);

        for (;;) {
            char key = waitKey();
            if (key == 'q') {
                break;
            }
        }

    }

}

//custom function that extracts a square from the center and uses rb chromacity on that square as the feature
void custom(string im, int n) {


    Mat img = cv::imread(im);


    map<float, Mat> m;


    vector<cv::String> fn;
    glob("C:\\Users\\karth\\Desktop\\olympus\\*.jpg", fn, false);
    vector<Mat> images;
    size_t count = fn.size();
    cout << "total images: " << count << endl;
    for (size_t i = 0; i < count; i++)
        images.push_back(imread(fn[i]));

    const int Hsize = 16;

    cv::Mat hist;

    int dim[2] = { Hsize,Hsize };

    hist = cv::Mat::zeros(2, dim, CV_32S);


    int x = ((img.rows) / 2) - 40;
    int y = ((img.cols) / 2) - 40;


    vector<float> v1;

    int c = 0;


    for (int i = x; i < x + 81; i++) {
        for (int j = y; j < y + 81; j++) {

            c++;
            Vec3b rgb = img.at<Vec3b>(i, j);


            float  r = (float)(rgb[0]) / (float)(rgb[0] + rgb[1] + rgb[2] + 1);
            float  g = (float)(rgb[1]) / (float)(rgb[0] + rgb[1] + rgb[2] + 1);


            int r1 = r * 16;
            int c1 = g * 16;

            int val = hist.at<int>(r1, c1);
            hist.at<int>(r1, c1) = val + 1;

        }

    }




    for (Mat img2 : images) {
        Mat hist2;

        hist2 = Mat::zeros(2, dim, CV_32S);
        int x2 = ((img2.rows) / 2) - 40;
        int y2 = ((img2.cols) / 2) - 40;

        for (int i = x2; i < x2 + 81; i++) {
            for (int j = y2; j < y2 + 81; j++) {

                Vec3b rgb = img2.at<Vec3b>(i, j);

                float  r = (float)(rgb[0]) / (float)(rgb[0] + rgb[1] + rgb[2] + 1);
                float  g = (float)(rgb[1]) / (float)(rgb[0] + rgb[1] + rgb[2] + 1);


                int r1 = r * 16;
                int c1 = g * 16;

                hist2.at<int>(r1, c1) = hist2.at<int>(r1, c1) + 1;

            }

        }

        int sim = 0;

        for (int i = 0; i < hist.rows; i++) {
            for (int j = 0; j < hist.cols; j++) {



                float s1 = hist.at<int>(i, j) - hist2.at<int>(i, j);
                float s2 = s1 * s1;
                sim = sim + s2;




            }

        }

        sim = sim / 19683;


        if (m.size() < n) {
            m.insert({ sim,img2 });

        }
        else if (m.begin()->first > sim) {

            m.erase(std::prev(m.end()));
            m.insert({ sim,img2 });
            //m.erase(m.begin()->first);
            //m.insert({ sim,img2 });
        }

    }


    for (auto i : m) {

        imshow("img", i.second);

        for (;;) {
            char key = waitKey();
            if (key == 'q') {
                break;
            }
        }

    }

}





