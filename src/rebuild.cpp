#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cmath>



using namespace std;

			
void InvHaarTransform(unsigned char* src, unsigned char* dst,
				   int width, int height, int iterations);
void writeHDR();

int main(int argc, char* argv[])
{
	writeHDR();
	
	
	
	ifstream src;
	
	src.open("../img/hdr.dat", ios_base::in|ios_base::binary);
	
	if(!src.is_open())
	{
		cout << "Can't open file" << endl;
		return -1;
	}
	
	ofstream dst;
	dst.open("../img/hdr_rebuild.pgm", ios_base::out|ios_base::binary);
	
	if(!dst.is_open())
	{
		cout << "Can't create dst file" << endl;
		return -1;
	}
	
	char magic_nbr[2];
	int width, height, max_val;
	
	src >> width;
	src >> height;
	src >> max_val;
	
	
	
	dst << 'P' << '5' << endl;
	dst << width << endl << height << endl;
	dst << max_val << endl;
	
	
	
	
	unsigned char* img = (unsigned char*) malloc(width*height*sizeof(char));
	unsigned char* dstImg = (unsigned char*) malloc(width*height*sizeof(char));
	
	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			img[i+j*width] = 127;
		}
	}
	
	
	unsigned char tmp;		
	src.read((char*)(&tmp),1);
	
	unsigned int x,y,val;
	
	for(int i = 0; i < 15; i++)
	{

		src.read((char*)(&tmp),1);
		x = 255*tmp;
		src.read((char*)(&tmp),1);
		x += tmp;
		
		src.read((char*)(&tmp),1);
		y = 255*tmp;
		src.read((char*)(&tmp),1);
		y += tmp;
		
		src.read((char*)(&tmp),1);
		val = tmp;
		cout << x << ", "<<y <<", " << val << endl;
		img[x+width*y] = val;
	}
	
	
	InvHaarTransform(img, dstImg, width, height, 11);
	
	dst.write((char*)dstImg, width*height);
	
	src.close();
	dst.close();
	
	free(img);
	free(dstImg);
}




void writeHDR()
{
	ofstream hdr;
	hdr.open("../img/hdr.dat", ios_base::out|ios_base::binary);
	
	if(!hdr.is_open())
	{
		cout << "Can't open file" << endl;
	}
	
	hdr << 3072 << endl << 2048 << endl << 255 << endl;
	
	
	unsigned char data[] =  {0,0,0,0,60,
							 0,2,0,0,51,
							 0,49,0,119,159,
							 0,49,0,120,86,
							 0,100,0,122,159,
							 0,104,0,123,93,
							 0,104,0,246,159,
							 0,104,1,239,93,
							 0,210,1,239,161,
							 0,224,1,237,94,
							 1,49,0,221,95,
							 3,70,1,239,94,
							 5,7,1,140,83,
							 6,6,0,2,91,
							 10,23,1,141,89};
	hdr.write((char*)data, 15*5*sizeof(char));
	hdr.close();
}
	
	

	
	
void InvHaarTransform(unsigned char* src, unsigned char* dst, int width, int height, int iterations)
{

	unsigned char a;
	char b;
	
	unsigned char* swap;
	
	unsigned char *temp = (unsigned char*) malloc (sizeof(unsigned char) *width*height);
		
	// Initialize dst with src
	memcpy( dst, src, width*height*sizeof(char) );
	
	// Compute Haar transform on the lines
	// Store averages in temp, and differences directly in dst
	
	
	for(int k = pow(2, iterations); k != 1 ; k /= 2)
	{				
	
		for(int i = 0; i < width; i++)
		{
			for(int j = 0; j < height/k; j++)
			{
				a = dst[i+j*width];
				b = dst[i+(j+height/k)*width];
				temp[i+2*j*width] = a+b - 127;
				temp[i+(2*j+1)*width] = a-b + 127;
	
			}
			// Copy the remaining part of the current step
			for(int j = 2*height/k; j < height; j++)
			{
				temp[i+j*width] = dst[i+j*width];
	
			}
		}
		swap = dst;
		dst = temp;
		temp = swap;
	}

	for(int k = pow(2, iterations); k !=1; k /= 2)
	{
		for(int j = 0; j < height; j++)
		{
			for(int i = 0; i < width/k; i++)
			{
				a = dst[i+j*width];
				b = dst[i+width/k+j*width];
				temp[2*i+j*width] = a+b - 127;
				temp[2*i+1+j*width] = a-b + 127;	
			}
			
			for(int i = 2*width/k; i < width; i++)
			{
				temp[i+j*width] = dst[i+j*width];
			}
		}
		swap = dst;
		dst = temp;
		temp = swap;
	}	

	free(temp);
	

}
