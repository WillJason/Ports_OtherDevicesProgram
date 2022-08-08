/*
���أ�Lonworksϵ�� FT5000
��ʾ��MC14489
ѹƵת��оƬ��LM331
*/
#pragma num_alias_table_entries 1
#pragma num_addr_table_entries 1
#pragma relaxed_casting_on
#pragma app_buf_in_size 66
#pragma app_buf_in_count 3
#pragma app_buf_out_size 66
#pragma app_buf_out_count 3
#pragma app_buf_out_priority_count 0

#pragma net_buf_in_size 66
#pragma net_buf_in_count 5
#pragma net_buf_out_size 66
#pragma net_buf_out_count 2
#pragma net_buf_out_priority_count 0
#pragma enable_io_pullups

#include <NM_STE.H>
#include <byte.h>
#include <mem.h>
#include <addrdefs.h>
#include <access.h>
#include <msg_addr.h> 
#include <status.h>
#include <control.h>
#include <float.h>
#include <stdlib.h>
#include <netmgmt.h>

unsigned long int caiji_Num=0;
#define uchar unsigned char

const float_type f1000={0X0,0X44,0X0,0X7A,0X0000};//1000
const float_type f2500={0X0,0X45,0X0,0X1C,0X4000};//2500

IO_0 output bit yimaqi_a;
IO_1 output bit yimaqi_b;
IO_2 output bit yimaqi_c;
IO_3 output bit ENA;
IO_5 input period mux  invert clock(1) lm331_out;
IO_6 output bit CLK;
IO_7 output bit DATA;


void DSPCMD(uchar CMD)
{
		uchar i,DATA_Bit;
		io_set_direction(ENA,IO_DIR_OUT);
		io_out(ENA,0);//ʹ��MC14489
		watchdog_update();
		for(i=8;i>=1;i--)//
		{
				//DATA_Bit=CMD&0x80;
				io_out(DATA,CMD&0x80);
				CMD=CMD<<1;
				io_set_direction(CLK,IO_DIR_OUT);
				//CLK=0;
				io_out(CLK,0);
				//CLK=1;
				io_out(CLK,1);
		}
		watchdog_update();
		//ENA=1;//
		io_set_direction(ENA,IO_DIR_OUT);
		io_out(ENA,1);//buʹ��MC14489
}

void DSPDATA(uchar DSCMD,uchar DSDATA1,uchar DSDATA2)
{
	uchar DSP,i,j;//
	i=0;
	//ENA=0;
	io_set_direction(ENA,IO_DIR_OUT);
	io_out(ENA,0);//ʹ��MC14489
	watchdog_update();
	while(i<24)
	{
		if(i<8)
		{DSP=DSCMD;}
		else if(i<16){DSP=DSDATA1;}
		else{DSP=DSDATA2;}
		watchdog_update();
		for(j=8;j>=1;j--)
		{
			//DATA=DSP&0x80;
			io_out(DATA,DSP&0x80);
			DSP=DSP<<1;
			//CLK=0;
			io_out(CLK,0);
			//CLK=1;
			io_out(CLK,1);
		}
		i=i+8;
	}
	watchdog_update();
	//ENA=1;//
	io_set_direction(ENA,IO_DIR_OUT);
	io_out(ENA,1);//disable Mc14489
}


/*
	��ʱ��/���������������������ź������أ��½��أ�����һ�������أ��½��أ�֮
	���ʱ���������������ڡ�
		�������ڣ�ns��=�����Ĵ����д�ŵ�ֵX�������ڣ�ns��
		�������ڣ�ns��=2000X2^(clock)/CLK1(MHZ)
		��������=Input_Num*400ns��
		������Ƶ��=1s/����������(ns)=1/��Input_Num*400ns/1000000000��=(2500/Input_Num)*1000��
		
*/
priority when(io_update_occurs(lm331_out))
{
	uchar cmd,dis1,dis2;
	unsigned long int Freq_resault;
 	float_type temp,caiji_Num_Float,F_Float;
	caiji_Num=0;
	caiji_Num=io_in(lm331_out);
	
	watchdog_update();
	fl_from_ulong(caiji_Num,&caiji_Num_Float);  /*ת��float������*/
	fl_div(&f2500,&caiji_Num_Float,&temp); 		/*2500��Input_Num*/
	fl_mul(&temp,&f1000,&F_Float);   		/*�ٳ���1000*/
	Freq_resault=fl_to_ulong(&F_Float);   			/*ת��Ϊ�޷�������*/
	
	cmd=(((uchar)(Freq_resault/1000))|0x80);
	dis1=((((uchar)((Freq_resault%1000)/100))<<4)|((uchar)((Freq_resault%100)/10)));
	dis2=((((uchar)(Freq_resault%10))<<4)|0x00);
	watchdog_update();
	DSPDATA(cmd,dis1,dis2);
}
//-----------------------reset-----------------------------------
when(reset)    
{ 
	
	io_set_direction(yimaqi_a,IO_DIR_OUT);
	io_out(yimaqi_a,1);
	watchdog_update();
	io_set_direction(yimaqi_b,IO_DIR_OUT);
	io_out(yimaqi_b,1);
	io_set_direction(yimaqi_c,IO_DIR_OUT);
	io_out(yimaqi_c,1);
	DSPCMD(0x01);
	watchdog_update();
	DSPDATA(0x85,0x43,0x21);//
}

