#ifndef WAVELET_WAVELET_H 
#define WAVELET_WAVELET_H 

#include <cmath>


class Wavelet
{
	public:
		virtual void test() = 0;
		virtual void transform(double* src, double* dst, int iterations) = 0;
		virtual void inverse(double* src, double* dst, int iterations) = 0;
		void trim(double* src, int val);
		
	private:
		int width;
		int height;
};

#endif
