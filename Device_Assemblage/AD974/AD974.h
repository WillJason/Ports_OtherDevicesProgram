#ifndef __AD974_H
#define __AD974_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//SPI���� ����
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////


#define	Addr1(x)	PDout(8)=GPIO(x)
#define	Addr2(x)	PDout(9)=GPIO(x)
#define	SPI_RC	PBout(12)

//SPI��������
#define SPI_SCL    PBout(13) //SCL
#define SPI_SDA    PBin(14) //SDA

#define	AD974_Addr0()	Addr2(0);Addr1(0)
#define	AD974_Addr1()	Addr2(0);Addr1(1)
#define	AD974_Addr2()	Addr2(1);Addr1(0)
#define	AD974_Addr3()	Addr2(1);Addr1(1)

#define	route0	0
#define	route1	1
#define	route2	2
#define	route3	3

void SPI2_Init(void);			 //��ʼ��SPI��
void AD974_Init(void);
u16 SPI_READ_DATA(void);
void SPI_Start(void);
extern u16	AD974_ReadData(u8 AD974_Addr);

#endif


