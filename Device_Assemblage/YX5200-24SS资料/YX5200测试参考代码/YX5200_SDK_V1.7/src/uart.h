#ifndef __SERIAL_H__
#define __SERIAL_H__



#define RESEND_MAX      5		  //����ط�����
/****************************************************************************************************
                                   �ο�����ָ��

[��һ��]        7E 10 06 01 00 00 00 FF E9 EF	
[��һ��]	       7E 10 06 02 00 00 00 FF E8 EF	
[ָ����Ŀ]	  7E 10 06 03 00 00 01 FF E6 EF   ָ����һ��
[����+] 	       7E 10 06 04 00 00 00 FF E6 EF	
[����-]	       7E 10 06 05 00 00 00 FF E5 EF	
[ָ������]	  7E 10 06 06 00 00 0F FF D5 EF	ָ������Ϊ15��
[ָ��EQ]    	  7E 10 06 07 00 00 01 FF E2 EF	ָ��EQΪPop
[ָ������ģʽ]  7E 10 06 08 00 00 00 FF E2 EF	ָ������ģʽΪѭ������
                7E 10 06 08 00 00 02 FF E0 EF   ����ѭ��
                7E 10 06 09 00 00 04 FF dd EF   SPIģʽ
[ָ�������豸]  7E 10 06 09 00 00 01 FF E0 EF	ָ�������豸ΪUDISK
[����˯��ģʽ]  7E 10 06 0A 00 00 00 FF E0 EF	
[ģ�鸴λ]	  7E 10 06 0C 00 00 00 FF DE EF	
[����]	       7E 10 06 0D 00 00 00 FF DD EF	
[��ͣ] 	       7E 10 06 0E 00 00 00 FF DC EF	
[ָ���ļ���]	   7E 10 06 0F 00 00 02 FF D9 EF	ָ������Ϊ"2"���ļ��в���
[��ѯ����]	  7E 10 06 43 00 00 00 FF A7 EF	
[��ѯ��ǰEQ]	  7E 10 06 44 00 00 00 FF A6 EF 	
[��ѯ����ģʽ]  7E 10 06 45 00 00 00 FF A5 EF	
[��ѯ�汾]	  7E 10 06 46 00 00 00 FF A4 EF	
[U TOTAL]	       7E 10 06 47 00 00 00 FF A3 EF	
[TF TOTAL]	  7E 10 06 48 00 00 00 FF A2 EF	
[FLASH TOTAL]	  7E 10 06 49 00 00 00 FF A1 EF	
[STAY] 	       7E 10 06 4A 00 00 00 FF A0 EF	
[U ��ǰ��Ŀ]	  7E 10 06 4B 00 00 00 FF 9F EF	
[TF ��ǰ��Ŀ]	  7E 10 06 4C 00 00 00 FF 9E EF	
****************************************************************************************************/

/*****************************************************************************************************
 - ������ö�ٵķ�ʽ����������Ҫ�õ���ָ���г���
 - ���ɻ���Բ��վ����ģ���ֲᡣ
 - ���µ�����������ͨ��������������ľ��庬��
*****************************************************************************************************/
enum
{ 
    UARTCMD_NEXT_FILE = 0x01,//��һ��
    UARTCMD_PREV_FILE , //��һ��   
    UARTCMD_NUM_POINT , //ָ����Ŀ
    UARTCMD_VOL_UP,     //����+
    UARTCMD_VOL_DOWN,   //����-
    UARTCMD_VOL_POINT,  //ָ������
    UARTCMD_EQ_POINT,   //ָ��EQ
    UARTCMD_LOOP_PLAY,//ָ��ѭ��������Ŀ
    UARTCMD_PLAYDEVICE,//ָ�������豸
    UARTCMD_SLEEP_MODE,//����˯�ߵ͹���
    
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
    UART_RECV_IDLE= 0 ,//���ڽ��տ���ģʽ
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



/************************************* ģ�����Ľӿ�--���� ***************************************/
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


