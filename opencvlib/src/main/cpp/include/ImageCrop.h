//
// Created by Cutie on 2018/7/23.
//

#ifndef OPENCVTEST_IMAGECROP_H
#define OPENCVTEST_IMAGECROP_H

#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;

namespace MyImageCrop {

    class ImageCrop {
    public:
        int resizeThreshold = 500;
        int areaThreshold = 20000; // 面积阀值

        ImageCrop(cv::Mat& bitmap);
        virtual ~ImageCrop();
        cv::Mat doCrop();
        vector<cv::Point> findCropArea();
    private:
        cv::Mat srcBmp;
        float resizeScale = 1.0f;
        cv::Mat resizeImage();

        double getAngle(cv::Point pt1, cv::Point pt2, cv::Point pt0);
        double getSpacePointToPoint(cv::Point p1, cv::Point p2);
        int findLargestSquare(const vector<vector<cv::Point> >& squares, vector<cv::Point>& biggest_square);
        cv::Point2f computeIntersect(cv::Vec4i a, cv::Vec4i b);
        vector<cv::Point> sortPointClockwise(std::vector<cv::Point> vector);
        long long pointSideLine(cv::Point& lineP1, cv::Point& lineP2, cv::Point& point);
    };
}

#endif //OPENCVTEST_IMAGECROP_H
