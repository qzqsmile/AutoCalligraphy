#include"point.h"
#include<vector>
#include<cmath>

using namespace std;

CvPoint point::getLongLineBegin() const
{
	return longbegin;
}

CvPoint point::getLongLineEnd() const
{
	return longend;
}

CvPoint point::getShortLineBegin() const
{
	return shortbegin;
}

CvPoint point::getShortLineEnd() const
{
	return shortend;
}
int point::getParA() const
{
	return longlength;
}

int point::getParB() const
{
	return shortlength;
}

void point::storeOutLine(const vector<CvPoint>& s)
{
	outline = s;
}

point::point():longlength(0), shortlength(0), area(0.0)
{
	longbegin.x = longbegin.y = 0;
	shortbegin = longend = shortend = longbegin;
};

void point::storeLongLine(const CvPoint& begin, const CvPoint& end)
{
	longbegin = begin;
	longend = end;
}

void point::storeShortLine(const CvPoint& begin, const CvPoint& end)
{
	shortbegin = begin;
	shortend = end;
}

void point::calPar()
{
	longlength = (int)sqrt(pow((longbegin.x-longend.x), 2) + pow((longbegin.y-longend.y), 2));
	shortlength = (int)sqrt(pow((shortbegin.x-shortend.x),2) + pow((shortbegin.y-shortend.y),2));
}


/*
画水滴形状，用以比较
*/
void DrawWaterDrop(IplImage *img, int a, int b)
{
	float i = 0;
	int x0 = img->width / 2;
	CvPoint prepoint, nextpoint;
	prepoint.x = nextpoint.x = -1;

	while(i < 8.0)
	{
		i = i+0.1;
		nextpoint.x = (int) a * (1 - sin(i)) * cos(i) + x0;
		nextpoint.y = (int) b * (sin(i) - 1);
		if(prepoint.x == -1)
			prepoint = nextpoint;
		cvLine(img, prepoint, nextpoint, CV_RGB(0,0,255),1,0);
		prepoint = nextpoint;
	}
}


/*
功能：判断输入的轮廓是不是一个点的轮廓
输入：pstroke指向一个轮廓
输出：如果是点的轮廓返回true，否则返回false
*/
bool IsPoint(const CvSeq *pstroke)
{
	if((pstroke->total > 10) && (pstroke->total < 100))
	//if(pstroke->total < 100)
		return true;
	return false;
}

/*
功能：计算点的长边
输入：cv里存储的是点轮廓的坐标，store返回的是长边的开始与结束点
输出：返回长边的长度，并把长边的开始与结束点存入store中。
*/

int CalPointLongLine(const vector<CvPoint>&cv, vector<CvPoint>& store)
{
	int max = 0;
	
	for(unsigned int i = 0; i < cv.size(); i++)
	{
		for(unsigned int j = i; j < cv.size(); j++)
		{
			//这里是否要改用float?
			int temp = (int)sqrt(pow(abs(cv[i].x-cv[j].x),2)+pow(abs(cv[i].y-cv[j].y),2));
			if(temp > max)
			{
				store.clear();
				max = temp;
				store.push_back(cv[i]);
				store.push_back(cv[j]);
			}
		}           
	}

	return max;
}

/*
功能：计算点轮廓短边长度，并返回开始与结束点
输入：cv是存储轮廓的店，shortstore返回的是短边的开始与结束点。
	  begin与end分别代表的是长边轮廓的开始与结束点的在cv中存储的下边
	  longvecx是长边的向量的x值，longvecy是长边向量的y值
输出：返回短边的长度，并把短边开始与结束点存入shortstore中
*/
int CalPointShortLine(const vector<CvPoint>& cv, vector<CvPoint>& shortstore, int begin, int end, int longvecx, int longvecy)
{
	int shortlinelength = 0;
	for(int i = begin + 1; i < end; i++)
	{
		for(int j = 0; j < begin; j++)
		{
			int shortvecx = 0, shortvecy = 0;
			shortvecx= cv[i].x - cv[j].x;
			shortvecy = cv[i].y - cv[j].y;
			//此处注意设置合适的阈值
			if(abs(shortvecx * longvecx + shortvecy * longvecy) < 15)
			{
				int temp = (int)sqrt(pow(abs(cv[i].x-cv[j].x),2)+pow(abs(cv[i].y-cv[j].y),2));
				if(temp > shortlinelength)
				{
					shortstore.clear();
					shortlinelength = temp;
					shortstore.push_back(cv[i]);
					shortstore.push_back(cv[j]);
				}
			}
		}

		for(unsigned int j = end + 1; j < cv.size(); j++)
		{
			int shortvecx = 0, shortvecy = 0;
			shortvecx= cv[i].x - cv[j].x;
			shortvecy = cv[i].y - cv[j].y;
			if(abs(shortvecx * longvecx + shortvecy * longvecy) < 15)
			{
				int temp = (int)sqrt(pow(abs(cv[i].x-cv[j].x),2)+pow(abs(cv[i].y-cv[j].y),2));
				if(temp > shortlinelength)
				{
					shortstore.clear();
					shortlinelength = temp;
					shortstore.push_back(cv[i]);
					shortstore.push_back(cv[j]);
				}
			}
		}	
	}

	return shortlinelength;
}