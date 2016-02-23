#include"pixel.h"
#include"stroke.h"
#include<algorithm>

using namespace cv;
using namespace std;  

enum{
	PIE_COUNT = 8 
};
enum{
	NA_COUNT = 2
};


/*
在轮廓点钟，寻找离点（x,y）最近的点。并返回其下边
*/
int findneareastpoint(const int x, const int y, const vector<CvPoint>& stroke)
{
	int minindex = 0, min = INT_MAX;
	for(unsigned int i = 0; i < stroke.size(); i++)
	{
		int res;
		res = (int)sqrt(pow((stroke[i].x-x), 2) + pow((stroke[i].y-y),2));
		if(res < min)
		{
			min = res;
			minindex = i;
		}
	}

	return minindex;
}

/*
for debug
vector中的点通过线连接起来
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
/*
确定是哪一个点
*/

void DrawLine(const CvPoint&s, IplImage *out_img)
{
	CvPoint s1 = s;
	s1.y = out_img->height;
	cvLine(out_img, s, s1, CV_RGB(0,199,155),2,0);
}
/**判断它是不是个横线
*/
bool IsHeng(const vector<CvPoint>& strokepoint, const IplImage * img)
{
	//version1 通过划线的方式进行判断,效果一般
	CvPoint testpoint = strokepoint[0];
	testpoint.y += 10;
	int res; // = GetPixel(img, &testpoint);
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
	int begin = max(0, testpoint.x-50), end = min(testpoint.x+50, img->width);
	for(int i = begin; i < end; i++)
	{
		countpoint.x = i;
		if(!GetPixel(img, &countpoint))
			pixelcount++;
	}

	//大于40像素则认为是横
	if(pixelcount > 60)
		return true;

	return false;
}
/*
确定是不是竖线
*/
bool IsShu(const vector<CvPoint>& stroke, const IplImage *img)
{
	//version1
	/*if(stroke.size() < 4)
	{
		cout << "point number is less than 4" << endl;
		return false;
	}
	CvPoint testpoint = stroke[3];
	testpoint.x += 5;
	if(GetPixel(img, &testpoint))
		testpoint.x -= 10;
	if(GetPixel(img, &testpoint))
		return false;
	width ?
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
	return false;*/

	//version2
	if(stroke.size() < 40)
	{
		cout <<"shu  point is too less"<<endl;
		return false;
	}
	CvPoint pre;
	int count = 0;
	//从index下标处开始计算，用以排除干扰
	int xindex = stroke[0].x;
	if(stroke.size() > 20)
		xindex = stroke[20].x;
	for(unsigned int i = 20; i < stroke.size(); i++)
	{
		if(i == 20)
		{
			pre = stroke[i];
			continue;
		}
		if(i > 40)
			return false;
		if(count >= 10)
			return true;
		//x, y是否符合相应的位置关系,这里注意设置合适的阈值
		if(((stroke[i].x <= (xindex+10)) && (stroke[i].x >= (xindex-10))) && (stroke[i].y >= pre.y))
		{
			count++;
		}
		pre = stroke[i];
	}
	return false;
}

/*
画竖的中位线
*/
void DrawShuMiddle(vector<CvPoint>&stroke, const IplImage *img, IplImage *outimg, vector<CvPoint>& shustroke)
{
	if(stroke.size() < 4)
	{
		cout << "point number is less than 4" << endl;
		return ;
	}
	CvPoint testpoint = stroke[10];
	//DrawLine(testpoint, outimg);
	testpoint.x += 10;
	if(IsWhite(img, &testpoint))
	{
		testpoint.x -= 20;
	}
	//DrawLine(testpoint, outimg);
	CvPoint testup, testdown;
	testup = testdown = testpoint;
	int up = 0, down = INT_MAX;
	up = down = 0;
	//确定上下边
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
	vector<int> shuindex;
	int midwidth = 0;
	//寻找中位数
	for(int i = up; i <= down; i++)
	{
		int left = 0, right = 0;
		CvPoint testleft, testright;
		testleft.y = testright.y = i;

		//确定左边
		for(int j = testpoint.x; j > 0; j--)
		{
			testleft.x = j;
			if(IsWhite(img, &testleft))
			{
				left = j;
				break;
			}
		}

		//确定右边
		for(int j = testpoint.x; j < (img->widthStep); j++)
		{
			testright.x = j;
			if(IsWhite(img, &testright))
			{
				right = j;
				break;
			}
		}

		shuwidth.push_back(right-left);
		shuindex.push_back((right+left)/2);
	}
	//排序，并记录中位线的左边，用以下边排除错误
	int shumidindex = 0;
	sort(shuwidth.begin(), shuwidth.end());
	sort(shuindex.begin(), shuindex.end());
	midwidth = shuwidth[shuwidth.size()/2];
	shumidindex = shuindex[shuindex.size()/2];

	//开始画线
	vector<int> leftstore, rightstore;
	for(int i = up; i <= down; i++)
	{
		int left = 0, right = 0;
		CvPoint testleft, testright;
		testleft.y = testright.y = i;

		//确定左边
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
				leftstore.push_back(left);
				if(maxleft > left)
					maxleft = left;
				break;
			}
		}
		//确定右边
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
				rightstore.push_back(right);
				if(maxright < right)
					maxright = right;
				break;
			}
		}
		//ave = (up+down) / 2;

		nextdrawpoint.x = (left + right) / 2;
		//这里要注意调整参数，
		if((right-left) > (midwidth+15))
			nextdrawpoint.x = shumidindex;
		nextdrawpoint.y = i;
		//第一次不画线，先确定第一个起始点
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

	sort(leftstore.begin(), leftstore.end());
	sort(rightstore.begin(), rightstore.end());
	maxleft = leftstore[leftstore.size() / 2]-5;
	maxright = rightstore[rightstore.size() / 2]+5;
	//删除竖线的点,有点问题
	vector<CvPoint> noshu;
	for(unsigned int i = 0; i < stroke.size(); i++)
	{
		CvPoint point = stroke[i];
		if((maxleft < point.x)&&(maxright > point.x)&&
			((down+10) > point.y)&&((up-5) < point.y))
			shustroke.push_back(point);
		else
			noshu.push_back(point);
	}
	CvPoint x;
	x.x = 2 * midwidth - maxright;
	x.y = 1;
	//DrawLine(x,outimg);
	stroke.clear();
	stroke = noshu;
	//DrawOutLine(stroke,outimg);
}

/*
画横的中位线
*/
void DrawHengMiddle(vector<CvPoint> &stroke, const IplImage *img, IplImage *Outimg, vector<CvPoint>& hengstroke)
{
	CvPoint testpoint = stroke[0];
	testpoint.y += 25;

	if(GetPixel(img, &testpoint))
	{
		testpoint.y -= 44;
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

	/*CvPoint testpoint, testleft, testright;
	int left, right;
	left = right = 0;*/

	vector<int> hengwidth;
	vector<int> hengindex;
	int midwidth = 0;
	//寻找中位数
	for(int i = left; i <= right; i++)
	{
		int up = 0, down = 0;
		CvPoint testup, testdown;
		testup.x = testdown.x = i;

		//确定上边
		for(int j = testpoint.y; j > 0; j--)
		{
			testup.y = j;
			if(IsWhite(img, &testup))
			{
				up = j;
				break;
			}
		}

		//确定下边
		for(int j = testpoint.y; j < (img->height); j++)
		{
			testdown.y = j;
			if(IsWhite(img, &testdown))
			{
				down = j;
				break;
			}
		}

		hengwidth.push_back(down-up);
		hengindex.push_back((up+down)/2);
	}
	//排序，并记录中位线的左边，用以下边排除错误
	int hengmidindex = 0;
	sort(hengwidth.begin(), hengwidth.end());
	sort(hengindex.begin(), hengindex.end());
	midwidth = hengwidth[hengwidth.size()/2];
	hengmidindex = hengindex[hengindex.size()/5];
	hengmidindex = hengindex[hengindex.size()/3];

	CvPoint testup, testdown, predrawpoint, nextdrawpoint;
	testup.x = testdown.x = testpoint.x;
	testup.y = testdown.y = testpoint.y;
	predrawpoint.x = left;
	predrawpoint.y = 10000;
	int maxup = INT_MAX, mindown = INT_MIN;
	//画中位线
	int ave = INT_MAX/2;
	//用以控制横线的角度
	int risecount = 0;
	//left从1开始可以排除些干扰
	for(int i = left+1; i < right; i++)
	{
		int up = 0, down = 0;
		
		//确定up
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
		//确定down
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
		//第一次不画线，先确定第一个起始点
		if(predrawpoint.y == 10000)
		{
			predrawpoint.y = (up+down) /2 ;
			continue;
		}
		
		//if((nextdrawpoint.y - predrawpoint.y) > 5)
		//{
		//	//right = predrawpoint.x;
		//	//break;
		//	nextdrawpoint.x = i;
		//	nextdrawpoint.y = predrawpoint.y;
		//}
		if((down-up) > midwidth+5)
		{
			nextdrawpoint.y = predrawpoint.y;
			risecount++;
			//这里主要是为了保持横线的角度
			if(risecount == 6)
			{
				risecount = 0;
				nextdrawpoint.y = predrawpoint.y - 1;	
			}
		}
		if(i >= right)
			nextdrawpoint.y = predrawpoint.y;
		cvLine(Outimg,predrawpoint,nextdrawpoint,CV_RGB(0,0,255),1,0);
		predrawpoint = nextdrawpoint;
	}

	//删除轮廓里这些点
	vector<CvPoint> noheng;
	for(unsigned int i = 0; i < stroke.size(); i++)
	{
		CvPoint point = stroke[i];
		if(((left-5) < point.x)&&(right+10 > point.x)&&
			(maxup-5 < point.y)&&(mindown+5 > point.y))
			hengstroke.push_back(point);
		else
			noheng.push_back(point);
	}
	stroke.clear();
	stroke = noheng;
	//DrawOutLine(stroke, Outimg);
	//DrawLine(stroke[0], Outimg);
}

/*
是不是撇,是撇的话返回true；否则返回false
*/
bool IsPie(const vector<CvPoint>& stroke, const IplImage *img, float *ang)
{
	if(stroke.size() < 40)
	{
		cout << "pie Stroke point is too less" << endl;
		return false;
	}
	vector<float> angle;
	int count = 0;

	for(int i = 0; i < 20; i++)
	{
		float x = 0;
		//防止除0
		if(stroke[i].x == stroke[i+20].x)
			continue;
		x = (float)(stroke[i+20].y - stroke[i].y) / (stroke[i+20].x - stroke[i].x);
		//x = fabs(x);
		if((-0.5 > x) && (x > -3.0))
			count++;
	//	angle.push_back(x);
	}

	if(count > 8)
		return true;
	return false;
	//sort(angle.begin(), angle.end());
	//float midangle = angle[angle.size()/2];
	//*ang = midangle;
	////如果角度在某个范围内的话，就认为其是撇
	//if((0.5 < midangle) && (midangle < 3.0))
	//	return true;
	//return false;
}
/*
画出撇的中位线
*/
void DrawPieMiddle(vector<CvPoint>&stroke, const IplImage *img, IplImage *outimg, vector<CvPoint>& piestroke, float ang)
{
	//确定up
	
	//version 3 利用中位线的角度来判断
	/*bool is_left_true = true;
	int count = 0;
	CvPoint testpoint = stroke[0];
	CvPoint prepoint, nextpoint;
	CvPoint testpoint1, testpoint2;

	testpoint1 = testpoint2 = testpoint;
	testpoint.x = testpoint.x - 3;

	if(IsWhite(img, &testpoint))
	{
		is_left_true = false;
	}

	for(auto i = testpoint.y; i < outimg->height; i++)
	{
		if(is_left_true)
		{
			testpoint1.y = i;
			count++;
			if(count >= 2)
			{
				count = 0;
				testpoint1.x--;
			}
			for(auto j = testpoint1.x; j > 0; j--){
				if(!IsWhite(img, &testpoint1))
				{
					testpoint2 = testpoint1;
					for(auto k = testpoint1.x; k >= 0; k--)
					{
						if(IsWhite(img, &testpoint2)){
							break;
						}
						testpoint2.x--;
					}
					break;
				}
				testpoint1.x--;
			}
			nextpoint.x = (testpoint1.x + testpoint2.x) / 2;
			nextpoint.y = (testpoint1.y + testpoint2.y) / 2;
			

		}
		else
		{
			//for(auto j; ;j--){
			//}
		}
	}*/

	//verison1 通过轮廓的轨迹来判断
	CvPoint testup, testdown, pre;
	
	int count = 0, down_index = 0, begin = 0;

	testup = stroke[0];
	pre = stroke[0];
	//确定开始begin

	for(unsigned int i = 0; i < stroke.size(); i++)
	{
		if((stroke[i].x <= pre.x) && (stroke[i].y >= pre.y))
		{
			pre = stroke[i];
			count++;
		}
		pre = stroke[i];
		if(count >= 1)
		{
			begin = i;
			break;
		}
	}

	count = 0;
	testup = pre = stroke[begin];
	//DrawLine(stroke[begin], outimg);
	//确定down
	for(unsigned int i = begin; i < stroke.size(); i++)
	{
		if((stroke[i].x <= pre.x) && (stroke[i].y >= pre.y))
		{
			pre = stroke[i];
		}
		else
		{
			count++;
			pre = stroke[i];
			//确定up点
			if(count > PIE_COUNT)
			{
				testdown = pre;
				down_index = i;
				break;
			}
		}
	}

	//DrawLine(testdown, outimg);
	//确定撇的方向
	bool is_left_true = true;
	CvPoint test = stroke[3];
	test.x = test.x - 3;
	if(IsWhite(img, &test))
	{
		is_left_true = false;
	}

	CvPoint predrawpoint, nextdrawpoint;
	predrawpoint.x = -1;

	for(int i = begin; i < down_index; i++)
	{
	/*	nextdrawpoint.y = stroke[i].y;
		if(is_left_true)
		{
			for(int j = stroke[i].x-3; j > 0; j--)
			{
				nextdrawpoint.x = j;
				if(IsWhite(img, &nextdrawpoint))
				{
					nextdrawpoint.x = (j + stroke[i].x) / 2;
					if(predrawpoint.x == -1)
					{
						predrawpoint = nextdrawpoint;
					}
					break;
				}
			}
		}
		else
		{
			for(int j = stroke[i].x+1; j < img->widthStep; j++)
			{
				nextdrawpoint.x = j;
				if(IsWhite(img, &nextdrawpoint))
				{
					nextdrawpoint.x = (j + stroke[i].x) / 2;
					if(predrawpoint.x == -1)
					{
						predrawpoint = nextdrawpoint;
					}
					break;
				}
			}
		}
*/
		nextdrawpoint.x = stroke[i].x;
		//通过这个参数确定轮廓的开始点是上边还是下边
		bool is_up = true;
		CvPoint testpoint = stroke[0];
		testpoint.y = testpoint.y + 5;
		if(IsWhite(outimg, &testpoint))
		{
			is_up = false;
		}
		int minindex = 0 ;
		//从3开始可以去除一些干扰
		if(is_up)
		{
			for(int j = stroke[i].y+3; j < img->height; j++)
			{
				nextdrawpoint.y = j;
				if(IsWhite(img, &nextdrawpoint))
				{
					nextdrawpoint.y = (j+stroke[i].y) / 2;
					if(predrawpoint.x == -1)
					{
						predrawpoint = nextdrawpoint;
						//这里要求捺不能有相交点
						minindex = findneareastpoint(nextdrawpoint.x, j, stroke);
					}
					break;
				}
			}
		}
		else
		{
			for(int j = stroke[i].y-3; j > 0; j--)
			{
				nextdrawpoint.y = j;
				if(IsWhite(img, &nextdrawpoint))
				{
					nextdrawpoint.y = (j + stroke[i].y) / 2;
					if(predrawpoint.x == -1)
					{
						predrawpoint = nextdrawpoint;
						minindex = findneareastpoint(nextdrawpoint.x, j, stroke);
					}
					break;
				}
			}
		}

		cvLine(outimg,predrawpoint,nextdrawpoint,CV_RGB(0,0,255),1,0);
		predrawpoint = nextdrawpoint;
	}
	//去掉相应的点

	/*vector<CvPoint> nopie;
	for(unsigned int i = 0; i < stroke.size(); i++)
	{
		CvPoint point = stroke[i];
		if((point.y >= stroke[0].y)  && (point.y <= stroke[down_index-1].y))
			piestroke.push_back(point);
		else
			nopie.push_back(point);
	}
	stroke.clear();
	stroke = nopie;*/

	//去除相应的点
	vector<CvPoint> pie;
	vector<CvPoint> nopie;
	for(unsigned int i = 0; i < stroke.size(); i++)
	{
		CvPoint point = stroke[i];
		if((i > begin) && (i < down_index))
			pie.push_back(point);
		else
			nopie.push_back(point);
	}
	stroke.clear();
	stroke = nopie;

	//version 2 根据角度来，与其它方法相似，效果较差
	//if(stroke.size() < 4)
	//{
	//	cout << "stroke point number is less than 4" << endl;
	//}
	//CvPoint testpoint = stroke[3];
	//CvPoint testpoint1 = testpoint;
	//
	////DrawLine(testpoint,outimg);
	//testpoint.x += 5;
	//if(IsWhite(img, &testpoint))
	//{
	//	testpoint.x -= 10;
	//}
	//
	//CvPoint testup, testdown;
	//testup = testdown = testpoint;
	//int up = 0, down = INT_MAX;
	//up = down = 0;
	////确定上下边
	//int ang_int = (int) ang;
	//for(int i = testpoint.y; i >= 0; i--)
	//{
	//	testup.y = i;
	//	testup.x = testup.x + ang_int;
	//	if(!IsWhite(img, &testup))
	//		up = i;
	//	else
	//		break;
	//}
	//DrawLine(testup, outimg);

	//for(int i = testpoint.y; i < (img->height); i++)
	//{
	//	testdown.y = i;
	//	testdown.x = testdown.x - ang_int+3;
	//	if(!IsWhite(img, &testdown))
	//		down = i;
	//	else
	//		break;
	//}
	//DrawLine(testdown, outimg);
	//int maxleft= INT_MAX, maxright = 0;

	//CvPoint predrawpoint, nextdrawpoint;
	//
	//predrawpoint.y = 10000;
	//vector<int> piewidth;
	//int midwidth = 0;
	////寻找中位数
	///*
	//for(int i = up; i <= down; i++)
	//{
	//	int left = 0, right = 0;
	//	CvPoint testleft, testright;
	//	testleft.y = testright.y = i;

	//	//确定左边
	//	for(int j = testpoint.x; j > 0; j--)
	//	{
	//		testleft.x = j;
	//		if(IsWhite(img, &testleft))
	//		{
	//			left = j;
	//			break;
	//		}
	//	}

	//	//确定右边
	//	for(int j = testpoint.x; j < (img->widthStep); j++)
	//	{
	//		testright.x = j;
	//		if(IsWhite(img, &testright))
	//		{
	//			right = j;
	//			break;
	//		}
	//	}

	//	piewidth.push_back((right+left)/2);
	//}
	////排序，并记录中位线的左边，用以下边排除错误
	//sort(piewidth.begin(), piewidth.end());
	//midwidth = piewidth[piewidth.size()/2];*/

	////开始画线
	//for(int i = up; i <= down; i++)
	//{
	//	int left = 0, right = 0;
	//	CvPoint testleft, testright;
	//	testleft.y = testright.y = i;
	//	
	//	//确定左边
	//	for(int j = testpoint.x; j > 0; j--)
	//	{
	//		testleft.x = j;
	//		if(IsWhite(img, &testleft))
	//		{
	//			left = j;
	//			/*if(j < (testpoint.x-10))
	//			{
	//				//up = maxup;
	//				break;
	//			}*/
	//			if(maxleft > left)
	//				maxleft = left;
	//			break;
	//		}
	//	}
	//	//确定右边
	//	for(int j = testpoint.x; j < (img->widthStep); j++)
	//	{
	//		testright.x = j;
	//		if(IsWhite(img, &testright))
	//		{
	//			right = j;
	//			/*if(j > (predrawpoint.y+50))
	//			{
	//				break ;
	//			}*/
	//			if(maxright < right)
	//				maxright = right;
	//			break;
	//		}
	//	}
	//	//ave = (up+down) / 2;
	//    
	//	nextdrawpoint.x = (left + right) / 2;
	//	//if((left+right / 2) > (midwidth+20))
	//	//	nextdrawpoint.x = midwidth;
	//	nextdrawpoint.y = i;
	//	//第一次不画线，先确定第一个起始点
	//	if(predrawpoint.y == 10000)
	//	{
	//		predrawpoint = nextdrawpoint;
	//		continue;
	//	}
	///*	if((nextdrawpoint.y - predrawpoint.y) > 5)
	//	{
	//		//right = predrawpoint.x;
	//		//break;
	//		nextdrawpoint.x = i;
	//		nextdrawpoint.y = predrawpoint.y;
	//	}*/
	//	cvLine(outimg,predrawpoint,nextdrawpoint,CV_RGB(0,0,255),1,0);
	//	predrawpoint = nextdrawpoint;
	//}

	///*删除撇的点,有点问题
	//vector<CvPoint> noshu;
	//for(unsigned int i = 0; i < stroke.size(); i++)
	//{
	//	CvPoint point = stroke[i];
	//	if(((maxleft) < point.x)&&(maxright > point.x)&&
	//		(down > point.y)&&(up < point.y))
	//		shustroke.push_back(point);
	//	else
	//		noshu.push_back(point);
	//}
	//stroke.clear();
	//stroke = noshu;*/
}
/*
是不是捺
*/ 
bool IsNa(const vector<CvPoint>&stroke, const IplImage *img)
{
	/*CvPoint pre;
	int count = 0;
	for(unsigned int i = 0; i < stroke.size(); i++)
	{
		if(i == 0)
		{
			pre = stroke[i];
			continue;
		}
		if(i > 15)
			return false;
		if(count >= 7)
			return true;
		if((stroke[i].x >= pre.x) && (stroke[i].y >= pre.y))
		{
			count++;
		}
		pre = stroke[i];
	}
	return false;*/
	if(stroke.size() < 40)
	{
		cout << "na Stroke point is too less" << endl;
		return false;
	}
	vector<float> angle;
	int count = 0;

	for(int i = 0; i < 20; i++)
	{
		float x = 0;
		//防止除0
		if(stroke[i].x == stroke[i+20].x)
			continue;
		x = (float)(stroke[i+20].y - stroke[i].y) / (stroke[i+20].x - stroke[i].x);
		//x = fabs(x);
		if((0.5 < x) && (x < 3.0))
			count++;
	//	angle.push_back(x);
	}

	if(count > 15)
		return true;
	return false;
	//sort(angle.begin(), angle.end());
	//float midangle = angle[angle.size()/2];
	//*ang = midangle;
	////如果角度在某个范围内的话，就认为其是撇
	//if((0.5 < midangle) && (midangle < 3.0))
	//	return true;
	//return false;
}

/*
 画捺的中位线
*/
void DrawNaMiddle(vector<CvPoint>&stroke, const IplImage *img, IplImage *outimg, vector<CvPoint>& nastroke)
{
	CvPoint testup, testdown, pre;
	
	int count = 0, down_index = 0, begin = 0;

	testup = stroke[0];
	pre = stroke[0];
	//确定开始begin

	for(unsigned int i = 0; i < stroke.size(); i++)
	{
		if((stroke[i].x >= pre.x) && (stroke[i].y >= pre.y))
		{
			pre = stroke[i];
			count ++;
		}
		pre = stroke[i];
		if(count >= 1)
		{
			begin = i;
			break;
		}
		
	}

	count = 0;
	testup = pre = stroke[begin];
//	DrawLine(stroke[0], outimg);
	//确定down
	for(unsigned int i = begin; i < stroke.size(); i++)
	{
		if((stroke[i].x >= pre.x) && (stroke[i].y >= pre.y))
		{
			pre = stroke[i];
		}
		else
		{
			count++;
			pre = stroke[i];
			//确定down点
			if(count > NA_COUNT)
			{
				testdown = pre;
				down_index = i;
				break;
			}
		}
	}
	//DrawLine(stroke[begin], outimg);
	//DrawLine(stroke[down_index],outimg);
	//DrawLine(testdown, outimg);
	//确定捺的方向
	bool is_left_true = true;
	CvPoint test = stroke[3];
	test.x = test.x - 3;
	if(IsWhite(img, &test))
	{
		is_left_true = false;
	}

	CvPoint predrawpoint, nextdrawpoint;
	vector<CvPoint> d;
	predrawpoint.x = -1;

	int minindex = 0;
	for(int i = begin; i < down_index; i++)
	{
		/*nextdrawpoint.y = stroke[i].y;
		if(is_left_true)
		{
			for(int j = stroke[i].x-8; j > 0; j--)
			{
				nextdrawpoint.x = j;
				if(IsWhite(img, &nextdrawpoint))
				{
					nextdrawpoint.x = (j + stroke[i].x) / 2;
					if(predrawpoint.x == -1)
					{
						predrawpoint = nextdrawpoint;
					}
					break;
				}
			}
		}
		else
		{
			for(int j = stroke[i].x+1; j < img->widthStep; j++)
			{
				nextdrawpoint.x = j;
				if(IsWhite(img, &nextdrawpoint))
				{
					nextdrawpoint.x = (j + stroke[i].x) / 2;
					if(predrawpoint.x == -1)
					{
						predrawpoint = nextdrawpoint;
					}
					break;
				}
			}
		}

		d.push_back(predrawpoint);*/

		nextdrawpoint.x = stroke[i].x;
		//从3开始可以去除一些干扰
		for(int j = stroke[i].y+3; j < img->height; j++)
		{
			nextdrawpoint.y = j;
			if(IsWhite(img, &nextdrawpoint))
			{
				nextdrawpoint.y = (j+stroke[i].y) / 2;
				if(predrawpoint.x == -1)
				{
					predrawpoint = nextdrawpoint;
					//这里要求捺不能有相交点
					minindex = findneareastpoint(nextdrawpoint.x, j, stroke);
					//DrawLine(nextdrawpoint, outimg);
					//DrawLine(stroke[minindex], outimg);
				}
				break;
			}
		}

		cvLine(outimg,predrawpoint,nextdrawpoint,CV_RGB(0,0,255),1,0);
		predrawpoint = nextdrawpoint;
	}

	//去掉相应的点
	vector<CvPoint> nona;
	for(unsigned int i = 0; i < stroke.size(); i++)
	{
	/*	CvPoint point = stroke[i];
		if((point.y >= stroke[0].y)  && (point.y <= stroke[down_index-1].y))
			nastroke.push_back(point);
		else
			nona.push_back(point);*/
		if((i >= begin && i <= down_index) || (i >= down_index && i <= minindex))
			nastroke.push_back(stroke[i]);
		else
			nona.push_back(stroke[i]);
	}
	stroke.clear();
	stroke = nona;
	DrawOutLine(stroke, outimg);
	//DrawLine(stroke[0], outimg);
}

/*
判断其是不是个钩
*/
bool IsGou(const vector<CvPoint>&stroke, const IplImage* img)
{
	if(stroke.size() > 30)
		return false;
	else
	{
		CvPoint pre;
		int count = 0;
		for(unsigned int i = 0; i < stroke.size(); i++)
		{
			if(i == 0)
			{
				pre = stroke[i];
				continue;
			}
			if(i > 10)
				return false;
			if(count >= 5)
				return true;
			if((stroke[i].x <= pre.x) && (stroke[i].y <= pre.y))
			{
				count++;
			}
			pre = stroke[i];
		}
		return false;
	}
}