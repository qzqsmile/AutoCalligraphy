/*
��Ͷ�ļ������˵�Ķ��壬�Լ���Ӧ��һЩ��������
*/
#ifndef POINT_H
#define POINT_H

#include<opencv2/opencv.hpp>  
#include<vector>

using namespace std;

class point
{
	private:
		int longlength;
		int shortlength;
		vector<CvPoint> outline;
		CvPoint longbegin;
		CvPoint shortbegin;
		CvPoint longend;
		CvPoint shortend;
		double area;
	public:
		point();
		virtual ~point(){};
		
		void calPar();
		void storeLongLine(const CvPoint &begin, const CvPoint& end);
		void storeShortLine(const CvPoint &begin, const CvPoint & end);
		void storeOutLine(const vector<CvPoint>& s);
		CvPoint getLongLineBegin() const;
		CvPoint getLongLineEnd() const;
		CvPoint getShortLineBegin() const;
		CvPoint getShortLineEnd() const;
		int getParA() const;
		int getParB() const;
};


bool IsPoint(const CvSeq *pstroke);
int CalPointLongLine(const vector<CvPoint>&cv, vector<CvPoint>& store);
int CalPointShortLine(const vector<CvPoint>&cv, vector<CvPoint> & shortstore, int begin, int end, int longvecx, int longvecy);
void DrawWaterDrop(IplImage *img, int a, int b);

#endif

