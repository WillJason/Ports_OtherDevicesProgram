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
                    ϵͳ�����������
 - ��ͨ����FUNC_UARTDBG_EN����������Ϊ0����1����ѡ��
     �Ƿ�򿪵�����Ϣ
**********************************************************/
#define  BAUD_NUM                   96//384//1152
#define  FUNC_UARTDBG_EN	        0//���ڵ���,��оƬ֮ǰ��������Ϊ[ռ560 byte]

#if FUNC_UARTDBG_EN
#define  DBG(str)   Uart_PutString(str)
#define  Put_u8(x)  Uart_PutChar(x)
#else
#define  DBG(str)
#define  Put_u8(x)
#endif




#define TRUE               1
#define FALSE              0

#define SENDLENTH         10   //���������ܳ���
#define MAX_MAIN_VOL      30   //�������

#define QUERYACKTIME      20
#define SYSRETURNTIME     20 

/************************************************************************
          ���Ź�
*************************************************************************/
#define WDT_ENABLE()    WDTCTRL = 0x87  //10xx x111 WDG��	
#define WDT_DISABLE()   WDTCTRL = WDTCTRL & 0x7f //���λ��0
#define FEED_WDT()      WDTCTRL |= 0x40 

/************************************************************************
          LED�� --- ����ָʾ��
*************************************************************************/
#define PLAYLEDCNT          6      //������Ч�Ƶ���˸ʱ��
#define PLAYLED_ON		    led = 0
#define PLAYLED_OFF		    led = 1
#define PLAYLED_FLASH	    led ^= 1

#define DISABLE_INT  	    EA = 0 ;
#define ENABLE_INT   	    EA = 1 ;

/************************************************************************
          Ӳ���ӿڣ��ܽŵĶ���
*************************************************************************/
#define KEYIO               P1  //����ɨ���
#define LCDDATAPORT         P0



sbit led   = P3^7 ;

//LCD1602�ӿڶ���
sbit rs	   = P2^6;		//
sbit rw    = P2^5;
sbit ep    = P2^7;







#endif