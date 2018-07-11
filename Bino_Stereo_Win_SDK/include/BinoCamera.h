#define POINTER_64 __ptr64
#ifndef BINOCAMERA_H
#define BINOCAMERA_H

#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <vector>
#include "opencv2/opencv.hpp"  
/**
*@brief ������������ṹ��
*���а�����
* */
struct BinoCameraParameterList{
	std::string extParameterPath = "";    /**< ����궨����ļ����ڵ�ַ*/
	std::string intParameterPath = "";    /**< ����궨�ڲ��ļ����ڵ�ַ*/
	std::string fisheyeParameterPath = "";    /**< ����궨�ڲ��ļ����ڵ�ַ*/
};

/**
*@brief imu���ݽṹ��
* */
struct ImuData{
	uint32_t time;/**< �ɼ�imu���ݵ�ʱ��*/

	float accel_x; /**< imu���ٶȼ�x�����ݣ���λ��g��*/
	float accel_y; /**< imu���ٶȼ�y�����ݣ���λ��g��*/
	float accel_z; /**< imu���ٶȼ�z�����ݣ���λ��g��*/

	float gyro_x; /**< imu������x�����ݣ���λ������/�룩*/
	float gyro_y; /**< imu������y�����ݣ���λ������/�룩*/
	float gyro_z; /**< imu������z�����ݣ���λ������/�룩*/
};


/**
*@brief BinoCamera ��ͷ�ļ�
*
* */
class  BinoCamera{
public:
	/**
	*@brief BinoCamera ���캯��
	*@param paraList ����������������
	* */
	BinoCamera(BinoCameraParameterList paraList);
	virtual ~BinoCamera();
	/**
	*@brief ���binocamera�豸�����г�ʼ������ѭ�������
	* */
	void CameraInit();
	/**
	*@brief ץȡһ��������ݣ���Ҫ����ѭ���е���
	* */
	void Grab();
	/**
	*@brief �õ�˫Ŀԭʼͼ��
	*@param L ���������ͼ��
	*@param R ���������ͼ��
	* */
	void getOrgImage(cv::Mat& L, cv::Mat& R);
	/**
	@brief �õ�˫Ŀ���������ͼ��
	@param L ���������������ͼ��
	@param R ���������������ͼ��
	* */
	void getRectImage(cv::Mat& L, cv::Mat& R);
	/**
	@brief imuԭʼ����У��
	@param deadmin ������Сֵ
	@param deadmax �������ֵ
	@param axerr   ���ٶȼ�x����ƫУ��
	@param ayerr   ���ٶȼ�y����ƫУ��
	@param azerr   ���ٶȼ�z����ƫУ��
	* */
	void setImuCorrection(float deadmin, float deadmax, float axerr, float ayerr, float azerr);
	/**
	@brief �õ�imuԭʼ����
	@param imudatas imu��ԭʼ����
	@param timestamp ͼ��֡�ع�ʱ�̵�ʱ��
	* */
	void getImuRawData(std::vector<ImuData> &imuDatas, uint32_t &timestamp);
	/**
	@brief �õ�ͼ��֡�ع�ʱ�̵�ʱ��
	* */
	uint32_t getImgTime();
	/**
	@brief imu�������
	@param imu imuԭʼ����
	@param timestamp ����imu���ݲɼ�֮���ʱ���
	@param q ��Ԫ��
	* */
	void ImuRect(ImuData imu, float timestamp, float *q);
	/**
	@brief �õ��õ��Ӳ�ͼ
	@param disparateU16Mat ����uint16���͵��Ӳ�ͼ����õ���ʵ�Ӳ�ֵ��Ҫת��Ϊfloat�����ݣ�ת�����ӣ�
	matU16.convertTo( matF32, CV_32F, 1.0/16);
	* */
	void getDisparity(cv::Mat& disparateU16Mat);
	/**
	@brief ˫Ŀ�궨
	@param imagelist �洢�궨ʹ��ͼ�����Ƶ�����
	@param boardSize ʹ�õ����̸�Ĺ��д���ʽΪ: boardSize = Size((һ�����̸�ĸ��� - 1), (һ�����̸�ĸ��� - 1));
	@param squareSize ʹ�õ����̸�ÿ�����ӵĴ�С(��λ: m)
	* */
	void StartStereoCalib(const std::vector<std::string>& imagelist, cv::Size boardSize, const float squareSize);
private:
	cv::Mat LeftImg, RightImg, LeftRect, RightRect, DispImg;
	std::vector<ImuData> ImuDatas;
	uint32_t TimeStamp;
};
#endif