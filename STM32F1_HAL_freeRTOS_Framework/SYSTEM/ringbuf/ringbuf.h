#ifndef _RINGBUF_H
#define _RINGBUF_H


#define ringbuf_MaxNum 200

typedef struct
{
	char ringbuf[ringbuf_MaxNum];
	int ad2write;
	int ad2read;
	char data;
    char writeOverRing;
    char readOverRing;
	char is_success;
}RingBuff_Typedef;

//串口1缓存区公共变量结构体
extern RingBuff_Typedef RingBuff_forUSART1;

#define ReturnOK	0
#define ReturnErr	1
char WriteDataToRingbuff(RingBuff_Typedef* ringbuf_Struct, char Data);
char ReadDataFromRingbuff(RingBuff_Typedef* ringbuf_Struct);



#endif

/*
例子程序：
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define random(x) (rand()%x)

int main()
{
	char buf[200];
	srand((int)time(0));
    
//    未来能够测试环形缓存，试验如下：
//        在缓存大小为200字节的情况下：
//        先存入150个字节，然后读出100个字节；
//        然后再存100个字节，然后读出150个字节；
//        检查数据是否符合FIFO先入先出，检查是否存在数据丢失。
    

    printf("\nstart write 150 bytes\n");
    for(i = 0;i < 150;i++)
    {
        buf[i] = (char)random(20);
        printf("%d\t",buf[i]);
        if(WriteDataToRingbuff(&RingBuff_forUSART1,buf[i]) == ReturnErr)
        {
            printf("\nwrite failed\n");
            while(1);
        }
    }
    printf("\nstart read 100 bytes\n");
    for(i = 0;i < 100;i++)
    {
        if(ReadDataFromRingbuff(&RingBuff_forUSART1) == ReturnErr)
        {
            printf("\nread failed\n");
            while(1);
        }else{
            printf("%d\t",RingBuff_forUSART1.data);
        }
    }
    printf("\nstart write 100 bytes\n");
    for(i = 0;i < 100;i++)
    {
        buf[i] = (char)random(20);
        printf("%d\t",buf[i]);
        if(WriteDataToRingbuff(&RingBuff_forUSART1,buf[i]) == ReturnErr)
        {
            printf("\nwrite failed\n");
            while(1);
        }
    }
    printf("\nstart read 151 bytes\n");
    for(i = 0;i < 151;i++)
    {
        if(ReadDataFromRingbuff(&RingBuff_forUSART1) == ReturnErr)
        {
            printf("\nread failed\n");
            while(1);
        }else{
            printf("%d\t",RingBuff_forUSART1.data);
        }
    }
}

*/


