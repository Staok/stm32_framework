#ifndef __PERIPHCONFIGCORE_H
#define __PERIPHCONFIGCORE_H


/*________________________________模板固定搭配____________________________________________*/
/*所有main的头文件都放在这里*/
#include "sysVar.h"								/*定义系统级常用的变量、数据类型和二进制表示宏*/
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"						/*在里面的stm32f4xx_hal_conf.h里面选择用哪些外设的HAL库——————！按需要进行修改！*/

#include "arm_math.h"							/*ARM DSP LIB CMSIS 5.7.0*/

#include "malloc.h"								/*借鉴的 内存管理 章节的源代码自实现的malloc和free*/	
#include "sys_menu.h"							/*模板固定搭配！必须开启！提供一个菜单模板，把系统的输入、输出、执行功能的标志位控制全部打包！相当于io_ctrl*/
#include "fsm.h"								/*模板固定搭配！必须开启！提供一个有限状态机（fsm）库，用于管理逻辑较为复杂的程序，帮助更好的写阅读性强的 状态图实现*/
												/*注：	sys_menu 用于管理输入（按键等）、输出（屏幕等）和系统参数的用户控制
														fsm 用于管理 用状态图 描述的多状态、多条件的复杂逻辑任务
														齐活！*/
#include "MyString.h"							
												/*模板固定搭配！必须开启！提供一个实现了string.h大部分字符操作函数的库*/
int myatoi(const char *str);					/*提供一个字符串转整形的实现*/
#include "lwrb.h"								/*提供一个FIFO实现，github开源库地址：https://github.com/MaJerle/lwrb*/
unsigned char BCD2HEX(unsigned char bcd_data); 	/*提供BCD转为HEX子程序*/
unsigned char HEX2BCD(unsigned char hex_data); 	/*提供HEX转为BCD子程序*/
unsigned int Curl_rand(void);					/*提供实现伪随机数的函数*/
#include "printf.h"								/*模板固定搭配！必须开启！包含且编译printf.h，github开源文件，无依赖，功能比较全。开源仓库地址：https://github.com/mpaland/printf*/
#define STSTEM_TIM4_ENABLE		1				/*模板固定搭配！必须开启！使用通用定时器4，提供时基，默认10ms周期中断*/
#define STSTEM_TIM3PWM_ENABLE	1				/*模板基本功能，必须开启！使用通用定时器3，默认不开启中断，提供四路PWM输出*/
												/*默认通道4给蜂鸣器，蜂鸣器低电平有效！蜂鸣器控制需要用到TIM4！
												改变占空比可调响度，改变频率可调声调*/
												/*默认向上计数，输出极性为LOW，PWM1模式，即设置为当计数值小于此值时输出低电平。*/
												/*默认：引脚上拉							(CH1/PA6, CH2/PA7, CH3/PB0, CH4/PB1)*/
		#define STSTEM_TIM3PWM_REMAP_PARTIAL	0	/*定时器3部分引脚重映射，Partial remap  (CH1/PB4, CH2/PB5, CH3/PB0, CH4/PB1)*/
		#define STSTEM_TIM3PWM_REMAP_ENABLE		0	/*定时器3全部引脚重映射，Full remap     (CH1/PC6, CH2/PC7, CH3/PC8, CH4/PC9)*/
		#define STSTEM_TIM3PWM_CHANNEL_ENABLE	(B0000_1000) /*输出通道选择，共四个通道*/
													/*可选  B0000_0001|B0000_0010|B0000_0100|B0000_1000	*/
		#define TIM3PWM_BuzzerCh				TIM3PWM_Channel_4	//选择用于蜂鸣器的PWM通道
		#define STSTEM_TIM3PWM_TI_ENABLE		0	/*是否开启定时器3的定时中断，除非急需用，否则一般不开*/
		
		#define tim3prsc STSTEM_TIM3PWM_prsc_12K	/*选择定时器输出频率，以下六条语句基本不动（重装值为1000，这里是选择预分频系数）*/
			#define STSTEM_TIM3PWM_prsc_2K	(42-1)
			#define STSTEM_TIM3PWM_prsc_4K	(21-1)
			#define STSTEM_TIM3PWM_prsc_12K	(7-1)
			#define STSTEM_TIM3PWM_prsc_21K	(4-1)
			#define STSTEM_TIM3PWM_prsc_42K	(4-1)
			#define STSTEM_TIM3PWM_prsc_84K	(1-1)
			#define tim3arr 1000-1

/*_____________________________________\\\                               ///____________________________________________*
 *___________________________________________________组件选择___________________________________________________________*
 *_____________________________________///                               \\\____________________________________________*/

#include "isUseFreeRTOS.h"						/*FreeRTOS 10.3.1，默认用于任务的RAM堆栈大小为30KB，按需修改，不要有没有语句的空任务！
													在此.h文件里面控制是否开启，在FreeRTOSConfig.h文件里配置*/
	
	#if SYSTEM_SUPPORT_OS
		#include "FreeRTOS.h"	//这里不要乱动	  
		#include "task.h"
		#include "queue.h"
		#include "TaskConfig.h"
	#else
		#include "BareConfig.h"
	#endif
	
#define SYSTEM_USB_ENABLE		1				/*STM32 USB库 使用*/
	
	#if SYSTEM_USB_ENABLE
		#include "usbd_usr.h"
	#endif

#include "lwipopts.h"							/*lwip 2.1.2，在里面配置和控制是否开启*/

#if SYS_SUPPORT_LWIP
	#include "lwip_comm.h"
#endif

#define SYSTEM_FATFS_ENABLE		0 				/*支持ff14，FATFS文件系统，管理SDIO驱动的SD卡，SPI驱动的SD卡，外部SPI FLASH，USB文件系统等等*/
												/*启用后，在初始化序列里加入FATFS的初始化和挂载相应的设备*/
												/*需要启用相应的SDIO外设或者SPI外设和其相应的IO*/
												/*到初始化序列中选择挂载哪些外设*/
#if SYSTEM_FATFS_ENABLE
	#include "ff.h"
	#include "fatfs_app.h"
#endif

#define SYSTEM_SUPPORT_pid		0				/*提供一个pid算法实现库，集成了积分分离和变限积分，以及可选的不完全微分和微分先行，具体用法看pid.h里面*/
												/*里面另提供一个一阶低通滤波实现函数*/											
	#if SYSTEM_SUPPORT_pid						/*如果不包含，其他文件就无法调用此组件的API*/
		#include "pid.h"
	#endif

#define SYSTEM_SUPPORT_simuspi	0				/*是否调用软件模拟SPI，具体用法在其.h文件里介绍，或者看Docs手册，更详细*/
	#if SYSTEM_SUPPORT_simuspi					
		#include "simuspi.h"					
	#endif
	
#define SYSTEM_SUPPORT_simui2c	0				/*是否调用软件模拟I2C，具体用法在其.h文件里介绍，或者看Docs手册，更详细*/
	#if SYSTEM_SUPPORT_simui2c
		#include "simui2c.h"
	#endif
	
#define SYSTEM_SUPPORT_SFUD		0				/*是否支持SPI串行FLASH，使用开源库https://github.com/armink/SFUD，驱动SPI FLASH*/
												/*配置SPI FLASH型号和具体使用看 sfud_cfg.h，配置底层在 sfud_port.c*/
	#if SYSTEM_SUPPORT_SFUD
		#include "sfud.h"
	#endif
	
	
/*DEVICES所有头文件*/
#include "TFTLCD.h"
//#include "OLED.h"
//#include "mpu6050.h"


/*_____________________________________\\\                               ///____________________________________________*
 *_________________________________________________用户GPIO配置_________________________________________________________*
 *_____________________________________///                               \\\____________________________________________*/

/*在PeriphConfig.c文件里去配置用户GPIO*/

/*_____________________________________\\\                               ///____________________________________________*
 *___________________________________________________外设配置___________________________________________________________*
 *_____________________________________///                               \\\____________________________________________*/

#define SYSTEM_IWDG_ENABLE		0			/*开启独立看门狗，默认1S的喂狗周期，默认在TIM4定时中断里喂狗，用IWDG必开TIM4*/
											/*注：看门狗和低功耗待机模式不能同时开启，因为看门狗不能关闭，看门狗复位会唤醒低功耗状态*/
											
#define SYSTEM_MCO_PA8_OUT		0			/*设置PA8为MCO输出，默认时钟源为HSE（5M），五倍分频（最大分频）*/

#define SYSTEM_RTC_ENABLE		1			/*配置使用RTC，确保LSE连接有36.768kHz的晶振，确保RTC有VBAT备用电源，默认不开启RTC闹钟中断*/

#define SYSTEM_RNG_ENABLE		0			/*硬件产生32位真随机数*/
	extern u8 GenerateRNG_Ready;				//RNG产生好标志位
	extern u32 randomNum32bit; 					//存32位RNG容器

#define SYSTEM_CRC_ENABLE		1			/*配置使用CRC循环冗余校验*/

#define SYSTEM_UART1_ENABLE			1		/*使能串口1	       TX/PA9, RX/PA10		*/
#define SYSTEM_UART1_REMAP_ENABLE	0		/*串口1引脚重映射：TX/PB6, RX/PB7		*/
#define SYSTEM_UART1_BOUND			115200	/*串口1波特率*/

#define SYSTEM_UART2_ENABLE			0		/*使能串口2	       TX/PA2, RX/PA3		*/
#define SYSTEM_UART2_REMAP_ENABLE	0		/*串口2引脚重映射：TX/PD5, RX/PD6		*/
#define SYSTEM_UART2_BOUND			115200	/*串口2波特率*/

#define SYSTEM_UART3_ENABLE			0		/*使能串口3	       TX/PB10, RX/PB11		*/
#define SYSTEM_UART3_REMAP_ENABLE	0		/*串口3引脚重映射：TX/PD8,  RX/PD9		*/
#define SYSTEM_UART3_BOUND			115200	/*串口3波特率*/
											/*默认均为：8位数据，1位停止，无校验，收发模式，开启接受中断*/
											/*注：串口2、3的接收回调函数没有补全，用时再补，和串口1的同理*/
											
#define SYSTEM_CAN1_ENABLE			0		/*使用CAN1,默认不使用中断，引脚默认上拉，默认使用过滤器0，关联FIFO0，默认接收任何ID的消息数据*/
	#define CAN1_RX0_INT_ENABLE		1		/*CAN1接收中断使能*/
											/*更多API详看docs.txt*/
											/*默认引脚：	PB8     ------> CAN1_RX		可到源码处修改
															PB9     ------> CAN1_TX		可以按照 stm32f407ze.pdf 手册的 62页 开始自行选择引脚，可选的还挺多的*/

#define SYSTEM_UseTIM5ForTiming_ENABLE	1			/*使用TIM5测量一个函数运行的时间，单位us*/

													/*高级定时器1，专门用于产生PWM信号：三路互补PWM输出、死区控制、刹车信号输入*/
#define STSTEM_TIM1PWM_ENABLE		0				/*是否启用并初始化TIM1的三路互补PWM（前三个通道）和一路PWM（通道4）*/
			#define STSTEM_TIM1PWM_useBreak	1		/*是否启用BKIN刹车信号控制输入，默认低电平有效*/
													/*选择每一个通道的有效电平和空闲电平（可能是刹车时的电平），默认死区3us，可修改*/
													/*			CH1/CH1N		CH2/CH2N		CH3/CH3N		CH4		BKIN（刹车信号输入）
													默认		PA8/PB13		PA9/PB14		PA10/PB15		PA11	PB12
													部分重映射	PA8/PA7			PA9/PB0			PA10/PB1		PA11	PA6
													完全重映射	PE9/PE8			PE11/PE10		PE13/PE12		PE14	PE15	*/
			#define STSTEM_TIM1PWM_REMAP_PARTIAL  0	/*定时器1部分引脚重映射（比较灵活，可以在配置文件处按照手册混搭更改）*/
			#define STSTEM_TIM1PWM_REMAP_ENABLE	  0	/*定时器1全部引脚重映射（比较灵活，可以在配置文件处按照手册混搭更改）*/
													/*输出通道选择，共四个通道，可以相与打开多个通道，互补通道默认成对打开*/
			#define STSTEM_TIM1PWM_CHANNEL_ENABLE (B0000_0001|B0000_0010|B0000_0100|B0000_1000)		/*必须加括号括起来*/
			#define tim1prsc STSTEM_TIM1PWM_prsc_4K		/*选择定时器输出PWM频率，以下六条语句基本不动（重装值为2000，这里是选择预分频系数）*/
			#define STSTEM_TIM1PWM_prsc_2K	(42-1)
			#define STSTEM_TIM1PWM_prsc_4K	(21-1)
			#define STSTEM_TIM1PWM_prsc_12K	(7-1)
			#define STSTEM_TIM1PWM_prsc_21K	(4-1)
			#define STSTEM_TIM1PWM_prsc_42K	(4-1)
			#define STSTEM_TIM1PWM_prsc_84K	(1-1)
				#define tim1arr 2000-1

													/*高级定时器8，专门用于产生PWM信号：三路互补PWM输出、死区控制、刹车信号输入*/
#define STSTEM_TIM8PWM_ENABLE		0				/*是否启用并初始化TIM8的三路互补PWM（前三个通道）和一路PWM（通道4）*/
			#define STSTEM_TIM8PWM_useBreak	1		/*是否启用BKIN刹车信号控制输入，默认低电平有效*/
													/*选择每一个通道的有效电平和空闲电平（可能是刹车时的电平），默认死区3us，可修改*/
													/*			CH1/CH1N		CH2/CH2N		CH3/CH3N		CH4		BKIN（刹车信号输入）
													默认引脚	PC6/PA7			PC7/PB0			PC8/PB1			PC9		PA6
													没有重映射功能		*/
													/*输出通道选择，共四个通道，可以相与打开多个通道，互补通道默认成对打开*/
			#define STSTEM_TIM8PWM_CHANNEL_ENABLE (B0000_0001|B0000_0010|B0000_0100|B0000_1000)		/*必须加括号括起来*/
			#define tim8prsc STSTEM_TIM8PWM_prsc_4K		/*选择定时器输出频率，以下六条语句基本不动（重装值为2000，这里是选择预分频系数）*/
			#define STSTEM_TIM8PWM_prsc_2K	(42-1)
			#define STSTEM_TIM8PWM_prsc_4K	(21-1)
			#define STSTEM_TIM8PWM_prsc_12K	(7-1)
			#define STSTEM_TIM8PWM_prsc_21K	(4-1)
			#define STSTEM_TIM8PWM_prsc_42K	(4-1)
			#define STSTEM_TIM8PWM_prsc_84K	(1-1)
				#define tim8arr 2000-1

											/*通过用定时器2：16位，四个独立通道可用于：输入捕获、输入比较、PWM、单脉冲，多种途径触发DMA中断*/
#define STSTEM_TIM2_ENABLE			0		/*通用定时器2，功能自定，默认分频系数为72，初始化函数在PeriphCconfig.c里面定义*/
	#define STSTEM_TIM2_TI_ENABLE	0		/*是否开启定时器2的定时中断*/
	
	#define STSTEM_TIM2_asPWMorCap	3		/*选择定时器2作为...注：PWM(输出比较)、输入捕获和正交解码三个功能不能共用！*/
											/*写3作为正交编码器的解码使用，只能使用CH1和CH2，即PA15和PB3（已经默认上拉），默认使用两路边沿触发，计数值为单路上升沿数的四倍
												正交解码使用到TIM2的定时中断，必须打开！但在初始化时已经默认打开*/
											/*写2作为普通定时器中断使用*/
											/*写1作为输入捕获：可以获取高电平或者低电平的时间，默认不分频，不滤波
												输入捕获使用到TIM2的定时中断，必须打开！但在初始化时已经默认打开*/
											/*写0作为PWM：默认PWM1模式，向上计数，低电平有效，一个周期内占空比越大低电平时间越长
												引脚说明：			没有重映射			两种部分重映射			完全重映射
												TIM2四个通道：		PA0 PA1 PA2 PA3		含有前面的引脚		PA15 PB3 PB10 PB11
												注意：由于PA2 PA3串口2占用，PA0 PA1由ADC占用，所以TIM2的PWM只用完全重映射！！
														但PB10 PB11串口3占用，要么不用串口3，要么串口3重映射
												初始化后自动打开相应通道的PWM输出 */
													/*PWM输出功能选项*/
		#define tim2arr STSTEM_TIM2PWM_Period_5K			/*选择定时器输出频率（预分频系数为84，计数频率为1Mhz，这里选择重装值）*/
			#define STSTEM_TIM2PWM_Period_1K	(1000-1)	/*注意：这里是选择重装值，也是占空比的分辨率，频率高则分辨率低，反之亦然，慎选*/
			#define STSTEM_TIM2PWM_Period_2K	(500-1)
			#define STSTEM_TIM2PWM_Period_5K	(200-1)
			#define STSTEM_TIM2PWM_Period_10K	(100-1)
			#define STSTEM_TIM2PWM_Period_20K	(50-1)
			#define STSTEM_TIM2PWM_Period_50K	(20-1)
			#define STSTEM_TIM2PWM_Period_100K	(10-1)
			#define STSTEM_TIM2PWM_Period_200K	(5-1)
		#define STSTEM_TIM2PWM_CHANNEL_ENABLE	(B0000_0001|B0000_0010) /*输出通道选择，共四个通道，可以相与打开多个通道，PA15 PB3 PB10 PB11*/
		
													/*输入捕获功能选项，自动设置庄装在值为0xff，计数频率为1Mhz*/
		#define STSTEM_TIM2_Cap_trigV	1			/*写1高电平触发一次输入捕获，写0相反，写2双边沿触发*/
		#define STSTEM_TIM2_Cap_Channel	B0000_0001	/*选择用哪个通道当输入捕获，只能四选一！！*/
		
													/*正交解码功能选项，自动设置庄装在值为0xff，计数频率为1Mhz*/
													// 4 : 			使用定时器编码器接口捕获AB相的上升沿和下降沿，一个脉冲*4
													// ENCODER：	编码器线数(转速一圈输出脉冲数)
													// SPEEDRATIO：	电机减数比，内部电机转动圈数与电机输出轴转动圈数比，即减速齿轮比，编码器一般和电机转速一致，即直接把编码器安装到电机轴上！
		#define ENCODER     300    					// 编码器线数
		#define SPEEDRATIO  1   					// 电机减速比
		#define PPR         (SPEEDRATIO*ENCODER*4) 	// Pulse/r 每圈可捕获的脉冲数，不用动！
			

													/*基本定时器6、7，只能用于定时中断，提供更多的同步功能*/
#define STSTEM_TIM6_ENABLE		0
	#define tim6arr STSTEM_TIM6_Period_5K			/*选择定时器6定时中断频率（预分频系数为84，这里选择重装值）*/
		#define STSTEM_TIM6_Period_1K	(1000-1)
		#define STSTEM_TIM6_Period_2K	(500-1)
		#define STSTEM_TIM6_Period_5K	(200-1)
		#define STSTEM_TIM6_Period_10K	(100-1)
		#define STSTEM_TIM6_Period_20K	(50-1)
		#define STSTEM_TIM6_Period_50K	(20-1)
		#define STSTEM_TIM6_Period_100K	(10-1)
		#define STSTEM_TIM6_Period_200K	(5-1)
#define STSTEM_TIM7_ENABLE		0
	#define tim7arr STSTEM_TIM6_Period_5K			/*选择定时器7定时中断频率（预分频系数为84，这里选择重装值）*/
		#define STSTEM_TIM7_Period_1K	(1000-1)
		#define STSTEM_TIM7_Period_2K	(500-1)
		#define STSTEM_TIM7_Period_5K	(200-1)
		#define STSTEM_TIM7_Period_10K	(100-1)
		#define STSTEM_TIM7_Period_20K	(50-1)
		#define STSTEM_TIM7_Period_50K	(20-1)
		#define STSTEM_TIM7_Period_100K	(10-1)
		#define STSTEM_TIM7_Period_200K	(5-1)

													/*DAC两个输出通道 PA4和PA5，默认初始化为12位右对齐，速度最快为250K左右，输出范围 0~Vref+，默认Vref+为3.3V，否则自行更改转换计算*/
#define SYSTEM_DAC_OUT1_ENABLE	0					/*使能DAC1*/
	#define SYSTEM_DAC_OUT1_TriangleWave_ENABLE	1	/*可选输出 噪声波形 和 三角波波形 ，默认不适用外部触发，可选触发源在初始化函数中修改（有定时器中断和外部中断线9）*/
	#define SYSTEM_DAC_OUT1_NoiseWave_ENABLE	0	
#define SYSTEM_DAC_OUT2_ENABLE	0					/*使能DAC2*/
	#define SYSTEM_DAC_OUT2_TriangleWave_ENABLE	0	/*可选输出 噪声波形 和 三角波波形 ，默认不适用外部触发，可选触发源在初始化函数中修改（有定时器中断和外部中断线9）*/
	#define SYSTEM_DAC_OUT2_NoiseWave_ENABLE	1	
													/*对于ADC和DAC，如果Vref+连接到3.0V基准，则要修改程序！*/
													/*hd系列外设，DAC两个输出通道 PA4和PA5，默认初始化为12位右对齐，速度最快为250K左右，
															输出范围 0~Vref+，默认Vref+为3.3V，否则自行更改转换计算*/


											/*参考电压 Vref-必须连到VSSA，Vref+可连到 2.4V~VDDA，ADC输入电压必须小于Vref+，默认Vref+为3.3V，否则自行更改转换计算*/
#define SYSTEM_ADC1_ENABLE		0			/*启否ADC1*/
	#define SYSTEM_ADC1_useScan		1		/*启否规则组的连续扫描，如果启用，则把下面定义的所有通道都放到规则组里，并使用DMA2 Stream0的通道0把转换结果放到目标位置
												如果不启用，则为软件触发的单次转换*/
		#define SYSTEM_ADC1_useCircular	1	/*只在扫描模式下有效；开启则自动循环ADC转换，只需要判断标志位和读数即可，不需要软件手动触发开启一次ADC转换*/
	#define SYSTEM_ADC1_useChanlNum	4		/*定义共用多少路通道*/
											/*	ADC1的通道与引脚映射：
											通道：	0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	   16		     17
											IO	：	A0	A1	A2	A3	A4	A5	A6	A7	B0	B1	C0	C1	C2	C3	C4	C5	内部温度	内部参考电压
											标志：B1in16					  B8in16					  B15in16	 InrTemp */
	#define SYSTEM_ADC1_useChanl	B4in16|B5in16|B8in16|InrTemp /*定义共用哪些通道，可写B1in16~B15in16（分别代表通道0~14），和InrTemp(内部温度通道)
														（第16个ADC1通道留作内部温度标志所以不可用）*/
													/*如果只用采集内部温度，而不用其他通道，应设置：SYSTEM_ADC1_useChanlNum为1  SYSTEM_ADC1_useChanl单设为InrTemp*/
		#define InrTemp B16in16
		extern u16 adValue[SYSTEM_ADC1_useChanlNum];/*扫描模式下DMA1把ADC转换结果传送的目标位置*/
		extern u8 adValueDone; 						/*扫描模式下转换完成标志*/


#define SYSTEM_SPI1_ENABLE		0			/*使能硬件SPI1*/
#define SYSTEM_SPI2_ENABLE		0			/*使能硬件SPI2*/
											/*	SPI1->CS	SPI1->CLK	SPI1->MISO	SPI1->MOSI 	—————— 	SPI2->CS	SPI2->CLK	SPI2->MISO	SPI2->MOSI */

#define SYSTEM_StdbyWKUP_ENABLE	1			/*使用待机-低功耗模式，PA0为唤醒键，占用0线外部中断*/
											/*注：看门狗和低功耗待机模式不能同时开启，因为看门狗不能关闭，看门狗复位会唤醒低功耗状态*/

#define SYSTEM_FSMC_ENABLE			0		//是否启用FSMC
	#define SYSTEM_FSMC_use4LCD		0		//启用FSMC用于驱动LCD，则相关代码被编译，相关API可用
	#define SYSTEM_FSMC_use4SRAM	1		//启用FSMC用于驱动SRAM，则相关代码被编译，相关API可用

#define SYSTEM_FLASH_IAP_ENABLE		0		/*启用对内部FLASH储存空间编程*/
	#define STM32_FLASH_WREN		0		/*启用写功能，否则只读不写*/
	#define FLASH_SAVE_FATFS_SIZE	124		/*内部FLASH给FATFS划分的大小，单位KB*/
												/*以下不用动了！！*/
	#define STM32F407ZG_FLASH_SIZE 	1024 		/*所选STM32的FLASH容量大小(单位为KB)*/
	#define STM32_FLASH_BASE 0x08000000 		//STM32 FLASH的起始地址
		//设置FLASH 保存地址(必须为页的首地址，且必须为2的倍数！且其值要大于代码所占用FLASH的大小+0X08000000)
	#define FLASH_SAVE_FATFS_ADDR 	(STM32_FLASH_BASE + (u32)((STM32F407ZG_FLASH_SIZE - FLASH_SAVE_FATFS_SIZE)*1024))   //最后124KB留给FATFS作为文件系统（如果使用FATFS的话）
	#define FLASH_SAVE_ADDR  		(STM32_FLASH_BASE + (u32)((STM32F407ZG_FLASH_SIZE - FLASH_SAVE_FATFS_SIZE - 2)*1024)) //留2KB作为参数保存


#define SYSTEM_SDIO_SD_ENABLE	1				/*启用SDIO接口驱动SD卡，引脚：PC8/SDIO_D0，PC9/SDIO/D1，PC10/SDIO_D2，PC11/SDIO_D3，PC12/SDIO_CK，PD2/SDIO_CMD*/
	#define SD_DMA_MODE    		0				/*1：DMA模式，0：查询模式 */  

#define SYSTEM_SPI_SD_ENABLE	1				/*启用SPI接口驱动SD卡*/
												/*详见 SPI_SDcard.c 文件*/

/*_____________系统变量和函数（莫要乱动撒）_______________*/
void sys_ENswd_DISjtag(void);
extern u16	StartUpTimes;					/*用于保存开机次数，储存在最后一个或倒数第二个页*/
extern uint32_t UIDw[3]; 					/*保存STM32内部UID识别码，全球唯一识别码*/
extern uint32_t sysCoreClock; 				/*获取HCLK频率，外设时钟均来自此再分频*/

/*蜂鸣器连接在TIM3的通道4上*/
extern u8 is_buzzer_once;
extern u8 is_buzzer_bibi;
#define buzzer_bibi_once	is_buzzer_once = 1;  //用户使用，蜂鸣器叫唤一声
#define buzzer_bibi_on		is_buzzer_bibi = 1;  //用户使用，蜂鸣器间歇叫唤开启
#define buzzer_bibi_off		is_buzzer_bibi = 0;  //用户使用，蜂鸣器间歇叫唤关闭

void delay_us(unsigned int time);
u16 sys_GetsysRunTime(u16* mins,u16* secs,u16* _10ms);/*提供获取系统运行时间的函数，具体看源函数处注释*/

void sys_MCU_Init_Seq(void);				/*MCU外设初始化序列，所有初始化写到这里面*/
void sys_Device_Init_Seq(void);				/*器件外设初始化，并开机自检*/

void FaultASSERT(char* FaultMessage,u8 code,u8 flag);		/*初始化等出现状况，串口提示，指示灯或者蜂鸣器进行提示*/
															/*传入参数：错误或者警告信息，故障代号，错误类别（可选flag_Fault或flag_Warning）*/
#define flag_Fault 		0	/*表动*/
#define flag_Warning 	1

u8 Stm32_Clock_Init(void);					/*时钟系统配置*/


/*_____________核心外设变量和函数（莫要乱动撒）_______________*/
/*_______________________________时基定时器4___________________________________*/
#if STSTEM_TIM4_ENABLE

	__packed struct TIM4_IT_FLAGS
	{
		u8 _10msec_flag;	//10毫秒标志和计数
		u16 _10msec;
		
		u8 _100msec_flag;	//100毫秒标志和计数
		u16 _100msec;
		
		u8 _300msec_flag;	//300毫秒标志
		
		u8 _1sec_flag;		//1秒标志和计数
		u16 _1sec;
		
		u8 _1min_flag;		//1分钟标志和计数
		u16 _1min;
	};
	extern struct TIM4_IT_FLAGS Timer_IT_flags;
	extern TIM_HandleTypeDef TIM4_Handler;      //通用定时器4句柄 
	void sys_TIM4_ENABLE(void);
#endif

/*_______________________________测时定时器5___________________________________*/
#if SYSTEM_UseTIM5ForTiming_ENABLE
	extern u16 TIM5_InterruptTimes;
	extern TIM_HandleTypeDef TIM5_Handler;
	void sys_TIM5_ENABLE(void);
	u32 Timing(void (*FunctionForTiming)());
#endif
/*_______________________________辅助PWM定时器3___________________________________*/
#if STSTEM_TIM3PWM_ENABLE
	extern TIM_HandleTypeDef 	TIM3_Handler;      	//定时器3句柄
	extern TIM_OC_InitTypeDef 	TIM3_CH1Handler,TIM3_CH2Handler,TIM3_CH3Handler,TIM3_CH4Handler;	//定时器3通道1-4句柄
	#define TIM3PWM_Channel_1	1
	#define TIM3PWM_Channel_2	2
	#define TIM3PWM_Channel_3	3
	#define TIM3PWM_Channel_4	4
	void sys_TIM3PWM_ENABLE(void);
	void TIM3_set_Channel_Pulse(u8 channel,float percent);
#endif


/*_______________________________串口们___________________________________*/
#if SYSTEM_UART1_ENABLE||SYSTEM_UART2_ENABLE||SYSTEM_UART3_ENABLE
	
	#define USART1_RX_BUF_MaxNum 	200 	/*定义用协议0只接受以'\r\n'结尾的数据时可以存在字符数组里面的最大字节数目*/
	#define USART1_RX_FIFO_MaxNum	200		/*定义用协议1用FIFO存储数据时可以存的最大字节数目*/
	
	#define UART1	1
	#define UART2	2
	#define UART3	3
	#define RXBUFFERSIZE   1 					//缓存大小
	extern u8 aRxBuffer1[RXBUFFERSIZE],aRxBuffer2[RXBUFFERSIZE],aRxBuffer3[RXBUFFERSIZE];//HAL库使用的串口接收缓冲
	extern UART_HandleTypeDef UART1_Handler,UART2_Handler,UART3_Handler; //UART句柄
	void sys_USART1_ENABLE(void);
	void sys_USART2_ENABLE(void);
	void sys_USART3_ENABLE(void);
	
	
	#define USART1_RX_DONE_mask 0x8000
	#define USART1_RX_MODE_mask 0x4000
	#define USART1_RX_Rec_r_mask 0x2000
	#define USART1_RX_Num_mask 0x1fff /*0001 1111 1111 1111*/
	
	extern u16 USART1_RX_CONFIG;	/*    x            x           x        x xxxx xxxx xxxx      */
									/*(接受完成) (协议模式0/1) (接收到\r)   串口1接受数据字节计数 */
									/*串口1接受协议：0为只接受以'\r\n'结尾的数据，1为以FIFO先进先出实现接受区，无协议*/
	
	#define USART1_SetDone 		USART1_RX_CONFIG |= (USART1_RX_DONE_mask) /*设置串口1接收完成标志位*/
	#define USART1_SetUnDone 	USART1_RX_CONFIG &= (USART1_RX_MODE_mask) /*在任务中处理完后对串口1标志位进行复位，除了mode位，其他位都写0*/
	#define USART1_SetMode(x) 	USART1_RX_CONFIG |= (((u16)x)<<14) 		/*用户可用，在任务中设置串口1接收协议*/
	#define USART1_isDone 		((USART1_RX_CONFIG) & (USART1_RX_DONE_mask))/*用户用，用于判断是否接受完成一次*/
	#define USART1_RX_ByteNum 	((USART1_RX_CONFIG) & (USART1_RX_Num_mask))/*用户可用，返回接收的字符数量*/
	
//	extern QueueHandle_t Uart1_fifo;
	u8 sys_USART1_RX_Fetch(u8 is_print, char *buf,u16 *RX_ByteNum);
	u8 sys_USART2_RX_Fetch(u8 is_print, char *buf,u16 *RX_ByteNum);
	u8 sys_USART3_RX_Fetch(u8 is_print, char *buf,u16 *RX_ByteNum);
	
	
#endif

/*_______________________________CAN1___________________________________*/
#if SYSTEM_CAN1_ENABLE
	u8 CAN1_Receive_Msg(u8* buf,u8* len,u8* frameType,u32* id);
	u8 CAN1_Send_Msg(u8* msg,u8 len,u8 frameType,u32 id);
	void sys_CAN1_Init(void);
	void CAN1_setExceptId(u8 care,u8 frameType,u32 FilterId);
	
	#if CAN1_RX0_INT_ENABLE
		extern CAN_RxHeaderTypeDef	CAN1_IT_RxMessage;	//CAN1从中断接收到数据的结构体
		extern u8 CAN1_IT_Rxdata[8];					//CAN1从中断接收到的数据
		extern u8 CAN1_IT_RxMessage_flag;				//CAN1从中断接收到数据的标志
	#endif
#endif
/*_______________________________MCO___________________________________*/
#if SYSTEM_MCO_PA8_OUT
	void sys_MCO_Out_Enable(void);				/*MCO在PA8上输出启动，默认时钟源HSE*/
#endif
/*_______________________________IWDG___________________________________*/
#if SYSTEM_IWDG_ENABLE
	extern IWDG_HandleTypeDef IWDG_Handler; //独立看门狗句柄
	void sys_IWDG_ENABLE(void);
	void IWDG_Feed(void);
#endif

/*_______________________________CRC___________________________________*/
#if SYSTEM_CRC_ENABLE
	#define BUFFER_SIZE    114
	extern CRC_HandleTypeDef hcrc;
	extern const uint32_t aDataBuffer[BUFFER_SIZE];
	extern uint32_t uwExpectedCRCValue;
	void sys_CRC_ENABLE(void);
	void HAL_CRC_MspInit(CRC_HandleTypeDef* hcrc);
#endif
/*_______________________________HASH_RNG___________________________________*/
#if SYSTEM_RNG_ENABLE
	extern RNG_HandleTypeDef RNG_Handle;
	void sys_RNG_ENABLE(void); 
#endif
/*_______________________________RTC___________________________________*/
#if SYSTEM_RTC_ENABLE
	extern RTC_HandleTypeDef RTC_Handler;  //RTC句柄

	void sys_RTC_Enable(void);						/*RTC配置*/

#endif

/*_______________________________TIM2___________________________________*/
#if STSTEM_TIM2_ENABLE
	extern TIM_HandleTypeDef TIM2_Handler;
	void sys_TIM2_ENABLE(void); //写在预编译外面为了外部文件能够调用
	#if (STSTEM_TIM2_asPWMorCap == 0)
		/*PWM私有变量*/
		#define TIM2PWM_Channel_1	1
		#define TIM2PWM_Channel_2	2
		#define TIM2PWM_Channel_3	3
		#define TIM2PWM_Channel_4	4
		extern TIM_OC_InitTypeDef 	TIM2_CH1Handler,TIM2_CH2Handler,TIM2_CH3Handler,TIM2_CH4Handler;
		void TIM2_set_Channel_Pulse(u8 channel,float percent);
	#elif (STSTEM_TIM2_asPWMorCap == 1)
		/*输入捕获私有变量*/
		extern u8  	TIM2CHx_CAPTURE_STA;	    				
		extern u16	TIM2CHx_CAPTURE_VAL;
		float Peek_TIM2_Cap_Val(void);
		extern TIM_IC_InitTypeDef TIM2_CHxConfig;
		void Process_TIM2_IC_CallBack_Channel_1(void);
		void Process_TIM2_IC_CallBack_Channel_2(void);
		void Process_TIM2_IC_CallBack_Channel_3(void);
		void Process_TIM2_IC_CallBack_Channel_4(void);
	#elif (STSTEM_TIM2_asPWMorCap == 3)
		/*正交解码私有变量*/
		extern TIM_Encoder_InitTypeDef sEncoderConfig;
		extern int32_t EncoderOverflowCount;//定时器溢出次数
		float peek_TIM2_Encoder_Speed(void);
		int32_t peek_TIM2_Encoder_Value(void);
	#endif
#endif

/*_______________________________TIM1PWM___________________________________*/

#if STSTEM_TIM1PWM_ENABLE
		void sys_TIM1PWM_ENABLE(void);
		extern TIM_HandleTypeDef TIM1_Handler;
		#define TIM1PWM_Channel_1	1
		#define TIM1PWM_Channel_2	2
		#define TIM1PWM_Channel_3	3
		#define TIM1PWM_Channel_4	4
		void TIM1_set_Channel_Pulse(u8 channel,float percent);
#endif

/*_______________________________TIM8PWM___________________________________*/

#if STSTEM_TIM8PWM_ENABLE
		extern TIM_HandleTypeDef TIM8_Handler;
		void sys_TIM8PWM_ENABLE(void);
		#define TIM8PWM_Channel_1	1
		#define TIM8PWM_Channel_2	2
		#define TIM8PWM_Channel_3	3
		#define TIM8PWM_Channel_4	4
		void TIM8_set_Channel_Pulse(u8 channel,float percent);
#endif

/*_______________________________TIM6___________________________________*/

#if STSTEM_TIM6_ENABLE
	void sys_TIM6_ENABLE(void);
	extern TIM_HandleTypeDef TIM6_Handler;
#endif
/*_______________________________TIM7___________________________________*/

#if STSTEM_TIM7_ENABLE
	void sys_TIM7_ENABLE(void);
	extern TIM_HandleTypeDef TIM7_Handler;
#endif


/*_______________________________ADC1___________________________________*/

#if SYSTEM_ADC1_ENABLE
	//void ADC_RegularChannelConfig(ADC_HandleTypeDef *AdcHandle, uint32_t Channel, uint32_t Rank, uint32_t SamplingTime);
	//u16 Get_Adc(u32 ch);
	#if SYSTEM_ADC1_useScan
		extern DMA_HandleTypeDef  ADC1rxDMA_Handler;
		void ADC_DMA_Cfg(void);
	#endif
	
	extern ADC_HandleTypeDef ADC1_Handler;
	void sys_ADC1_ENABLE(void);
	u32 Get_Adc_Average(u32 ch,u8 times);
	float Get_Temprate(u32 adcx);
#endif

/*_______________________________SPI1、2________________________________*/

#if (SYSTEM_SPI1_ENABLE) || (SYSTEM_SPI1_ENABLE)
extern SPI_HandleTypeDef SPI1_Handler;  //SPI1句柄
extern SPI_HandleTypeDef SPI2_Handler;  //SPI2句柄
void sys_SPI1_ENABLE(void);
void sys_SPI2_ENABLE(void);
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler);

//写读8字节
u8 SPI1_ReadWriteByte_8Byte(u8 TxData);
//写读16字节，传入一个包含2字节的u8*指针
u8 SPI1_ReadWriteByte_16Byte(u8* TxData);
//写8字节
u8 SPI1_WriteByte_8Byte(u8 TxData);
//写16字节，传入一个包含2字节的u8*指针
u8 SPI1_WriteByte_16Byte(u8* TxData);

u8 SPI2_ReadWriteByte(u8 TxData);

void sys_SPI1_SS_io_Init(void);
void sys_SPI2_SS_io_Init(void);

#endif
/*_________________________________低功耗StandbyMode________________________________*/
#if SYSTEM_StdbyWKUP_ENABLE
	void sys_StdbyWKUP_ENABLE(void);
	void sys_CheckWKUP_4RTOS(void);
	#define WKUP_KD HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)  //PA0 检测是否外部WK_UP按键按下
	u8 Check_WKUP(void);  			//检测WKUP脚的信号
	void Sys_Enter_Standby(void);	//系统进入待机模式
#endif

/*____________________________FLASH编程___________________________________*/

#if SYSTEM_FLASH_IAP_ENABLE
	
	void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead);   		//从指定地址开始读出指定长度的数据
	#define getWordSize(x)		sizeof(x)/4 + ((sizeof(x) % 4) ? 1 : 0)
	
	#if STM32_FLASH_WREN	//如果使能了写   
		#define FLASH_WAITETIME  50000          //FLASH等待超时时间

		//FLASH 扇区的起始地址
		#define ADDR_FLASH_SECTOR_0     ((u32)0x08000000) 	//扇区0起始地址, 16 Kbytes  
		#define ADDR_FLASH_SECTOR_1     ((u32)0x08004000) 	//扇区1起始地址, 16 Kbytes  
		#define ADDR_FLASH_SECTOR_2     ((u32)0x08008000) 	//扇区2起始地址, 16 Kbytes  
		#define ADDR_FLASH_SECTOR_3     ((u32)0x0800C000) 	//扇区3起始地址, 16 Kbytes  
		#define ADDR_FLASH_SECTOR_4     ((u32)0x08010000) 	//扇区4起始地址, 64 Kbytes  
		#define ADDR_FLASH_SECTOR_5     ((u32)0x08020000) 	//扇区5起始地址, 128 Kbytes  
		#define ADDR_FLASH_SECTOR_6     ((u32)0x08040000) 	//扇区6起始地址, 128 Kbytes  
		#define ADDR_FLASH_SECTOR_7     ((u32)0x08060000) 	//扇区7起始地址, 128 Kbytes  
		#define ADDR_FLASH_SECTOR_8     ((u32)0x08080000) 	//扇区8起始地址, 128 Kbytes  
		#define ADDR_FLASH_SECTOR_9     ((u32)0x080A0000) 	//扇区9起始地址, 128 Kbytes  
		#define ADDR_FLASH_SECTOR_10    ((u32)0x080C0000) 	//扇区10起始地址,128 Kbytes  
		#define ADDR_FLASH_SECTOR_11    ((u32)0x080E0000) 	//扇区11起始地址,128 Kbytes 
	 
		void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite);		//从指定地址开始写入指定长度的数据
	#endif
	
#endif

/*_____________________________________DAC________________________________________*/

#if ((SYSTEM_DAC_OUT1_ENABLE) || (SYSTEM_DAC_OUT2_ENABLE))
	extern DAC_HandleTypeDef DAC1_Handler;//DAC句柄
	void sys_DAC_ENABLE(void);
	//void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac);
	void DAC_Set_Ch1_Vol(float vol);
	void DAC_Set_Ch2_Vol(float vol);
#endif



/*____________________SDIO SD_____________________________________________*/

#if SYSTEM_SDIO_SD_ENABLE
	#define SD_TIMEOUT 			((uint32_t)100000000)  	//超时时间
	#define SD_TRANSFER_OK     	((uint8_t)0x00)
	#define SD_TRANSFER_BUSY   	((uint8_t)0x01)

	extern SD_HandleTypeDef        SDCARD_Handler;     	//SD卡句柄
	extern HAL_SD_CardInfoTypeDef  SDCardInfo;         	//SD卡信息结构体

	u8 SD_Init(void);
	u8 SD_GetCardInfo(HAL_SD_CardInfoTypeDef *cardinfo);
	u8 SD_GetCardState(void);
	u8 SD_ReadDisk(u8* buf,u32 sector,u32 cnt);
	u8 SD_WriteDisk(u8 *buf,u32 sector,u32 cnt);
	u8 SD_ReadBlocks_DMA(uint32_t *buf,uint64_t sector,uint32_t blocksize,uint32_t cnt);
	u8 SD_WriteBlocks_DMA(uint32_t *buf,uint64_t sector,uint32_t blocksize,uint32_t cnt);
#endif

/*____________________________FSMC for SARM_____________________________________________*/
#if ((SYSTEM_FSMC_ENABLE) && (SYSTEM_FSMC_use4SRAM))

	extern SRAM_HandleTypeDef SRAM_Handler;    //SRAM句柄
	//使用NOR/SRAM的 Bank1.sector3,地址位HADDR[27,26]=10 
	//对IS61LV25616/IS62WV25616,地址线范围为A0~A17 
	//对IS61LV51216/IS62WV51216,地址线范围为A0~A18
	#define SRAM1_BANK3_ADDR    ((u32)(0x68000000))	//计算： (0x6000 0000 | ((u32)64*1024*1024*(3 - 1)))
	
	void sys_FSMC_SRAM_ENABLE(void);
	void FSMC_SRAM_WriteBuffer(u8 *pBuffer,u32 WriteAddr,u32 n);
	void FSMC_SRAM_ReadBuffer(u8 *pBuffer,u32 ReadAddr,u32 n);
	
#endif


/*____________________________FSMC for LCD_____________________________________________*/
#if ((SYSTEM_FSMC_ENABLE) && (SYSTEM_FSMC_use4LCD))
	extern SRAM_HandleTypeDef TFTSRAM_Handler;    //SRAM句柄(用于控制LCD)

	void LCD_with_FSMC_init_FSMC(void);
	//LCD地址结构体
	typedef struct
	{
		vu16 LCD_REG;
		vu16 LCD_RAM;
	} LCD_TypeDef;
	//使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A10作为数据命令区分线 
	//注意设置时STM32内部会右移一位对其! 			    
	#define LCD_BASE        ((u32)(0x6C000000 | 0x000007FE))
	#define LCD             ((LCD_TypeDef *) LCD_BASE)
#endif


#define Version_of_stm32_framework "2.5"

/*Author:		MaxwellXyao*/
#define BIT(n) 					(1 << n) 				//位mask
#define bit_SetTrue(x,mask) 	(x |= mask)				//该位设置为真
#define bit_SetFalse(x,mask)	(x &= ~mask)			//该位设置为假
#define bit_Toggle(x,mask) 		(x ^= mask)				//位切换
#define bit_IsTrue(x,mask) 		((x & mask) != 0)		//该位是否真
#define bit_IsFalse(x,mask) 	((x & mask) == 0)		//该位是否假

//以下为汇编函数
void WFI_SET(void);				//执行WFI指令
void INTX_DISABLE(void);		//关闭所有中断
void INTX_ENABLE(void);			//开启所有中断
void MSR_MSP(uint32_t addr);	//设置堆栈地址

#endif




