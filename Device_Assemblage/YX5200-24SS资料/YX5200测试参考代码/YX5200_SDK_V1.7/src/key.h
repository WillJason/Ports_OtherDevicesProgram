#ifndef __KEY_H__
#define __KEY_H__

enum
{
    SYS_STATUS_IDLE = 0,
    SYS_STATUS_INIT ,
    SYS_STATUS_WORK ,
    SYS_STATUS_SLEEP ,
} ;

enum//
{
    EQ_NORMAL = 0,
    EQ_POP,
    EQ_ROCK,
    EQ_JAZZ,
    EQ_CLASSIC,
    EQ_BASS,
};

enum
{
    REPEAT_ALL = 0,
    REPEAT_FOLDER,
    REPEAT_ONE,
    REPEAT_RANDOM,
    MAX_PLAY_MODE,
} ;
   
enum
{
    PLAYDEVICE_NULL = 0 ,
    PLAYDEVICE_UDISK = 1,
    PLAYDEVICE_TFCARD ,
    PLAYDEVICE_PC ,
    PLAYDEVICE_FLASH , 
    PLAYDEVICE_SLEEP,  
    PLAYDEVICE_MAX ,
} ;

enum
{
    ONLINE_NULL  = 0 ,
    ONLINE_UDIDK = 0x01 ,
    ONLINE_TFCARD= 0x02 , 
    ONLINE_PC    = 0x04 ,
    ONLINE_FLASH = 0x08 ,
} ;

enum
{
    POINTNUM_NULL = 0 ,
    POINTNUM_ONE ,//指定单曲播放
    POINTNUM_LOOP ,//指定单曲循环播放
    POINTNUM_FOLDER ,//指定文件播放
    POINTNUM_MAX ,
} ;

enum
{
    UDISK = 0 ,
    TFCARD ,
    PC ,
    FLASH ,
} ;

//按键门槛值

#define KEY_BASE_CNT  4
#define KEY_LONG_CNT  75
#define KEY_HOLD_CNT  15


enum
{
    KEY_SHORT_UP ,  
    KEY_LONG , 
    KEY_HOLD ,  
    KEY_LONG_UP ,
} ;


#define NO_KEY          0xff


enum
{
	KEY_NULL  =0 ,
	KEY_VALUE1= 1 ,
	KEY_VALUE2= 2 ,
	KEY_VALUE3= 3 ,
	KEY_VALUE4= 4 ,
	KEY_VALUE5= 5 ,
	KEY_VALUE6= 6 
} ;

enum
{
    SET_PLAYLED_NONE,
    SET_PLAYLED_ON,
    SET_PLAYLED_OFF,
    SET_PLAYLED_100MS_FLASH,
    SET_PLAYLED_500MS_FLASH,
    SET_PLAYLED_900MS_ON_100MS_OFF,
    SET_PLAYLED_100MS_ON_900MS_OFF,
};

enum
{
    ERROR_IDLE = 0 ,
    ERROR_Busy ,//返回忙
    ERROR_SLEEP ,
    ERROR_RecvError ,//返回串口接收错误
    ERROR_CheckSError ,//返回校验出错
    ERROR_FolPointOver ,//返回指定文件输入错误[数据超范围]
    ERROR_FolPointNo ,//未找到指定的文件
};




/************************* 对 外 的 变 量 **************************/
extern INT8U UartRecvStatus ;
extern INT8U PlayledStatus;
extern INT8U UartRecv_100Ms ;
extern INT8U UartRecvACK ;
extern INT8U System_Status ;
extern INT8U FEEDBACK ;//是否需要反馈[=1:要应答][=0:不要应答]
extern INT8U OutTimeCnt ;
extern INT8U ResendCount ;//重发次数
extern INT8U PlayledCnt ;//测试led点亮的时间
extern INT8U CurrentEQ ;//当前的EQ
extern INT8U PlayMode  ;//当前的播放模式
extern INT8U input_vol ;//当前的音量
extern INT8U PlayDevice ;//当前播放的设备
extern INT8U OnlineDevice ;//检测到模块在线的设备
extern INT8U CurrentModule ;//当前的模块型号
extern INT8U SysReturnTime  ;
extern INT8U QueryACKTime ;//等待应答计时
extern INT8U DelayTime ;
extern INT8U ErrorStatus ;
extern INT8U PointFunc ;

extern INT16U input_number ;
extern INT16U CurDevTotal ;
extern INT16U TFTotal ;
extern INT16U UDiskTotal ;
extern INT16U FlashTotal ;

extern INT16U CurDevNUM ;
extern INT16U UDiskCurFile  ;
extern INT16U TFCurFile ;
extern INT16U FlashCurFile ;



 /************************* 对 外 的 接 口 **************************/

void Time_Task(void) ;
void key_scan(void);
void Key_Task(void) ;
void Indicator_Scan(void);
void MSG_Task(void);
void put_msg_lifo(INT8U msg);
INT8U app_get_msg(void);


#endif
