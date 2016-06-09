#include"word.h"
#include"stroke.h"

void Word::storePoint(const point & p)
{
	points.push_back(p);
}

void Word::storeStroke(const vector<stroke>& s)
{
	strokes = s;
}

vector<stroke> Word::getStroke()
{
	return strokes;
}

int Word::getStrokeNum()
{
	return strokecount; 
}

void Word::storeStrokeNum(int count)
{
	strokecount = count;
}

void Word::storeCrosspoints(vector<CvPoint> points)
{
	crosspoints = points;
}

vector<CvPoint> Word::getStrokeCrossPoints()
{
	return crosspoints;
}

void Word::stroksPoints(const vector<point>& p)
{
	points = p;
}

vector<point> Word::getPoints()
{
	return points;
}

void Word::convertPic(float ratio, CvPoint beginpic)
{
	for(unsigned int i = 0; i < beginplace.size(); i++)
	{
		for(unsigned int j = 0; j < beginplace[i].size(); j++)
		{
			beginplace[i][j] =(int)(beginplace[i][j] * ratio);
		}
	}

	for(unsigned int i = 0; i < crosspoints.size(); i++)
	{
		crosspoints[i].x = crosspoints[i].x - beginpic.x;
		crosspoints[i].y = crosspoints[i].y - beginpic.y;
		crosspoints[i].x = (int)(crosspoints[i].x * ratio);
		crosspoints[i].y = (int)(crosspoints[i].y * ratio);
	}

	for(unsigned int i = 0; i < places.size(); i++)
	{
		places[i].x = (int)(places[i].x - beginpic.x);
		places[i].y = (int)(places[i].y - beginpic.y);
		places[i].x = (int)(places[i].x * ratio);
		places[i].y = (int)(places[i].y * ratio);
	}
}