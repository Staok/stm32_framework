#ifndef __SPI_SDCARD_H
#define __SPI_SDCARD_H

#include "system.h"
#include "simuspi.h"		//模拟SPI协议


//SD卡CSD寄存器数据		  
typedef struct
{
	u8  CSDStruct;            /*!< CSD structure */
	u8  SysSpecVersion;       /*!< System specification version */
	u8  Reserved1;            /*!< Reserved */
	u8  TAAC;                 /*!< Data read access-time 1 */
	u8  NSAC;                 /*!< Data read access-time 2 in CLK cycles */
	u8  MaxBusClkFrec;        /*!< Max. bus clock frequency */
	u16 CardComdClasses;      /*!< Card command classes */
	u8  RdBlockLen;           /*!< Max. read data block length */
	u8  PartBlockRead;        /*!< Partial blocks for read allowed */
	u8  WrBlockMisalign;      /*!< Write block misalignment */
	u8  RdBlockMisalign;      /*!< Read block misalignment */
	u8  DSRImpl;              /*!< DSR implemented */
	u8  Reserved2;            /*!< Reserved */
	u32 DeviceSize;           /*!< Device Size */
	u8  MaxRdCurrentVDDMin;   /*!< Max. read current @ VDD min */
	u8  MaxRdCurrentVDDMax;   /*!< Max. read current @ VDD max */
	u8  MaxWrCurrentVDDMin;   /*!< Max. write current @ VDD min */
	u8  MaxWrCurrentVDDMax;   /*!< Max. write current @ VDD max */
	u8  DeviceSizeMul;        /*!< Device size multiplier */
	u8  EraseGrSize;          /*!< Erase group size */
	u8  EraseGrMul;           /*!< Erase group size multiplier */
	u8  WrProtectGrSize;      /*!< Write protect group size */
	u8  WrProtectGrEnable;    /*!< Write protect group enable */
	u8  ManDeflECC;           /*!< Manufacturer default ECC */
	u8  WrSpeedFact;          /*!< Write speed factor */
	u8  MaxWrBlockLen;        /*!< Max. write data block length */
	u8  WriteBlockPaPartial;  /*!< Partial blocks for write allowed */
	u8  Reserved3;            /*!< Reserded */
	u8  ContentProtectAppli;  /*!< Content protection application */
	u8  FileFormatGrouop;     /*!< File format group */
	u8  CopyFlag;             /*!< Copy flag (OTP) */
	u8  PermWrProtect;        /*!< Permanent write protection */
	u8  TempWrProtect;        /*!< Temporary write protection */
	u8  FileFormat;           /*!< File Format */
	u8  ECC;                  /*!< ECC code */
	u8  CSD_CRC;              /*!< CSD CRC */
	u8  Reserved4;            /*!< always 1*/
}SPI_SD_CSD; 
//SD卡CID寄存器数据
typedef struct
{
	u8  ManufacturerID;       /*!< ManufacturerID */
	u16 OEM_AppliID;          /*!< OEM/Application ID */
	u32 ProdName1;            /*!< Product Name part1 */
	u8  ProdName2;            /*!< Product Name part2*/
	u8  ProdRev;              /*!< Product Revision */
	u32 ProdSN;               /*!< Product Serial Number */
	u8  Reserved1;            /*!< Reserved1 */
	u16 ManufactDate;         /*!< Manufacturing Date */
	u8  CID_CRC;              /*!< CID CRC */
	u8  Reserved2;            /*!< always 1 */
}SPI_SD_CID;	

//SD卡信息,包括CSD,CID等数据
//TODO：若以后要用多个SPI SD卡，就把每个卡的CS引脚记在下面的卡句柄结构体里
//然后得改.c文件里的底层函数、初始化、读写和读出信息函数以支持区分不同的卡，函数传入参数为区分卡的句柄
typedef struct
{
  SPI_SD_CSD SD_csd;
  SPI_SD_CID SD_cid;
  long long CardCapacity;  	//SD卡容量,单位:字节,最大支持2^64字节大小的卡.
  u32 CardBlockSize; 		//SD卡块大小	
  u16 RCA;					//卡相对地址
  u8 CardType;				//卡类型
}SPI_SD_CardInfo;

extern SPI_SD_CardInfo SPI_SDcardInfo;				//SD卡信息（初始化时自动获取）
u8 SPI_SD_Init(void);								//用户用，一个函数初始化SPI驱动的SD卡
u8 SPI_SD_ReadDisk2(u8* buf,u32 sector,u8 cnt);		//用户用，读SD卡,fatfs/usb调用
u8 SPI_SD_WriteDisk2(u8* buf,u32 sector,u8 cnt);	//用户用，写SD卡,fatfs/usb调用


u8 SPI_SD_ReadInfo(void);
u32 SD_GetSectorCount(void);

// SD卡类型定义
#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06	   
// SD卡指令表	   
#define CMD0    0       //卡复位
#define CMD1    1
#define CMD8    8       //命令8 ,SEND_IF_COND
#define CMD9    9       //命令9 ,读CSD数据
#define CMD10   10      //命令10,读CID数据
#define CMD12   12      //命令12,停止数据传输
#define CMD16   16      //命令16,设置SectorSize 返回0x00
#define CMD17   17      //命令17,读sector
#define CMD18   18      //命令18,读 Multi sector
#define CMD23   23      //命令23,设置多sector写入前预先擦除N个block
#define CMD24   24      //命令24,写sector
#define CMD25   25      //命令25,写Multi sector
#define CMD41   41      //命令41,返回0x00
#define CMD55   55      //命令55,返回0x01
#define CMD58   58      //命令58,读OCR信息
#define CMD59   59      //命令59,使能/禁止CRC,应返回0x00
//数据写入回应字意义
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF
//SD卡回应标志字
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF


/*
网上总结：
		对SD卡的驱动主要有初始化、读、写、擦除等。
		1、 初始化 步骤：
		（1）      延时至少74clock
		（2）      发送CMD0，需要返回0x01，进入Idle状态
		（3）      循环发送CMD55+ACMD41，直到返回0x00，进入Ready状态。
		如果是MMC，此步应发送CMD1。
		2、 读 步骤：
		（1）      发送CMD17（单块）或CMD18（多块）读命令，返回0x00
		（2）      接收数据开始令牌0xfe（或0xfc） + 正式数据512Bytes + CRC 校验2Bytes
		默认正式传输的数据长度是512Bytes，可用CMD16设置。
		3、 写 步骤：
		（1）      发送CMD24（单块）或CMD25（多块）写命令，返回0x00
		（2）      发送数据开始令牌0xfe（或0xfc） + 正式数据512Bytes + CRC校验2Bytes
		4、 擦除 步骤：
		（1）      发送CMD32，跟一个参数来指定首个要擦出的扇区号（SD手册上说是块号）
		（2）      发送CMD33,，指定最后的扇区号
		（3）      发送CMD38，擦除指定区间的扇区
		此3步顺序不能颠倒。
		还要注意发送CMD命令时，后面要跟一个字节的CRC校验数据，总之要保证每次发送的数据包长度符合协议要求，命令、数据符合时序要求。
		
		
		 在sd卡初始化流程里面，和其它的sd卡初始化流程大体上是一样的，一些特别的地方有：
			1.在初始化命令发送前，需要发送72个时钟周期的时钟信号给sd卡，作为使能信号
			2.发送cmd0时，如果sd卡检测到DAT3为拉低状态，就会自动进入SPI模式，并且发送一个不为0xff的值作为回应
			3.在发送acmd41时，命令跟随的值不是0xff8000，而是0
			4.在读取sd卡cid寄存器时使用的命令是cmd10而不是cmd2
			5.spi模式不支持选择卡命令cmd3
*/



#endif

