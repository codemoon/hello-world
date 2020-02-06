#include<reg52.h>
#include<intrins.h>
sfr P4=0xe8;
//红外地址接口定义
sbit A0=P4^0;
sbit A1=P2^0;
sbit A2=P2^7;
sbit Beep = P3^7;
//红外传感器接收信号口定义
sbit irR1=P2^1; //前
sbit irR2=P2^2; //左前
sbit irR3=P2^3; //左
sbit irR4=P2^4; //右
sbit irR5=P2^5; //右前
extern bit irQ, irZQ, irZ, irY, irYQ;
#define MOUSE_IR_ON(GROUP_NO) \
do\
{	\
	A0=(GROUP_NO)&0x01; \
	A1=(GROUP_NO)&0x02; \
	A2=(GROUP_NO)&0x04; \
}while(0)
void setTime2(unsigned int us);
void initTime2();