#include <iostream>
#include <iomanip>
#include <cmath>
#include <queue>
#include "Encoder.h"




using namespace std;

class EZW : public Encoder
{
	
	public:
		EZW(int width, int height);
		void test();
		int encode(char* src, char* dst);
		void decode(char* src, char* dst);
	
	
	private:
		EZW();
		void print(char* src);
		void print(int* src);
		void printChar(char* src);
		void printEZW(char* src, int size);
		
		void writeBit(char* dst, int pos, bool bit);
		bool readBit(char* src, int pos);

		void generateMortonIndices(int* index);
		bool isZeroTree(char* src, int* index, int T, int x);
		
		int width;
		int height;
};

