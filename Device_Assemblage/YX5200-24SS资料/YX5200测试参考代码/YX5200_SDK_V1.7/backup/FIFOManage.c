#include "config.h"
#include <stdio.h>
#include "FIFOManage.h"

/*****************************************************************************************************
 - ����������ϵͳ��Ϣ�������û���Ϣ
 - ����˵������
 - ���ز�������
 - ע��
*****************************************************************************************************/
INT8U app_get_msg(void) 
{
    INT8U i = MESS_ARRAY_LEN -1;
    while(i >= 0)
    {
        if (MessageArray[i].priority == SYSTEM_MESS && 
            MessageArray[i].Messable == MESS_UNUSABLE)/*ϵͳ��Ϣ����ȳ�*/
        {  
            MessageArray[i].Messable = MESS_USABLE;
//            return ((INT8U)(MessageArray[i]));
        }
        i--;
    }
    i  = 0;
    while(i < MESS_ARRAY_LEN)
    {
        if (MessageArray[i].priority == USER_MESS &&
            MessageArray[i].Messable == MESS_UNUSABLE)/*�û���Ϣ�Ƚ��ȳ�*/
        {
            MessageArray[i].Messable = MESS_USABLE;
 //          return ((INT8U)(MessageArray[i]));
        }
        i++;
    }  
    return NULL;
}


/*****************************************************************************************************
 - ���������������ȼ��Ƚ��ȳ�
 - ����˵������
 - ���ز�������
 - ע��
*****************************************************************************************************/
void put_msg_fifo(INT8U msg)
{
    INT8U i = 0;
    while(i < MESS_ARRAY_LEN)
    {
        if(MessageArray[i].Messable == MESS_USABLE)
        {
            memcpy(&MessageArray[i], &msg, sizeof(MessageDef));
            MessageArray[i].priority = USER_MESS;
            MessageArray[i].Messable = MESS_UNUSABLE;
        }
        i++;
    }  
}


/*****************************************************************************************************
 - ���������������ȼ�Ϊ����ȳ�
 - ����˵������
 - ���ز�������
 - ע��
*****************************************************************************************************/
void put_msg_lifo(INT8U msg)
{
    INT16U i = MESS_ARRAY_LEN -1;
    while(i >= 0)
    {
        if(MessageArray[i].Messable == MESS_USABLE)
        {
            memcpy(&MessageArray[i], &msg, sizeof(MessageDef));   
            MessageArray[i].Messable = MESS_UNUSABLE;
            MessageArray[i].priority = SYSTEM_MESS;
        }
        i--;
    }
}

void flush_all_msg(void)
{
    memset(MessageArray, 0, sizeof(MessageArray));
}

