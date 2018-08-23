package com.example.cqt.opencvtest;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Point;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.view.View;

/**
 * Created by cqt on 2018/8/23.
 */

public class ScaleTestView extends View {

    private Paint paint;

    private Point pointTopLeft = new Point(100,  100);

    private Point pointTopRight = new Point(600, 120);

    private Point pointBottomRight = new Point(700, 740);

    private Point pointBottomLeft = new Point(90, 780);

    private Path path = new Path();


    public ScaleTestView(Context context) {
        this(context, null);
    }

    public ScaleTestView(Context context, @Nullable AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public ScaleTestView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);

        setWillNotDraw(false);

        paint = new Paint();
        paint.setColor(Color.BLACK);
        paint.setStyle(Paint.Style.STROKE);
        paint.setStrokeWidth(10);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        Point[] points = new Point[] {pointTopLeft, pointTopRight, pointBottomRight, pointBottomLeft};
        Path path = setupPath(points);
        canvas.drawPath(path, paint);

        Point centrePoint = obtainCentrePoint();
        canvas.drawCircle(centrePoint.x, centrePoint.y, 10, paint);

        Point[] pointsScale = new Point[] {
                scalePoint(pointTopLeft, centrePoint),
                scalePoint(pointTopRight, centrePoint),
                scalePoint(pointBottomRight, centrePoint),
                scalePoint(pointBottomLeft, centrePoint),};
        Path pathScale = setupPath(pointsScale);
        canvas.drawPath(pathScale, paint);
    }

    private Path setupPath(Point[] points) {
        path.reset();

        path.moveTo(points[0].x, points[0].y);
        for(int i=1; i<points.length; i++) {
            path.lineTo(points[i].x, points[i].y);
        }
        path.close();
        return path;
    }

    private Point scalePoint(Point targetPoint, Point centrePoint) {
        Matrix matrix = new Matrix();
        // 将Matrix移动到指定位置，
        // 然后再以点为中心进行缩放
        matrix.preTranslate(targetPoint.x,targetPoint.y);
        float scale = 0.5f;
        matrix.postScale(scale, scale, centrePoint.x, centrePoint.y);
        float[] values = new float[9];
        matrix.getValues(values);
        targetPoint.x = (int) values[Matrix.MTRANS_X];
        targetPoint.y = (int) values[Matrix.MTRANS_Y];
        return targetPoint;
    }

    private Point obtainCentrePoint() {
        int x1 = pointTopLeft.x;
        int y1 = pointTopLeft.y;

        int x2 = pointTopRight.x;
        int y2 = pointTopRight.y;

        int x3 = pointBottomRight.x;
        int y3 = pointBottomRight.y;

        int x4 = pointBottomLeft.x;
        int y4 = pointBottomLeft.y;

        int centreX = ((x3-x1)*(x4-x2)*(y2-y1)+x1*(y3-y1)*(x4-x2)-x2*(y4-y2)*(x3-x1))/((y3-y1)*(x4-x2)-(y4-y2)*(x3-x1));
        int centreY = (y3-y1)*((x4-x2)*(y2-y1)+(x1-x2)*(y4-y2))/((y3-y1)*(x4-x2)-(y4-y2)*(x3-x1))+y1;
        return new Point(centreX, centreY);
    }
}