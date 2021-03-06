// pkFind.cpp: 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "time.h"
#include "CPokeDetective.h"

using namespace cv;

//#pragma comment(lib,"../../../opencv/vc15/lib/opencv_world341d.lib")



int main2()
{
	CPokeDetective dete;
	//[1]load
	Mat mask = imread("./pokes/mask.png");	
	//[2]ROI
	Mat cardsAll(Size(CARD_W * 13, CARD_H * 4), CV_8UC3);		
	for (int i = 0; i < 13*4; i++) {
		char cname[33];
		sprintf_s(cname, 33,"./pokes/%d.png", i+1);
		Mat pic = imread(cname);	
		Mat imageROI = cardsAll(Rect(CARD_W * (i%13), CARD_H * (i/13), CARD_W, CARD_H));
		pic.copyTo(imageROI, mask);
	}
	imshow("AllCards", cardsAll);
	//[3]待识别的组合
	const int nDet = 1;
	Mat cards2Det(Size(CARD_W * nDet, CARD_H * 1), CV_8UC3);
	srand(time(0));
	for (int i = 0; i < nDet; i++) {
		char cname[33];
		sprintf_s(cname, 33, "./pokes/%d.png", rand()%52 + 1);
		Mat pic = imread(cname);
		Mat imageROI = cards2Det(Rect(CARD_W * (i % 13), CARD_H * (i / 13), CARD_W, CARD_H));
		pic.copyTo(imageROI, mask);
	}
	imshow("Detecative", cards2Det);
	//[4]切分图像	
	//数字识别，获得对应模版
	int n = dete.getTemplete(cards2Det, CARDS_NUM);
	char name[64] = { '\0' };
	dete.getPokeName(n, name);
	printf("待测试的牌:模版=%d,点数=%s\n",n, name);
	waitKey(0);
    return 0;
}

