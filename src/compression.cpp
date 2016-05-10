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

void readFile(char** srcImg, char* filename, int &width, int &height, int &max_val);
void writeFile(char* dstImg, char* filename, int width, int height, int max_val);
void writeEZW(char* dstImg, char* filename, int width, int height, int size);

float MSE(char* src, char* dst, int width, int height);
float PSNR(float mse, int max_val);


int main(int argc, char* argv[])
{

	
	
	
	int width, height, max_val;
	char* srcImg;
	int waveSteps = 3;
	
	readFile(&srcImg, "img/lena_gray.pgm", width, height, max_val);
	
	
	EZW encoder = EZW(width, height);
	DB97 wavelet = DB97(width, height);
	
	char* img1 = new char[width*height]();
	char* img2 = new char[width*height]();
	char* img3 = new char[width*height]();
	char* img4 = new char[width*height]();
	
	wavelet.transform(srcImg, img1, waveSteps);
	
	//Trim(img1, 10, width, height);
	
	int size = encoder.encode(img1, img2);
	encoder.decode(img2, img3);
	
	wavelet.inverse(img3, img4, waveSteps);
	
	writeFile(img1, "img/lena_gray_wav.pgm", width, height, max_val);
	writeEZW(img2, "img/lena_gray.ezw", width, height, size);
	writeFile(img3, "img/lena_gray_dec.pgm", width, height, max_val);
	writeFile(img4, "img/lena_gray_inv.pgm", width, height, max_val);
		
		

	float mse = MSE(srcImg, img4, width, height);
	cout << "MSE : " << mse << endl;
	cout << "PSNR : " << PSNR(mse, max_val) << " dB"<< endl;
	cout << "Ratio : " << width*height/size << ":1" << endl;
	
	float mse_encoder = MSE(img1, img3, width, height);
	cout << "PSNR encoder: " << PSNR(mse_encoder, max_val) << " dB"<< endl;		
	
	
	//wavelet.test();
	
	delete srcImg;
	delete img1;
	delete img2;
	delete img3;
	delete img4;
	

}


void readFile(char** srcImg, char* filename, int &width, int &height, int &max_val)
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
	
	*srcImg = new char[width*height]();

	char line[512];
	src.read(line, 1);
	for(int j = 0; j < height; j++)
	{	
		src.read(line, 512);		
		for(int i = 0; i < width; i++)
		{
			(*srcImg)[i+j*width] = (char) ( ((int)line[i])-128);
		}
	}
	
	src.close();
}

void writeFile(char* dstImg, char* filename, int width, int height, int max_val)
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
		
		pixel = ((unsigned char)((((int)(dstImg[i]))+128)));
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



float MSE(char* src, char* dst, int width, int height)
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
