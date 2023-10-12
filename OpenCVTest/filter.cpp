#include <iostream>
#include <opencv2/imgproc.hpp>
# include<opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/opencv.hpp>
#include <filter.h>

#include <stdio.h>
#include <sys/time.h>
#include <math.h>

double getTime() {
    struct timeval cur;

    gettimeofday(&cur, NULL);
    return(cur.tv_sec + cur.tv_usec / 1000000.0);
}

using namespace cv;




int greyscale(cv::Mat& src, cv::Mat& dst) {

    for (int r = 0; r < src.rows; ++r) {
        for (int c = 0; c < src.cols; ++c) {
            //std::cout << "Pixel at position (x, y) : (" << c << ", " << r << ") =" << src.at<Vec3b>(r, c) << std::endl;
            int c0 = src.at<Vec3b>(r, c)[0];
            int c1 = src.at<Vec3b>(r, c)[1];
            int c2 = src.at<Vec3b>(r, c)[2];

            int avg = (c0 + c1 + c2) / 3;



            Vec3b n = Vec3b(avg,avg,avg);


            dst.at<Vec3b>(r, c) = n;

        }
    }


	
    return(0);
}



int blur5x5(cv::Mat& src, cv::Mat& dst) {


    Mat in = Mat(src);

    for (int r = 0; r < src.rows; ++r) {
        for (int c = 0; c < src.cols; ++c) {
            
            Vec3b r1, r2, p, r3, r4;


            if (r - 1 < 0) {
                r1 = (1, 1, 1);
            }
            else {
                r1 = src.at<Vec3b>(r - 1, c);
            }

            if (r - 2 < 0) {
                r2 = (1, 1, 1);
            }
            else {
                r2 = src.at<Vec3b>(r - 2, c);
            }

            if (r + 1 >= src.rows) {
                r3 = (1, 1, 1);
            }
            else {
                r3 = src.at<Vec3b>(r + 1, c);
            }

            if (r + 2 >= src.rows) {
                r4 = (1, 1, 1);
            }   
            else {
                r4 = src.at<Vec3b>(r + 2, c);
            }




            p = src.at<Vec3b>(r, c);

            //Vec3b x = r1 + (2 * r2) + (4 * p) + (2 * r3) + r4;
            Vec3b x1 = ((r1[0] + (2 * r2[0]) + (4 * p[0]) + (2 * r3[0]) + r4[0])/5, (r1[1] + (2 * r2[1]) + (4 * p[1]) + (2 * r3[1]) + r4[1])/5, (r1[2] + (2 * r2[2]) + (4 * p[2]) + (2 * r3[2]) + r4[2])/5);
            in.at<Vec3b>(r, c) = x1;
            
            //std::cout   << src.at<Vec3b>(r, c) << std::endl;

        }
    }


    for (int r = 0; r < in.rows; ++r) {
        for (int c = 0; c < in.cols; ++c) {

            Vec3b p1, c1, c2, c3, c4;



            if (c - 1 < 0) {
                c1 = (1, 1, 1);
            }
            else {
                c1 = in.at<Vec3b>(r, c - 1);
            }

            if (c - 2 < 0) {
                c2 = (1, 1, 1);
            }
            else {
                c2 = in.at<Vec3b>(r, c - 2);
            }

            if (c + 1 >= in.cols) {
                c3 = (1, 1, 1);
            }
            else {
                c3 = in.at<Vec3b>(r, c + 1);
            }

            if (c + 2 >= in.cols) {
                c4 = (1, 1, 1);
            }
            else {
                c4 = in.at<Vec3b>(r, c + 2);
            }



            p1 = in.at<Vec3b>(r, c);

            //Vec3b x = r1 + (2 * r2) + (4 * p) + (2 * r3) + r4;
            Vec3b x = ((c1[0] + (2 * c2[0]) + (4 * p1[0]) + (2 * c3[0]) + c4[0])/5, (c1[1] + (2 * c2[1]) + (4 * p1[1]) + (2 * c3[1]) + c4[1])/5, (c1[2] + (2 * c2[2]) + (4 * p1[2]) + (2 * c3[2]) + c4[2])/5);
            
            dst.at<Vec3b>(r, c) = x;
            convertScaleAbs(src, dst);
            //std::cout   << src.at<Vec3b>(r, c) << std::endl;

        }
    }




    return(0);

    
}



int sobelX3x3(cv::Mat& src, cv::Mat& dst) {



    Mat in = Mat(src);

    for (int r = 0; r < src.rows; ++r) {
        for (int c = 0; c < src.cols; ++c) {

            Vec3b r1, p, r3;


            if (r - 1 < 0) {
                r1 = (1, 1, 1);
            }
            else {
                r1 = src.at<Vec3b>(r - 1, c);
            }

            if (r + 1 >= src.rows) {
                r3 = (1, 1, 1);
            }
            else {
                r3 = src.at<Vec3b>(r + 1, c);
            }




            p = src.at<Vec3b>(r, c);

            //Vec3b x = r1 + (2 * r2) + (4 * p) + (2 * r3) + r4;
            Vec3b x1 = ((r1[0] +  (2 * p[0]) + (1 * r3[0]))/4, (r1[1]   + (2 * p[1]) + (1 * r3[1]))/4 , (r1[2] + (2 * p[2]) + (1 * r3[2]) )/4);
            dst.at<Vec3b>(r, c) = x1;

            //std::cout   << src.at<Vec3b>(r, c) << std::endl;

        }
    }


    for (int r = 0; r < in.rows; ++r) {
        for (int c = 0; c < in.cols; ++c) {

            Vec3b p1, c1, c3;



            if (c - 1 < 0) {
                c1 = (1, 1, 1);
            }
            else {
                c1 = in.at<Vec3b>(r, c - 1);
            }

            if (c + 1 >= in.cols) {
                c3 = (1, 1, 1);
            }
            else {
                c3 = in.at<Vec3b>(r, c + 1);
            }




            p1 = in.at<Vec3b>(r, c);

            //Vec3b x = r1 + (2 * r2) + (4 * p) + (2 * r3) + r4;
            Vec3b x1 = ((-c1[0] +(0 * p1[0]) + (1 * c3[0])), (-c1[1]  +(0 * p1[1]) + (1 * c3[1])), (-c1[2] + (0 * p1[2]) + (1 * c3[2])));
            in.at<Vec3b>(r, c) = x1;
            //std::cout   << src.at<Vec3b>(r, c) << std::endl;

        }
    }




    return(0);


}
int sobelY3x3(cv::Mat& src, cv::Mat& dst) {




    Mat in = Mat(src);

    for (int r = 0; r < src.rows; ++r) {
        for (int c = 0; c < src.cols; ++c) {

            Vec3b r1, p, r3;


            if (r - 1 < 0) {
                r1 = (1, 1, 1);
            }
            else {
                r1 = src.at<Vec3b>(r - 1, c);
            }

            if (r + 1 >= src.rows) {
                r3 = (1, 1, 1);
            }
            else {
                r3 = src.at<Vec3b>(r + 1, c);
            }




            p = src.at<Vec3b>(r, c);

            //Vec3b x = r1 + (2 * r2) + (4 * p) + (2 * r3) + r4;
            Vec3b x1 = ((r1[0] + (0 * p[0]) + (-1 * r3[0])), (r1[1] + (0 * p[1]) + (-1 * r3[1])), (r1[2] + (0 * p[2]) + (-1 * r3[2])));
            dst.at<Vec3b>(r, c) = x1;

            //std::cout   << src.at<Vec3b>(r, c) << std::endl;

        }
    }


    for (int r = 0; r < in.rows; ++r) {
        for (int c = 0; c < in.cols; ++c) {

            Vec3b p1, c1, c3;



            if (c - 1 < 0) {
                c1 = (1, 1, 1);
            }
            else {
                c1 = in.at<Vec3b>(r, c - 1);
            }

            if (c + 1 >= in.cols) {
                c3 = (1, 1, 1);
            }
            else {
                c3 = in.at<Vec3b>(r, c + 1);
            }




            p1 = in.at<Vec3b>(r, c);

            //Vec3b x = r1 + (2 * r2) + (4 * p) + (2 * r3) + r4;
            Vec3b x1 = ((c1[0] + (2 * p1[0]) + (1 * c3[0])), (c1[1] + (2 * p1[1]) + (1 * c3[1])), (c1[2] + (2 * p1[2]) + (1 * c3[2])));
            in.at<Vec3b>(r, c) = x1;
            //std::cout   << src.at<Vec3b>(r, c) << std::endl;

        }
    }



    return(0);
}


int magnitude(cv::Mat& sx, cv::Mat& sy, cv::Mat& dst) {

    return(0);
}