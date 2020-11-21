#ifndef _can_H
#define _can_H

#include "system.h"

//CAN1接收RX0中断使能
#define CAN1_RX0_INT_ENABLE	1		//0,不使能;1,使能
/*
使用接收中断方式后：查询CAN1_IT_RxMessage_flag是否变为1，如果是则从 CAN1_IT_RxMessage 拾取数据和帧类型、ID等信息，并对CAN1_IT_RxMessage_flag清零
不使用接收中断：死循环函数 CAN1_Receive_Msg(rbuf,&ReceiveFrameType,&receiveID);
*/

//CAN1发送中断，发送完触发中断，非串口的缓存空中断
//Transmit mailbox empty Interrupt
#define CAN1_TX_INT_ENABLE	1



#if CAN1_RX0_INT_ENABLE
	extern u8 CAN1_IT_RxMessage_flag;	//CAN1从中断接收到数据的标志
	extern CanRxMsg CAN1_IT_RxMessage;	//CAN1从中断接收到数据的结构体
#endif

#if CAN1_TX_INT_ENABLE
	extern u8 CAN1_IT_TxMessage_flag;			//CAN1发送邮箱空的中断的标志
	extern u8 CAN1_IT_TxMessage_data[8];		//用于发送中断的8位数据
	extern u32 CAN1_IT_TxMessage_id;			//用于发送中断的id
	extern CanTxMsg CAN1_IT_TxMessage;
#endif

void CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化
/*
	//在CAN外设频率为42Mhz时，推荐分频系数(brp+1)为6，三段分别为：1tq\7tq\6tq
	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_Normal);
*/


/*
给 *TxMessage设置内容，不发送
返回：0正确，其他错误
*/
u8 CAN1_Set_Msg(CanTxMsg* TxMessage,u8* msg,u8 len,u8 frameType,u32 id);
 
//can发送一组数据			     
//msg:数据指针,最大为8个字节
//len:数据长度(最大为8)
//frameType:帧类型，0为标准帧（ID为11位），1为扩展帧（ID为29位）
//id:ID标识符，若frameType为0，则为标准标识符，若frameType为其他则为扩展标识符
//默认消息类型为数据帧
//返回值:0,成功;
//		 其他,失败;
u8 CAN1_Send_Msg(u8* msg,u8 len,u8 frameType,u32 id);	//发送数据


//can口接收数据查询，不使用中断方式时的使用
//buf:数据缓存区;
//frameType:帧类型，0为标准帧（ID为11位），1为扩展帧（ID为29位）
//id:ID标识符，若frameType为0，则为标准标识符，若frameType为其他则为扩展标识符
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
u8 CAN1_Receive_Msg(u8* buf,u8* frameType,u32* id);							//接收数据


//设置期望接收特定的类型帧和ID的信息
//care：0表示任何ID都接收，1表示只接受与FilterId一致的ID的消息
//FilterNumber：要设置的滤波器编号，It ranges from 0 to 13
//frameType：期望接收帧的类型，0为标准帧（ID为11位），1为扩展帧（ID为29位）
//ExceptId：期望接收的ID
//默认消息类型为数据帧
void CAN1_setExceptId(u8 care,u8 FilterNumber,u8 frameType,u32 ExceptId);


#endif
