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
    POINTNUM_ONE ,//ָ����������
    POINTNUM_LOOP ,//ָ������ѭ������
    POINTNUM_FOLDER ,//ָ���ļ�����
    POINTNUM_MAX ,
} ;

enum
{
    UDISK = 0 ,
    TFCARD ,
    PC ,
    FLASH ,
} ;

//�����ż�ֵ

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
    ERROR_Busy ,//����æ
    ERROR_SLEEP ,
    ERROR_RecvError ,//���ش��ڽ��մ���
    ERROR_CheckSError ,//����У�����
    ERROR_FolPointOver ,//����ָ���ļ��������[���ݳ���Χ]
    ERROR_FolPointNo ,//δ�ҵ�ָ�����ļ�
};




/************************* �� �� �� �� �� **************************/
extern INT8U UartRecvStatus ;
extern INT8U PlayledStatus;
extern INT8U UartRecv_100Ms ;
extern INT8U UartRecvACK ;
extern INT8U System_Status ;
extern INT8U FEEDBACK ;//�Ƿ���Ҫ����[=1:ҪӦ��][=0:��ҪӦ��]
extern INT8U OutTimeCnt ;
extern INT8U ResendCount ;//�ط�����
extern INT8U PlayledCnt ;//����led������ʱ��
extern INT8U CurrentEQ ;//��ǰ��EQ
extern INT8U PlayMode  ;//��ǰ�Ĳ���ģʽ
extern INT8U input_vol ;//��ǰ������
extern INT8U PlayDevice ;//��ǰ���ŵ��豸
extern INT8U OnlineDevice ;//��⵽ģ�����ߵ��豸
extern INT8U CurrentModule ;//��ǰ��ģ���ͺ�
extern INT8U SysReturnTime  ;
extern INT8U QueryACKTime ;//�ȴ�Ӧ���ʱ
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



 /************************* �� �� �� �� �� **************************/

void Time_Task(void) ;
void key_scan(void);
void Key_Task(void) ;
void Indicator_Scan(void);
void MSG_Task(void);
void put_msg_lifo(INT8U msg);
INT8U app_get_msg(void);


#endif
