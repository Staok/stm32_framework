#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

/*	当 SYS_SUPPORT_LWIP 为0时不启用LWIP，相关内存和FLASH也不会被编译
	
	LWIP占用大概45k的FLASH和20K(memp)+10K(MEM_SIZE大小)的RAM
	
	opt.h里面配置 MEMP_MEM_MALLOC 设为 1 （默认，就不要动了）
		即为 让memp的内存从mem.h（ram_heap）里面获取，
		所以设置下面的 MEM_SIZE 的大小为二和之和再大一点，
		默认设置为 30*1024
	
	STM32F407ZGT6 容量信息：1024KB(hd)FLASH，128KB（InrRAM）+ 64KB（InrCCM）RAM
	
	在malloc.h内存分配如下情况下：（不开外部SRAM）
		InrRAM_MAX_SIZE			60*1024
		ExRAM1_MAX_SIZE			960*1024 + 30*1024
		InrCCM_MAX_SIZE			60*1024
	
	在 MEM_SIZE 为 35*1024 且 不开ExRAM 的情况下：（随着日后增添代码，FLASH和SRAM消耗会有变化，下面的数据仅供互相对比）
		未开启LWIP时编译信息：	Program Size: Code=28206 RO-data=4298 RW-data=1156 ZI-data=144076  
		  开启LWIP时编译信息：	Program Size: Code=73258 RO-data=4790 RW-data=1344 ZI-data=145568  
		
	严重注意：不推荐LWIP占用CCM内存，因为CCM内存不能被DMA访问！
	严重注意：使用下载到SRAM模式调试程序时不能开启 SYS_SUPPORT_LWIP ，
				因为..留给程序的空间不够...如果多分配一些给程序空间那内存数据空间又不够了..
*/
#define SYS_SUPPORT_LWIP		0		/*是否启用 LWIP 2.1.2*/

#define NO_SYS                  1 	 	/*为1表示不使用操作系统*/
										/*
											当选择不使用操作系统时，TCP/UDP会只编译RAW API相关的程序， 并在 sys_arch.c 里面不提供操作系统相关的API ！注意严禁在操作系统的任务中调用   RAW API函数！
											当选择使用操作系统时，  TCP/UDP会只编译CONN API相关的程序，并在 sys_arch.c 里面提供操作系统相关的API ！  注意严禁在非操作系统的环境中调用CONN API函数！
										*/
										
#define SYS_Test_LWIP			1		//当只测试LWIP的运行但不做任何TCP/UDP通讯时打开，否则关闭
										//打开时，TCP,UDP,HTTP等APP相关代码不会被编译
#if SYS_Test_LWIP == 0
	/*可选使用TCP UDP HTTP服务*/
	#define LWIP_DHCP               1  	//使用DHCP
	#define LWIP_DNS               	1  	//使用DNS
	#define LWIP_TCP                1  	//使用TCP	（使用方法详看docs.bin说明手册）
	#define LWIP_UDP                1 	//使用UDP	（使用方法详看docs.bin说明手册）
	#define LWIP_HTTP				0	//使用HTTP	（使用方法详看docs.bin说明手册）
										/*！注意：	使用HTTP必须打开 LWIP_TCP
													httpd.c 里面的宏配置不用动！*/
										/*编译信息（仅供相互对比）：
										  开启 LWIP_HTTP 时：		Program Size: Code=82958 RO-data=66606 RW-data=1400 ZI-data=146872  
										未开启 LWIP_HTTP 时：		Program Size: Code=73646 RO-data=4794  RW-data=1344 ZI-data=145632  
										*/
#else
	/*均不使用*/
	#define LWIP_DHCP               0
	#define LWIP_DNS               	0
	#define LWIP_TCP                0
	#define LWIP_UDP                0
	#define LWIP_HTTP				0
#endif

#define SYS_LIGHTWEIGHT_PROT    0



/*相关网络接口是否编译*/
#define LWIP_SINGLE_NETIF               0
#define LWIP_NETIF_HOSTNAME             0
#define LWIP_NETIF_API                  0
#define LWIP_NETIF_STATUS_CALLBACK      0
#define LWIP_NETIF_EXT_STATUS_CALLBACK  0
#define LWIP_NETIF_LINK_CALLBACK        0
#define LWIP_NETIF_REMOVE_CALLBACK      1 //支持掉线时回调函数
#define LWIP_NETIF_HWADDRHINT           0


//使用4字节对齐模式
#define MEM_ALIGNMENT           4  

//MEM_SIZE:heap内存的大小,如果在应用中有大量数据发送的话这个值最好设置大一点 
#define MEM_SIZE                30*1024 //内存堆大小
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
#define LWIP_ICMP               1 //使用ICMP协议

/* ---------- UDP选项 ---------- */ 

#define UDP_TTL                 255 //UDP数据包生存时间

/* ---------- DNS选项---------- */
#define DNS_TABLE_SIZE			4
#define DNS_MAX_NAME_LENGTH		256

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
/*Sequential layer options*/
#define LWIP_TCPIP_TIMEOUT              0
#define LWIP_NETCONN_SEM_PER_THREAD     0
#define LWIP_NETCONN_FULLDUPLEX         0
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


//STM32F4x7允许通过硬件识别和计算IP,UDP和ICMP的帧校验和
#define CHECKSUM_BY_HARDWARE //定义CHECKSUM_BY_HARDWARE,使用硬件帧校验

#ifdef CHECKSUM_BY_HARDWARE
  //CHECKSUM_GEN_IP==0: 硬件生成IP数据包的帧校验和
  #define CHECKSUM_GEN_IP                 0
  //CHECKSUM_GEN_UDP==0: 硬件生成UDP数据包的帧校验和
  #define CHECKSUM_GEN_UDP                0
  //CHECKSUM_GEN_TCP==0: 硬件生成TCP数据包的帧校验和
  #define CHECKSUM_GEN_TCP                0 
  //CHECKSUM_CHECK_IP==0: 硬件检查输入的IP数据包帧校验和
  #define CHECKSUM_CHECK_IP               0
  //CHECKSUM_CHECK_UDP==0: 硬件检查输入的UDP数据包帧校验和
  #define CHECKSUM_CHECK_UDP              0
  //CHECKSUM_CHECK_TCP==0: 硬件检查输入的TCP数据包帧校验和
  #define CHECKSUM_CHECK_TCP              0
#else
  //CHECKSUM_GEN_IP==1: 软件生成IP数据包帧校验和
  #define CHECKSUM_GEN_IP                 1
  // CHECKSUM_GEN_UDP==1: 软件生成UDOP数据包帧校验和
  #define CHECKSUM_GEN_UDP                1
  //CHECKSUM_GEN_TCP==1: 软件生成TCP数据包帧校验和
  #define CHECKSUM_GEN_TCP                1
  // CHECKSUM_CHECK_IP==1: 软件检查输入的IP数据包帧校验和
  #define CHECKSUM_CHECK_IP               1
  // CHECKSUM_CHECK_UDP==1: 软件检查输入的UDP数据包帧校验和
  #define CHECKSUM_CHECK_UDP              1
  //CHECKSUM_CHECK_TCP==1: 软件检查输入的TCP数据包帧校验和
  #define CHECKSUM_CHECK_TCP              1
#endif

#endif /* __LWIPOPTS_H__ */

