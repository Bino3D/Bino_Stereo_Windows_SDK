#define POINTER_64 __ptr64
#ifndef BINOCAMERA_H
#define BINOCAMERA_H

#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <vector>
#include "opencv2/opencv.hpp"  
/**
*@brief 相机参数表述结构体
*其中包含相
* */
struct BinoCameraParameterList{
	std::string extParameterPath = "";    /**< 相机标定外参文件所在地址*/
	std::string intParameterPath = "";    /**< 相机标定内参文件所在地址*/
	std::string fisheyeParameterPath = "";    /**< 相机标定内参文件所在地址*/
};

/**
*@brief imu数据结构体
* */
struct ImuData{
	uint32_t time;/**< 采集imu数据的时间*/

	float accel_x; /**< imu加速度计x轴数据，单位（g）*/
	float accel_y; /**< imu加速度计y轴数据，单位（g）*/
	float accel_z; /**< imu加速度计z轴数据，单位（g）*/

	float gyro_x; /**< imu陀螺仪x轴数据，单位（弧度/秒）*/
	float gyro_y; /**< imu陀螺仪y轴数据，单位（弧度/秒）*/
	float gyro_z; /**< imu陀螺仪z轴数据，单位（弧度/秒）*/
};


/**
*@brief BinoCamera 类头文件
*
* */
class  BinoCamera{
public:
	/**
	*@brief BinoCamera 构造函数
	*@param paraList 传入的相机启动参数
	* */
	BinoCamera(BinoCameraParameterList paraList);
	virtual ~BinoCamera();
	/**
	*@brief 检测binocamera设备并进行初始化，在循环外调用
	* */
	void CameraInit();
	/**
	*@brief 抓取一次相机数据，需要在主循环中调用
	* */
	void Grab();
	/**
	*@brief 得到双目原始图像
	*@param L 返回左相机图像
	*@param R 返回右相机图像
	* */
	void getOrgImage(cv::Mat& L, cv::Mat& R);
	/**
	@brief 得到双目畸变矫正后图像
	@param L 返回左相机矫正后图像
	@param R 返回右相机矫正后图像
	* */
	void getRectImage(cv::Mat& L, cv::Mat& R);
	/**
	@brief imu原始数据校正
	@param deadmin 死区最小值
	@param deadmax 死区最大值
	@param axerr   加速度计x轴零偏校正
	@param ayerr   加速度计y轴零偏校正
	@param azerr   加速度计z轴零偏校正
	* */
	void setImuCorrection(float deadmin, float deadmax, float axerr, float ayerr, float azerr);
	/**
	@brief 得到imu原始数据
	@param imudatas imu的原始数据
	@param timestamp 图像帧曝光时刻的时间
	* */
	void getImuRawData(std::vector<ImuData> &imuDatas, uint32_t &timestamp);
	/**
	@brief 得到图像帧曝光时刻的时间
	* */
	uint32_t getImgTime();
	/**
	@brief imu解算程序
	@param imu imu原始数据
	@param timestamp 两次imu数据采集之间的时间戳
	@param q 四元数
	* */
	void ImuRect(ImuData imu, float timestamp, float *q);
	/**
	@brief 得到得到视差图
	@param disparateU16Mat 返回uint16类型的视差图，想得到真实视差值需要转换为float型数据，转换例子：
	matU16.convertTo( matF32, CV_32F, 1.0/16);
	* */
	void getDisparity(cv::Mat& disparateU16Mat);
	/**
	@brief 双目标定
	@param imagelist 存储标定使用图像名称的容器
	@param boardSize 使用的棋盘格的规格，写入格式为: boardSize = Size((一行棋盘格的个数 - 1), (一列棋盘格的个数 - 1));
	@param squareSize 使用的棋盘格每个格子的大小(单位: m)
	* */
	void StartStereoCalib(const std::vector<std::string>& imagelist, cv::Size boardSize, const float squareSize);
private:
	cv::Mat LeftImg, RightImg, LeftRect, RightRect, DispImg;
	std::vector<ImuData> ImuDatas;
	uint32_t TimeStamp;
};
#endif