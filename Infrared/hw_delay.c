/*
主控：Lonworks系列 FT5000
显示：MC14489
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

unsigned long int RED_lowlevel_Num=0;					  /*红外采集低电平计数用*/

#define uchar unsigned char


char Red_i=0;											  /*红外采集接收位数计数用*/
boolean Red_One;												  /*红外采集端口值用*/


far unsigned long Get_Buffer[24];

IO_3 output bit ENA;

    
IO_4 input bit RED_IO;




IO_6 output bit CLK;
IO_7 output bit DATA;


void DSPCMD(uchar CMD)
{
		uchar i,DATA_Bit;
		io_set_direction(ENA,IO_DIR_OUT);
		io_out(ENA,0);//使能MC14489
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
		io_out(ENA,1);//bu使能MC14489
}

void DSPDATA(uchar DSCMD,uchar DSDATA1,uchar DSDATA2)
{
	uchar DSP,i,j;//
	i=0;
	//ENA=0;
	io_set_direction(ENA,IO_DIR_OUT);
	io_out(ENA,0);//使能MC14489
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
	/*下降沿后读取IO口，计算低电平时间计数值*/
	Red_One=io_in(RED_IO);
	while(!Red_One)
	{
		RED_lowlevel_Num++;
		Red_One=io_in(RED_IO);
	}
	/*将收到的每个位的低电平时间计数值保存，相对于发射端是反相的*/
	Get_Buffer[Red_i]=RED_lowlevel_Num;
	Red_i++;
	RED_lowlevel_Num=0;/*准备接收的下一个位数据*/
	if(Red_i>=24)/*当接收完一次红外发射的数据*/
	{
		Receive=0;/*用于保存最后处理的完整的所有码值，总共12位，发了两遍*/
		/*分析一次取得的码值*/
		for(j=0;j<12;j++)
		{
			Receive=Receive*2;
			if(Get_Buffer[j]>120)
			Receive=Receive+1;
		}
		Receive_1=Receive-3700;
		/*分析第二次取得的码值*/
		Receive=0;
		for(j=12;j<24;j++)
		{
			Receive=Receive*2;
			if(Get_Buffer[j]>120)
			Receive=Receive+1;
		}
		Receive_2=Receive-3700;
		/*比较，如果相等，代表接收正确*/
		if(Receive_1==Receive_2)
		{
			/*可以根据接收的红外键值执行相应的操作*/
			cmd=(((uchar)(Receive_1/1000))|0x80);
			dis1=((((uchar)((Receive_1%1000)/100))<<4)|((uchar)((Receive_1%100)/10)));
			dis2=((((uchar)(Receive_1%10))<<4)|0x00);
			watchdog_update();
			DSPDATA(cmd,dis1,dis2);
		}
		
		/*为接收新的遥控器按键值做准备*/
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

