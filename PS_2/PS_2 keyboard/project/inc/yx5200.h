/********************************************************************************************
	�ļ�: yx5200.h
	����: WillJason
	�汾��V1.0
	���ڣ�2016/1/28
	��ע������ļ��ṩ��yx5200�̼��������  	   
*********************************************************************************************/
#ifndef __YX5200_H_ 
#define __YX5200_H_

typedef struct 				                              //����yx5200 ���ֲ��Žṹ��
{
	 uint8_t   cmd;		       							  //���ֲ�����ָ��
	 uint16_t  folder_num;	                              //���ֲ���Ŀ¼�����
	 uint8_t   break_flag;	                              //���ֲ��ſɷ��ϱ�ʾ 0:���Դ�� 1�����ɴ��
	 uint8_t   statu_flag;	                              //���ֲ���״̬��ʾ	  0:��ʾû����Ƶ���� 1�����ڲ���
}MUSIC;

extern MUSIC music;

extern void send_yx5200_cmd(uint8_t cmd , uint16_t dat);  //����1��yx5200оƬ��������[�������ƺͲ�ѯ]
extern uint8_t yx5200_play_finish_statu(void);			  //���yx5200���ֲ������״̬
extern void Play_Set_Sound(uint16_t dat);				  //����ָ���������ļ�


#endif 
