//
//  old_process.cpp
//  square_scan
//
//  Created by Cutie on 2019/10/8.
//  Copyright © 2019 cqt. All rights reserved.
//

#include "old_process.h"

vector<Point> doOldFind(Mat& srcImg) {
    Mat img_to_process;
    cvtColor(srcImg, img_to_process, COLOR_RGB2GRAY);
    blur(img_to_process, img_to_process, Size(3, 3));
    erode(img_to_process, img_to_process, Mat(), Point(-1, -1), 3, 1, 1);
    int thresh = 40;
    Canny(img_to_process, img_to_process, thresh, thresh*3, 3);
    dilate(img_to_process, img_to_process, Mat(), Point(-1, -1), 3, 1, 1);
    
    vector<Point> points = doProcess(img_to_process);
    if(!checkSquareArea(srcImg, points)) {
        points.clear();
    }
    if(!checkSquareWH(points)) {
        points.clear();
    }
    return points;
}
