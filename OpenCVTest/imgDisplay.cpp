#include <iostream>
#include <opencv2/imgproc.hpp>
# include<opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
using namespace cv;

int main(int argc, char* argv[]) {


    Mat img = cv::imread("C:\\Users\\karth\\Downloads\\opencvtest.jpg");
    namedWindow("image", WINDOW_AUTOSIZE);
    imshow("image", img);
    for (;;) {
        char key = waitKey();
        if (key == 'q') {
            break;
        }
    }

    

   
    return(0);
}