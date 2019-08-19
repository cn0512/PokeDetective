
#include "stdafx.h"

//读取摄像头
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	VideoCapture cap;// (0 + cv::CAP_DSHOW);
	cap.open(0);// 0 + cv::CAP_DSHOW); //打开摄像头

	if (!cap.isOpened())
		return -1;

	Mat frame;
	while (1)
	{
		cap >> frame;//等价于cap.read(frame);
		//Canny(frame, frame, 30, 100);//canny边缘检测，去掉这一行就是纯粹的读取摄像头了
		cvtColor(frame, frame, COLOR_BGR2HSV);//BGR空间转为HSV颜色空间，注意，两者不能同时对同一张图片（在此为frame）进行处理，否则报错
		if (frame.empty())
			break;
		imshow("video", frame);
		if (waitKey(20) > 0)//按下任意键退出摄像头　　因电脑环境而异，有的电脑可能会出现一闪而过的情况
			break;
	}
	cap.release();//释放所有资源
	destroyAllWindows();//关闭所有窗口

	return 0;
}