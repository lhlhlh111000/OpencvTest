//
// Created by Cutie on 2018/7/19.
//

#ifndef OPENCVTEST_BITMAPUTIL_H
#define OPENCVTEST_BITMAPUTIL_H

#include <opencv2/opencv.hpp>
#include <android/bitmap.h>

using namespace cv;

void bmpToMat(JNIEnv *env, jobject &srcBitmap, Mat &srcMat);

void matToBmp(JNIEnv *env, Mat &srcMat, jobject &dstBitmap);

void bytesToBmp(JNIEnv *env, jbyteArray &data, jint width, jint height, jobject &dstBitmap);

void bytesToMat(JNIEnv *env, jbyteArray &data, jint width, jint height, Mat &mat);

#endif //OPENCVTEST_BITMAPUTIL_H
