//
// Created by Cutie on 2018/7/18.
//

#include <jni.h>
#include <string>
#include <BitmapUtil.h>
#include <process_helper.h>

static const char* JavaBridgeClass = "com/pig/android/opencv/OpencvTestBridge";

static struct {
    jclass jClassPoint;
    jmethodID jMethodInit;
    jfieldID jFieldIDX;
    jfieldID jFieldIDY;
} gPointInfo;

static void initClassInfo(JNIEnv *env) {
    gPointInfo.jClassPoint = reinterpret_cast<jclass>(env -> NewGlobalRef(env -> FindClass("android/graphics/Point")));
    gPointInfo.jMethodInit = env -> GetMethodID(gPointInfo.jClassPoint, "<init>", "(II)V");
    gPointInfo.jFieldIDX = env -> GetFieldID(gPointInfo.jClassPoint, "x", "I");
    gPointInfo.jFieldIDY = env -> GetFieldID(gPointInfo.jClassPoint, "y", "I");
}

static void imageToCrop(JNIEnv *env, jclass type, jobject srcBmp, jobject outBmp) {
    Mat srcBmpMat;
    bmpToMat(env, srcBmp, srcBmpMat);
    MyImageCrop::process_helper crop(srcBmpMat);
    Mat cropBmpMat = crop.doCrop();
    matToBmp(env, cropBmpMat, outBmp);
}

static void convertBytesToBmp(JNIEnv *env, jclass type, jbyteArray data,
                       jint width, jint height, jobject outBmp) {
    Mat srcBmpMat;
    bytesToMat(env, data, width, height, srcBmpMat);
    transpose(srcBmpMat, srcBmpMat);
    flip(srcBmpMat, srcBmpMat, 1);
    matToBmp(env, srcBmpMat, outBmp);
    srcBmpMat.release();
}

static jobject createJavaPoint(JNIEnv *env, Point point_) {
    return env -> NewObject(gPointInfo.jClassPoint, gPointInfo.jMethodInit, point_.x, point_.y);
}

static void findCropArea(JNIEnv *env, jclass type, jbyteArray data,
                         jint width, jint height, jobjectArray points) {
    Mat srcMat;
    bytesToMat(env, data, width, height, srcMat); //预览帧做旋转90度操作
    transpose(srcMat, srcMat);
    flip(srcMat, srcMat, 1);
    MyImageCrop::process_helper crop(srcMat);
    vector<Point> scanPoints = crop.findCropArea();
    if(scanPoints.size() == 4) {
        for (int i = 0; i < 4; ++i) {
            env -> SetObjectArrayElement(points, i, createJavaPoint(env, scanPoints[i]));
        }
    }
}

static vector<Point> pointsToNative(JNIEnv *env, jobjectArray points_) {
    int arrayLength = env->GetArrayLength(points_);
    vector<Point> result;
    for(int i = 0; i < arrayLength; i++) {
        jobject point_ = env -> GetObjectArrayElement(points_, i);
        int pX = env -> GetIntField(point_, gPointInfo.jFieldIDX);
        int pY = env -> GetIntField(point_, gPointInfo.jFieldIDY);
        result.push_back(Point(pX, pY));
    }
    return result;
}

static void doCrop(JNIEnv *env, jclass type, jbyteArray data,
                   jint width, jint height, jobjectArray _points, jobject outBitmap) {
    vector<Point> points = pointsToNative(env, _points);
    if (points.size() != 4) {
        return;
    }

    Point leftTop = points[0];
    Point rightTop = points[1];
    Point rightBottom = points[2];
    Point leftBottom = points[3];

    Mat srcMat;
    bytesToMat(env, data, width, height, srcMat); //预览帧做旋转90度操作
    transpose(srcMat, srcMat);
    flip(srcMat, srcMat, 1);

    AndroidBitmapInfo outBitmapInfo;
    AndroidBitmap_getInfo(env, outBitmap, &outBitmapInfo);
    Mat dstBitmapMat;
    int newHeight = outBitmapInfo.height;
    int newWidth = outBitmapInfo.width;
    dstBitmapMat = Mat::zeros(newHeight, newWidth, srcMat.type());

    vector<Point2f> srcTriangle;
    vector<Point2f> dstTriangle;

    srcTriangle.push_back(Point2f(leftTop.x, leftTop.y));
    srcTriangle.push_back(Point2f(rightTop.x, rightTop.y));
    srcTriangle.push_back(Point2f(leftBottom.x, leftBottom.y));
    srcTriangle.push_back(Point2f(rightBottom.x, rightBottom.y));

    dstTriangle.push_back(Point2f(0, 0));
    dstTriangle.push_back(Point2f(newWidth, 0));
    dstTriangle.push_back(Point2f(0, newHeight));
    dstTriangle.push_back(Point2f(newWidth, newHeight));

    Mat transform = getPerspectiveTransform(srcTriangle, dstTriangle);
    warpPerspective(srcMat, dstBitmapMat, transform, dstBitmapMat.size());

    matToBmp(env, dstBitmapMat, outBitmap);
}

static JNINativeMethod gMethods[] = {
        {
                "imageToCrop",
                "(Landroid/graphics/Bitmap;Landroid/graphics/Bitmap;)V",
                (void*)imageToCrop
        },
        {
                "convertBytesToBmp",
                "([BIILandroid/graphics/Bitmap;)V",
                (void*)convertBytesToBmp
        },
        {
                "findCropArea",
                "([BII[Landroid/graphics/Point;)V",
                (void*)findCropArea
        },
        {
                "doCrop",
                "([BII[Landroid/graphics/Point;Landroid/graphics/Bitmap;)V",
                (void*)doCrop

        }
};

extern "C"
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return JNI_FALSE;
    }
    jclass classJavaBridge = env->FindClass(JavaBridgeClass);
    if(env -> RegisterNatives(classJavaBridge, gMethods, sizeof(gMethods)/ sizeof(gMethods[0])) < 0) {
        return JNI_FALSE;
    }
    initClassInfo(env);
    return JNI_VERSION_1_4;
}