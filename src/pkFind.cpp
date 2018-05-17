// pkFind.cpp: 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

using namespace cv;

#pragma comment(lib,"../../../opencv/vc15/lib/opencv_world341d.lib")

#define W_0 0
#define H_0 0
#define W_1 0
#define H_1 0
#define CARD_W 70
#define CARD_H 97
#define ROI_W 15
#define ROI_H 30

int main()
{
	//[1]load
	Mat pic = imread("../../pics/pk1.png");
	imshow("原图", pic);
	//[2]ROI
	Mat roiAll(Size(ROI_W * 13, ROI_H * 4), CV_8UC3);
	Mat mask(Size(ROI_W, ROI_H), CV_8UC3);
	for (int i = 0; i < 13*4; i++) {
		Mat roi = pic(Rect(W_0+(CARD_W+W_1)*(i%13),H_0+(CARD_H+H_1)*(i/13), ROI_W, ROI_H));		
		Mat imageROI = roiAll(Rect(ROI_W * (i%13), ROI_H * (i/13), ROI_W, ROI_H));
		roi.copyTo(imageROI, mask);
	}
	imshow("roi", roiAll);
	//[3]识别数字
	waitKey(0);
    return 0;
}

