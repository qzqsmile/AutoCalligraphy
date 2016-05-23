#ifndef THIN_H
#define THIN_H

#include <opencv2/opencv.hpp>  

void cvThin(cv::Mat& src, cv::Mat& dst, int intera);
void DrawRedMidLine(Mat src, IplImage* dst);
void DrawBlueMidLine(Mat src, IplImage* dst);


#endif