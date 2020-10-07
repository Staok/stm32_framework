/*
*项目说明：STM32F2编程框架
*作者：Staok，邮箱：superxhy@qq.com
*开源地址：仓库地址 https://github.com/Staok/stm32_framework
*如果觉得好用，请注明开源仓库地址，待我以后研究下开源协议再加上开源协议嘿嘿
*/

#include "PeriphconfigCore.h" /*所有头文件均放在这里面*/

int main(void)
{
	HAL_Init();                    		/*初始化HAL库*/
	sys_MCU_Init_Seq();					/*MCU外设初始化序列，所有初始化写到这里面——————！按需要进行修改！*/
	HAL_Delay(66);
	sys_Device_Init_Seq();				/*器件外设初始化，并开机自检——————！按需要进行修改！*/
	HAL_Delay(234);
	while(1)
	{
		#if SYSTEM_SUPPORT_OS
			Task_Begin(); 			/*开始跑FreeRTOS*/
		#else
			Bare_Begin();			/*开始跑裸机*/
		#endif
	}
}

/*
使用说明：
	外设剪裁和配置在PeriphconfigCore.h里
	
	初始化函数均在PeriphconfigCore.c里
	所有外设API均在PeriphconfigCore.h，新文件使用时请调用
	所有GPIO的API均在Periphconfig.h，新文件使用时请调用
	
	FreeRTOS任务编写在TaskConfig.c里
	裸机程序任务编写在BareConfig.c里
*/



