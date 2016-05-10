#include <iostream>
#include <fstream>



using namespace std;

int main(int argc, char* argv[])
{
	ifstream src;
	
	src.open("../img/flower_foveon.pgm", ios_base::in|ios_base::binary);
	
	if(!src.is_open())
	{
		cout << "Can't open file" << endl;
		return -1;
	}
	
	ofstream dst;
	dst.open("../img/flower_foveon_inv.pgm", ios_base::out|ios_base::binary);
	
	if(!dst.is_open())
	{
		cout << "Can't create dst file" << endl;
		return -1;
	}
	
	char magic_nbr[2];
	int width, height, max_val;
	
	src >> magic_nbr;
	src >> width;
	src >> height;
	src >> max_val;
	
	dst << magic_nbr << endl;
	dst << width << endl << height << endl;
	dst << max_val << endl;
	
	
	char val;
	for(int h = 0; h < height; h++)
	{
		for(int w = 0; w < width; w++)
		{
			src.read(&val, sizeof(val));
			val = max_val - val;
			dst.write(&val, sizeof(val));
		}
	}
	
	src.close();
	dst.close();
		
}


