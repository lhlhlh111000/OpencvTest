package com.example.cqt.opencvtest;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.widget.ImageView;

/**
 * Created by cqt on 2018/8/2.
 */

public class ResultShowActivity extends AppCompatActivity {

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_result_show);

        ImageView ivShow = findViewById(R.id.iv_result_show);
        if(null != Camera2Activity.sResult) {
            ivShow.setImageBitmap(Camera2Activity.sResult);
        }
    }
}