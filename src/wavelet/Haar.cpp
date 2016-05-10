#include "Haar.h"

Haar::Haar(int width, int height)
	: width(width), height(height)
{}



void Haar::test()
{
}

void Haar::transform(double* src, double* dst, int iterations)
{
	int stepWidth = width;
	int stepHeight = height;
	
	double a,b;
	
	double *tmp = new double[width*height]();
		
	// Initialize dst with src
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{	
			dst[i+j*width] = src[i+j*width];
		}	
	}
		
	
	// Compute Haar transform on the lines
	for(int step = 0; step < iterations; step++)
	{	
		for(int j = 0; j < stepHeight; j++)
		{
			for(int i = 0; i < stepWidth/2; i++)
			{
				a = dst[2*i+j*width];
				b = dst[2*i+1+j*width];
				tmp[i+j*width] = (a+b)/2;
				tmp[stepWidth/2 + i+j*width] = (a - b)/2;
			}
		}
	
		// Write changes into dst
		for(int j = 0; j < stepHeight; j++)
		{
			for(int i = 0; i < stepWidth; i++)
			{
				dst[i+j*width] = tmp[i+j*width];
			}
		}
	
		// Compute Haar transform on the columns
		for(int i = 0; i < stepWidth; i++)
		{
			for(int j = 0; j < stepHeight/2; j++)
			{
				a = dst[i+2*j*width];
				b = dst[i+(2*j+1)*width];
				tmp[i+j*width] = (a+b)/2;
				//(a >> 1) + (b >> 1) + (a & b & 0x1);
				tmp[i+(stepHeight/2+j)*width] = (a-b)/2;	
			}	
		}
		
		// Write changes into dst
		for(int j = 0; j < stepHeight; j++)
		{
			for(int i = 0; i < stepWidth; i++)
			{
				dst[i+j*width] = tmp[i+j*width];
			}
		}
		stepHeight /= 2;
		stepWidth /= 2;
	}	
	delete tmp;
}



void Haar::inverse(double* src, double* dst, int iterations)
{

	double a, b;
	
	double *tmp = new double[width*height]();
		
	// Initialize dst with src
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{	
			dst[i+j*width] = src[i+j*width];
		}	
	}
	
	// Compute Haar transform on the lines
	
	int stepWidth = width/pow(2, iterations);
	int stepHeight = height/pow(2, iterations);
	
	for(int step = 0; step < iterations; step++)
	{			
	
		for(int i = 0; i < 2*stepWidth; i++)
		{
			for(int j = 0; j < stepHeight; j++)
			{
				a = dst[i+j*width];
				b = dst[i+(j+stepHeight)*width];
				tmp[i+2*j*width] = a+b;
				tmp[i+(2*j+1)*width] = a-b;
	
			}
		}
		
		stepHeight *= 2;
		
		for(int j = 0; j < stepHeight; j++)
		{
			for(int i = 0; i < 2*stepWidth; i++)
			{
				dst[i+j*width] = tmp[i+j*width];
			}
		}
		
		

		for(int j = 0; j < stepHeight; j++)
		{
			for(int i = 0; i < stepWidth; i++)
			{
				a = dst[i+j*width];
				b = dst[i+stepWidth+j*width];
				tmp[2*i+j*width] = a+b ;
				tmp[2*i+1+j*width] = a-b;	
			}
		}
		
		stepWidth *= 2;
		
		for(int j = 0; j < stepHeight; j++)
		{
			for(int i = 0; i <stepWidth; i++)
			{
				dst[i+j*width] = tmp[i+j*width];
			}
		}
	}	
	delete tmp;
}
