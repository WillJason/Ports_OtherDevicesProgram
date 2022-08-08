/*�˳��������uPD6121ϵ��ң������ȡ����򣬽���ֵ��Im[2]�У���Irokʱ��������
*/
/*
��Ŀ���ƣ�TX-1c��չ�� ���ⲿ��
����оƬ��STC89C52
�ļ����ƣ�Inf_SetMotor
*/
#include <reg52.h>
#define uchar unsigned char
#define Imax 14000              //��Ծ���11.0592
#define Imin 8000
#define Inum1 1450
#define Inum2 700
#define Inum3 3000

uchar Im[4]={x00,0x00,0x00,0x00};
uchar f;

uchar IrOK;
unsigned long m,Tc;

void instersvr(void) interrupt 2 using 1
{
		Tc=TH0*256+TL0;
		TH0=0;
		TL0=0;
		if((Tc>Imin)&&(Tc<Imax))
		{
				m=0;
				f=1;
				return;
		}
		if(f==1)
		{
				if(Tc>Inum1&&Tc<Inum3)
				{
						Im[m/8]=Im[m/8]>>1|0x80;
						m++;
				}
				if(Tc>Inum2&&Tc<Inum1)
				{
						Im[m/8]=Im[m/8]>>1;
						m++;
				}
				if(m==32)
				{
						m=0;
						f=0;
						if(Im[2]==~Im[3])
						{
								IrOK=1;
						}
						else
						{
								IrOK=0;
						}
				}
		}
}
void Inf_Dispose(void)
{
		if(IrOK==1)
		{
				switch(Im[2])
				{
						case 0x0C:
							...
							break;
				}
				IrOK=0;
		}
}
void main()
{
		m=0;
		f=0;
		EA=1;
		
		IT1=1;
		EX1=1;
		
		TMOD=0x11;
		TH0=0;
		TL0=0;
		TR0=1;
		while(1)
		{
				Inf_Dispose();
		}
}