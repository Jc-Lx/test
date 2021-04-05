#include "delay.h"
#include  "sys.h"
#include "led.h"
#include "beep.h"
#include  "wdg.h"
#include "key.h"


int main(viod)
{
	delay_init();
	LED_Init();
	BEEP_Init();
	KEY_Init();
	delay_ms(1000);
	IWDG_Init(4,625);
	LED0=0;
	BEEP=1;
	while (1)
	{
		if (WK_UP==1)
		{	
			IWDG_Feed();
		}
		delay_ms(10);
		
	}

}

