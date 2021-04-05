#include "led.h"
#include "beep.h"
#include "rtc.h" 
#include "delay.h"
#include "sys.h"
#include "usart.h" 
#include "usmart.h" 
#include "oled.h"
//主函数//
 int main(void)
 {  
    u8 t=0,t1; 
    delay_init();                                      //延时函数初始化//   
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);    //设置中断优先级分组为组2：2位抢占优先级，2位响应优先级//
    uart_init(115200);                                 //串口初始化为115200//
    LED_Init();                                        //LED端口初始化//
    BEEP_Init();
    BEEP=1;
    delay_ms(1000);
    BEEP=0;
    LED0=0;                                            //LED0亮//
    usmart_dev.init(SystemCoreClock/1000000);          //初始化USMART// 
    RTC_Init();                                        //RTC初始化//
    OLED_Init(); 
      OLED_ShowString(0,0,"ALIENTEK",24);  
      OLED_ShowString(0,24, "0.96' OLED TEST",16);  
      OLED_ShowString(0,40,"ATOM 2015/1/14",12);  
      OLED_ShowString(0,52,"ASCII:",12);  
      OLED_ShowString(64,52,"CODE:",12); 
      OLED_Refresh_Gram();
      t1=' ';    
 while(1)
    { 
       
        if(t!=calendar.sec)
        {
        t=calendar.sec;                                  //如果calendar.sec不更新，则t不更新//

        LED0=!LED0;                                      //每次calendar.sec更新，LED0和LED1交替闪烁//
        LED1=!LED1;
        } 
        delay_ms(10);          
    }  
 }


 
