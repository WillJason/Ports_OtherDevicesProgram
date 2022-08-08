/********************************************************************************************
	文件: time.h
	作者: WillJason
	版本：V1.0
	日期：2017/5/3
	备注：这个文件提供了定时器模块固件库的声明   	   
*********************************************************************************************/
#ifndef  __TIMER_H_
#define  __TIMER_H_   
static uint8_t allow_30s=1;
static uint8_t Part_30s_State=0; 

static uint8_t Arrival_5s_10Part=0;						  //5s计时完成（用于程序密码输入连续10s计时用）
static uint8_t Arrival_10s_10Part=0;					  //10s计时完成（用于程序密码输入连续10s计时用）
static uint8_t Arrival_5s_30Part=0;						  //5s计时完成（用于程序开始30s计时用）
static uint8_t Arrival_10s_30Part=0;					  //10s计时完成（用于程序开始30s计时用）
static uint8_t Count_10s=0;								  //10s计时次数（用于程序开始30s计时用）
extern void timer0_init(void);							  //定时器0初始化
extern uint8_t timer0_10s(void);						  //在主程序中通过调用该函数来得到函数的返回值，当返回值为1时，表示10s计时完成
extern uint8_t timer0_30s(void) ;						  //在主程序中通过调用该函数来得到函数的返回值，当返回值为1时，表示30s计时完成
extern void Clean_Time0_Paramater(void);				  //清零定时器0计时10s的计数变量


#endif

