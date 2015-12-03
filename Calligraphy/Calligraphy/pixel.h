/*
��ģ���װ��һЩ�����صĲ���
*/

#ifndef PIXEL_H
#define PIXEL_H
#include <opencv2/opencv.hpp>  

using namespace cv;

void SetPixel(IplImage *img, const CvPoint *p);
bool IsWhite(const IplImage *img, const CvPoint *p);
uchar GetPixel(const IplImage *img, const CvPoint* p);

#endif