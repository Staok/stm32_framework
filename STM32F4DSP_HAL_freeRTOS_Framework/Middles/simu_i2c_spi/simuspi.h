#ifndef __SIMUSPI_H
#define __SIMUSPI_H

/**********************************************************************************
	Copyright (C) 2016 SOL.lab <maxwellxyao@foxmail.com>

*File name:		simuspi.h
*Description:	模拟spi协议
*Author:		MaxwellXyao
*Version:		V0.0.20160626
*Date:			2016-6-26 17:21:17
*History:		
[2016-6-26]	文件创建;

**********************************************************************************/

//###########################【定义】###########################
typedef struct _SimuSPI{		//SPI接口类型
	void (*PinSetMOSI)(unsigned char);	//MOSI引脚设置电平函数
	void (*PinSetSCLK)(unsigned char);	//SCLK引脚设置电平函数
	unsigned char (*PinGetMISO)();		//MISO引脚读取电平函数
	void (*Delayus)(unsigned int);		//us延时函数
	unsigned int IntervalTime;			//延时时间(us)
}SimuSPI;			

//###########################【API】###########################
unsigned char SimuSPI_ReadWriteByte(SimuSPI *SimuSPI_Struct,unsigned char data);		//模拟SPI传输字符
void SimuSPI_SetIntTime(SimuSPI *SimuSPI_Struct,unsigned int IntTime);			//模拟SPI设置速度

/*
【使用说明】	模拟SPI协议
方便用户快速构建模拟SPI协议层。只需在定义的结构体中填入
IO口操作函数和延时函数即可调用API来实现SPI协议。

这个文件是完全不依赖硬件的，所以可以移植到任何单片机的任何引脚上。

使用范例：
1.定义结构体，并初始化各成员变量。

SimuSPI W25QXX;
void W25QXX_SPI_Init(void)
{
	GPIO_Init();
	Delay_Init();	

	W25QXX.PinSetMOSI=writePA10;
	W25QXX.PinSetSCLK=writePA11;
	W25QXX.PinGetMISO=readPA12;
	W25QXX.Delayus=delay_us;
	W25QXX.IntervalTime=5;	
}

2.使用SimuSPI模拟SPI协议

void SPI_FLASH_Write_SR(u8 sr)   
{   
	SPI_FLASH_CS=0;                            //使能器件   
	SimuSPI_ReadWriteByte(&W25QXX,W25X_WriteStatusReg);   //发送写取状态寄存器命令    
	SimuSPI_ReadWriteByte(&W25QXX,sr);               //写入一个字节  
	SPI_FLASH_CS=1;                            //取消片选     	      
}


[注意]：CS引脚需要用户自行定义，并配合SimuSPI使用。   





*/


#endif

