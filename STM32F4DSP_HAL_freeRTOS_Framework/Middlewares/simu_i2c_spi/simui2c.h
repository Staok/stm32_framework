#ifndef __SIMUI2C_H
#define __SIMUI2C_H

/**********************************************************************************
	Copyright (C) 2016 SOL.lab <maxwellxyao@foxmail.com>

*File name:		simui2c.h
*Description:	模拟I2C协议
*Author:		MaxwellXyao
*Version:		V0.1.20170321
*History:		
[2016-6-26]	文件创建;
[2017-3-19]	修复SimuI2C_WriteByte()中读取失败的bug;
[2017-3-21]	修复SimuI2C_Init()中初始化失败的bug;

**********************************************************************************/

//###########################【定义】###########################
typedef struct _SimuI2C{				//I2C硬件管理块
//--------------------------------[硬件初始化接口]--------------------------------//
	void (*PinOutInitSCL)();			//SCL引脚输出初始化
	void (*PinOutInitSDA)();			//SDA引脚输出初始化
	void (*PinInInitSDA)();				//SDA引脚输入初始化
//--------------------------------[硬件控制接口]--------------------------------//	
	void (*PinSetSCL)(unsigned char);		//SCL引脚输出函数
	void (*PinSetSDA)(unsigned char);		//SDA引脚输出函数
	unsigned char (*PinGetSDA)();			//SDA引脚读取函数
	void (*Delayus)(unsigned int);		//us延时函数
}SimuI2C;								

//###########################【API】###########################
void SimuI2C_Init(SimuI2C *SimuI2C_Struct);					//初始化
void SimuI2C_Start(SimuI2C *SimuI2C_Struct);			 	//产生IIC起始信号
void SimuI2C_Stop(SimuI2C *SimuI2C_Struct);			   		//产生IIC停止信号
unsigned char SimuI2C_WaitAck(SimuI2C *SimuI2C_Struct);		//等待应答信号到来
void SimuI2C_Ack(SimuI2C *SimuI2C_Struct);					//产生ACK应答
void SimuI2C_NAck(SimuI2C *SimuI2C_Struct);					//不产生ACK应答
void SimuI2C_WriteByte(SimuI2C *SimuI2C_Struct,unsigned char data);			//IIC发送一个字节
unsigned char SimuI2C_ReadByte(SimuI2C *SimuI2C_Struct,unsigned char ack);	//IIC读一个字节

#endif
