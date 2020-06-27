#include "sys.h"

/*
需要改的地方：


3、写顺序：
	移植菜单模板，连带按键检测也都搞了
	FLASH存储开机次数
	定时器：RTOS发送任务通知
	开始完成裸机基本框架，加个菜单框架，再按照我的计划里有的
	开始完成RTOS框架，写上所有API
	歇会儿
	开始所有HAL库的罗列！可以参考的：原子的HAL手册和历程直接复制；硬石的HAL每天一例（这个很多）；安富莱
	其他FATFS等更多器件的驱动，参考优秀工程框架
	LWIP STemWIN

*/

/*__________MCU外设初始化序列_____________*/
/********************************
*描述：初始化序列，所有初始化写到这里面——————！按需要进行修改！
*参数：		1、NULL
*返回值：	1、NULL
********************************/
uint32_t UIDw[3]; /*保存STM32内部UID识别码，全球唯一识别码*/
uint32_t sysCoreClock; /*获取HCLK频率，外设时钟均来自此再分频*/
u8 is_buzzer_once = 0;
u8 is_buzzer_bibi = 0;
static uint8_t Init_OK_Num = 0;
uint8_t is_quitFault;
void sys_MCU_Init_Seq(void)
{
	/*设置时钟,8M * 9 = 72M，涉及到延时准确性，不要乱动！*/
	Stm32_Clock_Init(RCC_PLL_MUL9);
	Init_OK_Num++;
	/*delay初始化*/
	delay_init();
	Init_OK_Num++;
	/*设置MCO*/
	#if SYSTEM_MCO_PA8_OUT
		sys_MCO_Out_Enable();
		Init_OK_Num++;
	#endif
	/*设置RTC*/
	#if SYSTEM_RTC_ENABLE
		if(sys_RTC_Enable() != ReturnOK)
		{
			FaultASSERT(Init_OK_Num,"AT : sys_RTC_Enable",&is_quitFault);
			if(sys_RTC_Enable() == ReturnOK) is_quitFault = TRUE;
		}
		Init_OK_Num++;
	#endif
	/*设置CRC*/
	#if SYSTEM_CRC_ENABLE
		if(HAL_CRC_Accumulate(&hcrc, (uint32_t *)aDataBuffer, BUFFER_SIZE) != uwExpectedCRCValue)
		{
			FaultASSERT(Init_OK_Num,"AT : HAL_CRC_Accumulate",&is_quitFault);
			if(HAL_CRC_Accumulate(&hcrc, (uint32_t *)aDataBuffer, BUFFER_SIZE) == uwExpectedCRCValue) is_quitFault = TRUE;
		}
		Init_OK_Num++;
	#endif
	/*初始化并启动TIM4*/
	#if STSTEM_TIM4_ENABLE
		sys_TIM4_ENABLE();
		Init_OK_Num++;
	#endif
	/*初始化并启动TIM3PWM通道*/
	#if STSTEM_TIM3PWM_ENABLE
		sys_TIM3PWM_ENABLE();
		Init_OK_Num++;
	#endif
	/*按照设定初始化串口1、2、3*/
	#if SYSTEM_UART1_ENABLE
		sys_USART1_ENABLE();
		Init_OK_Num++;
	#endif
	#if SYSTEM_UART2_ENABLE
		sys_USART2_ENABLE();
		Init_OK_Num++;
	#endif
	#if SYSTEM_UART3_ENABLE
		sys_USART3_ENABLE();
		Init_OK_Num++;
	#endif
	/*初始化看门狗*/
	#if SYSTEM_IWDG_ENABLE
		sys_IWDG_ENABLE();
		Init_OK_Num++;
	#endif
	
	/*获取HCLK频率，外设时钟均来自此再分频*/
	sysCoreClock = HAL_RCC_GetHCLKFreq(); 
	/*保存STM32内部UID识别码，全球唯一识别码*/
	UIDw[0] = HAL_GetUIDw0();UIDw[1] = HAL_GetUIDw1();UIDw[2] = HAL_GetUIDw2(); 
	
	/*以下为用户应用的MCU外设初始化序列*/
	
}

/*__________器件外设初始化，并开机自检_____________*/
/********************************
*描述：进行MCU外设的开启，进行外设器件的IO、读测试等器件参数设置工作——————！按需要进行修改！
*参数：		1、NULL
*返回值：	1、NULL
********************************/
void sys_Device_Init_Seq(void)
{
	/*以下为用户应用的Device初始化序列*/
	
	
	buzzer_bibi_once; //响一声表示初始化结束
	
}

/*表示初始化有问题，串口提示，灯提示，声提示，并进入死循环*/
/*传入参数：错误代码，错误提示信息，退出本函数的标志位*/
void FaultASSERT(uint8_t errNum,char* message,uint8_t* is_quit)
{
	/*往串口1发送数据*/
	printf_uart(UART1,"Fault : %d\t",errNum);
	printf_uart(UART1,"Message : %s\n",message);
	//灯提示，声提示
	buzzer_bibi_on;
	if(!(*is_quit))
	{
		delay_ms(233);
		return ;//这里仍可以进行检测，如果恢复正常则可以继续跑或者复位
	}
	buzzer_bibi_off;
}
/*__________时钟系统配置函数_____________*/
/********************************
*描述：上电时必要进行的时钟配置，默认使用外部高速信号源
*参数：		1、PLL:选择的倍频数，RCC_PLL_MUL2~RCC_PLL_MUL16
*返回值：	1、ReturnOK,成功
			2、ReturnErr,失败
********************************/
uint8_t Stm32_Clock_Init(uint32_t PLL)
{
    HAL_StatusTypeDef ret = HAL_OK;
    RCC_OscInitTypeDef RCC_OscInitStructure; 
    RCC_ClkInitTypeDef RCC_ClkInitStructure;
    
    RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_HSE;    	//时钟源为HSE
    RCC_OscInitStructure.HSEState=RCC_HSE_ON;                      	//打开HSE
    RCC_OscInitStructure.HSEPredivValue=RCC_HSE_PREDIV_DIV1;		//HSE预分频
    RCC_OscInitStructure.PLL.PLLState=RCC_PLL_ON;					//打开PLL
    RCC_OscInitStructure.PLL.PLLSource=RCC_PLLSOURCE_HSE;			//PLL时钟源选择HSE
    RCC_OscInitStructure.PLL.PLLMUL=PLL; 							//主PLL倍频因子
    ret=HAL_RCC_OscConfig(&RCC_OscInitStructure);//初始化
	
    if(ret!=HAL_OK) return ReturnErr;
    
    //选中PLL作为系统时钟源并且配置HCLK,PCLK1和PCLK2
    RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;		//设置系统时钟时钟源为PLL
    RCC_ClkInitStructure.AHBCLKDivider=RCC_SYSCLK_DIV1;				//AHB分频系数为1
    RCC_ClkInitStructure.APB1CLKDivider=RCC_HCLK_DIV2; 				//APB1分频系数为2
    RCC_ClkInitStructure.APB2CLKDivider=RCC_HCLK_DIV1; 				//APB2分频系数为1
    ret=HAL_RCC_ClockConfig(&RCC_ClkInitStructure,FLASH_LATENCY_2);	//同时设置FLASH延时周期为2WS，也就是3个CPU周期。
		
    if(ret!=HAL_OK) return ReturnErr;
	
	return ReturnOK;
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
		HAL_NVIC_SetPriority(TIM4_IRQn,1,0);    //设置中断优先级，抢占优先级1，响应优先级0。分组为4
		HAL_NVIC_EnableIRQ(TIM4_IRQn);          //开启ITM4中断   
	}
	if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();            //使能TIM3时钟
		HAL_NVIC_SetPriority(TIM3_IRQn,3,0);    //设置中断优先级，抢占优先级1，子优先级3
		/*如果要开启定时中断，取消这个语句的注释和初始化函数中的使能更新IT注释*/
		//HAL_NVIC_EnableIRQ(TIM3_IRQn);          //开启ITM3中断
	}
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
		Timer_IT_flags._10msec_flag = 1;
		//发送10ms标志的信号量任务通知
		
		if(++Timer_IT_flags._10msec >= 10)
		{
			Timer_IT_flags._10msec = 0;
			Timer_IT_flags._100msec_flag = 1;
			Timer_IT_flags._100msec++;
			
			//发送100ms标志的信号量任务通知
		}
		
		if(Timer_IT_flags._100msec % 3 == 0)
		{
			Timer_IT_flags._300msec_flag = 1;
			
			#if SYSTEM_IWDG_ENABLE
				IWDG_Feed();//1s周期的看门狗，来喂狗了
			#endif
			
			if(is_buzzer_once)
			{
				TIM3_set_Channel_Pulse(TIM3PWM_Channel_4,80.0); //打开蜂鸣器
				is_buzzer_once = 0;
				is_runOnce4biOnce = 0;
			}else{
				if(!is_runOnce4biOnce)
				{
					TIM3_set_Channel_Pulse(TIM3PWM_Channel_4,0); //关闭蜂鸣器
					is_runOnce4biOnce = 1;
				}
			}
			
			//发送300ms标志的信号量任务通知
		}
		
		if(Timer_IT_flags._100msec >= 10)
		{
			Timer_IT_flags._100msec = 0;
			Timer_IT_flags._1sec_flag = 1;
			Timer_IT_flags._1sec++;
			
			if(is_buzzer_bibi)
			{
				if(is_reversal) TIM3_set_Channel_Pulse(TIM3PWM_Channel_4,80.0); //打开蜂鸣器
				else TIM3_set_Channel_Pulse(TIM3PWM_Channel_4,0); //关闭蜂鸣器
				is_runOnce4bibi = 0;
				is_reversal = ~is_reversal;
			}else{
				if(!is_runOnce4bibi)
				{
					TIM3_set_Channel_Pulse(TIM3PWM_Channel_4,0);//关闭蜂鸣器
					is_runOnce4bibi = 1;
				}
				
			}
			//发送1s标志的信号量任务通知
		}
		
		if(Timer_IT_flags._1sec >= 60)
		{
            Timer_IT_flags._1sec = 0;
			Timer_IT_flags._1min_flag = 1;
			Timer_IT_flags._1min++;
			
			//发送1min标志的信号量任务通知
			
			if((Timer_IT_flags._1min > 666)){
			Timer_IT_flags._1min = 0;
			//_OutofTime_Running_flag = 1;
			//CTRL_DCDC_ON_OFF = DCDC_OFF;
			}//超过时间，可以设定强制关闭
		}
    }
	
	if(htim==(&TIM3_Handler))
    {
        //LED1=!LED1;
    }
}


#if SYSTEM_UART1_ENABLE||SYSTEM_UART2_ENABLE||SYSTEM_UART3_ENABLE

UART_HandleTypeDef UART1_Handler,UART2_Handler,UART3_Handler; //UART句柄
u8 aRxBuffer1[RXBUFFERSIZE],aRxBuffer2[RXBUFFERSIZE],aRxBuffer3[RXBUFFERSIZE];//HAL库使用的串口接收缓冲


/*printf函数和printf_uart会调用此函数发送单个字节，需填入串口发送，只用printf_uart就行了*/
void _putchar(char character)
{
	// send char to console etc.
	//如果使用RTOS，这里是否应该停止其调度器，在结束处再打开
	switch(uart2sent)/*uart2sent全局变量，用于判断发送到哪一个串口*/
	{
		case UART1:/*发往串口1*/
			HAL_UART_Transmit(&UART1_Handler, (uint8_t *)&character, 1, 0x12C); //超时时间为0x12C，为300ms
			break;
		case UART2:/*发往串口2*/
			HAL_UART_Transmit(&UART2_Handler, (uint8_t *)&character, 1, 0x12C);
			break;
		case UART3:/*发往串口3*/
			HAL_UART_Transmit(&UART3_Handler, (uint8_t *)&character, 1, 0x12C);
			break;
		default:/*默认发往串口1*/
			HAL_UART_Transmit(&UART1_Handler, (uint8_t *)&character, 1, 0x12C);
			break;
	}
	uart2sent = 0; /*恢复*/
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
	HAL_UART_Init(&UART1_Handler);					    		//HAL_UART_Init()会使能UART1
	
	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer1, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
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
		__HAL_RCC_AFIO_CLK_ENABLE();
		#if SYSTEM_UART1_REMAP_ENABLE
			__HAL_AFIO_REMAP_USART1_ENABLE();
			__HAL_RCC_GPIOB_CLK_ENABLE();
			GPIO_Initure.Pin=GPIO_PIN_6;			//PB6 TX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
			GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PB6

			GPIO_Initure.Pin=GPIO_PIN_7;			//PB7 RX
			GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;	//模式要设置为复用输入模式！	
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PB7
		#else 
			__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
			GPIO_Initure.Pin=GPIO_PIN_9;			//PA9 TX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
			GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA9

			GPIO_Initure.Pin=GPIO_PIN_10;			//PA10 RX
			GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;	//模式要设置为复用输入模式！	
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA10
		#endif
		
		HAL_NVIC_SetPriority(USART1_IRQn,3,0);			//抢占优先级3，子优先级0
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//使能USART1中断通道
	}
	
	if(huart->Instance==USART2)//如果是串口2，进行串口2 MSP初始化
	{
		__HAL_RCC_USART2_CLK_ENABLE();			//使能USART2时钟
		__HAL_RCC_AFIO_CLK_ENABLE();
		#if SYSTEM_UART2_REMAP_ENABLE
			__HAL_AFIO_REMAP_USART2_ENABLE();
			__HAL_RCC_GPIOD_CLK_ENABLE();
			GPIO_Initure.Pin=GPIO_PIN_5;			//PD5 TX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
			GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
			HAL_GPIO_Init(GPIOD,&GPIO_Initure);	   	//初始化PD5

			GPIO_Initure.Pin=GPIO_PIN_6;			//PD6 RX
			GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;	//模式要设置为复用输入模式！	
			HAL_GPIO_Init(GPIOD,&GPIO_Initure);	   	//初始化PD6
		#else 
			__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
			GPIO_Initure.Pin=GPIO_PIN_2;			//PA2 TX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
			GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA2

			GPIO_Initure.Pin=GPIO_PIN_3;			//PA3 RX
			GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;	//模式要设置为复用输入模式！	
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA3
		#endif
		
		HAL_NVIC_SetPriority(USART2_IRQn,4,0);
		HAL_NVIC_EnableIRQ(USART2_IRQn);				//使能USART2中断通道
	}
	
	if(huart->Instance==USART3)//如果是串口3，进行串口3 MSP初始化
	{
		__HAL_RCC_USART3_CLK_ENABLE();			//使能USART3时钟
		__HAL_RCC_AFIO_CLK_ENABLE();
		#if SYSTEM_UART3_REMAP_ENABLE
			__HAL_AFIO_REMAP_USART3_ENABLE();
			__HAL_RCC_GPIOD_CLK_ENABLE();
			GPIO_Initure.Pin=GPIO_PIN_8;			//PD8 TX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
			GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
			HAL_GPIO_Init(GPIOD,&GPIO_Initure);	   	//初始化PD8

			GPIO_Initure.Pin=GPIO_PIN_9;			//PD9 RX
			GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;	//模式要设置为复用输入模式！	
			HAL_GPIO_Init(GPIOD,&GPIO_Initure);	   	//初始化PD9
		#else 
			__HAL_RCC_GPIOB_CLK_ENABLE();			//使能GPIOB时钟
			GPIO_Initure.Pin=GPIO_PIN_10;			//PB10 TX
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
			GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PB10

			GPIO_Initure.Pin=GPIO_PIN_11;			//PB11 RX
			GPIO_Initure.Mode=GPIO_MODE_AF_INPUT;	//模式要设置为复用输入模式！	
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//初始化PB11
		#endif
		
		HAL_NVIC_SetPriority(USART3_IRQn,4,0);
		HAL_NVIC_EnableIRQ(USART3_IRQn);				//使能USART3中断通道
	}
}




char USART1_RX_BUF[USART1_RX_BUF_MaxNum] = "0"; /*串口1的模式1数据接受区*/
#define USART1_RX_DONE_mask 0x8000
#define USART1_RX_MODE_mask 0x4000
#define USART1_RX_Rec_r_mask 0x2000
#define USART1_RX_Num_mask 0x1fff /*0001 1111 1111 1111*/
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
#define USART1_SetMode(x) USART1_RX_CONFIG |= (((u16)x)<<15) /*在任务中设置串口1接收协议*/
#define USART1_SetDone USART1_RX_CONFIG |= USART1_RX_DONE_mask /*设置串口1接收完成标志位*/
#define USART1_SetUnDone USART1_RX_CONFIG &= (~(USART1_RX_DONE_mask | USART1_RX_Rec_r_mask)) /*在任务中处理完后对串口1标志位进行复位*/
#define USART1_RX_ByteNum (USART1_RX_CONFIG & USART1_RX_Num_mask)

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)//如果是串口1
	{ 
		if(!(USART1_RX_CONFIG & USART1_RX_MODE_mask)) /*协议0：只接受以'\r\n'结尾的数据：从USART1_RX_BUF[]接受数据*/
		{ 
			if(!(USART1_RX_CONFIG & USART1_RX_DONE_mask)) /*如果接收未完成*/
			{
				if(USART1_RX_CONFIG & USART1_RX_Rec_r_mask) /*如果接收到了\r*/
				{
					if(aRxBuffer1[0] == '\n') /*如果接受到了\r\n*/
					{
						USART1_SetDone;
						USART1_RX_CONFIG++; /*保证接受完成后的 接受数据字节计数 算上 '\r\n' 两个*/
					}else{USART1_SetUnDone;}
				}else
				{
					if(aRxBuffer1[0] == '\r') {USART1_RX_CONFIG |= USART1_RX_Rec_r_mask;USART1_RX_CONFIG++;}
					else{
						USART1_RX_BUF[USART1_RX_CONFIG & USART1_RX_Num_mask] = aRxBuffer1[0];
						USART1_RX_CONFIG++;
						if((USART1_RX_CONFIG & USART1_RX_Num_mask) >= (USART1_RX_BUF_MaxNum - 1)) USART1_SetDone;
					}

				}
			}
		}else 								/*协议1：以FIFO先进先出的环形缓存实现接受区，无协议，需要及时接受，如果缓存满了则新入数据丢失*/
		{									/*从ReadDataFromRingbuff(&RingBuff_forUSART1);RingBuff_forUSART1.data接收数据，注意要检查是否成功读出！*/
			WriteDataToRingbuff(&RingBuff_forUSART1,aRxBuffer1[0]);
			//if(!(USART1_RX_CONFIG & USART1_RX_DONE_mask)) {;}/*如果接收未完成*/
		}
	}else if(huart->Instance==USART2)//如果是串口2
	{
		
	}else if(huart->Instance==USART3)//如果是串口3
	{
		
	}
}

#if SYSTEM_UART1_ENABLE
void USART1_IRQHandler(void)	//串口1中断服务程序
{ 
	u32 timeout=0;
	#if SYSTEM_SUPPORT_OS	 	//使用OS
		//OSIntEnter();    
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
	#endif
}
#endif

#if SYSTEM_UART2_ENABLE
void USART2_IRQHandler(void)	//串口2中断服务程序
{ 
	u32 timeout=0;
	#if SYSTEM_SUPPORT_OS	 	//使用OS
		//OSIntEnter();    
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
	#endif
}
#endif

#if SYSTEM_UART3_ENABLE
void USART3_IRQHandler(void)	//串口3中断服务程序
{ 
	u32 timeout=0;
	#if SYSTEM_SUPPORT_OS	 	//使用OS
		//OSIntEnter();    
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
	#endif
}
#endif

#endif

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
	TIM4_Handler.Instance=TIM4;                          		//通用定时器4
    TIM4_Handler.Init.Prescaler=(360-1);                     	//分频系数
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    		//向上计数器
    TIM4_Handler.Init.Period=(2000-1);                        	//自动装载值
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;		//时钟分频因子
    HAL_TIM_Base_Init(&TIM4_Handler);
    
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
	TIM3_Handler.Instance=TIM3;          	//定时器3
	TIM3_Handler.Init.Prescaler=prsc;       //定时器分频
	TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
	TIM3_Handler.Init.Period=arr;          //自动重装载值
	TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&TIM3_Handler);
	/*如果要开启定时中断，取消这个语句的注释和HAL_TIM_Base_MspInit中的TIM3的IRQ注释*/
	//HAL_TIM_Base_Start_IT(&TIM3_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE
	HAL_TIM_PWM_Init(&TIM3_Handler);       //初始化PWM
    
	if(STSTEM_TIM3PWM_CHANNEL_ENABLE & B0000_0010)
	{
		TIM3_CH2Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
		TIM3_CH2Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
		/*输出比较极性为低，对于PWM1模式时，当计数值小于比较值时输出低电平，PWM2与PWM1正好相反，注意规范定义低电平有效，高电平截止*/
		/*即当计数值小于比较值时输出有效，器件可以工作，即这个比较值越大，器件工作时间越长，即 占空比 越大*/
		TIM3_CH2Handler.OCPolarity=TIM_OCPOLARITY_LOW;
		HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH2Handler,TIM_CHANNEL_2);//配置TIM3通道2
		HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_2);//开启PWM通道2
	}
	if(STSTEM_TIM3PWM_CHANNEL_ENABLE & B0000_0001)
	{
		TIM3_CH1Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
		TIM3_CH1Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
		TIM3_CH1Handler.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低 
		HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH1Handler,TIM_CHANNEL_1);//配置TIM3通道1
		HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_1);//开启PWM通道1
	}
	if(STSTEM_TIM3PWM_CHANNEL_ENABLE & B0000_0100)
	{
		TIM3_CH3Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
		TIM3_CH3Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
		TIM3_CH3Handler.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低 
		HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH3Handler,TIM_CHANNEL_3);//配置TIM3通道3
		HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_3);//开启PWM通道3
	}
	if(STSTEM_TIM3PWM_CHANNEL_ENABLE & B0000_1000)
	{
		TIM3_CH4Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
		TIM3_CH4Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
		TIM3_CH4Handler.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低 
		HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH4Handler,TIM_CHANNEL_4);//配置TIM3通道4
		HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_4);//开启PWM通道4
	}

}
//void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)已经写在上面

//定时器底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_PWM_Init()调用
//htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_Initure;
	
    if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();			//使能定时器3
		
		if(STSTEM_TIM3PWM_REMAP_ENABLE)	//如果全部重映射
		{
			__HAL_AFIO_REMAP_TIM3_ENABLE();			//TIM3通道引脚全部重映射使能，具体怎么映射看库函数注释或sys.h里
			__HAL_RCC_GPIOC_CLK_ENABLE();			//开启GPIOC时钟
			if(STSTEM_TIM3PWM_CHANNEL_ENABLE & B0000_0001)	GPIO_Initure.Pin=GPIO_PIN_6;	//如果开启通道1，PC6
			if(STSTEM_TIM3PWM_CHANNEL_ENABLE & B0000_0010)	GPIO_Initure.Pin=GPIO_PIN_7;	//如果开启通道2，PC7
			if(STSTEM_TIM3PWM_CHANNEL_ENABLE & B0000_0100)	GPIO_Initure.Pin=GPIO_PIN_8;	//如果开启通道3，PC8
			if(STSTEM_TIM3PWM_CHANNEL_ENABLE & B0000_1000)	GPIO_Initure.Pin=GPIO_PIN_6;	//如果开启通道4，PC9
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
			GPIO_Initure.Pull=GPIO_PULLUP;          //上拉，所以硬件上最好低电平有效，高电平截止
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
			HAL_GPIO_Init(GPIOC,&GPIO_Initure); 
		}else if(STSTEM_TIM3PWM_REMAP_PARTIAL)	//如果部分重映射
		{
			__HAL_AFIO_REMAP_TIM3_PARTIAL();		//TIM3通道引脚部分重映射使能，具体怎么映射看库函数注释或sys.h里
			__HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
			if(STSTEM_TIM3PWM_CHANNEL_ENABLE & B0000_0001)	GPIO_Initure.Pin=GPIO_PIN_4;	//如果开启通道1，PB4
			if(STSTEM_TIM3PWM_CHANNEL_ENABLE & B0000_0010)	GPIO_Initure.Pin=GPIO_PIN_5;	//如果开启通道2，PB5
			if(STSTEM_TIM3PWM_CHANNEL_ENABLE & B0000_0100)	GPIO_Initure.Pin=GPIO_PIN_0;	//如果开启通道3，PB0
			if(STSTEM_TIM3PWM_CHANNEL_ENABLE & B0000_1000)	GPIO_Initure.Pin=GPIO_PIN_1;	//如果开启通道4，PB1
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
			GPIO_Initure.Pull=GPIO_PULLUP;          //上拉，所以硬件上最好低电平有效，高电平截止
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
			HAL_GPIO_Init(GPIOB,&GPIO_Initure); 			
		}else{									//没有重映射
			
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
			GPIO_Initure.Pull=GPIO_PULLUP;          //上拉，所以硬件上最好低电平有效，高电平截止
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
			
			if(STSTEM_TIM3PWM_CHANNEL_ENABLE & B0000_0001)	
			{
				__HAL_RCC_GPIOA_CLK_ENABLE();GPIO_Initure.Pin=GPIO_PIN_6;	//如果开启通道1，PA6
				HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
			}
			if(STSTEM_TIM3PWM_CHANNEL_ENABLE & B0000_0010)
			{
				__HAL_RCC_GPIOA_CLK_ENABLE();GPIO_Initure.Pin=GPIO_PIN_7;	//如果开启通道2，PA7
				HAL_GPIO_Init(GPIOA,&GPIO_Initure); 
			}
			if(STSTEM_TIM3PWM_CHANNEL_ENABLE & B0000_0100)
			{
				__HAL_RCC_GPIOB_CLK_ENABLE();GPIO_Initure.Pin=GPIO_PIN_0;	//如果开启通道3，PB0
				HAL_GPIO_Init(GPIOB,&GPIO_Initure); 
			}
			if(STSTEM_TIM3PWM_CHANNEL_ENABLE & B0000_1000)
			{
				__HAL_RCC_GPIOB_CLK_ENABLE();GPIO_Initure.Pin=GPIO_PIN_1;	//如果开启通道4，PB1
				HAL_GPIO_Init(GPIOB,&GPIO_Initure);
			}
		}
	}
}

//设置TIM3通道的占空比
//percent:占空比百分数
void TIM3_set_Channel_Pulse(u8 channel,float percent)
{
	float compare;
	if(percent < 0) percent = 0;
	if(percent > 100) percent = 100.0;
	percent /= 100.0;
	compare = (float)arr * percent;
	switch(channel)
	{
		case TIM3PWM_Channel_1: TIM3->CCR1=(u32)compare;break;
		case TIM3PWM_Channel_2: TIM3->CCR2=(u32)compare;break;
		case TIM3PWM_Channel_3: TIM3->CCR3=(u32)compare;break;
		case TIM3PWM_Channel_4: TIM3->CCR4=(u32)compare;break;
		default:break;
	}
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Handler);
}

//回调函数，定时器中断服务函数调用的void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)在上面
#endif

#if SYSTEM_IWDG_ENABLE
	IWDG_HandleTypeDef IWDG_Handler; //独立看门狗句柄
	//初始化独立看门狗
	//prer:分频数:IWDG_PRESCALER_4~IWDG_PRESCALER_256
	//rlr:自动重装载值,0~0XFFF.
	//时间计算(大概):Tout=((4*2^prer)*rlr)/32 (ms)
	void sys_IWDG_ENABLE(void)
	{
		IWDG_Handler.Instance=IWDG;
		IWDG_Handler.Init.Prescaler=IWDG_PRESCALER_64;	//设置IWDG分频系数
		IWDG_Handler.Init.Reload=500;		//重装载值
		HAL_IWDG_Init(&IWDG_Handler);		//初始化IWDG,默认会开启独立看门狗	
	}
	//喂独立看门狗
	void IWDG_Feed(void)
	{   
		HAL_IWDG_Refresh(&IWDG_Handler); 	//喂狗
	}
#endif
#if SYSTEM_MCO_PA8_OUT
void sys_MCO_Out_Enable(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1); 
	HAL_RCC_EnableCSS(); // 使能CSS功能，优先使用外部晶振，内部时钟源为备用
	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
#endif

#if SYSTEM_RTC_ENABLE
/*__________RTC初始化_____________*/
RTC_HandleTypeDef RTC_Handler;		//RTC句柄
_calendar_obj calendar;				//RTC结构体
/*初始化RTC时钟,同时检测时钟是否工作正常
BKP->DR1用于保存是否第一次配置的设置*/
uint8_t sys_RTC_Enable(void)
{
	RTC_Handler.Instance=RTC; 
	RTC_Handler.Init.AsynchPrediv=32767; 	//时钟周期设置(有待观察,看是否跑慢了?)理论值：32767	
	if(HAL_RTC_Init(&RTC_Handler)!=HAL_OK) return ReturnErr;
	
	if(HAL_RTCEx_BKUPRead(&RTC_Handler,RTC_BKP_DR1)!=0X5050)//是否第一次配置
	{
		RTC_Set(2049,10,1,17,7,0); //设置日期和时间，2049年10月1日，17点07分0秒		 									  
		HAL_RTCEx_BKUPWrite(&RTC_Handler,RTC_BKP_DR1,0X5050);//标记已经初始化过了
	 	//printf("FIRST TIME\n");
	}
	
	__HAL_RTC_ALARM_ENABLE_IT(&RTC_Handler,RTC_IT_SEC); 	//允许秒中断
	__HAL_RTC_ALARM_ENABLE_IT(&RTC_Handler,RTC_IT_ALRA); 	//允许闹钟中断
    HAL_NVIC_SetPriority(RTC_IRQn,0x01,0x02); 				//抢占优先级1,子优先级2
    HAL_NVIC_EnableIRQ(RTC_IRQn);	 
	
	RTC_Get();//更新时间
	return ReturnOK;
}

/*RTC底层驱动，时钟配置
此函数会被HAL_RTC_Init()调用
hrtc:RTC句柄*/
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    __HAL_RCC_PWR_CLK_ENABLE();	//使能电源时钟PWR
	HAL_PWR_EnableBkUpAccess();	//取消备份区域写保护
	__HAL_RCC_BKP_CLK_ENABLE();	//使能BSP时钟
	
	RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_LSE;//LSE配置
    RCC_OscInitStruct.PLL.PLLState=RCC_PLL_NONE;
    RCC_OscInitStruct.LSEState=RCC_LSE_ON;                  //RTC使用LSE
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    PeriphClkInitStruct.PeriphClockSelection=RCC_PERIPHCLK_RTC;//外设为RTC
    PeriphClkInitStruct.RTCClockSelection=RCC_RTCCLKSOURCE_LSE;//RTC时钟源为LSE
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
        
    __HAL_RCC_RTC_ENABLE();//RTC时钟使能
}
/*__________RTC读存_____________*/

//RTC闹钟中断的回调函数
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	if(hrtc->Instance == RTC)
	{
		//ptintf("RTC_AlarmIRQ!!!");
	}
	
}
//RTC时钟中断
//每秒触发一次  	 
void RTC_IRQHandler(void)
{		 
	if(__HAL_RTC_ALARM_GET_FLAG(&RTC_Handler,RTC_FLAG_SEC)!=RESET)  //秒中断
	{
	 	__HAL_RTC_ALARM_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_SEC); 		//清除秒中断
		RTC_Get();				//更新时间   
		//LED1=!LED1;													//LED1翻转
	}		

	if(__HAL_RTC_ALARM_GET_FLAG(&RTC_Handler,RTC_FLAG_SEC)!=RESET)  //闹钟中断
	{
		__HAL_RTC_ALARM_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_ALRAF); 	//清除闹钟中断   
		RTC_Get();				//更新时间   
		//printf("ALARM A!\r\n");
	}	
	__HAL_RTC_ALARM_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_OW); 		//清除溢出  	    						 	   	 
}

//判断是否是闰年函数
//月份   1  2  3  4  5  6  7  8  9  10 11 12
//闰年   31 29 31 30 31 30 31 31 30 31 30 31
//非闰年 31 28 31 30 31 30 31 31 30 31 30 31
//year:年份
//返回值:该年份是不是闰年.1,是.0,不是
u8 Is_Leap_Year(u16 year)
{			  
	if(year%4==0) //必须能被4整除
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//如果以00结尾,还要能被400整除 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}	

//设置时钟
//把输入的时钟转换为秒钟
//以1970年1月1日为基准
//1970~2099年为合法年份
//返回值:0,成功;其他:错误代码.
//月份数据表											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表	  
//平年的月份日期表
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
//syear,smon,sday,hour,min,sec：年月日时分秒
//返回值：设置结果。0，成功；1，失败。
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
//	RTC_DateTypeDef RTC_DateStructure;
//	RTC_TimeTypeDef RTC_TimeStructure;
	
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)	//把所有年份的秒钟相加
	{
		if(Is_Leap_Year(t))seccount+=31622400;//闰年的秒钟数
		else seccount+=31536000;			  //平年的秒钟数
	}
	smon-=1;
	for(t=0;t<smon;t++)	   //把前面月份的秒钟数相加
	{
		seccount+=(u32)mon_table[t]*86400;//月份秒钟数相加
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//闰年2月份增加一天的秒钟数	   
	}
	seccount+=(u32)(sday-1)*86400;//把前面日期的秒钟数相加 
	seccount+=(u32)hour*3600;//小时秒钟数
    seccount+=(u32)min*60;	 //分钟秒钟数
	seccount+=sec;//最后的秒钟加上去
	
//	RTC_DateStructure.Year=syear;
//	RTC_DateStructure.Month=smon;
//	RTC_DateStructure.Date=sday;
//	HAL_RTC_SetDate(&RTC_Handler,&RTC_DateStructure,RTC_FORMAT_BIN);
//	
//	RTC_TimeStructure.Hours=hour;
//	RTC_TimeStructure.Minutes=min;
//	RTC_TimeStructure.Seconds=sec;
//	HAL_RTC_SetTime(&RTC_Handler,&RTC_TimeStructure,RTC_FORMAT_BIN);
	

	//设置时钟
    RCC->APB1ENR|=1<<28;//使能电源时钟
    RCC->APB1ENR|=1<<27;//使能备份时钟
	PWR->CR|=1<<8;    //取消备份区写保护
	//上面三步是必须的!
	RTC->CRL|=1<<4;   //允许配置 
	RTC->CNTL=seccount&0xffff;
	RTC->CNTH=seccount>>16;
	RTC->CRL&=~(1<<4);//配置更新
	while(!(RTC->CRL&(1<<5)));//等待RTC寄存器操作完成 
	
	RTC_Get();//设置完之后更新一下数据 	
	return 0;	    
}
//初始化闹钟		  
//以1970年1月1日为基准
//1970~2099年为合法年份
//syear,smon,sday,hour,min,sec：闹钟的年月日时分秒   
//返回值:0,成功;其他:错误代码.
u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)	//把所有年份的秒钟相加
	{
		if(Is_Leap_Year(t))seccount+=31622400;//闰年的秒钟数
		else seccount+=31536000;			  //平年的秒钟数
	}
	smon-=1;
	for(t=0;t<smon;t++)	   //把前面月份的秒钟数相加
	{
		seccount+=(u32)mon_table[t]*86400;//月份秒钟数相加
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//闰年2月份增加一天的秒钟数	   
	}
	seccount+=(u32)(sday-1)*86400;//把前面日期的秒钟数相加 
	seccount+=(u32)hour*3600;//小时秒钟数
    seccount+=(u32)min*60;	 //分钟秒钟数
	seccount+=sec;//最后的秒钟加上去 			    
	//设置时钟
    RCC->APB1ENR|=1<<28;//使能电源时钟
    RCC->APB1ENR|=1<<27;//使能备份时钟
	PWR->CR|=1<<8;    //取消备份区写保护
	//上面三步是必须的!
	RTC->CRL|=1<<4;   //允许配置 
	RTC->ALRL=seccount&0xffff;
	RTC->ALRH=seccount>>16;
	RTC->CRL&=~(1<<4);//配置更新
	while(!(RTC->CRL&(1<<5)));//等待RTC寄存器操作完成  
	return 0;	    
}
//得到当前的时间，结果保存在calendar结构体里面
//返回值:0,成功;其他:错误代码.
u8 RTC_Get(void)
{
	static u16 daycnt=0;
	u32 timecount=0; 
	u32 temp=0;
	u16 temp1=0;	  
 	timecount=RTC->CNTH;//得到计数器中的值(秒钟数)
	timecount<<=16;
	timecount+=RTC->CNTL;			 

 	temp=timecount/86400;   //得到天数(秒钟数对应的)
	if(daycnt!=temp)//超过一天了
	{	  
		daycnt=temp;
		temp1=1970;	//从1970年开始
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))//是闰年
			{
				if(temp>=366)temp-=366;//闰年的秒钟数
				else break;  
			}
			else temp-=365;	  //平年 
			temp1++;  
		}   
		calendar.w_year=temp1;//得到年份
		temp1=0;
		while(temp>=28)//超过了一个月
		{
			if(Is_Leap_Year(calendar.w_year)&&temp1==1)//当年是不是闰年/2月份
			{
				if(temp>=29)temp-=29;//闰年的秒钟数
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//平年
				else break;
			}
			temp1++;  
		}
		calendar.w_month=temp1+1;	//得到月份
		calendar.w_date=temp+1;  	//得到日期 
	}
	temp=timecount%86400;     		//得到秒钟数   	   
	calendar.hour=temp/3600;     	//小时
	calendar.min=(temp%3600)/60; 	//分钟	
	calendar.sec=(temp%3600)%60; 	//秒钟
	calendar.week=RTC_Get_Week(calendar.w_year,calendar.w_month,calendar.w_date);//获取星期   
	return 0;
}	 
//获得现在是星期几
//功能描述:输入公历日期得到星期(只允许1901-2099年)
//year,month,day：公历年月日 
//返回值：星期号																						 
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{	
	u16 temp2;
	u8 yearH,yearL;
	
	yearH=year/100;	yearL=year%100; 
	// 如果为21世纪,年份数加100  
	if (yearH>19)yearL+=100;
	// 所过闰年数只算1900年之后的  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7);
}

#endif


#if SYSTEM_CRC_ENABLE
CRC_HandleTypeDef hcrc;
void sys_CRC_ENABLE(void)
{
	hcrc.Instance = CRC;
	HAL_CRC_Init(&hcrc);
}
void HAL_CRC_MspInit(CRC_HandleTypeDef* hcrc)
{
	if(hcrc->Instance==CRC)
	{
	/* 外设时钟使能 */
	__HAL_RCC_CRC_CLK_ENABLE();
	}
}
/* 私有变量 ------------------------------------------------------------------*/
const uint32_t aDataBuffer[BUFFER_SIZE] =
  {
    0x00001021, 0x20423063, 0x408450a5, 0x60c670e7, 0x9129a14a, 0xb16bc18c,
    0xd1ade1ce, 0xf1ef1231, 0x32732252, 0x52b54294, 0x72f762d6, 0x93398318,
    0xa35ad3bd, 0xc39cf3ff, 0xe3de2462, 0x34430420, 0x64e674c7, 0x44a45485,
    0xa56ab54b, 0x85289509, 0xf5cfc5ac, 0xd58d3653, 0x26721611, 0x063076d7,
    0x569546b4, 0xb75ba77a, 0x97198738, 0xf7dfe7fe, 0xc7bc48c4, 0x58e56886,
    0x78a70840, 0x18612802, 0xc9ccd9ed, 0xe98ef9af, 0x89489969, 0xa90ab92b,
    0x4ad47ab7, 0x6a961a71, 0x0a503a33, 0x2a12dbfd, 0xfbbfeb9e, 0x9b798b58,
    0xbb3bab1a, 0x6ca67c87, 0x5cc52c22, 0x3c030c60, 0x1c41edae, 0xfd8fcdec,
    0xad2abd0b, 0x8d689d49, 0x7e976eb6, 0x5ed54ef4, 0x2e321e51, 0x0e70ff9f,
    0xefbedfdd, 0xcFFCbf1b, 0x9f598f78, 0x918881a9, 0xb1caa1eb, 0xd10cc12d,
    0xe16f1080, 0x00a130c2, 0x20e35004, 0x40257046, 0x83b99398, 0xa3fbb3da,
    0xc33dd31c, 0xe37ff35e, 0x129022f3, 0x32d24235, 0x52146277, 0x7256b5ea,
    0x95a88589, 0xf56ee54f, 0xd52cc50d, 0x34e224c3, 0x04817466, 0x64475424,
    0x4405a7db, 0xb7fa8799, 0xe75ff77e, 0xc71dd73c, 0x26d336f2, 0x069116b0,
    0x76764615, 0x5634d94c, 0xc96df90e, 0xe92f99c8, 0xb98aa9ab, 0x58444865,
    0x78066827, 0x18c008e1, 0x28a3cb7d, 0xdb5ceb3f, 0xfb1e8bf9, 0x9bd8abbb,
    0x4a755a54, 0x6a377a16, 0x0af11ad0, 0x2ab33a92, 0xed0fdd6c, 0xcd4dbdaa,
    0xad8b9de8, 0x8dc97c26, 0x5c644c45, 0x3ca22c83, 0x1ce00cc1, 0xef1fff3e,
    0xdf7caf9b, 0xbfba8fd9, 0x9ff86e17, 0x7e364e55, 0x2e933eb2, 0x0ed11ef0
  };
/* Expected CRC Value */
uint32_t uwExpectedCRCValue = 0x379E9F06;
#endif


#if SYSTEM_SUPPORT_OS
extern void xPortSysTickHandler(void);
//systick中断服务函数
void SysTick_Handler(void)
{
	HAL_IncTick();
	
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
    {
        xPortSysTickHandler();	
    }
}
#endif

/*__________延时函数_____________*/
/********************************
*描述：延时函数定义，自动根据是否使用RTOS编译
********************************/
static u32 fac_us=0;							//us延时倍乘数.
#if SYSTEM_SUPPORT_OS
	static u16 fac_ms=0;							//ms延时倍乘数,在ucos下,代表每个节拍的ms数
#endif

#if SYSTEM_SUPPORT_OS		/*使用FreeRTOS的情况*/
//初始化延迟函数
//SYSTICK的时钟固定为AHB时钟，基础例程里面SYSTICK时钟频率为AHB/8
//这里为了兼容FreeRTOS，所以将SYSTICK的时钟频率改为AHB的频率！
//SYSCLK:系统时钟频率
void delay_init(void)
{
	u32 reload;
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//SysTick频率为HCLK
	fac_us=SystemCoreClock/1000000;				//不论是否使用OS,fac_us都需要使用
	reload=SystemCoreClock/1000000;				//每秒钟的计数次数 单位为M  
	reload*=1000000/configTICK_RATE_HZ;			//根据configTICK_RATE_HZ设定溢出时间
												//reload为24位寄存器,最大值:16777216,在72M下,约合0.233s左右	
	fac_ms=1000/configTICK_RATE_HZ;				//代表OS可以延时的最少单位	   

	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
	SysTick->LOAD=reload; 						//每1/configTICK_RATE_HZ秒中断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//开启SYSTICK    
}								    


//延时nus
//nus:要延时的us数.	
//nus:0~204522252(最大值即2^32/fac_us@fac_us=168)	    								   
void delay_us(uint32_t nus)
{	
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD的值	    	 
	ticks=nus*fac_us; 						//需要的节拍数 
	told=SysTick->VAL;        				//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
		}  
	};										    
}  
//延时nms
//nms:要延时的ms数
//nms:0~65535
void delay_ms(uint16_t nms)
{	
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
	{		
		if(nms>=fac_ms)						//延时的时间大于OS的最少时间周期 
		{ 
   			vTaskDelay(nms/fac_ms);	 		//FreeRTOS延时，即当前任务阻塞式延时
		}
		nms%=fac_ms;						//OS已经无法提供这么小的延时了,采用普通方式延时    
	}
	delay_us((u32)(nms*1000));				//普通方式延时
}

//延时nms,不会引起任务调度
//nms:要延时的ms数
void delay_xms(uint16_t nms)
{
	u32 i;
	for(i=0;i<nms;i++) delay_us(1000);
}


#else						/*不使用FreeRTOS的情况*/

void delay_init(void)
{
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//SysTick频率为HCLK
	fac_us=72;						//不论是否使用OS,fac_us都需要使用
}
//延时nus
//nus为要延时的us数.	
//nus:0~190887435(最大值即2^32/fac_us@fac_us=22.5)	 
void delay_us(uint32_t nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD的值	    	 
	ticks=nus*fac_us; 						//需要的节拍数 
	told=SysTick->VAL;        				//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
		}  
	};
}

//延时nms
//nms:要延时的ms数
void delay_ms(uint16_t nms)
{
	u32 i;
	for(i=0;i<nms;i++) delay_us(1000);
}
#endif

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


