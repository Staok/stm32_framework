#include "PeriphConfigCore.h"



/*_____________________________________\\\                               ///____________________________________________*
 *_________________________________________________外设初始化函数_______________________________________________________*
 *_____________________________________///                               \\\____________________________________________*/

/*__________MCU外设初始化序列_____________*/
void sys_MCU_Init_Seq(void)
{
	/*HSE外接25Mhz晶振，LSE外接32.768Khz晶振*/
	/*	PLLCLK = SYSCLK = AHBCLK = HCLK 均为120MHz，APB2外设为60MHz，APB1外设为30Mhz，APB2定时器为120MHz，APB1定时器为60Mhz
		
		ETH\RNG\FSMC\USB FS\USB HS\GPIO\DMA 均在120M
		RTC 时钟取自 LSE = 32.768K
		IWDG时钟取自 HSE = 25M
		DCMI 48 Mbyte/s max
		
		APB2外设(60M)：SDIO\USART1\USART6\SPI1\ADC123
		APB2定时器(120M)：TIM1 TIM8 TIM9 TIM10 TIM11
		APB1外设(30M)：USART2\USART3\UART4\UART5\SPI2、I2S2\SPI3、I2S3\I2C123\CAN12\DAC12
		APB1定时器(60M)：TIM2 TIM3 TIM4 TIM5 TIM12 TIM13 TIM14
	
		定时器不管16或者32位的，都按16位来用，也够用了
	*/
	Stm32_Clock_Init();
	
	/*关闭JTAG，启用SWD*/
	/*TODO：用本框架的GPIO初始化这里的SWD IO，先试验试验如果不初始化有没有什么问题*/
	GPIO_InitTypeDef GPIO_Initure;
	GPIO_Initure.Pin = GPIO_PIN_13;
	GPIO_Initure.Mode = GPIO_MODE_AF_PP;
	GPIO_Initure.Pull=GPIO_NOPULL;
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_Initure.Alternate = GPIO_AF0_SWJ;
	HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
	GPIO_Initure.Pin = GPIO_PIN_14;
	HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
	
	// /*设置MCO*/
	// #if SYSTEM_MCO_PA8_OUT
		// sys_MCO_Out_Enable();
	// #endif

	// /*按照设定初始化串口1、2、3*/
	// #if SYSTEM_UART1_ENABLE
		// sys_USART1_ENABLE();
		
		// /*串口1接受协议：0为只接受以'\r\n'结尾的数据，1为以FIFO先进先出的环形缓存实现接受区，无协议*/
		// USART1_SetMode(0);
	// #endif
	// #if SYSTEM_UART2_ENABLE
		// sys_USART2_ENABLE();
	// #endif
	// #if SYSTEM_UART3_ENABLE
		// sys_USART3_ENABLE();
	// #endif
	
	// /*设置RTC*/
	// #if SYSTEM_RTC_ENABLE
		// sys_RTC_Enable();
		// //用户自行更改设置RTC和闹钟的日期和时间，在	sys_RTC_Enable() 里面		
	// #endif
	// /*设置CRC*/
	// #if SYSTEM_CRC_ENABLE
		// sys_CRC_ENABLE();
		// if(HAL_CRC_Accumulate(&hcrc, (uint32_t *)aDataBuffer, BUFFER_SIZE) == uwExpectedCRCValue)
		// {}else{FaultASSERT("AT : CRC init");}
	// #endif
	
	// printf_uart(UART1,"Author : Staok\r\nEmail : superxhy@qq.com\r\nRepo : https://github.com/Staok/stm32_framework\r\n");
	// printf_uart(UART1,"Compile time : %s,%s\r\n",__DATE__,__TIME__);
	// printf_uart(UART1,"Version : %s\r\n",Version_of_stm32_framework);
	// printf_uart(UART1,"OK,then...System is starting...\r\n");
		
	
	// /*获取HCLK频率并打印到串口1，外设时钟均来自此再分频*/
	// sysCoreClock = HAL_RCC_GetHCLKFreq(); 
	// printf_uart(UART1,"sysCoreClock/HCLK : %d\r\n",sysCoreClock);
	// /*保存STM32内部UID识别码并打印，全球唯一识别码*/
	// UIDw[0] = HAL_GetUIDw0();UIDw[1] = HAL_GetUIDw1();UIDw[2] = HAL_GetUIDw2(); 
	// printf_uart(UART1,"UID : %d %d %d\r\n",UIDw[0],UIDw[1],UIDw[2]);
	
	// #if SYSTEM_FLASH_IAP_ENABLE
		// //获取开机次数
		// STMFLASH_Read( 	(0X08000000 + (u32)((STM32_FLASH_SIZE-2)*1024)),	&StartUpTimes,	sizeof(StartUpTimes));
		// StartUpTimes += 1;
		// STMFLASH_Write( (0X08000000 + (u32)((STM32_FLASH_SIZE-2)*1024)),	&StartUpTimes,	sizeof(StartUpTimes));
		// printf_uart(UART1,"StartUpTimes : %d\r\n",StartUpTimes);
	// #endif
	
	// #if SYSTEM_ADC1_ENABLE
		// /*说明ADC1的用途*/
		// sys_ADC1_ENABLE();
	// #endif
	
	// #if SYSTEM_SPI1_ENABLE
		// /*说明SPI1的用途*/
		// sys_SPI1_ENABLE();
	// #endif
	
	// #if SYSTEM_SPI2_ENABLE
		// /*说明SPI2的用途*/
		// sys_SPI2_ENABLE();
	// #endif
	
	// #if SYSTEM_StdbyWKUP_ENABLE
		// /*使能待机-低功耗模式，默认长按PA0(WKUP)3秒关开机*/
		// sys_StdbyWKUP_ENABLE();
	// #endif
	
	// #if ((SYSTEM_DAC_OUT1_ENABLE) || (SYSTEM_DAC_OUT2_ENABLE)) && ((STM32F103xG) || (STM32F103xE))
		// sys_DAC_ENABLE();
	// #endif
	
	// my_mem_init(InrRAM);
	
	// #if ((SYSTEM_FSMC_ENABLE) && (SYSTEM_FSMC_use4SRAM)) && ((STM32F103xG) || (STM32F103xE))
		// sys_FSMC_SRAM_ENABLE();
		// my_mem_init(ExRAM1);
	// #endif
	
}

/*__________器件外设初始化，并开机自检_____________*/
void sys_Device_Init_Seq(void)
{
	/*用户应用的Device初始化序列——开始*/
	
	/*用户IO初始化，可选择初始化某个特定器件或者所有器件（ALL_Index）*/
	Devices_Init(UserDevices,TestLED_Index);
	Devices_Init(UserDevices,KEY_Index);
	Devices_Init(UserDevices,LCD_Index); //使用LCD设备的初始化函数初始化IO
	
	// /*LCD初始化*/
	// #if ((SYSTEM_FSMC_ENABLE) && (SYSTEM_FSMC_use4LCD)) && ((STM32F103xG) || (STM32F103xE))
		// LCD_with_FSMC_init_FSMC();
		// LCD_with_FSMC_init_LCD();
	// #else
		// LCD_Init_no_FSMC();
	// #endif
	
	// /*用户应用的Device初始化序列——结束*/
	
	// /*__________启动模板的心跳，开启MCU的精彩一生__________*/
	/*初始化并启动TIM4*/
	#if STSTEM_TIM4_ENABLE
		sys_TIM4_ENABLE();
	#endif
	
	/*初始化并启动TIM3PWM通道*/
	#if STSTEM_TIM3PWM_ENABLE
		sys_TIM3PWM_ENABLE();
	#endif
	
	// /*初始化高级定时器1*/
	// #if STSTEM_TIM1PWM_ENABLE
		// sys_TIM1PWM_ENABLE();
	// #endif
	
	// /*初始化高级定时器8*/
	// #if (STSTEM_TIM8PWM_ENABLE) && ((STM32F103xG) || (STM32F103xE))
		// sys_TIM8PWM_ENABLE();
	// #endif
	
	// /*初始化时基定时器6*/
	// #if (STSTEM_TIM6_ENABLE) && ((STM32F103xG) || (STM32F103xE))
		// sys_TIM6_ENABLE();
	// #endif
	
	// /*初始化时基定时器7*/
	// #if (STSTEM_TIM7_ENABLE) && ((STM32F103xG) || (STM32F103xE))
		// sys_TIM7_ENABLE();
	// #endif
	
	// /*初始化多功能定时器2*/
	// #if STSTEM_TIM2_ENABLE
	// /*说明TIM2的用途*/
	// sys_TIM2_ENABLE();
	// #endif
	
	// /*初始化看门狗*/
	// #if SYSTEM_IWDG_ENABLE
		// sys_IWDG_ENABLE();
	// #endif
	// buzzer_bibi_once; //响一声表示初始化结束
	// printf_uart(UART1,"System init over\r\n");
}


/*____________运行错误提示和打印______________________________*/
/********************************
*描述：表示某步骤运行有问题，串口提示，灯提示，声提示
*参数：		FaultMessage:错误提示信息字符串
*返回值：	1、NULL
********************************/
void FaultASSERT(char* FaultMessage)
{
	/*往串口1发送数据*/
//	printf_uart(UART1,"Fault Message : %s\n",FaultMessage);
//	printf_uart(UART1,"File&Line : %s,%s\n",__FILE__,__LINE__);
	//灯提示，声提示
	buzzer_bibi_on;
	while(1){;}
}

/*_____________获取系统运行的时间________________________*/
/********************************
*描述：获取系统运行的时间
*参数：		mins、secs、_10ms:系统运行时间的分钟数，在此基础上多出的秒数，和在此基础上多出的10毫秒数
*返回值：	1、系统运行的总秒数
********************************/
u16 sys_GetsysRunTime(u16* mins,u16* secs,u16* _10ms)
{
	if(mins != NULL)
		*mins = Timer_IT_flags._1min;
	if(secs != NULL)
		*secs = Timer_IT_flags._1sec;
	if(_10ms != NULL)
		*_10ms = Timer_IT_flags._10msec;
	return ((Timer_IT_flags._1min)*60 + Timer_IT_flags._1sec);
}

/*__________时钟系统配置函数_____________*/
/********************************
*描述：上电时必要进行的时钟配置，默认使用外部高速信号源，外部晶振5M，主频120M
*参数：		1、NULL
*返回值：	1、HAL_OK,成功
			2、HAL_ERROR,失败
********************************/
u8 Stm32_Clock_Init(void)
{
	/*HSE外接25Mhz晶振，LSE外接32.768Khz晶振*/
	/*	PLLCLK = SYSCLK = AHBCLK = HCLK 均为120MHz，APB2外设为60MHz，APB1外设为30Mhz，APB2定时器为120MHz，APB1定时器为60Mhz
		
		ETH\RNG\FSMC\DCMI\USB FS\USB HS\GPIO\DMA 均在120M
		RTC 时钟取自 LSE = 32.768K
		IWDG时钟取自 HSE = 25M
		
		APB2外设(60M)：SDIO\USART1\USART6\SPI1\ADC123
		APB2定时器(120M)：TIM1 TIM8 TIM9 TIM10 TIM11
		APB1外设(30M)：USART2\USART3\UART4\UART5\SPI2、I2S2\SPI3、I2S3\I2C123\CAN12\DAC12
		APB1定时器(60M)：TIM2 TIM3 TIM4 TIM5 TIM12 TIM13 TIM14(无TIM67)		
	*/
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

	/** Initializes the RCC Oscillators according to the specified parameters
	* in the RCC_OscInitTypeDef structure.
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE
							  |RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
	RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;	//25M
	RCC_OscInitStruct.PLL.PLLM = 20;
	RCC_OscInitStruct.PLL.PLLN = 192;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;				//PLLP 120M
	RCC_OscInitStruct.PLL.PLLQ = 5;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		return HAL_ERROR;
	}
	/** Initializes the CPU, AHB and APB buses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
							  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;	//SYSCLK 120M
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;			//AHBCLK 120M
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;			//APB1CLK 30M
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;			//APB2CLK 60M

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
	{
		return HAL_ERROR;
	}
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
		return HAL_ERROR;
	}
	
	#if SYSTEM_SUPPORT_OS
		//这里为了兼容FreeRTOS
		u32 reload;
		HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//SysTick频率为HCLK
		reload=SystemCoreClock/1000000;				//每秒钟的计数次数 单位为M  
		reload*=1000000/configTICK_RATE_HZ;			//根据configTICK_RATE_HZ设定溢出时间   
		
		SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
		SysTick->LOAD=reload; 						//每1/configTICK_RATE_HZ秒中断一次	
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//开启SYSTICK
		
		HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);	
		HAL_NVIC_EnableIRQ(SysTick_IRQn);
	#else
		HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//SysTick频率为HCLK
		HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
	#endif
	
	return HAL_OK;
}

/*__________定时器中断回调函数_____________*/
/********************************
*描述：回调函数，定时器中断服务函数调用，不要写入太多程序！
*参数：		1、NULL
*返回值：	1、NULL
********************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static u8 is_reversal = 0;
	static u8 is_runOnce4bibi = 0;	//运行了一次置一，一次也没运行时置零
	static u8 is_runOnce4biOnce = 0;
	
	/*注意：不要写入太多程序从而占用时间！*/
    if(htim==(&TIM4_Handler))
    {
		Timer_IT_flags._10msec_flag = TRUE;
		
		/*如果启用定时器2的正交解码功能，这里定时10ms周期计算正交计数值并计算返回速度 单位 转/秒*/
		#if (STSTEM_TIM2_ENABLE)&&(STSTEM_TIM2_asPWMorCap == 3)
			//如果接着速度的变量是x，下句应写成 x = peek_TIM2_Encoder_Speed();
			peek_TIM2_Encoder_Speed();
		#endif
		
		if(++Timer_IT_flags._10msec >= 10)
		{
			Timer_IT_flags._10msec = 0;
			Timer_IT_flags._100msec_flag = TRUE;
			Timer_IT_flags._100msec++;
			
			#if SYSTEM_IWDG_ENABLE
				IWDG_Feed();//1s周期的看门狗，来喂狗了
			#endif
			
		}
		
		if(Timer_IT_flags._100msec % 3 == 0)
		{
			Timer_IT_flags._300msec_flag = TRUE;
			
			if(is_buzzer_once)
			{
				TIM3_set_Channel_Pulse(TIM3PWM_BuzzerCh,80.0); //打开蜂鸣器
				is_buzzer_once = FALSE;
				is_runOnce4biOnce = TRUE;
			}else{
				if(is_runOnce4biOnce)
				{
					TIM3_set_Channel_Pulse(TIM3PWM_BuzzerCh,0); //关闭蜂鸣器
					is_runOnce4biOnce = FALSE;
				}
			}
			
		}
		
		if(Timer_IT_flags._100msec >= 10)
		{
			Timer_IT_flags._100msec = 0;
			Timer_IT_flags._1sec_flag = TRUE;
			Timer_IT_flags._1sec++;
			
			if(is_buzzer_bibi)
			{
				if(is_reversal) TIM3_set_Channel_Pulse(TIM3PWM_BuzzerCh,80.0); //打开蜂鸣器
				else TIM3_set_Channel_Pulse(TIM3PWM_BuzzerCh,0); //关闭蜂鸣器
				is_runOnce4bibi = TRUE;
				is_reversal = ~is_reversal;
			}else{
				if(is_runOnce4bibi)
				{
					TIM3_set_Channel_Pulse(TIM3PWM_BuzzerCh,0);//关闭蜂鸣器
					is_runOnce4bibi = FALSE;
				}
				
			}
		}
		
		if(Timer_IT_flags._1sec >= 60)
		{
            Timer_IT_flags._1sec = 0;
			Timer_IT_flags._1min_flag = TRUE;
			Timer_IT_flags._1min++;
			
			
			if((Timer_IT_flags._1min > 666)){
				Timer_IT_flags._1min = 0;
				
				//超过时间，可以设定强制关闭
				//_OutofTime_Running_flag = 1;
				//CTRL_DCDC_ON_OFF = DCDC_OFF;
			}
		}
    }
	
	/*定时器3周期回调*/
	if(htim==(&TIM3_Handler))
    {
        //LED1=!LED1;
    }
	
	#if STSTEM_TIM6_ENABLE
		/*定时器6周期回调*/
		if(htim==(&TIM6_Handler))
		{
			
		}
	#endif
	
	#if STSTEM_TIM7_ENABLE
		/*定时器7周期回调*/
		if(htim==(&TIM7_Handler))
		{
			
		}
	#endif
		
	#if STSTEM_TIM2_ENABLE
	/*定时器2周期回调*/
	if(htim==(&TIM2_Handler))
    {
		#if (STSTEM_TIM2_asPWMorCap == 1)					//使用输入捕获功能，定时器溢出一次则溢出次数加一
			if((TIM2CHx_CAPTURE_STA&0X80)==0)				//还未成功捕获
			{
				if(TIM2CHx_CAPTURE_STA&0X40)				//已经捕获到高电平了
				{
					if((TIM2CHx_CAPTURE_STA&0X3F)==0X3F)	//高电平太长了
					{
						TIM2CHx_CAPTURE_STA|=0X80;			//标记成功捕获了一次
						TIM2CHx_CAPTURE_VAL=0XFFFF;
					}else TIM2CHx_CAPTURE_STA++;
				}	 
			}
		#endif
		
		#if (STSTEM_TIM2_asPWMorCap == 3)					//使用正交解码功能，计数溢出次数记录
			if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&TIM2_Handler))
			EncoderOverflowCount--;       //向下计数溢出
			else
			EncoderOverflowCount++;       //向上计数溢出
		#endif
    }
	#endif
}

/*__________定时器底层配置函数_____________*/
/********************************
*描述：定时器底册驱动，开启时钟，设置中断优先级，此函数会被HAL_TIM_Base_Init()函数调用
*参数：		1、NULL
*返回值：	1、NULL
********************************/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();            //使能TIM4时钟
		__HAL_TIM_CLEAR_IT(&TIM4_Handler, TIM_IT_UPDATE);  // 清除更新中断标志位
		__HAL_TIM_ENABLE_IT(&TIM4_Handler,TIM_IT_UPDATE);  // 使能更新中断
		
		HAL_NVIC_SetPriority(TIM4_IRQn,1,0);    //设置中断优先级，抢占优先级1，响应优先级0。分组为4
		HAL_NVIC_EnableIRQ(TIM4_IRQn);          //开启ITM4中断   
	}
	
	#if STSTEM_TIM6_ENABLE
		if(htim->Instance==TIM6)
		{
			__HAL_RCC_TIM6_CLK_ENABLE();            //使能TIM6时钟
			__HAL_TIM_CLEAR_IT(&TIM6_Handler, TIM_IT_UPDATE);  // 清除更新中断标志位
			__HAL_TIM_ENABLE_IT(&TIM7_Handler,TIM_IT_UPDATE);  // 使能更新中断
			
			HAL_NVIC_SetPriority(TIM6_IRQn,4,0);    //设置中断优先级，抢占优先级1，响应优先级0。分组为4
			HAL_NVIC_EnableIRQ(TIM6_IRQn);          //开启ITM6中断   
		}
	#endif
	
	#if STSTEM_TIM7_ENABLE
		if(htim->Instance==TIM7)
		{
			__HAL_RCC_TIM7_CLK_ENABLE();            //使能TIM7时钟
			__HAL_TIM_CLEAR_IT(&TIM7_Handler, TIM_IT_UPDATE);  // 清除更新中断标志位
			__HAL_TIM_ENABLE_IT(&TIM7_Handler,TIM_IT_UPDATE);  // 使能更新中断
			
			HAL_NVIC_SetPriority(TIM7_IRQn,4,0);    //设置中断优先级，抢占优先级1，响应优先级0。分组为4
			HAL_NVIC_EnableIRQ(TIM7_IRQn);          //开启ITM7中断   
		}
	#endif
	
	#if STSTEM_TIM3_ENABLE
		if(htim->Instance==TIM3)
		{
			__HAL_RCC_TIM3_CLK_ENABLE();            //使能TIM3时钟
			__HAL_TIM_CLEAR_IT(&TIM3_Handler, TIM_IT_UPDATE);  // 清除更新中断标志位
			
			#if STSTEM_TIM3PWM_TI_ENABLE
				__HAL_TIM_ENABLE_IT(&TIM3_Handler,TIM_IT_UPDATE);  // 使能更新中断
				HAL_NVIC_SetPriority(TIM3_IRQn,3,0);    //设置中断优先级，抢占优先级3，子优先级0
				HAL_NVIC_EnableIRQ(TIM3_IRQn);      //开启ITM3中断
			#endif
		}
	#endif
	
	#if STSTEM_TIM2_ENABLE
		if(htim->Instance==TIM2)
		{
			__HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM2时钟
			__HAL_TIM_CLEAR_IT(&TIM2_Handler, TIM_IT_UPDATE);  // 清除更新中断标志位
			
			#if STSTEM_TIM2_TI_ENABLE
				//__HAL_TIM_URS_ENABLE(&TIM2_Handler);               // 仅允许计数器溢出才产生更新中断
				__HAL_TIM_ENABLE_IT(&TIM2_Handler,TIM_IT_UPDATE);  // 使能更新中断
				HAL_NVIC_SetPriority(TIM2_IRQn,3,0);
				HAL_NVIC_EnableIRQ(TIM2_IRQn);
			#endif
		}
	#endif
}

#if STSTEM_TIM4_ENABLE
	struct TIM4_IT_FLAGS Timer_IT_flags = 
	{
		._10msec_flag = 0,
		._10msec = 0,
		._100msec_flag = 0,
		._100msec = 0,
		._300msec_flag = 0,
		._1sec_flag  = 0,
		._1sec = 0,
		._1min_flag = 0,
		._1min = 0
	};

	//通用定时器4中断初始化，arr：自动重装值，psc：时钟预分频数，定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us
	//此处默认频率为100Hz，10ms产生一次中断
	TIM_HandleTypeDef TIM4_Handler;

	void sys_TIM4_ENABLE(void)
	{
		TIM_ClockConfigTypeDef sClockSourceConfig = {0};
		TIM_MasterConfigTypeDef sMasterConfig = {0};
		
		TIM4_Handler.Instance=TIM4;                          		//通用定时器4
		TIM4_Handler.Init.Prescaler=(600-1);                     	//分频系数
		TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    		//向上计数器
		TIM4_Handler.Init.Period=(1000-1);                        	//自动装载值
		TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;		//时钟分频因子
		TIM4_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
		HAL_TIM_Base_Init(&TIM4_Handler);
		
		sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
		HAL_TIM_ConfigClockSource(&TIM4_Handler, &sClockSourceConfig);
		
		sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
		sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
		HAL_TIMEx_MasterConfigSynchronization(&TIM4_Handler, &sMasterConfig);
		
		HAL_TIM_Base_Start_IT(&TIM4_Handler); //使能定时器4和定时器4更新中断：TIM_IT_UPDATE 
	}

	//定时器4中断服务函数
	void TIM4_IRQHandler(void)
	{
		HAL_TIM_IRQHandler(&TIM4_Handler);
	}
#endif

#if STSTEM_TIM3PWM_ENABLE

TIM_HandleTypeDef 	TIM3_Handler;      	//定时器句柄
TIM_OC_InitTypeDef 	TIM3_CH1Handler,TIM3_CH2Handler,TIM3_CH3Handler,TIM3_CH4Handler;	//定时器3通道1-4句柄
void sys_TIM3PWM_ENABLE(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	TIM3_Handler.Instance=TIM3;          	//定时器3
	TIM3_Handler.Init.Prescaler=tim3prsc;       //定时器分频
	TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
	TIM3_Handler.Init.Period=tim3arr;          //自动重装载值
	TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	TIM3_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	
	
	/*如果要开启定时中断，取消这个语句的注释和HAL_TIM_Base_MspInit中的TIM3的IRQ注释*/
	#if STSTEM_TIM3PWM_TI_ENABLE
		HAL_TIM_Base_Init(&TIM3_Handler);
		HAL_TIM_Base_Start_IT(&TIM3_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE
	#endif
	
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&TIM3_Handler, &sClockSourceConfig);
	
	HAL_TIM_PWM_Init(&TIM3_Handler);       //初始化PWM
	
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&TIM3_Handler, &sMasterConfig);
    
	if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_0010)
	{
		TIM3_CH2Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
		TIM3_CH2Handler.Pulse=tim3arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
		/*输出比较极性为低，对于PWM1模式时，当计数值小于比较值时输出低电平，PWM2与PWM1正好相反，注意规范定义低电平有效，高电平截止*/
		/*即当计数值小于比较值时输出有效，器件可以工作，即这个比较值越大，器件工作时间越长，即 占空比 越大*/
		TIM3_CH2Handler.OCPolarity=TIM_OCPOLARITY_LOW;
		HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH2Handler,TIM_CHANNEL_2);//配置TIM3通道2
		__HAL_TIM_DISABLE_OCxPRELOAD(&TIM3_Handler, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_2);//开启PWM通道2
	}
	if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_0001)
	{
		TIM3_CH1Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
		TIM3_CH1Handler.Pulse=tim3arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
		TIM3_CH1Handler.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低 
		TIM3_CH1Handler.OCFastMode = TIM_OCFAST_DISABLE;
		HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH1Handler,TIM_CHANNEL_1);//配置TIM3通道1
		__HAL_TIM_DISABLE_OCxPRELOAD(&TIM3_Handler, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_1);//开启PWM通道1
	}
	if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_0100)
	{
		TIM3_CH3Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
		TIM3_CH3Handler.Pulse=tim3arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
		TIM3_CH3Handler.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低 
		HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH3Handler,TIM_CHANNEL_3);//配置TIM3通道3
		__HAL_TIM_DISABLE_OCxPRELOAD(&TIM3_Handler, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_3);//开启PWM通道3
	}
	if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_1000)
	{
		TIM3_CH4Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
		TIM3_CH4Handler.Pulse=tim3arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
		TIM3_CH4Handler.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低 
		HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH4Handler,TIM_CHANNEL_4);//配置TIM3通道4
		__HAL_TIM_DISABLE_OCxPRELOAD(&TIM3_Handler, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_4);//开启PWM通道4
	}

}

//定时器PWM初始化底层函数，时钟使能，引脚配置
//此函数会被HAL_TIM_PWM_Init()调用
//htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_Initure;
	
    if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();			//使能定时器3时钟
		
		if(STSTEM_TIM3PWM_REMAP_ENABLE)	//如果全部重映射
		{
			//__HAL_AFIO_REMAP_TIM3_ENABLE();			
			//TIM3通道引脚全部重映射使能，具体怎么映射看库函数注释或sys.h里
			__HAL_RCC_GPIOC_CLK_ENABLE();			//开启GPIOC时钟
			if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_0001)	GPIO_Initure.Pin=GPIO_PIN_6;	//如果开启通道1，PC6
			if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_0010)	GPIO_Initure.Pin=GPIO_PIN_7;	//如果开启通道2，PC7
			if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_0100)	GPIO_Initure.Pin=GPIO_PIN_8;	//如果开启通道3，PC8
			if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_1000)	GPIO_Initure.Pin=GPIO_PIN_6;	//如果开启通道4，PC9
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
			GPIO_Initure.Pull=GPIO_PULLUP;          //上拉，所以硬件上最好低电平有效，高电平截止
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
			GPIO_Initure.Alternate = GPIO_AF2_TIM3;
			HAL_GPIO_Init(GPIOC,&GPIO_Initure); 
		}else if(STSTEM_TIM3PWM_REMAP_PARTIAL)	//如果部分重映射
		{
			//__HAL_AFIO_REMAP_TIM3_PARTIAL();		
			//TIM3通道引脚部分重映射使能，具体怎么映射看库函数注释或sys.h里
			__HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
			if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_0001)	GPIO_Initure.Pin=GPIO_PIN_4;	//如果开启通道1，PB4
			if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_0010)	GPIO_Initure.Pin=GPIO_PIN_5;	//如果开启通道2，PB5
			if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_0100)	GPIO_Initure.Pin=GPIO_PIN_0;	//如果开启通道3，PB0
			if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_1000)	GPIO_Initure.Pin=GPIO_PIN_1;	//如果开启通道4，PB1
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
			GPIO_Initure.Pull=GPIO_PULLUP;          //上拉，所以硬件上最好低电平有效，高电平截止
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
			GPIO_Initure.Alternate = GPIO_AF2_TIM3;
			HAL_GPIO_Init(GPIOB,&GPIO_Initure); 			
		}else{									//没有重映射
			
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
			GPIO_Initure.Pull=GPIO_PULLUP;          //上拉，所以硬件上最好低电平有效，高电平截止
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
			
			if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_0001)	
			{
				__HAL_RCC_GPIOA_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_6;	//如果开启通道1，PA6
				GPIO_Initure.Alternate = GPIO_AF2_TIM3;
				HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
			}
			if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_0010)
			{
				__HAL_RCC_GPIOA_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_7;	//如果开启通道2，PA7
				GPIO_Initure.Alternate = GPIO_AF2_TIM3;
				HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
			}
			if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_0100)
			{
				__HAL_RCC_GPIOB_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_0;	//如果开启通道3，PB0
				GPIO_Initure.Alternate = GPIO_AF2_TIM3;
				HAL_GPIO_Init(GPIOB,&GPIO_Initure); 
			}
			if((STSTEM_TIM3PWM_CHANNEL_ENABLE) & B0000_1000)
			{
				__HAL_RCC_GPIOB_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_1;	//如果开启通道4，PB1
				GPIO_Initure.Alternate = GPIO_AF2_TIM3;
				HAL_GPIO_Init(GPIOB,&GPIO_Initure);
			}
		}
	}
	#if STSTEM_TIM2_ENABLE
	if(htim->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();			//使能定时器2时钟
		__HAL_AFIO_REMAP_TIM2_ENABLE();			/*TIM2通道引脚完全重映射使能 (CH1/ETR/PA15, CH2/PB3, CH3/PB10, CH4/PB11)*/
		
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;          //上拉，所以硬件上最好低电平有效，高电平截止
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
		
		if((STSTEM_TIM2PWM_CHANNEL_ENABLE) & B0000_0001)	//CH1/ETR/PA15
		{
			__HAL_RCC_GPIOA_CLK_ENABLE();
			GPIO_Initure.Pin=GPIO_PIN_15;
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);
		}
		if((STSTEM_TIM2PWM_CHANNEL_ENABLE) & B0000_0010)	//CH2/PB3
		{
			__HAL_RCC_GPIOB_CLK_ENABLE();
			GPIO_Initure.Pin=GPIO_PIN_3;
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);
		}
		if((STSTEM_TIM2PWM_CHANNEL_ENABLE) & B0000_0100)	//CH3/PB10
		{
			__HAL_RCC_GPIOB_CLK_ENABLE();
			GPIO_Initure.Pin=GPIO_PIN_10;
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);
		}
		if((STSTEM_TIM2PWM_CHANNEL_ENABLE) & B0000_1000)	//CH4/PB11
		{
			__HAL_RCC_GPIOB_CLK_ENABLE();
			GPIO_Initure.Pin=GPIO_PIN_11;
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);
		}
	}
	#endif
	
	#if STSTEM_TIM1PWM_ENABLE
	if(htim->Instance==TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();			//使能定时器1时钟
		
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;
		GPIO_Initure.Pull=GPIO_NOPULL;
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;
		
		/*引脚：
						CH1/CH1N		CH2/CH2N		CH3/CH3N		CH4		BKIN（刹车信号输入）
		默认			PA8/PB13		PA9/PB14		PA10/PB15		PA11	PB12
		部分重映射		PA8/PA7			PA9/PB0			PA10/PB1		PA11	PA6
		完全重映射		PE9/PE8			PE11/PE10		PE13/PE12		PE14	PE15
		*/
		if(STSTEM_TIM1PWM_REMAP_PARTIAL)
		{
			__HAL_AFIO_REMAP_TIM1_PARTIAL();
			
			if(STSTEM_TIM1PWM_useBreak)
			{
				__HAL_RCC_GPIOA_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_6;
				HAL_GPIO_Init(GPIOA,&GPIO_Initure);
			}
			
			if(STSTEM_TIM1PWM_CHANNEL_ENABLE & B0000_0001)
			{
				__HAL_RCC_GPIOA_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_7;
				HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
				GPIO_Initure.Pin=GPIO_PIN_8;
				HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
			}
			if(STSTEM_TIM1PWM_CHANNEL_ENABLE & B0000_0010)
			{
				__HAL_RCC_GPIOA_CLK_ENABLE();
				__HAL_RCC_GPIOB_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_9;
				HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
				GPIO_Initure.Pin=GPIO_PIN_0;
				HAL_GPIO_Init(GPIOB,&GPIO_Initure); 
			}
			if(STSTEM_TIM1PWM_CHANNEL_ENABLE & B0000_0100)
			{
				__HAL_RCC_GPIOA_CLK_ENABLE();
				__HAL_RCC_GPIOB_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_10;
				HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
				GPIO_Initure.Pin=GPIO_PIN_1;
				HAL_GPIO_Init(GPIOB,&GPIO_Initure); 
			}
			if(STSTEM_TIM1PWM_CHANNEL_ENABLE & B0000_1000)
			{
				__HAL_RCC_GPIOA_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_11;
				HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
			}
		}else if(STSTEM_TIM1PWM_REMAP_ENABLE)
		{
			__HAL_AFIO_REMAP_TIM1_ENABLE();
			__HAL_RCC_GPIOE_CLK_ENABLE();
			
			if(STSTEM_TIM1PWM_useBreak)
			{
				GPIO_Initure.Pin=GPIO_PIN_15;
				HAL_GPIO_Init(GPIOE,&GPIO_Initure);
			}
			
			if(STSTEM_TIM1PWM_CHANNEL_ENABLE & B0000_0001)
			{
				GPIO_Initure.Pin=GPIO_PIN_9;
				HAL_GPIO_Init(GPIOE,&GPIO_Initure); 
				GPIO_Initure.Pin=GPIO_PIN_8;
				HAL_GPIO_Init(GPIOE,&GPIO_Initure); 
			}
			if(STSTEM_TIM1PWM_CHANNEL_ENABLE & B0000_0010)
			{
				GPIO_Initure.Pin=GPIO_PIN_11;
				HAL_GPIO_Init(GPIOE,&GPIO_Initure); 
				GPIO_Initure.Pin=GPIO_PIN_10;
				HAL_GPIO_Init(GPIOE,&GPIO_Initure); 
			}
			if(STSTEM_TIM1PWM_CHANNEL_ENABLE & B0000_0100)
			{
				GPIO_Initure.Pin=GPIO_PIN_13;
				HAL_GPIO_Init(GPIOE,&GPIO_Initure); 
				GPIO_Initure.Pin=GPIO_PIN_12;
				HAL_GPIO_Init(GPIOE,&GPIO_Initure); 
			}
			if(STSTEM_TIM1PWM_CHANNEL_ENABLE & B0000_1000)
			{
				GPIO_Initure.Pin=GPIO_PIN_14;
				HAL_GPIO_Init(GPIOE,&GPIO_Initure); 
			}
		}else{
			/*引脚：
							CH1/CH1N		CH2/CH2N		CH3/CH3N		CH4		BKIN（刹车信号输入）
			默认			PA8/PB13		PA9/PB14		PA10/PB15		PA11	PB12
			部分重映射		PA8/PA7			PA9/PB0			PA10/PB1		PA11	PA6
			完全重映射		PE9/PE8			PE11/PE10		PE13/PE12		PE14	PE15
			*/
			
			if(STSTEM_TIM1PWM_useBreak)
			{
				__HAL_RCC_GPIOB_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_12;
				HAL_GPIO_Init(GPIOB,&GPIO_Initure);
			}
			
			if(STSTEM_TIM1PWM_CHANNEL_ENABLE & B0000_0001)
			{
				__HAL_RCC_GPIOA_CLK_ENABLE();
				__HAL_RCC_GPIOB_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_8;
				HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
				GPIO_Initure.Pin=GPIO_PIN_13;
				HAL_GPIO_Init(GPIOB,&GPIO_Initure); 
			}
			if(STSTEM_TIM1PWM_CHANNEL_ENABLE & B0000_0010)
			{
				__HAL_RCC_GPIOA_CLK_ENABLE();
				__HAL_RCC_GPIOB_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_9;
				HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
				GPIO_Initure.Pin=GPIO_PIN_14;
				HAL_GPIO_Init(GPIOB,&GPIO_Initure); 
			}
			if(STSTEM_TIM1PWM_CHANNEL_ENABLE & B0000_0100)
			{
				__HAL_RCC_GPIOA_CLK_ENABLE();
				__HAL_RCC_GPIOB_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_10;
				HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
				GPIO_Initure.Pin=GPIO_PIN_15;
				HAL_GPIO_Init(GPIOB,&GPIO_Initure); 
			}
			if(STSTEM_TIM1PWM_CHANNEL_ENABLE & B0000_1000)
			{
				__HAL_RCC_GPIOA_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_11;
				HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
			}
		}
		
	}
	#endif
}


//设置TIM3通道的占空比
//percent:占空比百分数
void TIM3_set_Channel_Pulse(u8 channel,float percent)
{
	float compare;
	if(percent < 0) percent = 0;
	if(percent > 100) percent = 100.0;
	percent /= 100.0;
	compare = ((float)tim3arr) * percent;
	switch(channel)
	{
		case TIM3PWM_Channel_1: TIM3->CCR1=(u32)compare;break;
		case TIM3PWM_Channel_2: TIM3->CCR2=(u32)compare;break;
		case TIM3PWM_Channel_3: TIM3->CCR3=(u32)compare;break;
		case TIM3PWM_BuzzerCh: TIM3->CCR4=(u32)compare;break;
		default:break;
	}
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Handler);
}
#endif




#if SYSTEM_UART1_ENABLE||SYSTEM_UART2_ENABLE||SYSTEM_UART3_ENABLE

UART_HandleTypeDef UART1_Handler,UART2_Handler,UART3_Handler; //UART句柄
u8 aRxBuffer1[RXBUFFERSIZE],aRxBuffer2[RXBUFFERSIZE],aRxBuffer3[RXBUFFERSIZE];//HAL库使用的串口接收缓冲
//QueueHandle_t Uart1_fifo;	/*串口1的模式1数据接受fifo句柄*/

/*printf函数和printf_uart会调用此函数发送单个字节，需填入串口发送，只用printf_uart就行了*/
void _putchar(char character)
{
	// send char to console etc.
	//如果使用RTOS，这里是否应该停止其调度器，在结束处再打开
	switch(uart2sent)/*uart2sent全局变量，用于判断发送到哪一个串口*/
	{
		case UART1:/*发往串口1*/
			HAL_UART_Transmit(&UART1_Handler, (uint8_t *)&character, 1, 0xA); //超时时间为0x12C，为10ms
			break;
		case UART2:/*发往串口2*/
			HAL_UART_Transmit(&UART2_Handler, (uint8_t *)&character, 1, 0x12C);
			break;
		case UART3:/*发往串口3*/
			HAL_UART_Transmit(&UART3_Handler, (uint8_t *)&character, 1, 0x12C);
			break;
		default:/*默认发往串口1*/
			HAL_UART_Transmit(&UART1_Handler, (uint8_t *)&character, 1, 0xA);
			break;
	}
}

#if SYSTEM_UART1_ENABLE
void sys_USART1_ENABLE(void)
{
	//UART 初始化设置
	UART1_Handler.Instance=USART1;					    		//USART1
	UART1_Handler.Init.BaudRate=(uint32_t)SYSTEM_UART1_BOUND;	//波特率
	
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   		//字长为8位数据格式
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    		//一个停止位
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    		//无奇偶校验位
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   		//无硬件流控
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    		//收发模式
	UART1_Handler.Init.OverSampling = UART_OVERSAMPLING_16;		//不知道有没有影响（TODO:试验有没有这句话有什么变化）
	HAL_UART_Init(&UART1_Handler);					    		//HAL_UART_Init()会使能UART1
	
	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer1, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
	
	
//	Uart1_fifo = xQueueCreate(USART1_RX_FIFO_MaxNum,sizeof( char ));
//	
//	if(Uart1_fifo == NULL)
//	{
//		FaultASSERT("AT:fifo_create");
//	}
}
char USART1_RX_BUF[USART1_RX_BUF_MaxNum]; 		/*串口1的模式0数据接受区*/

u8 sys_USART1_RX_Fetch(u8 is_print, char *buf,u16 *RX_ByteNum)
{
	if(!((USART1_RX_CONFIG) & (USART1_RX_MODE_mask)))
	{
		/*0为只接受以'\r\n'结尾的数据*/
		if(USART1_isDone)
		{
			if(is_print)
				printf_uart(UART1,"%s",USART1_RX_BUF);
			
			if(USART1_RX_ByteNum >= USART1_RX_BUF_MaxNum) /*防止栈溢出*/
			{
				mystrncpy(buf,USART1_RX_BUF,USART1_RX_BUF_MaxNum);
			}else{
				mystrncpy(buf,USART1_RX_BUF,USART1_RX_ByteNum);
			}
			//buf = USART1_RX_BUF;
			*RX_ByteNum = USART1_RX_ByteNum;
			/*USART1_RX_BUF共有USART1_RX_ByteNum个有效字节，不带\r\n的字符串，第USART1_RX_ByteNum的位置是结束符\0*/
			USART1_SetUnDone;
			return HAL_OK;
		}else{
			return HAL_ERROR;
			}
	}else{
		/*1为以FIFO先进先出的环形缓存实现接受区，无协议*/
		
//		u16 i = 0;
		char ibuf = 42;
		
//		while(xQueueReceive(Uart1_fifo, &ibuf, 0) == pdPASS)
//		{
//			buf[i++] = ibuf;
//		}
//		buf[i] = '\0';
//		*RX_ByteNum = i;
//		
//		if(is_print)
//			printf_uart(UART1,"%s",buf);
//		
		if(ibuf != 42)
		{
			return HAL_OK;
		}else{
			return HAL_ERROR;
			}
	}
}

#endif

#if SYSTEM_UART2_ENABLE
void sys_USART2_ENABLE(void)
{
	//UART 初始化设置
	UART2_Handler.Instance=USART2;					    		//USART2
	UART2_Handler.Init.BaudRate=(uint32_t)SYSTEM_UART2_BOUND;	//波特率
	
	UART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;   		//字长为8位数据格式
	UART2_Handler.Init.StopBits=UART_STOPBITS_1;	    		//一个停止位
	UART2_Handler.Init.Parity=UART_PARITY_NONE;		    		//无奇偶校验位
	UART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   		//无硬件流控
	UART2_Handler.Init.Mode=UART_MODE_TX_RX;		    		//收发模式
	HAL_UART_Init(&UART2_Handler);					    		//HAL_UART_Init()会使能UART2
	
	HAL_UART_Receive_IT(&UART2_Handler, (u8 *)aRxBuffer2, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
}


u8 sys_USART2_RX_Fetch(u8 is_print, char *buf,u16 *RX_ByteNum)
{
	return HAL_OK;
}
#endif
#if SYSTEM_UART3_ENABLE
void sys_USART3_ENABLE(void)
{
	//UART 初始化设置
	UART3_Handler.Instance=USART3;					    		//USART3
	UART3_Handler.Init.BaudRate=(uint32_t)SYSTEM_UART3_BOUND;	//波特率
	
	UART3_Handler.Init.WordLength=UART_WORDLENGTH_8B;   		//字长为8位数据格式
	UART3_Handler.Init.StopBits=UART_STOPBITS_1;	    		//一个停止位
	UART3_Handler.Init.Parity=UART_PARITY_NONE;		    		//无奇偶校验位
	UART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   		//无硬件流控
	UART3_Handler.Init.Mode=UART_MODE_TX_RX;		    		//收发模式
	HAL_UART_Init(&UART3_Handler);					    		//HAL_UART_Init()会使能UART3
	
	HAL_UART_Receive_IT(&UART3_Handler, (u8 *)aRxBuffer3, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
}

u8 sys_USART3_RX_Fetch(u8 is_print, char *buf,u16 *RX_ByteNum)
{
	return HAL_OK;
}
#endif


//UART底层初始化，时钟使能，引脚配置，中断配置
//此函数会被HAL_UART_Init()调用
//huart:串口句柄
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;
	if(huart->Instance==USART1)//如果是串口1，进行串口1 MSP初始化
	{
		__HAL_RCC_USART1_CLK_ENABLE();			//使能USART1时钟
		//__HAL_RCC_AFIO_CLK_ENABLE();
		#if SYSTEM_UART1_REMAP_ENABLE
			//__HAL_AFIO_REMAP_USART1_ENABLE();
			__HAL_RCC_GPIOB_CLK_ENABLE();
			GPIO_Initure.Pin=GPIO_PIN_6;			//PB6 TX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
			GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//高速
			GPIO_Initure.Alternate = GPIO_AF7_USART1;
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PB6

			GPIO_Initure.Pin=GPIO_PIN_7;			//PB7 RX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用模式
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PB7
		#else 
			__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
			GPIO_Initure.Pin=GPIO_PIN_9;			//PA9 TX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
			GPIO_Initure.Pull=GPIO_NOPULL;			//上拉
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//高速
			GPIO_Initure.Alternate = GPIO_AF7_USART1;
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA9

			GPIO_Initure.Pin=GPIO_PIN_10;			//PA10 RX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用模式	
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA10
		#endif
		
		HAL_NVIC_SetPriority(USART1_IRQn,3,0);			//抢占优先级3，子优先级0
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//使能USART1中断通道
	}
	
	if(huart->Instance==USART2)//如果是串口2，进行串口2 MSP初始化
	{
		__HAL_RCC_USART2_CLK_ENABLE();			//使能USART2时钟
		//__HAL_RCC_AFIO_CLK_ENABLE();
		#if SYSTEM_UART2_REMAP_ENABLE
			//__HAL_AFIO_REMAP_USART2_ENABLE();
			__HAL_RCC_GPIOD_CLK_ENABLE();
			GPIO_Initure.Pin=GPIO_PIN_5;			//PD5 TX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
			GPIO_Initure.Pull=GPIO_NOPULL;			//上拉
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//高速
			GPIO_Initure.Alternate = GPIO_AF7_USART2;
			HAL_GPIO_Init(GPIOD,&GPIO_Initure);	   	//初始化PD5

			GPIO_Initure.Pin=GPIO_PIN_6;			//PD6 RX
			GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;		
			HAL_GPIO_Init(GPIOD,&GPIO_Initure);	   	//初始化PD6
		#else 
			__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
			GPIO_Initure.Pin=GPIO_PIN_2;			//PA2 TX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
			GPIO_Initure.Pull=GPIO_NOPULL;			//上拉
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//高速
			GPIO_Initure.Alternate = GPIO_AF7_USART2;
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA2

			GPIO_Initure.Pin=GPIO_PIN_3;			//PA3 RX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;			
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA3
		#endif
		
		HAL_NVIC_SetPriority(USART2_IRQn,4,0);
		HAL_NVIC_EnableIRQ(USART2_IRQn);				//使能USART2中断通道
	}
	
	if(huart->Instance==USART3)//如果是串口3，进行串口3 MSP初始化
	{
		__HAL_RCC_USART3_CLK_ENABLE();			//使能USART3时钟
		//__HAL_RCC_AFIO_CLK_ENABLE();
		#if SYSTEM_UART3_REMAP_ENABLE
			//__HAL_AFIO_REMAP_USART3_ENABLE();
			__HAL_RCC_GPIOD_CLK_ENABLE();
			GPIO_Initure.Pin=GPIO_PIN_8;			//PD8 TX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
			GPIO_Initure.Pull=GPIO_NOPULL;			//上拉
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//高速
			GPIO_Initure.Alternate = GPIO_AF7_USART3;
			HAL_GPIO_Init(GPIOD,&GPIO_Initure);	   	//初始化PD8

			GPIO_Initure.Pin=GPIO_PIN_9;			//PD9 RX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;			
			HAL_GPIO_Init(GPIOD,&GPIO_Initure);	   	//初始化PD9
		#else 
			__HAL_RCC_GPIOB_CLK_ENABLE();			//使能GPIOB时钟
			GPIO_Initure.Pin=GPIO_PIN_10;			//PB10 TX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
			GPIO_Initure.Pull=GPIO_NOPULL;			//上拉
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//高速
			GPIO_Initure.Alternate = GPIO_AF7_USART3;
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PB10

			GPIO_Initure.Pin=GPIO_PIN_11;			//PB11 RX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;			
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PB11
		#endif
		
		HAL_NVIC_SetPriority(USART3_IRQn,4,0);
		HAL_NVIC_EnableIRQ(USART3_IRQn);				//使能USART3中断通道
	}
}


//char USART1_RX_FIFO_buf[USART1_RX_FIFO_MaxNum] = "0"; 	/*串口1的模式0的FIFO数据接受区，静态创建时才用*/
/*以下四行放在了sys.h里面*/
//#define USART1_RX_DONE_mask 0x8000
//#define USART1_RX_MODE_mask 0x4000
//#define USART1_RX_Rec_r_mask 0x2000
//#define USART1_RX_Num_mask 0x1fff /*0001 1111 1111 1111*/

u16 USART1_RX_CONFIG = 0;	/*    x            x           x        x xxxx xxxx xxxx      */
							/*(接受完成) (协议模式0/1) (接收到\r)   串口1接受数据字节计数 */
							/*串口1接受协议：0为只接受以'\r\n'结尾的数据，1为以FIFO先进先出的环形缓存实现接受区，无协议*/
/*
FIFO先进先出环形队列示意：	头指针出只读出，尾指针出只写入。
							 *		*		*		*		*	...
							 0头	1		2		3		4尾（新入的数据存在尾指针上，随着存储，尾指针向后移动）
							列队头的数据处理完了后，‘0’地址空间的数据进行释放掉，列队头指向下一个可以处理数据的地址‘1’，以此类推
							在一定内存内，尾指针一直移动到最后，这时如果0地址空闲，则把尾指针指向0，从而形成环形
*/
/*以下五行放在了.h里面*/
//#define USART1_SetMode(x) 	USART1_RX_CONFIG |= (((u16)x)<<14) /*用户可用，在任务中设置串口1接收协议*/
//#define USART1_isDone 		(USART1_RX_CONFIG & USART1_RX_DONE_mask)/*用户用，用于判断是否接受完成一次*/
//#define USART1_RX_ByteNum 	(USART1_RX_CONFIG & USART1_RX_Num_mask)/*用户可用，返回接收的字符数量*/
//#define USART1_SetDone 		USART1_RX_CONFIG |= USART1_RX_DONE_mask /*设置串口1接收完成标志位*/
//#define USART1_SetUnDone 		USART1_RX_CONFIG &= USART1_RX_MODE_mask /*用户可用，在任务中处理完后对串口1标志位进行复位，除了mode位，其他位都写0*/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)//如果是串口1
	{	
		if(!((USART1_RX_CONFIG) & (USART1_RX_MODE_mask))) /*协议0：只接受以'\r\n'结尾的数据：从USART1_RX_BUF[]接受数据*/
		{ 
			if(!((USART1_RX_CONFIG) & (USART1_RX_DONE_mask))) /*如果接收未完成*/
			{
				if((USART1_RX_CONFIG) & (USART1_RX_Rec_r_mask)) /*如果接收到了\r*/
				{
					if(aRxBuffer1[0] == '\n') /*如果接受到了\r\n*/
					{
						USART1_RX_BUF[USART1_RX_ByteNum] = '\0'; /*加个结束符\0*/
						USART1_SetDone;
						//USART1_RX_CONFIG++; 
						/*接受完成后的 接受数据字节计数 不包含 '\r\n' 两个*/
					}else{USART1_SetUnDone;} /*没有接收到\n，则重新开始*/
				}else
				{
					if(aRxBuffer1[0] == '\r') {
						USART1_RX_CONFIG |= (USART1_RX_Rec_r_mask);
						//USART1_RX_CONFIG++; /*接受完成后的 接受数据字节计数 不包含 '\r\n' 两个*/
					}else{
						USART1_RX_BUF[(USART1_RX_CONFIG) & (USART1_RX_Num_mask)] = aRxBuffer1[0];
						USART1_RX_CONFIG++;
						if(((USART1_RX_CONFIG) & (USART1_RX_Num_mask)) > (USART1_RX_BUF_MaxNum-1)) USART1_SetUnDone;
					}
					
				}
			}
		}else 		/*协议1：以FIFO先进先出的环形缓存实现接受区，无协议，需要及时拿走，如果缓存满了则新入数据丢失*/
		{
			/*这里不好使，当数据量很大的时候MCU就卡死了，以后换成一个别的协议解析比如modbus等等*/
//			BaseType_t YieldRequired;
//			if(uxQueueSpacesAvailable(Uart1_fifo) > 0)
//			{
//				xQueueSendFromISR(Uart1_fifo,&aRxBuffer1[0],&YieldRequired);
//			}else{
//				xQueueOverwriteFromISR(Uart1_fifo,&aRxBuffer1[0],&YieldRequired);
//			}
//			if(YieldRequired == pdTRUE) portYIELD_FROM_ISR(YieldRequired);
		}
	}else if(huart->Instance==USART2)//如果是串口2
	{
		
	}else if(huart->Instance==USART3)//如果是串口3
	{
		
	}
}

/*TODO：下面是不是可用精简精简...*/
#if SYSTEM_UART1_ENABLE
void USART1_IRQHandler(void)	//串口1中断服务程序
{ 
	u32 timeout=0;
	#if SYSTEM_SUPPORT_OS	 	//使用OS
		//OSIntEnter();  
		taskENTER_CRITICAL();           //进入临界区
	#endif
	
	HAL_UART_IRQHandler(&UART1_Handler);	//调用HAL库中断处理公用函数
	
	timeout=0;
    while (HAL_UART_GetState(&UART1_Handler) != HAL_UART_STATE_READY)//等待就绪
	{
	 timeout++;////超时处理
     if(timeout>HAL_MAX_DELAY) break;		
	
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer1, RXBUFFERSIZE) != HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
	 timeout++; //超时处理
	 if(timeout>HAL_MAX_DELAY) break;	
	}
	#if SYSTEM_SUPPORT_OS	 	//使用OS
		//OSIntExit();  	
		taskEXIT_CRITICAL();            //退出临界区
	#endif
}
#endif

#if SYSTEM_UART2_ENABLE
void USART2_IRQHandler(void)	//串口2中断服务程序
{ 
	u32 timeout=0;
	#if SYSTEM_SUPPORT_OS	 	//使用OS
		//OSIntEnter();    
		taskENTER_CRITICAL();           //进入临界区
	#endif
	
	HAL_UART_IRQHandler(&UART2_Handler);	//调用HAL库中断处理公用函数
	
	timeout=0;
    while (HAL_UART_GetState(&UART2_Handler) != HAL_UART_STATE_READY)//等待就绪
	{
	 timeout++;////超时处理
     if(timeout>HAL_MAX_DELAY) break;		
	
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART2_Handler, (u8 *)aRxBuffer2, RXBUFFERSIZE) != HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
	 timeout++; //超时处理
	 if(timeout>HAL_MAX_DELAY) break;	
	}
	#if SYSTEM_SUPPORT_OS	 	//使用OS
		//OSIntExit();  			
		taskEXIT_CRITICAL();            //退出临界区	
	#endif
}
#endif

#if SYSTEM_UART3_ENABLE
void USART3_IRQHandler(void)	//串口3中断服务程序
{ 
	u32 timeout=0;
	#if SYSTEM_SUPPORT_OS	 	//使用OS
		//OSIntEnter();    
		taskENTER_CRITICAL();           //进入临界区
	#endif
	
	HAL_UART_IRQHandler(&UART3_Handler);	//调用HAL库中断处理公用函数
	
	timeout=0;
    while (HAL_UART_GetState(&UART3_Handler) != HAL_UART_STATE_READY)//等待就绪
	{
	 timeout++;////超时处理
     if(timeout>HAL_MAX_DELAY) break;		
	
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART3_Handler, (u8 *)aRxBuffer3, RXBUFFERSIZE) != HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
	 timeout++; //超时处理
	 if(timeout>HAL_MAX_DELAY) break;	
	}
	#if SYSTEM_SUPPORT_OS	 	//使用OS
		//OSIntExit();  			
		taskEXIT_CRITICAL();            //退出临界区
	#endif
}
#endif

#endif






/*_____________系统变量和函数（莫要乱动撒）_______________*/
u16	StartUpTimes;			/*用于保存开机次数，储存在最后一个或倒数第二个页*/
uint32_t UIDw[3]; 			/*保存STM32内部UID识别码，全球唯一识别码*/
uint32_t sysCoreClock; 		/*获取HCLK频率，外设时钟均来自此再分频*/

u8 is_buzzer_once = 0;
u8 is_buzzer_bibi = 0;

/*字符串转整形，stm32不支持标准的atoi,这里自己实现*/
int myatoi(const char *str)
{
	int s=0;
	uint8_t falg=0;
	
	while(*str==' ')
	{
		str++;
	}

	if(*str=='-'||*str=='+')
	{
		if(*str=='-')
		falg=1;
		str++;
	}

	while(*str>='0'&&*str<='9')
	{
		s=s*10+*str-'0';
		str++;
		if(s<0)
		{
			s=2147483647;
			break;
		}
	}
	return s*(falg?-1:1);
}


//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//关闭所有中断
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(uint32_t addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}




