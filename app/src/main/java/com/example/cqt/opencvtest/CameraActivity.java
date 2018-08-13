package com.example.cqt.opencvtest;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;

/**
 *
 * Created by cqt on 2018/7/26.
 */

public class CameraActivity extends AppCompatActivity {

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_camera);

//        cameraView = (JavaCameraView) findViewById(R.id.camera);
//        cameraView.setCvCameraViewListener(new CameraBridgeViewBase.CvCameraViewListener2() {
//            @Override
//            public void onCameraViewStarted(int width, int height) {
//
//            }
//
//            @Override
//            public void onCameraViewStopped() {
//
//            }
//
//            @Override
//            public Bitmap onCameraFrame(CameraBridgeViewBase.CvCameraViewFrame inputFrame) {
//                return inputFrame.obtainFrame();
//            }
//        });
//        cameraView.enableView();
    }

    @Override
    protected void onResume() {
        super.onResume();
    }
}