#include<reg52.h>
//��ʱ�����뼶�� ����11.0592MHz
void delayms(unsigned int z)
{
	unsigned int x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);		 //1T��Ƭ��i=600������12T��Ƭ��i=125
}
