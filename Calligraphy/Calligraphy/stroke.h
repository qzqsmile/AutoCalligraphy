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
		int begin[2];
		int end[2];
		int index[2];
		int type;
		int angle;
		int length;
		vector<CvPoint> outline;
		vector<CvPoint> midline;
	public:
		Stroke(){}
		void display() const{};
		void getstroke() const{};
};

bool IsHeng(const vector<CvPoint>& strokepoint, const IplImage * img);
bool IsShu(const vector<CvPoint>& stroke, const IplImage *img);
bool IsPie(const vector<CvPoint>& stroke, const IplImage *img, float *ang);
bool IsNa(const vector<CvPoint>&stroke, const IplImage *img);
bool IsGou(const vector<CvPoint>&stroke, const IplImage* img);
void DrawShuMiddle(vector<CvPoint>&stroke, const IplImage *img, IplImage *outimg, vector<CvPoint>& shustroke);
void DrawHengMiddle(vector<CvPoint> &stroke, const IplImage *img, IplImage *Outimg, vector<CvPoint>& hengstroke);
void DrawOutLine(vector<CvPoint>&stroke, IplImage * img);
void DrawPieMiddle(vector<CvPoint>&stroke, const IplImage *img, IplImage *outimg, vector<CvPoint>& shustroke, float ang);
void DrawNaMiddle(vector<CvPoint>&stroke, const IplImage *img, IplImage *outimg, vector<CvPoint>& shustroke);
void DrawLine(CvPoint&s, IplImage *out_img);

#endif