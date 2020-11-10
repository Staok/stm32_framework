/**********************************************************************************
	Copyright (C) 2016 SOL.lab <maxwellxyao@foxmail.com>

*File name:		spi.h
*Description:	模拟spi协议
*Author:		MaxwellXyao
*Version:		V0.0.20160626
*Date:			2016-6-26 17:21:17
*History:		
[2016-6-26]	文件创建;

**********************************************************************************/
#include "simuspi.h"		//模拟SPI协议

#include "PeriphConfigCore.h"

SimuSPI simuSPI_Handle;					//simuSPI公用句柄


void simuSPI_SetMOSI(unsigned char sta)
{	simuSPI_MOSI = sta;}

void simuSPI_SetSCLK(unsigned char sta)
{	simuSPI_SCK = sta;}

unsigned char simuSPI_GetMISO(void)
{	return simuSPI_MISO;}
	
void simuSPI_IO_init(void)
{
	//simuSPI的三个公用数据IO口初始化，用户修改
	GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();	
	
	GPIO_Initure.Pin = GPIO_PIN_3|GPIO_PIN_5;
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;  	//推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;          	//上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     	//高速
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
	GPIO_Initure.Pin = GPIO_PIN_4;
	GPIO_Initure.Mode = GPIO_MODE_INPUT;		//MISO 输入
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
	simuSPI_Handle.PinSetMOSI=simuSPI_SetMOSI;
	simuSPI_Handle.PinSetSCLK=simuSPI_SetSCLK;
	simuSPI_Handle.PinGetMISO=simuSPI_GetMISO;
	simuSPI_Handle.Delayus=delay_us;
	simuSPI_Handle.IntervalTime = 5;
}

//###########################【函数】###########################
/*****************************************************************
*Function:	SimuSPI_ReadWriteByte
*Description:	模拟SPI协议实现
*Input:			模拟SPI结构体，发送的字符
*Output:		接收的字符
*Other:		
*****************************************************************/
unsigned char SimuSPI_ReadWriteByte(SimuSPI *SimuSPI_Struct,unsigned char data)   
{   
    unsigned char i=0;   
    unsigned char temp=0;

	SimuSPI_Struct->PinSetSCLK(0);   
    for(i=0;i<8;i++)   
    {   
        if(data&0x80)SimuSPI_Struct->PinSetMOSI(1);   
        else SimuSPI_Struct->PinSetMOSI(0);   
        data<<=1;   
        SimuSPI_Struct->PinSetSCLK(1);  
        SimuSPI_Struct->Delayus(SimuSPI_Struct->IntervalTime);  
        temp<<=1;   
        if(SimuSPI_Struct->PinGetMISO())temp++;   
		SimuSPI_Struct->Delayus(SimuSPI_Struct->IntervalTime);  
        SimuSPI_Struct->PinSetSCLK(0);   
    }   
    return temp;   
}

/*****************************************************************
*Function:	SetIntTime
*Description:	模拟SPI结构体，模拟SPI延时长度设置
*Input:		延时长度（us）
*Output:	无
*Other:		
*****************************************************************/
void SimuSPI_SetIntTime(SimuSPI *SimuSPI_Struct,unsigned int IntTime)
{
	SimuSPI_Struct->IntervalTime=IntTime;	
}



