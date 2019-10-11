//
// Created by Cutie on 2018/7/23.
//

#ifndef OPENCVTEST_PROCESS_HELPER_H
#define OPENCVTEST_PROCESS_HELPER_H

#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;

namespace MyImageCrop {

    class process_helper {
    public:
        int resizeThreshold = 500;

        process_helper(cv::Mat& bitmap);
        virtual ~process_helper();
        cv::Mat doCrop();
        vector<cv::Point> findCropArea();
    private:
        cv::Mat srcBmp;
        float resizeScale = 1.0f;
        cv::Mat resizeImage();
    };
}

#endif //OPENCVTEST_PROCESS_HELPER_H
