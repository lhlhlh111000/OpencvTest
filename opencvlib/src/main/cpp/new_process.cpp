//
//  new_process.cpp
//  square_scan
//
//  Created by Cutie on 2019/10/8.
//  Copyright © 2019 cqt. All rights reserved.
//

#include "new_process.h"


vector<Point> doFind(Mat &srcImg) {
    Mat img_to_process;
    cvtColor(srcImg, img_to_process, COLOR_BGR2HSV);
    vector<Mat> channels_hsv;
    split(img_to_process, channels_hsv);
    img_to_process = channels_hsv[1];
    GaussianBlur(img_to_process, img_to_process, Size(9,9), 2, 2);
    img_to_process.convertTo(img_to_process, CV_32FC1, 0.5f, 0.5f);
    
    Mat sobx, soby;
    Sobel(img_to_process, sobx, -1, 1, 0);
    Sobel(img_to_process, soby, -1, 0, 1);
    
    sobx = sobx.mul(sobx);
    soby = soby.mul(soby);
    Mat grad_abs_val_approx;
    pow(sobx + soby, 0.5, grad_abs_val_approx);
    
    Mat filtered;
    GaussianBlur(grad_abs_val_approx, filtered, Size(9, 9), 2, 2);
    
    Scalar mean, stdev;
    meanStdDev(filtered, mean, stdev);
    threshold(filtered, img_to_process, mean.val[0] + stdev.val[0], 1.0, THRESH_TOZERO);
    img_to_process.convertTo(img_to_process, CV_8UC1);
    
    vector<Point> points = doProcess(img_to_process);
    if(!checkSquareArea(srcImg, points)) {
        points.clear();
    }
    if(!checkSquareWH(points)) {
        points.clear();
    }
    return points;
}
