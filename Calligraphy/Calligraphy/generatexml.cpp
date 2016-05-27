#include"generatexml.h"
#include"tinyxml.h"
#include"tinystr.h"
#include"word.h"
#include<fstream>

void generatexml(Word w)
{
	TiXmlDocument *pDoc = new TiXmlDocument();
	if(!pDoc)
	{
		cout <<"create xml error!"<< endl;
		return ;
	}

	TiXmlDeclaration * pDec = new TiXmlDeclaration("1.0", "UTF-8", "yes");
	if(!pDec)
	{
		cout <<"declaration is error!"<< endl;
		return ;
	}
	pDoc->LinkEndChild(pDec);

	TiXmlElement* pRootNode = new TiXmlElement("Word");
	if(!pRootNode)
		return ;
	pDoc->LinkEndChild(pRootNode);

	//笔画数量
	TiXmlElement* pName = new TiXmlElement("strokesnum");
	if(!pName)
		return;

	char strokenum[5];
	_itoa_s(w.getStroke().size(), strokenum, 5);
	TiXmlText *strokenumtag = new TiXmlText(strokenum);
	pName->LinkEndChild(strokenumtag);
	pRootNode->LinkEndChild(pName);

	//笔画位置
	TiXmlElement *strokeplacetag = new TiXmlElement("strokeplace");

	for(unsigned int i = 0; i < w.getStroke().size(); i++)
	{
		TiXmlElement* strokebeginplace = new TiXmlElement("beginplace");
		TiXmlElement* placex = new TiXmlElement("x");
		TiXmlElement* placey = new TiXmlElement("y");

		char x[5], y[5];
		_itoa_s(w.getStroke()[i].getBegin().x, x, 5);
		_itoa_s(w.getStroke()[i].getBegin().y, y, 5);

		placex->LinkEndChild(new TiXmlText(x));
		placey->LinkEndChild(new TiXmlText(y));

		strokebeginplace->LinkEndChild(placex);
		strokebeginplace->LinkEndChild(placey);

		strokeplacetag->LinkEndChild(strokebeginplace);
		//delete strokebeginplace;
	}

	pRootNode->LinkEndChild(strokeplacetag);

	//笔画组合
	TiXmlElement* combinestrokes = new TiXmlElement("combinestrokes");
	pRootNode->LinkEndChild(combinestrokes);

	//笔画相交点
	TiXmlElement* crosspoints = new TiXmlElement("crosspoints");
	for(unsigned int j = 0; j < w.getStrokeCrossPoints().size(); j++)
	{
		TiXmlElement* crosstag = new TiXmlElement("points");
		TiXmlElement* placex = new TiXmlElement("x");
		TiXmlElement* placey = new TiXmlElement("y");

		char x[5], y[5];
		_itoa_s(w.getStrokeCrossPoints()[j].x, x, 5);
		_itoa_s(w.getStrokeCrossPoints()[j].y, y, 5);

		placex->LinkEndChild(new TiXmlText(x));
		placey->LinkEndChild(new TiXmlText(y));
		crosstag->LinkEndChild(placex);
		crosstag->LinkEndChild(placey);

		crosspoints->LinkEndChild(crosstag);
	}
	pRootNode->LinkEndChild(crosspoints);

	//笔画相对角度
	TiXmlElement* strokeangle = new TiXmlElement("strokeangle");
	pRootNode->LinkEndChild(strokeangle);

	//笔画信息
	TiXmlElement* strokestag = new TiXmlElement("Strokes");
	for(unsigned int j = 0; j < w.getStroke().size(); j++)
	{
		stroke s = w.getStroke()[j];
		TiXmlElement *stroke = new TiXmlElement("stroke");
		TiXmlElement *begin = new TiXmlElement("strokebegin");
		TiXmlElement *end = new TiXmlElement("strokeend");
		TiXmlElement *beginplacex = new TiXmlElement("x");
		TiXmlElement *beginplacey = new TiXmlElement("y");
		TiXmlElement *endplacex = new TiXmlElement("x");
		TiXmlElement *endplacey = new TiXmlElement("y");

		char x[5], y[5];
		_itoa_s(s.getBegin().x, x, 5);
		_itoa_s(s.getBegin().y, y, 5);

		beginplacex->LinkEndChild(new TiXmlText(x));
		beginplacey->LinkEndChild(new TiXmlText(y));
		begin->LinkEndChild(beginplacex);
		begin->LinkEndChild(beginplacey);

		_itoa_s(s.getEnd().x, x, 5);
		_itoa_s(s.getEnd().y, y, 5);

		endplacex->LinkEndChild(new TiXmlText(x));
		endplacey->LinkEndChild(new TiXmlText(y));
		end->LinkEndChild(endplacex);
		end->LinkEndChild(endplacey);

		vector<CvPoint> mid = s.getMidLine();
		vector<int>width = s.getWidth();

		TiXmlElement *midlinetag = new TiXmlElement("midline");

		for(unsigned int i = 0; i < mid.size(); i++)
		{
			TiXmlElement *midplacex = new TiXmlElement("x");
			TiXmlElement *midplacey = new TiXmlElement("y");
			TiXmlElement *strokewidthtag = new TiXmlElement("width");

			char x[5], y[5];
			_itoa_s(mid[i].x, x, 5);
			_itoa_s(mid[i].y, y, 5);

			midplacex->LinkEndChild(new TiXmlText(x));
			midplacey->LinkEndChild(new TiXmlText(y));

			char wid[7];
			_itoa_s(max(width[i],0), wid, 7);

			strokewidthtag->LinkEndChild(new TiXmlText(wid));

			midlinetag->LinkEndChild(midplacex);
			midlinetag->LinkEndChild(midplacey);
			midlinetag->LinkEndChild(strokewidthtag);
		}

		//长度
		char len[8];
		_itoa_s(w.getStroke()[j].getMidLine().size(),len, 8);
		TiXmlElement *strokelength = new TiXmlElement("length");
		strokelength->LinkEndChild(new TiXmlText(len));

		TiXmlElement* type = new TiXmlElement("type");
		char stroketype[6][5] = {"h","s","p","n","g","t"};
		type->LinkEndChild(new TiXmlText(stroketype[w.getStroke()[j].getType()]));

		stroke->LinkEndChild(type);
		stroke->LinkEndChild(strokelength);
		stroke->LinkEndChild(midlinetag);
		stroke->LinkEndChild(begin);
		stroke->LinkEndChild(end);
		strokestag->LinkEndChild(stroke);

	}
	pRootNode->LinkEndChild(strokestag);
	//点
	TiXmlElement* pointstag = new TiXmlElement("points");
	//points->LinkEndChild();
	for(unsigned int i = 0; i < w.getPoints().size(); i++)
	{
		TiXmlElement* point = new TiXmlElement("point");
		TiXmlElement* para = new TiXmlElement("para");
		TiXmlElement* parb = new TiXmlElement("parb");
		TiXmlElement* placex = new TiXmlElement("x");
		TiXmlElement* placey = new TiXmlElement("y");

		char par1[5], par2[5];

		_itoa_s(w.getPoints()[i].getParA(), par1, 5);
		_itoa_s(w.getPoints()[i].getParB(), par2, 5);	

		point->LinkEndChild(para);
		point->LinkEndChild(parb);
		point->LinkEndChild(placex);
		point->LinkEndChild(placey);
		pointstag->LinkEndChild(point);
	}
	pRootNode->LinkEndChild(pointstag);

	pDoc->SaveFile("Word.xml");

	return ;
}