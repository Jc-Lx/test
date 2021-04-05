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
    OLED_ShowString(0,50,"DesignBy:JC-LX",12);  

      OLED_Refresh_Gram();  
 while(1)
    {   
        if(t!=calendar.sec)
        {
         t=calendar.sec;                                  //如果calendar.sec不更新，则t不更新//  
         OLED_ShowNum(10,0,calendar.hour,2,16);
         OLED_ShowChar(20,0,':',12,1);
         OLED_ShowNum(40,0,calendar.min,2,16);
         OLED_ShowChar(50,0,':',12,1);
         OLED_ShowNum(80,0,calendar.sec,2,16);

         OLED_ShowNum(0,24,calendar.w_year,4,16);
         OLED_ShowChar(40,24,'/',12,1);
         OLED_ShowNum(55,24,calendar.w_month,2,16);
         OLED_ShowChar(48,24,'/',12,1);
         OLED_ShowNum(75,24,calendar.w_date,2,16);
         OLED_Refresh_Gram();
         LED0=!LED0;                                      //每次calendar.sec更新，LED0和LED1交替闪烁//
        } 
        delay_ms(10);          
    }  
 }


 
