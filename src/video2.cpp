#include "stdafx.h"
//
//#include <opencv2/core.hpp>
//#include <opencv2/videoio.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/videoio/container_avi.private.hpp>
//#include <iostream>
//
//
//using namespace std;
//using namespace cv;
//
//void main() {
//
//	VideoCapture capture(0);//如果是笔记本，0打开的是自带的摄像头，1 打开外接的相机  
//	double rate = 25.0;//视频的帧率  
//	Size videoSize(1280, 960);
//	VideoWriter writer("VideoTest.avi", CV_FOURCC('M', 'J', 'P', 'G'), rate, videoSize);
//	Mat frame;
//
//	while (capture.isOpened())
//	{
//		capture >> frame;
//		writer << frame;
//		imshow("video", frame);
//		if (waitKey(20) == 27)//27是键盘摁下esc时，计算机接收到的ascii码值  
//		{
//			break;
//		}
//	}
//
//}
