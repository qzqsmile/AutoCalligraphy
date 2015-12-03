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
	const char* imagename = "C:\\Users\\Q\\Desktop\\�ڰ�.jpg";
 
    //���ļ��ж���ͼ��
	IplImage *img = cvLoadImage(imagename,CV_LOAD_IMAGE_UNCHANGED);
	IplImage *gray_img = cvCreateImage(cvGetSize(img),IPL_DEPTH_8U, 1);
	cvCvtColor(img, gray_img, CV_BGR2GRAY);
	//��ֵ��ͼ
	IplImage *bin_img = cvCreateImage(cvGetSize(gray_img), IPL_DEPTH_8U, 1);
	cvThreshold(gray_img, bin_img, 100, 255, CV_THRESH_BINARY);
	//����һ��������������
	IplImage *bin_copy_image = cvCloneImage(bin_img);
 
	/*CvScalar cs;                            //�������ر���  
	for(int i = 0; i < img->height; i++)  
		for (int j = 0; j < img->width; j++)  
		{  
			cs = cvGet2D(img, i, j);   //��ȡ����  
			cs.val[0] = 255;             //��ָ�����ص�RGBֵ���������趨  
			//cs.val[1] = 255;  
			//cs.val[2] = 255;  
			cvSet2D(img, i, j, cs);    //���ı�����ر��浽ͼƬ��  
		}*/
	/*int width = bin_img->width;
	int height = bin_img->height;

	//��ȡ����ֵ
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
	//���������������������
	CvMemStorage *pcvMstorage = cvCreateMemStorage();
	CvSeq *pcvSeq = NULL;
	//������޸Ķ�ֵ��ͼ��
	cvFindContours(bin_copy_image, pcvMstorage, &pcvSeq, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0)); 
	//������ͼ
	IplImage *pOutlineImage = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
	int nlevels = 3;
	cvRectangle(pOutlineImage, cvPoint(0, 0), cvPoint(pOutlineImage->width, pOutlineImage->height), CV_RGB(255, 255, 255), CV_FILLED);  
	cvDrawContours(pOutlineImage, pcvSeq, CV_RGB(255,0,0), CV_RGB(0,255,0), nlevels, 2);
	
	//��ȡ��������������
	CvPoint *pPoint = NULL;
	int count = 0;
	for(; pcvSeq != NULL; pcvSeq = pcvSeq->h_next)
	{
		count++;
		//uchar res = 0;
		//ȡ������һ����
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

				//ȷ����ʼ���������
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
				//������ζ̱�
				int longvecx, longvecy;
				longvecx = longlengthpoint[0].x - longlengthpoint[1].x;
				longvecy = longlengthpoint[0].y - longlengthpoint[1].y;
				shortlength = CalPointShortLine(pointstroke,shortlengthpoint,begin, end,longvecx, longvecy);
				cvLine(pOutlineImage,shortlengthpoint[0],shortlengthpoint[1],CV_RGB(0,0,255),1,0);
				//�������
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

	//��ʾ����ͼ
	cvNamedWindow("����ͼ", CV_WINDOW_AUTOSIZE);
	cvShowImage("����ͼ", pOutlineImage);
	cvReleaseMemStorage(&pcvMstorage);

	cvNamedWindow("ԭͼ", CV_WINDOW_AUTOSIZE);
	cvShowImage("ԭͼ",img);
	cvNamedWindow("��ֵ��",CV_WINDOW_AUTOSIZE);
	cvShowImage("��ֵ��",bin_img);
    cvWaitKey(0);
    cvReleaseImage(&img);
	cvReleaseImage(&gray_img);
	cvReleaseImage(&bin_img);
	cvReleaseImage(&pOutlineImage);
	
    cvDestroyWindow("ԭͼ");
	cvDestroyWindow("��ֵ��");
	cvDestroyWindow("����ͼ");

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

/*
���ܣ��ж�����������ǲ���һ���������
���룺pstrokeָ��һ������
���������ǵ����������true�����򷵻�false
*/
bool IsPoint(const CvSeq *pstroke)
{
	if((pstroke->total > 10) && (pstroke->total < 100))
		return true;
	return false;
}

/*
���ܣ������ĳ���
���룺cv��洢���ǵ����������꣬store���ص��ǳ��ߵĿ�ʼ�������
��������س��ߵĳ��ȣ����ѳ��ߵĿ�ʼ����������store�С�
*/

int CalPointLongLine(const vector<CvPoint>&cv, vector<CvPoint>& store)
{
	int max = 0;
	
	for(int i = 0; i < cv.size(); i++)
	{
		for(int j = i; j < cv.size(); j++)
		{
			//�����Ƿ�Ҫ����float?
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
���ܣ�����������̱߳��ȣ������ؿ�ʼ�������
���룺cv�Ǵ洢�����ĵ꣬shortstore���ص��Ƕ̱ߵĿ�ʼ������㡣
	  begin��end�ֱ������ǳ��������Ŀ�ʼ����������cv�д洢���±�
	  longvecx�ǳ��ߵ�������xֵ��longvecy�ǳ���������yֵ
��������ض̱ߵĳ��ȣ����Ѷ̱߿�ʼ����������shortstore��
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

//�ж����ǲ��Ǹ�����
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

	//����40��������Ϊ�Ǻ�
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
//����λ��

void DrawHengMiddle(vector<CvPoint> &stroke, const IplImage *img, IplImage *Outimg, vector<CvPoint>& hengstroke)
{
	CvPoint testpoint = stroke[0];
	testpoint.y += 15;
	
	if(GetPixel(img, &testpoint))
	{
		testpoint.y -= 5;
	}
	int left = 0, right = 0;

	//ȷ�����
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
	//ȷ���ұ�
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
	//����λ��
	int ave = INT_MAX/2;
	for(int i = left; i < right; i++)
	{
		int up, down;
		//ȷ��up
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
		//ȷ��down
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
		//��һ�β����ߣ���ȷ����һ����ʼ��
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

	//ɾ����������Щ��
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
