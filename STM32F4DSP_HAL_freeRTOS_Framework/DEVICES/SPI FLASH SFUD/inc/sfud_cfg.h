/*
 * This file is part of the Serial Flash Universal Driver Library.
 *
 * Copyright (c) 2016-2018, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: It is the configure head file for this library.
 * Created on: 2016-04-23
 */

#ifndef _SFUD_CFG_H_
#define _SFUD_CFG_H_

/*开源库地址：https://github.com/armink/SFUD*/

/*注意，开源库更新时，sfud_cfg.h 和 sfud_port.c 和 sfud_flash_def.h 这三个文件绝对不要直接覆盖掉！
	因为这三个文件，被本工程"stm32_framework"修改过*/

/*支持的串行FLASH型号：
	1、所有支持SFUD标准的型号
		描述：SFUD标准规定了，每个 Flash 中会存在一个参数表，该表中会存放 Flash 容量、写粒度、擦除命令、地址模式等 Flash 规格参数。
				所以本库在初始化时会优先读取 SFDP 表参数。
	2、本库默认适配的型号（具体在 sfud_flash_def.h 中的 SFUD_FLASH_CHIP_TABLE 里面列出）
	3、用户添加的型号：
		1、在 SFUD_FLASH_CHIP_TABLE的最下边添加这个型号的信息：
		例如：| 名称 | 制造商 ID | 类型 ID | 容量 ID | 容量 | 写模式 | 擦除粒度（擦除的最小单位） | 擦除粒度对应的命令 | 
				{"GD25Q64B", SFUD_MF_ID_GIGADEVICE, 0x40, 0x17, 8*1024*1024, SFUD_WM_PAGE_256B, 4096, 0x20},
		2、然后再在下面的 SFUD_FLASH_DEVICE_TABLE 添加FLASH的用户设备信息
*/

#define SFUD_Use_simuSPI_or_hardwareSPI	1	/*1：使用硬件SPI外设（默认SPI1），0：使用软件模拟SPI*/

#define SFUD_DEBUG_MODE
/*从串口打印信息*/

//#define SFUD_USING_QSPI
/*开启后，SFUD 也将支持使用 QSPI 总线连接的 Flash*/

/*
	若用户的FLASH芯片本库默认有这个型号：
		则在这里添加 SFUD_FLASH_CHIP_TABLE 里面有的芯片的用户设备信息，绝对不能添加没有的
	
	若用户的FLASH芯片本库默认没有这个型号：
		1、在 SFUD_FLASH_CHIP_TABLE的最下边添加这个型号的信息：
			例如：| 名称 | 制造商 ID | 类型 ID | 容量 ID | 容量 | 写模式 | 擦除粒度（擦除的最小单位） | 擦除粒度对应的命令 | 
					{"GD25Q64B", SFUD_MF_ID_GIGADEVICE, 0x40, 0x17, 8*1024*1024, SFUD_WM_PAGE_256B, 4096, 0x20},
		2、然后再在这里添加FLASH的用户设备信息
*/
enum {
	SFUD_BY25Q128ASSIG_DEVICE_INDEX = 0,
};

#define SFUD_FLASH_DEVICE_TABLE                                               		\
{                                                                              		\
	[SFUD_BY25Q128ASSIG_DEVICE_INDEX] = {.name = "BY25Q128ASSIG", .spi.name = "SPI1"},		\
}


/*
	API：
&		初始化 SFUD 库：		sfud_err sfud_init(void)
			描述：逐一初始化设备表 SFUD_FLASH_DEVICE_TABLE 宏中定义的的所有串行FLASH
			注意：初始化完的 SPI Flash 默认都 已取消写保护 状态，如需开启写保护，请使用 sfud_write_status 函数修改 SPI Flash 状态。
		
&		获取 Flash 设备对象：	sfud_flash *sfud_get_device(size_t index)
			描述：输出参数为Flash 设备位于 FLash 设备表（SFUD_FLASH_DEVICE_TABLE）中的索引值，
				  本方法通过 Flash 设备位于设备表中索引值来返回 Flash 设备对象，超出设备表范围返回 NULL
			例如：				const sfud_flash *flash = sfud_get_device(SFUD_BY25Q128ASSIG_DEVICE_INDEX);
		
&		初始化指定的 Flash 设备：sfud_err sfud_device_init(sfud_flash *flash)
		
			
&		读取 Flash 数据：		sfud_err sfud_read(const sfud_flash *flash, uint32_t addr, size_t size, uint8_t *data)
			描述：	输入参数：	flash	Flash 设备对象
								addr	起始地址
								size	从起始地址开始读取数据的总大小
								data	读取到的数据
		
&		往 Flash 写数据：		sfud_err sfud_write(const sfud_flash *flash, uint32_t addr, size_t size, const uint8_t *data)
			描述：输入参数：	flash	Flash 设备对象
								addr	起始地址
								size	从起始地址开始写入数据的总大小
								data	待写入的数据
		
		使能快速读模式（仅当 SFUD 开启 QSPI 模式后可用）：略，要用的话看官方手册和F4L的例程
			描述：相比传统的 SPI 模式，使用 QSPI 能够加速 Flash 数据的读取，但当数据需要写入时，由于 Flash 本身的数据写入速度慢于 SPI 传输速度，所以 QSPI 模式下的数据写入速度提升并不明显。
		
		读取 Flash 状态：略
		写（修改） Flash 状态：略
		
		擦除 Flash 数据：（少用）sfud_err sfud_erase(const sfud_flash *flash, uint32_t addr, size_t size)	
			描述：	输入参数：	flash	Flash 设备对象
								addr	起始地址
								size	从起始地址开始擦除数据的总大小
			注意：擦除操作将会按照 Flash 芯片的擦除粒度（详见 Flash 数据手册，一般为 block 大小。
					初始化完成后，可以通过 sfud_flash->chip.erase_gran 查看）对齐，请注意保证起始地址和擦除数据大小按照 Flash 芯片的擦除粒度对齐，否则执行擦除操作后，将会导致其他数据丢失。
		
		擦除 Flash 全部数据：（少用）sfud_err sfud_chip_erase(const sfud_flash *flash)


*/
	


		
/*以下默认不用动*/
/*
如果想再减少编译后占用FLASH  2KB和RAM 0.1KB：
就把下面两个宏都注释掉，然后自己在初始化的地方自己定义FLASH芯片信息和调用 sfud_device_init() 对这个FLASH完成初始化

sfud_flash sfud_norflash0 = {
        .name = "norflash0",
        .spi.name = "SPI1",
        .chip = { "W25Q64FV", SFUD_MF_ID_WINBOND, 0x40, 0x17, 8L * 1024L * 1024L, SFUD_WM_PAGE_256B, 4096, 0x20 } };
......
sfud_device_init(&sfud_norflash0);
......
*/
	#define SFUD_USING_SFDP	/*是否使用 SFDP 参数功能，库会优先在默认支持的型号中找参数，若找不到则读取芯片SFUD数据*/
	/*关闭后只会查询该库在 /sfud/inc/sfud_flash_def.h 中提供的 Flash 信息表。这样虽然会降低软件的适配性，但减少代码量。*/

	#define SFUD_USING_FLASH_INFO_TABLE	/*是否使用该库自带的 Flash 参数信息表*/
	/*关闭后该库只驱动支持 SFDP 规范的 Flash，也会适当的降低部分代码量。*/

void sfud_log_debug(const char *file, const long line, const char *format, ...); /*不用动*/
void sfud_log_info(const char *format, ...); /*不用动*/
#endif /* _SFUD_CFG_H_ */


