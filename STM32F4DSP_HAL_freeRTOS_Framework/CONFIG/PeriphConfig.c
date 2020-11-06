#include "PeriphConfig.h"


/*_____________________________________\\\                               ///____________________________________________*
 *_________________________________________________用户GPIO配置_________________________________________________________*
 *_____________________________________///                               \\\____________________________________________*/
/*STEP 1:去.h文件里定义都有什么器件*/

/*STEP 2:定义一共有多少个器件*/
#define devicesNum	3

/*STEP 3:定义每个器件所用到的IO和其配置（只是定义，但可以分别选择初始化或者不初始化）*/
/*参数说明：参数必须在以下列举中选
	PIN：	GPIO_PIN_0~GPIO_PIN_15
	MODE：	输入		GPIO_MODE_INPUT、GPIO_MODE_ANALOG							
			输出		GPIO_MODE_OUTPUT_PP、GPIO_MODE_OUTPUT_OD、GPIO_MODE_AF_PP、GPIO_MODE_AF_OD		（AF不要用，外设IO交给外设初始化函数解决）
			外部中断	GPIO_MODE_IT_RISING、GPIO_MODE_IT_FALLING、GPIO_MODE_IT_RISING_FALLING  		选用IT表示启用EXTI，最多16线EXTI，如PA0和PB0共用EXTI0
	上下拉：	GPIO_NOPULL、GPIO_PULLUP、GPIO_PULLDOWN
	翻转速度：	GPIO_SPEED_FREQ_LOW、GPIO_SPEED_FREQ_MEDIUM、GPIO_SPEED_FREQ_HIGH、GPIO_SPEED_FREQ_VERY_HIGH
	GPIOx：		GPIOA~GPIOE、GPIOF、GPIOG、GPIOH、GPIOI
	默认状态:	GPIO_PIN_SET（1u）或GPIO_PIN_RESET（0u）
	EXTI优先级：0~15
	启否EXTI：	TRUE或者FALSE
*/
/*指示灯IO定义*/
GPIO_Init_Struct TestLED_IO_Struct[] = 
{	/*	PIN				MODE			  上下拉		翻转速度		  		GPIOx 	  默认状态     EXTI优先级	启否EXTI*/
	{{GPIO_PIN_9, 	GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOF,  GPIO_PIN_SET,		15,		  FALSE},
	{{GPIO_PIN_10, 	GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOF,  GPIO_PIN_SET,		15,		  FALSE}
};
/*用于菜单的外部输入按键定义*/
GPIO_Init_Struct KEY_IO_Struct[] =
{	/*	PIN				MODE			  上下拉		翻转速度		  	GPIOx 	  默认状态     EXTI优先级	启否EXTI*/
	{{GPIO_PIN_1, GPIO_MODE_IT_FALLING, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH},	GPIOA,  GPIO_PIN_SET,		2,		  TRUE},	//key_Up
	{{GPIO_PIN_2, GPIO_MODE_IT_FALLING, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH},	GPIOA,  GPIO_PIN_SET,		2,		  TRUE}		//key_Down
};
/*LCD IO驱动时的IO定义*/
GPIO_Init_Struct LCD_IO_Struct[] = 
{	/*	PIN				MODE			  上下拉		翻转速度		  	GPIOx 	  默认状态     EXTI优先级	启否EXTI*/
	{{GPIO_PIN_3, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		15,		  FALSE},
	{{GPIO_PIN_4, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		15,		  FALSE},
	{{GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		15,		  FALSE},
	{{GPIO_PIN_6, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		15,		  FALSE},
	{{GPIO_PIN_7, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		15,		  FALSE},
	
	{{GPIO_PIN_8, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		15,		  FALSE},
	{{GPIO_PIN_9, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		15,		  FALSE},
	{{GPIO_PIN_10, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		15,		  FALSE},
	{{GPIO_PIN_11, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		15,		  FALSE},
	{{GPIO_PIN_12, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		15,		  FALSE},
	{{GPIO_PIN_13, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		15,		  FALSE},
	{{GPIO_PIN_14, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		15,		  FALSE},
	{{GPIO_PIN_15, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH},	GPIOB,  GPIO_PIN_SET,		15,		  FALSE},
};

///*I2C的IO定义*/
//GPIO_Init_Struct simuI2C_IO_Struct[] =
//{	/*	PIN				MODE			  上下拉		翻转速度		  	GPIOx 	  默认状态     EXTI优先级	启否EXTI*/
//	{{GPIO_PIN_2, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH},	GPIOE,  GPIO_PIN_SET,		2,		  TRUE},	//MPU6050 SCL
//	{{GPIO_PIN_3, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH},	GPIOE,  GPIO_PIN_SET,		2,		  TRUE} 	//MPU6050 SDA
//};

/*STEP 3.5:在这里和去.h文件里填写用于外部中断的IO的中断标志位*/
//当IO使用外部中断时，用一个标志位记录中断是否发生，把长时间的处理函数放到中断外面！
u8 key_Up_Interrupted 		= 	FALSE; 
u8 key_Down_Interrupted 	= 	FALSE;


/*STEP 3.75:去.h文件里定义IO控制的宏和改变入出模式的宏*/


/*STEP4:按照器件的数量，按照"STEP 1:定义都有什么器件"里面的顺序！完善每个器件的信息*/
Devices_Init_Struct UserDevices[devicesNum] = 
{
	{	
		.deviceIO_Struct 	= 	TestLED_IO_Struct	,	//器件IO配置结构体
		.deviceIndex 		= 	TestLED_Index		,	//器件enum格式索引
		.deviceName 		= 	"TestLED"			,	//器件名称
		.device_IOnum 		= 	2						//器件有多少个IO口
	},
	{
		.deviceIO_Struct 	= 	KEY_IO_Struct	,
		.deviceIndex		= 	KEY_Index		,
		.deviceName 		= 	"Menu Key"		,
		.device_IOnum 		= 	2
	},
	{	
		.deviceIO_Struct 	= 	LCD_IO_Struct	,
		.deviceIndex 		= 	LCD_Index		,
		.deviceName 		= 	"LCD"			,
		.device_IOnum 		= 	13				
	},
//	{	
//		.deviceIO_Struct 	= 	simuI2C_IO_Struct	,
//		.deviceIndex 		= 	simuI2C_Index		,
//		.deviceName 		= 	"simuI2C"			,
//		.device_IOnum 		= 	2					
//	}
};


/*_____________________________________\\\                               ///____________________________________________*
 *_________________________________________________以下不用动___________________________________________________________*
 *_____________________________________///                               \\\____________________________________________*/
/*器件IO配置函数*/
void deviceIO_Init(Devices_Init_Struct* Devices , enum devicesIndex_enum device2Init)
{
	u8 dIndex,iIndex;
	dIndex = (u8)device2Init;
	for(iIndex = 0;iIndex < Devices[dIndex].device_IOnum;iIndex++)	//遍历某一个器件的所有IO
	{
		//时钟
		if(Devices[dIndex].deviceIO_Struct[iIndex].GPIOx == GPIOA)
			__HAL_RCC_GPIOA_CLK_ENABLE();
		else if(Devices[dIndex].deviceIO_Struct[iIndex].GPIOx == GPIOB)
			__HAL_RCC_GPIOB_CLK_ENABLE();
		else if(Devices[dIndex].deviceIO_Struct[iIndex].GPIOx == GPIOC)
			__HAL_RCC_GPIOC_CLK_ENABLE();
		else if(Devices[dIndex].deviceIO_Struct[iIndex].GPIOx == GPIOD)
			__HAL_RCC_GPIOD_CLK_ENABLE();
		else if(Devices[dIndex].deviceIO_Struct[iIndex].GPIOx == GPIOE)
			__HAL_RCC_GPIOE_CLK_ENABLE();
		else if(Devices[dIndex].deviceIO_Struct[iIndex].GPIOx == GPIOF)
			__HAL_RCC_GPIOF_CLK_ENABLE();
		else if(Devices[dIndex].deviceIO_Struct[iIndex].GPIOx == GPIOG)
			__HAL_RCC_GPIOG_CLK_ENABLE();
		
		//填入IO配置
		HAL_GPIO_Init(Devices[dIndex].deviceIO_Struct[iIndex].GPIOx, \
					 &Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure);
		
		//如果使用EXTI，配置中断线和优先级
		if(   (Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Mode == GPIO_MODE_IT_RISING) \
			||(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Mode == GPIO_MODE_IT_FALLING) \
			||(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Mode == GPIO_MODE_IT_RISING_FALLING) )
		{		
			if(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_0)
			{	
				HAL_NVIC_SetPriority(EXTI0_IRQn, Devices[dIndex].deviceIO_Struct[iIndex].PreemptPriority, 0);
				if(Devices[dIndex].deviceIO_Struct[iIndex].isEnableExit)  HAL_NVIC_EnableIRQ(EXTI0_IRQn); else HAL_NVIC_DisableIRQ(EXTI0_IRQn);
			}else if(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_1)
			{
				HAL_NVIC_SetPriority(EXTI1_IRQn, Devices[dIndex].deviceIO_Struct[iIndex].PreemptPriority, 0);
				if(Devices[dIndex].deviceIO_Struct[iIndex].isEnableExit)  HAL_NVIC_EnableIRQ(EXTI1_IRQn); else HAL_NVIC_DisableIRQ(EXTI1_IRQn);
			}else if(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_2)
			{
				HAL_NVIC_SetPriority(EXTI2_IRQn, Devices[dIndex].deviceIO_Struct[iIndex].PreemptPriority, 0);
				if(Devices[dIndex].deviceIO_Struct[iIndex].isEnableExit)  HAL_NVIC_EnableIRQ(EXTI2_IRQn); else HAL_NVIC_DisableIRQ(EXTI2_IRQn);
			}else if(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_3)
			{
				HAL_NVIC_SetPriority(EXTI3_IRQn, Devices[dIndex].deviceIO_Struct[iIndex].PreemptPriority, 0);
				if(Devices[dIndex].deviceIO_Struct[iIndex].isEnableExit)  HAL_NVIC_EnableIRQ(EXTI3_IRQn); else HAL_NVIC_DisableIRQ(EXTI3_IRQn);
			}else if(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_4)
			{
				HAL_NVIC_SetPriority(EXTI4_IRQn, Devices[dIndex].deviceIO_Struct[iIndex].PreemptPriority, 0);
				if(Devices[dIndex].deviceIO_Struct[iIndex].isEnableExit)  HAL_NVIC_EnableIRQ(EXTI4_IRQn); else HAL_NVIC_DisableIRQ(EXTI4_IRQn);
			}else if( (Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_5) \
					||(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_6) \
					||(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_7) \
					||(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_8) \
					||(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_9) )
			{
				HAL_NVIC_SetPriority(EXTI9_5_IRQn, Devices[dIndex].deviceIO_Struct[iIndex].PreemptPriority, 0);
				if(Devices[dIndex].deviceIO_Struct[iIndex].isEnableExit)  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn); else HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
			}else if( (Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_10) \
					||(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_11) \
					||(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_12) \
					||(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_13) \
					||(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_14) \
					||(Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin == GPIO_PIN_15) )
			{
				HAL_NVIC_SetPriority(EXTI15_10_IRQn, Devices[dIndex].deviceIO_Struct[iIndex].PreemptPriority, 0);
				if(Devices[dIndex].deviceIO_Struct[iIndex].isEnableExit)  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn); else HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
			}
		}
		
		//配置默认IO状态
		HAL_GPIO_WritePin(  Devices[dIndex].deviceIO_Struct[iIndex].GPIOx, \
							Devices[dIndex].deviceIO_Struct[iIndex].GPIO_Initure.Pin, \
							Devices[dIndex].deviceIO_Struct[iIndex].defaultState);
	}
}

void Devices_Init(Devices_Init_Struct* Devices , enum devicesIndex_enum device2Init)
{
	static u8 dIndex;
	if(device2Init == ALL_Index)
	{
		for(dIndex = 0;dIndex < devicesNum;dIndex++)	//遍历所有器件
		{
			deviceIO_Init(Devices,(enum devicesIndex_enum)dIndex);
		}
	}else{
		deviceIO_Init(Devices,device2Init);				////特定器件初始化
	}
}




/*中断服务函数*/
void EXTI0_IRQHandler(void){ HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);}
void EXTI1_IRQHandler(void){ HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);}
void EXTI2_IRQHandler(void){ HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);}
void EXTI3_IRQHandler(void){ HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);}
void EXTI4_IRQHandler(void){ HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);}
void EXTI9_5_IRQHandler(void)		//需要自己判断的！
{ 
//	if(PAin(5))		//提前配置PA5为外部中断时
//		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
}
void EXTI15_10_IRQHandler(void)		//需要自己判断的！
{ 
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}


/*外部中断回调函数*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch(GPIO_Pin)
    {
        case GPIO_PIN_0:
			/*如果使用RTOS则不初始化PA0的中断，使用裸机则使用PA0的外部中断*/
			#if SYSTEM_StdbyWKUP_ENABLE
				if(Check_WKUP())
				{
					Sys_Enter_Standby();//进入待机模式
				}
			#endif
            break;
		case GPIO_PIN_1:
			key_Up_Interrupted = TRUE;
			break;
        case GPIO_PIN_2:
			key_Down_Interrupted = TRUE;
            break;
        case GPIO_PIN_3:
            break;
        case GPIO_PIN_4:
            break;
    }
}

/*_________________________________________以下外设的声明代码均放在了PeriphConfigCore.h的最下面_________________________________________________________*/
#if SYSTEM_CAN1_ENABLE


CAN_TxHeaderTypeDef	TxHeader;      //发送
CAN_RxHeaderTypeDef	RxHeader;      //接收

CAN_HandleTypeDef CAN1_Handler;

/* CAN1 init function */
void MX_CAN1_Init(void)
{

  CAN1_Handler.Instance = CAN1;
  CAN1_Handler.Init.Prescaler = 3; 					//1~1024，分频自APB1 30MHZ
  CAN1_Handler.Init.Mode = CAN_MODE_NORMAL;			//无需动
  CAN1_Handler.Init.SyncJumpWidth = CAN_SJW_2TQ;	//CAN_SJW_1TQ ~ CAN_SJW_4TQ
  CAN1_Handler.Init.TimeSeg1 = CAN_BS1_10TQ;		//CAN_BS1_1TQ ~ CAN_BS1_16TQ
  CAN1_Handler.Init.TimeSeg2 = CAN_BS2_8TQ;			//CAN_BS2_1TQ ~ CAN_BS2_8TQ
	//CAN1工作在 500Kbps
	
  CAN1_Handler.Init.TimeTriggeredMode = DISABLE;	//非时间触发通信模式
  CAN1_Handler.Init.AutoBusOff = DISABLE;			//软件自动离线管理
  CAN1_Handler.Init.AutoWakeUp = DISABLE;			//睡眠模式通过软件唤醒
  CAN1_Handler.Init.AutoRetransmission = ENABLE;	//报文自动传送
  CAN1_Handler.Init.ReceiveFifoLocked = DISABLE;	//报文不锁定,新的覆盖旧的
  CAN1_Handler.Init.TransmitFifoPriority = DISABLE;//优先级由报文标识符决定
  
  HAL_CAN_Init(&CAN1_Handler);
  
}
void CAN_Config(void);
void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOI_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN1 GPIO Configuration
    PB8     ------> CAN1_RX
    PB9     ------> CAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
//    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 2, 0);
//    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
//    HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 2, 0);
//    HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
	
	CAN_Config();
  }
}

//void CAN1_RX0_IRQHandler(void)
//{
//  HAL_CAN_IRQHandler(&CAN1_Handler);
//}
//void CAN1_RX1_IRQHandler(void)
//{
//  HAL_CAN_IRQHandler(&CAN1_Handler);
//}

void CAN_Config(void)
{
  CAN_FilterTypeDef  sFilterConfig;

  /*配置CAN过滤器*/
  sFilterConfig.FilterBank = 0;                     //过滤器0
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = 0x0000;              //32位ID
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;          //32位MASK
  sFilterConfig.FilterMaskIdLow = 0x0000;
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;//过滤器0关联到FIFO0
  sFilterConfig.FilterActivation = ENABLE;          //激活滤波器0
  sFilterConfig.SlaveStartFilterBank = 14;
  
  //过滤器配置
  if (HAL_CAN_ConfigFilter(&CAN1_Handler, &sFilterConfig) != HAL_OK)
  {
	  FaultASSERT("HAL_CAN_ConfigFilter",1,flag_Fault);
  }

  //启动CAN外围设备
  if (HAL_CAN_Start(&CAN1_Handler) != HAL_OK)
  {
    FaultASSERT("HAL_CAN_Start",1,flag_Fault);
  }

  //激活可以RX通知
  if (HAL_CAN_ActivateNotification(&CAN1_Handler, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
  {
    FaultASSERT("HAL_CAN_ActivateNotification",1,flag_Fault);
  }
  
  /*配置传输过程*/
  TxHeader.StdId = 0x321;
  TxHeader.ExtId = 0x01;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.IDE = CAN_ID_STD;
  TxHeader.DLC = 2;
  TxHeader.TransmitGlobalTime = DISABLE;
}
//can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
//len:数据长度(最大为8)				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;
u8 CAN1_Send_Msg(u8* msg,u8 len)
{	
    u8 i=0;
	u32 TxMailbox;
	u8 message[8];
    TxHeader.StdId=0X12;        //标准标识符
    TxHeader.ExtId=0x12;        //扩展标识符(29位)
    TxHeader.IDE=CAN_ID_STD;    //使用标准帧
    TxHeader.RTR=CAN_RTR_DATA;  //数据帧
    TxHeader.DLC=len;                
    for(i=0;i<len;i++)
    {
		message[i]=msg[i];
	}
    if(HAL_CAN_AddTxMessage(&CAN1_Handler, &TxHeader, message, &TxMailbox) != HAL_OK)//发送
	{
		return HAL_ERROR;
	}
	while(HAL_CAN_GetTxMailboxesFreeLevel(&CAN1_Handler) != 3) {}
    return HAL_OK;
}

//can口接收数据查询
//buf:数据缓存区;	 
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
u8 CAN1_Receive_Msg(u8 *buf)
{
 	u32 i;
	u8	RxData[8];

	if(HAL_CAN_GetRxFifoFillLevel(&CAN1_Handler, CAN_RX_FIFO0) != 1)
	{
		return 0xF1;
	}

	if(HAL_CAN_GetRxMessage(&CAN1_Handler, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
	{
		return 0xF2;
	}
    for(i=0;i<RxHeader.DLC;i++)
    buf[i]=RxData[i];
	return RxHeader.DLC;
}

#endif
/*________________________________________用户定时器1PWM配置_________________________________________________________*/
#if STSTEM_TIM1PWM_ENABLE
TIM_HandleTypeDef TIM1_Handler;

void sys_TIM1PWM_ENABLE(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;
  TIM_OC_InitTypeDef sConfigOC;
  
  TIM1_Handler.Instance = TIM1;
  TIM1_Handler.Init.Prescaler = tim1prsc;
  TIM1_Handler.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1; //中心对齐
  TIM1_Handler.Init.Period = tim1arr;							  //重装载值，16位
  TIM1_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
  TIM1_Handler.Init.RepetitionCounter = 0;
  TIM1_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  HAL_TIM_Base_Init(&TIM1_Handler);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;	//时钟源来自内部（不启用外部ETR引脚作为时钟源）
  HAL_TIM_ConfigClockSource(&TIM1_Handler, &sClockSourceConfig);

  HAL_TIM_PWM_Init(&TIM1_Handler);
  
   /*如果要用主从定时器，参考https://my.oschina.net/u/4315748/blog/3220499*/
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;			//不输出信号给其他外设
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;	//不使用定时器主从功能，即不用TIM1的定时中断作为其他定时器的计数源
  HAL_TIMEx_MasterConfigSynchronization(&TIM1_Handler, &sMasterConfig);
  
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;		//默认disable（）
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;		//默认disable
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_3;				//锁定参数级别，如果死区等参数不再变的话就把LOCK调到最高级
  /*死区时间设计：DeadTime的8个bits定义如下：DT为死区时间，Tdts为1/72M~13.9ns
	DTG[7:5]=0xx	=>	DT = DTG[7:0] x Tdtg,        Tdtg  = Tdts; 		最大1.764us
	DTG[7:5]=10x	=>	DT = (64+DTG[5:0]) x Tdtg,   Tdtg  = 2 x Tdts;	最大3.5288us
	DTG[7:5]=110	=>	DT = (32+DTG[4:0]) x Tdtg,   Tdtg  = 8 x Tdts; 	最大7us
	DTG[7:5]=111	=>	DT = (32+DTG[4:0]) x Tdtg,   Tdtg  = 16 x Tdts;	最大14us（此时值为0xff）
																		要3us则设为0xab
	*/
  sBreakDeadTimeConfig.DeadTime = 0xab;							//死区时间设置 0x00~0xff
  #if STSTEM_TIM1PWM_useBreak
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_ENABLE;			//使能或失能TIMx刹车输入
  #else
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  #endif
  sBreakDeadTimeConfig.BreakFilter = 0xa;						//刹车输入滤波，0x0~0xF
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_LOW;	//刹车输入脚极性
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;	//默认
  HAL_TIMEx_ConfigBreakDeadTime(&TIM1_Handler, &sBreakDeadTimeConfig);

  sConfigOC.OCMode = TIM_OCMODE_PWM1;							//向上计数时，PWM1模式就是计数小于占空比值（TIMx_CNT<TIMx_CCR1），IO变为有效极性，否则相反
  sConfigOC.Pulse = tim1arr/2;									//占空比值（捕获比较值，16位）
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;					//可选此通道的有效极性为高还是低
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_LOW;					//可选互补通道的有效极性
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;					//disable就好
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;				//空闲电平极性选择（没有启用时的电平，谨慎修改）（也有可能是刹车时的电平）
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_SET;
  HAL_TIM_PWM_ConfigChannel(&TIM1_Handler, &sConfigOC, TIM_CHANNEL_1);
  __HAL_TIM_ENABLE_OCxPRELOAD(&TIM1_Handler, TIM_CHANNEL_1);

  sConfigOC.Pulse = tim1arr/2;
  HAL_TIM_PWM_ConfigChannel(&TIM1_Handler, &sConfigOC, TIM_CHANNEL_2);
  __HAL_TIM_ENABLE_OCxPRELOAD(&TIM1_Handler, TIM_CHANNEL_2);

  sConfigOC.Pulse = tim1arr/2;
  HAL_TIM_PWM_ConfigChannel(&TIM1_Handler, &sConfigOC, TIM_CHANNEL_3);
  __HAL_TIM_ENABLE_OCxPRELOAD(&TIM1_Handler, TIM_CHANNEL_3);

  sConfigOC.Pulse = tim1arr/2;
  HAL_TIM_PWM_ConfigChannel(&TIM1_Handler, &sConfigOC, TIM_CHANNEL_4);
  __HAL_TIM_ENABLE_OCxPRELOAD(&TIM1_Handler, TIM_CHANNEL_4);
}


void TIM1_set_Channel_Pulse(u8 channel,float percent)
{
	float compare;
	if(percent < 0) percent = 0f;
	if(percent > 100) percent = 100.0f;
	percent /= 100.0f;
	compare = ((float)tim1arr) * percent;
	switch(channel)
	{
		case TIM1PWM_Channel_1: TIM1->CCR1=(u32)compare;break;
		case TIM1PWM_Channel_2: TIM1->CCR2=(u32)compare;break;
		case TIM1PWM_Channel_3: TIM1->CCR3=(u32)compare;break;
		case TIM1PWM_Channel_4: TIM1->CCR4=(u32)compare;break;
		default:break;
	}
}


#endif

/*________________________________________用户定时器8PWM配置_________________________________________________________*/
#if STSTEM_TIM8PWM_ENABLE
TIM_HandleTypeDef TIM8_Handler;

void sys_TIM8PWM_ENABLE(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;
  TIM_OC_InitTypeDef sConfigOC;
  
  TIM8_Handler.Instance = TIM8;
  TIM8_Handler.Init.Prescaler = tim8prsc;
  TIM8_Handler.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
  TIM8_Handler.Init.Period = tim8arr;							//重装载值，16位
  TIM8_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
  TIM8_Handler.Init.RepetitionCounter = 0;
  TIM8_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&TIM8_Handler);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;	//时钟源来自内部（不启用外部ETR引脚作为时钟源）
  HAL_TIM_ConfigClockSource(&TIM8_Handler, &sClockSourceConfig);

  HAL_TIM_PWM_Init(&TIM8_Handler);
  
   /*如果要用主从定时器，参考https://my.oschina.net/u/4315748/blog/3220499*/
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;			//不输出信号给其他外设
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;	//不使用定时器主从功能，即不用TIM1的定时中断作为其他定时器的计数源
  HAL_TIMEx_MasterConfigSynchronization(&TIM8_Handler, &sMasterConfig);
  
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;		//默认disable（）
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;		//默认disable
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_3;				//锁定参数级别，如果死区等参数不再变的话就把LOCK调到最高级
  /*死区时间设计：DeadTime的8个bits定义如下：DT为死区时间，Tdts为1/72M~13.9ns
	DTG[7:5]=0xx	=>	DT = DTG[7:0] x Tdtg,        Tdtg  = Tdts; 		最大1.764us
	DTG[7:5]=10x	=>	DT = (64+DTG[5:0]) x Tdtg,   Tdtg  = 2 x Tdts;	最大3.5288us
	DTG[7:5]=110	=>	DT = (32+DTG[4:0]) x Tdtg,   Tdtg  = 8 x Tdts; 	最大7us
	DTG[7:5]=111	=>	DT = (32+DTG[4:0]) x Tdtg,   Tdtg  = 16 x Tdts;	最大14us（此时值为0xff）
																		要3us则设为0xab
	*/
  sBreakDeadTimeConfig.DeadTime = 0xab;							//死区时间设置 0x00~0xff
  #if STSTEM_TIM8PWM_useBreak
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_ENABLE;			//使能或失能TIMx刹车输入
  #else
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  #endif
  sBreakDeadTimeConfig.BreakFilter = 0xa;						//刹车输入滤波，0x0~0xF
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_LOW;	//刹车输入脚极性
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;	//默认
  HAL_TIMEx_ConfigBreakDeadTime(&TIM8_Handler, &sBreakDeadTimeConfig);

  sConfigOC.OCMode = TIM_OCMODE_PWM1;							//向上计数时，PWM1模式就是计数小于占空比值（TIMx_CNT<TIMx_CCR1），IO变为有效极性，否则相反
  sConfigOC.Pulse = tim8arr/2;									//占空比值（捕获比较值，16位）
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;					//可选此通道的有效极性为高还是低
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_LOW;					//可选互补通道的有效极性
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;					//disable就好
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;				//空闲电平极性选择（没有启用时的电平，谨慎修改）（也有可能是刹车时的电平）
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_SET;
  HAL_TIM_PWM_ConfigChannel(&TIM8_Handler, &sConfigOC, TIM_CHANNEL_1);
  __HAL_TIM_ENABLE_OCxPRELOAD(&TIM8_Handler, TIM_CHANNEL_1);

  sConfigOC.Pulse = tim8arr/2;
  HAL_TIM_PWM_ConfigChannel(&TIM8_Handler, &sConfigOC, TIM_CHANNEL_2);
  __HAL_TIM_ENABLE_OCxPRELOAD(&TIM8_Handler, TIM_CHANNEL_2);

  sConfigOC.Pulse = tim8arr/2;
  HAL_TIM_PWM_ConfigChannel(&TIM8_Handler, &sConfigOC, TIM_CHANNEL_3);
  __HAL_TIM_ENABLE_OCxPRELOAD(&TIM8_Handler, TIM_CHANNEL_3);

  sConfigOC.Pulse = tim8arr/2;
  HAL_TIM_PWM_ConfigChannel(&TIM8_Handler, &sConfigOC, TIM_CHANNEL_4);
  __HAL_TIM_ENABLE_OCxPRELOAD(&TIM8_Handler, TIM_CHANNEL_4);
}


void TIM8_set_Channel_Pulse(u8 channel,float percent)
{
	float compare;
	if(percent < 0) percent = 0f;
	if(percent > 100) percent = 100.0f;
	percent /= 100.0f;
	compare = ((float)tim8arr) * percent;
	switch(channel)
	{
		case TIM8PWM_Channel_1: TIM8->CCR1=(u32)compare;break;
		case TIM8PWM_Channel_2: TIM8->CCR2=(u32)compare;break;
		case TIM8PWM_Channel_3: TIM8->CCR3=(u32)compare;break;
		case TIM8PWM_Channel_4: TIM8->CCR4=(u32)compare;break;
		default:break;
	}
}


#endif


/*___________________________________________用户SPI1、2配置_______________________________________________*/
#if ((SYSTEM_SPI1_ENABLE)||(SYSTEM_SPI2_ENABLE))


/*初始化SPI1的SS器件选中引脚	默认PA4，用户可改*/
void sys_SPI1_SS_io_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull=GPIO_PULLUP;
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;
    
	/*主要改这三行*/
	__HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_Initure.Pin=GPIO_PIN_4;
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
	SPI1_CS = IO_High;			                	//SPI1默认不选中
}
/*初始化SPI2的SS器件选中引脚	默认PB12，用户可改*/
void sys_SPI2_SS_io_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull=GPIO_PULLUP;
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;
    
	/*主要改这三行*/
	__HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_Initure.Pin=GPIO_PIN_12;
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
	SPI2_CS = IO_High;			                	//SPI2默认不选中
}


//SPI底层驱动，时钟使能，引脚配置
//此函数会被HAL_SPI_Init()调用
//hspi:SPI句柄
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_Initure;
    /*
	SPI1->CS	SPI1->CLK	SPI1->MISO	SPI1->MOSI 	—————— 	SPI2->CS	SPI2->CLK	SPI2->MISO	SPI2->MOSI
	PA4			PA5			PA6			PA7					PB12		PB13		PB14		PB15
	*/
	if(hspi->Instance == SPI1)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();       //使能GPIOA时钟
		__HAL_RCC_SPI1_CLK_ENABLE();        //使能SPI1时钟
		//__HAL_RCC_AFIO_CLK_ENABLE();
		
		//PB13,14,15
		GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //复用推挽输出，MISO配置为复用推挽没问题
		GPIO_Initure.Pull=GPIO_PULLUP;                  //上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;        //快速   
		GPIO_Initure.Alternate=GPIO_AF5_SPI1;
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	}else if(hspi->Instance == SPI2)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();       //使能GPIOB时钟
		__HAL_RCC_SPI2_CLK_ENABLE();        //使能SPI2时钟
		//__HAL_RCC_AFIO_CLK_ENABLE();
		
		//PB13,14,15
		GPIO_Initure.Pin=GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //复用推挽输出，MISO配置为复用推挽没问题
		GPIO_Initure.Pull=GPIO_PULLUP;                  //上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;        //快速 
		GPIO_Initure.Alternate=GPIO_AF5_SPI1;
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	}

}


#if SYSTEM_SPI1_ENABLE

SPI_HandleTypeDef SPI1_Handler;  //SPI1句柄

void sys_SPI1_ENABLE(void)
{
    SPI1_Handler.Instance=SPI1;                         //SPI1
    SPI1_Handler.Init.Mode=SPI_MODE_MASTER;             //设置SPI工作模式，设置为主模式
    SPI1_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //设置SPI单向或者双向的数据模式:SPI设置为双线模式
    SPI1_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //设置SPI的数据大小:SPI发送接收8位帧结构
    
	SPI1_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //串行同步时钟的空闲状态为高电平
    SPI1_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    
	SPI1_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI1_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256;//定义波特率预分频的值:波特率预分频值为256
    SPI1_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI1_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //关闭TI模式
    SPI1_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
    SPI1_Handler.Init.CRCPolynomial=7;                  //CRC值计算的多项式
    HAL_SPI_Init(&SPI1_Handler);//初始化
    
	sys_SPI1_SS_io_Init();
	
    __HAL_SPI_ENABLE(&SPI1_Handler);                    //使能SPI1
	
    //SPI1_ReadWriteByte(0Xff);                           //启动传输
}

//SPI速度设置函数
//SPI1速度=APB2/分频系数
/*参数可选：
SPI_BAUDRATEPRESCALER_2 SPI_BAUDRATEPRESCALER_4 SPI_BAUDRATEPRESCALER_8 SPI_BAUDRATEPRESCALER_16 
SPI_BAUDRATEPRESCALER_32 SPI_BAUDRATEPRESCALER_64 SPI_BAUDRATEPRESCALER_128 SPI_BAUDRATEPRESCALER_256
*/
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
    __HAL_SPI_DISABLE(&SPI1_Handler);            //关闭SPI
    SPI1_Handler.Instance->CR1&=0XFFC7;          //位3-5清零，用来设置波特率
    SPI1_Handler.Instance->CR1|=SPI_BaudRatePrescaler;//设置SPI速度
    __HAL_SPI_ENABLE(&SPI1_Handler);             //使能SPI
    
}

//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte_8Byte(u8 TxData)
{
    u8 Rxdata;
	SPI1_CS = IO_Low;
    HAL_SPI_TransmitReceive(&SPI1_Handler,&TxData,&Rxdata,1, 100);
	SPI1_CS = IO_High;	
 	return Rxdata;          		    //返回收到的数据		
}

//传入一个包含2字节的u8*指针
u8 SPI1_ReadWriteByte_16Byte(u8* TxData)
{
    u8 Rxdata;
	SPI1_CS = IO_Low;
    HAL_SPI_TransmitReceive(&SPI1_Handler,TxData,&Rxdata, 2, 100);
	SPI1_CS = IO_High;	
 	return Rxdata;          		    //返回收到的数据		
}

u8 SPI1_WriteByte_8Byte(u8 TxData)
{
	u8 state;
	SPI1_CS = IO_Low;
	state = (u8)(HAL_SPI_Transmit(&SPI1_Handler, &TxData, 1, 100));	
	SPI1_CS = IO_High;
	return state;
}

//传入一个包含2字节的u8*指针
u8 SPI1_WriteByte_16Byte(u8* TxData)
{
	u8 state;
	SPI1_CS = IO_Low;
	state = (u8)(HAL_SPI_Transmit(&SPI1_Handler, TxData, 2, 100));	
	SPI1_CS = IO_High;
	return state;
}

#endif

#if SYSTEM_SPI2_ENABLE

SPI_HandleTypeDef SPI2_Handler;  //SPI2句柄

void sys_SPI2_ENABLE(void)
{
    SPI2_Handler.Instance=SPI2;                         //SPI2
    SPI2_Handler.Init.Mode=SPI_MODE_MASTER;             //设置SPI工作模式，设置为主模式
    SPI2_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //设置SPI单向或者双向的数据模式:SPI设置为双线模式
    SPI2_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //设置SPI的数据大小:SPI发送接收8位帧结构
    
	SPI2_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //串行同步时钟的空闲状态为高电平
    SPI2_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    
	SPI2_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI2_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256;//定义波特率预分频的值:波特率预分频值为256
    SPI2_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI2_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //关闭TI模式
    SPI2_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
    SPI2_Handler.Init.CRCPolynomial=7;                  //CRC值计算的多项式
    HAL_SPI_Init(&SPI2_Handler);//初始化
    
	sys_SPI2_SS_io_Init();
	
    __HAL_SPI_ENABLE(&SPI2_Handler);                    //使能SPI2
	
    //SPI2_ReadWriteByte(0Xff);                           //启动传输
}

//SPI速度设置函数
//SPI2速度=APB1/分频系数
/*参数可选：
SPI_BAUDRATEPRESCALER_2 SPI_BAUDRATEPRESCALER_4 SPI_BAUDRATEPRESCALER_8 SPI_BAUDRATEPRESCALER_16 
SPI_BAUDRATEPRESCALER_32 SPI_BAUDRATEPRESCALER_64 SPI_BAUDRATEPRESCALER_128 SPI_BAUDRATEPRESCALER_256
*/
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
    __HAL_SPI_DISABLE(&SPI2_Handler);            //关闭SPI
    SPI2_Handler.Instance->CR1&=0XFFC7;          //位3-5清零，用来设置波特率
    SPI2_Handler.Instance->CR1|=SPI_BaudRatePrescaler;//设置SPI速度
    __HAL_SPI_ENABLE(&SPI2_Handler);             //使能SPI
    
}

//SPI2 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI2_ReadWriteByte(u8 TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&SPI2_Handler,&TxData,&Rxdata,1, 100); 
 	return Rxdata;          		    //返回收到的数据		
}
#endif

#endif

/*________________________________________用户定时器2配置_________________________________________________________*/
#if STSTEM_TIM2_ENABLE
TIM_HandleTypeDef TIM2_Handler;

#if (STSTEM_TIM2_asPWMorCap == 0)
	TIM_OC_InitTypeDef 	TIM2_CH1Handler,TIM2_CH2Handler,TIM2_CH3Handler,TIM2_CH4Handler;
#elif (STSTEM_TIM2_asPWMorCap == 1)
	TIM_IC_InitTypeDef TIM2_CHxConfig;
#elif (STSTEM_TIM2_asPWMorCap == 3)
	TIM_Encoder_InitTypeDef sEncoderConfig;
#endif

void sys_TIM2_ENABLE(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	
	TIM2_Handler.Instance = 		TIM2;                          	//通用定时器2
    TIM2_Handler.Init.Prescaler=	(84-1);                 	   	//分频系数
    TIM2_Handler.Init.CounterMode=	TIM_COUNTERMODE_UP;    			//向上计数器
    TIM2_Handler.Init.Period=		tim2arr;                       	//自动装载值
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;			//时钟分频因子
	TIM2_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_Base_Init(&TIM2_Handler);
	
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&TIM2_Handler, &sClockSourceConfig);
	
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&TIM2_Handler, &sMasterConfig);
	
	#if (STSTEM_TIM2_asPWMorCap == 0)
		/*作为PWM*/
		HAL_TIM_PWM_Init(&TIM2_Handler);       //初始化PWM
		if((STSTEM_TIM2PWM_CHANNEL_ENABLE) & B0000_0001)
		{
			TIM2_CH1Handler.OCMode=TIM_OCMODE_PWM1; 		//模式选择PWM1
			TIM2_CH1Handler.Pulse=tim2arr/2;            	//设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
			TIM2_CH1Handler.OCPolarity=TIM_OCPOLARITY_LOW; 	//输出比较极性为低 
			HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&TIM2_CH1Handler,TIM_CHANNEL_1);//配置TIM2通道1
			__HAL_TIM_ENABLE_OCxPRELOAD(&TIM2_Handler, TIM_CHANNEL_1);
			HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_1);	//开启PWM通道1
		}
		if((STSTEM_TIM2PWM_CHANNEL_ENABLE) & B0000_0010)
		{
			TIM2_CH2Handler.OCMode=TIM_OCMODE_PWM1;
			TIM2_CH2Handler.Pulse=tim2arr/2;
			TIM2_CH2Handler.OCPolarity=TIM_OCPOLARITY_LOW;
			HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&TIM2_CH2Handler,TIM_CHANNEL_2);
			__HAL_TIM_ENABLE_OCxPRELOAD(&TIM2_Handler, TIM_CHANNEL_2);
			HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_2);
		}
		if((STSTEM_TIM2PWM_CHANNEL_ENABLE) & B0000_0100)
		{
			TIM2_CH3Handler.OCMode=TIM_OCMODE_PWM1;
			TIM2_CH3Handler.Pulse=tim2arr/2;
			TIM2_CH3Handler.OCPolarity=TIM_OCPOLARITY_LOW;
			HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&TIM2_CH3Handler,TIM_CHANNEL_3);
			__HAL_TIM_ENABLE_OCxPRELOAD(&TIM2_Handler, TIM_CHANNEL_3);
			HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_3);
		}
		if((STSTEM_TIM2PWM_CHANNEL_ENABLE) & B0000_1000)
		{
			TIM2_CH4Handler.OCMode=TIM_OCMODE_PWM1;
			TIM2_CH4Handler.Pulse=tim2arr/2;
			TIM2_CH4Handler.OCPolarity=TIM_OCPOLARITY_LOW;
			HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&TIM2_CH4Handler,TIM_CHANNEL_4);
			__HAL_TIM_ENABLE_OCxPRELOAD(&TIM2_Handler, TIM_CHANNEL_4);
			HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_4);
		}
	#elif (STSTEM_TIM2_asPWMorCap == 1)
		/*作为Cap*/
		TIM2_Handler.Init.Period=0XFFFF;       //自动装载值
		HAL_TIM_IC_Init(&TIM2_Handler);			//初始化输入捕获时基参数
		
		if(STSTEM_TIM2_Cap_trigV == 1)
			TIM2_CHxConfig.ICPolarity=TIM_ICPOLARITY_RISING;    //上升沿捕获
		else if(STSTEM_TIM2_Cap_trigV == 0)
			TIM2_CHxConfig.ICPolarity=TIM_ICPOLARITY_FALLING;    //下降沿捕获
		else if(STSTEM_TIM2_Cap_trigV == 2)
			TIM2_CHxConfig.ICPolarity=TIM_ICPOLARITY_BOTHEDGE;    //双边沿捕获
		
		TIM2_CHxConfig.ICSelection=TIM_ICSELECTION_DIRECTTI;//映射到TI1上
		TIM2_CHxConfig.ICPrescaler=TIM_ICPSC_DIV1;          //配置输入分频，不分频
		TIM2_CHxConfig.ICFilter=0;                          //配置输入滤波器，不滤波
		
		switch(STSTEM_TIM2_Cap_Channel)
		{
			case B0000_0001:				//选用通带1作为输入捕获
				HAL_TIM_IC_ConfigChannel(&TIM2_Handler,&TIM2_CHxConfig,TIM_CHANNEL_1);//配置TIM2通道1
				HAL_TIM_IC_Start_IT(&TIM2_Handler,TIM_CHANNEL_1);   //开启TIM2的捕获通道1，并且开启捕获中断
				break;
			case B0000_0010:				//选用通带2作为输入捕获
				HAL_TIM_IC_ConfigChannel(&TIM2_Handler,&TIM2_CHxConfig,TIM_CHANNEL_2);
				HAL_TIM_IC_Start_IT(&TIM2_Handler,TIM_CHANNEL_2);
				break;
			case B0000_0100:				//选用通带3作为输入捕获
				HAL_TIM_IC_ConfigChannel(&TIM2_Handler,&TIM2_CHxConfig,TIM_CHANNEL_3);
				HAL_TIM_IC_Start_IT(&TIM2_Handler,TIM_CHANNEL_3);
				break;
			case B0000_1000:				//选用通带4作为输入捕获
				HAL_TIM_IC_ConfigChannel(&TIM2_Handler,&TIM2_CHxConfig,TIM_CHANNEL_4);
				HAL_TIM_IC_Start_IT(&TIM2_Handler,TIM_CHANNEL_4);
				break;
			default:break;
		}
		__HAL_TIM_ENABLE_IT(&TIM2_Handler,TIM_IT_UPDATE);   //使能更新中断
	#elif (STSTEM_TIM2_asPWMorCap == 3)
		TIM2_Handler.Init.Prescaler=	0;
		TIM2_Handler.Init.Period=		0xFFFF;

	  sEncoderConfig.EncoderMode        = TIM_ENCODERMODE_TI12;
	  sEncoderConfig.IC1Polarity        = TIM_ICPOLARITY_RISING;
	  sEncoderConfig.IC1Selection       = TIM_ICSELECTION_DIRECTTI;
	  sEncoderConfig.IC1Prescaler       = TIM_ICPSC_DIV1;
	  sEncoderConfig.IC1Filter          = 0;

	  sEncoderConfig.IC2Polarity        = TIM_ICPOLARITY_RISING;
	  sEncoderConfig.IC2Selection       = TIM_ICSELECTION_DIRECTTI;
	  sEncoderConfig.IC2Prescaler       = TIM_ICPSC_DIV1;
	  sEncoderConfig.IC2Filter          = 0;
	  __HAL_TIM_SET_COUNTER(&TIM2_Handler,0);
	  
	  HAL_TIM_Encoder_Init(&TIM2_Handler, &sEncoderConfig);
	  
	__HAL_TIM_CLEAR_IT(&TIM2_Handler, TIM_IT_UPDATE);  // 清除更新中断标志位
	__HAL_TIM_URS_ENABLE(&TIM2_Handler);               // 仅允许计数器溢出才产生更新中断
	__HAL_TIM_ENABLE_IT(&TIM2_Handler,TIM_IT_UPDATE);  // 使能更新中断

	HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	HAL_TIM_Encoder_Start(&TIM2_Handler, TIM_CHANNEL_ALL);
	
	#endif
	
	#if (STSTEM_TIM2_asPWMorCap == 2)
		#if (!STSTEM_TIM2_TI_ENABLE)
			HAL_TIM_Base_Start(&TIM2_Handler);	//不带IT的开启定时器
		#else
			HAL_TIM_Base_Start_IT(&TIM2_Handler);//带IT的开启定时器
		#endif
	#endif
}

//定时器2中断服务函数
void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM2_Handler);
}

#if (STSTEM_TIM2_asPWMorCap == 3)				//如果使用正交解码
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef* htim_base)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim_base->Instance==TIM2)
  {
    /* 基本定时器外设时钟使能 */
    __HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_TIM2_CLK_ENABLE();
    //__HAL_AFIO_REMAP_TIM2_ENABLE();
	  
	/*PA15 PB3*/
	  
    /* 定时器通道1功能引脚IO初始化 */
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull=GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* 定时器通道2功能引脚IO初始化 */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }
}

/*在定时器溢出中断回调函数里面进行加减操作*/
int32_t EncoderOverflowCount = 0;//定时器溢出次数

/*获取一次编码器的当前计数值，用于表示编码器当前的绝对位置*/
int32_t peek_TIM2_Encoder_Value(void)
{
	return ( int32_t )__HAL_TIM_GET_COUNTER(&TIM2_Handler);
}

/*获取一次正交编码器的旋转速度 单位 转/秒，本函数放到TIM4的10ms定时中断中*/
float peek_TIM2_Encoder_Speed(void)
{
	float Speed = 0;
	int32_t CaptureNumber = 0;     // 输入捕获数
	
	/* 速度定时器编码器接口捕获值 */
	CaptureNumber = ( int32_t )__HAL_TIM_GET_COUNTER(&TIM2_Handler) + EncoderOverflowCount*65536;
	
	Speed = ((float)CaptureNumber / (float)PPR);
	Speed *= 100.0f;	//本函数10ms计算一次，这里换算成1秒
	
	/* 数据清零,等待下一秒重新记录数据 */
	EncoderOverflowCount = 0;
	__HAL_TIM_SET_COUNTER(&TIM2_Handler,0);
	
	return Speed;
}

#endif

#if (STSTEM_TIM2_asPWMorCap == 1)				//如果使用Cap功能

//定时器2的IC功能底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_IC_Init()调用
//htim:定时器句柄
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_Initure;
	if(htim->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM2时钟
		//__HAL_AFIO_REMAP_TIM2_ENABLE();			/*TIM2通道引脚完全重映射使能 (CH1/ETR/PA15, CH2/PB3, CH3/PB10, CH4/PB11)*/
		
		if(STSTEM_TIM2_Cap_trigV == 1)				//上升沿捕获
			GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
		else if(STSTEM_TIM2_Cap_trigV == 0)			//下降沿捕获
			GPIO_Initure.Pull=GPIO_PULLUP;        	//上拉
		else if(STSTEM_TIM2_Cap_trigV == 2)			//双边沿捕获
			GPIO_Initure.Pull=GPIO_NOPULL;        	//无拉
		
		GPIO_Initure.Mode=GPIO_MODE_AF_PP; 			//复用输入
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;	//高速
		GPIO_Initure.Alternate = GPIO_AF1_TIM2;
		
		switch(STSTEM_TIM2_Cap_Channel)
		{
			case B0000_0001:					//选用通带1作为输入捕获
				__HAL_RCC_GPIOA_CLK_ENABLE();	//开启GPIOA时钟
				GPIO_Initure.Pin=GPIO_PIN_15;            //PA15
				HAL_GPIO_Init(GPIOA,&GPIO_Initure);
				break;
			case B0000_0010:					//选用通带2作为输入捕获
				__HAL_RCC_GPIOB_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_3;            //PB3
				HAL_GPIO_Init(GPIOB,&GPIO_Initure);
				break;
			case B0000_0100:					//选用通带3作为输入捕获
				__HAL_RCC_GPIOB_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_10;            //PB10
				HAL_GPIO_Init(GPIOB,&GPIO_Initure);
				break;
			case B0000_1000:					//选用通带4作为输入捕获
				__HAL_RCC_GPIOB_CLK_ENABLE();
				GPIO_Initure.Pin=GPIO_PIN_11;            //PB11
				HAL_GPIO_Init(GPIOB,&GPIO_Initure);
				break;
			default:break;
		}
		
		HAL_NVIC_SetPriority(TIM2_IRQn,2,0);    //设置中断优先级，抢占优先级2，子优先级0
		HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM2中断通道  
	}
}

//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数
u8  TIM2CHx_CAPTURE_STA=0;							//输入捕获状态		    				
u16	TIM2CHx_CAPTURE_VAL;							//输入捕获值

/*获取最近一次TIM2输入捕获的脉冲时间，单位 毫秒，按照设置的捕获沿进行，可以随时随地调用此函数*/
float Peek_TIM2_Cap_Val(void)
{
	u8 Tim2overFlowTimes = 0;
	float temp = 0;
	if(TIM2CHx_CAPTURE_STA&0X80)        			//成功捕获到了一次高电平
	{
		Tim2overFlowTimes = TIM2CHx_CAPTURE_STA & 0X3F;
		temp = (float)Tim2overFlowTimes;
		temp *= 65536.0f;		 	    			//溢出时间总和
		temp += (float)TIM2CHx_CAPTURE_VAL;      	//加上当前计数值得到总的高电平时间
		TIM2CHx_CAPTURE_STA = 0;          			//开启下一次捕获
		temp /= 1000.0f;							//单位 毫秒
		return temp;
	}else{
		return 0.0f;
	}
}

//定时器输入捕获中断处理回调函数，该函数在HAL_TIM_IRQHandler中会被调用
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//捕获中断发生时执行
{
	if(htim==(&TIM2_Handler))
	{
			switch(STSTEM_TIM2_Cap_Channel)
			{
				case B0000_0001: //通道1
					Process_TIM2_IC_CallBack_Channel_1();
					break;
				case B0000_0010: //通带2
					Process_TIM2_IC_CallBack_Channel_2();
					break;
				case B0000_0100: //通道3
					Process_TIM2_IC_CallBack_Channel_3();
					break;
				case B0000_1000: //通带4
					Process_TIM2_IC_CallBack_Channel_4();
					break;
				default:break;
			}
	}	
}

void Process_TIM2_IC_CallBack_Channel_1(void)
{
	if((STSTEM_TIM2_Cap_trigV == 1)||(STSTEM_TIM2_Cap_trigV == 0))				//上升沿或者下降沿捕获
	{
		if((TIM2CHx_CAPTURE_STA&0X80)==0)				//还未成功捕获
		{
			if(TIM2CHx_CAPTURE_STA&0X40)				//捕获到一个下降沿 		
			{	  			
				TIM2CHx_CAPTURE_STA|=0X80;				//标记成功捕获到一次高电平脉宽
				TIM2CHx_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM2_Handler,TIM_CHANNEL_1);//获取当前的捕获值.
				TIM_RESET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_1);   //一定要先清除原来的设置！！
				if(STSTEM_TIM2_Cap_trigV == 1)	//如果配置为上升沿捕获
				{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//配置TIM2通道1上升沿捕获
				}else{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);//定时器2通道1设置为下降沿捕获
				}
				
			}else  										//还未开始,第一次捕获上升沿
			{
				TIM2CHx_CAPTURE_STA=0;					//清空
				TIM2CHx_CAPTURE_VAL=0;
				TIM2CHx_CAPTURE_STA|=0X40;				//标记捕获到了上升沿
				__HAL_TIM_DISABLE(&TIM2_Handler);      	//关闭定时器2
				__HAL_TIM_SET_COUNTER(&TIM2_Handler,0);
				TIM_RESET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_1);   //一定要先清除原来的设置！！
				
				if(STSTEM_TIM2_Cap_trigV == 1)	//如果配置为上升沿捕获
				{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);//定时器2通道1设置为下降沿捕获
				}else{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//定时器2通道1设置为上升沿捕获
				}
				
				__HAL_TIM_ENABLE(&TIM2_Handler);		//使能定时器2
			}		    
		}
	}else if(STSTEM_TIM2_Cap_trigV == 2)				//双沿捕获
	{
		if((TIM2CHx_CAPTURE_STA&0X80)==0)				//还未成功捕获
		{
			if(TIM2CHx_CAPTURE_STA&0X40)				//已经捕获到一个沿
			{
				TIM2CHx_CAPTURE_STA|=0X80;				//标记成功捕获到一次脉宽
				TIM2CHx_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM2_Handler,TIM_CHANNEL_1);//获取当前的捕获值
			}else										//还未捕获到一个沿
			{
				TIM2CHx_CAPTURE_STA=0;					//清空
				TIM2CHx_CAPTURE_VAL=0;
				TIM2CHx_CAPTURE_STA|=0X40;				//标记捕获到了一个沿
				__HAL_TIM_DISABLE(&TIM2_Handler);      	//关闭定时器2
				__HAL_TIM_SET_COUNTER(&TIM2_Handler,0);
				__HAL_TIM_ENABLE(&TIM2_Handler);		//使能定时器2
			}
		}
	}
}

void Process_TIM2_IC_CallBack_Channel_2(void)
{
	if((STSTEM_TIM2_Cap_trigV == 1)||(STSTEM_TIM2_Cap_trigV == 0))
	{
		if((TIM2CHx_CAPTURE_STA&0X80)==0)
		{
			if(TIM2CHx_CAPTURE_STA&0X40)	
			{	  			
				TIM2CHx_CAPTURE_STA|=0X80;	
				TIM2CHx_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM2_Handler,TIM_CHANNEL_2);
				TIM_RESET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_2); 
				if(STSTEM_TIM2_Cap_trigV == 1)
				{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_2,TIM_ICPOLARITY_RISING);
				}else{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_2,TIM_ICPOLARITY_FALLING);
				}
				
			}else 
			{
				TIM2CHx_CAPTURE_STA=0;
				TIM2CHx_CAPTURE_VAL=0;
				TIM2CHx_CAPTURE_STA|=0X40;
				__HAL_TIM_DISABLE(&TIM2_Handler);
				__HAL_TIM_SET_COUNTER(&TIM2_Handler,0);
				TIM_RESET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_2); 
				
				if(STSTEM_TIM2_Cap_trigV == 1)	
				{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_2,TIM_ICPOLARITY_FALLING);
				}else{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_2,TIM_ICPOLARITY_RISING);
				}
				
				__HAL_TIM_ENABLE(&TIM2_Handler);
			}		    
		}
	}else if(STSTEM_TIM2_Cap_trigV == 2)				//双沿捕获
	{
		if((TIM2CHx_CAPTURE_STA&0X80)==0)				//还未成功捕获
		{
			if(TIM2CHx_CAPTURE_STA&0X40)				//已经捕获到一个沿
			{
				TIM2CHx_CAPTURE_STA|=0X80;				//标记成功捕获到一次脉宽
				TIM2CHx_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM2_Handler,TIM_CHANNEL_2);//获取当前的捕获值
			}else										//还未捕获到一个沿
			{
				TIM2CHx_CAPTURE_STA=0;					//清空
				TIM2CHx_CAPTURE_VAL=0;
				TIM2CHx_CAPTURE_STA|=0X40;				//标记捕获到了一个沿
				__HAL_TIM_DISABLE(&TIM2_Handler);      	//关闭定时器2
				__HAL_TIM_SET_COUNTER(&TIM2_Handler,0);
				__HAL_TIM_ENABLE(&TIM2_Handler);		//使能定时器2
			}
		}
	}
}

void Process_TIM2_IC_CallBack_Channel_3(void)
{
	if((STSTEM_TIM2_Cap_trigV == 1)||(STSTEM_TIM2_Cap_trigV == 0))
	{
		if((TIM2CHx_CAPTURE_STA&0X80)==0)
		{
			if(TIM2CHx_CAPTURE_STA&0X40)	
			{	  			
				TIM2CHx_CAPTURE_STA|=0X80;	
				TIM2CHx_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM2_Handler,TIM_CHANNEL_3);
				TIM_RESET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_3); 
				if(STSTEM_TIM2_Cap_trigV == 1)
				{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_RISING);
				}else{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_FALLING);
				}
				
			}else 
			{
				TIM2CHx_CAPTURE_STA=0;
				TIM2CHx_CAPTURE_VAL=0;
				TIM2CHx_CAPTURE_STA|=0X40;
				__HAL_TIM_DISABLE(&TIM2_Handler);
				__HAL_TIM_SET_COUNTER(&TIM2_Handler,0);
				TIM_RESET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_3); 
				
				if(STSTEM_TIM2_Cap_trigV == 1)	
				{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_FALLING);
				}else{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_RISING);
				}
				
				__HAL_TIM_ENABLE(&TIM2_Handler);
			}		    
		}
	}else if(STSTEM_TIM2_Cap_trigV == 2)				//双沿捕获
	{
		if((TIM2CHx_CAPTURE_STA&0X80)==0)				//还未成功捕获
		{
			if(TIM2CHx_CAPTURE_STA&0X40)				//已经捕获到一个沿
			{
				TIM2CHx_CAPTURE_STA|=0X80;				//标记成功捕获到一次脉宽
				TIM2CHx_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM2_Handler,TIM_CHANNEL_3);//获取当前的捕获值
			}else										//还未捕获到一个沿
			{
				TIM2CHx_CAPTURE_STA=0;					//清空
				TIM2CHx_CAPTURE_VAL=0;
				TIM2CHx_CAPTURE_STA|=0X40;				//标记捕获到了一个沿
				__HAL_TIM_DISABLE(&TIM2_Handler);      	//关闭定时器2
				__HAL_TIM_SET_COUNTER(&TIM2_Handler,0);
				__HAL_TIM_ENABLE(&TIM2_Handler);		//使能定时器2
			}
		}
	}
}
void Process_TIM2_IC_CallBack_Channel_4(void)
{
	if((STSTEM_TIM2_Cap_trigV == 1)||(STSTEM_TIM2_Cap_trigV == 0))
	{
		if((TIM2CHx_CAPTURE_STA&0X80)==0)
		{
			if(TIM2CHx_CAPTURE_STA&0X40)	
			{	  			
				TIM2CHx_CAPTURE_STA|=0X80;	
				TIM2CHx_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM2_Handler,TIM_CHANNEL_4);
				TIM_RESET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_4); 
				if(STSTEM_TIM2_Cap_trigV == 1)
				{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_4,TIM_ICPOLARITY_RISING);
				}else{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_4,TIM_ICPOLARITY_FALLING);
				}
				
			}else 
			{
				TIM2CHx_CAPTURE_STA=0;
				TIM2CHx_CAPTURE_VAL=0;
				TIM2CHx_CAPTURE_STA|=0X40;
				__HAL_TIM_DISABLE(&TIM2_Handler);
				__HAL_TIM_SET_COUNTER(&TIM2_Handler,0);
				TIM_RESET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_4); 
				
				if(STSTEM_TIM2_Cap_trigV == 1)	
				{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_4,TIM_ICPOLARITY_FALLING);
				}else{
					TIM_SET_CAPTUREPOLARITY(&TIM2_Handler,TIM_CHANNEL_4,TIM_ICPOLARITY_RISING);
				}
				
				__HAL_TIM_ENABLE(&TIM2_Handler);
			}		    
		}
	}else if(STSTEM_TIM2_Cap_trigV == 2)				//双沿捕获
	{
		if((TIM2CHx_CAPTURE_STA&0X80)==0)				//还未成功捕获
		{
			if(TIM2CHx_CAPTURE_STA&0X40)				//已经捕获到一个沿
			{
				TIM2CHx_CAPTURE_STA|=0X80;				//标记成功捕获到一次脉宽
				TIM2CHx_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM2_Handler,TIM_CHANNEL_4);//获取当前的捕获值
			}else										//还未捕获到一个沿
			{
				TIM2CHx_CAPTURE_STA=0;					//清空
				TIM2CHx_CAPTURE_VAL=0;
				TIM2CHx_CAPTURE_STA|=0X40;				//标记捕获到了一个沿
				__HAL_TIM_DISABLE(&TIM2_Handler);      	//关闭定时器2
				__HAL_TIM_SET_COUNTER(&TIM2_Handler,0);
				__HAL_TIM_ENABLE(&TIM2_Handler);		//使能定时器2
			}
		}
	}
}


#endif
#if (STSTEM_TIM2_asPWMorCap == 0)				//如果使用PWM
	//设置TIM2通道的占空比
	//percent:占空比百分数
	void TIM2_set_Channel_Pulse(u8 channel,float percent)
	{
		float compare;
		if(percent < 0) percent = 0f;
		if(percent > 100) percent = 100.0f;
		percent /= 100.0f;
		compare = ((float)tim2arr) * percent;
		switch(channel)
		{
			case TIM2PWM_Channel_1: TIM2->CCR1=(u32)compare;break;
			case TIM2PWM_Channel_2: TIM2->CCR2=(u32)compare;break;
			case TIM2PWM_Channel_3: TIM2->CCR3=(u32)compare;break;
			case TIM2PWM_Channel_4: TIM2->CCR4=(u32)compare;break;
			default:break;
		}
	}
#endif

#endif


/*________________________________________用户定时器6配置_________________________________________________________*/
#if STSTEM_TIM6_ENABLE

TIM_HandleTypeDef TIM6_Handler;
void sys_TIM6_ENABLE(void)
{
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	
	TIM6_Handler.Instance=TIM6;                          		//基本定时器6
    TIM6_Handler.Init.Prescaler = (84-1);                     	//分频系数
    TIM6_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    		//向上计数器
    TIM6_Handler.Init.Period = tim6arr;                        	//自动装载值
    TIM6_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;		//时钟分频因子
	TIM6_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_Base_Init(&TIM6_Handler);
	
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&TIM6_Handler, &sMasterConfig);
	
    HAL_TIM_Base_Start_IT(&TIM6_Handler); //使能定时器6和定时器6更新中断：TIM_IT_UPDATE 
}


//定时器6中断服务函数
void TIM6_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM6_Handler);
}

#endif

/*________________________________________用户定时器7配置_________________________________________________________*/
#if STSTEM_TIM7_ENABLE

TIM_HandleTypeDef TIM7_Handler;
void sys_TIM7_ENABLE(void)
{
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	
	TIM7_Handler.Instance=TIM7;                          		//基本定时器7
    TIM7_Handler.Init.Prescaler = (84-1);                     	//分频系数
    TIM7_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    		//向上计数器
    TIM7_Handler.Init.Period = tim7arr;                        	//自动装载值
    TIM7_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;		//时钟分频因子
	TIM7_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_Base_Init(&TIM7_Handler);
	
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&TIM7_Handler, &sMasterConfig);
    
    HAL_TIM_Base_Start_IT(&TIM7_Handler); //使能定时器6和定时器7更新中断：TIM_IT_UPDATE 
}


//定时器7中断服务函数
void TIM7_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM7_Handler);
}

#endif




/*__________________________________低功耗StandbyMode__________________________________________*/
#if SYSTEM_StdbyWKUP_ENABLE

void sys_StdbyWKUP_ENABLE(void)
{
	#if !SYSTEM_SUPPORT_OS
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_0;            //PA0
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;  //中断,上升沿
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//快速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
	/*PA0外部中断函数在 HAL_GPIO_EXTI_Callback 里*/
    
//    //检查是否是正常开机
//    if(Check_WKUP()==0)
//    {
//        Sys_Enter_Standby();//不是开机，进入待机模式
//    }

    HAL_NVIC_SetPriority(EXTI0_IRQn,2,0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	
	#else
	
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_0;            //PA0
    GPIO_Initure.Mode=GPIO_MODE_INPUT;  //输入检测
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//快速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
	#endif
}


//系统进入待机模式
void Sys_Enter_Standby(void)
{
	while(WKUP_KD == 1){;} //WKUP按键松开后，下面会延时1s，用于准备和处理关机前的事务
	HAL_Delay(500);
	while(WKUP_KD == 1){;}
	HAL_Delay(500);
	
    __HAL_RCC_APB2_FORCE_RESET();       //复位所有IO口 
	__HAL_RCC_PWR_CLK_ENABLE();
		
//	#if SYSTEM_RTC_ENABLE
//		__HAL_RCC_BACKUPRESET_FORCE();      //复位备份区域
//		HAL_PWR_EnableBkUpAccess();         //后备区域访问使能  
//		
//		//STM32F4,当开启了RTC相关中断后,必须先关闭RTC中断,再清中断标志位,然后重新设置
//		//RTC中断,再进入待机模式才可以正常唤醒,否则会有问题.	
//		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
//		__HAL_RTC_WRITEPROTECTION_DISABLE(&RTC_Handler);//关闭RTC写保护
//		
//		//关闭RTC相关中断，可能RTC打开了
//		__HAL_RTC_WAKEUPTIMER_DISABLE_IT(&RTC_Handler,RTC_IT_WUT);
//		__HAL_RTC_TIMESTAMP_DISABLE_IT(&RTC_Handler,RTC_IT_TS);
//		__HAL_RTC_ALARM_DISABLE_IT(&RTC_Handler,RTC_IT_ALRA|RTC_IT_ALRB);
//		
//		//清除RTC相关中断标志位
//		__HAL_RTC_ALARM_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_ALRAF|RTC_FLAG_ALRBF);
//		__HAL_RTC_TIMESTAMP_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_TSF); 
//		__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_WUTF);
//		
//		__HAL_RCC_BACKUPRESET_RELEASE();                    //备份区域复位结束
//		__HAL_RTC_WRITEPROTECTION_ENABLE(&RTC_Handler);     //使能RTC写保护
//	#endif
	
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);                  //清除Wake_UP标志
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);           //设置WKUP用于唤醒
    HAL_PWR_EnterSTANDBYMode();                         //进入待机模式     
}

//检测WKUP脚的信号
//返回值1:连续按下3s以上
//      0:错误的触发	
u8 Check_WKUP(void) 
{
	u8 t=0;	//记录按下的时间
	while(1)
	{
		if(WKUP_KD)
		{
			t++;			//已经按下了 
			HAL_Delay(30);
			if(t>=100)		//按下超过3秒钟
			{
				return 1; 	//按下3s以上了
			}
		}else 
		{ 
			return 0; //按下不足3秒
		}
	}
}

void sys_CheckWKUP_4RTOS(void)
{
	static u8 t=0;	//记录按下的时间
	if(WKUP_KD)
	{
		t++;			//已经按下了 
		HAL_Delay(30);
		if(t>=100)		//按下超过3秒钟
		{
			Sys_Enter_Standby();//按下3s以上了，进入待机模式
		}
	}else 
	{} //按下不足3秒
}

#endif



/*________________________________________用户ADC1配置_________________________________________________________*/
#if SYSTEM_ADC1_ENABLE


void ADC_RegularChannelConfig(ADC_HandleTypeDef *AdcHandle, uint32_t Channel, uint32_t Rank, uint32_t SamplingTime)
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;      
    ADC1_ChanConf.Channel      = Channel;                      		//通道
    ADC1_ChanConf.Rank         = Rank;                          	//第几个序列
    ADC1_ChanConf.SamplingTime = SamplingTime;                  	//采样时间 
    HAL_ADC_ConfigChannel(AdcHandle,&ADC1_ChanConf);              	//通道配置
}

ADC_HandleTypeDef ADC1_Handler;		//ADC句柄
void sys_ADC1_ENABLE(void)
{
	u8 i;
	i = 1;
	
	#if SYSTEM_ADC1_useScan
		/*先初始化DMA，再初始化ADC*/
		ADC_DMA_Cfg();
		//HAL_ADC_Start_DMA(&ADC1_Handler, (uint32_t*)&adValue,12);	//开始DMA，指定接收数组，最后一个参数为数据长度（要传多少次）
	#endif
	
    ADC1_Handler.Instance=ADC1;
	ADC1_Handler.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2; //也不知道这时钟是多少
    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //右对齐
	ADC1_Handler.Init.Resolution = ADC_RESOLUTION_12B;
	
	if(!SYSTEM_ADC1_useScan)
	{
		ADC1_Handler.Init.ScanConvMode=DISABLE;                  //非扫描模式
		ADC1_Handler.Init.ContinuousConvMode=DISABLE,
		ADC1_Handler.Init.DMAContinuousRequests = DISABLE;
		ADC1_Handler.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	}else{
		ADC1_Handler.Init.ScanConvMode=ENABLE;					 //扫描模式
		
		if(SYSTEM_ADC1_useCircular)
		{	ADC1_Handler.Init.ContinuousConvMode=ENABLE;               //连续转换 开启：本次转换玩规则通道内全部通道后又自动启动下一次转换；不开启：触发一次转换一次；
			ADC1_Handler.Init.DMAContinuousRequests = ENABLE;
			ADC1_Handler.Init.EOCSelection = ADC_EOC_SEQ_CONV;
		}
		else{
			ADC1_Handler.Init.ContinuousConvMode=DISABLE;
			ADC1_Handler.Init.DMAContinuousRequests = DISABLE;
			ADC1_Handler.Init.EOCSelection = ADC_EOC_SEQ_CONV;
		}
	}
	
	if(SYSTEM_ADC1_useScan)
		ADC1_Handler.Init.NbrOfConversion=SYSTEM_ADC1_useChanlNum;   	//n个转换在规则序列中
	else ADC1_Handler.Init.NbrOfConversion=1;							//只转换规则序列1
	
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;             //禁止不连续采样模式
    ADC1_Handler.Init.NbrOfDiscConversion=0;                     //不连续采样通道数为0
	ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;  	 //软件触发
	ADC1_Handler.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    HAL_ADC_Init(&ADC1_Handler);                                 //初始化 
	
	//HAL_ADCEx_Calibration_Start(&ADC1_Handler);					 //校准ADC
	
	if(SYSTEM_ADC1_useScan)		//如果启用扫描，则把所有通道都加入到规则组里
	{               
		if((SYSTEM_ADC1_useChanl) & B1in16) 	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_0, (uint32_t)(i++), ADC_SAMPLETIME_56CYCLES);
		if((SYSTEM_ADC1_useChanl) & B2in16) 	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_1, (uint32_t)(i++), ADC_SAMPLETIME_56CYCLES);
		if((SYSTEM_ADC1_useChanl) & B3in16) 	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_2, (uint32_t)(i++), ADC_SAMPLETIME_56CYCLES);
		if((SYSTEM_ADC1_useChanl) & B4in16) 	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_3, (uint32_t)(i++), ADC_SAMPLETIME_56CYCLES);
		if((SYSTEM_ADC1_useChanl) & B5in16) 	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_4, (uint32_t)(i++), ADC_SAMPLETIME_56CYCLES);
		if((SYSTEM_ADC1_useChanl) & B6in16) 	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_5, (uint32_t)(i++), ADC_SAMPLETIME_56CYCLES);
		if((SYSTEM_ADC1_useChanl) & B7in16) 	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_6, (uint32_t)(i++), ADC_SAMPLETIME_56CYCLES);
		if((SYSTEM_ADC1_useChanl) & B8in16) 	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_7, (uint32_t)(i++), ADC_SAMPLETIME_56CYCLES);
		if((SYSTEM_ADC1_useChanl) & B9in16) 	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_8, (uint32_t)(i++), ADC_SAMPLETIME_56CYCLES);
		if((SYSTEM_ADC1_useChanl) & B10in16)	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_9, (uint32_t)(i++), ADC_SAMPLETIME_56CYCLES);
		if((SYSTEM_ADC1_useChanl) & B11in16)	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_10, (uint32_t)(i++), ADC_SAMPLETIME_56CYCLES);
		if((SYSTEM_ADC1_useChanl) & B12in16)	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_11, (uint32_t)(i++), ADC_SAMPLETIME_56CYCLES);
		if((SYSTEM_ADC1_useChanl) & B13in16)	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_12, (uint32_t)(i++), ADC_SAMPLETIME_56CYCLES);
		if((SYSTEM_ADC1_useChanl) & B14in16)	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_13, (uint32_t)(i++), ADC_SAMPLETIME_56CYCLES);
		if((SYSTEM_ADC1_useChanl) & B15in16) 	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_14, (uint32_t)(i++), ADC_SAMPLETIME_56CYCLES);
		//if((SYSTEM_ADC1_useChanl) & 0x10) 	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_15, (uint32_t)(i++), ADC_SAMPLETIME_56CYCLES);
		
		if((SYSTEM_ADC1_useChanl) & B16in16) 	ADC_RegularChannelConfig(&ADC1_Handler, ADC_CHANNEL_TEMPSENSOR, (uint32_t)(i++), ADC_SAMPLETIME_56CYCLES);
	}
}

//此函数会被HAL_ADC_Init()调用
//hadc:ADC句柄
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
/*	
	通道：	0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	   16		     17
	IO	：	A0	A1	A2	A3	A4	A5	A6	A7	B0	B1	C0	C1	C2	C3	C4	C5	内部温度	内部参考电压
*/
	if(hadc->Instance == ADC1)
	{
		GPIO_InitTypeDef GPIO_Initure;
		__HAL_RCC_ADC1_CLK_ENABLE();            //使能ADC1时钟
		__HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
		__HAL_RCC_GPIOB_CLK_ENABLE();
		
		GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
		GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
		               
		if((SYSTEM_ADC1_useChanl) & B1in16) {
			GPIO_Initure.Pin=GPIO_PIN_0; 
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);
			}
		if((SYSTEM_ADC1_useChanl) & B2in16) {
			GPIO_Initure.Pin=GPIO_PIN_1; 
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);
			}
		if((SYSTEM_ADC1_useChanl) & B3in16) {
			GPIO_Initure.Pin=GPIO_PIN_2; 
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);
			}
		if((SYSTEM_ADC1_useChanl) & B4in16) {
			GPIO_Initure.Pin=GPIO_PIN_3; 
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);
			}
		if((SYSTEM_ADC1_useChanl) & B5in16) {GPIO_Initure.Pin=GPIO_PIN_4; HAL_GPIO_Init(GPIOA,&GPIO_Initure);}
		if((SYSTEM_ADC1_useChanl) & B6in16) {GPIO_Initure.Pin=GPIO_PIN_5; HAL_GPIO_Init(GPIOA,&GPIO_Initure);}
		if((SYSTEM_ADC1_useChanl) & B7in16) {GPIO_Initure.Pin=GPIO_PIN_6; HAL_GPIO_Init(GPIOA,&GPIO_Initure);}
		if((SYSTEM_ADC1_useChanl) & B8in16) {GPIO_Initure.Pin=GPIO_PIN_7; HAL_GPIO_Init(GPIOA,&GPIO_Initure);}
		if((SYSTEM_ADC1_useChanl) & B9in16) {GPIO_Initure.Pin=GPIO_PIN_0; HAL_GPIO_Init(GPIOB,&GPIO_Initure);}
		if((SYSTEM_ADC1_useChanl) & B10in16) {GPIO_Initure.Pin=GPIO_PIN_1; HAL_GPIO_Init(GPIOB,&GPIO_Initure);}
		if((SYSTEM_ADC1_useChanl) & B11in16) {GPIO_Initure.Pin=GPIO_PIN_0; HAL_GPIO_Init(GPIOC,&GPIO_Initure);}
		if((SYSTEM_ADC1_useChanl) & B12in16) {GPIO_Initure.Pin=GPIO_PIN_1; HAL_GPIO_Init(GPIOC,&GPIO_Initure);}
		if((SYSTEM_ADC1_useChanl) & B13in16) {GPIO_Initure.Pin=GPIO_PIN_2; HAL_GPIO_Init(GPIOC,&GPIO_Initure);}
		if((SYSTEM_ADC1_useChanl) & B14in16) {GPIO_Initure.Pin=GPIO_PIN_3; HAL_GPIO_Init(GPIOC,&GPIO_Initure);}
		if((SYSTEM_ADC1_useChanl) & B15in16) {GPIO_Initure.Pin=GPIO_PIN_4; HAL_GPIO_Init(GPIOC,&GPIO_Initure);}
		//if((SYSTEM_ADC1_useChanl) & B166in16) {GPIO_Initure.Pin=GPIO_PIN_5; HAL_GPIO_Init(GPIOC,&GPIO_Initure);}  留作内部温度通道，第16个ADC1通道不可用
	}
}


u16 adValue[SYSTEM_ADC1_useChanlNum];		  /*DMA1把ADC转换结果传送的目标位置*/
u8 adValueDone = 0;			/*DMA把ADC1的值传送到adValue完成标志*/

//获得ADC值
//ch: 通道值 0~16，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
//返回值:转换结果
u16 Get_Adc(u32 ch)   
{
	ADC_ChannelConfTypeDef ADC1_ChanConf;
	if (!SYSTEM_ADC1_useScan)
	{	
		ADC1_ChanConf.Channel=ch;                                   //通道
		ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
		ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;      	//采样时间               
		HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //通道配置
		
		HAL_ADC_Start(&ADC1_Handler);                               //开启ADC
		HAL_ADC_PollForConversion(&ADC1_Handler,2);                 //死等
		return (u16)HAL_ADC_GetValue(&ADC1_Handler);	        	//返回最近一次ADC1规则组的转换结果
	}else
	{
		return 2333;
	}
}

u32 Get_Adc_Average(u32 ch,u8 times)
{
	u32 temp_val = 0;
	u8 t;
	
	for(t=0;t < times;t++)
	{
		temp_val += Get_Adc(ch);
		HAL_Delay(1);
	}
	return (temp_val/times);
}

/*把AD采集温度通道的原始值，转化为温度值*/
float Get_Temprate(u32 adcx)
{
 	float temperate;
	temperate = ((float)adcx)*(3.3f/4096.0f);				//电压值
	temperate=(temperate-0.76f)/0.0025f + 25.0f;			//转换为温度值 	 
	return temperate;
}

#if SYSTEM_ADC1_useScan
DMA_HandleTypeDef  ADC1rxDMA_Handler; //DMA句柄

//DMA1的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
void ADC_DMA_Cfg(void)
{
	__HAL_RCC_DMA2_CLK_ENABLE();

    ADC1rxDMA_Handler.Instance = DMA2_Stream0;                                 //DMA Stream0
	ADC1rxDMA_Handler.Init.Channel = DMA_CHANNEL_0;
    ADC1rxDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;                     //外设到存储器
    ADC1rxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                        //外设非增量模式
    ADC1rxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                             //存储器增量模式
    ADC1rxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_HALFWORD;        //外设数据长度:16位
    ADC1rxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_HALFWORD;          //存储器数据长度:16位
	
	if(SYSTEM_ADC1_useCircular)
		ADC1rxDMA_Handler.Init.Mode=DMA_CIRCULAR;                                 	//如果ADC选择连续模式，这里是循环，如果不开启连续模式，这里是正常模式
	else ADC1rxDMA_Handler.Init.Mode=DMA_NORMAL;
   
	ADC1rxDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;                       //优先级

    //ADC1rxDMA_Handler.XferCpltCallback = HAL_DMA_IRQHandler;
	ADC1rxDMA_Handler.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    
	HAL_DMA_DeInit(&ADC1rxDMA_Handler);                                      //DMA复位
    HAL_DMA_Init(&ADC1rxDMA_Handler);                                        //DMA初始化 
        
		/*			数据源   数据源句柄中的DMA变量	  使用的DMA句柄*/
	__HAL_LINKDMA(&ADC1_Handler,DMA_Handle,ADC1rxDMA_Handler);               //将DMA与ADC联系起来(发送DMA)
	/*把DMA句柄ADC1rxDMA_Handler赋给ADC1_Handler的DMA_Handle*/
        
    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 2, 0);                          //DMA中断优先级 必须开，勿动！
    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);                                  //使能DMA中断
}

//DMA2 Stream0中断服务函数，完成传送一次时的中断，不用，因为要等AD全部规则通道转换完后再产生一次中断
void DMA2_Stream0_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&ADC1rxDMA_Handler); /*清除中断标志，并判断是否完成全部转换，勿动！*/
}

//HAL_DMA_IRQHandler的回调函数，ADC转换完成所有规则组通道后的中断函数
/*HAL_ADC_Start_DMA这个函数把HAL_ADC_ConvCpltCallback设为ADC1rxDMA_Handler这个DMA句柄的回调函数了，
自动的，所以只需开DMA的中断，不需要开ADC中断，当所有通道都转换好后，DMA中断就会回调这个函数*/
/*TODO：试验一下，把ADC1rxDMA_Handler.XferCpltCallback = HAL_DMA_IRQHandler;这句话注释掉，应该不影响效果，
因为HAL_ADC_Start_DMA这个函数必须要最后调用，所以DMA的回调函数被更新了！*/
/*总之，把ADC1rxDMA_Handler.XferCpltCallback = HAL_DMA_IRQHandler;注释掉，剩下的部分就是最标准的写法，大概*/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	adValueDone = 1; /*标志本次规则组通道全部转换完成，勿动！*/
}

#endif

#endif


/*_________________________________________________DAC_________________________________________________________*/
#if ((SYSTEM_DAC_OUT1_ENABLE) || (SYSTEM_DAC_OUT2_ENABLE))

DAC_HandleTypeDef DAC1_Handler;//DAC句柄

void sys_DAC_ENABLE(void)
{
	DAC_ChannelConfTypeDef DACCH1_Config;
	DAC_ChannelConfTypeDef DACCH2_Config;
	
	DAC1_Handler.Instance=DAC;
	HAL_DAC_Init(&DAC1_Handler);                 			//初始化DAC
    
		/*通道1配置*/
		DACCH1_Config.DAC_Trigger=DAC_TRIGGER_SOFTWARE;             		//触发功能
							/*可选：
								DAC_TRIGGER_T6_TRGO
								DAC_TRIGGER_T7_TRGO
								DAC_TRIGGER_T8_TRGO
								DAC_TRIGGER_T2_TRGO
								DAC_TRIGGER_T4_TRGO
								DAC_TRIGGER_EXT_IT9
								DAC_TRIGGER_SOFTWARE*/
		DACCH1_Config.DAC_OutputBuffer=DAC_OUTPUTBUFFER_ENABLE;		//DAC1输出缓冲
		/*通道2配置*/
		DACCH2_Config.DAC_Trigger=DAC_TRIGGER_SOFTWARE;             	//触发功能
		DACCH2_Config.DAC_OutputBuffer=DAC_OUTPUTBUFFER_ENABLE;		//DAC1输出缓冲
		
		if(SYSTEM_DAC_OUT1_ENABLE)
		{	HAL_DAC_ConfigChannel(&DAC1_Handler,&DACCH1_Config,DAC_CHANNEL_1);//DAC通道1配置
			HAL_DAC_Start(&DAC1_Handler,DAC_CHANNEL_1);  			//开启DAC通道1
		}
		
		if(SYSTEM_DAC_OUT2_ENABLE)
		{	HAL_DAC_ConfigChannel(&DAC1_Handler,&DACCH2_Config,DAC_CHANNEL_2);//DAC通道2配置
			HAL_DAC_Start(&DAC1_Handler,DAC_CHANNEL_2);  			//开启DAC通道2
		}
    
	#if SYSTEM_DAC_OUT1_TriangleWave_ENABLE
		HAL_DACEx_TriangleWaveGenerate(&DAC1_Handler, DAC_CHANNEL_1, DAC_TRIANGLEAMPLITUDE_1);
			//可选DAC_TRIANGLEAMPLITUDE_1 ~ DAC_TRIANGLEAMPLITUDE_4095 TODO:不知道这个参数干什么的，有待实验
	#elif SYSTEM_DAC_OUT1_NoiseWave_ENABLE
		HAL_DACEx_NoiseWaveGenerate(&DAC1_Handler, DAC_CHANNEL_1,DAC_LFSRUNMASK_BITS1_0);
			//可选DAC_LFSRUNMASK_BITS1_0 ~ DAC_LFSRUNMASK_BITS11_0 TODO:不知道这个参数干什么的，有待实验
	#endif
	
	#if SYSTEM_DAC_OUT2_TriangleWave_ENABLE
		HAL_DACEx_TriangleWaveGenerate(&DAC1_Handler, DAC_CHANNEL_2, DAC_TRIANGLEAMPLITUDE_1);
			//可选DAC_TRIANGLEAMPLITUDE_1 ~ DAC_TRIANGLEAMPLITUDE_4095 
	#elif SYSTEM_DAC_OUT2_NoiseWave_ENABLE
		HAL_DACEx_NoiseWaveGenerate(&DAC1_Handler, DAC_CHANNEL_2,DAC_LFSRUNMASK_BITS1_0);
			//可选DAC_LFSRUNMASK_BITS1_0 ~ DAC_LFSRUNMASK_BITS11_0 
	#endif
}

//DAC底层驱动，时钟配置，引脚 配置
//此函数会被HAL_DAC_Init()调用
//hdac:DAC句柄
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{      
	if(SYSTEM_DAC_OUT1_ENABLE)
	{
		GPIO_InitTypeDef GPIO_Initure;
		__HAL_RCC_DAC_CLK_ENABLE();             //使能DAC时钟
		__HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
		
		GPIO_Initure.Pin=GPIO_PIN_4;            //PA4
		GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
		GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	}
	
	if(SYSTEM_DAC_OUT2_ENABLE)
	{
		GPIO_InitTypeDef GPIO_Initure;
		__HAL_RCC_DAC_CLK_ENABLE();             //使能DAC时钟
		__HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
		
		GPIO_Initure.Pin=GPIO_PIN_5;            //PA5
		GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
		GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	}
}

//设置通道1输出电压
//vol:0~3.30V
void DAC_Set_Ch1_Vol(float vol)
{
	float temp = vol;
	temp = (temp/3.30f)*4096.0f;
    HAL_DAC_SetValue(&DAC1_Handler,DAC_CHANNEL_1,DAC_ALIGN_12B_R,(uint32_t)temp);//12位右对齐数据格式设置DAC值
}

//设置通道2输出电压
//vol:0~3.30V
void DAC_Set_Ch2_Vol(float vol)
{
	float temp = vol;
	temp = (temp/3.30f)*4096.0f;
    HAL_DAC_SetValue(&DAC1_Handler,DAC_CHANNEL_2,DAC_ALIGN_12B_R,(uint32_t)temp);//12位右对齐数据格式设置DAC值
}


#endif


/*________________________________________________FSMC_________________________________________________________*/
#if ((SYSTEM_FSMC_ENABLE) && (SYSTEM_FSMC_use4SRAM))

SRAM_HandleTypeDef SRAM_Handler;    //SRAM句柄

//SRAM初始化
void sys_FSMC_SRAM_ENABLE(void)
{	
	FSMC_NORSRAM_TimingTypeDef FSMC_ReadWriteTim;
	GPIO_InitTypeDef GPIO_Initure;
	
//	XmRamInit();
//	HAL_Delay(1);
		
		__HAL_RCC_FSMC_CLK_ENABLE();                //使能FSMC时钟
		__HAL_RCC_GPIOD_CLK_ENABLE();               //使能GPIOD时钟
		__HAL_RCC_GPIOE_CLK_ENABLE();               //使能GPIOE时钟
		__HAL_RCC_GPIOF_CLK_ENABLE();               //使能GPIOF时钟
		__HAL_RCC_GPIOG_CLK_ENABLE();               //使能GPIOG时钟
		
		//PD0,1,4,5,8~15
		GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|\
						 GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|\
						 GPIO_PIN_14|GPIO_PIN_15;  
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //推挽复用
		GPIO_Initure.Pull=GPIO_PULLUP;              //
		GPIO_Initure.Speed=GPIO_SPEED_HIGH;    //高速 
		GPIO_Initure.Alternate = GPIO_AF12_FSMC;
		HAL_GPIO_Init(GPIOD,&GPIO_Initure);     	
		
		//PE0,1,7~15
		GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|\
						 GPIO_PIN_10| GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|\
						 GPIO_PIN_15;    
		HAL_GPIO_Init(GPIOE,&GPIO_Initure);    
		
		//PF0~5,12~15
		GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|\
						 GPIO_PIN_5|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;              
		HAL_GPIO_Init(GPIOF,&GPIO_Initure);     
		
		//PG0~5,10
		GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_10;              
		HAL_GPIO_Init(GPIOG,&GPIO_Initure);  
		
		
	SRAM_Handler.Instance=FSMC_NORSRAM_DEVICE;                
	SRAM_Handler.Extended=FSMC_NORSRAM_EXTENDED_DEVICE;    
	
	SRAM_Handler.Init.NSBank=FSMC_NORSRAM_BANK3;     					//使用NE3
	SRAM_Handler.Init.DataAddressMux=FSMC_DATA_ADDRESS_MUX_DISABLE; 	//地址/数据线不复用
	SRAM_Handler.Init.MemoryType=FSMC_MEMORY_TYPE_SRAM;   				//SRAM
	SRAM_Handler.Init.MemoryDataWidth=FSMC_NORSRAM_MEM_BUS_WIDTH_16; 	//16位数据宽度
	SRAM_Handler.Init.BurstAccessMode=FSMC_BURST_ACCESS_MODE_DISABLE; 	//是否使能突发访问,仅对同步突发存储器有效,此处未用到
	SRAM_Handler.Init.WaitSignalPolarity=FSMC_WAIT_SIGNAL_POLARITY_LOW;	//等待信号的极性,仅在突发模式访问下有用
	//SRAM_Handler.Init.WrapMode = FSMC_WRAP_MODE_DISABLE; 
	SRAM_Handler.Init.WaitSignalActive=FSMC_WAIT_TIMING_BEFORE_WS;   	//存储器是在等待周期之前的一个时钟周期还是等待周期期间使能NWAIT
	SRAM_Handler.Init.WriteOperation=FSMC_WRITE_OPERATION_ENABLE;    	//存储器写使能
	SRAM_Handler.Init.WaitSignal=FSMC_WAIT_SIGNAL_DISABLE;           	//等待使能位,此处未用到
	SRAM_Handler.Init.ExtendedMode=FSMC_EXTENDED_MODE_DISABLE;        	//读写使用相同的时序
	SRAM_Handler.Init.AsynchronousWait=FSMC_ASYNCHRONOUS_WAIT_DISABLE;	//是否使能同步传输模式下的等待信号,此处未用到
	SRAM_Handler.Init.WriteBurst=FSMC_WRITE_BURST_DISABLE;           	//禁止突发写
	SRAM_Handler.Init.ContinuousClock=FSMC_CONTINUOUS_CLOCK_SYNC_ASYNC;
  
	//FMC读时序控制寄存器
	FSMC_ReadWriteTim.AddressSetupTime=0x0f;       	//地址建立时间（ADDSET） 多少个HCLK，可以改成0x00试试：为1个HCLK 1/168M=6ns*16=96ns
	FSMC_ReadWriteTim.AddressHoldTime=0x0f;			//地址保持时间（ADDHLD）模式A未用到
	FSMC_ReadWriteTim.DataSetupTime=0x18;			//数据保存时间为18个HCLK
	FSMC_ReadWriteTim.BusTurnAroundDuration=0X00;
	FSMC_ReadWriteTim.AccessMode=FSMC_ACCESS_MODE_A;//模式A
	HAL_SRAM_Init(&SRAM_Handler,&FSMC_ReadWriteTim,&FSMC_ReadWriteTim);	
	
}

//在指定地址(WriteAddr+Bank1_SRAM3_ADDR)开始,连续写入n个字节.
//pBuffer:字节指针
//WriteAddr:要写入的地址
//n:要写入的字节数
void FSMC_SRAM_WriteBuffer(u8 *pBuffer,u32 WriteAddr,u32 n)
{
	for(;n!=0;n--)
	{
		*(vu8*)(SRAM1_BANK3_ADDR + WriteAddr) = *pBuffer;
		WriteAddr++;
		pBuffer++;
	}
}

//在指定地址((WriteAddr+Bank1_SRAM3_ADDR))开始,连续读出n个字节.
//pBuffer:字节指针
//ReadAddr:要读出的起始地址
//n:要写入的字节数
void FSMC_SRAM_ReadBuffer(u8 *pBuffer,u32 ReadAddr,u32 n)
{
	for(;n!=0;n--)
	{
		*pBuffer++ = *(vu8*)(SRAM1_BANK3_ADDR + ReadAddr);
		ReadAddr++;
	}
}

#endif

#if ((SYSTEM_FSMC_ENABLE) && (SYSTEM_FSMC_use4LCD))

SRAM_HandleTypeDef TFTSRAM_Handler;    //SRAM句柄(用于控制LCD)

void LCD_with_FSMC_init_FSMC(void)
{
	FSMC_NORSRAM_TimingTypeDef FSMC_ReadWriteTim;
	FSMC_NORSRAM_TimingTypeDef FSMC_WriteTim;
	
		GPIO_InitTypeDef GPIO_Initure;
		
		__HAL_RCC_FSMC_CLK_ENABLE();			//使能FSMC时钟
		__HAL_RCC_GPIOD_CLK_ENABLE();			//使能GPIOD时钟
		__HAL_RCC_GPIOE_CLK_ENABLE();			//使能GPIOE时钟
		__HAL_RCC_GPIOG_CLK_ENABLE();			//使能GPIOG时钟
		
		//初始化PD0,1,4,5,8,9,10,14,15
		GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|\
						 GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_15;
		GPIO_Initure.Mode=GPIO_MODE_AF_PP; 		//推挽复用
		GPIO_Initure.Pull=GPIO_NOPULL;			//
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//高速
		GPIO_Initure.Alternate = GPIO_AF12_FSMC;
		HAL_GPIO_Init(GPIOD,&GPIO_Initure);     //初始化
		
		//初始化PE7,8,9,10,11,12,13,14,15
		GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|\
						 GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
		HAL_GPIO_Init(GPIOE,&GPIO_Initure);
		
		//初始化PG0,12
		GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_12;
		HAL_GPIO_Init(GPIOG,&GPIO_Initure);
		
	TFTSRAM_Handler.Instance=FSMC_NORSRAM_DEVICE;                
	TFTSRAM_Handler.Extended=FSMC_NORSRAM_EXTENDED_DEVICE;    
    
	TFTSRAM_Handler.Init.NSBank=FSMC_NORSRAM_BANK4;     				//使用NE4
	TFTSRAM_Handler.Init.DataAddressMux=FSMC_DATA_ADDRESS_MUX_DISABLE; 	//地址/数据线不复用
	TFTSRAM_Handler.Init.MemoryType=FSMC_MEMORY_TYPE_SRAM;   			//SRAM
	TFTSRAM_Handler.Init.MemoryDataWidth=FSMC_NORSRAM_MEM_BUS_WIDTH_16; //16位数据宽度
	TFTSRAM_Handler.Init.BurstAccessMode=FSMC_BURST_ACCESS_MODE_DISABLE; //是否使能突发访问,仅对同步突发存储器有效,此处未用到
	TFTSRAM_Handler.Init.WaitSignalPolarity=FSMC_WAIT_SIGNAL_POLARITY_LOW;//等待信号的极性,仅在突发模式访问下有用
	TFTSRAM_Handler.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
	TFTSRAM_Handler.Init.WaitSignalActive=FSMC_WAIT_TIMING_BEFORE_WS;   //存储器是在等待周期之前的一个时钟周期还是等待周期期间使能NWAIT
	TFTSRAM_Handler.Init.WriteOperation=FSMC_WRITE_OPERATION_ENABLE;    //存储器写使能
	TFTSRAM_Handler.Init.WaitSignal=FSMC_WAIT_SIGNAL_DISABLE;           //等待使能位,此处未用到
	TFTSRAM_Handler.Init.ExtendedMode=FSMC_EXTENDED_MODE_ENABLE;        //读写使用不同的时序
	TFTSRAM_Handler.Init.AsynchronousWait=FSMC_ASYNCHRONOUS_WAIT_DISABLE;//是否使能同步传输模式下的等待信号,此处未用到
	TFTSRAM_Handler.Init.WriteBurst=FSMC_WRITE_BURST_DISABLE;           //禁止突发写
    
	//FMC读时序控制寄存器
	FSMC_ReadWriteTim.AddressSetupTime=0x0F;       	//地址建立时间（ADDSET）为几个HCLK
	FSMC_ReadWriteTim.AddressHoldTime=0;
	FSMC_ReadWriteTim.DataSetupTime=60;				//数据保存时间
	FSMC_ReadWriteTim.AccessMode=FSMC_ACCESS_MODE_A;//模式A
	//FMC写时序控制寄存器
	FSMC_WriteTim.BusTurnAroundDuration=0;			//总线周转阶段持续时间为0，此变量不赋值的话会莫名其妙的自动修改为4。导致程序运行正常
	FSMC_WriteTim.AddressSetupTime=9;          		//地址建立时间（ADDSET）为几个HCLK
	FSMC_WriteTim.AddressHoldTime=0;
	FSMC_WriteTim.DataSetupTime=8;              	//数据保存时间
	FSMC_WriteTim.AccessMode=FSMC_ACCESS_MODE_A;    //模式A
	HAL_SRAM_Init(&TFTSRAM_Handler,&FSMC_ReadWriteTim,&FSMC_WriteTim);	
	
	HAL_Delay(50); // delay 50 ms 
}

#endif

/*________________________________________用户FLASH编程_________________________________________________________*/
#if SYSTEM_FLASH_IAP_ENABLE

//读取指定地址的字(32位数据) 
//faddr:读地址 
//返回值:对应数据.
u32 STMFLASH_ReadWord(u32 faddr)
{
	return *(vu32*)faddr; 
}

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToRead:字(32位)数
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)   	
{
	u32 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//读取4个字节.
		ReadAddr+=4;//偏移4个字节.	
	}
}


#if STM32_FLASH_WREN	//如果使能了写   

//获取某个地址所在的flash扇区
//addr:flash地址
//返回值:0~11,即addr所在的扇区
u8 STMFLASH_GetFlashSector(u32 addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return FLASH_SECTOR_0;
	else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_SECTOR_1;
	else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_SECTOR_2;
	else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_SECTOR_3;
	else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_SECTOR_4;
	else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_SECTOR_5;
	else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_SECTOR_6;
	else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_SECTOR_7;
	else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_SECTOR_8;
	else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_SECTOR_9;
	else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_SECTOR_10;   
	return FLASH_SECTOR_11;	
}

//从指定地址开始写入指定长度的数据
//特别注意:因为STM32F4的扇区实在太大,没办法本地保存扇区数据,所以本函数
//         写地址如果非0XFF,那么会先擦除整个扇区且不保存扇区数据.所以
//         写非0XFF的地址,将导致整个扇区数据丢失.建议写之前确保扇区里
//         没有重要数据,最好是整个扇区先擦除了,然后慢慢往后写. 
//该函数对OTP区域也有效!可以用来写OTP区!
//OTP区域地址范围:0X1FFF7800~0X1FFF7A0F(注意：最后16字节，用于OTP数据块锁定，别乱写！！)
//WriteAddr:起始地址(此地址必须为4的倍数!!)
//pBuffer:数据指针
//NumToWrite:字(32位)数(就是要写入的32位数据的个数.) 
void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)	
{ 
	FLASH_EraseInitTypeDef FlashEraseInit;
	HAL_StatusTypeDef FlashStatus=HAL_OK;
	u32 SectorError=0;
	u32 addrx=0;
	u32 endaddr=0;	
	if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return;	//非法地址
    
	HAL_FLASH_Unlock();             //解锁	
	addrx=WriteAddr;				//写入的起始地址
	endaddr=WriteAddr+NumToWrite*4;	//写入的结束地址
    
	if(addrx<0X1FFF0000)
	{
		while(addrx<endaddr)		//扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
		{
			 if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//有非0XFFFFFFFF的地方,要擦除这个扇区
			{   
				FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //擦除类型，扇区擦除 
				FlashEraseInit.Sector=STMFLASH_GetFlashSector(addrx);   //要擦除的扇区
				FlashEraseInit.NbSectors=1;                             //一次只擦除一个扇区
				FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //电压范围，VCC=2.7~3.6V之间!!
				if(HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError)!=HAL_OK) 
				{
					break;//发生错误了	
				}
				}else addrx+=4;
				FLASH_WaitForLastOperation(FLASH_WAITETIME);                //等待上次操作完成
		}
	}
	FlashStatus=FLASH_WaitForLastOperation(FLASH_WAITETIME);            //等待上次操作完成
	if(FlashStatus==HAL_OK)
	{
		 while(WriteAddr<endaddr)//写数据
		 {
			if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,WriteAddr,*pBuffer)!=HAL_OK)//写入数据
			{ 
				break;	//写入异常
			}
			WriteAddr+=4;
			pBuffer++;
		}  
	}
	HAL_FLASH_Lock();           //上锁
} 

#endif

#endif


/*____________________SDIO SD_____________________________________________*/
#if SYSTEM_SDIO_SD_ENABLE

SD_HandleTypeDef        SDCARD_Handler;     //SD卡句柄
HAL_SD_CardInfoTypeDef  SDCardInfo;         //SD卡信息结构体
DMA_HandleTypeDef SDTxDMAHandler,SDRxDMAHandler;    //SD卡DMA发送和接收句柄

//SD_ReadDisk/SD_WriteDisk函数专用buf,当这两个函数的数据缓存区地址不是4字节对齐的时候,
//需要用到该数组,确保数据缓存区地址是4字节对齐的.
__align(4) u8 SDIO_DATA_BUFFER[512];

//SD卡初始化
//返回值:0 初始化正确；其他值，初始化错误
u8 SD_Init(void)
{
    u8 SD_Error;
    
    //初始化时的时钟不能大于400KHZ 
    SDCARD_Handler.Instance=SDIO;
    SDCARD_Handler.Init.ClockEdge=SDIO_CLOCK_EDGE_RISING;          			//上升沿     
    SDCARD_Handler.Init.ClockBypass=SDIO_CLOCK_BYPASS_DISABLE;     			//不使用bypass模式，直接用HCLK进行分频得到SDIO_CK
    SDCARD_Handler.Init.ClockPowerSave=SDIO_CLOCK_POWER_SAVE_DISABLE;    	//空闲时不关闭时钟电源
    SDCARD_Handler.Init.BusWide=SDIO_BUS_WIDE_1B;                        	//1位数据线
    SDCARD_Handler.Init.HardwareFlowControl=SDIO_HARDWARE_FLOW_CONTROL_DISABLE;//关闭硬件流控
    SDCARD_Handler.Init.ClockDiv=SDIO_TRANSFER_CLK_DIV;            			//SD传输时钟频率最大25MHZ
    
    SD_Error=HAL_SD_Init(&SDCARD_Handler);
    if(SD_Error!=HAL_OK) return 1;
	
	//获取SD卡信息
	HAL_SD_GetCardInfo(&SDCARD_Handler,&SDCardInfo);
    
    SD_Error=HAL_SD_ConfigWideBusOperation(&SDCARD_Handler,SDIO_BUS_WIDE_4B);//使能宽总线模式
    if(SD_Error!=HAL_OK) return 2;
    return 0;
}

//SDMMC底层驱动，时钟使能，引脚配置，DMA配置
//此函数会被HAL_SD_Init()调用
//hsd:SD卡句柄
void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
    DMA_HandleTypeDef TxDMAHandler,RxDMAHandler;
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_SDIO_CLK_ENABLE();    //使能SDIO时钟
    __HAL_RCC_DMA2_CLK_ENABLE();    //使能DMA2时钟 
    __HAL_RCC_GPIOC_CLK_ENABLE();   //使能GPIOC时钟
    __HAL_RCC_GPIOD_CLK_ENABLE();   //使能GPIOD时钟
    
    //PC8,9,10,11,12
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //推挽复用
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    GPIO_Initure.Alternate=GPIO_AF12_SDIO;  //复用为SDIO
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);     //初始化
    
    //PD2
    GPIO_Initure.Pin=GPIO_PIN_2;            
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);     //初始化

#if (SD_DMA_MODE==1)                        //使用DMA模式
    HAL_NVIC_SetPriority(SDMMC1_IRQn,2,0);  //配置SDMMC1中断，抢占优先级2，子优先级0
    HAL_NVIC_EnableIRQ(SDMMC1_IRQn);        //使能SDMMC1中断
    
    //配置发送DMA
    SDRxDMAHandler.Instance=DMA2_Stream3;
    SDRxDMAHandler.Init.Channel=DMA_CHANNEL_4;
    SDRxDMAHandler.Init.Direction=DMA_PERIPH_TO_MEMORY;
    SDRxDMAHandler.Init.PeriphInc=DMA_PINC_DISABLE;
    SDRxDMAHandler.Init.MemInc=DMA_MINC_ENABLE;
    SDRxDMAHandler.Init.PeriphDataAlignment=DMA_PDATAALIGN_WORD;
    SDRxDMAHandler.Init.MemDataAlignment=DMA_MDATAALIGN_WORD;
    SDRxDMAHandler.Init.Mode=DMA_PFCTRL;
    SDRxDMAHandler.Init.Priority=DMA_PRIORITY_VERY_HIGH;
    SDRxDMAHandler.Init.FIFOMode=DMA_FIFOMODE_ENABLE;
    SDRxDMAHandler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;
    SDRxDMAHandler.Init.MemBurst=DMA_MBURST_INC4;
    SDRxDMAHandler.Init.PeriphBurst=DMA_PBURST_INC4;

    __HAL_LINKDMA(hsd, hdmarx, SDRxDMAHandler); //将接收DMA和SD卡的发送DMA连接起来
    HAL_DMA_DeInit(&SDRxDMAHandler);
    HAL_DMA_Init(&SDRxDMAHandler);              //初始化接收DMA
    
    //配置接收DMA 
    SDTxDMAHandler.Instance=DMA2_Stream6;
    SDTxDMAHandler.Init.Channel=DMA_CHANNEL_4;
    SDTxDMAHandler.Init.Direction=DMA_MEMORY_TO_PERIPH;
    SDTxDMAHandler.Init.PeriphInc=DMA_PINC_DISABLE;
    SDTxDMAHandler.Init.MemInc=DMA_MINC_ENABLE;
    SDTxDMAHandler.Init.PeriphDataAlignment=DMA_PDATAALIGN_WORD;
    SDTxDMAHandler.Init.MemDataAlignment=DMA_MDATAALIGN_WORD;
    SDTxDMAHandler.Init.Mode=DMA_PFCTRL;
    SDTxDMAHandler.Init.Priority=DMA_PRIORITY_VERY_HIGH;
    SDTxDMAHandler.Init.FIFOMode=DMA_FIFOMODE_ENABLE;
    SDTxDMAHandler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_FULL;
    SDTxDMAHandler.Init.MemBurst=DMA_MBURST_INC4;
    SDTxDMAHandler.Init.PeriphBurst=DMA_PBURST_INC4;
    
    __HAL_LINKDMA(hsd, hdmatx, SDTxDMAHandler);//将发送DMA和SD卡的发送DMA连接起来
    HAL_DMA_DeInit(&SDTxDMAHandler);
    HAL_DMA_Init(&SDTxDMAHandler);              //初始化发送DMA 
  

    HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 3, 0);  //接收DMA中断优先级
    HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
    HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 3, 0);  //发送DMA中断优先级
    HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
#endif
}

//得到卡信息
//cardinfo:卡信息存储区
//返回值:错误状态
u8 SD_GetCardInfo(HAL_SD_CardInfoTypeDef *cardinfo)
{
    u8 sta;
    sta=HAL_SD_GetCardInfo(&SDCARD_Handler,cardinfo);
    return sta;
}

//判断SD卡是否可以传输(读写)数据
//返回值:SD_TRANSFER_OK 传输完成，可以继续下一次传输
//		 SD_TRANSFER_BUSY SD卡正忙，不可以进行下一次传输
u8 SD_GetCardState(void)
{
  return((HAL_SD_GetCardState(&SDCARD_Handler)==HAL_SD_CARD_TRANSFER )?SD_TRANSFER_OK:SD_TRANSFER_BUSY);
}

 #if (SD_DMA_MODE==1)        //DMA模式

//通过DMA读取SD卡一个扇区
//buf:读数据缓存区
//sector:扇区地址
//blocksize:扇区大小(一般都是512字节)
//cnt:扇区个数	
//返回值:错误状态;0,正常;其他,错误代码;
u8 SD_ReadBlocks_DMA(uint32_t *buf,uint64_t sector,uint32_t blocksize,uint32_t cnt)
{
    u8 err=0;
	u32 timeout=SD_TIMEOUT;
	err=HAL_SD_ReadBlocks_DMA(&SDCARD_Handler,(uint8_t*)buf,sector,cnt);//通过DMA读取SD卡n个扇区

	if(err==0)
	{
		//等待SD卡读完
		while(SD_GetCardState()!=SD_TRANSFER_OK)
		{
			if(timeout-- == 0)
			{	
				err=SD_TRANSFER_BUSY;
			}
		} 
	}
    return err;
}

//写SD卡
//buf:写数据缓存区
//sector:扇区地址
//blocksize:扇区大小(一般都是512字节)
//cnt:扇区个数	
//返回值:错误状态;0,正常;其他,错误代码;	
u8 SD_WriteBlocks_DMA(uint32_t *buf,uint64_t sector,uint32_t blocksize,uint32_t cnt)
{
    u8 err=0; 
	u32 timeout=SD_TIMEOUT;
	err=HAL_SD_WriteBlocks_DMA(&SDCARD_Handler,(uint8_t*)buf,sector,cnt);//通过DMA写SD卡n个扇区

	if(err==0)
	{
		//等待SD卡写完
		while(SD_GetCardState()!=SD_TRANSFER_OK)
		{
			if(timeout-- == 0)
			{	
				err=SD_TRANSFER_BUSY;
			}
		} 
	}
    return err;
}

//读SD卡
//buf:读数据缓存区
//sector:扇区地址
//cnt:扇区个数	
//返回值:错误状态;0,正常;其他,错误代码;
u8 SD_ReadDisk(u8* buf,u32 sector,u32 cnt)
{
    u8 sta=HAL_OK;
    long long lsector=sector;
	u32 timeout=SD_TIMEOUT;
    u8 n;
	
	sta=SD_ReadBlocks_DMA((uint32_t*)buf,lsector, 512,cnt);
   
    return sta;
}  

//写SD卡
//buf:写数据缓存区
//sector:扇区地址
//cnt:扇区个数	
//返回值:错误状态;0,正常;其他,错误代码;	
u8 SD_WriteDisk(u8 *buf,u32 sector,u32 cnt)
{   
    u8 sta=HAL_OK;
    long long lsector=sector;
	u32 timeout=SD_TIMEOUT;
    u8 n;

	sta=SD_WriteBlocks_DMA((uint32_t*)buf,lsector,512,cnt);//多个sector的写操作

    return sta;
} 

//SDMMC1中断服务函数
void SDMMC1_IRQHandler(void)
{
    HAL_SD_IRQHandler(&SDCARD_Handler);
}

//DMA2数据流6中断服务函数
void DMA2_Stream6_IRQHandler(void)
{
    HAL_DMA_IRQHandler(SDCARD_Handler.hdmatx);
}

//DMA2数据流3中断服务函数
void DMA2_Stream3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(SDCARD_Handler.hdmarx);
}
#else                                   //轮训模式
 
//读SD卡
//buf:读数据缓存区
//sector:扇区地址
//cnt:扇区个数	
//返回值:错误状态;0,正常;其他,错误代码;
u8 SD_ReadDisk(u8* buf,u32 sector,u32 cnt)
{
    u8 sta=HAL_OK;
	u32 timeout=SD_TIMEOUT;
    long long lsector=sector;
    INTX_DISABLE();//关闭总中断(POLLING模式,严禁中断打断SDIO读写操作!!!)
	sta=HAL_SD_ReadBlocks(&SDCARD_Handler, (uint8_t*)buf,lsector,cnt,SD_TIMEOUT);//多个sector的读操作
	
	//等待SD卡读完
	while(SD_GetCardState()!=SD_TRANSFER_OK)
    {
		if(timeout-- == 0)
		{	
			sta=SD_TRANSFER_BUSY;
		}
    }
    INTX_ENABLE();//开启总中断
    return sta;
}  


//写SD卡
//buf:写数据缓存区
//sector:扇区地址
//cnt:扇区个数	
//返回值:错误状态;0,正常;其他,错误代码;	
u8 SD_WriteDisk(u8 *buf,u32 sector,u32 cnt)
{   
    u8 sta=HAL_OK;
	u32 timeout=SD_TIMEOUT;
    long long lsector=sector;
    INTX_DISABLE();//关闭总中断(POLLING模式,严禁中断打断SDIO读写操作!!!)
	sta=HAL_SD_WriteBlocks(&SDCARD_Handler,(uint8_t*)buf,lsector,cnt,SD_TIMEOUT);//多个sector的写操作
		
	//等待SD卡写完
	while(SD_GetCardState()!=SD_TRANSFER_OK)
    {
		if(timeout-- == 0)
		{	
			sta=SD_TRANSFER_BUSY;
		}
    }    
	INTX_ENABLE();//开启总中断
    return sta;
}
#endif

#endif







