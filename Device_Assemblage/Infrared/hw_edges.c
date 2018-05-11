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

#define uchar unsigned char

unsigned int Edge_Num;
far unsigned long Get_Buffer[48];
unsigned long  pre_load=1369;

IO_3 output bit ENA;
IO_4 input edgelog clock(7) Red;/*定义边沿记录输入对象，周期25.6us，*/       
IO_4 input bit RED_IO;/*指定输入的端口*/
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
{  char i;
   unsigned char Receive_1,Receive_2;
   unsigned long int Receive;
   uchar cmd,dis1,dis2;
	   Receive=0;/*用于保存最后处理的完整的所有码值*/
	   Edge_Num=io_in(Red,Get_Buffer,48);/*返回周期的数量*/
	   for(i=0;i<Edge_Num;i=i+2)
	   Get_Buffer[i]=Get_Buffer[i]+pre_load;/*需要加上pre_load的值*/
	   /*分析一次取得的码值*/
	   for(i=0;i<24;i=i+2)
	      {   Receive=Receive*2;
	          if(Get_Buffer[i]>32)
	          Receive=Receive+1;
	      }
	   Receive_1=Receive-3700;
	   /*分析第二次取得的码值*/
	   Receive=0;
	   for(i=24;i<Edge_Num;i=i+2)
	      {Receive=Receive*2;
	       if(Get_Buffer[i]>32)
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
}	     
//-----------------------reset-----------------------------------
when(reset)    
{ 
	
	io_edgelog_preload(pre_load); /*用于指定每次测量周期的最大值*/
	DSPCMD(0x01);
	watchdog_update();
	DSPDATA(0x85,0x43,0x21);//
}

