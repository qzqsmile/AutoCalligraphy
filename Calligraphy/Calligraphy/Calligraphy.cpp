#include <opencv2/opencv.hpp>  
#include<vector>
#include<iostream>
#include<cmath>
#include"tinyxml.h"
#include"point.h"
#include"stroke.h"
#include"word.h"
#include"pixel.h"
#include"thin.h"
#include"generatexml.h"

using namespace cv;
using namespace std;  

uchar GetPixel(const IplImage *img, const CvPoint* p);
bool IsHeng(const vector<CvPoint>& strokepoint, const IplImage * img);
void SetPixel(IplImage *img, const CvPoint *p);
//void DrawHengMiddle(vector<CvPoint> &stroke,const IplImage *img, IplImage * outimg, vector<CvPoint>& hengstroke);

void imagereverse(IplImage *img1)
{
	for(int i = 0; i < img1->width; i++)
	{
		for(int j = 0; j < img1->height; j++)
		{
			/*img1[j*(img1->widthStep)+i] = 255 - img1[j * (img1->widthStep) + i];
				uchar *ptr = (uchar *)img->imageData + ((p->y) * (img->widthStep));

				return ptr[p->x];*/

			uchar *p = (uchar *) img1->imageData + j * (img1->widthStep);
			CvPoint p1;
			p1.x = i;
			p1.y = j;
			p[i] = 255 - GetPixel(img1, &p1);
		}
	}
}


void rotateImage(IplImage* img, IplImage *img_rotate,int degree)  
{  
    //旋转中心为图像中心  
    CvPoint2D32f center;    
    center.x=float (img->width/2.0+0.5);  
    center.y=float (img->height/2.0+0.5);  
    //计算二维旋转的仿射变换矩阵  
    float m[6];              
    CvMat M = cvMat( 2, 3, CV_32F, m );  
    cv2DRotationMatrix( center, degree,1, &M);  
    //变换图像，并用黑色填充其余值  
    cvWarpAffine(img,img_rotate, &M,CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS,cvScalarAll(0) );  
}  


int main( int argc, char** argv)  
{     
	const char* imagename = "C:\\Users\\Q\\Google 云端硬盘\\工作汇报\\论文材料\\图片\\dian1.jpg";
	const char* imagename1 = "..\\Res\\4.jpg";

	/* Mat image;
	 Mat gray_image;
	 Mat bin_image;
	 Mat dst;
	image = imread(imagename, CV_LOAD_IMAGE_COLOR);
	cvtColor( image, gray_image, CV_BGR2GRAY );
	threshold(gray_image, bin_image, 100,255, CV_THRESH_BINARY);

	namedWindow( imagename, CV_WINDOW_AUTOSIZE );
	namedWindow( "Gray image", CV_WINDOW_AUTOSIZE );
	bitwise_not(bin_image, bin_image);

	cvThin(bin_image, dst, 1000);

	imshow("结果",dst);
	imshow( imagename, image );
	imshow( "Gray image", gray_image);

	waitKey(0);

	return 0;*/
	//从文件中读入图像
	IplImage *img = cvLoadImage(imagename,CV_LOAD_IMAGE_UNCHANGED);
	IplImage *img1 = cvLoadImage(imagename1,CV_LOAD_IMAGE_UNCHANGED);

	IplImage * im_median_filter = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);

	IplImage *gray_img = cvCreateImage(cvGetSize(img),IPL_DEPTH_8U, 1);
	IplImage *gray_img1 = cvCreateImage(cvGetSize(img1),IPL_DEPTH_8U, 1);


	cvCvtColor(img, gray_img, CV_BGR2GRAY);
	cvCvtColor(img1, gray_img1, CV_BGR2GRAY);

	//滤波
	cvSmooth(gray_img, im_median_filter,CV_BLUR,5,5);
	//二值化图
	IplImage *bin_img = cvCreateImage(cvGetSize(gray_img), IPL_DEPTH_8U, 1);
	IplImage *bin_img1 = cvCreateImage(cvGetSize(gray_img1), IPL_DEPTH_8U, 1);
	cvThreshold(gray_img, bin_img, 100, 255, CV_THRESH_BINARY);
	cvThreshold(gray_img1, bin_img1, 100, 255, CV_THRESH_BINARY);
	//复制一份留作轮廓处理
	IplImage *bin_copy_image = cvCloneImage(bin_img);
	IplImage *bin_copy_image1 = cvCloneImage(bin_img1);
	IplImage *rotateimage = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	//IplImage *clone = cvCreateImage(cvGetSize(bin_img), IPL_DEPTH_8U, 1);

	waitKey(0);

	Mat calwidth(bin_img, true);
	imagereverse(bin_img);
	imagereverse(bin_img1);

	IplImage *clone = cvCloneImage(bin_img);
	Mat wordoutline1;
	Mat wordoutline2;
	Mat src(bin_img, true);
	Mat src1(bin_img1, true);
	
	//bitwise_not(src, clone1);

	cvThin(src, wordoutline1,100);
	cvThin(src1, wordoutline2,100);
	//thin(bin_img, clone, 100);
	bitwise_not(wordoutline1, wordoutline1);
	bitwise_not(wordoutline2, wordoutline2);

	namedWindow("轮廓");
	imshow("轮廓", wordoutline1);
	waitKey();

//	return 0;

/*	Mat wordoutlineclone;
	wordoutline1.copyTo(wordoutlineclone);
	vector<vector<CvPoint> > wordstrokes;
	vector<vector<CvPoint> > strokesbones;
	vector<CvPoint> crosspoint;
	Word word1;

	storestrokes(wordoutlineclone, strokesbones);
	crosspoint.push_back(strokesbones[0][150]);
	crosspoint.push_back(strokesbones[0][450]);
	crosspoint.push_back(strokesbones[1][129]);
	crosspoint.push_back(strokesbones[1][217]);
	//DrawLineOnMat(wordoutline1, strokesbones[0][160]);
	wordoutline1.copyTo(wordoutlineclone);
	//DrawVerticalOnMat(wordoutline1, strokesbones[0][156]);

	wordoutline1.copyTo(wordoutlineclone);
	vector<vector<CvPoint> >strokesclone;
	cutstroke(wordoutlineclone,strokesclone, crosspoint);
	//DrawLineOnMat(wordoutline1, strokesbones[2][100]);

	wordstrokes.push_back(strokesclone[1]);
	wordstrokes.push_back(strokesclone[3]);
	wordstrokes.push_back(strokesclone[4]);
	wordstrokes.push_back(strokesclone[5]);

	crosspoint.erase(crosspoint.begin());
	word1.storeCrosspoints(crosspoint);

	crosspoint.clear();
	strokesclone.clear();

	wordoutline1.copyTo(wordoutlineclone);
	crosspoint.push_back(strokesbones[0][156]);
	crosspoint.push_back(strokesbones[0][353]);
	cutstroke(wordoutlineclone, strokesclone, crosspoint);

	//DrawLineOnMat(wordoutline1, strokesbones[0][300]);
	wordstrokes.push_back(strokesclone[0]);
	//DrawLineOnMat(wordoutline1, wordstrokes[2][2]);
	//DrawVerticalOnMat(wordoutline1, strokesclone[4][10]);
	//存储笔画和计算宽度
	vector<stroke> stk;
	for(unsigned int i =0;  i < wordstrokes.size(); i++)
	{
		stroke s;
		s.storeMidLine(wordstrokes[i]);
		TYPE t;
		calType(s, t);
		vector<int> width;
		sortstroke(s);
		//计算宽度
		calWidth(calwidth,s,width);
		s.storeWidth(width);
		stk.push_back(s);
	}

	word1.storeStroke(stk);
	
	generatexml(word1);

	namedWindow("轮廓");
	imshow("轮廓", calwidth);
	waitKey();*/
	
	//cvNamedWindow("二值化图", CV_WINDOW_AUTOSIZE);
	//cvShowImage("二值化图", bin_img);
	
	//cvWaitKey(0);

	//cvReleaseImage(&clone);32
	//cvDestroyWindow("二值化图");

	//rotateImage(img, rotateimage, 30);

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
	CvMemStorage *MemCompare1 = cvCreateMemStorage();
	CvMemStorage *MemCompare2 = cvCreateMemStorage();
	CvSeq *pcvSeq = NULL;
	CvSeq *compare1 = NULL;
	CvSeq *compare2 = NULL;


	//这里会修改二值化图像
	cvFindContours(bin_copy_image, pcvMstorage, &pcvSeq, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0)); 
	cvFindContours(bin_copy_image, MemCompare1, &compare1, sizeof(CvContour),CV_RETR_TREE);  
	cvFindContours(bin_copy_image1, MemCompare2, &compare2, sizeof(CvContour),CV_RETR_TREE);  
	//画轮廓图
	IplImage *pOutlineImage = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
	int nlevels = 3;
	cvRectangle(pOutlineImage, cvPoint(0, 0), cvPoint(pOutlineImage->width, pOutlineImage->height), CV_RGB(255, 255, 255), CV_FILLED);  
	IplImage *waterdrop = cvCloneImage(pOutlineImage);

	cvDrawContours(pOutlineImage, pcvSeq, CV_RGB(0,255,0), CV_RGB(0,255,0), nlevels, 1);
	
	Mat pOut(pOutlineImage, true);
	DrawRedMidLine(wordoutline1, pOutlineImage);
	//DrawBlueMidLine(wordoutline2, pOutlineImage);
	
	double compareres = cvMatchShapes(compare1, compare2, CV_RETR_TREE);
	
	//namedWindow("c");
	//imshow("c", pOut);
	//waitKey();

	cvNamedWindow("二值化图", CV_WINDOW_AUTOSIZE);
	cvShowImage("二值化图", pOutlineImage);
	
	cvWaitKey(0);

	return 0;

	//goto lunkuo;
	//获取轮廓的像素坐标
	CvPoint *pPoint = NULL;
	int count = 0;
	//设置存储点以及笔画的容器
	Word w;
	stroke s;
	for(; pcvSeq != NULL; pcvSeq = pcvSeq->h_next)
	{
		count++;
		//uchar res = 0;
		//取轮廓中一个点
		pPoint = (CvPoint *)cvGetSeqElem(pcvSeq, 0);

		//该判断是为了确定轮廓在图像的范围内
		if((pPoint->x >= 5) && (pPoint->y >= 5) && (pPoint->x < ((bin_img->width)-10)) && (pPoint->y < ((bin_img->height)-10)))
		{
			//if(pcvSeq->total < 10)
			//	continue;
			if(IsPoint(pcvSeq))
			{
				//printf("this is a Point! count = %d\n", count);
				//CvPoint *cv = NULL;
				point s;
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
				//DrawLine(pointstroke[0],pOutlineImage);
				maxlength = CalPointLongLine(pointstroke, longlengthpoint);
				//cvLine(pOutlineImage,longlengthpoint[0],longlengthpoint[1],CV_RGB(0,0,255),1,0);

				//确定开始点与结束点
				int begin, end;
				begin = end = 0;

				for(unsigned int i = 0; i < pointstroke.size(); i++)
				{
					if((pointstroke[i].x == longlengthpoint[0].x) && (pointstroke[i].y == longlengthpoint[0].y))
						begin = i;
					if((pointstroke[i].x == longlengthpoint[1].x) && (pointstroke[i].y == longlengthpoint[1].y))
						end = i;
				}
				//计算线段的中位点,这里要注意调整相应的参数
				//end = begin + pointstroke.size() / 2 - 5;
				//begin =  (end + pointstroke.size()) / 2;
				//begin = begin - 2;
				
				
				//int a;
				//画长边
				cvLine(pOutlineImage,pointstroke[begin],pointstroke[end],CV_RGB(0,0,255),1,0);
				//cvLine(pOutlineImage,longlengthpoint[0],longlengthpoint[1],CV_RGB(0,0,255),1,0);
				
				//a = (int)sqrt(pow(pointstroke[begin].x-pointstroke[end].x,2)+pow(pointstroke[begin].y-pointstroke[end].y, 2));
				if(begin > end)
					swap(begin, end);
				//计算雨滴短边
				int longvecx, longvecy;
				longvecx = longlengthpoint[0].x - longlengthpoint[1].x;
				longvecy = longlengthpoint[0].y - longlengthpoint[1].y;
				shortlength = CalPointShortLine(pointstroke,shortlengthpoint, begin, end, longvecx, longvecy);
				cvLine(pOutlineImage,shortlengthpoint[0],shortlengthpoint[1],CV_RGB(0,0,255),1,0);
				//DrawWaterDrop(waterdrop, a, shortlength);

				s.storeLongLine(longlengthpoint[0], longlengthpoint[1]);
				s.storeShortLine(shortlengthpoint[0], shortlengthpoint[1]);
				s.calPar();
				//计算面积
				//pointaera = fabs(cvContourArea(pcvSeq,CV_WHOLE_SEQ));
				
			}
			else 
			{
				vector<CvPoint> strokepoint;
				vector<CvPoint> hengstroke;
				vector<CvPoint> piestroke;
				vector<CvPoint> nastroke;

				for(int i = 0; i < pcvSeq->total; i++)
				{
					pPoint = (CvPoint *)cvGetSeqElem(pcvSeq, i);
					strokepoint.push_back(*pPoint);
				}
				//笔画有的地方有时过于尖锐会导致轮廓分为好几个部分
				//此处就是为了调试而实现的部分
	//			DrawLine(strokepoint[0], pOutlineImage);
	//			if(count == 2)
	//				break;
				//DrawLine(strokepoint[0], pOutlineImage);
				/*CvPoint x;
				x = strokepoint[0];
				for(unsigned int i = 0; i < 50; i++)
				{
				x.x++;
				SetPixel(bin_img, &x);
				}*/
				//SetPixel(bin_img, &x);
				//cvLine(bin_img,x,y,CV_RGB(0,0,0),1,0);
				//SetPixel(pOutlineImage, &x);

				//if(count == 3)
				//cvLine(pOutlineImage,strokepoint[0],strokepoint[50],CV_RGB(0,0,255),1,0);
				while(1){
					if(IsHeng(strokepoint, bin_img))
					{
						//break;
						stroke s;
						cout <<"is heng"<<endl;
						DrawHengMiddle(strokepoint, bin_img, pOutlineImage, hengstroke, s);
						//break;
						//w.storeStroke(s);
					}

					float ang = 0;
					if(IsShu(strokepoint, bin_img))
					{
						stroke s;
						vector<CvPoint>shustroke;
						//DrawOutLine(strokepoint, img);
						DrawShuMiddle(strokepoint, bin_img, pOutlineImage, shustroke, s);
						cout <<"is shu" <<endl;
						//w.storeStroke(s);
					
					}
				/*	drawLine(strokepoint[10], pOutlineImage);
					break;*/
					////if(count == 1)
					////	DrawOutLine(strokepoint, img);
					//DrawLine(strokepoint[0], pOutlineImage);
					//break;
					ang = 0;
					if(IsPie(strokepoint, bin_img, &ang))
					{
						stroke s;
						//DrawLine(strokepoint[20], pOutlineImage);
						cout << "is pie" << endl;
						DrawPieMiddle(strokepoint, bin_img, pOutlineImage,piestroke, ang, s);
						//w.storeStroke(s);
					}
				
					if(IsNa(strokepoint, bin_img))
					{
						//DrawLine(strokepoint[0], pOutlineImage);
						stroke s;
						cout << "Is Na" << endl;
						DrawNaMiddle(strokepoint, bin_img, pOutlineImage, nastroke, s);
						//DrawLine(strokepoint[0],pOutlineImage);
						//w.storeStroke(s);
					}
					//break;

					if(strokepoint.size() < 130)
						break;
				}

				//DrawLine(strokepoint[0], pOutlineImage);
				//if(strokepoint.size() == 28)
				//DrawOutLine(strokepoint, pOutlineImage);
				//if(IsGou(strokepoint, bin_img))
				//{
				//	cout << "Is Gou" << endl;
				//}
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

	vector<CvPoint> res;
	//generatexml(w, res);

	//assert(count != 2);
	CvPoint p;
	p.x = 179;
	p.y = 110;
	uchar res1 = GetPixel(bin_img, &p);
	//SetPixel(bin_img,&p);

	//显示轮廓图
	//lunkuo:
	vector<stroke> strokes = w.getStroke();
	//DrawOutLine(res, pOutlineImage);
	cvNamedWindow("轮廓图", CV_WINDOW_AUTOSIZE);
	cvShowImage("轮廓图", pOutlineImage);
	cvReleaseMemStorage(&pcvMstorage);
	

	cvNamedWindow("原图", CV_WINDOW_AUTOSIZE);
	cvShowImage("原图",img);
	cvNamedWindow("二值化",CV_WINDOW_AUTOSIZE);
	cvShowImage("二值化",bin_img);

	//cvNamedWindow("水滴图",CV_WINDOW_AUTOSIZE);
	//cvShowImage("水滴图", waterdrop);

	cvWaitKey(0);
	cvReleaseImage(&img);
	cvReleaseImage(&gray_img);
	cvReleaseImage(&bin_img);
	cvReleaseImage(&pOutlineImage);
	cvReleaseImage(&waterdrop);

	cvDestroyWindow("原图");
	cvDestroyWindow("二值化");
	cvDestroyWindow("轮廓图");
	cvDestroyWindow("水滴图");

	return 0;  
}