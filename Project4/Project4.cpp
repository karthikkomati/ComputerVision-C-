// Project4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iostream>
#include <opencv2/imgproc.hpp>
# include<opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/opencv.hpp>

#include "csv_util.h"
#include "filter.h"
#include <opencv2/calib3d.hpp>
#include "opencv2/imgproc/imgproc_c.h"

using namespace cv;


//program for task 1 to 3 of project 4
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

            Vec3f point (i, -j, 0);
            //std::cout << point << '\n';
            point_set.push_back(cv::Vec3f(point));

        }
    }


    for (Vec3f p : point_set)
        //std::cout << p << '\n';

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

        //std::cout << found << "\n";
        if (found) {
            //std::cout << "M = " << std::endl << " " << out<< std::endl << std::endl;
            //std::cout << "\n";
            //std::cout << "------------------------------------------------------------------";
        }

        //draws the corners on the frame
        drawChessboardCorners(frame, size, out, found);
               




        imshow("Video", frame);
        char c = cv::waitKey(1);
        if (c == 'q') {
            break;
        }

        //stores the corners and 3d points
        else if (c == 's') {
            corner_list.push_back(std::vector<cv::Point2f> (out));
            point_list.push_back(point_set);
            std::cout << point_list.size() << "\n";
        }
        else if (c == 'c') {
            //calibrates if there are alteast 5 stored
            if (corner_list.size() < 5) {
                std::cout << "Not enough calibration frames \n";
            }
            else {
            
                float data[9] = { 1, 0, frame.cols/2, 0, 1, frame.rows / 2, 0, 0, 1 };
                cv::Mat cam_mat = cv::Mat(3, 3, CV_64FC1, data);
                Mat rvecs, tvecs, stdDeviationsIntrinsics, stdDeviationsExtrinsics, perViewErrors;
                std::vector<double> distCoeff;
                double error = calibrateCamera(point_list,corner_list, size,cam_mat, distCoeff, rvecs, tvecs, stdDeviationsIntrinsics, stdDeviationsExtrinsics, perViewErrors);
                std::cout << error <<" \n";

                std::cout << cam_mat;

                std::cout << "\n -----------------------";

                //std::cout << distCoeff;
                for (auto i : distCoeff)
                    std::cout << i << ' ';

                //FILE* fp;
                //fp = fopen("example.txt", "w+");

                //fprintf(fp, "%d %d %d\n", cam_mat.at<double>(0,0), cam_mat.at<double>(0, 1), cam_mat.at<double>(0, 2));
                //fprintf(fp, "%d %d %d\n", cam_mat.at<double>(1, 0), cam_mat.at<double>(1, 1), cam_mat.at<double>(1, 2));
                //fprintf(fp, "%d %d %d\n", cam_mat.at<double>(2, 0), cam_mat.at<double>(2, 1), cam_mat.at<double>(2, 2));

                //fclose(fp);

                //std::cout << cmat;
            }
        }

        else if (c == 'e') {

            imwrite("C:\\Users\\karth\\Downloads\\Project4Screenshot.bmp", frame);

        }

    }


    
}

