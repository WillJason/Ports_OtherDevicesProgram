/********************************************************************************************
	�ļ�: key.c
	����: WillJason
	�汾��V1.0
	���ڣ�2016/1/28
	��ע������ļ��ṩ�˰����Ĺ̼���  	   
*********************************************************************************************/
#include "STC12C5A60S2.h"
#include "key.h"
#include "io_config.h"


/*******************************************************************************************
	�������ܣ�������⺯��
	��ڲ�������
	���ز�����0x00:�ް��������£�0x01:����0�����£�0x02:����1������	
	����˵����һ�ΰ��º���Ҫ̧����һ�εİ��²Ż�ʶ�𣬼�ֻ��⵽һ��
********************************************************************************************/
uint8_t key_down_edge_function(void)
{
	uint8_t key_value = 0;
	static uint8_t key_down_flag[5] = {0,0};
	static uint8_t key_down_conuter[5] = {5,5};		
	/*******************����0�ж�*********************/		
	if((key0_port != 1) && (key_down_flag[0] != 1))			 //�����ж�
	{
		if(key_down_conuter[0] < 10) 
		{
			key_down_conuter[0]++;
		}
		else
		{
			key_down_flag[0] = 1;
			key_value |= 0x01; 					             //ȷ������������
		}	
	}
	else if((key0_port == 1) && (key_down_flag[0] != 0))	  //̧���ж�
	{
		if(key_down_conuter[0] > 1) 
		{
			key_down_conuter[0]--;
		}
		else 
		{
			key_down_flag[0] = 0;							//ȷ��������̧��
			key_value &= ~(0x01);		 
		}		
	}
	else
	{
		key_value &= ~(0x01);	
	} 
	/*******************����1�ж�*********************/	
	if((key1_port != 1) && (key_down_flag[1] != 1))			//�����ж�
	{
		if(key_down_conuter[1] < 10) 
		{
			key_down_conuter[1]++;
		}
		else
		{
			key_down_flag[1] = 1;
			key_value |= 0x02; 						        //ȷ������������
		}
	
	}
	else if((key1_port == 1) && (key_down_flag[1] != 0))	//̧���ж�
	{
		if(key_down_conuter[1] > 1) 
		{
			key_down_conuter[1]--;
		}
		else
		{
		 	key_down_flag[1] = 0;
			key_value &= ~(0x02);		 				    //ȷ��������̧��
		}										       	
	}
	else
	{
		key_value &= ~(0x02);	
	} 

	return key_value;
}

/*******************************************************************************************
	�������ܣ�������ƽ����⺯��
	��ڲ�������
	���ز�����1:�͵�ƽ   0���ߵ�ƽ	
	����˵����
********************************************************************************************/
uint8_t key_level_cheak_function(void)
{
    static uint8_t level_statu = 0;  
	static uint8_t level_conuter = 0;
	if(key2_port == 0)	
    { 
		if(level_conuter < 5)	{ level_conuter++;} 
		else level_statu = 1;						  		
    }
    else
	{
		if(level_conuter > 0)	 { level_conuter--;}     
		else level_statu = 0;	
	}
	return level_statu;		  	  
}












