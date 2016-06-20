#include <opencv2/opencv.hpp>  
#include "pixel.h"

using cv::Mat;

void cvThin(cv::Mat& src, cv::Mat& dst, int intera)
    {
    if(src.type()!=CV_8UC1)
        {
        printf("ֻ�ܴ����ֵ��Ҷ�ͼ��\n");
        return;
        }
    //��ԭ�ز���ʱ��copy src��dst
    if(dst.data!=src.data)
        {
        src.copyTo(dst);
        }

    int i, j, n;
    int width, height;
    width = src.cols -1;
    //֮���Լ�1���Ƿ��㴦��8���򣬷�ֹԽ��
    height = src.rows -1;
    int step = src.step;
    int  p2,p3,p4,p5,p6,p7,p8,p9;
    uchar* img;
    bool ifEnd;
    int A1;
    cv::Mat tmpimg;
    //n��ʾ��������
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
                    if(p[-step]==0&&p[-step+1]>0) //p2,p3 01ģʽ
                        {
                        A1++;
                        }
                    if(p[-step+1]==0&&p[1]>0) //p3,p4 01ģʽ
                        {
                        A1++;
                        }
                    if(p[1]==0&&p[step+1]>0) //p4,p5 01ģʽ
                        {
                        A1++;
                        }
                    if(p[step+1]==0&&p[step]>0) //p5,p6 01ģʽ
                        {
                        A1++;
                        }
                    if(p[step]==0&&p[step-1]>0) //p6,p7 01ģʽ
                        {
                        A1++;
                        }
                    if(p[step-1]==0&&p[-1]>0) //p7,p8 01ģʽ
                        {
                        A1++;
                        }
                    if(p[-1]==0&&p[-step-1]>0) //p8,p9 01ģʽ
                        {
                        A1++;
                        }
                    if(p[-step-1]==0&&p[-step]>0) //p9,p2 01ģʽ
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
                            dst.at<uchar>(i,j) = 0; //����ɾ�����������õ�ǰ����Ϊ0
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
                    if(p[-step]==0&&p[-step+1]>0) //p2,p3 01ģʽ
                        {
                        A1++;
                        }
                    if(p[-step+1]==0&&p[1]>0) //p3,p4 01ģʽ
                        {
                        A1++;
                        }
                    if(p[1]==0&&p[step+1]>0) //p4,p5 01ģʽ
                        {
                        A1++;
                        }
                    if(p[step+1]==0&&p[step]>0) //p5,p6 01ģʽ
                        {
                        A1++;
                        }
                    if(p[step]==0&&p[step-1]>0) //p6,p7 01ģʽ
                        {
                        A1++;
                        }
                    if(p[step-1]==0&&p[-1]>0) //p7,p8 01ģʽ
                        {
                        A1++;
                        }
                    if(p[-1]==0&&p[-step-1]>0) //p8,p9 01ģʽ
                        {
                        A1++;
                        }
                    if(p[-step-1]==0&&p[-step]>0) //p9,p2 01ģʽ
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
                            dst.at<uchar>(i,j) = 0; //����ɾ�����������õ�ǰ����Ϊ0
                            ifEnd = true;
                            }
                        }
                    }
                }
            }

        //��������ӵ����Ѿ�û�п���ϸ���������ˣ����˳�����
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

		//cvNamedWindow("��ֵ��ͼ", CV_WINDOW_AUTOSIZE);
		//cvShowImage("��ֵ��ͼ", dst);
	
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
					if(p[-step]==255&&p[-step+1]<255) //p2,p3 01ģʽ
					{
						A1++;
					}
					if(p[-step+1]==255&&p[1]<255) //p3,p4 01ģʽ
					{
						A1++;
					}
					if(p[1]==255&&p[step+1]<255) //p4,p5 01ģʽ
					{
						A1++;
					}
					if(p[step+1]==255&&p[step]<255) //p5,p6 01ģʽ
					{
						A1++;
					}
					if(p[step]==255&&p[step-1]<255) //p6,p7 01ģʽ
					{
						A1++;
					}
					if(p[step-1]==255&&p[-1]<255) //p7,p8 01ģʽ
					{
						A1++;
					}
					if(p[-1]==255&&p[-step-1]<255) //p8,p9 01ģʽ
					{
						A1++;
					}
					if(p[-step-1]==255&&p[-step]<255) //p9,p2 01ģʽ
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
							 //����ɾ�����������õ�ǰ����Ϊ0
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
