#include "SPI_SDcard.h"

/*程序借鉴自：https://blog.csdn.net/qq_37182134/article/details/83859618*/

/*

	测试板接线说明：
		DAT3接CS线，		PC11
		CLK接SCK线，		PC12
		CMD接MOSI线，		PD2
		DAT0接MISO线		PC8
		（信号线需要10~100K上拉电阻，测试板上硬件已经上拉）
		
		
	项目板接线说明：
		DAT3接CS线，		PC5
		CLK接SCK线，		PB14
		CMD接MOSI线，		PB15
		DAT0接MISO线		PC13
*/

SPI_SD_CardInfo SPI_SDcardInfo;	//SD卡信息（初始化时自动获取）


/*选择用模拟SPI还是硬件SPI。0：模拟SPI；1：硬件SPI
	如果用模拟SPI，则在下面定义好IO接口和初始化
	如果用硬件SPI，则去打开一个硬件SPI，再到PeriphConfig.c里面定义使能CS引脚的接口和初始化
*/

#define SPI_SD_Use_simuSPI_or_hardwareSPI	0	


#if (SPI_SD_Use_simuSPI_or_hardwareSPI == 0)
	//接口定义，用户修改
	
	#define SDcard_simuSPI_CS 	PBout(14) 		//simuSPI的CS线
	#define SDcard_simuSPI_SCK 	simuSPI_SCK		//simuSPI的SCK线
	#define SDcard_simuSPI_MOSI simuSPI_MOSI 	//simuSPI的MOSI线
	#define SDcard_simuSPI_MISO simuSPI_MISO 	//simuSPI的MISO线
	
	#define CS_H SDcard_simuSPI_CS = IO_High
	#define CS_L SDcard_simuSPI_CS = IO_Low
	
	#define SPI_ReadWrite_Byte(x) SimuSPI_ReadWriteByte(&simuSPI_Handle,x) //SPI发读函数接口定义
	
	void SDcard_simuSPI_CS_IO_init(void)
	{
		//simuSPI的CS引脚初始化，用户修改
		GPIO_InitTypeDef GPIO_Initure;
		
		__HAL_RCC_GPIOB_CLK_ENABLE();	
		
		GPIO_Initure.Pin = GPIO_PIN_14;
		GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;  	//推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;          	//上拉
		GPIO_Initure.Speed=GPIO_SPEED_HIGH;     	//高速
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	}

#endif

#if (SPI_SD_Use_simuSPI_or_hardwareSPI == 1)
	//接口定义，用户修改
	#define CS_H SPI1_CS = IO_High
	#define CS_L SPI1_CS = IO_Low
	
	u8 SPI_ReadWrite_Byte(u8 TxData)
	{
		u8 Rxdata;
		HAL_SPI_TransmitReceive(&SPI1_Handler,&TxData,&Rxdata,1, 100);
		return Rxdata;
	}

#endif

/*下面是固定不变的部分*/
//IO初始化
void SDcard_SPI_Init(void)
{
	#if (SPI_SD_Use_simuSPI_or_hardwareSPI == 0)
		
		simuSPI_IO_init();
		SDcard_simuSPI_CS_IO_init();

		HAL_Delay(10);
	#endif
	
	#if (SPI_SD_Use_simuSPI_or_hardwareSPI == 1)
		//硬件SPI初始化，用户修改
		sys_SPI1_ENABLE();
		HAL_Delay(10);
	#endif
	
	SPI_ReadWrite_Byte(0xff);
}

//取消片选 释放SPI总线
void SD_DisSelect(void)
{
  CS_H;
  SPI_ReadWrite_Byte(0xff);
}
//等待卡准备好
//准备好了返回值0；其他错误代码
u8 SD_WaitReady(void)
{
  u32 t=0;
  do
  {
	if(SPI_ReadWrite_Byte(0xff)==0XFF)return 0;//OK
	t++;		  	
  }while(t<0XFFFFFF);
  return 1;
}
//选择SD卡并等待卡准备好
//返回值 0成功，1失败
u8 SD_Select(void)
{
  CS_L;
  if(SD_WaitReady()==0)return 0;
  SD_DisSelect();
  return 1;
}
//获得响应
//Response 要得到的回应?
//其他响应错误
u8 SD_GetResponse(u8 Response)
{
  u16 Count=0xFFFF;   		//等待次数				  
  while ((SPI_ReadWrite_Byte(0xff)!=Response)&&Count)Count--;	  
  if (Count==0)return MSD_RESPONSE_FAILURE;  
  else return MSD_RESPONSE_NO_ERROR;
}
//从SD卡读取一个数据包的内容
//buf数据缓存区
//len读取的数据长度
u8 SD_RecvData(u8* buf,u16 len)
{			  	  
  if(SD_GetResponse(0xFE))return 1;
  while(len--)
  {
    *buf=SPI_ReadWrite_Byte(0xff);
     buf++;
  }
  SPI_ReadWrite_Byte(0xff);
  SPI_ReadWrite_Byte(0xff);									  					    
  return 0;
}
//向SD卡写入一个数据包的内容512字节
//buf数据缓存
//命令
//返回值0成功 其他失败
u8 SD_SendBlock(u8* buf,u8 cmd)
{	
  u16 t;		  	  
  if(SD_WaitReady())return 1;
  SPI_ReadWrite_Byte(cmd);
  if(cmd!=0XFD)
  {
	for(t=0;t<512;t++)SPI_ReadWrite_Byte(buf[t]);
	SPI_ReadWrite_Byte(0xFF);
	SPI_ReadWrite_Byte(0xFF);
	t=SPI_ReadWrite_Byte(0xFF);
	if((t&0x1F)!=0x05)return 2;								  					    
   }						 									  					    
  return 0;
}
//向SD卡发送一个命令
//u8 cmd  命令
//u32 arg 命令参数
u8 SD_SendCmd(u8 cmd, u32 arg, u8 crc)
{
  u8 r1;	
  u8 Retry=0; 
  SD_DisSelect();
  if(SD_Select())return 0XFF;
    SPI_ReadWrite_Byte(cmd | 0x40);
    SPI_ReadWrite_Byte(arg >> 24);
    SPI_ReadWrite_Byte(arg >> 16);
    SPI_ReadWrite_Byte(arg >> 8);
    SPI_ReadWrite_Byte(arg);	  
    SPI_ReadWrite_Byte(crc); 
  if(cmd==CMD12) SPI_ReadWrite_Byte(0xff);//Skip a stuff byte when stop reading
	Retry=0X1F;
  do
   {
	 r1=SPI_ReadWrite_Byte(0xFF);
   }while((r1&0X80) && Retry--);	 
  return r1;
}
//获取SD卡的CID信息包括 包括制造商信息
//u8 *cid_data	存放CID的内存至少16个字节
//返回 0:NO_ERR
//		 1:错误														   
u8 SD_GetCID(u8 *cid_data)
{
  u8 r1;	   
  r1=SD_SendCmd(CMD10,0,0x01);
  if(r1==0x00)
  {
	r1=SD_RecvData(cid_data,16);	//接收16个字节的数据 
  }
  SD_DisSelect();
  if(r1)return 1;
  else return 0;
}																				  
//获取SD卡的CSD信息包括 包括容量和速度信息
//u8 *cid_data	存放CsD的内存至少16个字节
//返回 0:NO_ERR
//		 1:错误														   
u8 SD_GetCSD(u8 *csd_data)
{
  u8 r1;	 
  r1=SD_SendCmd(CMD9,0,0x01);
  if(r1==0)
  {
     r1=SD_RecvData(csd_data, 16); 
   }
  SD_DisSelect();
  if(r1)return 1;
  else return 0;
}  
//获取SD卡总扇区数
//返回值:0: 取出容量出错
//      其他:SD容量(扇区数/512字节)														  
u32 SD_GetSectorCount(void)
{
  u8 csd[16];
  u32 Capacity;  
  u8 n;
  u16 csize;  					    
  //取CSD信息
  if(SD_GetCSD(csd)!=0) return 0;	    
    
  if((csd[0]&0xC0)==0x40)	 //V2.00卡
  {	
	 csize = csd[9] + ((u16)csd[8] << 8) + 1;
	 Capacity = (u32)csize << 10;//得到扇区数		   
  }else//V1.XX卡
   {	
	  n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
	  csize = (csd[8] >> 6) + ((u16)csd[7] << 2) + ((u16)(csd[6] & 3) << 10) + 1;
	  Capacity= (u32)csize << (n - 9);//得到扇区数 
   }
   return Capacity;
}

u8 SD_Type;

//初始化SD
//返回0成功，其他失败
u8 SPI_SD_Init(void)
{
  u8 r1;      // 
  u16 retry;  // 
  u8 buf[4];  
  u16 i;
 
  SDcard_SPI_Init();	//初始化IO
	
  for(i=0;i<10;i++)SPI_ReadWrite_Byte(0XFF);//发送至少74个脉冲
  retry=20;
  do
  {
	 r1=SD_SendCmd(CMD0,0,0x95);//进入IDLE状态
   }while((r1!=0X01) && retry--);
   SD_Type=0;//默认无卡
  if(r1==0X01)
   {
	  if(SD_SendCmd(CMD8,0x1AA,0x87)==1)//SD V2.0
	  {
		for(i=0;i<4;i++)buf[i]=SPI_ReadWrite_Byte(0XFF);	//Get trailing return value of R7 resp
		if(buf[2]==0X01&&buf[3]==0XAA)//卡是否支持2.7~3.6V
		{
		  retry=0XFFFE;
		  do
		  {
			 SD_SendCmd(CMD55,0,0X01);	//发送CMD55
			 r1=SD_SendCmd(CMD41,0x40000000,0X01);//发送CMD41
		    }while(r1&&retry--);
		  if(retry&&SD_SendCmd(CMD58,0,0X01)==0)//鉴别SD2.0开始
		  {
			 for(i=0;i<4;i++)buf[i]=SPI_ReadWrite_Byte(0XFF);//得到OCR值
			 if(buf[0]&0x40)SD_Type=SD_TYPE_V2HC;    //检测CCS
			  else SD_Type=SD_TYPE_V2;   
		    }
		 }
		}else//SD V1.x/ MMC	V3
		{
		  SD_SendCmd(CMD55,0,0X01);		//发送CMD55
		  r1=SD_SendCmd(CMD41,0,0X01);	//发送CMD41
		  if(r1<=1)
		  {		
			SD_Type=SD_TYPE_V1;
			retry=0XFFFE;
			do 
			{
			  SD_SendCmd(CMD55,0,0X01);	//发送CMD55
			  r1=SD_SendCmd(CMD41,0,0X01);//发送CMD41
			}while(r1&&retry--);
		  }else
		     {
			   SD_Type=SD_TYPE_MMC;//MMC V3
			   retry=0XFFFE;
			   do 
			    {											    
				  r1=SD_SendCmd(CMD1,0,0X01);
			     }while(r1&&retry--);  
		   }
		  if(retry==0||SD_SendCmd(CMD16,512,0X01)!=0)SD_Type=SD_TYPE_ERR;
		}
   }
  SD_DisSelect();
   
//  SPI2_SetSpeed(SPI_BaudRatePrescaler_2);
	
   HAL_Delay(10);
   
   SPI_SD_ReadInfo();	//获取卡信息，填入 SPI_SDcardInfo 句柄结构体
   
  if(SD_Type)return 0;
  else if(r1)return r1; 	   
  return 0xaa;
}
u8 SPI_SD_ReadDisk2(u8*buf,u32 sector,u8 cnt)
{
  u8 r1;
  if(SD_Type!=SD_TYPE_V2HC)sector <<= 9;
  if(cnt==1)
  {
	r1=SD_SendCmd(CMD17,sector,0X01);
	if(r1==0)
	{
	  r1=SD_RecvData(buf,512);	   
	}
  }else{
	   r1=SD_SendCmd(CMD18,sector,0X01);
	   do
	   {
		  r1=SD_RecvData(buf,512);	 
		  buf+=512;  
		}while(--cnt && r1==0); 	
	   SD_SendCmd(CMD12,0,0X01);	
  }   
  SD_DisSelect();
  return r1;
}
//u8*buf数据缓存
//sector起始扇区
//扇区数
u8 SPI_SD_WriteDisk2(u8*buf,u32 sector,u8 cnt)
{
  u8 r1;
  if(SD_Type!=SD_TYPE_V2HC)sector *= 512;
  if(cnt==1)
  {
	 r1=SD_SendCmd(CMD24,sector,0X01);
	 if(r1==0)
	 {
		r1=SD_SendBlock(buf,0xFE);  
	  }
   }else
    {
	  if(SD_Type!=SD_TYPE_MMC)
	   {
		  SD_SendCmd(CMD55,0,0X01);	
		  SD_SendCmd(CMD23,cnt,0X01);
		}
 	  r1=SD_SendCmd(CMD25,sector,0X01);
	  if(r1==0)
	  {
		do
		{
		  r1=SD_SendBlock(buf,0xFC);	 
		  buf+=512;  
		 }while(--cnt && r1==0);
		 r1=SD_SendBlock(0,0xFD);
		}
	}   
  SD_DisSelect();
  return r1;
}

//读取SPI驱动的SD卡的信息
//返回0成功
u8 SPI_SD_ReadInfo(void)
{
	u8 cid_buf[16],csd_buf[16],sta = 1;
	
	SPI_SDcardInfo.CardType = SD_Type;
	
	sta = SD_GetCID(cid_buf);
	if(sta == 0)
	{
		SPI_SDcardInfo.SD_cid.ManufacturerID = cid_buf[0];
		
		SPI_SDcardInfo.SD_cid.OEM_AppliID = cid_buf[1]<<8;
		SPI_SDcardInfo.SD_cid.OEM_AppliID |= cid_buf[2];
		
		SPI_SDcardInfo.SD_cid.ProdName1 = cid_buf[3]<<24;
		SPI_SDcardInfo.SD_cid.ProdName1 |= cid_buf[4]<<16;
		SPI_SDcardInfo.SD_cid.ProdName1 |= cid_buf[5]<<8;
		SPI_SDcardInfo.SD_cid.ProdName1 |= cid_buf[6];
		
		SPI_SDcardInfo.SD_cid.ProdName2 = cid_buf[7];
		
		SPI_SDcardInfo.SD_cid.ProdRev = cid_buf[8];
		
		SPI_SDcardInfo.SD_cid.ProdSN = cid_buf[9]<<24;
		SPI_SDcardInfo.SD_cid.ProdSN |= cid_buf[10]<<16;
		SPI_SDcardInfo.SD_cid.ProdSN |= cid_buf[11]<<8;
		SPI_SDcardInfo.SD_cid.ProdSN |= cid_buf[12];
		
		SPI_SDcardInfo.SD_cid.Reserved1 |= (cid_buf[13] & 0xf0)>>4;
		
		SPI_SDcardInfo.SD_cid.ManufactDate = (cid_buf[13] &0x0f)<<8;
		SPI_SDcardInfo.SD_cid.ManufactDate |= cid_buf[14];
		
		SPI_SDcardInfo.SD_cid.CID_CRC =  (cid_buf[15] & 0xfe) >> 1;
		
		SPI_SDcardInfo.SD_cid.Reserved2 = 1;
	}else{return 1;}
	
	sta = SD_GetCSD(csd_buf);
	if(sta == 0)
	{
		SPI_SDcardInfo.SD_csd.CSDStruct = (csd_buf[0] & 0xC0) >> 6;
		SPI_SDcardInfo.SD_csd.SysSpecVersion = (csd_buf[0] & 0x3C) >> 2;
		SPI_SDcardInfo.SD_csd.Reserved1 = csd_buf[0] & 0x03;
		
		SPI_SDcardInfo.SD_csd.TAAC = csd_buf[1];
		
		SPI_SDcardInfo.SD_csd.NSAC = csd_buf[2];
		
		SPI_SDcardInfo.SD_csd.MaxBusClkFrec = csd_buf[3];
		
		SPI_SDcardInfo.SD_csd.CardComdClasses = csd_buf[4] << 4;
		SPI_SDcardInfo.SD_csd.CardComdClasses |= (csd_buf[5] & 0xF0) >> 4;
		
		SPI_SDcardInfo.SD_csd.RdBlockLen = csd_buf[5] & 0x0F;
		
		SPI_SDcardInfo.SD_csd.PartBlockRead = (csd_buf[6] & 0x80) >> 7;
		SPI_SDcardInfo.SD_csd.WrBlockMisalign = (csd_buf[6] & 0x40) >> 6;
		SPI_SDcardInfo.SD_csd.RdBlockMisalign = (csd_buf[6] & 0x20) >> 5;
		SPI_SDcardInfo.SD_csd.DSRImpl = (csd_buf[6] & 0x10) >> 4;
		SPI_SDcardInfo.SD_csd.Reserved2 = 0;
		
		SPI_SDcardInfo.SD_csd.DeviceSize = (csd_buf[6] & 0x03) << 10;
		
		if ((SD_Type == SD_TYPE_V1) || (SD_Type == SD_TYPE_V2))
		{
			/*!< Byte 7 */
			SPI_SDcardInfo.SD_csd.DeviceSize |= (csd_buf[7]) << 2;
	 
			/*!< Byte 8 */
			SPI_SDcardInfo.SD_csd.DeviceSize |= (csd_buf[8] & 0xC0) >> 6;
	 
			SPI_SDcardInfo.SD_csd.MaxRdCurrentVDDMin = (csd_buf[8] & 0x38) >> 3;
			SPI_SDcardInfo.SD_csd.MaxRdCurrentVDDMax = (csd_buf[8] & 0x07);
	 
			/*!< Byte 9 */
			SPI_SDcardInfo.SD_csd.MaxWrCurrentVDDMin = (csd_buf[9] & 0xE0) >> 5;
			SPI_SDcardInfo.SD_csd.MaxWrCurrentVDDMax = (csd_buf[9] & 0x1C) >> 2;
			SPI_SDcardInfo.SD_csd.DeviceSizeMul = (csd_buf[9] & 0x03) << 1;
			/*!< Byte 10 */
			SPI_SDcardInfo.SD_csd.DeviceSizeMul |= (csd_buf[10] & 0x80) >> 7;
		}else if (SD_Type == SD_TYPE_V2HC)	//V2.0 SDHC card info.
		{
			SPI_SDcardInfo.SD_csd.DeviceSize = (csd_buf[7] & 0x3F) << 16;
	 
			SPI_SDcardInfo.SD_csd.DeviceSize |= (csd_buf[8] << 8);
		
			SPI_SDcardInfo.SD_csd.DeviceSize |= (csd_buf[9]);		
		}
		
		SPI_SDcardInfo.SD_csd.EraseGrSize = (csd_buf[10] & 0x40) >> 6;
		SPI_SDcardInfo.SD_csd.EraseGrMul = (csd_buf[10] & 0x3F) << 1;

		/*!< Byte 11 */
		SPI_SDcardInfo.SD_csd.EraseGrMul |= (csd_buf[11] & 0x80) >> 7;
		SPI_SDcardInfo.SD_csd.WrProtectGrSize = (csd_buf[11] & 0x7F);

		/*!< Byte 12 */
		SPI_SDcardInfo.SD_csd.WrProtectGrEnable = (csd_buf[12] & 0x80) >> 7;
		SPI_SDcardInfo.SD_csd.ManDeflECC = (csd_buf[12] & 0x60) >> 5;
		SPI_SDcardInfo.SD_csd.WrSpeedFact = (csd_buf[12] & 0x1C) >> 2;
		SPI_SDcardInfo.SD_csd.MaxWrBlockLen = (csd_buf[12] & 0x03) << 2;

		/*!< Byte 13 */
		SPI_SDcardInfo.SD_csd.MaxWrBlockLen |= (csd_buf[13] & 0xC0) >> 6;
		SPI_SDcardInfo.SD_csd.WriteBlockPaPartial = (csd_buf[13] & 0x20) >> 5;
		SPI_SDcardInfo.SD_csd.Reserved3 = 0;
		SPI_SDcardInfo.SD_csd.ContentProtectAppli = (csd_buf[13] & 0x01);

		/*!< Byte 14 */
		SPI_SDcardInfo.SD_csd.FileFormatGrouop = (csd_buf[14] & 0x80) >> 7;
		SPI_SDcardInfo.SD_csd.CopyFlag = (csd_buf[14] & 0x40) >> 6;
		SPI_SDcardInfo.SD_csd.PermWrProtect = (csd_buf[14] & 0x20) >> 5;
		SPI_SDcardInfo.SD_csd.TempWrProtect = (csd_buf[14] & 0x10) >> 4;
		SPI_SDcardInfo.SD_csd.FileFormat = (csd_buf[14] & 0x0C) >> 2;
		SPI_SDcardInfo.SD_csd.ECC = (csd_buf[14] & 0x03);

		/*!< Byte 15 */
		SPI_SDcardInfo.SD_csd.CSD_CRC = (csd_buf[15] & 0xFE) >> 1;
		SPI_SDcardInfo.SD_csd.Reserved4 = 1;
	}else{return 1;}
	
	if ((SD_Type == SD_TYPE_V1) || (SD_Type == SD_TYPE_V2))
	{
		SPI_SDcardInfo.CardCapacity=(SPI_SDcardInfo.SD_csd.DeviceSize+1);			//计算卡容量
		SPI_SDcardInfo.CardCapacity*=(1<<(SPI_SDcardInfo.SD_csd.DeviceSizeMul+2));
		SPI_SDcardInfo.CardBlockSize=1<<(SPI_SDcardInfo.SD_csd.RdBlockLen);			//块大小
		SPI_SDcardInfo.CardCapacity*=SPI_SDcardInfo.CardBlockSize;
	}else if(SD_Type == SD_TYPE_V2HC)
	{
 		SPI_SDcardInfo.CardCapacity=(long long)(SPI_SDcardInfo.SD_csd.DeviceSize+1)*512*1024;//计算卡容量
		SPI_SDcardInfo.CardBlockSize=512; 			//块大小固定为512字节
	}
	
	return 0;
}





