#ifndef __PERIPHCONFIGCORE_H
#define __PERIPHCONFIGCORE_H
/*
	STM32F207IET6：
		512KB(hd)FLASH，128KB（SRAM1）+112KB+16KB（SRAM2）RAM，176脚LQFP封装，140个GPIO，-40~85℃
	
	时钟分配：
		HSE外接25Mhz晶振，LSE外接32.768Khz晶振
		PLLCLK = SYSCLK = AHBCLK = HCLK 均为120MHz，APB2外设为60MHz，APB1外设为30Mhz，APB2定时器为120MHz，APB1定时器为60Mhz
		
		ETH\RNG\FSMC\USB FS\USB HS\GPIO\DMA 均在120M
		RTC 时钟取自 LSE = 32.768K
		IWDG时钟取自 HSE = 25M
		DCMI 48 Mbyte/s max
		
		APB2外设(60M)：SDIO\USART1\USART6\SPI1\ADC123
		APB2定时器(120M)：TIM1 TIM8 TIM9 TIM10 TIM11
		APB1外设(30M)：USART2\USART3\UART4\UART5\SPI2、I2S2\SPI3、I2S3\I2C123\CAN12\DAC12
		APB1定时器(60M)：TIM2 TIM3 TIM4 TIM5 TIM12 TIM13 TIM14
	
		定时器不管16或者32位的，都按16位来用，也很够了
	本框架TIM分配：
		TIM4	10ms时基，用户不可写中断程序
		TIM3	产生辅助PWM，可中断提供辅助时基，通道按照其他外设使用情况灵活分配
		TIM2	多功能使用，可中断提供辅助时基，通道可用于PWM、正交编码解码、输入捕获
		TIM5	同上
		TIM1\8	主PWM，产生3对互补、带死区、带刹车和电平可调的PWM
		TIM6\7	官方推荐用于触发DAC（也可产生三角波）或者产生额外时基
	
*/

/*
启动：	BOOT1	BOOT0		启动区域
		  x		  0			FLASH
		  0		  1			系统储存器(ISP下载)
		  1		  1			SRAM
*/

/*所有main的头文件都放在这里*/
#include "sysVar.h"								/*定义系统级常用的变量、数据类型和二进制表示宏*/
#include "stm32f2xx.h"
#include "stm32f2xx_hal.h"						/*在里面的stm32f2xx_hal_conf.h里面选择用哪些外设的HAL库——————！按需要进行修改！*/

//#include "malloc.h"								/*借鉴的 内存管理 章节的源代码自实现的malloc和free*/	
#include "sys_menu.h"							/*模板固定搭配！必须开启！提供一个菜单模板，把系统的输入、输出、执行功能的标志位控制全部打包！相当于io_ctrl*/
#include "MyString.h"							
												/*模板固定搭配！必须开启！提供一个实现了string.h大部分字符操作函数的库
													具体API看MyString.c文件里的注释
													int mystrlen (const char *str);
													char * mystrcat (char *dest, const char *src);
													char * mystrncat (char *dest, const char *src, int n);
													char * mystrcpy (char *dest, const char *src);
													char * mystrncpy (char *dest, const char *src, int n);
													int mystrcmp (const char *s1, const char *s2);
													int mystrncmp (const char *s1, const char *s2, int n);
													void * mymemset (void *s, int c, unsigned n);
													void *mymemcpy(void *des,const void *src,size_t len);
													char *mystrtok(char *s, const char *delim);*/
int myatoi(const char *str);					/*提供一个字符串转整形的实现*/
#include "printf.h"								
												/*模板固定搭配！必须开启！包含且编译printf.h，github开源文件，无依赖，功能比较全。
													约占6KB，对于stm32够，对于其他小容量MCU则看“其他几个sprintf实现”文件夹里面的,不要纠结了。
													开源仓库地址：https://github.com/mpaland/printf
													提供API：
													int printf(const char* format, ...); 								不用
													int printf_uart(unsigned char uart,const char* format, ...); 		用，可选发送串口
																		串口选择可选：UART1/UART2/UART3，需要先使能相关串口
													int sprintf(char* buffer, const char* format, ...); 				不带字节数量限制
													int snprintf(char* buffer, size_t count, const char* format, ...); 	带字节数量限制，更安全
												注意：另在Middlewares文件夹内提供strprintf.h，功能比较单一，但很小。*/
#define STSTEM_TIM4_ENABLE		1				/*模板固定搭配！必须开启！使用通用定时器4，提供时基，默认10ms周期中断*/
#define STSTEM_TIM3PWM_ENABLE	1				/*模板基本功能，必须开启！使用通用定时器3，默认不开启中断，提供四路PWM输出*/
												/*默认通道4给蜂鸣器，蜂鸣器低电平有效！蜂鸣器控制需要用到TIM4！
												改变占空比可调响度，改变频率可调声调*/
												/*默认向上计数，输出极性为LOW，PWM1模式，即设置为当计数值小于此值时输出低电平。*/
												/*默认：引脚上拉							(CH1/PA6, CH2/PA7, CH3/PB0, CH4/PB1)*/
		#define STSTEM_TIM3PWM_REMAP_PARTIAL	0	/*定时器3部分引脚重映射，Partial remap  (CH1/PB4, CH2/PB5, CH3/PB0, CH4/PB1)*/
		#define STSTEM_TIM3PWM_REMAP_ENABLE		0	/*定时器3全部引脚重映射，Full remap     (CH1/PC6, CH2/PC7, CH3/PC8, CH4/PC9)*/
		#define STSTEM_TIM3PWM_CHANNEL_ENABLE	B0000_1000 /*输出通道选择，共四个通道*/
													/*可选  B0000_0001|B0000_0010|B0000_0100|B0000_1000	*/
		#define TIM3PWM_BuzzerCh				TIM3PWM_Channel_4	//选择用于蜂鸣器的PWM通道
		#define STSTEM_TIM3PWM_TI_ENABLE		0	/*是否开启定时器3的定时中断，除非急需用，否则一般不开*/
		
		#define tim3prsc STSTEM_TIM3PWM_Period_5K		/*选择定时器输出频率，以下六条语句基本不动（重装值为720，这里是选择预分频系数）*/
			#define STSTEM_TIM3PWM_Period_2K	(30-1)
			#define STSTEM_TIM3PWM_Period_5K	(12-1)
			#define STSTEM_TIM3PWM_Period_10K	(6-1)
			#define STSTEM_TIM3PWM_Period_20K	(3-1)
			#define STSTEM_TIM3PWM_Period_60K	(1-1)
			#define tim3arr 1000-1
		/*可用API：
					HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_2);		开启TIM3的PWM通道2
					HAL_TIM_PWM_Stop(&TIM3_Handler,TIM_CHANNEL_2);		关闭TIM3的PWM通道2，但看源码好像使所有通道都关闭了，待实验
					
					设置TIM3的PWM通道2的占空比百分数为88.8%，值需在0~100.0之间。即88.8%的低电平时间（本模板规范：低电平有效，即用电器工作电压）
					TIM3_set_Channel_Pulse(TIM3PWM_Channel_2,88.8);
					
					buzzer_bibi_once;			用户使用，蜂鸣器叫唤一声
					define buzzer_bibi_on;		用户使用，蜂鸣器间歇叫唤开启
					define buzzer_bibi_off;		用户使用，蜂鸣器间歇叫唤关闭
		*/


/*_____________________________________\\\                               ///____________________________________________*
 *___________________________________________________组件选择___________________________________________________________*
 *_____________________________________///                               \\\____________________________________________*/
#define SYSTEM_SUPPORT_pid		1				/*提供一个pid算法实现库，集成了积分分离和变限积分，以及可选的不完全微分和微分先行，具体用法看pid.h里面*/
												/*里面另提供一个一阶低通滤波实现函数*/
												/*一阶低通滤波器 float FirstOrderLPF(float NewValue);*/												
	#if SYSTEM_SUPPORT_pid						/*如果不包含，其他文件就无法调用此组件的API*/
		#include "pid.h"
	#endif


/*_____________________________________\\\                               ///____________________________________________*
 *_________________________________________________用户GPIO配置_________________________________________________________*
 *_____________________________________///                               \\\____________________________________________*/












/*_____________________________________\\\                               ///____________________________________________*
 *___________________________________________________外设配置___________________________________________________________*
 *_____________________________________///                               \\\____________________________________________*/


/*
 *FreeRTOS启用
 */
#define SYSTEM_SUPPORT_OS		0				/*定义是否使用FreeRTOS，不是0就是1——————！按需要进行修改！
													FreeRTOS版本：v10.3.1
													默认用于任务的RAM堆栈大小为5KB，按需修改！
												注意：还要在FreeRTOSConfig.h里面的SYSTEM_SUPPORT_OS宏定义与此处保持一致，否则系统不会调用FreeRTOS的任务切换SVC中断函数，跑不起来！*/
	#if SYSTEM_SUPPORT_OS
		 #include "FreeRTOS.h"	//FreeRTOS使用,这里不要乱动	  
		 #include "task.h"
		 #include "queue.h"
		#include "TaskConfig.h"
	#else
		#include "BareConfig.h"	
	#endif

/*
 *串口配置
 */
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
/*缓存大小去*/
/*可用的API： 目前串口接收的问题就是一次涌入数据过多会崩，可能是存储数据时栈溢出，但是暂时找不到在哪里，小数据量接收没大问题
	发送：	注：不要用printf！
				参数：UART1~UART3
				printf_uart(UART1,"Fault : %d\t",x);
				也可以用	sprintf(char* buffer, const char* format, ...); 				不带字节数量限制
							snprintf(char* buffer, size_t count, const char* format, ...); 	带字节数量限制，更安全
			如果没有启用 SYSTEM_SUPPORT_sprintf ，则可用以下：
			参数：UART1_Handler~UART3_Handler ， 发送数据的字符类型指针 ， 数据长度(字节数} ， 超时时间
				HAL_UART_Transmit(&UART1_Handler,UART_BUF,12,10); 				发送函数
				while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET); 	等待发送结束
	接收：（以下只针对串口1说明）（一次接收字符不要超过200个字节，如果超出则丢弃200之前的数据）
			接受协议设置(默认协议0)：USART1_SetMode(0);	串口1接受协议：0为只接受以'\r\n'结尾的数据（数据以去掉\r\n保存），1协议暂时保留，等以后加上可以解析别的协议如modbus等等
				接受协议设置可用随时切换，切换后接收方式也随下面的说明换。
			统一接收函数： 返回HAL_OK即有数据可以取，反之为HAL_ERROR；is_print：是否打印接收到的数据；buf用于存放接收到的数据，必须先给个足够大的容器
				char sys_USART1_RX_Fetch(u8 is_print, char* buf)
				用例：
					u16 num4uart1;
					char* buf4uart1 = mymalloc(InrRAM,(USART1_RX_BUF_MaxNum > USART1_RX_FIFO_MaxNum ? USART1_RX_BUF_MaxNum : USART1_RX_FIFO_MaxNum));
					if(buf4uart1 != NULL)
					{
						if(sys_USART1_RX_Fetch(FALSE, buf4uart1,&num4uart1) == HAL_OK)
						{
							printf_uart(UART1,"%s-%d",buf4uart1,num4uart1);
						}
					}
					myfree(InrRAM,buf4uart1);
			关于两个协议0、1的接收细节，已经在sys_USART1_RX_Fetch()内实现，一般不用看
*/



/*_____________系统变量和函数（莫要乱动撒）_______________*/
extern u16	StartUpTimes;					/*用于保存开机次数，储存在最后一个或倒数第二个页*/
extern uint32_t UIDw[3]; 					/*保存STM32内部UID识别码，全球唯一识别码*/
extern uint32_t sysCoreClock; 				/*获取HCLK频率，外设时钟均来自此再分频*/

/*蜂鸣器连接在TIM3的通道4上*/
extern u8 is_buzzer_once;
extern u8 is_buzzer_bibi;
#define buzzer_bibi_once	is_buzzer_once = 1;  //用户使用，蜂鸣器叫唤一声
#define buzzer_bibi_on		is_buzzer_bibi = 1;  //用户使用，蜂鸣器间歇叫唤开启
#define buzzer_bibi_off		is_buzzer_bibi = 0;  //用户使用，蜂鸣器间歇叫唤关闭

u16 sys_GetsysRunTime(u16* mins,u16* secs,u16* _10ms);/*提供获取系统运行时间的函数，具体看源函数处注释*/

void sys_MCU_Init_Seq(void);				/*MCU外设初始化序列，所有初始化写到这里面*/
void sys_Device_Init_Seq(void);				/*器件外设初始化，并开机自检*/

void FaultASSERT(char* FaultMessage);		/*表示初始化有问题，串口提示，指示灯或者蜂鸣器进行提示*/
u8 Stm32_Clock_Init(void);					/*时钟系统配置*/


/*_____________核心外设变量和函数（莫要乱动撒）_______________*/
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




