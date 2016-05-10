#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include "Wavelet.h"


using namespace std;

class Haar : public Wavelet
{
	
	public:
		Haar(int width, int height);
		void test();
		void transform(double* src, double* dst, int iterations);
		void inverse(double* src, double* dst, int iterations);
		
	private:
		int width;
		int height;
	
};
