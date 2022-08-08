/********************************************************************************************
	文件: yx5200.h
	作者: WillJason
	版本：V1.0
	日期：2017/5/3
	备注：这个头文件提供了键盘外部中断和编码处理的函数库用到的标志位定义和函数的声明 	   
*********************************************************************************************/
#ifndef __PS2_INT_H_ 
#define __PS2_INT_H_
//static uint8_t Counter=0;				                  //记录接收到的按键的码的字节数，第一个字节为通码，二三字节为断码
static uint8_t Administrator_Ok=0;						  //管理员登录标志位：当登录成功时，该标志位置1，否则登录失败清0
static uint8_t Impactful_Key=0;			                  //有效按键标志位
static uint8_t Finish_Config=0;							  //配置密码标志位：当配置密码成功时，该标志位置1
static uint8_t Finish_30s=0;							  //计时30s完成标志
static uint8_t Allow_Set_Password=0;					  //允许配置密码状态
static uint8_t Config_Password_State=0;	                  //判断程序是否在配置密码程序状态
static uint8_t Password_Size=0;		                      //配置的密码的数目大小
static uint8_t isTruePassWord=0;						  //接收Password_Determine函数的返回值
static uint8_t isTruePassWord_In30s=0;					  //接收进入配置模式的密码
static uint8_t Key_Data=0;								  //保存确定取到的按键通码数据


static uint8_t Make_Code=0;                               //通码
static uint8_t isCode_F0=0;							  //用来确定收到0xF0
static uint8_t Start_10s=0;								  //10s倒计时标志位，被置1表示启动10s倒计时

static uint8_t KeyV=0x00;                                 //键值
static uint8_t BF = 0;                                    //标识是否有字符被收到
static uint8_t IntNum = 0;                                //中断次数计数

static uint8_t Player_Input_Num = 0;                      //玩家输入密码计数值
static uint8_t Config_Input_Num = 0;                      //配置输入密码计数值

extern void Ps2_Out_Free(void);						      //将中断服务函数中的变量清零，以准备接收之后输入的键值
extern void Determine_10s(void);						  //声明 判断10s内是否有连续的输入
extern void Gain_Key_Data(void);						  //获得键值函数
extern void isIn_30s(void);					  	      	  //声明 判断是否在开始的30s内
extern void Player_Set_Password(void);					  //声明 用于配置密码
extern void Send_Analysis(void);						  //发送接收到的键盘码值数据
extern void Decode(void);								  //声明 解码用于蜂鸣器

extern void Password_Input(uint8_t Password_One);		  //声明 接收、保存密码、解码实现相应的功能
extern uint8_t Password_Determine(uint8_t *Player_Password, uint8_t *Fix_Password,uint8_t Num);
                     									  //声明 判断输入的密码与正确的密码是否一致
extern void Password_Free(uint8_t Num);					  //声明 清除输入的密码
extern void Password_Save(uint8_t Player_Password,uint8_t i);	             //声明 保存密码


#endif 