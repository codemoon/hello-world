#include<reg52.h>
#include<intrins.h>
sbit BEEP = P2^4;
unsigned char j;
unsigned int i;
void main()
{
	while(1)
	{
		for(i=0;i<400;i++)
		{
			for(j=0;j<75;j++);
			BEEP = ~BEEP;
		}
		for(i=0;i<3000;i++)
		{
			for(j=0;j<100;j++);
			BEEP = ~BEEP;
		}
		for(i=0;i<200;i++)
		{
			for(j=0;j<150;j++);
			BEEP = ~BEEP;
		}
	}
}



