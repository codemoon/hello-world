#include"mouse.h"
void setTime2(unsigned int us) //װT2�ĳ�ֵ��us���ж����ʱ��
{
	TH2=(65536-us)/256;
	RCAP2H=(65536-us)/256;
	TL2=(65536-us)%256;
	RCAP2L=(65536-us)%256;
}
void initTime2()
{
	EA = 1;
	ET2 = 1;��
	setTime2(5000);//װ��ֵ
	TR2=1;
}
