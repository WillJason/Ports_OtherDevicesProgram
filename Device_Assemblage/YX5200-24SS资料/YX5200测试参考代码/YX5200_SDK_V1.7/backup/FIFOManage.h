#ifndef _MESSAGE_H
#define _MESSAGE_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MESS_ARRAY_LEN       32

#define MESS_USABLE          0x00
#define MESS_UNUSABLE        0x01

#define COMMON_MESS          0
#define SYSTEM_MESS          1
#define USER_MESS            2

typedef struct TagMessageDef
{
    INT16U priority:2;
    INT8U  Messable:1;
    INT8U  MessData:5;
}MessageDef;

MessageDef MessageArray[MESS_ARRAY_LEN] = { 0 };

INT8U app_get_msg(void);

void put_msg_fifo(INT8U msg);

void put_msg_lifo(INT8U msg);

void flush_all_msg(void);

#endif 