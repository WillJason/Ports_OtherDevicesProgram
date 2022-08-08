#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "STK6037.h"
#include "key.h"
#include "mylib.h"
#include "SystemInit.h"
#include "uart.h"
#include "LCD1602.h"
#include "MSG.h"

#define VER       0x10 

/**********************************************************
                    系统调试相关设置
 - 、通过将FUNC_UARTDBG_EN这条宏设置为0或者1，来选择
     是否打开调试信息
**********************************************************/
#define  BAUD_NUM                   96//384//1152
#define  FUNC_UARTDBG_EN	        0//串口调试,烧芯片之前必须设置为[占560 byte]

#if FUNC_UARTDBG_EN
#define  DBG(str)   Uart_PutString(str)
#define  Put_u8(x)  Uart_PutChar(x)
#else
#define  DBG(str)
#define  Put_u8(x)
#endif




#define TRUE               1
#define FALSE              0

#define SENDLENTH         10   //发送数据总长度
#define MAX_MAIN_VOL      30   //最大音量

#define QUERYACKTIME      20
#define SYSRETURNTIME     20 

/************************************************************************
          看门狗
*************************************************************************/
#define WDT_ENABLE()    WDTCTRL = 0x87  //10xx x111 WDG打开	
#define WDT_DISABLE()   WDTCTRL = WDTCTRL & 0x7f //最高位置0
#define FEED_WDT()      WDTCTRL |= 0x40 

/************************************************************************
          LED灯 --- 播放指示灯
*************************************************************************/
#define PLAYLEDCNT          6      //按键有效灯的闪烁时间
#define PLAYLED_ON		    led = 0
#define PLAYLED_OFF		    led = 1
#define PLAYLED_FLASH	    led ^= 1

#define DISABLE_INT  	    EA = 0 ;
#define ENABLE_INT   	    EA = 1 ;

/************************************************************************
          硬件接口，管脚的定义
*************************************************************************/
#define KEYIO               P1  //按键扫描口
#define LCDDATAPORT         P0



sbit led   = P3^7 ;

//LCD1602接口定义
sbit rs	   = P2^6;		//
sbit rw    = P2^5;
sbit ep    = P2^7;







#endif