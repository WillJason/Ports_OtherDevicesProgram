#ifndef __MSG_H__
#define __MSG_H__

enum
{
    MSG_0 = 0,                        
    MSG_1,
    MSG_2,
    MSG_3,
    MSG_4,
    MSG_5,
    MSG_6,
    MSG_7,
    MSG_8,
    MSG_9,
    MSG_DEVICE_REMOVED,             
    MSG_CHANGE_WORK_MODE,
    MSG_MUSIC_NEW_DEVICE_IN,
    MSG_MUSIC_SELECT_NEW_DEVICE,
    MSG_MUSIC_SELECT_NEW_FILE,
    MSG_MUSIC_PLAY_NEW_FILE,
    MSG_MUSIC_CONTINUE_PLAY_FILE,       
    MSG_VOL_UP,
    MSG_VOL_DOWN,
    MSG_MUSIC_NEXT_FILE,
    MSG_MUSIC_PREV_FILE,
    MSG_MUSIC_PP,
    MSG_MUSIC_PLAY,
    MSG_MUSIC_PAUSE,
    MSG_MUSIC_STOP,
    MSG_MUSIC_FF,
    MSG_MUSIC_FR,
    MSG_MUSIC_FFR_DONE,
    MSG_MUSIC_NEXT_EQ,
    MSG_MUSIC_PREV_EQ,
    MSG_NEXT_PLAYMODE,
    MSG_PREV_PLAYMODE,
    MSG_NEXT_WORKMODE,                  
    MSG_PREV_WORKMODE,
    MSG_PICH_SONG,
    MSG_PICH_DEVICE,
    MSG_NEXT_DEVICE,
    MSG_PREV_DEVICE,
    MSG_DELAY_TIME,
    MSG_MUSIC_OVER,

    MSG_RETURN_INIT,
    MSG_RETURN_MINIT,  //����ģ���ϵ�Ĳ���
    MSG_QUERY_NUMTOTAL,//��ѯ��ǰ�豸���ļ���
    MSG_RETURN_NUMTOTAL,//��ʾ���ص����ļ���
    MSG_QUERY_CURNUM,   //��ѯ��ǰ���ŵ���Ŀ
    MSG_RETURN_CURNUM,  //��ʾ��ǰ���ŵ���Ŀ
    MSG_QUERY_VOL,      //��ѯ��ǰ������
    MSG_RETURN_VOL,     //��ʾ��ǰ������
    MSG_QUERY_EQ,       //��ѯ��ǰ��EQ
    MSG_RETURN_EQ,      //��ʾ��ǰ��EQ
    MSG_QUERY_PLAYMODE, //��ѯ��ǰ�Ĳ���ģʽ
    MSG_RETURN_PLAYMODE,//��ʾ��ǰ�Ĳ���ģʽ

    MSG_QUERY_ERROR ,
    MSG_RECV_ERROR ,
    MSG_UART_ACK,
    MSG_TRIG_MODE,//����ģʽ[����һ����������ͣ]
    MSG_10MS,
    MSG_100MS,                      
    MSG_200MS,
    MSG_INPUT_TIMEOUT,  

    MSG_SET_PLAYMODE,//ָ������ģʽ[����������ѭ����ָ���ļ���]
    MSG_PLAYALL,
    MSG_PLAYALL_ON,
    MSG_PLAYALL_OFF,
    
    MSG_UDISK_IN,
    MSG_TF_IN,
    MSG_PC_IN,
    MSG_UDISK_OUT,
    MSG_TF_OUT,
    MSG_PC_OUT,
    
    NO_MSG = 0xFF,
};



#endif
