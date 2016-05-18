#include"generatexml.h"
#include"tinyxml.h"
#include"tinystr.h"
#include"word.h"
#include<fstream>

/*void generatexml(Word w, vector<CvPoint>& res)
{
	ofstream file;
	file.open("word.xml");
	file << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>"<<endl;
	file << "<Word>" << endl;
	vector<stroke> strokes = w.getStroke();
	for(int i = 0; i < strokes.size(); i++)
	{
		file <<"<stroke>"<<endl;
		vector<CvPoint> midline = strokes[i].getMidLine();
		vector<int> x;
		vector<int> y;
		int ratio = 2;

		if(i == 1) ratio += 1;

		for(int i = 0; i < (midline.size()-1); i++)
		{
			x.push_back(midline[i+1].x - midline[i].x);
			y.push_back(midline[i+1].y - midline[i].y);
		}
		
		for(int i = 1; i < (midline.size()); i++)
		{
			midline[i].x = midline[i-1].x + ratio * x[i-1];
			midline[i].y = midline[i-1].y + ratio * y[i-1];
			
		}

		if(i == 1) 
		{
			res = midline;
		}

		for(int j = 1; j < midline.size(); j++)
		{
			file <<"<point>"<<endl;
			file <<"\t<x>"<< midline[j].x <<"</x>"<<endl;
			file <<"\t<y>" << midline[j].y<< "</y>"<<endl;
			file << "\t<s>" << 1 << "</s>" <<endl;
			file << "\t<v>" << 1 <<"</v>"<<endl;
			file <<"</point>"<<endl;
		}

		file <<"</stroke>"<<endl;
	}
	file << "</Word>" << endl;
	file.close();
}*/

void generatexml(Word w, vector<CvPoint>& res)
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

	//± ª≠ ˝¡ø
	TiXmlElement* pName = new TiXmlElement("strokesnum");
	if(!pName)
		return;

	char strokenum[5];
	_itoa_s(w.getStroke().size(), strokenum, 5);
	TiXmlText *strokenumtag = new TiXmlText(strokenum);
	pName->LinkEndChild(strokenumtag);
	pRootNode->LinkEndChild(pName);

	//± ª≠Œª÷√
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

	TiXmlElement *pAdd = new TiXmlElement("Add");
	if(!pAdd)
	{
		return ;
	}
	pRootNode->LinkEndChild(pAdd);

	TiXmlText *pNameValue = new TiXmlText("STM100");
	//pName->LinkEndChild(pNameValue);
	TiXmlText * pAddValue = new TiXmlText("0001");
	pAdd->LinkEndChild(pAddValue);

	pDoc->SaveFile("Word.xml");

	return ;
}