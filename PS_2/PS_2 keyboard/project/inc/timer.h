/********************************************************************************************
	�ļ�: time.h
	����: WillJason
	�汾��V1.0
	���ڣ�2017/5/3
	��ע������ļ��ṩ�˶�ʱ��ģ��̼��������   	   
*********************************************************************************************/
#ifndef  __TIMER_H_
#define  __TIMER_H_   
static uint8_t allow_30s=1;
static uint8_t Part_30s_State=0; 

static uint8_t Arrival_5s_10Part=0;						  //5s��ʱ��ɣ����ڳ���������������10s��ʱ�ã�
static uint8_t Arrival_10s_10Part=0;					  //10s��ʱ��ɣ����ڳ���������������10s��ʱ�ã�
static uint8_t Arrival_5s_30Part=0;						  //5s��ʱ��ɣ����ڳ���ʼ30s��ʱ�ã�
static uint8_t Arrival_10s_30Part=0;					  //10s��ʱ��ɣ����ڳ���ʼ30s��ʱ�ã�
static uint8_t Count_10s=0;								  //10s��ʱ���������ڳ���ʼ30s��ʱ�ã�
extern void timer0_init(void);							  //��ʱ��0��ʼ��
extern uint8_t timer0_10s(void);						  //����������ͨ�����øú������õ������ķ���ֵ��������ֵΪ1ʱ����ʾ10s��ʱ���
extern uint8_t timer0_30s(void) ;						  //����������ͨ�����øú������õ������ķ���ֵ��������ֵΪ1ʱ����ʾ30s��ʱ���
extern void Clean_Time0_Paramater(void);				  //���㶨ʱ��0��ʱ10s�ļ�������


#endif

