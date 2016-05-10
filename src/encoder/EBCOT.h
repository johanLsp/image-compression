#include <iostream>
#include "Encoder.h"

using namespace std;

class EBCOT : public Encoder
{
	
	public:
		EBCOT();
		void test();
		int encode(char* src, char* dst, int width, int height);
		void decode(char* src, char* dst, int width, int height);
	
	
	private:

};
