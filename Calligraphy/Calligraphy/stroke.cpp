#include"pixel.h"
#include"stroke.h"

using namespace cv;
using namespace std;  

//判断它是不是个横线
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

	//大于40像素则认为是横
	if(pixelcount > 40)
		return true;

	return false;
}

bool Isshu(const vector<CvPoint>& stroke, const IplImage *img)
{
	CvPoint testpoint = stroke[0];
	testpoint.x += 5;
	if(!GetPixel(img, &testpoint))
		testpoint.x -= 10;
	if(!GetPixel(img, &testpoint))
		return false;
	int begin = max(0, testpoint.y-30), end = min(testpoint.y+30, img->width);
	int pixelcount = 0;
	CvPoint countpoint = testpoint;
	for(int i = begin; i < end; i++)
	{
		countpoint.y = i;
		if(!GetPixel(img, &countpoint))
			pixelcount++;
	}

	if(pixelcount > 40)
		return true;
	return false;
}

void DrawshuMiddle(vector<CvPoint>&stroke, const IplImage *img, IplImage *outimg, vector<CvPoint>& shustroke)
{

}
//画中位线

void DrawHengMiddle(vector<CvPoint> &stroke, const IplImage *img, IplImage *Outimg, vector<CvPoint>& hengstroke)
{
	CvPoint testpoint = stroke[0];
	testpoint.y += 15;
	
	if(GetPixel(img, &testpoint))
	{
		testpoint.y -= 5;
	}
	int left = 0, right = 0;

	//确定左边
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
	//确定右边
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
	//画中位线
	int ave = INT_MAX/2;
	for(int i = left; i < right; i++)
	{
		int up, down;
		//确定up
		testup.x = testdown.x = i;
		for(int j = testpoint.y; j > 0; j--)
		{
			testup.y = j;
			if(IsWhite(img, &testup))
			{
			//	if(i < (testpoint.x-10))
			//		break;
				up = j;
				if(maxup > up)
					maxup = up;
				break;
			}
		}
		//确定down
		for(int j = testpoint.y; j < (img->height); j++)
		{
			testdown.y = j;
			if(IsWhite(img, &testdown))
			{
				if(j > (predrawpoint.y+50))
					return ;
				down = j;
				if(mindown < down)
					mindown = down;
				break;
			}
		}
		//ave = (up+down) / 2;
		nextdrawpoint.x = i;
		nextdrawpoint.y = (up+down)/ 2;
		//第一次不画线，先确定第一个起始点
		if(predrawpoint.y == 10000)
		{
			predrawpoint.y = (up+down) /2 ;
			continue;
		}
		if((nextdrawpoint.y - predrawpoint.y) > 5)
		{
			right = predrawpoint.x;
			break;
		}
		cvLine(Outimg,predrawpoint,nextdrawpoint,CV_RGB(0,0,255),1,0);
		predrawpoint = nextdrawpoint;
	}

	//删除轮廓里这些点
	vector<CvPoint> notheng;
	for(int i = 0; i < stroke.size(); i++)
	{
		CvPoint point = stroke[i];
		if((left < point.x)&&(right > point.x)&&
			(maxup > point.y)&&(mindown < point.y))
			hengstroke.push_back(point);
		else
			notheng.push_back(point);
	}
	stroke.clear();
	stroke = notheng;
}
