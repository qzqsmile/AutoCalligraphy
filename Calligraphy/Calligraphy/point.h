/*
��Ͷ�ļ������˵�Ķ��壬�Լ���Ӧ��һЩ��������
*/
#ifndef POINT_H
#define POINT_H

#include<opencv2/opencv.hpp>  
#include<vector>

using namespace std;

class Point
{
	private:
		int maxlength;
		int shrotlength;
		int x, y;
		vector<CvPoint> points;
		CvPoint longbegin;
		CvPoint shortbegin;
		CvPoint longend;
		CvPoint shortend;
		double area;
	public:
		Point();
		~Point();
};

bool IsPoint(const CvSeq *pstroke);
int CalPointLongLine(const vector<CvPoint>&cv, vector<CvPoint>& store);
int CalPointShortLine(const vector<CvPoint>&cv, vector<CvPoint> & shortstore, int begin, int end, int longvecx, int longvecy);
void DrawWaterDrop(IplImage *img, int a, int b);

#endif

