#include<reg52.h>
#include<intrins.h>
#include"mouse.h"
#define daxiao 8
unsigned char code forward[]={0x11,0x93,0x82,0xc6,0x44,0x6c,0x28,0x39};
unsigned char code turnR[]={0x11,0x33,0x22,0x66,0x44,0xcc,0x88,0x99};
unsigned char code turnL[] ={0x11,0x99,0x88,0xcc,0x44,0x66,0x22,0x33};
unsigned char code cRight[] ={0x10,0x90,0x80,0xc0,0x40,0x60,0x20,0x30};
unsigned char code cLeft[] ={0x01,0x03,0x02,0x06,0x04,0x0c,0x08,0x09};
unsigned char zuhao = -1, x=0, y=0;
unsigned char map[daxiao][daxiao];//高四位记录从上一格到当前格的绝对方向，低四位记录此格是否走过(f走过,0没走过)
bit irQ=0, irZQ=0, irZ=0, irY=0, irYQ=0;
unsigned char table[] = {0xc0, 0xcf, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8};
sbit tube1 = P4^2;
sbit tube2 = P4^3;
unsigned char idata H[daxiao][daxiao];
unsigned char idata stackX[20];
unsigned char idata stackY[20];
void delay_ms(unsigned int z)
{
	unsigned char a, b;
	while(--z)
	{
		_nop_();
		a = 2;
		b = 199;
		do
		{
			while(--b);
		}while(--a);
	}
}
void go_right() //右转90度
{
	unsigned char i, j;
	for(j=0;j<50;j++)
	{
		for(i=0;i<8;i++)
		{		
			P1=turnR[i];
			delay_ms(2);
		}
	}
}
void go_left() //左转90度
{
	unsigned char i, j;
	for(j=0;j<50;j++)
	{
		for(i=0;i<8;i++)
		{		
			P1=turnL[i];
			delay_ms(2);
		}
	}
}
void go_back() //掉头
{
	unsigned char i, j;
	for(j=0;j<100;j++)
	{
		for(i=0;i<8;i++)
		{		
			P1=turnR[i];
			delay_ms(2);
		}
	}
}
void go_straight(unsigned char STEP) //走直线（向前）
{
	unsigned char ii, jj, n, flagz=0, flagy=0;
	for(jj=0;jj<STEP;jj++)
	{
		if(jj<75&&irQ==0)
		{
			if(irYQ)
			{
				for(n=0;n<8;n++)
				{
					P1=cRight[n];
					delay_ms(3);
				}
				flagy=1;
			}
			if(irZQ)
			{
				for(n=0;n<8;n++)
				{
					P1=cLeft[n];
					delay_ms(3);
				}
				flagz=1;
			}
		}
		for(ii=0;ii<8;ii++)
		{		
			P1=forward[ii];
			delay_ms(3);
		}
		if(flagz&&flagy)
		{
			jj++;
			flagz = flagy = 0;
		}
	}
	delay_ms(300);
}

unsigned char iszhongdian()
{
	unsigned char i, j;
	for(i=0;i<daxiao;i++)
	{
		for(j=0;j<daxiao;j++)
		{
			if((map[i][j]&0xf0)==0x00)
				return 0;
		}
	}
	if(x!=0||y!=0)
		return 0;
	return 1;
}
void initmap()
{
	unsigned char ii, jj;
	for(ii=0;ii<daxiao;ii++)
	{
		for(jj=0;jj<daxiao;jj++)
			map[ii][jj]==0x00;
	}
	map[0][0] = 0x70;
}
void display(unsigned char a, b)
{
	tube1 = 0;
	tube2 = 1;
	P0 = table[a];
	delay_ms(5);
	tube1 = 1;
	tube2 = 0;
	P0 = table[b];
	delay_ms(5);
}
void buildH()
{
	unsigned char k, i, j, disiwei, ff;
	Beep=0;delay_ms(300);Beep=1;
	for(i=0;i<daxiao;i++)
	{
		for(j=0;j<daxiao;j++)
		{
			H[i][j]=0;
		}
	}
	H[0][0]=1;k=0;
	while(1)
	{
		k++;
		for(i=0;i<daxiao;i++)
		{
			for(j=0;j<daxiao;j++)
			{
				if(H[i][j]==k)
				{
					disiwei = map[i][j]&0x0f;
					if((disiwei==0x00||disiwei==0x01||disiwei==0x02||disiwei==0x03||disiwei==0x04||disiwei==0x05||disiwei==0x06||disiwei==0x07)&&H[i][j+1]==0)
					{
						H[i][j+1]=k+1;
					}//上
					if((disiwei==0x00||disiwei==0x01||disiwei==0x02||disiwei==0x03||disiwei==0x08||disiwei==0x09||disiwei==0x0a||disiwei==0x0b)&&H[i+1][j]==0)
					{
						H[i+1][j]=k+1;
					}//右
					if((disiwei==0x00||disiwei==0x01||disiwei==0x04||disiwei==0x05||disiwei==0x08||disiwei==0x09||disiwei==0x0c||disiwei==0x0d)&&H[i][j-1]==0)
					{
						H[i][j-1]=k+1;
					}//下
					if((disiwei==0x00||disiwei==0x02||disiwei==0x04||disiwei==0x06||disiwei==0x08||disiwei==0x0a||disiwei==0x0c||disiwei==0x0e)&&H[i-1][j]==0)
					{
						H[i-1][j]=k+1;
					}//左
				}
			}
		}
		ff=1;
		for(i=0;i<daxiao;i++)
		{
			for(j=0;j<daxiao;j++)
			{
				if(H[i][j]==0)
					ff=0;
			}
		}
		if(ff)
			break;
	}
}
void shortest()
{
	unsigned char top=0, k, disiwei;
	delay_ms(1000);
	Beep=0;delay_ms(300);Beep=1;
	stackX[0]=0;stackY[0]=0;
	x = y = 7;k=H[7][7];
	while(1)
	{
		if(x==0&&y==0)
			break;
		disiwei = map[x][y]&0x0f;
		if((disiwei==0x00||disiwei==0x01||disiwei==0x04||disiwei==0x05||disiwei==0x08||disiwei==0x09||disiwei==0x0c||disiwei==0x0d)&&H[x][y-1]==(k-1))//下
		{
			top++;
			stackX[top]=x;stackY[top]=y-1;
			y--;
			k--;
		}//下
		else if((disiwei==0x00||disiwei==0x02||disiwei==0x04||disiwei==0x06||disiwei==0x08||disiwei==0x0a||disiwei==0x0c||disiwei==0x0e)&&H[x-1][y]==(k-1))//左
		{
			top++;
			stackX[top]=x-1;stackY[top]=y;
			x--;
			k--;
		}//左
	}
}
void main()
{
	unsigned char choose, D=0, d=0, flag, xx=0, yy=0, find;
	initTime2(); //初始化定时器2，并打开定时器开始计时
	delay_ms(1000);
	initmap();
	x = 0; y = 0;
	while(1)
	{
		find = 1;
		display(x, y);
		if(iszhongdian())
		{
			Beep=0;delay_ms(200);Beep=1;
			while(1);
		}
		if(find&&irY==0)//右方无挡板
		{
			switch((1+d)%4)//判断右边可不可以走
			{
				case 0:xx = x;yy = y+1;break;
				case 1:xx = x+1;yy = y;break;
				case 2:xx = x;yy = y-1;break;
				case 3:xx = x-1;yy = y;break;
			}
			if((map[xx][yy]&0x0f)==0x00)
			{choose = 0;find=0;D = (1+d)%4;} 
		}
		if(find&&irQ==0)//判断前方可不可以走
		{
			switch((0+d)%4)
			{
				case 0:xx = x;yy = y+1;break;
				case 1:xx = x+1;yy = y;break;
				case 2:xx = x;yy = y-1;break;
				case 3:xx = x-1;yy = y;break;
			}
			if((map[xx][yy]&0x0f)==0x00)
			{choose = 0;find=0;D = (0+d)%4;}
		}
		if(find&&irZ==0)
		{
			switch((3+d)%4)//获取小车左边格子的坐标
			{
				case 0:xx = x;yy = y+1;break;
				case 1:xx = x+1;yy = y;break;
				case 2:xx = x;yy = y-1;break;
				case 3:xx = x-1;yy = y;break;
			}
			if((map[xx][yy]&0x0f)==0x00)
			{	choose = 0;find=0;D = (3+d)%4;}
		}
		if(find)
			choose = 1;
	/*到此为止，得到了work/back的指令，且有走向下一格的绝对方向D*/
		if(choose==0) //work one step
		{
			flag = d - D;
			if(flag==0)
			{go_straight(105);d = D;}
			else if((flag==1)||(flag==-3))//左
			{go_left();go_straight(105);d = D;}
			else if((flag==-1)||(flag==3))
			{go_right();go_straight(105);d = D;}
			switch(d)//根据从上一步来的绝对方向，刷新坐标x,y
			{
				case 0:y++;break;
				case 1:x++;break;
				case 2:y--;break;
				case 3:x--;break;
			}
			switch(d)//刷新坐标后，把d记录进去
			{
				case 0:map[x][y] = 0x7f;break;
				case 1:map[x][y] = 0xbf;break;
				case 2:map[x][y] = 0xdf;break;
				case 3:map[x][y] = 0xef;break;
			}
			//refresh
		}
		else    //back one step
		{
			Beep=0;delay_ms(100);Beep=1;
			switch(map[x][y]&0xf0)//获取map高四位,获取当前来的方向,取反
			{
				case 0x70:D=2;break;
				case 0xb0:D=3;break;
				case 0xd0:D=0;break;
				case 0xe0:D=1;break;
			}
			flag = d - D;
			if(flag==0)
			{go_straight(105);}
			else if((flag==-1)||(flag==3))
			{go_right();go_straight(105);}
			else if((flag==1)||(flag==-3))//左转
			{go_left();go_straight(105);}
			else if((flag==2)||(flag==-2))
			{go_back();go_straight(103);}
			//turn
			d = D;
			switch(d)//根据从上一步来的绝对方向，刷新坐标x,y
			{
				case 0:y++;break;
				case 1:x++;break;
				case 2:y--;break;
				case 3:x--;break;
			}
		}
	}
}
void Time2() interrupt 5
{
	setTime2(5000);  //重装初值
	TF2 = 0; //标志清零
	zuhao++;  //每调用一次中断，组别加一
	if(zuhao%2==0) //现在是打开红外的组别
	{
		MOUSE_IR_ON(zuhao/2);
	}
	else  //现在是检测红外的组别
	{
		switch(zuhao)
		{
			case 1:if(irR1) irQ=0;
						 else     irQ=1;break;
			case 3:if(irR2) irZQ=0;
						 else     irZQ=1;break;
			case 5:if(irR3) irZ=0;
						 else     irZ=1;break;
			case 7:if(irR4) irY=0;
						 else     irY=1;break;
			case 9:if(irR5) irYQ=0;
						 else     irYQ=1;break;
		}
	}
	if(zuhao==9)  //若已经处理完最后一个方向，将zuhao重置
		zuhao = -1;
}