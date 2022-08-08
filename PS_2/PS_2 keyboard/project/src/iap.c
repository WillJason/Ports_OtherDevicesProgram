/********************************************************************************************
	�ļ�: iap.c
	����: WillJason
	�汾��V1.0
	��ע������ļ��ṩ��iapģ��Ĺ̼���   	   
*********************************************************************************************/
#include "STC12C5A60S2.h"
#include "iap.h"
#include "intrins.h"

#define   CMD_IDLE     0         //Stand-By        ����ָ��
#define   CMD_READ     1         //Byte-Read	   �ֽڶ�ָ��
#define   CMD_PROGRAM  2         //Byte-Program	   �ֽڱ��ָ��
#define   CMD_ERASE    3         //Sector-Erase    ��������ָ��
#define   ENABLE_IAP   0x83      //if SYSCLK<12MHz ʹ��IAP
  
/******************************************************************************************
	�������ܣ�IAP�������״̬
	��ڲ�������
	���ز�������	
	����˵������
*******************************************************************************************/
void iap_idle()
{
	IAP_CONTR = 0;          //Close IAP function
	IAP_CMD = 0;            //Clear command to standby
	IAP_TRIG = 0;           //Clear trigger register
	IAP_ADDRH = 0x80;       //Data ptr point to non-EEPROM area
	IAP_ADDRL = 0;          //Clear IAP address to prevent misuse
}
/******************************************************************************************
	�������ܣ�IAP���ֽ�
	��ڲ�����addr���ֽڵ�ַ
	���ز������������ֽ�	
	����˵������
*******************************************************************************************/
uint8_t iap_read_byte(uint16_t addr)
{
	uint8_t dat;       //Data buffer
	IAP_CONTR = ENABLE_IAP;  //Open IAP function, and set wait time
	IAP_CMD = CMD_READ;      //Set ISP/IAP/EEPROM READ command
	IAP_ADDRL = addr;        //Set ISP/IAP/EEPROM address low
	IAP_ADDRH = addr >> 8;   //Set ISP/IAP/EEPROM address high
	IAP_TRIG = 0x5a;         //Send trigger command1 (0x5a)
	IAP_TRIG = 0xa5;         //Send trigger command2 (0xa5)
	_nop_();                 //MCU will hold here until ISP/IAP/EEPROM  //operation complete
	dat = IAP_DATA;          //Read ISP/IAP/EEPROM data
	iap_idle();              //Close ISP/IAP/EEPROM function
	return dat;              //Return Flash data
}
/******************************************************************************************
	�������ܣ�IAP����ֽ�
	��ڲ�����addr����̵�ַ dat������ֽ�
	���ز�������	
	����˵������
*******************************************************************************************/
void iap_program_byte(uint16_t addr, uint8_t dat)
{
	IAP_CONTR = ENABLE_IAP;  //Open IAP function, and set wait time
	IAP_CMD = CMD_PROGRAM;   //Set ISP/IAP/EEPROM PROGRAM command
	IAP_ADDRL = addr;        //Set ISP/IAP/EEPROM address low
	IAP_ADDRH = addr >> 8;   //Set ISP/IAP/EEPROM address high
	IAP_DATA = dat;          //Write ISP/IAP/EEPROM data
	IAP_TRIG = 0x5a;         //Send trigger command1 (0x5a)
	IAP_TRIG = 0xa5;         //Send trigger command2 (0xa5)
	_nop_();                 //MCU will hold here until ISP/IAP/EEPROM 	 //operation complete
	iap_idle();
}
/******************************************************************************************
	�������ܣ�IAP��������
	��ڲ�����������ַ
	���ز�������	
	����˵������
*******************************************************************************************/
void iap_erase_sector(uint16_t addr)
{
	IAP_CONTR = ENABLE_IAP;   //Open IAP function, and set wait time
	IAP_CMD = CMD_ERASE;      //Set ISP/IAP/EEPROM ERASE command
	IAP_ADDRL = addr;         //Set ISP/IAP/EEPROM address low
	IAP_ADDRH = addr >> 8;    //Set ISP/IAP/EEPROM address high
	IAP_TRIG = 0x5a;          //Send trigger command1 (0x5a)
	IAP_TRIG = 0xa5;          //Send trigger command2 (0xa5)
	_nop_();                  //MCU will hold here until ISP/IAP/EEPROM  //operation complete
	iap_idle();
}
