#ifndef __CONVERSION_H
#define __CONVERSION_H

#include "PeriphConfigCore.h"
#include "PeriphConfig.h"


//x 个 Byte的补码转源码，做多64bit（8字节）
signed long int Comp_to_origin_xByte(const signed long int comp_num, const unsigned char xByte);

//二进制取反  
int BinFlip(unsigned char *dst, const unsigned char *src, int length);

/* 将二进制数转成十进制数*/
int BintoDec(char *str);

//十六进制转为十进制  
unsigned long HextoDec(const unsigned char *hex, int length);

//十进制转十六进制   
int DectoHex(int dec, unsigned char *hex, int length);

/*将十进制数转化成二进制输出*/
void DectoBin(DateType num,char *code,int bitNum);

//十进制转BCD编码
unsigned char DecToBCD(unsigned char _dec);

//BCD编码转十进制
unsigned char BCDToDec(unsigned char _BCD);

//暂时看不懂。。
unsigned long power(int base, int times);


#endif



