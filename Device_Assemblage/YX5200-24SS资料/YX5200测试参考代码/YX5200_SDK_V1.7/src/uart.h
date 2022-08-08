#ifndef __SERIAL_H__
#define __SERIAL_H__



#define RESEND_MAX      5		  //最大重发次数
/****************************************************************************************************
                                   参考测试指令

[下一首]        7E 10 06 01 00 00 00 FF E9 EF	
[上一首]	       7E 10 06 02 00 00 00 FF E8 EF	
[指定曲目]	  7E 10 06 03 00 00 01 FF E6 EF   指定第一曲
[音量+] 	       7E 10 06 04 00 00 00 FF E6 EF	
[音量-]	       7E 10 06 05 00 00 00 FF E5 EF	
[指定音量]	  7E 10 06 06 00 00 0F FF D5 EF	指定音量为15级
[指定EQ]    	  7E 10 06 07 00 00 01 FF E2 EF	指定EQ为Pop
[指定播放模式]  7E 10 06 08 00 00 00 FF E2 EF	指定播放模式为循环播放
                7E 10 06 08 00 00 02 FF E0 EF   单曲循环
                7E 10 06 09 00 00 04 FF dd EF   SPI模式
[指定播放设备]  7E 10 06 09 00 00 01 FF E0 EF	指定播放设备为UDISK
[进入睡眠模式]  7E 10 06 0A 00 00 00 FF E0 EF	
[模块复位]	  7E 10 06 0C 00 00 00 FF DE EF	
[播放]	       7E 10 06 0D 00 00 00 FF DD EF	
[暂停] 	       7E 10 06 0E 00 00 00 FF DC EF	
[指定文件夹]	   7E 10 06 0F 00 00 02 FF D9 EF	指定名字为"2"的文件夹播放
[查询音量]	  7E 10 06 43 00 00 00 FF A7 EF	
[查询当前EQ]	  7E 10 06 44 00 00 00 FF A6 EF 	
[查询播放模式]  7E 10 06 45 00 00 00 FF A5 EF	
[查询版本]	  7E 10 06 46 00 00 00 FF A4 EF	
[U TOTAL]	       7E 10 06 47 00 00 00 FF A3 EF	
[TF TOTAL]	  7E 10 06 48 00 00 00 FF A2 EF	
[FLASH TOTAL]	  7E 10 06 49 00 00 00 FF A1 EF	
[STAY] 	       7E 10 06 4A 00 00 00 FF A0 EF	
[U 当前曲目]	  7E 10 06 4B 00 00 00 FF 9F EF	
[TF 当前曲目]	  7E 10 06 4C 00 00 00 FF 9E EF	
****************************************************************************************************/

/*****************************************************************************************************
 - 这里用枚举的方式，将所有需要用到的指令列出来
 - 有疑惑可以参照具体的模块手册。
 - 以下的命名均可以通过翻译明白命令的具体含义
*****************************************************************************************************/
enum
{ 
    UARTCMD_NEXT_FILE = 0x01,//下一曲
    UARTCMD_PREV_FILE , //上一曲   
    UARTCMD_NUM_POINT , //指定曲目
    UARTCMD_VOL_UP,     //音量+
    UARTCMD_VOL_DOWN,   //音量-
    UARTCMD_VOL_POINT,  //指定音量
    UARTCMD_EQ_POINT,   //指定EQ
    UARTCMD_LOOP_PLAY,//指定循环播放曲目
    UARTCMD_PLAYDEVICE,//指定播放设备
    UARTCMD_SLEEP_MODE,//进入睡眠低功耗
    
    NO_UARTCMD = 0X0B,
    UARTCMD_RESET,
    UARTCMD_MUSIC_PLAY,
    UARTCMD_MUSIC_PAUSE,
    UARTCMD_MUSIC_FOLDER= 0X0F,
    UARTCMD_KALA ,
    UARTCMD_PLAYALL = 0x11 ,

    UARTQUR_STAY1 = 0X3C ,
    UARTQUR_STAY2 ,
    UARTQUR_STAY3 ,
    UARTQUR_ERR ,
    UARTQUR_STAY5 , 
    UARTQUR_ACK = 0X41,
    UARTQUR_STATUS  ,
    UARTQUR_VOL ,
    UARTQUR_EQ ,
    UARTQUR_PLAYMODE ,
    UARTQUR_VER , 
    UARTQUR_UDISK_TOTAL= 0X47,
    UARTQUR_TF_TOTAL,
    UARTQUR_FLASH_TOTAL,
    UARTQUR_STAY6,
    UARTQUR_UDISK_FILE,
    UARTQUR_TF_FILE,
    UARTQUR_FLASH_FILE,    
} ;

enum
{
    UART_RECV_IDLE= 0 ,//串口接收空闲模式
    UART_RECV_VER ,  
    UART_RECV_LENTH,
    UART_RECV_CMD,
    UART_RECV_FEEDBACK ,
    UART_RECV_DATAH,
    UART_RECV_DATAL,
    UART_RECV_CHECKSUMH,
    UART_RECV_CHECKSUML,    
    UART_RECV_OVER,
} ;



/************************************* 模块对外的接口--函数 ***************************************/
void Uart_PutByte(INT8U ch) ;
void Uart_PutChar(INT8U dat) ;
void Uart_PutString(INT8U* value) ;
void Send(INT8U len,bit flag,INT8U time) ;
void SendMaster_Collide_Task(void) ;
void ResendClean(void) ;
void Uart_SendCMD(INT8U CMD ,INT8U feedback , INT16U dat);
void UartSend_Collide_Task(void);
void Uart_communication(void);

#endif


