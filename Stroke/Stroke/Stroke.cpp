#include <opencv2/opencv.hpp>  
#include<vector>
#include<iostream>
#include<cmath>

using namespace cv;
using namespace std;  

uchar GetPixel(const IplImage *img, const CvPoint* p);
bool IsPoint(const CvSeq *pstroke);
int CalPointLongLine(const vector<CvPoint>&cv, vector<CvPoint>& store);
int CalPointShortLine(const vector<CvPoint>&cv, vector<CvPoint> & shortstore, int begin, int end, int longvecx, int longvecy);
bool IsHeng(const vector<CvPoint>& strokepoint, const IplImage * img);
void SetPixel(IplImage *img, const CvPoint *p);
void DrawHengMiddle(vector<CvPoint> &stroke,const IplImage *img, IplImage * outimg, vector<CvPoint>& hengstroke);

class Point
{
	private:
		int maxlength;
		int shrotlength;
		int x, y;
		vector<CvPoint> points;
		CvPoint longbegin;
		CvPoint shortbegin;
		CvPoint longend;
		CvPoint shortend;
		double area;
	public:
		Point();
		~Point();
};

class Stroke
{
	private:
	public:
};

class Word
{
	private:
	public:
};

int main( int argc, char** argv )  
{     
	const char* imagename = "C:\\Users\\Q\\Desktop\\黑白.jpg";
 
    //从文件中读入图像
	IplImage *img = cvLoadImage(imagename,CV_LOAD_IMAGE_UNCHANGED);
	IplImage *gray_img = cvCreateImage(cvGetSize(img),IPL_DEPTH_8U, 1);
	cvCvtColor(img, gray_img, CV_BGR2GRAY);
	//二值化图
	IplImage *bin_img = cvCreateImage(cvGetSize(gray_img), IPL_DEPTH_8U, 1);
	cvThreshold(gray_img, bin_img, 100, 255, CV_THRESH_BINARY);
	//复制一份留作轮廓处理
	IplImage *bin_copy_image = cvCloneImage(bin_img);
 
	/*CvScalar cs;                            //声明像素变量  
	for(int i = 0; i < img->height; i++)  
		for (int j = 0; j < img->width; j++)  
		{  
			cs = cvGet2D(img, i, j);   //获取像素  
			cs.val[0] = 255;             //对指定像素的RGB值进行重新设定  
			//cs.val[1] = 255;  
			//cs.val[2] = 255;  
			cvSet2D(img, i, j, cs);    //将改变的像素保存到图片中  
		}*/
	/*int width = bin_img->width;
	int height = bin_img->height;

	//读取像素值
	/*for(size_t row=0; row < height; row++)
	{
		uchar *ptr = (uchar *)bin_img->imageData+row*bin_img->widthStep;
		for(size_t col = 0; col < width; col++)
		{
			int intensity = ptr[col];
			cout << intensity << " ";
		}
		cout << endl;
	}*/
	//检测轮廓并返回轮廓个数
	CvMemStorage *pcvMstorage = cvCreateMemStorage();
	CvSeq *pcvSeq = NULL;
	//这里会修改二值化图像
	cvFindContours(bin_copy_image, pcvMstorage, &pcvSeq, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0)); 
	//画轮廓图
	IplImage *pOutlineImage = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
	int nlevels = 3;
	cvRectangle(pOutlineImage, cvPoint(0, 0), cvPoint(pOutlineImage->width, pOutlineImage->height), CV_RGB(255, 255, 255), CV_FILLED);  
	cvDrawContours(pOutlineImage, pcvSeq, CV_RGB(255,0,0), CV_RGB(0,255,0), nlevels, 2);
	
	//获取轮廓的像素坐标
	CvPoint *pPoint = NULL;
	int count = 0;
	for(; pcvSeq != NULL; pcvSeq = pcvSeq->h_next)
	{
		count++;
		//uchar res = 0;
		//取轮廓中一个点
		pPoint = (CvPoint *)cvGetSeqElem(pcvSeq, 0);

		if((pPoint->x >= 5) && (pPoint->y >= 5) && (pPoint->x < ((bin_img->width)-10)) && (pPoint->y < ((bin_img->height)-10)))
		{
			if(IsPoint(pcvSeq))
			{
				//printf("this is a Point! count = %d\n", count);
				//CvPoint *cv = NULL;
				vector<CvPoint>pointstroke;
				vector<CvPoint>longlengthpoint;
				vector<CvPoint>shortlengthpoint;
				int maxlength = 0;
				int shortlength = 0;
				double pointaera = 0;
				for(int i = 0; i < pcvSeq->total; i++)
				{
					pPoint = (CvPoint *)cvGetSeqElem(pcvSeq, i);
					pointstroke.push_back(*pPoint);
				}
				maxlength = CalPointLongLine(pointstroke, longlengthpoint);
				cvLine(pOutlineImage,longlengthpoint[0],longlengthpoint[1],CV_RGB(0,0,255),1,0);

				//确定开始点与结束点
				int begin, end;
				begin = end = 0 ;

				for(int i = 0; i < pointstroke.size(); i++)
				{
					if((pointstroke[i].x == longlengthpoint[0].x) && (pointstroke[i].y == longlengthpoint[0].y))
						begin = i;
					if((pointstroke[i].x == longlengthpoint[1].x) && (pointstroke[i].y == longlengthpoint[1].y))
						end = i;
				}
				if(begin > end)
					swap(begin, end);
				//计算雨滴短边
				int longvecx, longvecy;
				longvecx = longlengthpoint[0].x - longlengthpoint[1].x;
				longvecy = longlengthpoint[0].y - longlengthpoint[1].y;
				shortlength = CalPointShortLine(pointstroke,shortlengthpoint,begin, end,longvecx, longvecy);
				cvLine(pOutlineImage,shortlengthpoint[0],shortlengthpoint[1],CV_RGB(0,0,255),1,0);
				//计算面积
				//pointaera = fabs(cvContourArea(pcvSeq,CV_WHOLE_SEQ));
			}
			else 
			{
				vector<CvPoint> strokepoint;
				vector<CvPoint> hengstroke;
				for(int i = 0; i < pcvSeq->total; i++)
				{
					pPoint = (CvPoint *)cvGetSeqElem(pcvSeq, i);
					strokepoint.push_back(*pPoint);
				}
				//if(count == 3)
					//cvLine(pOutlineImage,strokepoint[0],strokepoint[50],CV_RGB(0,0,255),1,0);
				if(IsHeng(strokepoint, bin_img))
				{
					DrawHengMiddle(strokepoint,bin_img,pOutlineImage,hengstroke);
					//cout << count << endl;
					//cout << "is heng" << endl;
				}

/*				if(Isshu(strokepoint, bin_img))
				{

				}*/

			}
			printf("count = %d\n", count);
		}
		/*
		for(int i = 0; i < pcvSeq->total; i++)
		{
			pPoint = (CvPoint *)cvGetSeqElem(pcvSeq, i);
			res = GetPixel(bin_img, pPoint);
		}*/
	}
	//assert(count != 2);
	CvPoint p;
	p.x = 179;
	p.y = 110;
	uchar res1 = GetPixel(bin_img, &p);
	//SetPixel(bin_img,&p);

	//显示轮廓图
	cvNamedWindow("轮廓图", CV_WINDOW_AUTOSIZE);
	cvShowImage("轮廓图", pOutlineImage);
	cvReleaseMemStorage(&pcvMstorage);

	cvNamedWindow("原图", CV_WINDOW_AUTOSIZE);
	cvShowImage("原图",img);
	cvNamedWindow("二值化",CV_WINDOW_AUTOSIZE);
	cvShowImage("二值化",bin_img);
    cvWaitKey(0);
    cvReleaseImage(&img);
	cvReleaseImage(&gray_img);
	cvReleaseImage(&bin_img);
	cvReleaseImage(&pOutlineImage);
	
    cvDestroyWindow("原图");
	cvDestroyWindow("二值化");
	cvDestroyWindow("轮廓图");

    return 0;  
}

//for debug
void SetPixel(IplImage *img, const CvPoint *p)
{
	uchar *ptr = (uchar *)img->imageData + ((p->y) * (img->widthStep));
	ptr[p->x] = 0;
}
bool IsWhite(const IplImage *img, const CvPoint *p)
{
	if(GetPixel(img, p))
		return true;
	return false;
}

/*
功能：获取图像的像素，这里的输入是二值化后的图像
输入：img是需要获取像素的图像
	 p 是所要获取像素的坐标点
输出：输出获取的像素值
*/
uchar GetPixel(const IplImage *img, const CvPoint* p)
{
	uchar *ptr = (uchar *)img->imageData + ((p->y) * (img->widthStep));
	return ptr[p->x];
}

/*
功能：判断输入的轮廓是不是一个点的轮廓
输入：pstroke指向一个轮廓
输出：如果是点的轮廓返回true，否则返回false
*/
bool IsPoint(const CvSeq *pstroke)
{
	if((pstroke->total > 10) && (pstroke->total < 100))
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
	
	for(int i = 0; i < cv.size(); i++)
	{
		for(int j = i; j < cv.size(); j++)
		{
			//这里是否要改用float?
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
功能：计算点轮廓短边长度，并返回开始与结束点
输入：cv是存储轮廓的店，shortstore返回的是短边的开始与结束点。
	  begin与end分别代表的是长边轮廓的开始与结束点的在cv中存储的下边
	  longvecx是长边的向量的x值，longvecy是长边向量的y值
输出：返回短边的长度，并把短边开始与结束点存入shortstore中
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
