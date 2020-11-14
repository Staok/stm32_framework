#include "can.h"
#include "usart.h"


/*
CAN 		TX:PH13 		RX:PI9
*/


//CAN初始化
//tsjw:重新同步跳跃时间单元.范围:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:时间段2的时间单元.   范围:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:时间段1的时间单元.   范围:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :波特率分频器.范围:1~1024; tq=(brp)*tpclk1
//波特率=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,普通模式;CAN_Mode_LoopBack,回环模式;
//Fpclk1的时钟在初始化的时候设置为42M,如果设置CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_7tq,CAN_BS1_6tq,6,CAN_Mode_Normal);
//则波特率为:42M/((6+7+1)*6)=500Kbps
void CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{
	GPIO_InitTypeDef 		GPIO_InitStructure;
	CAN_InitTypeDef        	CAN_InitStructure;
	CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
	
	
	
	//使能相关时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOI, ENABLE);//使能PORTA时钟	                   											 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟	
	
	//引脚复用映射配置
	GPIO_PinAFConfig(GPIOH,GPIO_PinSource13,GPIO_AF_CAN1); //复用为CAN1
	GPIO_PinAFConfig(GPIOI,GPIO_PinSource9,GPIO_AF_CAN1);
		
	//初始化GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOH, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOI, &GPIO_InitStructure);
	
	//CAN单元设置
   	CAN_InitStructure.CAN_TTCM=DISABLE;		//非时间触发通信模式   	0=时间触发通讯不使用，保持默认
  	CAN_InitStructure.CAN_ABOM=DISABLE;		//软件自动离线管理	  	0=总线断开状态根据软件请求，保持默认
  	CAN_InitStructure.CAN_AWUM=DISABLE;		//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)，保持默认
  	
	CAN_InitStructure.CAN_NART=DISABLE;		//使用报文自动传送 		
		//0=The CAN hardware will automatically retransmit the message until it has been successfully transmitted according to the CAN standard.
		//1=A message will be transmitted only once, independently of the transmission result (successful, error or arbitration lost).
  	
	CAN_InitStructure.CAN_RFLM=DISABLE;		//报文不锁定,新的覆盖旧的，保持默认
  	CAN_InitStructure.CAN_TXFP=DISABLE;		//优先级由报文标识符ID决定，保持默认
  	
	CAN_InitStructure.CAN_Mode= mode;	 	//模式设置 
  	CAN_InitStructure.CAN_SJW=tsjw;			//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; 		//Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;			//Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;  	//分频系数(Fdiv)为brp+1	
  	CAN_Init(CAN1, &CAN_InitStructure);   	// 初始化CAN1
	
	//配置过滤器
 	CAN_FilterInitStructure.CAN_FilterNumber=0;	  //过滤器0
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; //屏蔽位模式
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK	（都是0表示不关心特定的接收ID，来者都收或者由接收函数再设定）
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器0
  	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化
	
	#if CAN1_RX0_INT_ENABLE || CAN1_TX_INT_ENABLE
		
		NVIC_InitTypeDef  NVIC_InitStructure;
		
		#if CAN1_RX0_INT_ENABLE && CAN1_TX_INT_ENABLE
			
			//FIFO 0 message pending Interrupt
			//Transmit mailbox empty Interrupt
			CAN_ITConfig(CAN1,CAN_IT_FMP0 | CAN_IT_TME,ENABLE);
		#elif CAN1_RX0_INT_ENABLE
			//FIFO 0 message pending Interrupt
			CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
		
		#elif CAN1_TX_INT_ENABLE
			//Transmit mailbox empty Interrupt
			CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE);
		
		#endif
		
		NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	// 主优先级为1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			// 次优先级为0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	#endif
}

#if CAN1_RX0_INT_ENABLE
	CanRxMsg CAN1_IT_RxMessage;
	u8 CAN1_IT_RxMessage_flag;
	//FIFO0消息挂号中断，RX0中断服务函数	
	//FIFO 0 message pending Interrupt
	void CAN1_RX0_IRQHandler(void)
	{
//		if(CAN_GetITStatus(CAN1,CAN_IT_FMP0) == SET) //FIFO 0 message pending Interrupt 
		//看其他例程没有检查中断标志位和清除中断标志位的代码
		{
			CAN1_IT_RxMessage_flag = 1;
			CAN_Receive(CAN1, CAN_FIFO0, &CAN1_IT_RxMessage);
			
//			CAN_ClearFlag(CAN1,CAN_FLAG_RQCP0);
		}

	}
#endif

#if CAN1_TX_INT_ENABLE
//	u8 CAN1_IT_TxMessage_flag;
	u8 CAN1_IT_TxMessage_data[8];	//用于发送中断的8位数据
	u32 CAN1_IT_TxMessage_id;		//用于发送中断的id
	//CAN1发送邮箱空中断
	//Transmit mailbox empty Interrupt
	void CAN1_TX_IRQHandler(void)
	{
		CanTxMsg CAN1_IT_TxMessage;
		
		//默认发送数据帧扩展帧 发送 CAN1_IT_TxMessage_data的8字节数据 和 CAN1_IT_TxMessage_id 29位id
		CAN1_Set_Msg(&CAN1_IT_TxMessage,CAN1_IT_TxMessage_data,8,CAN_Id_Extended,CAN1_IT_TxMessage_id);  
		CAN_Transmit(CAN1, &CAN1_IT_TxMessage);
		
		//关闭发送中断，等待用户填好数据手动开启
		CAN_ITConfig(CAN1,CAN_IT_TME,DISABLE);
//		CAN1_IT_TxMessage_flag = 0;
		
		CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
	}
#endif



//设置期望接收特定的类型帧和ID的信息
//care：0表示任何ID都接收，1表示只接受与FilterId一致的ID的消息
//frameType：期望接收帧的类型，0为标准帧（ID为11位），1为扩展帧（ID为29位）
//FilterId：期望接收的ID
//默认消息类型为数据帧
void CAN1_setExceptId(u8 care,u8 frameType,u32 FilterId)
{
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;		//配置过滤器
	if(care == 0)
	{
		CAN_FilterInitStructure.CAN_FilterNumber=0;	  //过滤器0
		CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; //屏蔽位模式
		CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
		CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
		CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
		CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK	（都是0表示不关心特定的接收ID，来者都收或者由接收函数再设定）
		CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
		CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
		CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器0
		CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化
	}else
	{
		if(frameType == CAN_Id_Standard)	//只接受标准数据帧
		{
			CAN_FilterInitStructure.CAN_FilterNumber=0;
			CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; //屏蔽位模式
			CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
			CAN_FilterInitStructure.CAN_FilterIdHigh = ((FilterId << 21) & 0xffff0000) >> 16;
			CAN_FilterInitStructure.CAN_FilterIdLow = ((FilterId << 21) | CAN_ID_STD | CAN_RTR_DATA) & 0xffff;
			CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xFFFF;	
			CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0xFFFF;
			CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
			CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
			CAN_FilterInit(&CAN_FilterInitStructure);
		}else								//只接受扩展数据帧
		{
			CAN_FilterInitStructure.CAN_FilterNumber=0;
			CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; //屏蔽位模式
			CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
			CAN_FilterInitStructure.CAN_FilterIdHigh = ((FilterId << 3) & 0xffff0000) >> 16;
			CAN_FilterInitStructure.CAN_FilterIdLow = ((FilterId << 3) | CAN_ID_EXT | CAN_RTR_DATA) & 0xffff;
			CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xFFFF;	
			CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0xFFFF;
			CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
			CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
			CAN_FilterInit(&CAN_FilterInitStructure);
		}

	}
}

/*
给 *TxMessage设置内容，不发送
返回：0正确，其他错误
*/
u8 CAN1_Set_Msg(CanTxMsg* TxMessage,u8* msg,u8 len,u8 frameType,u32 id)
{
	u16 i=0;
	if(len > 8)return 1;
	
	if(frameType == CAN_Id_Standard)
	{
		TxMessage->IDE=CAN_Id_Standard;		// 使用标准标识符
		TxMessage->StdId = id;	 			// 设置标准标识符	（11位）
	}else{
		TxMessage->IDE=CAN_Id_Extended;		// 使用扩展标识符
		TxMessage->ExtId = id;	 			// 设置扩展标示符	（29位）
	}
	TxMessage->RTR=CAN_RTR_Data;		  	// 消息类型为数据帧	（数据帧或者遥控帧：CAN_RTR_Data 或者 CAN_RTR_Remote）

	TxMessage->DLC=len;						// 发送几字节信息	（小于等于8）
	for(i=0;i<len;i++)
		TxMessage->Data[i]=msg[i];
	
	return 0;
}


//无中断模式，can发送一组数据			     
//msg:数据指针,最大为8个字节
//len:数据长度(最大为8)
//frameType:帧类型，0为标准帧（ID为11位），1为扩展帧（ID为29位）
//id:ID标识符，若frameType为0，则为标准标识符，若frameType为其他则为扩展标识符
//默认消息类型为数据帧
//返回值:0,成功;
//		 其他,失败;
u8 CAN1_Send_Msg(u8* msg,u8 len,u8 frameType,u32 id)
{	
	u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
	
	if(len > 8)return 1;
	
	if(frameType == CAN_Id_Standard)
	{
		TxMessage.IDE=CAN_Id_Standard;		// 使用标准标识符
		TxMessage.StdId = id;	 			// 设置标准标识符	（11位）
	}else{
		TxMessage.IDE=CAN_Id_Extended;		// 使用扩展标识符
		TxMessage.ExtId = id;	 			// 设置扩展标示符	（29位）
	}
	TxMessage.RTR=CAN_RTR_Data;		  		// 消息类型为数据帧	（数据帧或者遥控帧：CAN_RTR_Data 或者 CAN_RTR_Remote）
	
	TxMessage.DLC=len;						// 发送几字节信息	（小于等于8）
	for(i=0;i<len;i++)
		TxMessage.Data[i]=msg[i];   
	mbox= CAN_Transmit(CAN1, &TxMessage);
	i=0;
	while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
	if(i>=0XFFF)return 2;
	return 0;
}

//无中断模式，can口接收数据查询
//buf:数据缓存区;
//frameType:帧类型，0为标准帧（ID为11位），1为扩展帧（ID为29位）
//id:ID标识符，若frameType为0，则为标准标识符，若frameType为其他则为扩展标识符
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
u8 CAN1_Receive_Msg(u8* buf,u8* frameType,u32* id)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//没有接收到数据,直接退出 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);				//从FIFO0读取数据
    for(i=0;i<RxMessage.DLC;i++)
		buf[i] = RxMessage.Data[i];
	
	if(RxMessage.IDE == CAN_Id_Standard)
	{
		*frameType = CAN_Id_Standard;
		*id = RxMessage.StdId;
	}else{
		*frameType = 1;
		*id = RxMessage.ExtId;
	}
	
	return RxMessage.DLC;	
}


