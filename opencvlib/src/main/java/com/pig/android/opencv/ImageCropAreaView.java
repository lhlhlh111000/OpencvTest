package com.pig.android.opencv;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.PixelFormat;
import android.graphics.Point;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;


/**
 * 显示裁剪区域
 * Created by cqt on 2018/8/1.
 */

public class ImageCropAreaView extends SurfaceView implements SurfaceHolder.Callback, Runnable {

    private boolean mIsInit = false;

    private boolean mIsStop = true;

    private byte[] mData = null;

    private int mWidth;

    private int mHeight;

    private Point[] mPoints;

    private Thread mThread = null;

    private Path mPath = new Path();

    private Paint mPaint = new Paint();

    private float mScale = 1.0f;

    public ImageCropAreaView(Context context) {
        this(context, null);
    }

    public ImageCropAreaView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public ImageCropAreaView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        setZOrderOnTop(true);
        getHolder().setFormat(PixelFormat.TRANSLUCENT);
        getHolder().addCallback(this);

        mPaint.setAntiAlias(true);
        mPaint.setColor(Color.BLACK);
        mPaint.setStrokeWidth(10);
        mPaint.setStyle(Paint.Style.STROKE);
    }

    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {
        mIsInit = true;
        start();
    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {
        mIsInit = false;
        mIsStop = true;
    }

    @Override
    public void run() {
        while (!mIsStop) {
            drawCropArea();
        }
    }

    public void start() {
        postDelayed(new Runnable() {
            @Override
            public void run() {
                synchronized (ImageCropAreaView.this) {
                    if(!mIsInit) {
                        return;
                    }

                    if(!mIsStop) {
                        return;
                    }

                    mIsStop = false;
                    if(null == mThread) {
                        mThread = new Thread(ImageCropAreaView.this);
                        mThread.start();
                    }
                }
            }
        }, 1000);
    }

    public void stop() {
        try {
            mIsStop = true;
            if (mThread != null){
                mThread.interrupt();
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            mThread =  null;
        }
    }

    /**
     * 绑定数据
     * @param data
     * @param width
     * @param height
     */
    public void setupData(byte[] data, int width, int height) {
        if(mIsStop) {
            return;
        }

        this.mData = data;
        this.mWidth = width;
        this.mHeight = height;

        float widthScale = (float) getWidth()/height;
        float heightScale = (float) getHeight()/width;
        mScale = Math.max(widthScale, heightScale);
    }

    /**
     * 裁剪
     */
    public Bitmap doCrop() {
        Bitmap result;
        if(null == mData) {
            return null;
        }

        if(checkPointArea(mPoints)) { // 裁剪识别区域
            Point leftTop = mPoints[0];
            Point rightTop = mPoints[1];
            Point rightBottom = mPoints[2];
            Point leftBottom = mPoints[3];

            int cropWidth = (int) ((getPointsDistance(leftTop, rightTop)
                    + getPointsDistance(leftBottom, rightBottom))/2);
            int cropHeight = (int) ((getPointsDistance(leftTop, leftBottom)
                    + getPointsDistance(rightTop, rightBottom))/2);
            result = Bitmap.createBitmap(cropWidth, cropHeight, Bitmap.Config.ARGB_8888);
            OpencvTestBridge.doCrop(mData, mWidth, mHeight, mPoints, result);
        }else { // 返回原图
            result = Bitmap.createBitmap(mHeight, mWidth, Bitmap.Config.ARGB_8888);
            OpencvTestBridge.convertBytesToBmp(mData, mWidth, mHeight, result);
        }

        return result;
    }

    private void drawCropArea() {
        Canvas canvas = null;
        try {
            canvas = getHolder().lockCanvas();
            if(null == canvas) {
                return;
            }
            canvas.drawColor(0, android.graphics.PorterDuff.Mode.CLEAR);
            if(null == mData || 0 == mData.length) {
                return;
            }
            Point[] points = new Point[4];
            OpencvTestBridge.findCropArea(mData, mWidth, mHeight, points);
            Path path = resetPointPath(points);
            if(null != path) {
                canvas.drawPath(path, mPaint);
            }

            mPoints = points;
        }catch(Exception e) {
            e.printStackTrace();
        }finally {
            if(null != canvas) {
                getHolder().unlockCanvasAndPost(canvas);
            }
        }
    }

    private boolean checkPointArea(Point[] points) {
        return points != null && points.length == 4
                && points[0] != null && points[1] != null && points[2] != null && points[3] != null;
    }

    private Path resetPointPath(Point[] points) {
        if (!checkPointArea(points)) {
            return null;
        }
        mPath.reset();
        Point lt = scalePoint(points[0], obtainCentrePoint(points));
        Point rt = scalePoint(points[1], obtainCentrePoint(points));
        Point rb = scalePoint(points[2], obtainCentrePoint(points));
        Point lb = scalePoint(points[3], obtainCentrePoint(points));
        mPath.moveTo(getPreviewX(lt.x), getPreviewY(lt.y));
        mPath.lineTo(getPreviewX(rt.x), getPreviewY(rt.y));
        mPath.lineTo(getPreviewX(rb.x), getPreviewY(rb.y));
        mPath.lineTo(getPreviewX(lb.x), getPreviewY(lb.y));
        mPath.close();
        return mPath;
    }

    private float getPreviewX(float x) {
        return x*mScale;
    }

    private float getPreviewY(float y) {
        return y*mScale;
    }

    public static double getPointsDistance(Point p1, Point p2) {
        return getPointsDistance(p1.x, p1.y, p2.x, p2.y);
    }

    public static double getPointsDistance(float x1, float y1, float x2, float y2) {
        return Math.sqrt(Math.pow(x1 - x2, 2) + Math.pow(y1 - y2, 2));
    }

    private Point obtainCentrePoint(Point[] points) {
        int x1 = points[0].x;
        int y1 = points[0].y;

        int x2 = points[1].x;
        int y2 = points[1].y;

        int x3 = points[2].x;
        int y3 = points[2].y;

        int x4 = points[3].x;
        int y4 = points[3].y;

        int centreX = ((x3-x1)*(x4-x2)*(y2-y1)+x1*(y3-y1)*(x4-x2)-x2*(y4-y2)*(x3-x1))/((y3-y1)*(x4-x2)-(y4-y2)*(x3-x1));
        int centreY = (y3-y1)*((x4-x2)*(y2-y1)+(x1-x2)*(y4-y2))/((y3-y1)*(x4-x2)-(y4-y2)*(x3-x1))+y1;
        return new Point(centreX, centreY);
    }

    private Point scalePoint(Point targetPoint, Point centrePoint) {
        Matrix matrix = new Matrix();
        // 将Matrix移动到指定位置，
        // 然后再以点为中心进行缩放
        matrix.preTranslate(targetPoint.x,targetPoint.y);
        float scale = 1.1f;
        matrix.postScale(scale, scale, centrePoint.x, centrePoint.y);
        float[] values = new float[9];
        matrix.getValues(values);
        targetPoint.x = (int) values[Matrix.MTRANS_X];
        targetPoint.y = (int) values[Matrix.MTRANS_Y];
        return targetPoint;
    }
}