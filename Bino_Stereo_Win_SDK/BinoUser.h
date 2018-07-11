#ifndef BINOUSER_H
#define BINOUSER_H
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <stdio.h>
#include <future>
#include <thread>
#include "BinoCamera.h"
using namespace std;
using namespace cv;
/***IMU死区***/
float deadmin = -0.8f;
float deadmax = 0.8f;

/***IMU加速度计零偏***/
float axerr = 0.0045f;
float ayerr = -0.0266f;
float azerr = -0.0100f;

static bool readStringList(const string& filename, vector<string>& l)
{
	l.resize(0);
	FileStorage fs(filename, FileStorage::READ);
	if (!fs.isOpened())
		return false;
	FileNode n = fs.getFirstTopLevelNode();
	if (n.type() != FileNode::SEQ)
		return false;
	FileNodeIterator it = n.begin(), it_end = n.end();
	for (; it != it_end; ++it)
		l.push_back((string)*it);
	return true;
}


void getImage(std::string type){
	BinoCameraParameterList paraList;
	paraList.intParameterPath = ".\\params\\intrinsics.yml";
	paraList.extParameterPath = ".\\params\\extrinsics.yml";
	cv::Mat left;
	cv::Mat right;
	cv::Mat para;
	cv::Mat disp;
	int cnt = 0;
	std::shared_ptr<BinoCamera> camera(new BinoCamera(paraList));
	camera->CameraInit();
	for (;;){
		camera->Grab();
		if (type == "Rect"){
			camera->getRectImage(left, right);
			cv::imshow("image_rect_left", left);
			cv::imshow("image_rect_right", right);
		}

		else if (type == "Raw"){
			camera->getOrgImage(left, right);
			cv::imshow("image_left", left);
			cv::imshow("image_right", right);
		}

		else if(type == "Disp"){
			camera->getDisparity(disp);
			disp.convertTo(disp, CV_8U, 255.0 / (48 * 16));
			cv::applyColorMap(disp, disp, cv::COLORMAP_JET);
			cv::imshow("image_disparity", disp);
		}
		int key = cv::waitKey(1);
		if (key == 'q')
			break;
		else if (key == 's'){
			//cv::imwrite("/home/li/workspace/Bino_Stereo_SDK/pic/l" + std::to_string(cnt) + ".png", left);
			//cv::imwrite("/home/li/workspace/Bino_Stereo_SDK/pic/r" + std::to_string(cnt) + ".png", right);
			cnt++;
		}
	}
}

void getImuData(){
	BinoCameraParameterList paraList;
	paraList.intParameterPath = ".\\params\\intrinsics.yml";
	paraList.extParameterPath = ".\\params\\extrinsics.yml";
	std::vector<ImuData> imudatas;
	ImuData imudata;
	uint32_t current_time = 0, old_time = 0;
	float timestamp = 0.0f;
	uint32_t time_image = 0;
	float q[4] = { 0 };
	std::shared_ptr<BinoCamera> camera(new BinoCamera(paraList));
	camera->CameraInit();
	camera->setImuCorrection(deadmin, deadmax, axerr, ayerr, azerr);
	for (;;){
		camera->Grab();
		camera->getImuRawData(imudatas, time_image);
		size_t size = imudatas.size();
		for (uint8_t i = 0; i < size; i++){
			imudata = imudatas[i];
			current_time = imudata.time;
			timestamp = (float)((current_time - old_time) / 1000.0f);
			camera->ImuRect(imudata, timestamp, q);
			old_time = current_time;
			printf("imudata(w,x,y,z): [%f, %f, %f, %f]\n", q[0], q[1], q[2], q[3]);
		}
	}
}

void getCalibImage(){
	BinoCameraParameterList paraList;
	paraList.intParameterPath = ".\\params\\intrinsics.yml";
	paraList.extParameterPath = ".\\params\\extrinsics.yml";
	cv::Mat imageLeft;
	cv::Mat imageRight;
	cv::Size boardSize = cv::Size(11, 7);
	bool isFoundLeft, isFoundRight;
	int cnt = 0;
	std::shared_ptr<BinoCamera> camera(new BinoCamera(paraList));
	camera->CameraInit();
	for (;;) {
		camera->Grab();
		camera->getOrgImage(imageLeft, imageRight);
		std::vector<cv::Point2f> corenrsL;
		std::vector<cv::Point2f> corenrsR;
		cv::Mat showMatLeft(imageLeft.rows, imageLeft.cols, CV_8UC3, cv::Scalar::all(0));
		cv::Mat showMatRight(imageRight.rows, imageRight.cols, CV_8UC3, cv::Scalar::all(0));
		isFoundLeft = cv::findChessboardCorners(imageLeft, boardSize, corenrsL, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);
		isFoundRight = cv::findChessboardCorners(imageRight, boardSize, corenrsR, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);

		if (isFoundLeft && isFoundRight){
			cv::cvtColor(imageLeft, showMatLeft, CV_GRAY2BGR);
			cv::cvtColor(imageRight, showMatRight, CV_GRAY2BGR);
			cv::drawChessboardCorners(showMatLeft, boardSize, corenrsL, true);
			cv::drawChessboardCorners(showMatRight, boardSize, corenrsR, true);
		}
		else{
			showMatLeft = imageLeft.clone();
			showMatRight = imageRight.clone();
		}
		cv::imshow("image_left", showMatLeft);
		cv::imshow("image_right", showMatRight);
		int key = cv::waitKey(1);
		if(key == 'q')
			break;
		else if(key == 's'){
			stringstream num;
			num << format("%02d", cnt);;
			cv::imwrite(".\\calib\\left" + num.str() + ".jpg", imageLeft);
			cv::imwrite(".\\calib\\right" + num.str() + ".jpg", imageRight);
			cout << "save image" << cnt << " success!" << endl;
			cnt++;
		}

	}
}

void startCalibStereo(){
	BinoCameraParameterList paraList;
	paraList.intParameterPath = ".\\params\\intrinsics.yml";
	paraList.extParameterPath = ".\\params\\extrinsics.yml";
	Size boardSize;
	string imagelistfn;
	float squareSize = 0.02f;
	boardSize = Size(11, 7);
	std::shared_ptr<BinoCamera> camera(new BinoCamera(paraList));
	if (imagelistfn == "")
	{
		imagelistfn = "calib/stereo_calib.xml";
	}
	else if (boardSize.width <= 0 || boardSize.height <= 0)
	{
		cout << "if you specified XML file with chessboards, you should also specify the board width and height (-w and -h options)" << endl;
		exit(0);
	}
	vector<string> imagelist;
	bool ok = readStringList(imagelistfn, imagelist);
	cout << "size = " << imagelist.size() << endl;
	camera->StartStereoCalib(imagelist, boardSize, squareSize);
}




#endif