// OpenCVTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <opencv2/imgproc.hpp>
# include<opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/opencv.hpp>
#include "csv_util.h"
#include "filter.h"


using namespace cv;

int threshold(cv::Mat& src, cv::Mat& dst);
int threshold2(cv::Mat& src, cv::Mat& dst);
String knn3(std::vector<char*> names, std::vector<std::vector<float>> features, std::vector<float> fVec);



// main function where an image is taken from the camera and is then processed.
int main(int argc, char* argv[]) {
    cv::VideoCapture* capdev;

    // open the video device
    capdev = new cv::VideoCapture(1);
    if (!capdev->isOpened()) {
        printf("Unable to open video device\n");
        return(-1);
    }

    // get some properties of the image
    cv::Size refS((int)capdev->get(cv::CAP_PROP_FRAME_WIDTH),
        (int)capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
    printf("Expected size: %d %d\n", refS.width, refS.height);

    //cv::namedWindow("Video", 1); // identifies a window
    cv::Mat frame;


    for (;;) {




        *capdev >> frame; // get a new frame from the camera, treat as a stream

        if (frame.empty()) {
            printf("frame is empty\n");
            break;
        }

        //Mat m1 = Mat::zeros(frame.rows, frame.cols, CV_64F);
        //threshold(frame, frame);
        ////erode(frame, frame,1);

        //imshow("1",frame);

        //erode(frame, frame, Mat());
        //dilate(frame, frame, Mat());


        //dilate(frame, frame, Mat());
        //erode(frame, frame, Mat());

        //imshow("2", frame);


        cvtColor(frame, frame, COLOR_BGR2GRAY);
        threshold2(frame, frame);

        erode(frame, frame, Mat());
        dilate(frame, frame, Mat());


        dilate(frame, frame, Mat());
        erode(frame, frame, Mat());

        //imshow("2", frame);

       

        //threshold(frame,frame);

        

        Mat out;


        Mat stats, centroids;
        int nLabels = connectedComponentsWithStats(frame, out,stats,centroids);

   
        
        
        
        Mat comps;
        normalize(out, comps, 0, 255, NORM_MINMAX, CV_8U);
        Mat seeMyLabels;
        
        //imwrite("C:\\Users\\karth\\Downloads\\project3.bmp", comps);

        //applyColorMap(comps, seeMyLabels, COLORMAP_JET);

        //imshow("3", seeMyLabels);

        Moments m = moments(comps);
        double huMoments[7];
        HuMoments(m, huMoments);
        for (int i = 0; i < 7; i++) {
                huMoments[i] = -1 * copysign(1.0, huMoments[i]) * log10(abs(huMoments[i]));
        }


        //for (int i = 0; i < 7; i++) {
        //    //std::cout << i << ":" << huMoments[i] << "|" << "\n";
        //}
        
        std::vector<std::vector<cv::Point> > contours;
        findContours(comps.clone(), contours, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
        sort(contours.begin(), contours.end(), [](const  std::vector<Point>& c1, const  std::vector<Point>& c2) {
            return contourArea(c1, false) < contourArea(c2, false);
            });
        float ar = 0;
        float area = 0;

        if (!contours.empty()) {
            RotatedRect rec = minAreaRect(contours.back());
            Point2f vertices[4];
            rec.points(vertices);
            for (int i = 0; i < 4; i++)
                line(comps, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0), 2);
            Rect brect = rec.boundingRect();
            rectangle(comps, brect, Scalar(255, 0, 0), 2);


            ar = brect.width / brect.height;
            area = brect.area();
        }

        float maxArea = 0;
        for (int i = 1; i < nLabels; ++i) { //label  0 is the background
            if ((stats.at<int>(i, CC_STAT_AREA)) > maxArea) {
                maxArea = stats.at<int>(i, CC_STAT_AREA);
                
            }
        }

        float areaPercent = maxArea / area;

        String name = "data";

        std::vector<double> dataVector(huMoments, huMoments + sizeof huMoments / sizeof huMoments[0]);
        std::vector<float> fVec(dataVector.begin(), dataVector.end());
        fVec.push_back(ar);
        fVec.push_back(areaPercent);

        std::vector<char*> names;
        std::vector<std::vector<float>> features;

        read_image_data_csv(&name[0], names, features);

        String closestI = names[0];
        float closestD = norm(fVec, features[0], NORM_L2);


        for (int i = 0; i < features.size(); i++) {
            if (norm(fVec, features[i], NORM_L2) < closestD) {
                closestD = norm(fVec, features[i], NORM_L2);
                closestI = names[i];

            }
        }


        //closestI = knn3(names,features,fVec);
        std::cout << closestI << "\n";

        

        cv::putText(comps, 
            closestI, 
            cv::Point(comps.cols/2, comps.rows / 2), //top-left position
            cv::FONT_HERSHEY_DUPLEX,
            0.5,
            CV_RGB(0, 0, 255), //font color
            2);

        imshow("out", comps);

        

        // see if there is a waiting keystroke
        char key = cv::waitKey(10);

        if (key == 'w') {
            imwrite("C:\\Users\\karth\\Downloads\\project3.bmp", comps);
        }
        else if (key == 't') {
            String lab;
            std::cin >> lab;
            

            char* lab1 = &lab[0];
            
            append_image_data_csv(&name[0], lab1, fVec);
        }
        else if (key == 'q') {
            break;
        }
        else if (key == 'c') {
            std::cout << closestI;
        }
 

    }

    delete capdev;
    return(0);
}
//Runs a knn with 3 neighbours.
String knn3(std::vector<char*> names, std::vector<std::vector<float>> features, std::vector<float> fVec) {

    if (names.size() < 1) {
        return NULL;
    }

    std::vector<float> dis;
    for (int i = 0; i < features.size(); i++) {
        dis.push_back(norm(fVec, features[i], NORM_L2));
    }



    std::vector<float> copy(dis);

    sort(copy.begin(), copy.end());


    String neighbours[3];

    for (int i = 0; i < fVec.size(); i++) {
        if (dis[i] == copy[0]) {
            neighbours[0] = names[i];
        }
        else if (dis[i] == copy[1]) {
            neighbours[1] = names[i];
        }
        else if (dis[i] == copy[2]) {
            neighbours[2] = names[1];
        }
    
    }

    std::string str0(neighbours[0]);
    std::string str1(neighbours[1]);
    std::string str2(neighbours[2]);

    if (str0 == str1) {
        return str0;
    }
    else if (str1 == str2) {
        return str1;
    }
    else if (str0 == str2) {
        return str0;
    }
    else {
        return str0;
    }

}



//Thresholds the image into black and white.
int threshold(cv::Mat& src, cv::Mat& dst) {

    for (int r = 0; r < src.rows; ++r) {
        for (int c = 0; c < src.cols; ++c) {
            //std::cout << "Pixel at position (x, y) : (" << c << ", " << r << ") =" << src.at<Vec3b>(r, c) << std::endl;
            int c0 = src.at<Vec3b>(r, c)[0];
            int c1 = src.at<Vec3b>(r, c)[1];
            int c2 = src.at<Vec3b>(r, c)[2];

            int avg = (c0 + c1 + c2) / 3;

            Vec3b n = Vec3b(avg, avg, avg);

            if (avg < 75) {
             n = Vec3b(255, 255, 255);
            }
            else {
              n=  Vec3b(0, 0, 0);
            }
            dst.at<Vec3b>(r, c) = n;


        }
    }



    return(0);
}


//Thresholds a single channel image.
int threshold2(cv::Mat& src, cv::Mat& dst) {

    for (int r = 0; r < src.rows; ++r) {
        for (int c = 0; c < src.cols; ++c) {
            


            int avg =  src.at<uchar>(r, c);

            uchar n = avg;

            if (avg < 128) {
                n = 255;
            }
            else {
                n = 0;
            }
            dst.at<uchar>(r, c) = n;





        }
    }



    return(0);
}