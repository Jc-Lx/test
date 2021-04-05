#include "delay.h"
#include  "sys.h"
#include "led.h"
#include "beep.h"
#include  "wdg.h"
#include "key.h"


int main(viod)
{
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
	BEEP_Init();
	KEY_Init();
	LED0=0;
	// BEEP=1;
	delay_ms(300);
	WWDG_Init(0x7f,0x5f,WWDG_Prescaler_8);
	while (1)
	{
		LED0=1;
		delay_ms(10);
		
	}

}

