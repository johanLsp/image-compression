#include "EZW.h"



EZW::EZW(int width, int height)
	:width(width), height(height)
{
}

void EZW::test()
{

	/*char wavelets[64] = {63, -34, 49, 10, 5, 18, -12, 7,
						 -31, 23, 14, -13, 3, 4, 6, -1,
						 -25, -7, -14, 8, 5, -7, 3, 9,
						 -9, 14, 3, -12, 4, -2, 3, 2,
						 -5, 9, -1, 47, 4, 6, -2, 2, 
						 3, 0, -3, 2, 3, -2, 0, 4,
						 2, -3, 6, -4, 3, 6, 3, 6,
						 5, 11, 5, 6, 0, 3, -4, 4};
						 
	char wavelets2[64] = {57,-35,52,8,5,12,-10,5,
						  -29,25,14,-14,3,1,5,-2,
						  15,15,2,-9,8,-10,6,12,
						  -10,-6,-11,7,7,-2,4,5,
						  -2,12,-1,47,6,6,-2,3,
						  0,3,-2,1,1,-4,3,1,
						  0,-4,8,-4,4,5,3,3,
						  5,14,4,3,-2,5,-4,1};
	*/					
					
	double wavelets3[width*height] = {0};
	
	wavelets3[0] = -128;
	for(int i = 0; i < width*height ; i++)
	{
		wavelets3[i] = 16;
	}
	
	
	print(wavelets3);
	
	char dst[10*width*height] = {0};
	
	encode(wavelets3, dst);
	cout << endl;
	
	double output[1024] = {0};
	
	
	decode(dst, output);
	
	print(output);
	
}


void EZW::print(double* src)
{
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			cout << setw(3) << src[i+j*width] << " ";
		}
		cout << endl;
	}
}

void EZW::print(int* src)
{
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			cout << setw(3) << src[i+j*width] << " ";
		}
		cout << endl;
	}
}


void EZW::print(char* src)
{
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			cout << setw(3) << src[i+j*width] << " ";
		}
		cout << endl;
	}
}


void EZW::printEZW(char* src, int size)
{
	//char t;
	for(int j = 0; j < size; j++)
	{
		//t = src[j];
		
		//cout << (t&0x80) <<", "<< (t&0x40) <<", " << (t&0x20) <<", "<< (t&0x10) <<", "<< (t&0x08) <<", "<< (t&0x04) <<", "<< (t&0x02) <<", " << (t&0x01) << ", " <<endl;
		cout << ((unsigned int)src[j]) << endl;
	}
}

bool EZW::readBit(char* src, int pos)
{
	unsigned char* x = (unsigned char*)src + pos/8;
	int i = pos%8;
	return (*x >> (7-i)) & 1;
}

void EZW::writeBit(char* dst, int pos, bool bit)
{
	unsigned char* x = (unsigned char*)dst + pos/8;
	int i = pos%8;
	
	if(bit)
	{
		*x |= 1U << (7-i);
	}
	else
	{
		*x &= ~(1U << (7-i));
	}
	//if(readBit(dst, pos) != bit)
	//	cout << "Erreur !" << endl;
}



int EZW::encode(double* src, char* dst)
{

	int max_coef = 1;
	
	for(int i = 0; i < width*height; i++)
	{
		//double v = src[i];
	/*	
		// Avoid dealing with overflow (v=-128 => -v = -128 !)
		if (v == -128)
		{
			src[i]++;
			v++;
		}*/
		if (abs(src[i]) > max_coef) 
			max_coef = abs(src[i]);
	}
	
	int steps = floor(log2(max_coef));
	double T = pow(2, steps);
	dst[0] = (unsigned char)T;
	
	int pos = 8;
	

	// Morton Scan

	
	// Inverse
/*	
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			for (int k = 0; k < 8; k++)
			{
 				morton[i+j*width] |= (i & 1U << k) << k | (j & 1U << k) << (k + 1);
			}
		}
	}
	
	
	for(int i = 0; i < width*height; i++)
	{
		dst[i] = morton[i];
	}
	*/
	
	// Forward

	
	int index[width*height] = {0};
	

	generateMortonIndices(index);

	double map[width*height] = {0};
	
	
	//print(index, width, height);
	
	
	for(int step = 0; step < steps; step++)
	{
	
		//Significance
		queue<int> toEncode;
		
		toEncode.push(0);
	
		while(!toEncode.empty())
		{
			int x = toEncode.front();
			int i = index[x];
			toEncode.pop();


		
			// Only process insignificant values
			if(map[i] == 0)
			{
	
				if(abs(src[i]) >= T)
				{
					//dst[pos++] = src[i] > 0 ? '+' : '-';
					if(src[i] > 0)
					{
						writeBit(dst, pos++, 0); // + : 01
						writeBit(dst, pos++, 1);
					}
					else
					{
						writeBit(dst, pos++, 0); // - : 00
						writeBit(dst, pos++, 0);
					}
					map[i] = T;
				}
				else
				{
					if(x >= height*width/4)
					{
						//dst[pos++] = 'I';
						writeBit(dst, pos++, 1); // I : 11
						writeBit(dst, pos++, 1);
					}
					else
					{
						//cout <<x << ", " << i << endl;
						if(isZeroTree(src, index, T, x))
						{
							//dst[pos++] = 'R';
							writeBit(dst, pos++, 1); // R : 10
							writeBit(dst, pos++, 0);
							continue; // Skip the remaining part of this quadtree
						}
						else
						{
							//dst[pos++] = 'I';
							writeBit(dst, pos++, 1); // I : 11
							writeBit(dst, pos++, 1);
						}
					}
				}
			}
			
			
						
		/*	if(i == 5+14*width)
			{
				cout << x << ", " << (int)src[i] << ", " << (int)map[i] << endl;
			}
			
			if(i/width >= 28 && i/width <30)
			{
				if(i%width >=10 && i%width < 12)
				cout << "   " << x << ", " << (int)src[i] << ", " << (int)map[i] << endl;
			}
			
			if(i/width >= 56 && i/width <60)
			{
				if(i%width >=20 && i%width < 24)
				cout << "      " << x << ", " << (int)src[i] << ", " << (int)map[i] << endl;
			}
			*/

			if(x < height*width/4)
			{
				if(x !=0)
					toEncode.push(4*x);
				toEncode.push(4*x+1);
				toEncode.push(4*x+2);
				toEncode.push(4*x+3);
			}
		}

	
		// Refinement
		for(int x = 0; x < width*height; x++)
		{		
			int i = index[x];
		
			if(map[i] <= T)
				continue;
			
			if(abs(src[i]) >= map[i] && abs(src[i]) < map[i]+T)
			{
				//dst[pos++] = '0';
				writeBit(dst, pos++, 0);
			}
			else
			{
				//dst[pos++] = '1';
				writeBit(dst, pos++, 1);
				map[i] += T;
			}
		}
		
		T /= 2;
		
	}
	
	return pos/8;
	//print(map, 8, 8);
}


void EZW::decode(char* src, double* dst)
{
	double T = (double)static_cast<unsigned char>(src[0]);
	
	int steps = log2(T);
	
	int pos = 8;
	int index[width*height] = {0};
	generateMortonIndices(index);
	
	for(int step = 0; step < steps; step++)
	{
	
		//Significance
		queue<int> toDecode;
		
		toDecode.push(0);
	
		while(!toDecode.empty())
		{
			int x = toDecode.front();
			int i = index[x];
			toDecode.pop();
			
			if(dst[i] == 0) // Process only not decoded values
			{
				
				if(readBit(src, pos++)) // R/I information
				{
					if(!readBit(src, pos++))
						continue; // R : skip sub quadtree
				}
				else // +/- information
				{
					dst[i] = readBit(src, pos++) ? T : -T;
				}
			}
						
			if(x < height*width/4)
			{
				if(x !=0)
					toDecode.push(4*x);
				toDecode.push(4*x+1);
				toDecode.push(4*x+2);
				toDecode.push(4*x+3);
			}
		}
			
		// Refinement
		for(int x = 0; x < width*height; x++)
		{		
			int i = index[x];
		
			if(abs(dst[i]) <= T)
				continue;
			
			if(readBit(src, pos++))
				dst[i] += dst[i] > 0 ? T : -T;	
		}
		T /= 2;	
	}
}

void EZW::generateMortonIndices(int* index)
{

	for(int a = 0; a < width*height; a++)
	{
		int i = 0;
		int j = 0;
		for (unsigned int k = 0; k < 4*sizeof(int); k++)
		{
			i |= (a & 1U << (2*k)) >> k;
			j |= (a & 1U << (2*k+1)) >> (k+1);
		}
		index[a] = i+j*width;
	}	
}

bool EZW::isZeroTree(double* src, int* index ,int T, int x)
{
	if (x == 0) return false;
	
	int i = index[x];
	//if (abs(src[i]) >= 2*T) return true;
	if (abs(src[i]) >= T && abs(src[i]) < 2*T) return false;
	
	//cout << i << ", " << x << endl;
	int child = 4*x;
	if (child >= width*height) return true;
	
	return isZeroTree(src, index, T, 4*x)
		&& isZeroTree(src, index, T, 4*x+1)
		&& isZeroTree(src, index, T, 4*x+2)
		&& isZeroTree(src, index, T, 4*x+3);
}
