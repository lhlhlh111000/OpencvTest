//
// Created by Cutie on 2018/7/23.
//

#include <ImageCrop.h>

using namespace MyImageCrop;
using namespace cv;

ImageCrop::ImageCrop(cv::Mat &bitmap) {
    srcBmp = bitmap;
}

ImageCrop::~ImageCrop() {
// Do Nothing
}

cv::Mat ImageCrop::doCrop() {
    Mat grayMat, filtered, edges, dilated_edges;
    Mat resizeMat = resizeImage();
    //获取灰度图像
    cvtColor(resizeMat, grayMat, COLOR_RGB2GRAY);
    //滤波，模糊处理，消除某些背景干扰信息
    blur(grayMat, filtered, cv::Size(3, 3));
    //腐蚀操作，消除某些背景干扰信息
    erode(filtered, filtered, Mat(), cv::Point(-1, -1), 3, 1, 1);
    int thresh = 40;
    //边缘检测
    Canny(filtered, edges, thresh, thresh * 3, 3);
    //膨胀操作，尽量使边缘闭合
    dilate(edges, dilated_edges, Mat(), cv::Point(-1, -1), 3, 1, 1);
    vector<vector<cv::Point>> contours, squares, hulls;
    //寻找边框
    findContours(dilated_edges, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    vector<cv::Point> hull, approx;
    for (size_t i = 0; i < contours.size(); i++) {
        //边框的凸包
        convexHull(contours[i], hull);
        //多边形拟合凸包边框(此时的拟合的精度较低)
        approxPolyDP(Mat(hull), approx, arcLength(Mat(hull), true) * 0.02, true);
        //筛选出面积大于某一阈值的，且四边形的各个角度都接近直角的凸四边形
        if (approx.size() == 4 && fabs(contourArea(Mat(approx))) > areaThreshold &&
            isContourConvex(Mat(approx))) {
            double maxCosine = 0;
            for (int j = 2; j < 5; j++) {
                double cosine = fabs(getAngle(approx[j % 4], approx[j - 2], approx[j - 1]));
                maxCosine = MAX(maxCosine, cosine);
            }
            //角度大概72度
            if (maxCosine < 0.3) {
                squares.push_back(approx);
                hulls.push_back(hull);
            }
        }
    }

    vector<cv::Point> largest_square;
    //找出外接矩形最大的四边形
    int idex = findLargestSquare(squares, largest_square);
    if (largest_square.size() == 0 || idex == -1) {
        return srcBmp;
    }

    //找到这个最大的四边形对应的凸边框，再次进行多边形拟合，此次精度较高，拟合的结果可能是大于4条边的多边形
    //接下来的操作，主要是为了解决，证件有圆角时检测到的四个顶点的连线会有切边的问题
    hull = hulls[idex];
    approxPolyDP(Mat(hull), approx, 3, true);
    vector<cv::Point> newApprox;
    double maxL = arcLength(Mat(approx), true) * 0.02;
    //找到高精度拟合时得到的顶点中 距离小于 低精度拟合得到的四个顶点 maxL的顶点，排除部分顶点的干扰
    for (cv::Point p : approx) {
        if (!(getSpacePointToPoint(p, largest_square[0]) > maxL &&
              getSpacePointToPoint(p, largest_square[1]) > maxL &&
              getSpacePointToPoint(p, largest_square[2]) > maxL &&
              getSpacePointToPoint(p, largest_square[3]) > maxL)) {
            newApprox.push_back(p);
        }
    }
    //找到剩余顶点连线中，边长大于 2 * maxL的四条边作为四边形物体的四条边
    vector<Vec4i> lines;
    for (int i = 0; i < newApprox.size(); i++) {
        cv::Point p1 = newApprox[i];
        cv::Point p2 = newApprox[(i + 1) % newApprox.size()];
        if (getSpacePointToPoint(p1, p2) > 2 * maxL) {
            lines.push_back(Vec4i(p1.x, p1.y, p2.x, p2.y));
        }
    }

    //计算出这四条边中 相邻两条边的交点，即物体的四个顶点
    vector<cv::Point> cornors1;
    for (int i = 0; i < lines.size(); i++) {
        cv::Point cornor;
        cornor = computeIntersect(lines[i], lines[(i + 1) % lines.size()]);
        cornor.x *= resizeScale; // 坐标还原
        cornor.y *= resizeScale;
        cornors1.push_back(cornor);
    }
    //绘制出四条边
    for (int i = 0; i < cornors1.size(); i++) {
        line(srcBmp, cornors1[i], cornors1[(i + 1) % cornors1.size()], Scalar(0, 0, 255), 5);
    }

    return srcBmp;
}

vector<cv::Point> ImageCrop::findCropArea() {
    Mat grayMat, filtered, edges, dilated_edges;
    Mat resizeMat = resizeImage();
    //获取灰度图像
    cvtColor(resizeMat, grayMat, COLOR_RGB2GRAY);
    //滤波，模糊处理，消除某些背景干扰信息
    blur(grayMat, filtered, cv::Size(3, 3));
    //腐蚀操作，消除某些背景干扰信息
    erode(filtered, filtered, Mat(), cv::Point(-1, -1), 3, 1, 1);
    int thresh = 40;
    //边缘检测
    Canny(filtered, edges, thresh, thresh * 3, 3);
    //膨胀操作，尽量使边缘闭合
    dilate(edges, dilated_edges, Mat(), cv::Point(-1, -1), 3, 1, 1);
    vector<vector<cv::Point>> contours, squares, hulls;
    //寻找边框
    findContours(dilated_edges, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    vector<cv::Point> hull, approx;
    for (size_t i = 0; i < contours.size(); i++) {
        //边框的凸包
        convexHull(contours[i], hull);
        //多边形拟合凸包边框(此时的拟合的精度较低)
        approxPolyDP(Mat(hull), approx, arcLength(Mat(hull), true) * 0.02, true);
        //筛选出面积大于某一阈值的，且四边形的各个角度都接近直角的凸四边形
        if (approx.size() == 4 && fabs(contourArea(Mat(approx))) > areaThreshold &&
            isContourConvex(Mat(approx))) {
            double maxCosine = 0;
            for (int j = 2; j < 5; j++) {
                double cosine = fabs(getAngle(approx[j % 4], approx[j - 2], approx[j - 1]));
                maxCosine = MAX(maxCosine, cosine);
            }
            //角度大概72度
            if (maxCosine < 0.3) {
                squares.push_back(approx);
                hulls.push_back(hull);
            }
        }
    }

    vector<cv::Point> largest_square;
    //找出外接矩形最大的四边形
    int idex = findLargestSquare(squares, largest_square);
    if (largest_square.size() == 0 || idex == -1) {
        return largest_square;
    }

    //找到这个最大的四边形对应的凸边框，再次进行多边形拟合，此次精度较高，拟合的结果可能是大于4条边的多边形
    //接下来的操作，主要是为了解决，证件有圆角时检测到的四个顶点的连线会有切边的问题
    hull = hulls[idex];
    approxPolyDP(Mat(hull), approx, 3, true);
    vector<cv::Point> newApprox;
    double maxL = arcLength(Mat(approx), true) * 0.02;
    //找到高精度拟合时得到的顶点中 距离小于 低精度拟合得到的四个顶点 maxL的顶点，排除部分顶点的干扰
    for (cv::Point p : approx) {
        if (!(getSpacePointToPoint(p, largest_square[0]) > maxL &&
              getSpacePointToPoint(p, largest_square[1]) > maxL &&
              getSpacePointToPoint(p, largest_square[2]) > maxL &&
              getSpacePointToPoint(p, largest_square[3]) > maxL)) {
            newApprox.push_back(p);
        }
    }
    //找到剩余顶点连线中，边长大于 2 * maxL的四条边作为四边形物体的四条边
    vector<Vec4i> lines;
    for (int i = 0; i < newApprox.size(); i++) {
        cv::Point p1 = newApprox[i];
        cv::Point p2 = newApprox[(i + 1) % newApprox.size()];
        if (getSpacePointToPoint(p1, p2) > 2 * maxL) {
            lines.push_back(Vec4i(p1.x, p1.y, p2.x, p2.y));
        }
    }

    //计算出这四条边中 相邻两条边的交点，即物体的四个顶点
    vector<cv::Point> cornors1;
    for (int i = 0; i < lines.size(); i++) {
        cv::Point cornor;
        cornor = computeIntersect(lines[i], lines[(i + 1) % lines.size()]);
        cornor.x *= resizeScale; // 坐标还原
        cornor.y *= resizeScale;
        cornors1.push_back(cornor);
    }

    // 按左上，右上，右下，左下排序（顺时针排序）
    return sortPointClockwise(cornors1);
//    return cornors1;
}

cv::Mat ImageCrop::resizeImage() {
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
        areaThreshold = (int) (resizedBitmap.cols*resizedBitmap.rows * 0.2); // 阀值取总面积0.2
        return resizedBitmap;
    }
    return srcBmp;
}

/**
 * 寻找最大边框
 * @param squares
 * @param biggest_square
 * @return
 */
int ImageCrop::findLargestSquare(const vector<vector<cv::Point> > &squares,
                                 vector<cv::Point> &biggest_square) {
    if (!squares.size()) return -1;

    int max_width = 0;
    int max_height = 0;
    int max_square_idx = 0;
    for (int i = 0; i < squares.size(); i++) {
        cv::Rect rectangle = boundingRect(Mat(squares[i]));
        if ((rectangle.width >= max_width) && (rectangle.height >= max_height)) {
            max_width = rectangle.width;
            max_height = rectangle.height;
            max_square_idx = i;
        }
    }
    biggest_square = squares[max_square_idx];
    return max_square_idx;
}

/**
 根据三个点计算中间那个点的夹角   pt1 pt0 pt2
 */
double ImageCrop::getAngle(cv::Point pt1, cv::Point pt2, cv::Point pt0) {
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1 * dx2 + dy1 * dy2) /
           sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
}

/**
 点到点的距离

 @param p1 点1
 @param p2 点2
 @return 距离
 */
double ImageCrop::getSpacePointToPoint(cv::Point p1, cv::Point p2) {
    int a = p1.x - p2.x;
    int b = p1.y - p2.y;
    return sqrt(a * a + b * b);
}

/**
 两直线的交点

 @param a 线段1
 @param b 线段2
 @return 交点
 */
cv::Point2f ImageCrop::computeIntersect(cv::Vec4i a, cv::Vec4i b) {
    int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3], x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];

    if (float d = ((float) (x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4))) {
        cv::Point2f pt;
        pt.x = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / d;
        pt.y = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / d;
        return pt;
    } else{
        return cv::Point2f(-1, -1);
    }
}

vector<Point> ImageCrop::sortPointClockwise(std::vector<cv::Point> points) {
    if (points.size() != 4) {
        return points;
    }

    Point unFoundPoint;
    vector<Point> result = {unFoundPoint, unFoundPoint, unFoundPoint, unFoundPoint};

    long minDistance = -1;
    for(Point &point : points) {
        long distance = point.x * point.x + point.y * point.y;
        if(minDistance == -1 || distance < minDistance) {
            result[0] = point;
            minDistance = distance;
        }
    }
    if (result[0] != unFoundPoint) {
        Point &leftTop = result[0];
        points.erase(std::remove(points.begin(), points.end(), leftTop));
        if ((pointSideLine(leftTop, points[0], points[1]) * pointSideLine(leftTop, points[0], points[2])) < 0) {
            result[2] = points[0];
        } else if ((pointSideLine(leftTop, points[1], points[0]) * pointSideLine(leftTop, points[1], points[2])) < 0) {
            result[2] = points[1];
        } else if ((pointSideLine(leftTop, points[2], points[0]) * pointSideLine(leftTop, points[2], points[1])) < 0) {
            result[2] = points[2];
        }
    }
    if (result[0] != unFoundPoint && result[2] != unFoundPoint) {
        Point &leftTop = result[0];
        Point &rightBottom = result[2];
        points.erase(std::remove(points.begin(), points.end(), rightBottom));
        if (pointSideLine(leftTop, rightBottom, points[0]) > 0) {
            result[1] = points[0];
            result[3] = points[1];
        } else {
            result[1] = points[1];
            result[3] = points[0];
        }
    }

    if (result[0] != unFoundPoint && result[1] != unFoundPoint && result[2] != unFoundPoint && result[3] != unFoundPoint) {
        return result;
    }

    return points;
}

long long ImageCrop::pointSideLine(cv::Point &lineP1, cv::Point &lineP2, cv::Point &point) {
    long x1 = lineP1.x;
    long y1 = lineP1.y;
    long x2 = lineP2.x;
    long y2 = lineP2.y;
    long x = point.x;
    long y = point.y;
    return (x - x1)*(y2 - y1) - (y - y1)*(x2 - x1);
}