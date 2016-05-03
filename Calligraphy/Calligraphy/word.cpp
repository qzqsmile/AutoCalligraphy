#include"word.h"
#include"stroke.h"

void Word::storePoint(const point & p)
{
	points.push_back(p);
}

void Word::storeStroke(const stroke& s)
{
	strokes.push_back(s);
}

vector<stroke> Word::getStroke()
{
	return strokes;
}
