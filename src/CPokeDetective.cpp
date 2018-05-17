#include "stdafx.h"
#include "CPokeDetective.h"


CPokeDetective::CPokeDetective()
{
}


CPokeDetective::~CPokeDetective()
{
}

void CPokeDetective::getPXSum(Mat &src, int &a)
{
	threshold(src, src, 100, 255, CV_THRESH_BINARY);
	a = 0;
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			a += src.at <uchar>(i, j);
		}
	}
}

int  CPokeDetective::getTemplete(Mat &src, int TemplateNum)
{
	int diff = 0;
	Mat img_result;
	int min = 1000000;
	int serieNum = 0;
	for (int i = 0; i < TemplateNum; i++) {
		char name[33];
		sprintf_s(name, "./pokes/%d.png", i+1);
		Mat Template = imread(name);
		resize(src, src, Size(CARD_W_X, CARD_H_X), 0, 0, CV_INTER_LINEAR);
		resize(Template, Template, Size(CARD_W_X, CARD_H_X), 0, 0, CV_INTER_LINEAR);//µ÷Õû³ß´ç		
		absdiff(Template, src, img_result);//  
		getPXSum(img_result, diff);
		if (diff < min)
		{
			min = diff;
			serieNum = i;
		}
	}
	return serieNum;
}

void CPokeDetective::getPokeName(int nidx,char *name)
{
	if (nidx > CARDS_NUM || nidx < 0)
		return;

	char sz[33] = { '\0' };
	char szPt[13];
	char szCol[13];
	sprintf_s(szPt, 13, "%d", (nidx % 13) + 1);

	int col = nidx / 13;
	switch (col) {
	case 0:
		sprintf_s(szCol, 13, "%s", "ºÚÌÒ");
		break;
	case 1:
		sprintf_s(szCol, 13, "%s", "ºìÌÒ");
		break;
	case 2:
		sprintf_s(szCol, 13, "%s", "Ã·»¨");
		break;
	case 3:
		sprintf_s(szCol, 13, "%s", "·½¿é");
		break;
	}
	sprintf_s(name, 33,"%s%s",szCol, szPt);
}