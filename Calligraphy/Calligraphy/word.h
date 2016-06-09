#ifndef WORD_H
#define WORD_H

#include<vector>
#include"point.h"
#include"stroke.h"

using std::vector;

class Word
{
	private:
		int strokecount;
		vector<int> order;
		vector<vector<int>> beginplace;
		vector<stroke> strokes;
		vector<CvPoint> places;
		vector<CvPoint> crosspoints;
		vector<point> points;
	public:
		Word() {};
		virtual ~Word() {};
		void storePoint(const point & p);
		void storeStroke(const vector<stroke>& s);
		void stroksPoints(const vector<point>& p);
		void storeStrokeNum(int count);
		void storeCrosspoints(vector<CvPoint> points);
		void storeStrokePlace();
		vector<CvPoint> getStrokePlace();
		int getStrokeNum();
		vector<stroke> getStroke();
		vector<CvPoint> getStrokeCrossPoints();
		vector<point> getPoints();
		void convertPic(float ratio, CvPoint beginpic);
};

#endif