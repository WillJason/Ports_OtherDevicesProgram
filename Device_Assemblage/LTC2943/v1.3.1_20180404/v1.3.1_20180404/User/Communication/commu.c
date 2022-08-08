#include  "commu.h"
#include "stm32f10x.h"
#include "485.h"





//CRCУ��
u32 N_CRC16(u8 *updata,u32 len)  
{  
  u32 i,j;  
  u32 wPolynom = 0xa001;
  u32 wCrc = 0xffff;  
 for(i=0;i<len;i++)
	{
	  wCrc ^= updata[i];
		for(j=0;j<8;j++)
		{
		  if(wCrc & 0x001)
			  wCrc = (wCrc >> 1)^wPolynom;
			else
				wCrc = wCrc >> 1;
		}
	}
  return wCrc;  
} 



uint8_t *pbuf;
uint16_t len;
uint8_t DeviceADD,commandd,addrL,addrH,content1,content2,CRC_L,CRC_H;

//�����ܵ��������ݷֽ����
uint8_t datadispose(void)
{
	u32 crc=0;
	
	pbuf = get_rebuff(&len);
	
	if(len>=8)
	{
		DeviceADD =pbuf[0];
		commandd  =pbuf[1];      //������
		
		addrH=pbuf[2];        //Ԥ�üĴ�����ַ��λ
		addrL=pbuf[3];        //Ԥ�üĴ�����ַ��λ 06λ�����ĵ�ַ/ 07 λ��������������
		
		content1=pbuf[4];
		content2=pbuf[5];     //����Ҫ�޸ĵ�����   ��ַ �� ������
		
		CRC_L = pbuf[6];
		CRC_H = pbuf[7];
		
		crc = N_CRC16(pbuf,6);
		
		if((CRC_H <<8 | CRC_L) == crc )
			return commandd;
	}

		return 0;
	


}
