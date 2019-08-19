
#include "stdafx.h"

//��ȡ����ͷ
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
	cap.open(0);// 0 + cv::CAP_DSHOW); //������ͷ

	if (!cap.isOpened())
		return -1;

	Mat frame;
	while (1)
	{
		cap >> frame;//�ȼ���cap.read(frame);
		//Canny(frame, frame, 30, 100);//canny��Ե��⣬ȥ����һ�о��Ǵ���Ķ�ȡ����ͷ��
		cvtColor(frame, frame, COLOR_BGR2HSV);//BGR�ռ�תΪHSV��ɫ�ռ䣬ע�⣬���߲���ͬʱ��ͬһ��ͼƬ���ڴ�Ϊframe�����д������򱨴�
		if (frame.empty())
			break;
		imshow("video", frame);
		if (waitKey(20) > 0)//����������˳�����ͷ��������Ի������죬�еĵ��Կ��ܻ����һ�����������
			break;
	}
	cap.release();//�ͷ�������Դ
	destroyAllWindows();//�ر����д���

	return 0;
}