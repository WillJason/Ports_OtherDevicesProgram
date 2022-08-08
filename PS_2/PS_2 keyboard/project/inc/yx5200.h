/********************************************************************************************
	文件: yx5200.h
	作者: WillJason
	版本：V1.0
	日期：2016/1/28
	备注：这个文件提供了yx5200固件库的声明  	   
*********************************************************************************************/
#ifndef __YX5200_H_ 
#define __YX5200_H_

typedef struct 				                              //定义yx5200 音乐播放结构体
{
	 uint8_t   cmd;		       							  //音乐播放器指令
	 uint16_t  folder_num;	                              //音乐播放目录及编号
	 uint8_t   break_flag;	                              //音乐播放可否打断标示 0:可以打断 1：不可打断
	 uint8_t   statu_flag;	                              //音乐播放状态标示	  0:表示没有音频播放 1：正在播放
}MUSIC;

extern MUSIC music;

extern void send_yx5200_cmd(uint8_t cmd , uint16_t dat);  //串口1向yx5200芯片发送命令[包括控制和查询]
extern uint8_t yx5200_play_finish_statu(void);			  //获得yx5200音乐播放完成状态
extern void Play_Set_Sound(uint16_t dat);				  //播放指定的音乐文件


#endif 
