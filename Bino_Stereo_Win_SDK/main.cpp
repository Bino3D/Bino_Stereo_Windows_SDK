#include "BinoUser.h"
//#define IMU //运行imu数据获取的例子
#define IMAGE //运行图像数据获取的例子
//#define CalibGetImage //采集标定数据集
//#define CalibStereo //执行双目标定

#ifdef IMAGE 
int main(){
	getImage("Rect");//you can input "Rect"(获取双目矫正后图像), "Raw"(获取双目原始图像), "Disp"(取视差图像)
	return 0;
}
#endif

#ifdef IMU
int main(){
	getImuData();
	return 0;
}
#endif

#ifdef CalibGetImage
int main(){
	getCalibImage();
	return 0;
}
#endif

#ifdef CalibStereo
int main(){
	startCalibStereo();
	return 0;
}
#endif
