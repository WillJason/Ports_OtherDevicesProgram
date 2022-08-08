#include "config.h"

/******************************************************************************************************************
        
						+--------------------------------------------------+
						                  �����ļ�⴦����
						+--------------------------------------------------+

 - ʵ�ֹ��ܣ�

 - Ŀǰ��չ��

 - ��ע    ���������õ���3*4�ľ��󰴼���ռ��STC��7��IO
******************************************************************************************************************/
static INT8U MSG_FIFO = NO_MSG;
static INT8U InputNumCnt = 0 ;
static INT8U FolderNum = 0 ;//ָ���ļ��в���ʱ���ļ�����Ŀ
static INT8U FileNum = 0 ;//ָ���ļ�������ʱ���ļ�����Ŀ

INT8U ErrorStatus = 0 ;
INT8U FEEDBACK = 0 ;//�Ƿ���Ҫ����[=1:ҪӦ��][=0:��ҪӦ��]
INT8U UartRecvStatus ;
INT8U UartRecvACK ;
INT8U UartRecv_100Ms ;//��֤���ڽ��մ���Ҳ�ܻص���ʼ״̬
INT8U System_Status = 0 ;//ϵͳ״̬

INT8U OutTimeCnt  = 0 ;//�ط���ʱ
INT8U ResendCount = 5 ;//�ط�����

INT8U QueryACKTime = 0 ;//[��ѯ����]�ȴ�Ӧ���ʱ
INT8U SysReturnTime = 0 ;//ϵͳ���س�ʼ�����ʱ
INT8U PlayledCnt = 0 ;//����led������ʱ��
INT8U PlayledStatus;
INT8U CurrentEQ = 0 ;//��ǰ��EQ
INT8U PlayMode  = 0 ;//��ǰ�Ĳ���ģʽ
INT8U PlayDevice = 0;//��ǰ���ŵ��豸
INT8U OnlineDevice = 0 ;//���ߵ��豸

INT8U DelayTime = 0 ;

INT8U input_vol = 0 ;//��ǰ������
INT8U CurrentModule = 0 ;//��ǰ��ģ���ͺ�
INT8U PointFunc = 0 ;

INT16U input_number ;//ѡ�����������
INT16U CurDevTotal = 0 ;//��ǰ�豸�����ļ���
INT16U UDiskTotal =0  ;//U�����ļ���
INT16U TFTotal = 0 ;//TF���ļ���
INT16U FlashTotal = 0 ;//FLASH�е��ļ���

INT16U CurDevNUM = 0 ;//��ǰ�豸���ŵ���Ŀ
INT16U UDiskCurFile =0  ;//��ǰU�̲��ŵ��ļ���
INT16U TFCurFile = 0 ;//��ǰTF���ŵ��ļ���
INT16U FlashCurFile = 0 ;//��ǰFLASH���ŵ��ļ���

/**********************************************************************
- 1���������õ���3*4�ľ��󰴼���ռ��7��IO
- 2��
**********************************************************************/ 
INT8U code key_table[12] = {
                                0xED,0xDD,0xBD,0x7D,
                                0xEB,0xDB,0xBB,0x7B,//4 
                                0xE7,0xD7,0xB7,0x77  
                           };
//�̰�
#define ADKEY_16KEY_MUSIC_SHORT			\
/* - 0 -*/		MSG_0,\
/* - 1 -*/		MSG_1,\
/* - 2 -*/		MSG_2,\
/* - 3 -*/		MSG_3,\
/* - 4 -*/		MSG_4,\
/* - 5 -*/		MSG_5,\
/* - 6 -*/		MSG_PLAYALL,\
/* - 7 -*/		MSG_SET_PLAYMODE,\
/* - 8 -*/		MSG_MUSIC_NEXT_FILE,\
/* - 9 -*/		MSG_MUSIC_PREV_FILE,\
/* - 10-*/		MSG_VOL_UP,\
/* - 11-*/		MSG_VOL_DOWN,\

//����
#define ADKEY_16KEY_MUSIC_LONG			\
/* - 0 -*/		MSG_MUSIC_NEXT_EQ,\
/* - 1 -*/		MSG_MUSIC_PLAY,\
/* - 2 -*/		MSG_MUSIC_PAUSE,\
/* - 3 -*/		NO_MSG,\
/* - 4 -*/		MSG_NEXT_WORKMODE,\
/* - 5 -*/		MSG_QUERY_NUMTOTAL,\
/* - 6 -*/		MSG_QUERY_CURNUM,\
/* - 7 -*/		MSG_QUERY_VOL,\
/* - 8 -*/		MSG_QUERY_EQ,\
/* - 9 -*/		MSG_QUERY_PLAYMODE,\
/* - 10-*/		MSG_TRIG_MODE,\
/* - 11-*/		MSG_UART_ACK,\


//���Ų�����
#define ADKEY_16KEY_MUSIC_HOLD			\
/* - 0 -*/		NO_MSG,\
/* - 1 -*/		NO_MSG,\
/* - 2 -*/		NO_MSG,\
/* - 3 -*/		NO_MSG,\
/* - 4 -*/		NO_MSG,\
/* - 5 -*/		NO_MSG,\
/* - 6 -*/		NO_MSG,\
/* - 7 -*/		NO_MSG,\
/* - 8 -*/		NO_MSG,\
/* - 9 -*/		NO_MSG,\
/* - 10-*/		NO_MSG,\
/* - 11-*/		NO_MSG,\

//����̧��
#define ADKEY_16KEY_MUSIC_LONG_UP		\
/* - 0 -*/		NO_MSG,\
/* - 1 -*/		NO_MSG,\
/* - 2 -*/		NO_MSG,\
/* - 3 -*/		NO_MSG,\
/* - 4 -*/		NO_MSG,\
/* - 5 -*/		NO_MSG,\
/* - 6 -*/		NO_MSG,\
/* - 7 -*/		NO_MSG,\
/* - 8 -*/		NO_MSG,\
/* - 9 -*/		NO_MSG,\
/* - 10-*/		NO_MSG,\
/* - 11-*/		NO_MSG,\
		

INT8U code adkey_msg_16key_table[4][12] =		
{
/*�̰�--*/	{ADKEY_16KEY_MUSIC_SHORT},
/*����--*/	{ADKEY_16KEY_MUSIC_LONG},
/*����--*/	{ADKEY_16KEY_MUSIC_HOLD},
/*����̧��*/	{ADKEY_16KEY_MUSIC_LONG_UP},
};


void PointPlayNum(INT8U num) ;
void PointPlayFolderNum(INT8U num);
void DispModlueError(INT8U error);

/*****************************************************************************************************
 - ���������� ����Ϣ������Ϣ��
 - ����ģ�飺 �ⲿ 
 - ����˵���� ��
 - ����˵���� 
 - ע���� 
*****************************************************************************************************/
void put_msg_lifo(INT8U msg)
{
    MSG_FIFO = msg ;           
}

/*****************************************************************************************************
 - ���������� ȡ��Ϣ
 - ����ģ�飺 �ⲿ 
 - ����˵���� ��
 - ����˵���� 
 - ע���� 
*****************************************************************************************************/
INT8U app_get_msg(void)
{
    static INT8U msg ;

    msg = MSG_FIFO ;
    
    if(MSG_FIFO != NO_MSG)
    {
        MSG_FIFO = NO_MSG ;
        return msg ;
    }
    else
    {
        return NO_MSG ;//����Ϣ���
    }
}
/*****************************************************************************************************
 - ���������� Music ģʽ������Ϣ���˴���
 - ����ģ�飺 �ⲿ 
 - ����˵���� ��
 - ����˵���� ��⵽�İ���ֵ
 - ע���� 
*****************************************************************************************************/

void music_msg_filter(INT8U key_status, INT8U back_last_key)
{
	INT8U msg;

	msg = adkey_msg_16key_table[key_status][back_last_key];//��ȡ��Ϣ

	put_msg_lifo(msg);
}



/***************************************************************************
 - ���������� ����ȥ������������ȶ���ֵ
 - ����ģ�飺 �ⲿ
 - ����˵���� ��
 - ����˵���� ��
 - ע��	     Ӳ��������----����ʽ
**************************************************************************/
static INT8U KeyScan(void) 
{
    INT8U i,temp;
    INT8U code *ptr;
    static INT8U g_key_value ;
    
	KEYIO   = 0x0f;
	temp = KEYIO & 0x0f;
	if(temp != 0x0f)//�м�������
	{
		KEYIO = 0xf0;
		g_key_value = temp|KEYIO;//�õ���ֵ
		for(i=0,ptr=key_table; i<13; i++,ptr++) //���ֵ
		{
			if(g_key_value == *ptr)
			{
				g_key_value = i;//�õ���ֵ
				return i ;
			}
		}
        return NO_KEY ;
    } 
	return NO_KEY ; 
}


/*****************************************************************************************************
 - ���������� ����ȥ������������ȶ���ֵ
 - ����ģ�飺 �ڲ� 
 - ����˵���� ��
 - ����˵���� ��⵽�İ���ֵ
 - ע���� 
*****************************************************************************************************/
static INT8U key_filter(INT8U key)
{
	static INT8U used_key = NO_KEY;
	static INT8U old_key;
	static INT8U key_counter;

	if(old_key != key)
	{
		key_counter = 0;
		old_key = key;
	}
	else
	{
		key_counter++;
		if(key_counter == KEY_BASE_CNT)
		{
			used_key = key;
		}
	}

	return used_key;
}

/*****************************************************************************************************
 - ���������� ����-��Ϣת������,��������˳�򣺶̰�̧��/����-->����
 - ����ģ�飺 �ⲿ 
 - ����˵���� ��
 - ����˵���� ��⵽�İ���ֵ
 - ע��       ����/�̰�/���� --- OK 
*****************************************************************************************************/
void key_scan(void)
{
	static INT8U last_key = NO_KEY;
	static INT8U key_press_counter;
	INT8U cur_key, key_status, back_last_key;

	cur_key = KeyScan();       //����ɨ��

	cur_key = key_filter(cur_key);//���˰������൱��ȥ��

	back_last_key = last_key;

	if(cur_key == last_key)//��ʱ�䰴��
	{
		if(cur_key == NO_KEY)
			return;

		key_press_counter++;

		if(key_press_counter == KEY_LONG_CNT)//����
		{
			key_status = KEY_LONG;
		}
		else if(key_press_counter == (KEY_LONG_CNT + KEY_HOLD_CNT))//����
		{
			key_status = KEY_HOLD;
			key_press_counter = KEY_LONG_CNT;
		}
		else
		{
			return;
		}
	}
	else  //cur_key = NO_KEY, ̧��
	{
		last_key = cur_key;
		if((key_press_counter < KEY_LONG_CNT) && (cur_key == NO_KEY))//�̰�̧��
		{
			key_press_counter = 0;
			key_status = KEY_SHORT_UP;
		}
		else
		{
			key_press_counter = 0;
			return;
		}
	}
      
    Put_u8(key_status) ;
    Put_u8(back_last_key) ;
	music_msg_filter(key_status, back_last_key);
}

/*****************************************************************************************************
 - ����˵������Ϣ����
 - ����ģ�飺�ڲ�
 - ����˵������
 - ����˵����
 - ע      ��
*****************************************************************************************************/
void MSG_Task(void)
{
    static INT8U msg ;

    msg = app_get_msg();

    switch(msg)
    {
       /*****************************************************
                          ��ʾģ�鷵�صĴ���
        *****************************************************/
        case MSG_RECV_ERROR:
                DispModlueError(ErrorStatus);
                SysReturnTime = SYSRETURNTIME ;           
                break ;
                
       /*****************************************************
                          ���س�ʼ״̬
        *****************************************************/
        case MSG_RETURN_INIT:
                RAM_Init() ;
                LCD1602_Init();
                DBG("MSG_RETURN_INIT\n") ;            
                break ;
       /*****************************************************
                          �豸����
        *****************************************************/
        case MSG_UDISK_IN:
                LCD1602_ClearLine(1);
                LCD1602_DispStr(1 , 0  , "UDISK IN");
                PlayDevice = PLAYDEVICE_UDISK ;
                OnlineDevice |= BIT(UDISK) ;
                SysReturnTime = SYSRETURNTIME ;
                break ;
        case MSG_TF_IN:
                LCD1602_ClearLine(1);
                LCD1602_DispStr(1 , 0  , "TF IN");
                PlayDevice = PLAYDEVICE_TFCARD ;
                OnlineDevice |= BIT(TFCARD) ;
                SysReturnTime = SYSRETURNTIME ;
                break ;
        case MSG_PC_IN:
                LCD1602_ClearLine(1);
                LCD1602_DispStr(1 , 0  , "PC IN");
                PlayDevice = PLAYDEVICE_PC ;
                OnlineDevice |= BIT(PC) ;
                SysReturnTime = SYSRETURNTIME ;
                break ;
       /*****************************************************
                          �豸�γ�
        *****************************************************/
        case MSG_UDISK_OUT:
                LCD1602_ClearLine(1);
                LCD1602_DispStr(1 , 0  , "UDISK OUT");
                OnlineDevice &= ~BIT(UDISK) ;
                GotoNextDevice(UDISK) ;
                SysReturnTime = SYSRETURNTIME ;
                break ;
        case MSG_TF_OUT:
                LCD1602_ClearLine(1);
                LCD1602_DispStr(1 , 0  , "TF OUT");
                OnlineDevice &= ~BIT(TFCARD) ;
                GotoNextDevice(TFCARD) ;
                SysReturnTime = SYSRETURNTIME ;
                break ;
        case MSG_PC_OUT:
                LCD1602_ClearLine(1);
                LCD1602_DispStr(1 , 0  , "PC OUT");
                OnlineDevice &= ~BIT(PC) ;
                GotoNextDevice(PC) ;
                SysReturnTime = SYSRETURNTIME ;
                break ;
       /*****************************************************
                          ���س�ʼ״̬
        *****************************************************/
        case MSG_DELAY_TIME:
                DelayTime = 6 ;        
                break ;                   

       /*****************************************************
                          ��ѯ������Ӧ��
        *****************************************************/
        case MSG_QUERY_ERROR:
                LCD1602_Clear() ;
                LCD1602_DispStr(1 , 0  , "ERROR CMD");
                SysReturnTime = SYSRETURNTIME ;
                break ;
                
       /*****************************************************
                          ��ѯ��������
        *****************************************************/
        case MSG_QUERY_NUMTOTAL:
                LCD1602_Clear() ;
                LCD1602_DispStr(0 , 0  , "QUERY TOTAL NUM ");    
#if 1
                if(PlayDevice == PLAYDEVICE_SLEEP)
                {
                    LCD1602_Clear() ;
                    LCD1602_DispStr(1 , 0  , "MOD IN SLEEP  ");    
                    QueryACKTime = QUERYACKTIME ;//�ȴ�Ӧ���ʱ
                    break;
                }
#endif
                if(PlayDevice == PLAYDEVICE_UDISK)
                {
                    Uart_SendCMD(UARTQUR_UDISK_TOTAL , FEEDBACK , 0);
                }
                else if(PlayDevice == PLAYDEVICE_TFCARD)
                {
                    Uart_SendCMD(UARTQUR_TF_TOTAL , FEEDBACK , 0);   
                }
                else if(PlayDevice == PLAYDEVICE_FLASH)
                {
                    Uart_SendCMD(UARTQUR_FLASH_TOTAL , FEEDBACK , 0);   
                }
                LCD1602_DispStr(1 , 0  , "WAIT RETURN ...."); 
                QueryACKTime = QUERYACKTIME ;//�ȴ�Ӧ���ʱ
                break ;

       /*****************************************************
                        ��ʾ��ѯ��ǰ�����豸��Ŀ����
        *****************************************************/
        case MSG_RETURN_NUMTOTAL:
                QueryACKTime = 0 ;//����ȴ���ʱ
                LCD1602_Clear() ;
                LCD1602_DispStr(0 , 0  , "DEVICE:         "); 
                LCD1602_DispStr(1 , 0  , "TOTAL NUM:      "); 
                if(PlayDevice == PLAYDEVICE_UDISK)
                {
                    CurDevTotal = UDiskTotal ;
                    LCD1602_DispStr(0 , 8  , "UDISK");     
                }
                else if(PlayDevice == PLAYDEVICE_TFCARD)
                {
                    CurDevTotal = TFTotal ;
                    LCD1602_DispStr(0 , 8  , "TF");       
                }
                else if(PlayDevice == PLAYDEVICE_FLASH)
                {
                    CurDevTotal = FlashTotal ;
                    LCD1602_DispStr(0 , 8  , "FLASH");      
                }
                itoa4(CurDevTotal);
                LCD1602_DispStr(1 , 11 , (INT8U *)bcd_number);
                SysReturnTime = SYSRETURNTIME ;
                break ;

                
       /*****************************************************
                          ��ѯ��ǰ��Ŀ
        *****************************************************/
        case MSG_QUERY_CURNUM:
                LCD1602_Clear() ;
                LCD1602_DispStr(0 , 0  , "QUERY CUR NUM   "); 
                if(PlayDevice == PLAYDEVICE_SLEEP)
                {
                    LCD1602_Clear() ;
                    LCD1602_DispStr(1 , 0  , "MOD IN SLEEP ");   
                    QueryACKTime = QUERYACKTIME ;//�ȴ�Ӧ���ʱ
                    break;
                }  
                if(PlayDevice == PLAYDEVICE_UDISK)
                {
                    Uart_SendCMD(UARTQUR_UDISK_FILE , FEEDBACK , 0);
                }
                else if(PlayDevice == PLAYDEVICE_TFCARD)
                {
                    Uart_SendCMD(UARTQUR_TF_FILE , FEEDBACK , 0);   
                }
                else if(PlayDevice == PLAYDEVICE_FLASH)
                {
                    Uart_SendCMD(UARTQUR_FLASH_FILE, FEEDBACK , 0);   
                }
                LCD1602_DispStr(1 , 0  , "WAIT RETURN ....");

                QueryACKTime = QUERYACKTIME ;//�ȴ�Ӧ���ʱ
                break ;

       /*****************************************************
                        ��ʾ��ѯ��ǰ������Ŀ
        *****************************************************/
        case MSG_RETURN_CURNUM:
                QueryACKTime = 0 ;//����ȴ���ʱ
                LCD1602_Clear() ;
                LCD1602_DispStr(0 , 0  , "DEVICE:         "); 
                LCD1602_DispStr(1 , 0  , "CUR UM:            "); 
                if(PlayDevice == PLAYDEVICE_UDISK)
                {
                    CurDevNUM = UDiskCurFile ;
                    LCD1602_DispStr(0 , 8  , "UDISK");     
                }
                else if(PlayDevice == PLAYDEVICE_TFCARD)
                {
                    CurDevNUM = TFCurFile ;
                    LCD1602_DispStr(0 , 8  , "TF");       
                }
                else if(PlayDevice == PLAYDEVICE_FLASH)
                {
                    CurDevNUM = FlashCurFile ;
                    LCD1602_DispStr(0 , 8  , "FLASH");      
                }
                itoa4(CurDevNUM);
                LCD1602_DispStr(1 , 11 , (INT8U *)bcd_number);
                SysReturnTime = SYSRETURNTIME ;
                break ;
       /*****************************************************
                          ��ѯ��ǰ����
        *****************************************************/
        case MSG_QUERY_VOL:
                LCD1602_Clear() ;
                LCD1602_DispStr(0 , 0  , "QUERY CUR VOL   ");
#if 0
                if(PlayDevice == PLAYDEVICE_SLEEP)
                {
                    LCD1602_DispStr(1 , 0  , "MOD IN SLEEP ");
                }
                else
#endif
                {
                    LCD1602_DispStr(1 , 0  , "WAIT RETURN ...."); 
                    Uart_SendCMD(UARTQUR_VOL , FEEDBACK , 0);
                }
                QueryACKTime = QUERYACKTIME ;//�ȴ�Ӧ���ʱ
                break ;

       /*****************************************************
                        ��ʾ��ѯ��ǰ����������
        *****************************************************/
        case MSG_RETURN_VOL:
                QueryACKTime = 0 ;//����ȴ���ʱ
                LCD1602_Clear() ;
                LCD1602_DispStr(1 , 0  , "VOL:            "); 
                itoa2(input_vol);
                LCD1602_DispStr(1 , 11 , (INT8U *)bcd_number);
                SysReturnTime = SYSRETURNTIME ;
                break ;
                
       /*****************************************************
                          ��ѯ��ǰEQ
        *****************************************************/
        case MSG_QUERY_EQ:
                LCD1602_Clear() ;
                LCD1602_DispStr(0 , 0  , "QUERY CUR EQ    ");
#if 1
                if(PlayDevice == PLAYDEVICE_SLEEP)
                {
                    LCD1602_DispStr(1 , 0  , "MOD IN SLEEP ");
                }
                else
#endif
                {
                    LCD1602_DispStr(1 , 0  , "WAIT RETURN ...."); 
                    Uart_SendCMD(UARTQUR_EQ , FEEDBACK , 0);
                }
                QueryACKTime = QUERYACKTIME ;//�ȴ�Ӧ���ʱ
                break ;

       /*****************************************************
                        ��ʾ��ѯ��ǰEQ������
        *****************************************************/
        case MSG_RETURN_EQ:
                QueryACKTime = 0 ;//����ȴ���ʱ
                LCD1602_Clear() ;
                Disp_PlayEQ(CurrentEQ);
                SysReturnTime = SYSRETURNTIME ;
                break ;

                
       /*****************************************************
                          ��ѯ��ǰ����ģʽ
        *****************************************************/
        case MSG_QUERY_PLAYMODE:
                LCD1602_Clear() ;
                LCD1602_DispStr(0 , 0  , "QUERY PLAY MODE ");

                if(PlayDevice == PLAYDEVICE_SLEEP)
                {
                    LCD1602_DispStr(1 , 0  , "MOD IN SLEEP   ");
                }
                else

                {
                    LCD1602_DispStr(1 , 0  , "WAIT RETURN ...."); 
                    Uart_SendCMD(UARTQUR_PLAYMODE , FEEDBACK , 0);
                }
                QueryACKTime = QUERYACKTIME ;//�ȴ�Ӧ���ʱ
                break ;

       /*****************************************************
                        ��ʾ��ѯ��ǰ����ģʽ������
        *****************************************************/
        case MSG_RETURN_PLAYMODE:
                QueryACKTime = 0 ;//����ȴ���ʱ
                LCD1602_Clear() ;
                Disp_PlayMode(PlayMode);
                SysReturnTime = SYSRETURNTIME ;
                break ;

       /*****************************************************
                          ��ʾģ���ʼ������������
        *****************************************************/
        case MSG_RETURN_MINIT:
                LCD1602_Clear() ;
                LCD1602_DispStr(0 , 0  , "RECV MODL OK    ");
                LCD1602_DispStr(1 , 0  , "ONLINE DEV:     ");
                Disp_OnlineDevice(OnlineDevice);
                GoInitDevice(OnlineDevice) ;
                System_Status = SYS_STATUS_WORK ;
                SysReturnTime = SYSRETURNTIME ;//����3S��ʱ��ȴ�ģ���ʼ�����
                break ;
#if 0
       /*****************************************************
                          �л�����ģʽ
        *****************************************************/
        case MSG_TRIG_MODE:
                TrigEnable = !TrigEnable ;
                LCD1602_ClearLine(1);
                if(TrigEnable == PLAYDEVICE_UDISK)
                {
                    LCD1602_DispStr(1 , 0  , "PLAYM :  TRIG");//1:����ģʽ
                }
                else
                {
                    LCD1602_DispStr(1 , 0  , "PLAYM :  CONT");//0:����ģʽ
                }
                SysReturnTime = SYSRETURNTIME ;
                break ;
#endif                

       /*****************************************************
                          �л���ǰ�����豸
        *****************************************************/
        case MSG_NEXT_WORKMODE: 
                LCD1602_Clear() ;
                LCD1602_DispStr(0 , 0  , "PLAY DEVICE SET");
                PlayDevice ++ ;
                if(PlayDevice == PLAYDEVICE_MAX)
                {
                    PlayDevice = PLAYDEVICE_UDISK;  
                }
                ChangeDevice(PlayDevice);
                Disp_PlayDevice(PlayDevice);
                Uart_SendCMD(UARTCMD_PLAYDEVICE , FEEDBACK , PlayDevice);
                
                SysReturnTime = SYSRETURNTIME ;
                break ;
       /*****************************************************
                          EQ
        *****************************************************/
        case MSG_MUSIC_NEXT_EQ:
                LCD1602_Clear() ;
                LCD1602_DispStr(0 , 0  , "EQ SET");
                CurrentEQ ++ ;
                if(CurrentEQ == EQ_BASS)
                {
                    CurrentEQ = EQ_NORMAL;  
                }
                Uart_SendCMD(UARTCMD_EQ_POINT , FEEDBACK , CurrentEQ);
                Disp_PlayEQ(CurrentEQ);
                SysReturnTime = SYSRETURNTIME ;
                break ;

       /*****************************************************
                          ����
        *****************************************************/
        case MSG_MUSIC_PLAY: 
                Uart_SendCMD(UARTCMD_MUSIC_PLAY , FEEDBACK , 0);
                DBG("MUSIC_PLAY\n") ;
                LCD1602_ClearLine(1);
                LCD1602_DispStr(1 , 0  , "MUSIC_PLAY");
                SysReturnTime = SYSRETURNTIME ;
                break ;
       /*****************************************************
                          ��ͣ
        *****************************************************/
        case MSG_MUSIC_PAUSE: 
                Uart_SendCMD(UARTCMD_MUSIC_PAUSE , FEEDBACK , 0);
                DBG("MUSIC_PAUSE\n") ;
                LCD1602_ClearLine(1);
                LCD1602_DispStr(1 , 0  , "MUSIC_PAUSE");
                SysReturnTime = SYSRETURNTIME ;
                break ;                   
       /*****************************************************
                          ��һ��
        *****************************************************/
        case MSG_MUSIC_NEXT_FILE:
                if(input_vol > 0)
                {
                    input_vol -- ;
                }
                Uart_SendCMD(UARTCMD_NEXT_FILE , FEEDBACK , 0);
                DBG("MSG_MUSIC_NEXT_FILE\n") ;
                LCD1602_ClearLine(1);
                LCD1602_DispStr(1 , 0  , "NEXT FILE PLAY");
                SysReturnTime = SYSRETURNTIME ;
                break ;

       /*****************************************************
                          ��һ��
        *****************************************************/
        case MSG_MUSIC_PREV_FILE:
                Uart_SendCMD(UARTCMD_PREV_FILE , FEEDBACK , 0);
                DBG("MSG_MUSIC_PREV_FILE\n") ;
                LCD1602_ClearLine(1);
                LCD1602_DispStr(1 , 0  , "PREV FILE PLAY");
                SysReturnTime = SYSRETURNTIME ;
                break ;

       /*****************************************************
                         ѭ������ģʽ����
        *****************************************************/
        case MSG_PLAYALL:
                PlayAllFlag = !PlayAllFlag ;
                LCD1602_ClearLine(1);
                if(PlayAllFlag)
                {
                    LCD1602_DispStr(1 , 0  , "ALL PLAY ON   ");
                    put_msg_lifo(MSG_PLAYALL_ON);
                    
                }
                else
                {
                    LCD1602_DispStr(1 , 0  , "ALL PLAY OFF");
                    put_msg_lifo(MSG_PLAYALL_OFF);
                }
                break ;
       /*****************************************************
                         ѭ������ģʽ--��
        *****************************************************/
        case MSG_PLAYALL_ON:                
                Uart_SendCMD(UARTCMD_PLAYALL , FEEDBACK , 0x01);
                DBG("MSG_MUSIC_PREV_FILE\n") ;
                SysReturnTime = SYSRETURNTIME ;
                break ;

       /*****************************************************
                         ѭ������ģʽ--��
        *****************************************************/
        case MSG_PLAYALL_OFF:                
                Uart_SendCMD(UARTCMD_PLAYALL , FEEDBACK , 0);
                DBG("MSG_MUSIC_PREV_FILE\n") ;
                SysReturnTime = SYSRETURNTIME ;
                break ;
        /******************************************************
                          ��������
         *****************************************************/
	    case MSG_VOL_UP:
        		input_vol++;
        		input_vol++;
        case MSG_VOL_DOWN:
                input_vol--;
                if ( input_vol == 255 )
                {
                    input_vol = 0 ;
                }
                else if (input_vol > MAX_MAIN_VOL)
                {
                    input_vol = MAX_MAIN_VOL ;
                }
                Uart_SendCMD(UARTCMD_VOL_POINT , FEEDBACK , input_vol);

                LCD1602_ClearLine(1);
                LCD1602_DispStr(1 , 0  , "CUR VOL[  ]");
                SysReturnTime = SYSRETURNTIME ;
                itoa2(input_vol);
                LCD1602_DispStr(1 , 8 , (INT8U *)bcd_number);
                break ;
                
       /*****************************************************
                          ���볬ʱ����
        *****************************************************/
		case MSG_INPUT_TIMEOUT:
                DBG("MSG_INPUT_TIMEOUT\n") ;
    			put_msg_lifo(MSG_MUSIC_PLAY_NEW_FILE);
    			break;
                
       /*****************************************************
                          ѡ��ָ������Ŀ
        *****************************************************/		
        case MSG_MUSIC_PLAY_NEW_FILE:
			    DBG("MSG_MUSIC_PLAY_NEW_FILE\n") ;
                LCD1602_Clear() ;
                if(PlayDevice == PLAYDEVICE_SLEEP)
                {
                    LCD1602_DispStr(1 , 0  , "MOD IN SLEEP ");
                }
                else
                {
                    if(POINTNUM_ONE == PointFunc)//ָ����������
                    {
                        Uart_SendCMD(UARTCMD_NUM_POINT , FEEDBACK , input_number);
                        LCD1602_DispStr(1 , 0  , "SEND ONE NUM ");
                    }
                    else if(POINTNUM_LOOP == PointFunc)//ָ������ѭ������
                    {
                        Uart_SendCMD(UARTCMD_LOOP_PLAY , FEEDBACK , input_number);
                        LCD1602_DispStr(1 , 0  , "SEND LOOP NUM ");
                    }
                    else if(POINTNUM_FOLDER == PointFunc)//ָ���ļ��к��ļ�
                    {
                        input_number = ((INT16U)(FolderNum)<<8)| FileNum ;
                        FileNum  = 0 ; 
                        FolderNum= 0 ;
                        Uart_SendCMD(UARTCMD_MUSIC_FOLDER , FEEDBACK , input_number);
                        LCD1602_DispStr(1 , 0  , "FOLD FILE POINT");
                    }
                }          
                input_number = 0 ;
                SysReturnTime = SYSRETURNTIME ;
			    break;

       /*****************************************************
                      ָ���ļ����Լ�ָ���ļ���
        *****************************************************/		
                
        case MSG_SET_PLAYMODE:
                if((++PointFunc) == POINTNUM_MAX)
                {
                    PointFunc = POINTNUM_ONE ;   
                }
                SysReturnTime = SYSRETURNTIME ;
                LCD1602_ClearLine(1);
                if(PointFunc == POINTNUM_ONE)
                {
                    LCD1602_DispStr(1 , 0  , "POINT ONE      ");
                }
                else if(PointFunc == POINTNUM_LOOP)
                {
                    LCD1602_DispStr(1 , 0  , "POINT LOOP     ");   
                }
                else if(PointFunc == POINTNUM_FOLDER)
                {
                    LCD1602_DispStr(1 , 0  , "POINT FOLDER   ");    
                }
			    break;

       /*****************************************************
                          �л��Ƿ���Ҫ����
        *****************************************************/		
                
        case MSG_UART_ACK:
                LCD1602_ClearLine(1);
                if(FEEDBACK)
                {
                    FEEDBACK = 0 ;
                    LCD1602_DispStr(1 , 0  , "NO NEED ACK");
                }
                else
                {
                    FEEDBACK = 1 ;
                    LCD1602_DispStr(1 , 0  , "NEED ACK");
                }    
                SysReturnTime = SYSRETURNTIME ;
			    break;
       /*****************************************************
                          ���ּ���������������ѡ��
                          ���ļ���ָ�����ļ���ָ��
        *****************************************************/
        case MSG_0:
        case MSG_1:
        case MSG_2:
        case MSG_3:
        case MSG_4:
        case MSG_5:
        case MSG_6:
        case MSG_7:
        case MSG_8:
        case MSG_9:
                if(POINTNUM_FOLDER == PointFunc)//ָ���ļ��е�״̬
                {
                    PointPlayFolderNum(msg);                 
                }
                else
                {
                    PointPlayNum(msg) ;                 
                }
                break ;
    }
}
/*****************************************************************************************************
 - ����˵������ʾģ�鷵�صĴ���
 - ����ģ�飺�ڲ�
 - ����˵������
 - ����˵����
 - ע      ��

*****************************************************************************************************/
void DispModlueError(INT8U error)
{
    LCD1602_Clear();
    LCD1602_DispStr(0 , 0  , "RETURN ERROR    ");
    switch(error)
    {
        case ERROR_IDLE :
                        
                        break ;
        case ERROR_Busy :
                        LCD1602_DispStr(1 , 0  , "BUSY    ");
                        break ;
        case ERROR_SLEEP :
                        LCD1602_DispStr(1 , 0  , "SLEEP    ");
                        break ;
        case ERROR_RecvError :
                        LCD1602_DispStr(1 , 0  , "RECV ERROR   ");
                        break ;
        case ERROR_CheckSError :
                        LCD1602_DispStr(1 , 0  , "SUM ERROR    ");
                        break ;
        case ERROR_FolPointOver :
                        LCD1602_DispStr(1 , 0  , "FOL OVER     ");
                        break ;
        case ERROR_FolPointNo :
                        LCD1602_DispStr(1 , 0  , "NO FIND FILE ");
                        break ;
    }
}

/*****************************************************************************************************
 - ����˵��������ѡ������
 - ����ģ�飺�ڲ�
 - ����˵������
 - ����˵����
 - ע      ��
*****************************************************************************************************/
void PointPlayNum(INT8U num)
{
    DBG("MSG_input_num\n") ;
    if(0 == InputNumCnt)
    {
        LCD1602_ClearLine(1);
    }
    InputNumCnt = 30 ;//����3Sû�а�������,���볬ʱ
    SysReturnTime = 0 ;
    LCD1602_DispStr(1 , 0  , "INPUT NUM:[");
    LCD1602_DispStr(1 , 15 , "]");
    
    input_number = input_number * 10 + num ;
    if ( input_number > 1999 )
    {
        input_number = 0;
    }
    itoa4(input_number);
    LCD1602_DispStr(1 , 11 , (INT8U *)bcd_number);    
}

/*****************************************************************************************************
 - ����˵����ָ���ļ����Լ��ļ�������
 - ����ģ�飺�ڲ�
 - ����˵������
 - ����˵����
 - ע      ��
*****************************************************************************************************/
void PointPlayFolderNum(INT8U num)
{
    static INT8U InputCnt = 0 ; 
    DBG("MSG_input_num\n") ;
    if(0 == InputNumCnt)
    {
        InputCnt = 5 ;
        LCD1602_Clear();
        LCD1602_DispStr(0 , 0  , "FOLD NUM[  ]");//ָ�����ļ���
        LCD1602_DispStr(1 , 0  , "FILE NUM[   ]");//ָ�����ļ���
    }
    InputNumCnt = 30 ;//����3Sû�а�������,���볬ʱ
    SysReturnTime = 0 ;
    if(InputCnt > 3)
    {
        FolderNum = FolderNum * 10 + num ;
        if( FileNum > 99 )
        {
            FileNum = 0;
        }
        itoa2(FolderNum);
        LCD1602_DispStr(0 , 9 , (INT8U *)bcd_number);  
    }
    else
    {
        FileNum = FileNum * 10 + num ;
        if( FileNum > 255 )
        {
            FileNum = 0;
        }
        itoa3(FileNum);
        LCD1602_DispStr(1 , 9 , (INT8U *)bcd_number);  
    }  
    if((--InputCnt) == 0)
    {
        InputCnt = 0 ;
        InputNumCnt = 10 ;        
    }
}

/*****************************************************************************************************
 - ����˵��������ָʾ��[100ms����һ��]
 - ����ģ�飺�ڲ�
 - ����˵������
 - ����˵����
 - ע      ��
*****************************************************************************************************/
void Indicator_Scan(void)
{
	static INT8U flash_cnt;

	switch(PlayledStatus)
	{
	case SET_PLAYLED_ON:			
		PLAYLED_ON;
		break;
	case SET_PLAYLED_OFF:			
		PLAYLED_OFF;
		break;
	case SET_PLAYLED_100MS_FLASH:	
		PLAYLED_FLASH;
		break;
	case SET_PLAYLED_500MS_FLASH:	
		if(++flash_cnt > 4)
		{
			flash_cnt = 0;
			PLAYLED_FLASH;
		}
		break;
	case SET_PLAYLED_900MS_ON_100MS_OFF:
		if(++flash_cnt > 4)
		{
			flash_cnt = 0;
			PLAYLED_ON;
		}
		else
		{
			PLAYLED_OFF;
		}
		break;
	}
}

/*****************************************************************************************************
 - ����˵������ʱ[100ms����һ��]
 - ����ģ�飺�ڲ�
 - ����˵������
 - ����˵����
 - ע      ��
*****************************************************************************************************/
void Time_Task(void) 
{
    if((DelayTime) && ((--DelayTime) == 0))
	{		
        put_msg_lifo(MSG_MUSIC_PLAY); ;//      	
	}
    if((UartRecv_100Ms) && ((--UartRecv_100Ms) == 0))
	{		
        UartRecvStatus = UART_RECV_IDLE ;//���ڽ����޶�ʱ��δ�������        	
	}
    if((PlayledCnt) && ((--PlayledCnt) == 0)) 
	{	
        PlayledStatus = SET_PLAYLED_OFF	;
	}   
    if((InputNumCnt) && ((--InputNumCnt) == 0)) 
	{	
        put_msg_lifo(MSG_INPUT_TIMEOUT);
	}  
    if((SysReturnTime) && ((--SysReturnTime) == 0)) 
	{	
	    if(System_Status == SYS_STATUS_INIT)
        {
            put_msg_lifo(MSG_RETURN_INIT);
        }
        else if(System_Status == SYS_STATUS_WORK)
        {
            put_msg_lifo(MSG_RETURN_INIT);
        }
	}   
    if((UartRecvACK) && ((--UartRecvACK) == 0)) //û���յ�Ӧ��
	{	
	    if(FEEDBACK)
        {
	        PlayledCnt = PLAYLEDCNT ; 
		    PlayledStatus = SET_PLAYLED_100MS_FLASH ;
        }
	}   
    if((QueryACKTime) && ((--QueryACKTime) == 0)) //û���յ�Ӧ��
	{	
	    if(PlayDevice == PLAYDEVICE_NULL)
        {
            SysReturnTime = 1 ;    
        }
        else
        {   
            put_msg_lifo(MSG_QUERY_ERROR);
        }
	} 
}


