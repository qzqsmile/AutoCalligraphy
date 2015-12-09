#include"pixel.h"
#include"stroke.h"
#include<algorithm>

using namespace cv;
using namespace std;  


/*
	for debug
	vector�еĵ�ͨ������������
*/
void DrawOutLine(vector<CvPoint>&stroke, IplImage * img)
{
	CvPoint  pre, next;
	for(unsigned int i = 0; i < stroke.size(); i++)
	{
		if(i == 0)
		{
			pre = next = stroke[i];
			continue;
		}
		next = stroke[i];
		cvLine(img,pre,next,CV_RGB(0,199,155),2,0);
		pre = next;
	}
}

/**�ж����ǲ��Ǹ�����
*/
bool IsHeng(const vector<CvPoint>& strokepoint, const IplImage * img)
{
	CvPoint testpoint = strokepoint[0];
	testpoint.y += 10;
	int res;// = GetPixel(img, &testpoint);
	res = GetPixel(img, &strokepoint[0]);
	
	if(GetPixel(img, &testpoint))
	{
		testpoint.y = strokepoint[0].y-5;
	}
	else
	{
		testpoint.y += 2;
	}
	int pixelcount = 0;
	CvPoint countpoint = testpoint;
	int begin = max(0, testpoint.x-20), end = min(testpoint.x+50, img->width);
	for(int i = begin; i < end; i++)
	{
		countpoint.x = i;
		if(!GetPixel(img, &countpoint))
			pixelcount++;
	}

	//����40��������Ϊ�Ǻ�
	if(pixelcount > 40)
		return true;

	return false;
}
/*
ȷ���ǲ�������
*/
bool Isshu(const vector<CvPoint>& stroke, const IplImage *img)
{
	if(stroke.size() < 4)
	{
		cout << "point number is less than 4" << endl;
	}
	CvPoint testpoint = stroke[4];
	testpoint.x += 5;
	if(GetPixel(img, &testpoint))
		testpoint.x -= 10;
	if(GetPixel(img, &testpoint))
		return false;
	//width ?
	int up = max(0, testpoint.y-80), down = min(testpoint.y+100, img->width);
	int pixelcount = 0;
	CvPoint countpoint = testpoint;
	for(int i = up; i < down; i++)
	{
		countpoint.y = i;
		if(!GetPixel(img, &countpoint))
			pixelcount++;
	}

	if(pixelcount > 100)
		return true;
	return false;
}

/*
��������λ��
*/
void DrawShuMiddle(vector<CvPoint>&stroke, const IplImage *img, IplImage *outimg, vector<CvPoint>& shustroke)
{
	if(stroke.size() < 4)
	{
		cout << "point number is less than 4" << endl;
	}
	CvPoint testpoint = stroke[3];
	testpoint.x += 5;
	if(IsWhite(img, &testpoint))
	{
		testpoint.x -= 10;
	}
	
	CvPoint testup, testdown;
	testup = testdown = testpoint;
	int up = 0, down = INT_MAX;
	up = down = 0;
	//ȷ�����±�
	for(int i = testpoint.y; i >= 0; i--)
	{
		testup.y = i;
		if(!IsWhite(img, &testup))
			up = i;
		else
			break;
	}

	for(int i = testpoint.y; i < (img->height); i++)
	{
		testdown.y = i;
		if(!IsWhite(img, &testdown))
			down = i;
		else
			break;
	}

	int maxleft= INT_MAX, maxright = 0;

	CvPoint predrawpoint, nextdrawpoint;
	
	predrawpoint.y = 10000;
	vector<int> shuwidth;
	int midwidth = 0;
	//Ѱ����λ��
	for(int i = up; i <= down; i++)
	{
		int left = 0, right = 0;
		CvPoint testleft, testright;
		testleft.y = testright.y = i;

		//ȷ�����
		for(int j = testpoint.x; j > 0; j--)
		{
			testleft.x = j;
			if(IsWhite(img, &testleft))
			{
				left = j;
				break;
			}
		}

		//ȷ���ұ�
		for(int j = testpoint.x; j < (img->widthStep); j++)
		{
			testright.x = j;
			if(IsWhite(img, &testright))
			{
				right = j;
				break;
			}
		}

		shuwidth.push_back((right+left)/2);
	}
	//���򣬲���¼��λ�ߵ���ߣ������±��ų�����
	sort(shuwidth.begin(), shuwidth.end());
	midwidth = shuwidth[shuwidth.size()/2];

	//��ʼ����
	for(int i = up; i <= down; i++)
	{
		int left = 0, right = 0;
		CvPoint testleft, testright;
		testleft.y = testright.y = i;
		
		//ȷ�����
		for(int j = testpoint.x; j > 0; j--)
		{
			testleft.x = j;
			if(IsWhite(img, &testleft))
			{
				left = j;
				/*if(j < (testpoint.x-10))
				{
					//up = maxup;
					break;
				}*/
				if(maxleft > left)
					maxleft = left;
				break;
			}
		}
		//ȷ���ұ�
		for(int j = testpoint.x; j < (img->widthStep); j++)
		{
			testright.x = j;
			if(IsWhite(img, &testright))
			{
				right = j;
				/*if(j > (predrawpoint.y+50))
				{
					break ;
				}*/
				if(maxright < right)
					maxright = right;
				break;
			}
		}
		//ave = (up+down) / 2;
	    
		nextdrawpoint.x = (left + right) / 2;
		if((left+right / 2) > (midwidth+20))
			nextdrawpoint.x = midwidth;
		nextdrawpoint.y = i;
		//��һ�β����ߣ���ȷ����һ����ʼ��
		if(predrawpoint.y == 10000)
		{
			predrawpoint = nextdrawpoint;
			continue;
		}
	/*	if((nextdrawpoint.y - predrawpoint.y) > 5)
		{
			//right = predrawpoint.x;
			//break;
			nextdrawpoint.x = i;
			nextdrawpoint.y = predrawpoint.y;
		}*/
		cvLine(outimg,predrawpoint,nextdrawpoint,CV_RGB(0,0,255),1,0);
		predrawpoint = nextdrawpoint;
	}

	//ɾ�����ߵĵ�,�е�����
	vector<CvPoint> noshu;
	for(unsigned int i = 0; i < stroke.size(); i++)
	{
		CvPoint point = stroke[i];
		if(((maxleft) < point.x)&&(maxright > point.x)&&
			(down > point.y)&&(up < point.y))
			shustroke.push_back(point);
		else
			noshu.push_back(point);
	}
	stroke.clear();
	stroke = noshu;
}

/*
�������λ��
*/
void DrawHengMiddle(vector<CvPoint> &stroke, const IplImage *img, IplImage *Outimg, vector<CvPoint>& hengstroke)
{
	CvPoint testpoint = stroke[0];
	testpoint.y += 15;
	
	if(GetPixel(img, &testpoint))
	{
		testpoint.y -= 5;
	}
	int left = 0, right = 0;

	//ȷ�����
	CvPoint testleft, testright;
	testleft.x = testright.x = testpoint.x;
	testleft.y = testright.y = testpoint.y;
	
	for(int i = testpoint.x; i > 0; i--)
	{
		testleft.x = i;
		if(GetPixel(img, &testleft))
		{
			left = i;
			break;
		}
	}
	//ȷ���ұ�
	for(int i = testpoint.x; i < (img->widthStep); i++)
	{
		testright.x = i;
		if(GetPixel(img, &testright))
		{
			right = i;
			break;
		}
	}
	CvPoint testup, testdown, predrawpoint, nextdrawpoint;
	testup.x = testdown.x = testpoint.x;
	testup.y = testdown.y = testpoint.y;
	predrawpoint.x = left;
	predrawpoint.y = 10000;
	int maxup = INT_MAX, mindown = INT_MIN;
	//����λ��
	int ave = INT_MAX/2;
	for(int i = left; i < right; i++)
	{
		int up = 0, down = 0;
		//ȷ��up
		testup.x = testdown.x = i;
		for(int j = testpoint.y; j > 0; j--)
		{
			testup.y = j;
			if(IsWhite(img, &testup))
			{
				up = j;
				if(i < (testpoint.x-10))
				{
					//up = maxup;
					break;
				}
				if(maxup > up)
					maxup = up;
				break;
			}
		}
		//ȷ��down
		for(int j = testpoint.y; j < (img->height); j++)
		{
			testdown.y = j;
			if(IsWhite(img, &testdown))
			{
				down = j;
				if(j > (predrawpoint.y+50))
				{
					break ;
				}
				if(mindown < down)
					mindown = down;
				break;
			}
		}
		//ave = (up+down) / 2;
		nextdrawpoint.x = i;
		nextdrawpoint.y = (up+down)/ 2;
		//��һ�β����ߣ���ȷ����һ����ʼ��
		if(predrawpoint.y == 10000)
		{
			predrawpoint.y = (up+down) /2 ;
			continue;
		}
		if((nextdrawpoint.y - predrawpoint.y) > 5)
		{
			//right = predrawpoint.x;
			//break;
			nextdrawpoint.x = i;
			nextdrawpoint.y = predrawpoint.y;
		}
		cvLine(Outimg,predrawpoint,nextdrawpoint,CV_RGB(0,0,255),1,0);
		predrawpoint = nextdrawpoint;
	}

	//ɾ����������Щ��
	vector<CvPoint> noheng;
	for(unsigned int i = 0; i < stroke.size(); i++)
	{
		CvPoint point = stroke[i];
		if(((left-18) < point.x)&&(right > point.x)&&
			(maxup < point.y)&&(mindown > point.y))
			hengstroke.push_back(point);
		else
			noheng.push_back(point);
	}
	stroke.clear();
	stroke = noheng;
}

/*
�ǲ���Ʋ,��Ʋ�Ļ�����true�����򷵻�false
*/
bool IsPie(const vector<CvPoint>& stroke, const IplImage *img)
{
	;
}

void DrawPieMiddle(vector<CvPoint>&stroke, const IplImage *img, IplImage *outimg, vector<CvPoint>& shustroke)
{
	;
}
/*
�ǲ�����
*/ 
bool IsNa(const vector<CvPoint>&stroke, const IplImage *img)
{
	;
}

void DrawNaMiddle(vector<CvPoint>&stroke, const IplImage *img, IplImage *outimg, vector<CvPoint>& shustroke)
{
	;
}

/*

*/