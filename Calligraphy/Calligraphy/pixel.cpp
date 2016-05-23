#include"pixel.h"

//for debug
/*
	���ܣ�����ͼ������أ�����imgΪҪ���õ�ͼ��pΪҪ���õĵ�
	0ָ���Ǻ�ɫ��
*/
void SetPixel(IplImage *img, const CvPoint *p)
{
	uchar *ptr = (uchar *)img->imageData + ((p->y) * (img->widthStep));
	ptr[p->x] = 0;
}

/*

*/
bool IsWhite(const IplImage *img, const CvPoint *p)
{
	if(GetPixel(img, p))
		return true;
	return false;
}


/*
���ܣ���ȡͼ������أ�����������Ƕ�ֵ�����ͼ��
���룺img����Ҫ��ȡ���ص�ͼ��
	 p ����Ҫ��ȡ���ص������
����������ȡ������ֵ
*/
uchar GetPixel(const IplImage *img, const CvPoint* p)
{
	uchar *ptr = (uchar *)img->imageData + ((p->y) * (img->widthStep));
	return ptr[p->x];
}


void SetPixelOnPic(Mat& pic, vector<CvPoint> points)
{
	for(int i = 0; i < points.size(); i++)
	{
		pic.at<uchar>(points[i].x, points[i].y) = 255;
	}
}


void DrawLineOnMat(Mat& pic, CvPoint p)
{
	for(int i = p.y; i < pic.cols; i++)
	{
		pic.at<uchar>(p.x, i) = 0;
	}
}

void DrawVerticalOnMat(Mat& pic, CvPoint p)
{
	for(int i = p.x; i < pic.rows; i++)
	{
		pic.at<uchar>(i, p.y) = 0;
	}
}