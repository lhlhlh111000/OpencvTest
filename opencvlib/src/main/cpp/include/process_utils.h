//
//  process_utils.hpp
//  square_scan
//
//  Created by Cutie on 2019/10/9.
//  Copyright © 2019 cqt. All rights reserved.
//

#ifndef process_utils_hpp
#define process_utils_hpp

#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

double getAngle(Point pt1, Point pt2, Point pt0);
int findLargeSquare(const vector<vector<Point> > &squares,
                    vector<Point> &biggest_square);
double getSpacePointToPoint(Point p1, Point p2);
Point2f computeIntersect(Vec4i a, Vec4i b);
vector<Point> findSquarePoints(vector<Point> &hull, vector<Point> &largest_square);
vector<Point> doProcess(Mat &gray_img);
bool checkSquareArea(Mat &srcImg, vector<Point> points);
vector<Point> sortPointClockwise(vector<Point> points);
long long pointSideLine(Point &lineP1, Point &lineP2, Point &point);
double getPointDistance(Point &p1, Point &p2);
bool checkSquareWH(vector<Point> points);

#endif /* process_utils_hpp */
