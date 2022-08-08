#ifndef __SYSTEMINIT_H__
#define __SYSTEMINIT_H__


//运行晶振：11.05926MHZ
#define TIMER0_H   (65536-1800)/256//定时2Ms
#define TIMER0_L   (65536-1800)%256

#define SYS_Fosc        11059200  //晶振频率			   
#define COMM_BAUD_RATE  9600      //串口波特率

#define OSC_FREQ        11059200  //11059200  
/******************************串口1的波特率********************************/
//T1作波特率发生器
//在波特率加倍情况下 
#define BAUD_57600    			256 - (OSC_FREQ/192L)/57600L    // 254 FF
#define BAUD_28800    			256 - (OSC_FREQ/192L)/28800L    // 254 FE
#define BAUD_19200    			256 - (OSC_FREQ/192L)/19200L    // 253 FD
#define BAUD_14400    			256 - (OSC_FREQ/192L)/14400L    // 252 FC
#define BAUD_9600     			256 - (OSC_FREQ/192L)/9600L     // 250 FA

/******************外界 的 接口********************/
void System_Init(void) ;
void RAM_Init(void) ;

#endif
