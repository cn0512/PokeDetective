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
//	VideoCapture capture(0);//����ǱʼǱ���0�򿪵����Դ�������ͷ��1 ����ӵ����  
//	double rate = 25.0;//��Ƶ��֡��  
//	Size videoSize(1280, 960);
//	VideoWriter writer("VideoTest.avi", CV_FOURCC('M', 'J', 'P', 'G'), rate, videoSize);
//	Mat frame;
//
//	while (capture.isOpened())
//	{
//		capture >> frame;
//		writer << frame;
//		imshow("video", frame);
//		if (waitKey(20) == 27)//27�Ǽ�������escʱ����������յ���ascii��ֵ  
//		{
//			break;
//		}
//	}
//
//}
