/*
���أ�Lonworksϵ�� FT5000
��ʾ��MC14489
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

unsigned long int RED_lowlevel_Num=0;					  /*����ɼ��͵�ƽ������*/

#define uchar unsigned char


char Red_i=0;											  /*����ɼ�����λ��������*/
boolean Red_One;												  /*����ɼ��˿�ֵ��*/


far unsigned long Get_Buffer[24];

IO_3 output bit ENA;

    
IO_4 input bit RED_IO;




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
priority when(io_changes(RED_IO)to 0)
{  
	unsigned char Receive_1,Receive_2;
	unsigned long int Receive;
	uchar j;
	uchar cmd,dis1,dis2;
	/*�½��غ��ȡIO�ڣ�����͵�ƽʱ�����ֵ*/
	Red_One=io_in(RED_IO);
	while(!Red_One)
	{
		RED_lowlevel_Num++;
		Red_One=io_in(RED_IO);
	}
	/*���յ���ÿ��λ�ĵ͵�ƽʱ�����ֵ���棬����ڷ�����Ƿ����*/
	Get_Buffer[Red_i]=RED_lowlevel_Num;
	Red_i++;
	RED_lowlevel_Num=0;/*׼�����յ���һ��λ����*/
	if(Red_i>=24)/*��������һ�κ��ⷢ�������*/
	{
		Receive=0;/*���ڱ���������������������ֵ���ܹ�12λ����������*/
		/*����һ��ȡ�õ���ֵ*/
		for(j=0;j<12;j++)
		{
			Receive=Receive*2;
			if(Get_Buffer[j]>120)
			Receive=Receive+1;
		}
		Receive_1=Receive-3700;
		/*�����ڶ���ȡ�õ���ֵ*/
		Receive=0;
		for(j=12;j<24;j++)
		{
			Receive=Receive*2;
			if(Get_Buffer[j]>120)
			Receive=Receive+1;
		}
		Receive_2=Receive-3700;
		/*�Ƚϣ������ȣ����������ȷ*/
		if(Receive_1==Receive_2)
		{
			/*���Ը��ݽ��յĺ����ִֵ����Ӧ�Ĳ���*/
			cmd=(((uchar)(Receive_1/1000))|0x80);
			dis1=((((uchar)((Receive_1%1000)/100))<<4)|((uchar)((Receive_1%100)/10)));
			dis2=((((uchar)(Receive_1%10))<<4)|0x00);
			watchdog_update();
			DSPDATA(cmd,dis1,dis2);
		}
		
		/*Ϊ�����µ�ң��������ֵ��׼��*/
		Receive=0;
		Red_i=0;
	}
	
}
//-----------------------reset-----------------------------------
when(reset)    
{ 
	
	DSPCMD(0x01);
	watchdog_update();
	DSPDATA(0x85,0x43,0x21);//
}

