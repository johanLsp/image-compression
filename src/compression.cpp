#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <climits>
#include "encoder/EZW.h"
#include "encoder/EBCOT.h"
#include "wavelet/Haar.h"
#include "wavelet/DB97.h"



using namespace std;

void readFile(double** srcImg, char* filename, int &width, int &height, int &max_val);
void writeFile(double* dstImg, char* filename, int width, int height, int max_val);
void writeEZW(char* dstImg, char* filename, int width, int height, int size);

float MSE(double* src, double* dst, int width, int height);
float PSNR(float mse, int max_val);


int main(int argc, char* argv[])
{

	
	
	
	int width, height, max_val;
	double* srcImg;
	int waveSteps = 1;
	
	readFile(&srcImg, "img/lena_gray.pgm", width, height, max_val);
	
	
	EZW encoder = EZW(width, height);
	DB97 wavelet = DB97(width, height);
	
	double* wavImg = new double[width*height]();
	char* encImg = new char[10*width*height]();
	double* decImg = new double[width*height]();
	double* invImg = new double[width*height]();
	
	wavelet.transform(srcImg, wavImg, waveSteps);
	
	//Trim(img1, 10, width, height);
	
	int size = encoder.encode(wavImg, encImg);
	encoder.decode(encImg, decImg);
	
	wavelet.inverse(decImg, invImg, waveSteps);
	
	writeFile(wavImg, "img/lena_gray_wav.pgm", width, height, max_val);
	writeEZW(encImg, "img/lena_gray.ezw", width, height, size);
	writeFile(decImg, "img/lena_gray_dec.pgm", width, height, max_val);
	writeFile(invImg, "img/lena_gray_inv.pgm", width, height, max_val);
		
		

	float mse = MSE(srcImg, invImg, width, height);
	cout << "MSE : " << mse << endl;
	cout << "PSNR : " << PSNR(mse, max_val) << " dB"<< endl;
	cout << "Ratio : " << width*height/size << ":1" << endl;
	
	float mse_encoder = MSE(wavImg, decImg, width, height);
	cout << "PSNR encoder: " << PSNR(mse_encoder, max_val) << " dB"<< endl;		
	
	
	//wavelet.test();
	
	delete srcImg;
	delete wavImg;
	delete encImg;
	delete decImg;
	delete invImg;
	
	//wavelet.test();
	

}


void readFile(double** srcImg, char* filename, int &width, int &height, int &max_val)
{
	ifstream src;
	
	src.open(filename, ios_base::in|ios_base::binary);
	
	if(!src.is_open())
	{
		cout << "Can't open file " << filename << endl;
	}
	
	int tmp;
	char magic_nbr[2];
	src >> magic_nbr;
	src >> tmp;
	width = tmp;
	src >> tmp;
	height = tmp;
	src >> tmp;
	max_val = tmp;
	
	*srcImg = new double[width*height]();

	char line[512];
	src.read(line, 1);
	for(int j = 0; j < height; j++)
	{	
		src.read(line, 512);		
		for(int i = 0; i < width; i++)
		{
			(*srcImg)[i+j*width] = ((double)static_cast<unsigned char>(line[i])) - 128;
		}
	}
	
	src.close();
}

void writeFile(double* dstImg, char* filename, int width, int height, int max_val)
{
	ofstream dst;
	
	dst.open(filename, ios_base::out|ios_base::binary);
	
	if(!dst.is_open())
	{
		cout << "Can't open file " << filename << endl;
	}
	
	dst << 'P' << '5' << endl;
	dst << width << endl << height << endl;
	dst << max_val << endl;
	
	char pixel;
	for(int i = 0; i < width*height; i++)
	{
		
		pixel = (unsigned char)(dstImg[i]+128);
		dst.write(&pixel, 1);	
	}
	
	dst.close();	
}

void writeEZW(char* dstImg, char* filename, int width, int height, int size)
{
	ofstream dst;
	
	dst.open(filename, ios_base::out|ios_base::binary);
	
	if(!dst.is_open())
	{
		cout << "Can't open file " << filename << endl;
	}
	dst << width << endl << height << endl;
	
	dst.write(dstImg, size);	
	
	dst.close();	

}



float MSE(double* src, double* dst, int width, int height)
{

	float mse = 0;
	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			mse += (src[i+j*width]-dst[i+j*width])*(src[i+j*width]-dst[i+j*width])	;
		}
	}

	return mse/(width*height);
}



float PSNR(float mse, int max_val)
{
	if (mse == 0)
		return 0;
	return 10*log(max_val*max_val/mse);
}
