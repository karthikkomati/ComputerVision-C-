#include <iostream>
#include <iostream>
#include <opencv2/imgproc.hpp>
# include<opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/opencv.hpp>

//#include "csv_util.h"
#include "filter.h"
#include <opencv2/calib3d.hpp>
#include "opencv2/imgproc/imgproc_c.h"

using namespace cv;


//Program to for task 4 to 7

int main(int argc, char* argv[]) {
    cv::VideoCapture* capdev;

    // open the video device
    capdev = new cv::VideoCapture(0);
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


    std::vector<cv::Vec3f> point_set;
    std::vector<std::vector<cv::Vec3f> > point_list;
    std::vector<std::vector<cv::Point2f> > corner_list;


    for (int j = 0; j < 6; j++) {

        for (int i = 0; i < 9; i++) {

            Vec3f point(i, -j, 0);
            //std::cout << point << '\n';
            point_set.push_back(cv::Vec3f(point));

        }
    }

    bool calibration = false;
    bool v = false;
    bool h = false;

   /* for (Vec3f p : point_set)
        std::cout << p << '\n';*/


    float data[9] = { 1, 0, frame.cols / 2, 0, 1, frame.rows / 2, 0, 0, 1 };
    cv::Mat cam_mat = cv::Mat(3, 3, CV_64FC1, data);
    Mat  stdDeviationsIntrinsics, stdDeviationsExtrinsics, perViewErrors, rvec1, tvec1;
    std::vector<double> distCoeff;
    std::vector<cv::Mat> rvecs, tvecs;

        for (;;) {




            *capdev >> frame; // get a new frame from the camera, treat as a stream

            if (frame.empty()) {
                printf("frame is empty\n");
                break;
            }

            CvSize size = cvSize(9, 6);
            Mat out;


            //finds the chess board corners
            bool found = findChessboardCorners(frame, size, out);

            if (found) {
                //std::cout << "M = " << std::endl << " " << out<< std::endl << std::endl;
                //std::cout << "\n";
                //std::cout << "------------------------------------------------------------------";
            }


            //drawChessboardCorners(frame, size, out, found);


            char c = cv::waitKey(1);
            if (c == 'q') {
                break;
            }
            //stores the corners and the 3d points of the current frame
            else if (c == 's') {
                corner_list.push_back(std::vector<cv::Point2f>(out));
                point_list.push_back(point_set);
                std::cout << point_list.size() << "\n";
            }
            //calibrates the camera if there are at least 5 images stored
            else if (c == 'c') {
                if (corner_list.size() < 5) {
                    std::cout << "Not enough calibration frames \n";
                }
                else {

                    h = false;

                    double error = calibrateCamera(point_list, corner_list, size, cam_mat, distCoeff, rvecs, tvecs, stdDeviationsIntrinsics, stdDeviationsExtrinsics, perViewErrors);
                    //std::cout << error << " \n";
                    calibration = true;
                    //for (auto i : distCoeff)
                    //    std::cout << i << ' ';
                }
            }
            else if (c == 'h') {

                if (h) {
                    h = false;
                }
                else {
                    h = true;
                }

            }

            else if (c == 'e') {

                imwrite("C:\\Users\\karth\\Downloads\\Project4Screenshot.bmp", frame);

            }


            if (h) {
            


                Mat gray;

                //Calculates and draws the harris corners.
                cvtColor(frame, gray, CV_BGR2GRAY);
                Mat dst, dst_norm, dst_norm_scaled;
                dst = Mat::zeros(frame.size(), CV_32FC1);

                cornerHarris(gray, dst, 7, 5, 0.05, BORDER_DEFAULT);


                normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
                convertScaleAbs(dst_norm, dst_norm_scaled);

                for (int j = 0; j < dst_norm.rows; j++)
                {
                    for (int i = 0; i < dst_norm.cols; i++)
                    {
                        if ((int)dst_norm.at<float>(j, i) > 200)
                        {
                            circle(frame, Point(i, j), 5, Scalar(0), 2, 8, 0);
                        }
                    }
                }


            
            }

                if (calibration && found  &&!h) {



                    //task 4 and 5 (calculates current position and project axis)
                    solvePnP(point_set, out, cam_mat, distCoeff, rvec1, tvec1);
                    std::cout << rvec1 <<"rotation \n";
                    std::cout << tvec1 << "translation \n";
                    std::vector<Point2f>  out1;

                    projectPoints(point_set,rvec1,tvec1,cam_mat,distCoeff,out1);

                    std::cout << "____________________________________________________\n";


                    //std::cout << out1.size();

                    //draws the axis
                    drawFrameAxes(frame, cam_mat, distCoeff, rvec1, tvec1, 5, 1);

                    //for (int i = 0; i < out1.size()-1; i++) {
                    //    //std::cout << i << ' ';
                    //    for (int j = i + 1; j < out1.size(); j++) {
                    //        line(frame, out1[i], out1[j], Scalar(255, 0, 0));
                    //    }
                    //    
                    //}

                    //draws the object
                    line(frame, out1[1], out1[2], Scalar(255, 0, 0));
                    line(frame, out1[2], out1[3], Scalar(255, 0, 0));
                    line(frame, out1[3], out1[4], Scalar(255, 0, 0));
                    line(frame, out1[4], out1[1], Scalar(255, 0, 0));

                    line(frame, out1[5], out1[6], Scalar(255, 0, 0));
                    line(frame, out1[6], out1[7], Scalar(255, 0, 0));
                    line(frame, out1[7], out1[8], Scalar(255, 0, 0));
                    line(frame, out1[8], out1[5], Scalar(255, 0, 0));

                    line(frame, out1[1], out1[5], Scalar(255, 0, 0));
                    line(frame, out1[2], out1[6], Scalar(255, 0, 0));
                    line(frame, out1[3], out1[7], Scalar(255, 0, 0));
                    line(frame, out1[4], out1[8], Scalar(255, 0, 0));


                    line(frame, out1[1], out1[15], Scalar(255, 0, 0));
                    line(frame, out1[2], out1[16], Scalar(255, 0, 0));
                    line(frame, out1[3], out1[17], Scalar(255, 0, 0));
                    line(frame, out1[4], out1[18], Scalar(255, 0, 0));



                



                }


            imshow("Video", frame);


        }

}