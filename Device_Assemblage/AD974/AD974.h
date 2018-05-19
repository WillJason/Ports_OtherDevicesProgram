#ifndef __AD974_H
#define __AD974_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//SPI驱动 代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////


#define	Addr1(x)	PDout(8)=GPIO(x)
#define	Addr2(x)	PDout(9)=GPIO(x)
#define	SPI_RC	PBout(12)

//SPI操作函数
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

void SPI2_Init(void);			 //初始化SPI口
void AD974_Init(void);
u16 SPI_READ_DATA(void);
void SPI_Start(void);
extern u16	AD974_ReadData(u8 AD974_Addr);

#endif


