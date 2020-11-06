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
#include "malloc.h"

ETH_HandleTypeDef ETH_Handler;      //以太网句柄
__align(4) ETH_DMADescTypeDef *DMARxDscrTab;	//以太网DMA接收描述符数据结构体指针
__align(4) ETH_DMADescTypeDef *DMATxDscrTab;	//以太网DMA发送描述符数据结构体指针 
__align(4) uint8_t *Rx_Buff; 					//以太网底层驱动接收buffers指针 
__align(4) uint8_t *Tx_Buff; 					//以太网底层驱动发送buffers指针



extern u8_t *ram_heap;					//LWIP的总内存堆ram_heap，在mem.c里面定义，内存池mem_pool在opt.h里面被配置为从内存堆ram_heap获取
/*LWIP源文件修改记录：
	仅仅把mem.c里面的
		349行的
			struct mem搬到了mem.h里面
		371行的
			这两个宏搬到了mem.h里面
	关于lwip的mem精彩解析：https://blog.csdn.net/ZCShouCSDN/article/details/80282907
*/

#define LWIP_MAX_DHCP_TRIES		4   //DHCP服务器最大重试次数

//用户自定：网卡的名字
#define IFNAME0 'e'
#define IFNAME1 '0'

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

//用于发送数据包的最底层函数(lwip通过netif->linkoutput指向该函数)
//netif:网卡结构体指针
//p:pbuf数据结构体指针
//返回值:ERR_OK,发送正常
//       ERR_MEM,发送失败
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
    err_t errval;
    struct pbuf *q;
    uint8_t *buffer=(uint8_t *)(ETH_Handler.TxDesc->Buffer1Addr);
    __IO ETH_DMADescTypeDef *DmaTxDesc;
    uint32_t framelength = 0;
    uint32_t bufferoffset = 0;
    uint32_t byteslefttocopy = 0;
    uint32_t payloadoffset = 0;

    DmaTxDesc = ETH_Handler.TxDesc;
    bufferoffset = 0;

    //从pbuf中拷贝要发送的数据
    for(q=p;q!=NULL;q=q->next)
    {
        //判断此发送描述符是否有效，即判断此发送描述符是否归以太网DMA所有
        if((DmaTxDesc->Status&ETH_DMATXDESC_OWN)!=(uint32_t)RESET)
        {
            errval=ERR_USE;
            goto error;             //发送描述符无效，不可用
        }
        byteslefttocopy=q->len;     //要发送的数据长度
        payloadoffset=0;
   
        //将pbuf中要发送的数据写入到以太网发送描述符中，有时候我们要发送的数据可能大于一个以太网
        //描述符的Tx Buffer，因此我们需要分多次将数据拷贝到多个发送描述符中
        while((byteslefttocopy+bufferoffset)>ETH_TX_BUF_SIZE )
        {
            //将数据拷贝到以太网发送描述符的Tx Buffer中
            mymemcpy((uint8_t*)((uint8_t*)buffer+bufferoffset),(uint8_t*)((uint8_t*)q->payload+payloadoffset),(ETH_TX_BUF_SIZE-bufferoffset));
            //DmaTxDsc指向下一个发送描述符
            DmaTxDesc=(ETH_DMADescTypeDef *)(DmaTxDesc->Buffer2NextDescAddr);
            //检查新的发送描述符是否有效
            if((DmaTxDesc->Status&ETH_DMATXDESC_OWN)!=(uint32_t)RESET)
            {
                errval = ERR_USE;
                goto error;     //发送描述符无效，不可用
            }
            buffer=(uint8_t *)(DmaTxDesc->Buffer1Addr);   //更新buffer地址，指向新的发送描述符的Tx Buffer
            byteslefttocopy=byteslefttocopy-(ETH_TX_BUF_SIZE-bufferoffset);
            payloadoffset=payloadoffset+(ETH_TX_BUF_SIZE-bufferoffset);
            framelength=framelength+(ETH_TX_BUF_SIZE-bufferoffset);
            bufferoffset=0;
        }
        //拷贝剩余的数据
        mymemcpy( (uint8_t*)((uint8_t*)buffer+bufferoffset),(uint8_t*)((uint8_t*)q->payload+payloadoffset),byteslefttocopy );
        bufferoffset=bufferoffset+byteslefttocopy;
        framelength=framelength+byteslefttocopy;
    }
    //当所有要发送的数据都放进发送描述符的Tx Buffer以后就可发送此帧了
    HAL_ETH_TransmitFrame(&ETH_Handler,framelength);
    errval = ERR_OK;
error:    
    //发送缓冲区发生下溢，一旦发送缓冲区发生下溢TxDMA会进入挂起状态
    if((ETH_Handler.Instance->DMASR&ETH_DMASR_TUS)!=(uint32_t)RESET)
    {
        //清除下溢标志
        ETH_Handler.Instance->DMASR = ETH_DMASR_TUS;
        //当发送帧中出现下溢错误的时候TxDMA会挂起，这时候需要向DMATPDR寄存器
        //随便写入一个值来将其唤醒，此处我们写0
        ETH_Handler.Instance->DMATPDR=0;
    }
    return errval;
}  
///用于接收数据包的最底层函数
//neitif:网卡结构体指针
//返回值:pbuf数据结构体指针
static struct pbuf * low_level_input(struct netif *netif)
{  
	struct pbuf *p = NULL;
    struct pbuf *q;
    uint16_t len;
    uint8_t *buffer;
    __IO ETH_DMADescTypeDef *dmarxdesc;
    uint32_t bufferoffset=0;
    uint32_t payloadoffset=0;
    uint32_t byteslefttocopy=0;
    uint32_t i=0;
  
    if(HAL_ETH_GetReceivedFrame(&ETH_Handler)!=HAL_OK)  //判断是否接收到数据
    return NULL;
    
    len=ETH_Handler.RxFrameInfos.length;                //获取接收到的以太网帧长度
    buffer=(uint8_t *)ETH_Handler.RxFrameInfos.buffer;  //获取接收到的以太网帧的数据buffer
  
    if(len>0) p=pbuf_alloc(PBUF_RAW,len,PBUF_POOL);     //申请pbuf
    if(p!=NULL)                                        //pbuf申请成功
    {
        dmarxdesc=ETH_Handler.RxFrameInfos.FSRxDesc;    //获取接收描述符链表中的第一个描述符
        bufferoffset = 0;
        for(q=p;q!=NULL;q=q->next)                      
        {
            byteslefttocopy=q->len;                  
            payloadoffset=0;
            //将接收描述符中Rx Buffer的数据拷贝到pbuf中
            while((byteslefttocopy+bufferoffset)>ETH_RX_BUF_SIZE )
            {
                //将数据拷贝到pbuf中
                mymemcpy((uint8_t*)((uint8_t*)q->payload+payloadoffset),(uint8_t*)((uint8_t*)buffer+bufferoffset),(ETH_RX_BUF_SIZE-bufferoffset));
                 //dmarxdesc向下一个接收描述符
                dmarxdesc=(ETH_DMADescTypeDef *)(dmarxdesc->Buffer2NextDescAddr);
                //更新buffer地址，指向新的接收描述符的Rx Buffer
                buffer=(uint8_t *)(dmarxdesc->Buffer1Addr);
 
                byteslefttocopy=byteslefttocopy-(ETH_RX_BUF_SIZE-bufferoffset);
                payloadoffset=payloadoffset+(ETH_RX_BUF_SIZE-bufferoffset);
                bufferoffset=0;
            }
            //拷贝剩余的数据
            mymemcpy((uint8_t*)((uint8_t*)q->payload+payloadoffset),(uint8_t*)((uint8_t*)buffer+bufferoffset),byteslefttocopy);
            bufferoffset=bufferoffset+byteslefttocopy;
        }
    }    
    //释放DMA描述符
    dmarxdesc=ETH_Handler.RxFrameInfos.FSRxDesc;
    for(i=0;i<ETH_Handler.RxFrameInfos.SegCount; i++)
    {  
        dmarxdesc->Status|=ETH_DMARXDESC_OWN;       //标记描述符归DMA所有
        dmarxdesc=(ETH_DMADescTypeDef *)(dmarxdesc->Buffer2NextDescAddr);
    }
    ETH_Handler.RxFrameInfos.SegCount =0;           //清除段计数器
    if((ETH_Handler.Instance->DMASR&ETH_DMASR_RBUS)!=(uint32_t)RESET)  //接收缓冲区不可用
    {
        //清除接收缓冲区不可用标志
        ETH_Handler.Instance->DMASR = ETH_DMASR_RBUS;
        //当接收缓冲区不可用的时候RxDMA会进去挂起状态，通过向DMARPDR写入任意一个值来唤醒Rx DMA
        ETH_Handler.Instance->DMARPDR=0;
    }
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
	
	#if LWIP_NETIF_HOSTNAME					//LWIP_NETIF_HOSTNAME 
		netif->hostname="lwip";  			//初始化名称
	#endif 
	
	netif->name[0] = IFNAME0; 				//初始化变量netif的name字段
	netif->name[1] = IFNAME1;
	
	netif->hwaddr_len = ETHARP_HWADDR_LEN; //设置MAC地址长度,为6个字节
	
	netif->flags = NETIF_FLAG_BROADCAST|NETIF_FLAG_ETHARP|NETIF_FLAG_LINK_UP;
	
											//给lwip_handle初始化MAC地址
	netif->hwaddr[0]=lwip_inf.mac[0]; netif->hwaddr[1]=lwip_inf.mac[1]; netif->hwaddr[2]=lwip_inf.mac[2];
	netif->hwaddr[3]=lwip_inf.mac[3]; netif->hwaddr[4]=lwip_inf.mac[4]; netif->hwaddr[5]=lwip_inf.mac[5];
	
	netif->mtu=1500; 						//最大允许传输单元,允许该网卡广播和ARP功能
	
	netif->output = etharp_output;			//IP层发送数据包函数
	
	netif->linkoutput = low_level_output;	//ARP模块发送数据包函数
	
	HAL_ETH_DMATxDescListInit(&ETH_Handler,DMATxDscrTab,Tx_Buff,ETH_TXBUFNB);//初始化发送描述符
	HAL_ETH_DMARxDescListInit(&ETH_Handler,DMARxDscrTab,Rx_Buff,ETH_RXBUFNB);//初始化接收描述符
	HAL_ETH_Start(&ETH_Handler); 			//开启MAC和DMA
	
	return ERR_OK;
}



//lwip中内存堆ram_heap内存释放
void lwip_comm_mem_free(void)
{ 	
	myfree(InrRAM,ram_heap);
}

//lwip中内存堆ram_heap内存申请
//返回值:0,成功;
//    其他,失败
u8 lwip_comm_mem_malloc(void)
{
	u32 ram_heap_size; 
	
	ram_heap_size = MEM_SIZE_ALIGNED + (2 * SIZEOF_STRUCT_MEM);
	ram_heap = mymalloc(InrRAM,ram_heap_size);	//为ram_heap申请内存 
	
	if(!(u32)&ram_heap)//有申请失败的
	{
		lwip_comm_mem_free();
		return 1;
	}
	return 0;	
}

//释放ETH 底层驱动申请的内存
void ETH_Mem_Free(void)
{ 
	myfree(InrRAM,DMARxDscrTab);//释放内存
	myfree(InrRAM,DMATxDscrTab);//释放内存
	myfree(InrRAM,Rx_Buff);		//释放内存
	myfree(InrRAM,Tx_Buff);		//释放内存  
}

//为ETH底层驱动申请内存
//返回值:0,正常
//    其他,失败
u8 ETH_Mem_Malloc(void)
{ 
	DMARxDscrTab=mymalloc(InrRAM,ETH_RXBUFNB*sizeof(ETH_DMADescTypeDef));//申请内存
	DMATxDscrTab=mymalloc(InrRAM,ETH_TXBUFNB*sizeof(ETH_DMADescTypeDef));//申请内存  
	Rx_Buff=mymalloc(InrRAM,ETH_RX_BUF_SIZE*ETH_RXBUFNB);	//申请内存
	Tx_Buff=mymalloc(InrRAM,ETH_TX_BUF_SIZE*ETH_TXBUFNB);	//申请内存
	if(!(u32)&DMARxDscrTab||!(u32)&DMATxDscrTab||!(u32)&Rx_Buff||!(u32)&Tx_Buff)
	{
		ETH_Mem_Free();
		return 1;	//申请失败
	}	
	return 0;		//申请成功
}


struct netif lwip_handle;					//定义一个全局的网络接口句柄
//LWIP初始化(LWIP启动的时候使用)
//返回值:0,成功
//      1,内存错误
//      2,网卡初始化失败
//      3,网卡添加失败
//单独调用 lwip_comm_init(&lwip_handle);
u8 lwip_comm_init(struct netif *netif)
{
	//为lwip的memp和mem申请内存，默认让lwip自己管理，所以无需申请
	
	//TODO：ETH的内存的申请和释放试试能不能换成CCM，这样内部SRAM就省了
	if(ETH_Mem_Malloc())return 1;		//内存申请失败
	if(lwip_comm_mem_malloc())return 1;	//内存申请失败
	
	
	//补一下MAC信息给lwip_inf，现在lwip_inf的信息全了
	//MAC地址信息设置：高三字节固定为:2.0.0,低三字节用STM32唯一ID
	u32 sn0 = *(vu32*)(0x1FFF7A10);		//获取STM32的唯一ID的前24位作为MAC地址后三字节
	lwip_inf.mac[0]=2;//高三字节(IEEE称之为组织唯一ID,OUI)地址固定为:2.0.0
	lwip_inf.mac[1]=0;
	lwip_inf.mac[2]=0;
	lwip_inf.mac[3]=(sn0>>16)&0XFF;//低三字节用STM32的唯一ID
	lwip_inf.mac[4]=(sn0>>8)&0XFFF;
	lwip_inf.mac[5]=sn0&0XFF; 
	
	
	u8 retry = 0;
    while(1)		        //初始化NetCard_PHY,如果失败的话就重试5次
    {
		NetCard_PHY_RST=0;					//硬件复位NetCard_PHY
		HAL_Delay(50);	
		NetCard_PHY_RST=1;				 	//复位结束 
		
		u8 macaddress[6];
		macaddress[0]=lwip_inf.mac[0];macaddress[1]=lwip_inf.mac[1];macaddress[2]=lwip_inf.mac[2];
		macaddress[3]=lwip_inf.mac[3];macaddress[4]=lwip_inf.mac[4];macaddress[5]=lwip_inf.mac[5];
		
		ETH_Handler.Instance=ETH;
		ETH_Handler.Init.AutoNegotiation=ETH_AUTONEGOTIATION_ENABLE;//使能自协商模式 
		ETH_Handler.Init.Speed=ETH_SPEED_100M;						//速度100M,如果开启了自协商模式，此配置就无效
		ETH_Handler.Init.DuplexMode=ETH_MODE_FULLDUPLEX;			//全双工模式，如果开启了自协商模式，此配置就无效
		ETH_Handler.Init.PhyAddress=NetCard_PHY_ADDRESS;			//NetCard_PHY地址  
		ETH_Handler.Init.MACAddr=macaddress;            			//MAC地址  
		ETH_Handler.Init.RxMode=ETH_RXINTERRUPT_MODE;   			//中断接收模式 
		ETH_Handler.Init.ChecksumMode=ETH_CHECKSUM_BY_HARDWARE;		//硬件帧校验  
		ETH_Handler.Init.MediaInterface=ETH_MEDIA_INTERFACE_RMII;	//RMII接口  
		if(HAL_ETH_Init(&ETH_Handler)==HAL_OK)
		{
			break;   //成功
		}
        
		retry++;
        if(retry>5) {retry=0;return 2;} //初始化失败
    }
	
	lwip_init();			//初始化LWIP内核

	#if LWIP_DHCP			//使用动态IP
		ip4_addr_t ipaddr;  //ip地址
		ip4_addr_t netmask; //子网掩码
		ip4_addr_t gw;      //默认网关 
		
		ipaddr.addr = 0;
		netmask.addr = 0;
		gw.addr = 0;
	#else								//使用静态IP
		IP4_ADDR(&ipaddr,lwip_inf.ip[0],lwip_inf.ip[1],lwip_inf.ip[2],lwip_inf.ip[3]);
		IP4_ADDR(&netmask,lwip_inf.netmask[0],lwip_inf.netmask[1] ,lwip_inf.netmask[2],lwip_inf.netmask[3]);
		IP4_ADDR(&gw,lwip_inf.gateway[0],lwip_inf.gateway[1],lwip_inf.gateway[2],lwip_inf.gateway[3]);
	#endif
	
	struct netif *Netif_Init_Flag;		//调用netif_add()函数时的返回值,用于判断网络初始化是否成功
	Netif_Init_Flag = netif_add(netif,&ipaddr,&netmask,&gw,NULL,&ethernetif_init,&ethernet_input);//向网卡列表中添加一个网口
	
	if(Netif_Init_Flag==NULL)return 3;	//网卡添加失败 
	
	//网口添加成功后,设置netif为默认值,并且打开netif网口
	netif_set_default(netif); 	//设置netif为默认网口
	netif_set_link_up(netif); 	//打开接口连接，不管接口是否已经连上，调用一下对DHCP等有好处
	netif_set_up(netif);		//打开netif网口
	
	netif_set_remove_callback(netif,lwip_comm_destroy); //物理连线掉线后调用lwip_comm_destroy关闭此次LWIP连接
	
	#if LWIP_DHCP				//如果使用DHCP的话，可以自动获取到：IP、子网掩码和网关
		dhcp_start(netif);		//开启DHCP服务
		lwip_inf.dhcpstatus = 0;//别动
	#endif
	
	return 0;
}

//关闭LWIP,并释放内存
//退出LWIP时使用
//掉线时会自动调用，或者单独调用 lwip_comm_destroy(&lwip_handle);
void lwip_comm_destroy(struct netif *netif)
{
	#if LWIP_DHCP
		dhcp_stop(netif); 			//关闭DHCP
	#endif
	netif_set_link_down(netif);		//连接强制下线
	netif_remove(netif);  			//删除lwip_netif网卡
	lwip_comm_mem_free();			//释放内存
 	ETH_Mem_Free();					//释放内存
	HAL_ETH_DeInit(&ETH_Handler);  	//复位以太网外设
}

//ETH底层驱动，时钟使能，引脚配置
//此函数会被HAL_ETH_Init()调用
//heth:以太网句柄
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_ETH_CLK_ENABLE();             //开启ETH时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
	__HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();			//开启GPIOC时钟
	__HAL_RCC_GPIOD_CLK_ENABLE();           //开启GPIOD时钟
    __HAL_RCC_GPIOG_CLK_ENABLE();			//开启GPIOG时钟
    
    /*网络引脚设置 RMII接口 
    ETH_MDIO -------------------------> PA2
    ETH_MDC --------------------------> PC1
    ETH_RMII_REF_CLK------------------> PA1
    ETH_RMII_CRS_DV ------------------> PA7
    ETH_RMII_RXD0 --------------------> PC4
    ETH_RMII_RXD1 --------------------> PC5
    ETH_RMII_TX_EN -------------------> PG11
    ETH_RMII_TXD0 --------------------> PG13
    ETH_RMII_TXD1 --------------------> PG14
    ETH_RESET-------------------------> PD3*/
    
    //PA1,2,7
    GPIO_Initure.Pin=GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_7; 
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //推挽复用
    GPIO_Initure.Pull=GPIO_NOPULL;              //不带上下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;         //高速
    GPIO_Initure.Alternate=GPIO_AF11_ETH;       //复用为ETH功能
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);         //初始化
    
    //PC1,4,5
    GPIO_Initure.Pin=GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5; //PC1,4,5
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);         //初始化
	
    //PG11,13,14
    GPIO_Initure.Pin=GPIO_PIN_11|GPIO_PIN_13|GPIO_PIN_14;   //PG11,PG13,14
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);         //初始化
	
	//配置PD3为推完输出,NetCard_PHY复位引脚
    GPIO_Initure.Pin=GPIO_PIN_3; 				//PD3
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
    GPIO_Initure.Pull=GPIO_NOPULL;          	//无上下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     	//高速
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);
    
    HAL_NVIC_SetPriority(ETH_IRQn,1,0);         //网络中断优先级应该高一点
    HAL_NVIC_EnableIRQ(ETH_IRQn);
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



//获取接收到的帧长度
//DMARxDesc:接收DMA描述符
//返回值:接收到的帧长度
u32  ETH_GetRxPktSize(ETH_DMADescTypeDef *DMARxDesc)
{
    u32 frameLength = 0;
    if(((DMARxDesc->Status&ETH_DMARXDESC_OWN)==(uint32_t)RESET) &&
     ((DMARxDesc->Status&ETH_DMARXDESC_ES)==(uint32_t)RESET) &&
     ((DMARxDesc->Status&ETH_DMARXDESC_LS)!=(uint32_t)RESET)) 
    {
        frameLength=((DMARxDesc->Status&ETH_DMARXDESC_FL)>>ETH_DMARXDESC_FRAME_LENGTHSHIFT);
    }
    return frameLength;
}

//中断服务函数
void ETH_IRQHandler(void)
{
    while(ETH_GetRxPktSize(ETH_Handler.RxDesc))   
    {
        lwip_pkt_handle();//处理以太网数据，即将数据提交给LWIP
    }
    //清除中断标志位
    __HAL_ETH_DMA_CLEAR_IT(&ETH_Handler,ETH_DMA_IT_NIS);    //清除DMA中断标志位
    __HAL_ETH_DMA_CLEAR_IT(&ETH_Handler,ETH_DMA_IT_R);      //清除DMA接收中断标志位
}




