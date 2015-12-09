#include"pixel.h"

//for debug
/*
	功能：设置图像的像素，输入img为要设置的图像，p为要设置的点
	0指的是黑色的
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