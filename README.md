> 利用opencv库对Android摄像头实时摄入图像进行边框检测

* ps：项目为练手项目，有许多未经完善和优化的地方，可自行clone后进行改造或优化


moduel | 说明
---|---
opencvlib | 实现输入图像并检测图像中符合边框区域
camera | 实现摄像头预览功能，并回调实时摄入图像数据
app | 使用示例

> 截图：

![Alt text](/crop/ezgif.gif)


> 实现简介：

> ImageCropAreaView持有camera返回的实时摄入图像数据，开启线程轮询检测，根据检测结果绘制边框区域。

> OpencvTestBridge实现java与opencv桥接

* convertBytesToBmp 根据输入流输出图片
* findCropArea 查找边框区域
* doCrop 进行裁剪

> 开发环境：

* as 2.3.3
* ndk 14r
* opencv 2.4.11

感谢和参照：

[SmartCropper](https://github.com/pqpo/SmartCropper)

[cameraview](https://github.com/google/cameraview)

[opencv 实现对摄像头输入图像中文件及证件等的实时跟踪，四边形检测及提取](https://blog.csdn.net/zxw_xzr/article/details/77358815)