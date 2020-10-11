/**********************************************************************************
	Copyright (C) 2016 SOL.lab <maxwellxyao@foxmail.com>

*File name:		strprintf.c
*Description:	格式化字符串核函数
*Author:		MaxwellXyao
*Version:		V0.0.20160626
*Date:			2016-6-26 02:40:29
*History:		
[2016-6-26]	文件创建;

**********************************************************************************/
#include "strprintf.h"
#include<stdarg.h> 	
	

//###########################【函数】###########################

/*****************************************************************
*Function:		_strprintf
*Description:	格式化字符串核，参考FATFS0.09a中ff.c下f_printf()函数。
*Input:			向设备输出字符函数指针，
				格式化字符串，
				变长参数传递变量
*Output:		空
*Other:			必须被使用变长参数的壳函数调用，壳函数由用户构造
******************************************************************/
void _strprintf(void (*myputc)(char),const char* str,va_list arp)		//sprintf核函数（需被含有变长参数的函数调用）
{
	unsigned char f,r,fl=0,l=3,lt,jj;		//默认留3位小数,小数可留0~7位
	unsigned char i,j,w,lp;
	unsigned long v;
	char c, d, s[16], *p;
	int res, chc, cc,ll;
	int kh,kl,pow=1;																
	double k;

	for (cc=res=0;cc!=-1;res+=cc) 		   			//解析格式化字符串，且输出
	{
		c = *str++;						   			//每一轮取一个字符
//---------------------------------//读取到'\0',结束程序
		if (c == 0) break;					
//---------------------------------//读取到非'%'符号时
		if (c != '%') {						
			myputc(c);									
			continue;
		}
//---------------------------------//读取到'%'符号时
		w=f=0;
		k=0;
		lp=0;
		c=*str++;				   					//越过'%'，读其格式
		if (c == '0') {								//%0，0填充
			f = 1; c = *str++;						//f0填充标记位置1，读取下一个字符
		} 
		else if (c == '-') {							//%-，左对齐（左边填空格）
				f = 2; c = *str++;					//f左对齐标记位置1，读取下一个字符
			}
		else if (c == '.') {						//%.3f表示留3位小数
			fl=1;c=*str++;
		}

		while (((c)>='0')&&((c)<='9')) {			//"%030"，将30转换为数字
			if(fl==1){
				lp=lp*10+c-'0';		
				c=*str++;
			}
			else{
				w=w*10+c-'0';		
				c=*str++;
			}								//将数字读完
		}
		if(fl==1) l=(lp>7)? 7:lp;
		if (c == 'l' || c == 'L') {					//%ld等长形数
			f |= 4; c = *str++;		 				//f长形标记位置1，读下一个字符
		}
		if (!c) break;								//如果此时无字符，结束输出
//---------------------------------//处理格式化标识符（d,s,c,x,o,f）
		d = c;						
		if (((c)>='a')&&((c)<='z')) d -= 0x20;		//如果是小写，划归成大写处理
		switch (d) {								//分类%*的情况

		case 'S' :					/* String */
			p = va_arg(arp,char*);					//取字符串变量
			for(j=0;p[j];j++);						//长度计算
			ll=j;
			chc = 0;
			if (!(f&2)) {							//不用左对齐，左边就要补空格%06s
				while (j++ < w) 
				{
					myputc(' ');
					chc+=1;					
				}
			}

			jj=0;	
			while (p[jj]!='\0')
			{		
				myputc(p[jj]);										
				jj++;
			}
											
			chc+=ll;
			while (j++ < w) 						//左对齐，左边就不用空格，右边填空格%-06s
			{
				myputc(' ');
				chc+=1;						
			}	 
			cc = chc;
			continue;

		case 'C' :
		{					/* Character */
			myputc((char)va_arg(arp,int));	continue;
		}

		case 'F' :											//默认保留3位小数
		{													/* double(64位)/float(32位) */												
			k=va_arg(arp,double);
			if(k<0){
				l|=8;
				k*=-1;										//负数置低位1
			}
			kh=(int)k;										//整数小数分离
			pow=1;
			lt=l&7;
			while((lt-1)>=0){
				pow*=10;
				lt--;
			}
			kl=(int)(pow*(k-kh));				
			i=0;

			lt=l&7;
			while(lt--){												//存入小数部分
				if(kl){
					d=(char)(kl%10);							//按10进制取余，即取个位，化归为字符处理
					kl/=10;										//递进								
					s[i++]='0'+d;								//倒叙存入s
				}
				else s[i++]='0';
			}

			s[i++]='.';										//加入小数点
			do{												//存入整数部分
				d=(char)(kh%10);								//按10进制取余，即取个位，化归为字符处理
				kh/=10;										//递进
				s[i++]='0'+d;								//倒叙存入s
			}while(kh && i<sizeof s /sizeof s[0]);
			if (l&8)s[i++]='-';								//添加符号（注意低位在前）
			fl=0;	
			goto PRT;
		}
		case 'B' :					/* Binary */
			r = 2; break;
		case 'O' :					/* Octal */
			r = 8; break;
		case 'D' :					/* Signed decimal */
		case 'U' :					/* Unsigned decimal */
			r = 10; break;
		case 'X' :					/* Hexdecimal */
			r = 16; break;
		default:{					/* Unknown type (pass-through) */
				myputc(c);								
				cc=1;
				continue;
			}
		}

		/* Get an argument and put it in numeral */
/*取变量  是%ld型？		有则按long取变量			没有l但有d则按int取变量						没有l也没有d则先按unsigned int取变量在做处理	*/
		v =(f&4)?(unsigned long)va_arg(arp, long):((d=='D')?(unsigned long)(long)va_arg(arp, int):(unsigned long)va_arg(arp, unsigned int));
		if (d == 'D' && (v & 0x80000000)) {					//是%d，且符号位为1（负数）	//只有int型有增幅之分
			v = 0 - v;										//化归为正数处理
			f |= 8;											//f符号标记位置1
		}
		i = 0;
		do {												//数字解析成字符
			d=(char)(v%r);									//按进制r取余，即取个位，化归为字符处理
			v/=r;											//递进
			if(d>9)d+=(c=='x')?0x27:0x07;					//判读，不是10进制而是16进制时，d欲先加一个间隔跳转至'A'(大写X)或'a'(小写X)
			s[i++]=d+'0';									//化归为字符串处理（注意低位在前）
		} while (v && i<sizeof s /sizeof s[0]);				//（i<sizeof s /sizeof s[0]）防止含0类数字误判
		if (f & 8) s[i++] = '-';							//添加符号（注意低位在前）
PRT:		
		j=i;
		d=(f&1)?'0':' ';									//判断0填充还是左对齐
		while (!(f&2)&&j++<w)
		{
			myputc(d);									
		}
		do 
		{
			myputc(s[--i]);									
		}while(i);

		while (j++ < w) 
		{
			myputc(d);										
		}
	}
}








