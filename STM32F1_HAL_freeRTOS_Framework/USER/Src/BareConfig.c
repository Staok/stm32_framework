#include "BareConfig.h"

#define RunTimeFaultCheck_ENABLE 0	/*使能故障巡检，300ms周期*/

void Bare_Begin(void)
{
	#if RunTimeFaultCheck_ENABLE
		static u8 RunTimeFaultCheck_TtemNow = 1;	/*故障巡检当前检查的项目，可以定位*/
		const u8 RunTimeFaultCheck_TtemMaxNum = 3;	/*故障巡检的最大项目数量*/
	#endif
	
//	char* Head_buf = {"For Test!"};
//	DrawPageHead((u8*)Head_buf);
	
	for(;;)
	{
		/*注意：这里尽量不要用10毫秒中断标志！这里只用100毫秒、300毫秒、1秒和一分钟的中断标志，够用了*/
		
		if(Timer_IT_flags._10msec_flag == TRUE)
		{
			Timer_IT_flags._10msec_flag = FALSE;
			
			
		}
		
		
		/*100ms周期执行：可以为按键检测和处理等*/
		if(Timer_IT_flags._100msec_flag == TRUE)
        {
            Timer_IT_flags._100msec_flag = FALSE;
			
			/*100ms周期要做的事情*/
			//TestLED_Ctrl = !TestLED_Ctrl;
			
			/*串口回显*/
			char buf4uart1[(USART1_RX_BUF_MaxNum > USART1_RX_FIFO_MaxNum) ? (USART1_RX_BUF_MaxNum):(USART1_RX_FIFO_MaxNum)];
			if(sys_USART1_RX_Fetch(FALSE, buf4uart1) == ReturnOK)
			{
				printf_uart(UART1,"%s-%d",buf4uart1,mystrlen(buf4uart1));
			}
			
		}
		/*300ms周期执行：通常为运行时错误自检以及处理，刷屏等*/
		if(Timer_IT_flags._300msec_flag == TRUE)
		{
			Timer_IT_flags._300msec_flag = FALSE;
			
			/*集中故障巡检START*/
			#if RunTimeFaultCheck_ENABLE
				switch(RunTimeFaultCheck_TtemNow)
				{
					case 1:	
							//形式参考-> if( 执行检查的函数 ！= ReturnOK ) FaultASSERT(Init_OK_Num);
							break;
					case 2:	
							break;
					case 3:	
							break;
					default:break;
				}
				if(++RunTimeFaultCheck_TtemNow > RunTimeFaultCheck_TtemMaxNum)
					RunTimeFaultCheck_TtemNow = 1;
			#endif
			/*集中故障巡检END*/
			
			/*3100ms周期要做的事情*/
			
			/*读入器件信息*/
			
			/*刷新界面信息*/
			char RTC_buf[50];
			sprintf(RTC_buf,"%d-%d-%d   %d-%d-%d",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);
			printf_uart(UART1,"%s",RTC_buf);
			
			
		}
		
		if(Timer_IT_flags._1sec_flag == TRUE)
		{
			Timer_IT_flags._1sec_flag = FALSE;
			
			/*1s周期要做的事情*/
			//printf_uart(UART3,"UART333---One Second\r\n");
			//printf_uart(UART2,"UART222---One Second\r\n");
			//printf_uart(UART1,"UART111---One Second\r\n");
			
			
		}
		
		if(Timer_IT_flags._1min_flag == TRUE)
		{
			Timer_IT_flags._1min_flag = FALSE;
			
			/*1min周期要做的事情*/
			
		}
	}
}


