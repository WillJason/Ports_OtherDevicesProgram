#ifndef __SYSTEMINIT_H__
#define __SYSTEMINIT_H__


//���о���11.05926MHZ
#define TIMER0_H   (65536-1800)/256//��ʱ2Ms
#define TIMER0_L   (65536-1800)%256

#define SYS_Fosc        11059200  //����Ƶ��			   
#define COMM_BAUD_RATE  9600      //���ڲ�����

#define OSC_FREQ        11059200  //11059200  
/******************************����1�Ĳ�����********************************/
//T1�������ʷ�����
//�ڲ����ʼӱ������ 
#define BAUD_57600    			256 - (OSC_FREQ/192L)/57600L    // 254 FF
#define BAUD_28800    			256 - (OSC_FREQ/192L)/28800L    // 254 FE
#define BAUD_19200    			256 - (OSC_FREQ/192L)/19200L    // 253 FD
#define BAUD_14400    			256 - (OSC_FREQ/192L)/14400L    // 252 FC
#define BAUD_9600     			256 - (OSC_FREQ/192L)/9600L     // 250 FA

/******************��� �� �ӿ�********************/
void System_Init(void) ;
void RAM_Init(void) ;

#endif
