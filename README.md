# Bino_Stereo_Win_SDK
## **注意由于固件更新，2018年5月16日前出货的双目产品不能使用此版本SDK。请老客户通过"Bino双目技术支持"群联系我们，望谅解。**

## 1.使用前准备
    本SDK是BinoCamera的windows版SDK，测试版本为windows10

#### 1.1 安装Visual Studio2013
    Visual Studio2013是我们测试使用的版本，其他版本未经测试。
#### 1.2 安装OpenCV3.3
##### 我们测试使用的OpenCV版本为[OpenCV3.3](https://github.com/opencv/opencv/archive/3.3.0.zip)
##### OpenCV的windows配置请见[OpenCV官网说明](https://docs.opencv.org/3.3.0/d3/d52/tutorial_windows_install.html)

## 2.编译使用例子程序

#### 2.1 打开工程
    使用Visual Studio打开Bino_Stereo_Win_SDK工程
#### 2.2 进行环境配置
##### 2.2.1 鼠标右键点击工程名，选择:属性->配置属性->C/C++,在附加包含库目录中添加：
```
your_opencv_path\include
your_path\Bino_Stereo_Win_SDK\Bino_Stereo_Win_SDK\include
```

##### 2.2.2 鼠标右键点击工程名，选择:属性->配置属性->链接器->常规，在附加库目录中添加：
```
your_opencv_path\lib
your_path\Bino_Stereo_Win_SDK\Bino_Stereo_Win_SDK\lib
```
##### 2.2.3 鼠标右键点击工程名，选择:属性->配置属性->链接器->输入，在附加依赖项中添加：   

###### 若配置为Debug，则添加以下项:
```
	BinoCameraWind.lib
	opencv_calib3d330d.lib
	opencv_core330d.lib
	opencv_dnn330d.lib
	opencv_features2d330d.lib
	opencv_flann330d.lib
	opencv_highgui330d.lib
	opencv_imgcodecs330d.lib
	opencv_imgproc330d.lib
	opencv_ml330d.lib
	opencv_objdetect330d.lib
	opencv_photo330d.lib
	opencv_shape330d.lib
	opencv_stitching330d.lib
	opencv_superres330d.lib
	opencv_video330d.lib
	opencv_videoio330d.lib
	opencv_videostab330d.lib
	strmiids.lib
	Quartz.lib
```
###### 若配置为Release，则添加以下项:
```
	BinoCameraWin.lib
	opencv_calib3d330.lib
	opencv_core330.lib
	opencv_dnn330.lib
	opencv_features2d330.lib
	opencv_flann330.lib
	opencv_highgui330.lib
	opencv_imgcodecs330.lib
	opencv_imgproc330.lib
	opencv_ml330.lib
	opencv_objdetect330.lib
	opencv_photo330.lib
	opencv_shape330.lib
	opencv_stitching330.lib
	opencv_superres330.lib
	opencv_video330.lib
	opencv_videoio330.lib
	opencv_videostab330.lib
	strmiids.lib
	Quartz.lib
```
#### 2.3 运行例子代码

##### 2.3.1 运行图像显示例子
     插入binocamera，打开#define IMAGE宏定义（将其他打开的宏定义注释掉），编译运行即可
##### 2.3.2 运行imu数据采集例子
     插入binocamera，打开#define IMU宏定义（将其他打开的宏定义注释掉），编译运行即可

## 3.双目标定
```
双目标定由两个功能组成：棋盘格图像采集和双目标定
```
* 注意：示例中使用的是规格为11*7，单格棋盘格边长为0.02m的棋牌格标定板，若使用其他规格的棋盘格标定板，请自行修改BinoUser.h中的getCalibImage()函数的boardSize参数以及BinoUser.h中的startCalibStereo()函数的squareSize参数和boardSize参数，然后运行build.sh重新编译。

#### 3.1 棋盘格图像采集
```
1 插入binocamera，打开#define CalibGetImage宏定义（将其他打开的宏定义注释掉），编译运行
2 点击键盘'S'按键同时采集左右目图片。
3 采集采集左右目图片各35张作为标定使用的图像集。
```
* 注意：
* 1 保证两个摄像头都能拍摄到完整棋盘格：效果应该如'srv/采集图像事例.png'所示.
* 2 保证两个摄像头拍摄的棋盘格的角点计算结果一致: 红色角点在上，紫色角点在下，效果如'srv/采集图像事例.png'。
* 3 采集多种姿态的棋盘格图片：可参照 'srv/棋盘格姿态样例.pdf'

#### 3.2 双目标定 
```
插入binocamera，打开#define CalibStereo宏定义（将其他打开的宏定义注释掉），编译运行
标定程序分三个阶段

1 采集图片读入及棋盘格角点计算检测;此阶段请检查每张读入图片的棋盘格角点计算顺序是否一致。若一致，按空格键跳转到下一张图片，继续检查;
若不一致，查看命令行的打印，记住出错图片的编号，退出程序后打开calib文件夹下的stereo_calib.xml文件，将出错编号对应的左右图的索引删掉，然后重新运行程序。
2 直到命令行打印“Running stereo calibration ...”，表示标定程序开始运行，等待标定结束。
3 标定结束后会显示标定后的图像，用户可查看标定的结果是否正确。
```

## 4.购买链接
[链接](https://item.taobao.com/item.htm?spm=a230r.1.14.36.30447e471pexT6&id=562336856228&ns=1&abbucket=1#detail)

## 5.相关论文
* Cai Luo, Leijian Yu, Peng Ren, A Vision-aided Approach to Perching a Bio-inspired Unmanned Aerial Vehicle, IEEE Transactions on Industrial Electronics, DOI: 10.1109/TIE.2017.2764849.
* Cai Luo, Xiu Li, Yipeng Li, Qionghai Dai, Biomimetic Design for Unmanned Aerial Vehicle SafeLanding in Hazardous Terrain, IEEE/ASME Transactions on Mechatronics, 2016.02.01, 21(1):31 541.
* Cai Luo, Xiu Li, Qionghai Dai, Biology’s drones: New and improved, Science, 2014.6.20, 344(6190):1351 1351.
