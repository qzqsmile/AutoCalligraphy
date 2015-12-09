/*
包含了笔画的定义以及相应的操作函数
*/
#ifndef STROKE_H
#define STROKE_H

#include <opencv2/opencv.hpp>  
#include<vector>

using namespace std;
using namespace cv;

class Stroke
{
	private:
		int type;
		int angle;
		vector<CvPoint> outline;
		vector<CvPoint> midline;
	public:
};

bool IsHeng(const vector<CvPoint>& strokepoint, const IplImage * img);
bool Isshu(const vector<CvPoint>& stroke, const IplImage *img);
void DrawShuMiddle(vector<CvPoint>&stroke, const IplImage *img, IplImage *outimg, vector<CvPoint>& shustroke);
void DrawHengMiddle(vector<CvPoint> &stroke, const IplImage *img, IplImage *Outimg, vector<CvPoint>& hengstroke);
void DrawOutLine(vector<CvPoint>&stroke, IplImage * img);
bool IsPie(const vector<CvPoint>& stroke, const IplImage *img);
void DrawPieMiddle(vector<CvPoint>&stroke, const IplImage *img, IplImage *outimg, vector<CvPoint>& shustroke);
bool IsNa(const vector<CvPoint>&stroke, const IplImage *img);
void DrawNaMiddle(vector<CvPoint>&stroke, const IplImage *img, IplImage *outimg, vector<CvPoint>& shustroke);

#endif