/********************************************************************************************
	�ļ�: iap.h
	����: WillJason
	�汾��V1.0
	���ڣ�2016/1/28
	��ע������ļ��ṩ��iap�̼��������  	   
*********************************************************************************************/
#ifndef  __IAP_H_
#define  __IAP_H_

#define   IAP_ADDRESS   0x0000	 //IAP�洢��ַ��ʼ��ַ����
extern uint8_t iap_read_byte(uint16_t addr);
extern void iap_program_byte(uint16_t addr, uint8_t dat);
extern void iap_erase_sector(uint16_t addr);

#endif

