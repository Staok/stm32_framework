/**********************************************************************************
	Copyright (C) 2016 SOL.lab <maxwellxyao@foxmail.com>

*File name:		simui2c.c
*Description:	模拟I2C协议
*Author:		MaxwellXyao
*Versionc:		V0.1.20170321
*History:		
[2016-6-26]	文件创建;
[2017-3-19]	修复SimuI2C_WriteByte()中读取失败的bug;
[2017-3-21]	修复SimuI2C_Init()中初始化失败的bug;

**********************************************************************************/
#include "simui2c.h"	//模拟I2C协议

//###########################【函数】###########################

/*****************************************************************
*Function:	SimuI2C_Init
*Description:	初始化模拟IIC协议
*Input:		模拟I2C结构体
*Output:	无
*Other:		无
*****************************************************************/
void SimuI2C_Init(SimuI2C *SimuI2C_Struct)
{
	SimuI2C_Struct->PinOutInitSDA();
	SimuI2C_Struct->PinOutInitSCL();
	SimuI2C_Struct->PinSetSCL(1);
	SimuI2C_Struct->PinSetSDA(1);
}

/*****************************************************************
*Function:	SimuI2C_Start
*Description:	产生IIC起始信号
*Input:		模拟I2C结构体
*Output:	无
*Other:		无
*****************************************************************/
void SimuI2C_Start(SimuI2C *SimuI2C_Struct)			 //产生IIC起始信号
{
	SimuI2C_Struct->PinOutInitSDA();
	SimuI2C_Struct->PinSetSDA(1);	  	  
	SimuI2C_Struct->PinSetSCL(1);
	SimuI2C_Struct->Delayus(4);
 	SimuI2C_Struct->PinSetSDA(0);//START:when CLK is high,DATA change form high to low 
	SimuI2C_Struct->Delayus(4);
	SimuI2C_Struct->PinSetSCL(0);//钳住I2C总线，准备发送或接收数据 
}	  

/*****************************************************************
*Function:	SimuI2C_Stop
*Description:	产生IIC停止信号
*Input:		模拟I2C结构体
*Output:	无
*Other:		无
*****************************************************************/
void SimuI2C_Stop(SimuI2C *SimuI2C_Struct)			   //产生IIC停止信号
{
	SimuI2C_Struct->PinOutInitSDA();
	SimuI2C_Struct->PinSetSCL(0);
	SimuI2C_Struct->PinSetSDA(0);//STOP:when CLK is high DATA change form low to high
 	SimuI2C_Struct->Delayus(4);
	SimuI2C_Struct->PinSetSCL(1); 
	SimuI2C_Struct->PinSetSDA(1);//发送I2C总线结束信号
	SimuI2C_Struct->Delayus(4);							   	
}

/*****************************************************************
*Function:	SimuI2C_WaitAck
*Description:	等待应答信号到来
*Input:		模拟I2C结构体
*Output:	1，接收应答失败
        	0，接收应答成功
*Other:		无
*****************************************************************/
unsigned char SimuI2C_WaitAck(SimuI2C *SimuI2C_Struct)
{
	unsigned char ucErrTime=0; 
	SimuI2C_Struct->PinInInitSDA();	
	SimuI2C_Struct->PinSetSDA(1);SimuI2C_Struct->Delayus(1);	   
	SimuI2C_Struct->PinSetSCL(1);SimuI2C_Struct->Delayus(1);	 
	while(SimuI2C_Struct->PinGetSDA())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			SimuI2C_Stop(SimuI2C_Struct);
			return 1;
		}
	}
	SimuI2C_Struct->PinSetSCL(0);//时钟输出0 	   
	return 0;  
} 

/*****************************************************************
*Function:	SimuI2C_Ack
*Description:	产生ACK应答
*Input:		模拟I2C结构体
*Output:	无
*Other:		无
*****************************************************************/
void SimuI2C_Ack(SimuI2C *SimuI2C_Struct)
{
	SimuI2C_Struct->PinSetSCL(0);
	SimuI2C_Struct->PinOutInitSDA();
	SimuI2C_Struct->PinSetSDA(0);
	SimuI2C_Struct->Delayus(2);
	SimuI2C_Struct->PinSetSCL(1);
	SimuI2C_Struct->Delayus(2);
	SimuI2C_Struct->PinSetSCL(0);
}

/*****************************************************************
*Function:	SimuI2C_NAck
*Description:	不产生ACK应答
*Input:		模拟I2C结构体
*Output:	无
*Other:		无
*****************************************************************/	    
void SimuI2C_NAck(SimuI2C *SimuI2C_Struct)
{
	SimuI2C_Struct->PinSetSCL(0);
	SimuI2C_Struct->PinOutInitSDA();
	SimuI2C_Struct->PinSetSDA(1);
	SimuI2C_Struct->Delayus(2);
	SimuI2C_Struct->PinSetSCL(1);
	SimuI2C_Struct->Delayus(2);
	SimuI2C_Struct->PinSetSCL(0);
}


/*****************************************************************
*Function:	SimuI2C_WriteByte
*Description:	IIC发送一个字节
*Input:			模拟I2C结构体，发送的字节
*Output:		1，有应答
				0，无应答	
*Other:		无
*****************************************************************/					 				     	  
void SimuI2C_WriteByte(SimuI2C *SimuI2C_Struct,unsigned char data)
{                        
    unsigned char t;   	
	SimuI2C_Struct->PinOutInitSDA();    
    SimuI2C_Struct->PinSetSCL(0);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        SimuI2C_Struct->PinSetSDA((data&0x80)>>7);
        data<<=1; 	  
		SimuI2C_Struct->Delayus(2);   //对TEA5767这三个延时都是必须的
		SimuI2C_Struct->PinSetSCL(1);
		SimuI2C_Struct->Delayus(2); 
		SimuI2C_Struct->PinSetSCL(0);	
		SimuI2C_Struct->Delayus(2);
    }	 
} 


/*****************************************************************
*Function:	SimuI2C_ReadByte
*Description:	IIC读一个字节
*Input:			模拟I2C结构体，应答信号
*Output:		无
*Other:			ack=1时，发送ACK，ack=0，发送nACK   
*****************************************************************/	    
unsigned char SimuI2C_ReadByte(SimuI2C *SimuI2C_Struct,unsigned char ack)
{
	unsigned char i,receive=0;
	SimuI2C_Struct->PinInInitSDA();
    for(i=0;i<8;i++ )
	{
        SimuI2C_Struct->PinSetSCL(0); 
        SimuI2C_Struct->Delayus(2);
		SimuI2C_Struct->PinSetSCL(1);
        receive<<=1;
        if(SimuI2C_Struct->PinGetSDA())receive++;   
		SimuI2C_Struct->Delayus(1); 
    }					 
    if (!ack)
        SimuI2C_NAck(SimuI2C_Struct);//发送nACK
    else
        SimuI2C_Ack(SimuI2C_Struct); //发送ACK   
    return receive;
}




