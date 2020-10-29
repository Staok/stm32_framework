#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

/*当为0时不启用LWIP，相关内存和FLASH也不会被编译
	LWIP占用大概45k的FLASH和20K(memp)+10K(MEM_SIZE大小)的RAM
	stm32f103c8没有足够的内存
	开LWIP必须选型xE系列，不开LWIP时可以选择xB系列
	（如果用外部SRAM，再用本地malloc为ram_heap分配内存也可以，但是（转折），
		一个小小的stm32f103xB系列为了上ETH，不仅速度慢，还要破费买昂贵的SRAM和ETH芯片（二者都是10元起步），
		不划算，所以F103这里不采用malloc分配内存）
	
	xE系列容量信息：FLASH:512K(hd)	SRAM:64K
	
	opt.h里面配置 MEMP_MEM_MALLOC 设为 1 （默认，就不要动了）
		即为 让memp的内存从mem.h（ram_heap）里面获取，
		所以设置下面的 MEM_SIZE 的大小为二和之和再大一点，
		默认设置为 30*1024
	
	在 MEM_SIZE			30*1024 
	   InrRAM_MAX_SIZE	20*1024
	的情况下：（随着日后增添代码，FLASH和SRAM消耗会有变化，下面的数据仅供互相对比）
		不开LWIP后的编译信息：			Program Size: Code=21628 RO-data=4060 RW-data=1008 ZI-data=24016	
		开LWIP后的编译信息：
			MEMP_MEM_MALLOC 为 1 时：	Program Size: Code=66756 RO-data=4608 RW-data=1192 ZI-data=56312	
			MEMP_MEM_MALLOC 为 0 时：	在上一行的基础上，FLASH基本不变，SRAM消耗增加约20KB（这种情况不推荐）
*/

#define SYS_SUPPORT_LWIP		0	

#define SYS_Test_LWIP			1	//当只测试LWIP的运行但不做任何TCP/UDP通讯时打开，否则关闭
									//打开时，TCP,UDP,HTTP等APP相关代码不会被编译

#define SYS_LIGHTWEIGHT_PROT    0

//NO_SYS==1:不使用操作系统
#define NO_SYS                  1  //不使用UCOS操作系统

//使用4字节对齐模式
#define MEM_ALIGNMENT           4  

//MEM_SIZE:heap内存的大小,如果在应用中有大量数据发送的话这个值最好设置大一点 
#define MEM_SIZE                30*1024 //内存堆大小
//#define MEM_LIBC_MALLOC			1
//MEMP_NUM_PBUF:memp结构的pbuf数量,如果应用从ROM或者静态存储区发送大量数据时,这个值应该设置大一点
#define MEMP_NUM_PBUF           10

//MEMP_NUM_UDP_PCB:UDP协议控制块(PCB)数量.每个活动的UDP"连接"需要一个PCB.
#define MEMP_NUM_UDP_PCB        6

//MEMP_NUM_TCP_PCB:同时建立激活的TCP数量
#define MEMP_NUM_TCP_PCB        10

//MEMP_NUM_TCP_PCB_LISTEN:能够监听的TCP连接数量
#define MEMP_NUM_TCP_PCB_LISTEN 6

//MEMP_NUM_TCP_SEG:最多同时在队列中的TCP段数量
#define MEMP_NUM_TCP_SEG        20

//MEMP_NUM_SYS_TIMEOUT:能够同时激活的timeout个数
#define MEMP_NUM_SYS_TIMEOUT    5


/* ---------- Pbuf选项---------- */
//PBUF_POOL_SIZE:pbuf内存池个数. 
#define PBUF_POOL_SIZE          10

//PBUF_POOL_BUFSIZE:每个pbuf内存池大小. 
#define PBUF_POOL_BUFSIZE       1500


/* ---------- TCP选项---------- */
#define LWIP_TCP                1  //为1是使用TCP
#define TCP_TTL                 255//生存时间

/*当TCP的数据段超出队列时的控制位,当设备的内存过小的时候此项应为0*/
#define TCP_QUEUE_OOSEQ         0

//最大TCP分段
#define TCP_MSS                 (1500 - 40)	  //TCP_MSS = (MTU - IP报头大小 - TCP报头大小

//TCP发送缓冲区大小(bytes).
#define TCP_SND_BUF             (4*TCP_MSS)

//TCP_SND_QUEUELEN: TCP发送缓冲区大小(pbuf).这个值最小为(2 * TCP_SND_BUF/TCP_MSS) 
#define TCP_SND_QUEUELEN        (4* TCP_SND_BUF/TCP_MSS)

//TCP发送窗口
#define TCP_WND                 (2*TCP_MSS)


/* ---------- ICMP选项---------- */
#define LWIP_ICMP                 1 //使用ICMP协议

/* ---------- DHCP选项---------- */
#define LWIP_DHCP               1  //使用DHCP

/* ---------- UDP选项 ---------- */ 
#define LWIP_UDP                1 //使用UDP服务
#define UDP_TTL                 255 //UDP数据包生存时间

/* ---------- DNS选项 ---------- */ 
#define LWIP_DNS				1 //使用DNS服务
unsigned int Curl_rand(void);
#define LWIP_RAND				Curl_rand //使用自实现的伪随机数

/* ---------- Statistics options ---------- */
#define LWIP_STATS 0
#define LWIP_PROVIDE_ERRNO 1


/*
   ----------------------------------------------
   ---------- SequentialAPI选项----------
   ----------------------------------------------
*/

//LWIP_NETCONN==1:使能NETCON函数(要求使用api_lib.c)
#define LWIP_NETCONN                    0

/*
   ------------------------------------
   ---------- Socket API选项----------
   ------------------------------------
*/
//LWIP_SOCKET==1:使能Socket API(要求使用sockets.c)
#define LWIP_SOCKET                     0

#define LWIP_COMPAT_MUTEX               1

#define LWIP_SO_RCVTIMEO                1 //通过定义LWIP_SO_RCVTIMEO使能netconn结构体中recv_timeout,使用recv_timeout可以避免阻塞线程


/*
   ----------------------------------------
   ---------- Lwip调试选项----------
   ----------------------------------------
*/
//#define LWIP_DEBUG                     1 //开启DEBUG选项

#define ICMP_DEBUG                      LWIP_DBG_OFF //开启/关闭ICMPdebug

#endif /* __LWIPOPTS_H__ */

