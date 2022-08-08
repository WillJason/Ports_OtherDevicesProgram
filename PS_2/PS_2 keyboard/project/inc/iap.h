/********************************************************************************************
	文件: iap.h
	作者: WillJason
	版本：V1.0
	日期：2016/1/28
	备注：这个文件提供了iap固件库的声明  	   
*********************************************************************************************/
#ifndef  __IAP_H_
#define  __IAP_H_

#define   IAP_ADDRESS   0x0000	 //IAP存储地址起始地址定义
extern uint8_t iap_read_byte(uint16_t addr);
extern void iap_program_byte(uint16_t addr, uint8_t dat);
extern void iap_erase_sector(uint16_t addr);

#endif

