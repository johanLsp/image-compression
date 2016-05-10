#ifndef WAVELET_WAVELET_H 
#define WAVELET_WAVELET_H 

#include <cmath>


class Wavelet
{
	public:
		virtual void test() = 0;
		virtual void transform(char* src, char* dst, int iterations) = 0;
		virtual void inverse(char* src, char* dst, int iterations) = 0;
		void trim(char* src, int val);
		
	private:
		int width;
		int height;
};

#endif
