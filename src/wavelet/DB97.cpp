#include "DB97.h"




/**  Lifting based Wavelet Transform : 
 *
 * Step 1 : Splitting : split the input X between Xo (odd indices)
 *		    and Xe (even indices)
 *
 * Step 2 : Lifting : Estimate Xo from Xe (with prediction operation P)
 *			Compute error d = Xo - P(Xe)
 *			Update error before adding it to Xe (in order to smooth ?)
 *			s = Xe + U(d) 
 *
 * Step 3 : Scaling : Multiply d & s by factor Ko & Ke resp.
 *			Xh1 = Ko*d
 *			XL1 = Ke*s
 */


DB97::DB97(int width, int height)
	: width(width), height(height)
{}




void DB97::test()
{

	char src[64] = {33, 30, 28, 28, 26, 24, 20, 22,
					23, 29, 29, 29, 30, 5, 14, 14,
					10, 15, 18, 9, 7, 7, 7, 9,
					0, 5, 5, 0, -3, -4, -2, -1,
					-15, -18, -6, -12, -30, -32, -35, -40,
					-17, -23, -10, -24, -32, -47, -55, -57,
					-25, -27, -15, -20, -23, -26, -28, -28,
					-12, -18, -9, -4, -12, -5, -1, 5};
	
	width = 8;
	height = 8;
	
	for (int j= 0; j < height; j++)
	{
		for (int i= 0; i < width; i++)
		{
			cout << setw(4) << (int)src[i+j*width];
		
		}
		cout << endl;
	}
	cout << endl;
	char dst[64];
	transform(src, dst, 1);
	
	
	for (int j= 0; j < height; j++)
	{
		for (int i= 0; i < width; i++)
		{
			cout << setw(4) << (int)dst[i+j*width];
		
		}
		cout << endl;
	}
	cout << endl;
	
	char inv[64];
	inverse(dst, inv, 1);
	
	
	for (int j= 0; j < height; j++)
	{
		for (int i= 0; i < width; i++)
		{
			cout << setw(4) << (int)inv[i+j*width];
		
		}
		cout << endl;
	}
	cout << endl;
	
}
  
void DB97::transformWaterfall(char* src, char* dst, int iterations)
{

	// Work directly on dst

	double* tmp = new double[width*height]();

	//memcpy(tmp, src, height*width*sizeof(char));
	
	for(int j = 0; j < height*width; j++)
	{
		tmp[j] = src[j];
	}

	double* X; // Points to the current row in src;

	for(int j = 0; j < height; j++)
	{ 
		X = tmp + j*width;
	
		// Compute first coef X[0..3]
	
		// Predict 1
		X[1] += a*(X[0]+X[2]);
		X[3] += a*(X[2]+X[4]);
	
		// Update 1
		X[0] += b*(X[1]+X[1]); // Missing value equal to first
		X[2] += b*(X[1]+X[3]);
	
		// Predict 2
		X[1] += c*(X[0]+X[2]);
	
		// Update 2
		X[0] += d*(X[1]+X[1]); // Missing value equal to first
	
		int i;
		for(i = 5; i < width - 1; i+=2)
		{
			X[i]   += a*(X[i-1]+X[i+1]); // Predict 1
			X[i-1] += b*(X[i-2]+X[i]);   // Update 1
			X[i-2] += c*(X[i-3]+X[i-1]); // Predict 2
			X[i-3] += d*(X[i-4]+X[i-2]); // Update 2
		
			X[i-4] = k1*X[i-4];					// Scale odd
			X[i-5] = k2*X[i-5];					// Scale even
		}


		// Compute last coefs
		if (width%2 == 0) // 4 coefs remaining
		{
			X[i]   += a*(X[i-1]+X[i-1]); // Missing value equal to first
			X[i-1] += b*(X[i-2]+X[i]); 
		
			X[i-2] += c*(X[i-3]+X[i-1]);
			X[i-3] += d*(X[i-4]+X[i-2]);
		
			X[i]   += c*(X[i-1]+X[i-1]); // Missing value equal to first
			X[i-1] += d*(X[i-2]+X[i]);
		
			X[i]   = k1*X[i];
		}
		else // 3 coefs remaining
		{
			X[i-1] += b*(X[i-2]+X[i-2]); // Missing value equal to first
		
			X[i-2] += c*(X[i-3]+X[i-1]);
			X[i-3] += d*(X[i-4]+X[i-2]);
			X[i-1] += d*(X[i-2]+X[i-2]); // Missing value equal to first	
		}
	
		// Scale last coefs
		X[i-1] = k2*X[i-1];
		X[i-2] = k1*X[i-2];
		X[i-3] = k2*X[i-3];
		X[i-4] = k1*X[i-4];
		X[i-5] = k2*X[i-5];
	}

	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width/2; i++)
		{
			dst[i+j*width] = tmp[2*i+j*width];
			dst[i+j*width+width/2] = tmp[2*i+1+j*width];
		}
	}	

	delete tmp;
}
 
 
void DB97::transform(char* src, char* dst, int iterations)
{

	/*	-17		-23		-10		-24		-32		-47		-55		-57
				19.83			42.62			90.99			117.47
			
	
	*/

	double* tmp = new double[width*height]();

	// Copy src into tmp
	for(int j = 0; j < height*width; j++)
	{
		tmp[j] = src[j];
	}

	int stepWidth = width;
	int stepHeight = height;


	for(int step = 1; step <= iterations; step++)
	{
		for(int pass = 1; pass <= 2; pass++)
		{
			double* X; // Points to the current row in tmp;	
			for(int j = 0; j < stepHeight; j++)
			{
				X = tmp + j*width;
		
		
			if( j==59 && 2*step+pass == 4)
			{ 
			for(int k = -4; k < 5; k++)
				cout << tmp[272+j*width+k] << " ";
			cout << endl;
			}
				
				
					
				for(int i = 1; i < stepWidth - 1; i += 2)
				{
					X[i] += a*(X[i-1]+X[i+1]);
				}
								
				if(stepWidth%2 == 0)
					X[stepWidth-1] += 2*a*X[stepWidth-2];




			if( j==59 && 2*step+pass == 4)
			{ 
			for(int k = -4; k < 5; k++)
				cout << tmp[272+j*width+k] << " ";
			cout << endl;
			}
				
				
				X[0] += 2*b*X[1];
	
				for(int i = 2; i < stepWidth - 1; i += 2)
				{
					X[i] += b*(X[i-1]+X[i+1]);
				}
				if(stepWidth%2 == 1)
					X[stepWidth-1] += 2*b*(X[stepWidth-2]);



			if( j==59 && 2*step+pass == 4)
			{ 
			for(int k = -4; k < 5; k++)
				cout << tmp[272+j*width+k] << " ";
			cout << endl;
			}
				
				
				for(int i = 1; i < stepWidth - 1; i += 2)
				{
					X[i] += c*(X[i-1]+X[i+1]);
				}
				if(stepWidth%2 == 0)
					X[stepWidth-1] += 2*c*X[stepWidth-2];
		
		
			if( j==59 && 2*step+pass == 4)
			{ 
			for(int k = -4; k < 5; k++)
				cout << tmp[272+j*width+k] << " ";
			cout << endl;
			}
				
	
				X[0] += 2*d*X[1];
				for(int i = 2; i < stepWidth - 1; i += 2)
				{
					X[i] += d*(X[i-1]+X[i+1]);
				}
				if(stepWidth%2 == 1)
					X[stepWidth-1] += 2*d*(X[stepWidth-2]);	

			if( j==59 && 2*step+pass == 4)
			{ 
			for(int k = -4; k < 5; k++)
				cout << tmp[272+j*width+k] << " ";
			cout << endl;
			}
				
				
				// Can be included in the transpose/re-arrange step
				for(int i = 0; i < stepWidth; i += 2)
				{
					X[i] = k2*X[i];
					X[i+1] = k1*X[i+1];
				}
		
				if(stepWidth%2 == 1)
					X[stepWidth-1] = k2*X[stepWidth-1];							
	
	
			if( j==59 && 2*step+pass == 4)
			{ 
			for(int k = -4; k < 5; k++)
				cout << tmp[272+j*width+k] << " ";
			cout << endl;
			}
	
			}
			



			// i = 272, j = 59

	
			// Re-transpose tmp into dst & re-arrange coefs
			for(int j = 0; j < stepHeight; j++)
		 	{
		 		for(int i = 0; i < stepWidth/2; i++)
		 		{

		 			
		 			dst[i*height+j] = tmp[2*i+j*width];
		 			dst[i*height+j +width*stepHeight/2] = tmp[2*i+1+j*width];
		 			
		 			if(abs(tmp[2*i+j*width]) > 127)
		 			{
		 			dst[i*height+j] = tmp[2*i+j*width] > 0? 127:-127;

		 		//	cout << "Overflow " << j << ", " << i << " : " << tmp[2*i+j*width] << endl;
 						//dst[i*height+j] = -125;
		 			}
		 			if(abs(tmp[2*i+1+j*width]) > 127)
		 			{
		 			dst[i*height+j +width*stepHeight/2] = tmp[2*i+1+j*width] > 0? 127:-127;
		 			//cout << "Overflow" << endl;
		 			//cout << "Overflow' " << i+stepHeight/2  << ", " << j<< " : " << tmp[2*i+1+j*width] << endl;

 					}
		 			//dst[i+height*j] = tmp[2*i+j*width];
		 			//dst[i+height*j +width*stepWidth/2] = tmp[2*i+1+j*width];
		 		}
		 	}
		 	
		 /*	char filename[32]; // enough to hold all numbers up to 64-bits
			sprintf(filename, "img/tmp%d.pgm",  2*step+pass);
		 	writeFile(dst, filename, width, height, 255);
		 	*/
		 	int swap = stepWidth;
		 	stepWidth = stepHeight;
		 	stepHeight= swap;
		 	
		 	swap = width; 
		 	width = height; 
		 	height = swap;
		 	
		 	 		
			if( !(step == iterations && pass == 2) )
			{
				for(int j = 0; j < height*width; j++)
				{
					tmp[j] = dst[j];
				}
			}
		}
		stepWidth /= 2;
		stepHeight /= 2;
	}
	delete tmp;
}



void DB97::inverse(char* src, char* dst, int iterations)
{


/*

  33  30  28  28  26  24  20  22  23  29  29  29  30   5  14  14

  33  28  26  20  23  29  30  14 -66 -57 -48 -46 -53 -64 -64 -30

*/
	double* tmp = new double[width*height]();



	// Copy src into tmp
	for(int j = 0; j < height*width; j++)
	{
		tmp[j] = src[j];
	}

	int stepWidth = width/pow(2, iterations-1);
	int stepHeight = height/pow(2, iterations-1);


	for(int step = 1; step <= iterations; step++)
	{
		for(int pass = 1; pass <= 2; pass++)
		{
			double* X; // Points to the current row in tmp;	
			
			
			// Transpose tmp into dst & re-arrange coefs
			for(int j = 0; j < stepHeight/2; j++)
		 	{
		 		for(int i = 0; i < stepWidth; i++)
		 		{
		 			dst[2*j+i*height] = tmp[i+j*width];
		 			dst[2*j+1+i*height] = tmp[i+(j+stepHeight/2)*width];
		 			
		 			//dst[i+height*j] = tmp[2*i+j*width];
		 		}
		 		
		 	 		//dst[i*height+j] = tmp[2*i+j*width];
		 			//dst[i*height+j +width*stepHeight/2] = tmp[2*i+1+j*width];	 			
		 	}
		 	
		 	
		 	int swap = stepWidth;
		 	stepWidth = stepHeight;
		 	stepHeight= swap;
		 	
		 	swap = width; 
		 	width = height; 
		 	height = swap; 
		 	
		 	

			for(int j = 0; j < height*width; j++)
			{
				tmp[j] = dst[j];
			}
			
		
			for(int j = 0; j < stepHeight; j++)
			{
				X = tmp + j*width;
				
				
				for(int i = 0; i < stepWidth; i += 2)
				{
					X[i] = X[i]/k2;
					X[i+1] = X[i+1]/k1;
				}
	
				if(stepWidth%2 == 1)
					X[stepWidth-1] = X[stepWidth-1]/k1;	
		
			
				if(stepWidth%2 == 1)
					X[stepWidth-1] = X[stepWidth-1]/k1;	
		
		
				X[0] -= 2*d*X[1];
				for(int i = 2; i < stepWidth - 1; i += 2)
				{
					X[i] -= d*(X[i-1]+X[i+1]);
				}
				if(stepWidth%2 == 1)
					X[stepWidth-1] -= 2*d*(X[stepWidth-2]);	

			 	
		 		for(int i = 1; i < stepWidth - 1; i += 2)
				{
					X[i] -= c*(X[i-1]+X[i+1]);
				}
				if(stepWidth%2 == 0)
					X[stepWidth-1] -= 2*c*X[stepWidth-2];

		
			 	X[0] -= 2*b*X[1];
				for(int i = 2; i < stepWidth - 1; i += 2)
				{
					X[i] -= b*(X[i-1]+X[i+1]);
				}
				if(stepWidth%2 == 1)
					X[stepWidth-1] -= 2*b*(X[stepWidth-2]);

				
				for(int i = 1; i < stepWidth - 1; i += 2)
				{
					X[i] -= a*(X[i-1]+X[i+1]);
				}
				if(stepWidth%2 == 1)
					X[stepWidth-1] -= 2*a*X[stepWidth-2];
				
				X[stepWidth-1] -= 2*a*X[stepWidth-2];
			}
			
			/*
		 	char filename[32]; // enough to hold all numbers up to 64-bits
			sprintf(filename, "img/tmp%di.pgm",  2*step+pass);
		 	writeFile(dst, filename, width, height, 255);
		 */
		 	 		
			if( step == iterations && pass == 2 )
			{
				for(int j = 0; j < height*width; j++)
				{
					dst[j] = tmp[j];
				}
			}
		}
		stepWidth *= 2;
		stepHeight *= 2;
	}
	delete tmp;
}

