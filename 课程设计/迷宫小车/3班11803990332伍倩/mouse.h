#include<reg52.h>
#include<intrins.h>
sfr P4=0xe8;
//�����ַ�ӿڶ���
sbit A0=P4^0;
sbit A1=P2^0;
sbit A2=P2^7;
sbit Beep = P3^7;
//���⴫���������źſڶ���
sbit irR1=P2^1; //ǰ
sbit irR2=P2^2; //��ǰ
sbit irR3=P2^3; //��
sbit irR4=P2^4; //��
sbit irR5=P2^5; //��ǰ
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