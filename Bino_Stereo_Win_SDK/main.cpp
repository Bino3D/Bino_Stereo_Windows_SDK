#include "BinoUser.h"
//#define IMU //����imu���ݻ�ȡ������
#define IMAGE //����ͼ�����ݻ�ȡ������
//#define CalibGetImage //�ɼ��궨���ݼ�
//#define CalibStereo //ִ��˫Ŀ�궨

#ifdef IMAGE 
int main(){
	getImage("Rect");//you can input "Rect"(��ȡ˫Ŀ������ͼ��), "Raw"(��ȡ˫Ŀԭʼͼ��), "Disp"(ȡ�Ӳ�ͼ��)
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
