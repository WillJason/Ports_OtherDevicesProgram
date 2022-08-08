/********************************************************************************************
	文件: io_config.h
	作者: WillJason
	版本：V1.0
	日期：2016/1/28
	备注：这个文件提供了单片机引脚的声明  	   
*********************************************************************************************/
#ifndef __IO_CONFIG_H_
#define __IO_CONFIG_H_

#include "STC12C5A60S2.h"


sbit key0_port = P1^0;		             //按键0引脚
sbit key1_port = P1^1;			         //按键1引脚
sbit key2_port = P2^1;			         //按键2引脚

sbit Ps2_Data  = P2^0;					 //ps2键盘接收串行数据

sbit LED_DatPort = P0^0;				 //RGB灯带控制引脚
				  
sbit beep_port = P4^0;					 //板上蜂鸣器引脚，高电平蜂鸣器工作
sbit led_port = P3^6;			         //板上小led灯引脚，高电平灯亮起
sbit relay_port = P3^5;	                 //继电器控制引脚
sbit yx5200_play_busy_port = P3^4;       //yx5200播放状态引脚


#endif
