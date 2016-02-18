#ifndef EXCEPTION_H
#define EXCEPTION_H

#include<stdexcept>

using namespace std;

class StrokeException:public exception{
	public:
		StrokeException() throw();
		const char * what();
	private:
};

#endif