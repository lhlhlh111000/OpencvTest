package com.example.cqt.opencvtest;

import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.ImageView;


import java.io.File;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 *
 * Created by cqt on 2018/7/23.
 */
public class CropActivity extends AppCompatActivity {

    private String photoPath;

    private Uri photoUri;

    private ImageView ivSrc;

    private ImageView ivShow;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_crop);

        findViewById(R.id.btn_camera).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
                File photoFile = createImgFile();
                photoUri = Uri.fromFile(photoFile);
                intent.putExtra(MediaStore.EXTRA_OUTPUT, photoUri);
                startActivityForResult(intent, 1000);
            }
        });

        findViewById(R.id.btn_gallery).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
                intent.setType("image/*");
                startActivityForResult(intent, 1001);
            }
        });
        ivSrc = (ImageView) findViewById(R.id.iv_crop_src);
        ivShow = (ImageView) findViewById(R.id.iv_crop_show);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode == RESULT_OK) {
            switch (requestCode) {
                case 1000:
                    setImageBitmap();
                    break;
                case 1001:
                    //data中自带有返回的uri
                    photoUri = data.getData();
                    //获取照片路径
                    String[] filePathColumn = {MediaStore.Audio.Media.DATA};
                    Cursor cursor = getContentResolver().query(photoUri, filePathColumn, null, null, null);
                    cursor.moveToFirst();
                    photoPath = cursor.getString(cursor.getColumnIndex(filePathColumn[0]));
                    cursor.close();
                    //有了照片路径，之后就是压缩图片，和之前没有什么区别
                    setImageBitmap();
                    break;
            }
        }
    }

    /**
     * 自定义图片名，获取照片的file
     */
    private File createImgFile() {
        //确定文件名
        String fileName = "img_" + new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date()) + ".jpg";
        File dir;
        if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {
            dir = Environment.getExternalStorageDirectory();
        } else {
            dir = getExternalFilesDir(Environment.DIRECTORY_PICTURES);
        }
        File tempFile = new File(dir, fileName);
        try {
            if (tempFile.exists()) {
                tempFile.delete();
            }
            tempFile.createNewFile();
        } catch (IOException e) {
            e.printStackTrace();
        }
        //获取文件路径
        photoPath = tempFile.getAbsolutePath();
        return tempFile;
    }

    /**
     * 压缩图片
     */
    private void setImageBitmap() {
        //获取imageview的宽和高
        int targetWidth=ivSrc.getWidth();
        int targetHeight=ivSrc.getHeight();

        //根据图片路径，获取bitmap的宽和高
        BitmapFactory.Options options=new BitmapFactory.Options();
        options.inJustDecodeBounds=true;
        BitmapFactory.decodeFile(photoPath,options);
        int photoWidth=options.outWidth;
        int photoHeight=options.outHeight;

        //获取缩放比例
        int inSampleSize=1;
        if(photoWidth>targetWidth||photoHeight>targetHeight){
            int widthRatio=Math.round((float)photoWidth/targetWidth);
            int heightRatio=Math.round((float)photoHeight/targetHeight);
            inSampleSize=Math.min(widthRatio,heightRatio);
        }

        //使用现在的options获取Bitmap
        options.inSampleSize=(int) (inSampleSize*1.5);
        options.inJustDecodeBounds=false;
        Bitmap bitmap=BitmapFactory.decodeFile(photoPath,options);
        Bitmap cropBmp = Bitmap.createBitmap(bitmap.getWidth(), bitmap.getHeight(), Bitmap.Config.ARGB_8888);
//        OpencvTestBridge.imageToCrop(bitmap, cropBmp);
//        ivSrc.setImageBitmap(bitmap);
//        if(null != cropBmp) {
//            ivShow.setImageBitmap(cropBmp);
//        }
    }
}