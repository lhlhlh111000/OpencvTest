package com.pig.android.opencv;

import android.graphics.Bitmap;
import android.graphics.Point;

/**
 * Created by cqt on 2018/7/18.
 */
public class OpencvTestBridge {

    static {
        System.loadLibrary("opencv_test");
    }

    public static native String obtainNativeText();

    public static native void imageToGray(Bitmap srcBmp, Bitmap outBmp);

    public static native void imageToCrop(Bitmap srcBmp, Bitmap outBmp);

    public static native void convertBytesToBmp(byte[] bytes, int width, int height, Bitmap outBmp);

    public static native void findCropArea(byte[] bytes, int width, int height, Point[] outPoints);

    public static native void doCrop(byte[] bytes, int width, int height, Point[] points, Bitmap outBitmap);
}