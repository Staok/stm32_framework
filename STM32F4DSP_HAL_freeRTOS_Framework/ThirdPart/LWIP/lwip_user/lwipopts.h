#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

/*	�� SYS_SUPPORT_LWIP Ϊ0ʱ������LWIP������ڴ��FLASHҲ���ᱻ����
	
	LWIPռ�ô��45k��FLASH��20K(memp)+10K(MEM_SIZE��С)��RAM
	
	opt.h�������� MEMP_MEM_MALLOC ��Ϊ 1 ��Ĭ�ϣ��Ͳ�Ҫ���ˣ�
		��Ϊ ��memp���ڴ��mem.h��ram_heap�������ȡ��
		������������� MEM_SIZE �Ĵ�СΪ����֮���ٴ�һ�㣬
		Ĭ������Ϊ 30*1024
	
	STM32F407ZGT6 ������Ϣ��1024KB(hd)FLASH��128KB��InrRAM��+ 64KB��InrCCM��RAM
	
	��malloc.h�ڴ������������£��������ⲿSRAM��
		InrRAM_MAX_SIZE			60*1024
		ExRAM1_MAX_SIZE			960*1024 + 30*1024
		InrCCM_MAX_SIZE			60*1024
	
	�� MEM_SIZE Ϊ 35*1024 �� ����ExRAM ������£��������պ��������룬FLASH��SRAM���Ļ��б仯����������ݽ�������Աȣ�
		δ����LWIPʱ������Ϣ��	Program Size: Code=28206 RO-data=4298 RW-data=1156 ZI-data=144076  
		����LWIPʱ������Ϣ��	Program Size: Code=73258 RO-data=4790 RW-data=1344 ZI-data=145568  
		
	����ע�⣺���Ƽ�LWIPռ��CCM�ڴ棬��ΪCCM�ڴ治�ܱ�DMA���ʣ�
	����ע�⣺ʹ�����ص�SRAMģʽ���Գ���ʱ���ܿ��� SYS_SUPPORT_LWIP ��
				��Ϊ..��������Ŀռ䲻��...��������һЩ������ռ����ڴ����ݿռ��ֲ�����..
*/
#define SYS_SUPPORT_LWIP		0

#define SYS_Test_LWIP			1	//��ֻ����LWIP�����е������κ�TCP/UDPͨѶʱ�򿪣�����ر�
									//��ʱ��TCP,UDP,HTTP��APP��ش��벻�ᱻ����

#define SYS_LIGHTWEIGHT_PROT    0

//NO_SYS==1:��ʹ�ò���ϵͳ
#define NO_SYS                  1  //��ʹ�ò���ϵͳ

/*�������ӿ��Ƿ����*/
#define LWIP_SINGLE_NETIF               0
#define LWIP_NETIF_HOSTNAME             0
#define LWIP_NETIF_API                  0
#define LWIP_NETIF_STATUS_CALLBACK      0
#define LWIP_NETIF_EXT_STATUS_CALLBACK  0
#define LWIP_NETIF_LINK_CALLBACK        0
#define LWIP_NETIF_REMOVE_CALLBACK      1 //֧�ֵ���ʱ�ص�����
#define LWIP_NETIF_HWADDRHINT           0


//ʹ��4�ֽڶ���ģʽ
#define MEM_ALIGNMENT           4  

//MEM_SIZE:heap�ڴ�Ĵ�С,�����Ӧ�����д������ݷ��͵Ļ����ֵ������ô�һ�� 
#define MEM_SIZE                30*1024 //�ڴ�Ѵ�С
//#define MEM_LIBC_MALLOC			1
//MEMP_NUM_PBUF:memp�ṹ��pbuf����,���Ӧ�ô�ROM���߾�̬�洢�����ʹ�������ʱ,���ֵӦ�����ô�һ��
#define MEMP_NUM_PBUF           10

//MEMP_NUM_UDP_PCB:UDPЭ����ƿ�(PCB)����.ÿ�����UDP"����"��Ҫһ��PCB.
#define MEMP_NUM_UDP_PCB        6

//MEMP_NUM_TCP_PCB:ͬʱ���������TCP����
#define MEMP_NUM_TCP_PCB        10

//MEMP_NUM_TCP_PCB_LISTEN:�ܹ�������TCP��������
#define MEMP_NUM_TCP_PCB_LISTEN 6

//MEMP_NUM_TCP_SEG:���ͬʱ�ڶ����е�TCP������
#define MEMP_NUM_TCP_SEG        20

//MEMP_NUM_SYS_TIMEOUT:�ܹ�ͬʱ�����timeout����
#define MEMP_NUM_SYS_TIMEOUT    5


/* ---------- Pbufѡ��---------- */
//PBUF_POOL_SIZE:pbuf�ڴ�ظ���. 
#define PBUF_POOL_SIZE          10

//PBUF_POOL_BUFSIZE:ÿ��pbuf�ڴ�ش�С. 
#define PBUF_POOL_BUFSIZE       1500


/* ---------- TCPѡ��---------- */
#define LWIP_TCP                1  //Ϊ1��ʹ��TCP
#define TCP_TTL                 255//����ʱ��

/*��TCP�����ݶγ�������ʱ�Ŀ���λ,���豸���ڴ��С��ʱ�����ӦΪ0*/
#define TCP_QUEUE_OOSEQ         0

//���TCP�ֶ�
#define TCP_MSS                 (1500 - 40)	  //TCP_MSS = (MTU - IP��ͷ��С - TCP��ͷ��С

//TCP���ͻ�������С(bytes).
#define TCP_SND_BUF             (4*TCP_MSS)

//TCP_SND_QUEUELEN: TCP���ͻ�������С(pbuf).���ֵ��СΪ(2 * TCP_SND_BUF/TCP_MSS) 
#define TCP_SND_QUEUELEN        (4* TCP_SND_BUF/TCP_MSS)

//TCP���ʹ���
#define TCP_WND                 (2*TCP_MSS)


/* ---------- ICMPѡ��---------- */
#define LWIP_ICMP               1 //ʹ��ICMPЭ��

/* ---------- DHCPѡ��---------- */
#define LWIP_DHCP               1  //ʹ��DHCP

/* ---------- UDPѡ�� ---------- */ 
#define LWIP_UDP                1 //ʹ��UDP����
#define UDP_TTL                 255 //UDP���ݰ�����ʱ��

/* ---------- DNSѡ��---------- */
#define LWIP_DNS               	1  //ʹ��DNS
#define DNS_TABLE_SIZE                  4
#define DNS_MAX_NAME_LENGTH             256

unsigned int Curl_rand(void);
#define LWIP_RAND				Curl_rand //ʹ����ʵ�ֵ�α�����

/* ---------- Statistics options ---------- */
#define LWIP_STATS 0
#define LWIP_PROVIDE_ERRNO 1


/*
   ----------------------------------------------
   ---------- SequentialAPIѡ��----------
   ----------------------------------------------
*/
//LWIP_NETCONN==1:ʹ��NETCON����(Ҫ��ʹ��api_lib.c)
#define LWIP_NETCONN                    0
/*Sequential layer options*/
#define LWIP_TCPIP_TIMEOUT              0
#define LWIP_NETCONN_SEM_PER_THREAD     0
#define LWIP_NETCONN_FULLDUPLEX         0
/*
   ------------------------------------
   ---------- Socket APIѡ��----------
   ------------------------------------
*/
//LWIP_SOCKET==1:ʹ��Socket API(Ҫ��ʹ��sockets.c)
#define LWIP_SOCKET                     0

#define LWIP_COMPAT_MUTEX               1

#define LWIP_SO_RCVTIMEO                1 //ͨ������LWIP_SO_RCVTIMEOʹ��netconn�ṹ����recv_timeout,ʹ��recv_timeout���Ա��������߳�


/*
   ----------------------------------------
   ---------- Lwip����ѡ��----------
   ----------------------------------------
*/
//#define LWIP_DEBUG                     1 //����DEBUGѡ��

#define ICMP_DEBUG                      LWIP_DBG_OFF //����/�ر�ICMPdebug

#endif /* __LWIPOPTS_H__ */
