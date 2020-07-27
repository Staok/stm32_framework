#include "BareConfig.h"



void Bare_Begin(void)
{
	static u8 RunTimeFaultCheck_ENABLE = 0;		/*使能故障巡检，300ms周期*/
	static u8 RunTimeFaultCheck_TtemNow = 1;	/*故障巡检当前检查的项目，可以定位*/
	const u8 RunTimeFaultCheck_TtemMaxNum = 3;	/*故障巡检的最大项目数量*/
	
	for(;;)
	{
		/*注意：这里尽量不要用10毫秒中断标志！这里只用100毫秒、300毫秒、1秒和一分钟的中断标志，够用了*/
		
		/*100ms周期执行：可以为按键检测和处理等*/
		if(Timer_IT_flags._100msec_flag == TRUE)
        {
            Timer_IT_flags._100msec_flag = FALSE;
		}
		/*300ms周期执行：通常为运行时错误自检以及处理，刷屏等*/
		if(Timer_IT_flags._300msec_flag == TRUE)
		{
			Timer_IT_flags._300msec_flag = FALSE;
			/*集中故障巡检START*/
			if(RunTimeFaultCheck_ENABLE)
			{
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
			}
			/*集中故障巡检END*/
			
			/*读入器件信息*/
			
			/*刷新界面信息*/
			
		}
		
		if(Timer_IT_flags._1sec_flag == TRUE)
		{
			Timer_IT_flags._1sec_flag = FALSE;
			
		}
		
		if(Timer_IT_flags._1min_flag == TRUE)
		{
			Timer_IT_flags._1min_flag = FALSE;
			
		}
	}
}


