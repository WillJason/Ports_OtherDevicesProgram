/*MC124489��һ��SPI���нӿ�LED������*/
#pragma db oe sb
#define uchar unsigned char

sbit DATA =0x90;//����P1.0ΪDATA In
sbit CLK  =0x91;//����P1.1ΪCLOCK
sbit ENA  =0x92;//����P1.2ΪENABLE

void DSPCMD(uchar CMD)
{//���ֽ��������д��MC14489�ڲ����üĴ���
	uchar i;
	ENA =0;//ʹ��MC14489
	for (1=8;i>=1;i--)
	{
			DATA=CMD&0X80;
			CMD =CMD<1;
			CLK =0;
			CLK =1;
	}
	ENA=1;//��ֹMC14489
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
		ENA=1;//��ֹMC14489
}

void main()
{
		DSPCMD(0x01);//���ֽ������������LED
		DSPDATA(0x85,0x43,0x21);//��5λ��LED�Ϸֱ���ʾ54321����������ʾ
		DSPDATA(0x01,0x23,0x45);//��5λ��LED�Ϸֱ���ʾ12345����������ʾ
}



