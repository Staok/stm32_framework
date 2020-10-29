#include "lwip_comm.h" 
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/init.h"

#include "lwip/timeouts.h"
#include "lwip/mem.h"

//#include "lwip/timers.h"		//这几个是LWIP 1.4版本调用的。。
//#include "lwip/tcp_impl.h"
//#include "lwip/ip_frag.h"
//#include "lwip/tcpip.h" 
//#include "lwip/timers.h"
#include "lwip/priv/tcp_priv.h"

#include "NetCard.h"


//用户自定：网卡的名字
#define IFNAME0 'e'
#define IFNAME1 'n'

//lwip信息结构体 
__lwip_dev lwip_inf = 
{
	/*用户自定：
		远程IP地址，IP地址，子网掩码，网关 （MAC地址在NetCard.c文件里面定义）
	*/
	
	//默认远端IP为:192.168.1.100
	.remoteip[0] = 192,
	.remoteip[1] = 168,
	.remoteip[2] = 1,
	.remoteip[3] = 100,
	
	//默认本地IP为:192.168.1.30
	.ip[0] = 192,
	.ip[1] = 168,
	.ip[2] = 1,
	.ip[3] = 30,
	
	//默认子网掩码:255.255.255.0
	.netmask[0] = 255,
	.netmask[1] = 255,
	.netmask[2] = 255,
	.netmask[3] = 0,
	
	//默认网关:192.168.1.1
	.gateway[0] = 192,
	.gateway[1] = 168,
	.gateway[2] = 1,
	.gateway[3] = 1,
	
	//DHCP状态
	.dhcpstatus = 0  //DHCP还未初始化
};


/*_____用户修改底层收发函数________*/

//用于发送数据包的最底层函数(lwip通过netif->linkoutput指向该函数)
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
	//默认只用一个网络接口实体，否则可以选择往哪个网卡发送
	NetCard_SendPacket(p);
	return ERR_OK;
}  
///用于接收数据包的最底层函数
static struct pbuf * low_level_input(struct netif *netif)
{
	//默认只用一个网络接口实体，否则可以选择在哪个网卡接收
	struct pbuf *p;
	p = NetCard_Receive_Packet();
	return p;
}

/*____________________________________对于裸机来说，下面的都不用动了！____________________________________*/


//网卡接收数据，软件循环调用，当接收到数据包后就发送到ethernet_input，其处理后再发到etharp_input（arp层）
//netif:网卡结构体指针
//返回值:ERR_OK,发送正常
//       ERR_MEM,发送失败
err_t ethernetif_input(struct netif *netif)
{
	err_t err;
	struct pbuf *p;
	p=low_level_input(netif);   //调用low_level_input函数接收数据
	if(p==NULL) return ERR_MEM;
	err=netif->input(p, netif); //调用netif结构体中的input字段(一个函数)来处理数据包
	if(err!=ERR_OK)
	{
		LWIP_DEBUGF(NETIF_DEBUG,("ethernetif_input: IP input error\n"));
		pbuf_free(p);
		p = NULL;
	} 
	return err;
}

err_t ethernetif_init(struct netif *netif)
{
	LWIP_ASSERT("netif!=NULL",(netif!=NULL));
#if LWIP_NETIF_HOSTNAME			//LWIP_NETIF_HOSTNAME 
	netif->hostname="lwip";  	//初始化名称
#endif 
	netif->name[0] = IFNAME0; 	//初始化变量netif的name字段
	netif->name[1] = IFNAME1;
	
	//MAC地址设置：高三字节固定为:2.0.0,低三字节用STM32唯一ID
	lwip_inf.mac[0] = NetCardcfg.mac_addr[0];
	lwip_inf.mac[1] = NetCardcfg.mac_addr[0];
	lwip_inf.mac[2] = NetCardcfg.mac_addr[0];
	lwip_inf.mac[3] = NetCardcfg.mac_addr[0];
	lwip_inf.mac[4] = NetCardcfg.mac_addr[0];
	lwip_inf.mac[5] = NetCardcfg.mac_addr[0];
	
	netif->output = etharp_output;//IP层发送数据包函数
	
	netif->linkoutput = low_level_output;//ARP模块发送数据包函数
	
	netif->hwaddr_len = ETHARP_HWADDR_LEN; //设置MAC地址长度,为6个字节
	
	//初始化MAC地址,设置什么地址由用户自己设置,但是不能与网络中其他设备MAC地址重复
	netif->hwaddr[0]=lwip_inf.mac[0]; netif->hwaddr[1]=lwip_inf.mac[1]; netif->hwaddr[2]=lwip_inf.mac[2];
	netif->hwaddr[3]=lwip_inf.mac[3]; netif->hwaddr[4]=lwip_inf.mac[4]; netif->hwaddr[5]=lwip_inf.mac[5];
	
	netif->mtu=1500; //最大允许传输单元,允许该网卡广播和ARP功能
	
	netif->flags = NETIF_FLAG_BROADCAST|NETIF_FLAG_ETHARP|NETIF_FLAG_LINK_UP;
	
	return ERR_OK;
}

struct netif lwip_handle;					//定义一个全局的网络接口句柄

//LWIP初始化(LWIP启动的时候使用)
//返回值:0,成功
//      1,内存错误
//      2,DM9000初始化失败
//      3,网卡添加失败.
u8 lwip_comm_init(void)
{
	//为lwip的memp和mem申请内存，默认让lwip自己管理，所以无需申请
	
	if(NetCard_Init())return 2;			//初始化网卡
	
	lwip_init();						//初始化LWIP内核

	#if LWIP_DHCP		//使用动态IP
		ip4_addr_t ipaddr;  			//ip地址
		ip4_addr_t netmask; 			//子网掩码
		ip4_addr_t gw;      			//默认网关 
		
		ipaddr.addr = 0;
		netmask.addr = 0;
		gw.addr = 0;
	#else				//使用静态IP
		IP4_ADDR(&ipaddr,lwip_inf.ip[0],lwip_inf.ip[1],lwip_inf.ip[2],lwip_inf.ip[3]);
		IP4_ADDR(&netmask,lwip_inf.netmask[0],lwip_inf.netmask[1] ,lwip_inf.netmask[2],lwip_inf.netmask[3]);
		IP4_ADDR(&gw,lwip_inf.gateway[0],lwip_inf.gateway[1],lwip_inf.gateway[2],lwip_inf.gateway[3]);
	#endif
	
	struct netif *Netif_Init_Flag;		//调用netif_add()函数时的返回值,用于判断网络初始化是否成功
	Netif_Init_Flag=netif_add(&lwip_handle,&ipaddr,&netmask,&gw,NULL,ethernetif_init,&ethernet_input);//向网卡列表中添加一个网口
	
	
	
	#if LWIP_DHCP			//如果使用DHCP的话，可以自动获取到：IP、子网掩码和网关
		dhcp_start(&lwip_handle);	//开启DHCP服务
		lwip_inf.dhcpstatus = 0;	//别动
	#endif
	
	if(Netif_Init_Flag==NULL)return 3;//网卡添加失败 
	else//网口添加成功后,设置netif为默认值,并且打开netif网口
	{
		netif_set_default(&lwip_handle); //设置netif为默认网口
		netif_set_up(&lwip_handle);		//打开netif网口
	}
	return 0;
}

//如果使能了DHCP
#if LWIP_DHCP
//DHCP处理任务
void lwip_dhcp_process_handle(void)
{
	u32 ip=0,netmask=0,gw=0;
	switch(lwip_inf.dhcpstatus)
	{
		case 0: 	//开启DHCP
			dhcp_start(&lwip_handle);
			lwip_inf.dhcpstatus = 1;		//等待通过DHCP获取到的地址
			//printf("正在查找DHCP服务器,请稍等...........\r\n");  
			break;
		case 1:		//等待获取到IP地址
		{
			static u8 try_times = 0;
			//DHCP获取到IP地址后，先给lwip_handle，读出，然后软件再给lwip_inf
			ip=lwip_handle.ip_addr.addr;		//读取新IP地址
			netmask=lwip_handle.netmask.addr;	//读取子网掩码
			gw=lwip_handle.gw.addr;				//读取默认网关
			
			if(ip!=0)			//正确获取到IP地址的时候
			{
				lwip_inf.dhcpstatus = 2;		//DHCP成功
				//printf("网卡en的MAC地址为:................%d.%d.%d.%d.%d.%d\r\n",lwip_inf.mac[0],lwip_inf.mac[1],lwip_inf.mac[2],lwip_inf.mac[3],lwip_inf.mac[4],lwip_inf.mac[5]);
				
				//解析出通过DHCP获取到的IP地址
				lwip_inf.ip[3]=(uint8_t)(ip>>24); 
				lwip_inf.ip[2]=(uint8_t)(ip>>16);
				lwip_inf.ip[1]=(uint8_t)(ip>>8);
				lwip_inf.ip[0]=(uint8_t)(ip);
				//printf("通过DHCP获取到IP地址..............%d.%d.%d.%d\r\n",lwip_inf.ip[0],lwip_inf.ip[1],lwip_inf.ip[2],lwip_inf.ip[3]);
				
				//解析通过DHCP获取到的子网掩码地址
				lwip_inf.netmask[3]=(uint8_t)(netmask>>24);
				lwip_inf.netmask[2]=(uint8_t)(netmask>>16);
				lwip_inf.netmask[1]=(uint8_t)(netmask>>8);
				lwip_inf.netmask[0]=(uint8_t)(netmask);
				//printf("通过DHCP获取到子网掩码............%d.%d.%d.%d\r\n",lwip_inf.netmask[0],lwip_inf.netmask[1],lwip_inf.netmask[2],lwip_inf.netmask[3]);
				
				//解析出通过DHCP获取到的默认网关
				lwip_inf.gateway[3]=(uint8_t)(gw>>24);
				lwip_inf.gateway[2]=(uint8_t)(gw>>16);
				lwip_inf.gateway[1]=(uint8_t)(gw>>8);
				lwip_inf.gateway[0]=(uint8_t)(gw);
				//printf("通过DHCP获取到的默认网关..........%d.%d.%d.%d\r\n",lwip_inf.gateway[0],lwip_inf.gateway[1],lwip_inf.gateway[2],lwip_inf.gateway[3]);
				
				try_times = 0;
			}
			try_times++;
			if(try_times > LWIP_MAX_DHCP_TRIES) //通过DHCP服务获取IP地址失败,且超过最大尝试次数
			{
				try_times = 0;
				lwip_inf.dhcpstatus=0XFF;//DHCP超时失败.
				//使用静态IP地址
				IP4_ADDR(&(lwip_handle.ip_addr),lwip_inf.ip[0],lwip_inf.ip[1],lwip_inf.ip[2],lwip_inf.ip[3]);
				IP4_ADDR(&(lwip_handle.netmask),lwip_inf.netmask[0],lwip_inf.netmask[1],lwip_inf.netmask[2],lwip_inf.netmask[3]);
				IP4_ADDR(&(lwip_handle.gw),lwip_inf.gateway[0],lwip_inf.gateway[1],lwip_inf.gateway[2],lwip_inf.gateway[3]);
//				printf("DHCP服务超时,使用静态IP地址!\r\n");
//				printf("网卡en的MAC地址为:................%d.%d.%d.%d.%d.%d\r\n",lwip_inf.mac[0],lwip_inf.mac[1],lwip_inf.mac[2],lwip_inf.mac[3],lwip_inf.mac[4],lwip_inf.mac[5]);
//				printf("静态IP地址........................%d.%d.%d.%d\r\n",lwip_inf.ip[0],lwip_inf.ip[1],lwip_inf.ip[2],lwip_inf.ip[3]);
//				printf("子网掩码..........................%d.%d.%d.%d\r\n",lwip_inf.netmask[0],lwip_inf.netmask[1],lwip_inf.netmask[2],lwip_inf.netmask[3]);
//				printf("默认网关..........................%d.%d.%d.%d\r\n",lwip_inf.gateway[0],lwip_inf.gateway[1],lwip_inf.gateway[2],lwip_inf.gateway[3]);
			}
		}
		break;
		default : break;
	}
}
#endif 



u32 lwip_1ms_Tick;							//lwip本地时间计数器，单位:ms
//lwip裸跑日常任务，必须放到10ms循环中
void lwip_periodic_handle(void)
{
	lwip_1ms_Tick +=10;
	
	sys_check_timeouts(); /*check timeouts,handle all timers for all protocols in the stack*/
	
	#if SYS_Test_LWIP	//如果不使用TCP/UDP通讯的话
		if(lwip_1ms_Tick % 250 == 0)
		{
			#if LWIP_TCP
				//每250ms调用一次tcp_tmr()函数
				tcp_tmr();
			#endif
		}
		
		if(lwip_1ms_Tick % 5000 == 0)
		{
			//ARP每5s周期性调用一次
			etharp_tmr();
		}
	#endif
	
	#if LWIP_DHCP //如果使用DHCP的话
		if(lwip_1ms_Tick % 500 == 0)
		{
			//每500ms调用一次dhcp_fine_tmr()
			dhcp_fine_tmr();
			if ((lwip_inf.dhcpstatus != 2)&&(lwip_inf.dhcpstatus != 0XFF))
			{ 
				lwip_dhcp_process_handle();  //DHCP处理
			}
		}
	
		if(lwip_1ms_Tick % (60*1000) == 0)
		{
			//每60s执行一次DHCP粗糙处理
			dhcp_coarse_tmr();
		}
	#endif
}

//循环扫描网卡是否有数据发来，必须放到无延迟大循环中
void lwip_pkt_handle(void)
{
	//从网络缓冲区中读取接收到的数据包并将其发送给LWIP处理 
	ethernetif_input(&lwip_handle);
}



























