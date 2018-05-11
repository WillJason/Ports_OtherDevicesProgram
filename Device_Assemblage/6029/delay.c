#include<reg52.h>
//延时（毫秒级） 晶振11.0592MHz
void delayms(unsigned int z)
{
	unsigned int x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);		 //1T单片机i=600，若是12T单片机i=125
}
