/*MC124489是一款SPI串行接口LED驱动器*/
#pragma db oe sb
#define uchar unsigned char

sbit DATA =0x90;//定义P1.0为DATA In
sbit CLK  =0x91;//定义P1.1为CLOCK
sbit ENA  =0x92;//定义P1.2为ENABLE

void DSPCMD(uchar CMD)
{//单字节命令函数，写入MC14489内部设置寄存器
	uchar i;
	ENA =0;//使能MC14489
	for (1=8;i>=1;i--)
	{
			DATA=CMD&0X80;
			CMD =CMD<1;
			CLK =0;
			CLK =1;
	}
	ENA=1;//禁止MC14489
} 

void DSPDATA(uchar DSCMD,uchar DSDATA,uchar DSDATA2)
{
		uchar DSP,i,j;
		i=0;
		ENA=0;
		while(i<24)
		{
				if ( i<8 ){DSP=DSCMD;}
			  else if(i<16){DSP=DSDATA;}
				else{DSP=DSDATA2;}
				for(j=8;j>=1;j--)
				{
					DATA=DSP&0x80;
					DSP =DSP<<1;
					CLK=0;
					CLK=1;
				}
				i=i+8;
		}
		ENA=1;//禁止MC14489
}

void main()
{
		DSPCMD(0x01);//单字节命令，点亮所有LED
		DSPDATA(0x85,0x43,0x21);//在5位的LED上分别显示54321，满亮度显示
		DSPDATA(0x01,0x23,0x45);//在5位的LED上分别显示12345，半亮度显示
}



