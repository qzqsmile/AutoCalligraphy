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
		vector<stroke> strokes;
		vector<point> points;
	public:
		Word() {};
		virtual ~Word() {};
		void storePoint(const point & p);
		void storeStroke(const stroke& s);
		vector<stroke> getStroke();
};


#endif