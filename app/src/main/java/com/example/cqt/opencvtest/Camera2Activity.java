package com.example.cqt.opencvtest;

import android.content.Intent;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Toast;

import com.google.android.cameraview.CameraView;
import com.pig.android.opencv.ImageCropAreaView;

/**
 * Created by cqt on 2018/8/1.
 */

public class Camera2Activity extends AppCompatActivity {

    CameraView cameraView;

    ImageCropAreaView areaView;

    public static Bitmap sResult = null;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_camera_2);

        cameraView = (CameraView) findViewById(R.id.camera2);
        areaView = (ImageCropAreaView) findViewById(R.id.crop_view);

        cameraView.addCallback(new CameraView.Callback() {
            @Override
            public void onCameraOpened(CameraView cameraView) {
                super.onCameraOpened(cameraView);
            }

            @Override
            public void onCameraClosed(CameraView cameraView) {
                super.onCameraClosed(cameraView);
            }

            @Override
            public void onPictureTaken(CameraView cameraView, byte[] data) {
                super.onPictureTaken(cameraView, data);
            }

            @Override
            public void onPreviewFrame(byte[] data, int width, int height) {
                super.onPreviewFrame(data, width, height);
                areaView.setupData(data, width, height);
            }
        });

        findViewById(R.id.btn_do_crop).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(null == areaView) {
                    return;
                }

                Bitmap bitmap = areaView.doCrop();
                if(null == bitmap) {
                    Toast.makeText(Camera2Activity.this, "没检测到", Toast.LENGTH_SHORT).show();
                    return;
                }
                Intent intent = new Intent(Camera2Activity.this, ResultShowActivity.class);
                sResult = bitmap;
                startActivity(intent);
            }
        });
    }

    @Override
    protected void onResume() {
        super.onResume();
        cameraView.start();
        if(null != areaView) {
            areaView.start();
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        if(null != areaView) {
            areaView.stop();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        cameraView.stop();
    }
}