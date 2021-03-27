#include "conversion.h"

//x 个 Byte的补码转源码，做多64bit（8字节）
signed long int Comp_to_origin_xByte(const signed long int comp_num, const unsigned char xByte)
{
	unsigned char i;
	unsigned long int temp;
	//补码转源码，正数的补码不变，负数的补码为源码除符号位取反加一，负数的补码再除符号位取反加一得原码
	//判断最高位是否为1
	if(comp_num & (0x80 << (xByte - 1)) )
	{
		//构造 0x7f_ff_ff（3字节为例）
		for(i = 0;i < xByte;i++)
		{
			if(i == 0)
			{
				temp = (0x7F << ((xByte - 1) * 8) );
			}else
			{
				temp |= 0xFF << ((xByte - i - 1) * 8);
			}
		}
		//除符号位取反加一
		return ( (comp_num ^ temp) + 1 );
	}else
	{
		return comp_num;
	}
}
 
//二进制取反  
int BinFlip(unsigned char *dst, const unsigned char *src, int length)  
{  
	int i;  

	for(i=0; i<length; i++)  
	{  
			dst[i] = src[i]^0xFF;  
	}  
	return 0;  
}  

/* 将二进制数转成十进制数*/
int BintoDec(char *str)
{
    int sum =0;
    int j = 1;
    /*用作循环上限*/
    int pos = BitNum - 1;
    for(;pos>=0;pos--)
    {
        /*刚开始，pos最大，对应的是数组的最右边*/
        sum += (str[pos]-'0')*j;
        j *= 2;
    }
    return sum;
}



//十六进制转为十进制  
unsigned long HextoDec(const unsigned char *hex, int length)  
{  
    int i;  
    unsigned long rslt = 0;  
   
    for(i=0; i<length; i++)  
    {  
        rslt += (unsigned long)(hex[i])<<(8*(length-1-i));  
                                                           
    }  
   
    return rslt;  
}  
   
   
 
//十进制转十六进制   
int DectoHex(int dec, unsigned char *hex, int length)  
{  
    int i;  
   
    for(i=length-1; i>=0; i--)  
    {  
        hex[i] = (dec%256)&0xFF;  
        dec /= 256;  
    }  
   
    return 0;  
}  
   
  
// 求权  
//  
// 输入： int base                    进制基数  
//        int times                   权级数  
// 返回： unsigned long               当前数据位的权  
//暂时看不懂。。
unsigned long power(int base, int times)  
{   
    int i;  
    unsigned long rslt = 1;  
   
    for(i=0; i<times; i++)  
        rslt *= base;  
   
    return rslt;  
}

/*将十进制数转化成二进制输出*/
void DectoBin(DateType num,char *code,int bitNum)
{
  //  findNegcode(num,code,bitNum);
    findNegCode_iter(num,code,bitNum,0);
    /*递归终止条件*/
    if(num>0)   return;
    /**/
   // findNegcode(num*(-1),code,bitNum);
    findNegCode_iter(num*(-1),code,bitNum,0);
    code[0] = '1';
}
   



//十进制转BCD编码
unsigned char DecToBCD(unsigned char _dec)
{
        unsigned char temp = 0;
        while (_dec >= 10)
        {
                temp++;
                _dec -= 10;
        }
        return ((unsigned char)(temp << 4) | _dec);
}


//BCD编码转十进制
unsigned char BCDToDec(unsigned char _BCD)
{
        unsigned char temp = 0;

        temp = ((unsigned char)(_BCD & (unsigned char)0xF0) >> (unsigned char)0x04) * 10;
        return (temp + (_BCD & (unsigned char)0x0F));
}


