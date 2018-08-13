package com.pig.android.opencv;

import android.graphics.Bitmap;
import android.util.Log;

import java.nio.ByteBuffer;

/**
 * Created by cqt on 2018/7/26.
 */
public class Util {

    public static byte[] bmpToByte(Bitmap bmp) {
        byte[] result = null;
        if(null == bmp) {
            return result;
        }

        int bytes = bmp.getByteCount();

        ByteBuffer buf = ByteBuffer.allocate(bytes);
        bmp.copyPixelsToBuffer(buf);

        result = buf.array();
        return result;
    }

    public static Bitmap byteToBmp(byte[] bytes, int width, int height, Bitmap cacheBmp) {
        long startTime = System.currentTimeMillis();
        if(null == cacheBmp) {
            cacheBmp = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        }
        Log.e("AAAAAA", "create Time: " + (System.currentTimeMillis() - startTime));
        OpencvTestBridge.convertBytesToBmp(bytes, width, height, cacheBmp);
        Log.e("AAAAAA", "end Time: " + (System.currentTimeMillis() - startTime));
        return cacheBmp;
    }
}