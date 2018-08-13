//
// Created by Cutie on 2018/7/19.
//

#include <ImageToGray.h>

using namespace MyGrayProcess;
using namespace cv;

ImageToGray::ImageToGray(cv::Mat &bitmap) {
    srcBmp = bitmap;
}

ImageToGray::~ImageToGray() {
    // Do Nothing
}

cv::Mat ImageToGray::processToGray() {
    Mat grayMat;
    cvtColor(srcBmp, grayMat, COLOR_BGR2GRAY);
    return grayMat;
}