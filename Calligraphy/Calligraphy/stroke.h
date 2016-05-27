/*
包含了笔画的定义以及相应的操作函数
*/
#ifndef STROKE_H
#define STROKE_H

#include <opencv2/opencv.hpp>  
#include<vector>

using namespace std;
using namespace cv;

enum TYPE
{
	NONE=0, HENG, SHU, PIE, NA, GOU, TI,  
};

class stroke
{
	private:
		CvPoint begin;
		CvPoint end;
		enum TYPE type;
		float angle;
		int length;
		vector<CvPoint> outline;
		vector<CvPoint> midline;
		vector<int> width;
	public:
		stroke();
		virtual ~stroke(){};
	
		//存储函数
		void storeBegin(const CvPoint& b);
		void storeEnd(const CvPoint& e);
		void storeOutLine(const vector<CvPoint>& out);
		void storeMidLine(const vector<CvPoint>& mid);
		void storeAngle(float a);
		void storeLength(int len);
		void storeType(enum TYPE t);
		void storeWidth(const vector<int>& w);
		void convertPic(float ratio);

		//获取函数
		vector<int> getWidth();
		CvPoint getBegin();
		CvPoint getEnd();
		vector<CvPoint> getOutLine() const;
		vector<CvPoint> getMidLine() const;
		float getAngle() const;
		int getLen() const;
		enum TYPE getType() const;
};

bool IsHeng(const vector<CvPoint>& strokepoint, const IplImage * img);
bool IsShu(const vector<CvPoint>& stroke, const IplImage *img);
bool IsPie(const vector<CvPoint>& stroke, const IplImage *img, float *ang);
bool IsNa(const vector<CvPoint>&stroke, const IplImage *img);
bool IsGou(const vector<CvPoint>&stroke, const IplImage* img);
void DrawShuMiddle(vector<CvPoint>&strokeoutline, const IplImage *img, IplImage *outimg, vector<CvPoint>& shustroke, stroke&s);
void DrawHengMiddle(vector<CvPoint> &strokeoutline, const IplImage *img, IplImage *Outimg, vector<CvPoint>& hengstroke, stroke&s);
void DrawOutLine(vector<CvPoint>&strokeoutline, IplImage * img);
void DrawPieMiddle(vector<CvPoint>&strokeoutline, const IplImage *img, IplImage *outimg, vector<CvPoint>& shustroke, float ang, stroke& s);
void DrawNaMiddle(vector<CvPoint>&strokeoutline, const IplImage *img, IplImage *outimg, vector<CvPoint>& shustroke, stroke& s);
void DrawLine(const CvPoint&s, IplImage *out_img);
int findneareastpoint(const int x, const int y, const vector<CvPoint>& stroke);
void storestrokes(Mat &pic, vector<vector<CvPoint> >& p);
int calfreedom(int i, int j, const Mat& pic);
void dfs(Mat& pic, int i, int j, vector<CvPoint>& stroke);
void cutstroke(Mat& pic, vector<vector<CvPoint> >& strokes, const vector<CvPoint>& crosspoint);
void calWidth(const Mat & pic, const stroke& s, vector<int>& width);
void calType(stroke& s, TYPE& t);
bool strokecmp(CvPoint s1, CvPoint s2);
void sortstroke(stroke &s);

#endif