#include"point.h"
#include<vector>

using namespace std;

/*
���ܣ��ж�����������ǲ���һ���������
���룺pstrokeָ��һ������
���������ǵ����������true�����򷵻�false
*/
bool IsPoint(const CvSeq *pstroke)
{
	if((pstroke->total > 10) && (pstroke->total < 100))
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
	
	for(int i = 0; i < cv.size(); i++)
	{
		for(int j = i; j < cv.size(); j++)
		{
			//�����Ƿ�Ҫ����float?
			int temp = sqrt(pow(abs(cv[i].x-cv[j].x),2)+pow(abs(cv[i].y-cv[j].y),2));
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
	  begin��end�ֱ�������ǳ��������Ŀ�ʼ����������cv�д洢���±�
	  longvecx�ǳ��ߵ�������xֵ��longvecy�ǳ���������yֵ
��������ض̱ߵĳ��ȣ����Ѷ̱߿�ʼ����������shortstore��
*/
int CalPointShortLine(const vector<CvPoint>&cv, vector<CvPoint> & shortstore, int begin, int end, int longvecx, int longvecy)
{
	int shortlinelength = 0;
	for(int i = begin + 1; i < end; i++)
	{
		for(int j = 0; j < begin; j++)
		{
			int shortvecx = 0, shortvecy = 0;
			shortvecx= cv[i].x - cv[j].x;
			shortvecy = cv[i].y - cv[j].y;
			if(abs(shortvecx * longvecx + shortvecy * longvecy) < 10)
			{
				int temp = sqrt(pow(abs(cv[i].x-cv[j].x),2)+pow(abs(cv[i].y-cv[j].y),2));
				if(temp > shortlinelength)
				{
					shortstore.clear();
					shortlinelength = temp;
					shortstore.push_back(cv[i]);
					shortstore.push_back(cv[j]);
				}
			}
		}

		for(int j = end + 1; j < cv.size(); j++)
		{
			int shortvecx = 0, shortvecy = 0;
			shortvecx= cv[i].x - cv[j].x;
			shortvecy = cv[i].y - cv[j].y;
			if(abs(shortvecx * longvecx + shortvecy * longvecy) < 10)
			{
				int temp = sqrt(pow(abs(cv[i].x-cv[j].x),2)+pow(abs(cv[i].y-cv[j].y),2));
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