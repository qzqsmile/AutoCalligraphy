#include"generatexml.h"
#include"word.h"
#include<fstream>

void generatexml(Word w, vector<CvPoint>& res)
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
}