#include "Wavelet.h"

void Trim(char* src, int val)
{
	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			if (abs(src[i+j*width]) < val)
				src[i+j*width] = 0;
		}
	}
}
