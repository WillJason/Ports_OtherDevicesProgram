/********************************************************************************************
	�ļ�: yx5200.h
	����: WillJason
	�汾��V1.0
	���ڣ�2017/5/3
	��ע�����ͷ�ļ��ṩ�˼����ⲿ�жϺͱ��봦��ĺ������õ��ı�־λ����ͺ��������� 	   
*********************************************************************************************/
#ifndef __PS2_INT_H_ 
#define __PS2_INT_H_
//static uint8_t Counter=0;				                  //��¼���յ��İ���������ֽ�������һ���ֽ�Ϊͨ�룬�����ֽ�Ϊ����
static uint8_t Administrator_Ok=0;						  //����Ա��¼��־λ������¼�ɹ�ʱ���ñ�־λ��1�������¼ʧ����0
static uint8_t Impactful_Key=0;			                  //��Ч������־λ
static uint8_t Finish_Config=0;							  //���������־λ������������ɹ�ʱ���ñ�־λ��1
static uint8_t Finish_30s=0;							  //��ʱ30s��ɱ�־
static uint8_t Allow_Set_Password=0;					  //������������״̬
static uint8_t Config_Password_State=0;	                  //�жϳ����Ƿ��������������״̬
static uint8_t Password_Size=0;		                      //���õ��������Ŀ��С
static uint8_t isTruePassWord=0;						  //����Password_Determine�����ķ���ֵ
static uint8_t isTruePassWord_In30s=0;					  //���ս�������ģʽ������
static uint8_t Key_Data=0;								  //����ȷ��ȡ���İ���ͨ������


static uint8_t Make_Code=0;                               //ͨ��
static uint8_t isCode_F0=0;							  //����ȷ���յ�0xF0
static uint8_t Start_10s=0;								  //10s����ʱ��־λ������1��ʾ����10s����ʱ

static uint8_t KeyV=0x00;                                 //��ֵ
static uint8_t BF = 0;                                    //��ʶ�Ƿ����ַ����յ�
static uint8_t IntNum = 0;                                //�жϴ�������

static uint8_t Player_Input_Num = 0;                      //��������������ֵ
static uint8_t Config_Input_Num = 0;                      //���������������ֵ

extern void Ps2_Out_Free(void);						      //���жϷ������еı������㣬��׼������֮������ļ�ֵ
extern void Determine_10s(void);						  //���� �ж�10s���Ƿ�������������
extern void Gain_Key_Data(void);						  //��ü�ֵ����
extern void isIn_30s(void);					  	      	  //���� �ж��Ƿ��ڿ�ʼ��30s��
extern void Player_Set_Password(void);					  //���� ������������
extern void Send_Analysis(void);						  //���ͽ��յ��ļ�����ֵ����
extern void Decode(void);								  //���� �������ڷ�����

extern void Password_Input(uint8_t Password_One);		  //���� ���ա��������롢����ʵ����Ӧ�Ĺ���
extern uint8_t Password_Determine(uint8_t *Player_Password, uint8_t *Fix_Password,uint8_t Num);
                     									  //���� �ж��������������ȷ�������Ƿ�һ��
extern void Password_Free(uint8_t Num);					  //���� ������������
extern void Password_Save(uint8_t Player_Password,uint8_t i);	             //���� ��������


#endif 