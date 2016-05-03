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
��ˮ����״�����ԱȽ�
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
���ܣ��ж�����������ǲ���һ���������
���룺pstrokeָ��һ������
���������ǵ����������true�����򷵻�false
*/
bool IsPoint(const CvSeq *pstroke)
{
	if((pstroke->total > 10) && (pstroke->total < 100))
	//if(pstroke->total < 100)
		return true;
	return false;
}

/*
���ܣ������ĳ���
���룺cv��洢���ǵ����������꣬store���ص��ǳ��ߵĿ�ʼ�������
��������س��ߵĳ��ȣ����ѳ��ߵĿ�ʼ����������store�С�
*/

int CalPointLongLine(const vector<CvPoint>&cv, vector<CvPoint>& store)
{
	int max = 0;
	
	for(unsigned int i = 0; i < cv.size(); i++)
	{
		for(unsigned int j = i; j < cv.size(); j++)
		{
			//�����Ƿ�Ҫ����float?
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
���ܣ�����������̱߳��ȣ������ؿ�ʼ�������
���룺cv�Ǵ洢�����ĵ꣬shortstore���ص��Ƕ̱ߵĿ�ʼ������㡣
	  begin��end�ֱ������ǳ��������Ŀ�ʼ����������cv�д洢���±�
	  longvecx�ǳ��ߵ�������xֵ��longvecy�ǳ���������yֵ
��������ض̱ߵĳ��ȣ����Ѷ̱߿�ʼ����������shortstore��
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
			//�˴�ע�����ú��ʵ���ֵ
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