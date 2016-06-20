#include <opencv2/opencv.hpp>  
#include "pixel.h"

using cv::Mat;

void cvThin(cv::Mat& src, cv::Mat& dst, int intera)
    {
    if(src.type()!=CV_8UC1)
        {
        printf("只能处理二值或灰度图像\n");
        return;
        }
    //非原地操作时候，copy src到dst
    if(dst.data!=src.data)
        {
        src.copyTo(dst);
        }

    int i, j, n;
    int width, height;
    width = src.cols -1;
    //之所以减1，是方便处理8邻域，防止越界
    height = src.rows -1;
    int step = src.step;
    int  p2,p3,p4,p5,p6,p7,p8,p9;
    uchar* img;
    bool ifEnd;
    int A1;
    cv::Mat tmpimg;
    //n表示迭代次数
    for(n = 0; n<intera; n++)
        {
        dst.copyTo(tmpimg);
        ifEnd = false;
        img = tmpimg.data;
        for(i = 1; i < height; i++)
            {
            img += step;
            for(j =1; j<width; j++)
                {
                uchar* p = img + j;
                A1 = 0;
                if( p[0] > 0)
                    {
                    if(p[-step]==0&&p[-step+1]>0) //p2,p3 01模式
                        {
                        A1++;
                        }
                    if(p[-step+1]==0&&p[1]>0) //p3,p4 01模式
                        {
                        A1++;
                        }
                    if(p[1]==0&&p[step+1]>0) //p4,p5 01模式
                        {
                        A1++;
                        }
                    if(p[step+1]==0&&p[step]>0) //p5,p6 01模式
                        {
                        A1++;
                        }
                    if(p[step]==0&&p[step-1]>0) //p6,p7 01模式
                        {
                        A1++;
                        }
                    if(p[step-1]==0&&p[-1]>0) //p7,p8 01模式
                        {
                        A1++;
                        }
                    if(p[-1]==0&&p[-step-1]>0) //p8,p9 01模式
                        {
                        A1++;
                        }
                    if(p[-step-1]==0&&p[-step]>0) //p9,p2 01模式
                        {
                        A1++;
                        }
                    p2 = p[-step]>0?1:0;
                    p3 = p[-step+1]>0?1:0;
                    p4 = p[1]>0?1:0;
                    p5 = p[step+1]>0?1:0;
                    p6 = p[step]>0?1:0;
                    p7 = p[step-1]>0?1:0;
                    p8 = p[-1]>0?1:0;
                    p9 = p[-step-1]>0?1:0;
                    if((p2+p3+p4+p5+p6+p7+p8+p9)>1 && (p2+p3+p4+p5+p6+p7+p8+p9)<7  &&  A1==1)
                        {
                        if((p2==0||p4==0||p6==0)&&(p4==0||p6==0||p8==0)) //p2*p4*p6=0 && p4*p6*p8==0
                            {
                            dst.at<uchar>(i,j) = 0; //满足删除条件，设置当前像素为0
                            ifEnd = true;
                            }
                        }
                    }
                }
            }
        
        dst.copyTo(tmpimg);
        img = tmpimg.data;
        for(i = 1; i < height; i++)
            {
            img += step;
            for(j =1; j<width; j++)
                {
                A1 = 0;
                uchar* p = img + j;
                if( p[0] > 0)
                    {
                    if(p[-step]==0&&p[-step+1]>0) //p2,p3 01模式
                        {
                        A1++;
                        }
                    if(p[-step+1]==0&&p[1]>0) //p3,p4 01模式
                        {
                        A1++;
                        }
                    if(p[1]==0&&p[step+1]>0) //p4,p5 01模式
                        {
                        A1++;
                        }
                    if(p[step+1]==0&&p[step]>0) //p5,p6 01模式
                        {
                        A1++;
                        }
                    if(p[step]==0&&p[step-1]>0) //p6,p7 01模式
                        {
                        A1++;
                        }
                    if(p[step-1]==0&&p[-1]>0) //p7,p8 01模式
                        {
                        A1++;
                        }
                    if(p[-1]==0&&p[-step-1]>0) //p8,p9 01模式
                        {
                        A1++;
                        }
                    if(p[-step-1]==0&&p[-step]>0) //p9,p2 01模式
                        {
                        A1++;
                        }
                    p2 = p[-step]>0?1:0;
                    p3 = p[-step+1]>0?1:0;
                    p4 = p[1]>0?1:0;
                    p5 = p[step+1]>0?1:0;
                    p6 = p[step]>0?1:0;
                    p7 = p[step-1]>0?1:0;
                    p8 = p[-1]>0?1:0;
                    p9 = p[-step-1]>0?1:0;
                    if((p2+p3+p4+p5+p6+p7+p8+p9)>1 && (p2+p3+p4+p5+p6+p7+p8+p9)<7  &&  A1==1)
                        {
                        if((p2==0||p4==0||p8==0)&&(p2==0||p6==0||p8==0)) //p2*p4*p8=0 && p2*p6*p8==0
                            {
                            dst.at<uchar>(i,j) = 0; //满足删除条件，设置当前像素为0
                            ifEnd = true;
                            }
                        }
                    }
                }
            }

        //如果两个子迭代已经没有可以细化的像素了，则退出迭代
        if(!ifEnd) break;
        }

    }

	
void DrawRedMidLine(Mat src, IplImage* dst)
{
	int height = min(src.rows, dst->height);
	int width = min(src.cols, dst->width);
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			if(src.at<uchar>(i, j ) == 0)
			{
				CvScalar cs = cvGet2D(dst, i, j);
				cs.val[0] = 255;
				cs.val[1] = 0;
				cs.val[2] = 0;
				cvSet2D(dst, i, j, cs);
			}
		}
	}
}

void DrawBlueMidLine(Mat src, IplImage* dst)
{
	int height = min(src.rows, dst->height);
	int width = min(src.cols, dst->width);
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			if((src.at<uchar>(i, j) == 0) && (j > 15))
			{
				CvScalar cs = cvGet2D(dst, i, j-12);
				cs.val[0] = 255;
				cs.val[1] = 0;
				cs.val[2] = 0;
				cvSet2D(dst, i, j-8, cs);
			}
		}
	}
}
void thin(IplImage *src, IplImage *dst, int iter)
{
	int step = src->widthStep;

	for(int n = 0; n < iter; n++)
	{
		int A1 = 0;
		bool ifend = true;
		int p2, p3, p4, p5, p6, p7, p8, p9;

		//cvNamedWindow("二值化图", CV_WINDOW_AUTOSIZE);
		//cvShowImage("二值化图", dst);
	
		//cvWaitKey(0);

		for(int i = 1; i < src->height; i++)
		{
			IplImage *tmp_img = cvCloneImage(dst);

			uchar *img = (uchar *)tmp_img->imageData;
			img += step;

			for(int j = 1; j < src->widthStep; j++)
			{
				uchar *p = img + j;
				A1 = 0;

				if( p[0] < 255)
				{
					if(p[-step]==255&&p[-step+1]<255) //p2,p3 01模式
					{
						A1++;
					}
					if(p[-step+1]==255&&p[1]<255) //p3,p4 01模式
					{
						A1++;
					}
					if(p[1]==255&&p[step+1]<255) //p4,p5 01模式
					{
						A1++;
					}
					if(p[step+1]==255&&p[step]<255) //p5,p6 01模式
					{
						A1++;
					}
					if(p[step]==255&&p[step-1]<255) //p6,p7 01模式
					{
						A1++;
					}
					if(p[step-1]==255&&p[-1]<255) //p7,p8 01模式
					{
						A1++;
					}
					if(p[-1]==255&&p[-step-1]<255) //p8,p9 01模式
					{
						A1++;
					}
					if(p[-step-1]==255&&p[-step]<255) //p9,p2 01模式
					{
						A1++;
					}
					p2 = p[-step]<255?1:0;
					p3 = p[-step+1]<255?1:0;
					p4 = p[1]<255?1:0;
					p5 = p[step+1]<255?1:0;
					p6 = p[step]<255?1:0;
					p7 = p[step-1]<255?1:0;
					p8 = p[-1]<255?1:0;
					p9 = p[-step-1]<255?1:0;
					if((p2+p3+p4+p5+p6+p7+p8+p9)>1 && (p2+p3+p4+p5+p6+p7+p8+p9)<7  &&  (A1==1))
					{
						if((p2==0||p4==0||p6==0)&&(p4==0||p6==0||p8==0)) //p2*p4*p6=0 && p4*p6*p8==0
						{
							 //满足删除条件，设置当前像素为0
							uchar *p1 = (uchar *)dst->imageData + (i * (dst->widthStep));
							p1[j] = 255;
							ifend = false;
						}
					}
				}
			}
		}

		if(ifend)
			return ;
	}
}
