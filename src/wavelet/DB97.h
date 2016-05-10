#include <cstring>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "Wavelet.h"


using namespace std;

class DB97 : public Wavelet
{	
	public:
		DB97(int width, int height);
		void test();
		void transformWaterfall(char* src, char* dst, int iterations);
		void transform(char* src, char* dst, int iterations);
		void inverse(char* src, char* dst, int iterations);
		
	private:
		int width;
		int height;
		
		double a = -1.5861343420693648; // 6497/4096
		double b = -0.0529801185718856; // -217/4096
		double c = 0.8829110755411875;  // 3616/4096
		double d = 0.4435068520511142;  // 1817/4096

		//double k1 = 0.230174104914126;
		//double k2 = 1.6257861322319229;
		double k1 = 1.1496043988602418; // Almost best for lena with overflow correction (66.00 dB)
		//double k1 = 1.175;	// Best without overflow correction for lena (65.77dB)
		double k2 = 1/k1;
};
