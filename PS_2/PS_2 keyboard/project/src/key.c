/********************************************************************************************
	文件: key.c
	作者: WillJason
	版本：V1.0
	日期：2016/1/28
	备注：这个文件提供了按键的固件库  	   
*********************************************************************************************/
#include "STC12C5A60S2.h"
#include "key.h"
#include "io_config.h"


/*******************************************************************************************
	函数功能：按键检测函数
	入口参数：无
	返回参数：0x00:无按键被按下，0x01:按键0被按下，0x02:按键1被按下	
	函数说明：一次按下后，需要抬起，再一次的按下才会识别，即只检测到一次
********************************************************************************************/
uint8_t key_down_edge_function(void)
{
	uint8_t key_value = 0;
	static uint8_t key_down_flag[5] = {0,0};
	static uint8_t key_down_conuter[5] = {5,5};		
	/*******************按键0判断*********************/		
	if((key0_port != 1) && (key_down_flag[0] != 1))			 //按下判断
	{
		if(key_down_conuter[0] < 10) 
		{
			key_down_conuter[0]++;
		}
		else
		{
			key_down_flag[0] = 1;
			key_value |= 0x01; 					             //确定按键被按下
		}	
	}
	else if((key0_port == 1) && (key_down_flag[0] != 0))	  //抬起判断
	{
		if(key_down_conuter[0] > 1) 
		{
			key_down_conuter[0]--;
		}
		else 
		{
			key_down_flag[0] = 0;							//确定按键被抬起
			key_value &= ~(0x01);		 
		}		
	}
	else
	{
		key_value &= ~(0x01);	
	} 
	/*******************按键1判断*********************/	
	if((key1_port != 1) && (key_down_flag[1] != 1))			//按下判断
	{
		if(key_down_conuter[1] < 10) 
		{
			key_down_conuter[1]++;
		}
		else
		{
			key_down_flag[1] = 1;
			key_value |= 0x02; 						        //确定按键被按下
		}
	
	}
	else if((key1_port == 1) && (key_down_flag[1] != 0))	//抬起判断
	{
		if(key_down_conuter[1] > 1) 
		{
			key_down_conuter[1]--;
		}
		else
		{
		 	key_down_flag[1] = 0;
			key_value &= ~(0x02);		 				    //确定按键被抬起
		}										       	
	}
	else
	{
		key_value &= ~(0x02);	
	} 

	return key_value;
}

/*******************************************************************************************
	函数功能：按键电平检测检测函数
	入口参数：无
	返回参数：1:低电平   0：高电平	
	函数说明：
********************************************************************************************/
uint8_t key_level_cheak_function(void)
{
    static uint8_t level_statu = 0;  
	static uint8_t level_conuter = 0;
	if(key2_port == 0)	
    { 
		if(level_conuter < 5)	{ level_conuter++;} 
		else level_statu = 1;						  		
    }
    else
	{
		if(level_conuter > 0)	 { level_conuter--;}     
		else level_statu = 0;	
	}
	return level_statu;		  	  
}












