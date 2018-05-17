#pragma once
#include "opencv2/opencv.hpp"

using namespace cv;

#define W_0 0
#define H_0 0
#define W_1 0
#define H_1 0
#define CARD_W 52
#define CARD_H 69
#define ROI_W 15
#define ROI_H 30
#define CARD_W_X 26
#define CARD_H_X 35

#define CARDS_NUM (13*4)

class CPokeDetective
{
public:
	CPokeDetective();
	virtual ~CPokeDetective();
public:
	//模版匹配
	int getTemplete(Mat &src, int TemplateNum);
	//获取所有像素点和  
	void getPXSum(Mat &src, int &a);
	void getPokeName(int nidx,char *name);
};

