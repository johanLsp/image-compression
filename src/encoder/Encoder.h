#ifndef ENCODER_ENCODER_H 
#define ENCODER_ENCODER_H 
class Encoder
{
	public:
		virtual void test() = 0;
		virtual int encode(char* src, char* dst) = 0;
		virtual void decode(char* src, char* dst) = 0;
	
	private:

};

#endif
