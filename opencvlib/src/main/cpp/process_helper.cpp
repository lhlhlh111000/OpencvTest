//
// Created by Cutie on 2018/7/23.
//

#include <process_helper.h>
#include "old_process.h"
#include "new_process.h"

using namespace MyImageCrop;
using namespace cv;

process_helper::process_helper(Mat &bitmap) {
    srcBmp = bitmap;
}

process_helper::~process_helper() {
// Do Nothing
}

Mat process_helper::doCrop() {
    Mat grayMat, filtered, edges, dilated_edges;
    Mat resizeMat = resizeImage();

    vector<Point> points = doOldFind(resizeMat);
    if(!points.size()) {
        points = doFind(resizeMat);
    }
    if(points.size()) {
        for(int i=0; i<points.size(); i++) {
            points[i].x *= resizeScale; // 坐标还原
            points[i].y *= resizeScale;
        }
    }
    //绘制出四条边
    for (int i = 0; i < points.size(); i++) {
        line(srcBmp, points[i], points[(i + 1) % points.size()], Scalar(0, 0, 255), 5);
    }

    return srcBmp;
}

vector<Point> process_helper::findCropArea() {
    Mat grayMat, filtered, edges, dilated_edges;
    Mat resizeMat = resizeImage();
    vector<Point> points = doOldFind(resizeMat);
    if(!points.size()) {
        points = doFind(resizeMat);
    }
    if(points.size()) {
        for(int i=0; i<points.size(); i++) {
            points[i].x *= resizeScale; // 坐标还原
            points[i].y *= resizeScale;
        }
    }
    return points;
}

cv::Mat process_helper::resizeImage() {
    int width = srcBmp.cols;
    int height = srcBmp.rows;
    int maxSize = width > height? width : height;
    if (maxSize > resizeThreshold) {
        resizeScale = 1.0f * maxSize / resizeThreshold;
        width = static_cast<int>(width / resizeScale);
        height = static_cast<int>(height / resizeScale);
        Size size(width, height);
        Mat resizedBitmap(size, CV_8UC3);
        resize(srcBmp, resizedBitmap, size);
        return resizedBitmap;
    }
    return srcBmp;
}