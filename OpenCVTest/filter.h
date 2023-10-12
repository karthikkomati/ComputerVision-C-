#pragma once
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/opencv.hpp>


int greyscale(cv::Mat& src, cv::Mat& dst);

int blur5x5(cv::Mat& src, cv::Mat& dst);


int sobelX3x3(cv::Mat& src, cv::Mat& dst);
int sobelY3x3(cv::Mat& src, cv::Mat& dst);


int magnitude(cv::Mat& sx, cv::Mat& sy, cv::Mat& dst);;