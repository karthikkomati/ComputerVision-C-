// OpenCVTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <opencv2/imgproc.hpp>
# include<opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/opencv.hpp>

#include "filter.h"

using namespace cv;

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

    cv::namedWindow("Video", 1); // identifies a window
    cv::Mat frame;
    bool g = false;
    bool ag = false;

    for (;;) {        




        *capdev >> frame; // get a new frame from the camera, treat as a stream
        if (frame.empty()) {
            printf("frame is empty\n");
            break;
        }

        if (g) {
            Mat grayscale;
            cvtColor(frame, frame, COLOR_BGR2GRAY);

            imshow("Video", frame);
        } 
        if (ag) {
            Mat grayscale1;
            greyscale(frame, frame);
            imshow("Video", frame);
        }

        else { 
            //greyscale(frame, frame);
            Mat g;
            //blur5x5(frame,frame);
            cv::imshow("Video", frame); 
            
        }
        

        // see if there is a waiting keystroke
        char key = cv::waitKey(10);
        if (key == 'q') {
            break;
        }
        else if (key == 'g') {
            if (g) {
                g = false;
            }
            else {
                g = true;
                ag = false;
            }

        }
        else if (key == 'h') {

            if (ag) {
                ag = false;
            }
            else {
                ag = true;
                g = false;
            }

        }
        else if (key == 's') {
            imwrite("C:\\Users\\karth\\Downloads\\screenshot.jpg", frame);
        }

        
    }

    delete capdev;
    return(0);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
