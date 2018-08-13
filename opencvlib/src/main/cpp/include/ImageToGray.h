//
// Created by Cutie on 2018/7/19.
//

#ifndef OPENCVTEST_IMAGETOGRAY_H
#define OPENCVTEST_IMAGETOGRAY_H

#include <opencv2/opencv.hpp>

namespace MyGrayProcess {

    class ImageToGray {

    public:
        ImageToGray(cv::Mat& bitmap);
        virtual ~ImageToGray();
        cv::Mat processToGray();

    private:
        cv::Mat srcBmp;
    };
}

#endif //OPENCVTEST_IMAGETOGRAY_H
