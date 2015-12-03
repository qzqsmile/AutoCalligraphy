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
	public:
};

bool IsHeng(const vector<CvPoint>& strokepoint, const IplImage * img);
bool Isshu(const vector<CvPoint>& stroke, const IplImage *img);
void DrawshuMiddle(vector<CvPoint>&stroke, const IplImage *img, IplImage *outimg, vector<CvPoint>& shustroke);
void DrawHengMiddle(vector<CvPoint> &stroke, const IplImage *img, IplImage *Outimg, vector<CvPoint>& hengstroke);


#endif