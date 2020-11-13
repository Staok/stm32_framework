#include "malloc.h"

//内存池(32字节对齐)
__align(32) u8 mem1base[MEM1_MAX_SIZE];														//内部SRAM内存池
//__align(32) u8 mem2base[MEM2_MAX_SIZE] __attribute__((at(0X68000000)));					//外部SRAM内存池
//__align(32) u8 mem3base[MEM3_MAX_SIZE] __attribute__((at(0X10000000)));					//内部CCM内存池
//内存管理表
u16 mem1mapbase[MEM1_ALLOC_TABLE_SIZE];													//内部SRAM内存池MAP
//u16 mem2mapbase[MEM2_ALLOC_TABLE_SIZE] __attribute__((at(0X68000000+MEM2_MAX_SIZE)));	//外部SRAM内存池MAP
//u16 mem3mapbase[MEM3_ALLOC_TABLE_SIZE] __attribute__((at(0X10000000+MEM3_MAX_SIZE)));	//内部CCM内存池MAP
//内存管理参数	   
//const u32 memtblsize[SRAMBANK]={MEM1_ALLOC_TABLE_SIZE,MEM2_ALLOC_TABLE_SIZE,MEM3_ALLOC_TABLE_SIZE};	//内存表大小
//const u32 memblksize[SRAMBANK]={MEM1_BLOCK_SIZE,MEM2_BLOCK_SIZE,MEM3_BLOCK_SIZE};					//内存分块大小
//const u32 memsize[SRAMBANK]={MEM1_MAX_SIZE,MEM2_MAX_SIZE,MEM3_MAX_SIZE};							//内存总大小

const u32 memtblsize[SRAMBANK]={MEM1_ALLOC_TABLE_SIZE};				//内存表大小
const u32 memblksize[SRAMBANK]={MEM1_BLOCK_SIZE};					//内存分块大小
const u32 memsize[SRAMBANK]={MEM1_MAX_SIZE};						//内存总大小

//内存管理控制器
//struct malloc_cortol_struct malloc_cortol=
//{
//	my_mem_init,						//内存初始化
//	my_mem_perused,						//内存使用率
//	mem1base,mem2base,mem3base,			//内存池
//	mem1mapbase,mem2mapbase,mem3mapbase,//内存管理状态表
//	0,0,0,  		 					//内存管理未就绪
//};

struct malloc_cortol_struct malloc_cortol=
{
	my_mem_init,						//内存初始化
	my_mem_perused,						//内存使用率
	mem1base,							//内存池
	mem1mapbase,						//内存管理状态表
	0,  		 						//内存管理未就绪
};

//复制内存
//*des:目的地址
//*src:源地址
//len:需要复制的内存长度(字节为单位)
void my_mem_cpy(void *des,void *src,u32 len)  
{  
    u8 *xdes=des;
	u8 *xsrc=src; 
    while(len--)*xdes++=*xsrc++;  
}  

//设置内存
//*s:内存首地址
//c :要设置的值
//num:需要设置的内存大小(字节为单位)
void my_mem_set(void *s,u8 c,u32 num)  
{  
    u8 *xs = s;  
    while(num--)*xs++=c;  
}	   

//内存管理初始化  
//memx:所属内存块
void my_mem_init(u8 memx)  
{  
    my_mem_set(malloc_cortol.memmap[memx], 0,memtblsize[memx]*2);//内存状态表数据清零  
	my_mem_set(malloc_cortol.membase[memx], 0,memsize[memx]);	//内存池所有数据清零  
	malloc_cortol.memrdy[memx]=1;								//内存管理初始化OK  
}

//获取内存使用率
//memx:所属内存块
//返回值:使用率(0~100)
u8 my_mem_perused(u8 memx)  
{  
    u32 used=0;  
    u32 i;  
    for(i=0;i<memtblsize[memx];i++)  
    {  
        if(malloc_cortol.memmap[memx][i])used++; 
    } 
    return (used*100)/(memtblsize[memx]);  
}

//内存分配(内部调用)
//memx:所属内存块
//size:要分配的内存大小(字节)
//返回值:0XFFFFFFFF,代表错误;其他,内存偏移地址 
u32 my_mem_malloc(u8 memx,u32 size)  
{  
    signed long offset=0;  
    u32 xmemb;	//需要的内存块数  
	u32 kmemb=0;//连续空内存块数
    u32 i;  
    if(!malloc_cortol.memrdy[memx])malloc_cortol.init(memx);//未初始化,先执行初始化 
    if(size==0)return 0XFFFFFFFF;//不需要分配
    xmemb=size/memblksize[memx];  	//获取需要分配的连续内存块数
    if(size%memblksize[memx])xmemb++;  
    for(offset=memtblsize[memx]-1;offset>=0;offset--)//搜索整个内存控制区  
    {     
		if(!malloc_cortol.memmap[memx][offset])kmemb++;//连续空内存块数增加
		else kmemb=0;								//连续内存块清零
		if(kmemb==xmemb)							//找到了连续nmemb个空内存块
		{
            for(i=0;i<xmemb;i++)  					//标注内存块非空 
            {  
                malloc_cortol.memmap[memx][offset+i]=xmemb;  
            }  
            return (offset*memblksize[memx]);//返回偏移地址  
		}
    }  
    return 0XFFFFFFFF;//未找到符合分配条件的内存块  
}  

//释放内存(内部调用) 
//memx:所属内存块
//offset:内存地址偏移
//返回值:0,释放成功;1,释放失败;  
u8 my_mem_free(u8 memx,u32 offset)  
{  
    int i;  
    if(!malloc_cortol.memrdy[memx])//未初始化,先执行初始化
	{
		malloc_cortol.init(memx);    
        return 1;//未初始化  
    }  
    if(offset<memsize[memx])//偏移在内存池内. 
    {  
        int index=offset/memblksize[memx];			//偏移所在内存块号码  
        int nmemb=malloc_cortol.memmap[memx][index];	//内存块数量
        for(i=0;i<nmemb;i++)  						//内存块清零
        {  
            malloc_cortol.memmap[memx][index+i]=0;  
        }  
        return 0;  
    }else return 2;//偏移超区了.  
}  

//释放内存(外部调用) 
//memx:所属内存块
//ptr:内存首地址 
void myfree(u8 memx,void *paddr)  
{  
	u32 offset;   
	if(paddr==NULL)return;//地址为0.  
 	offset=(u32)paddr-(u32)malloc_cortol.membase[memx];     
    my_mem_free(memx,offset);	//释放内存      
}  

//分配内存(外部调用)
//memx:所属内存块
//size:内存大小(字节)
//返回值:分配到的内存首地址.
void *mymalloc(u8 memx,u32 size)  
{  
    u32 offset;   
	offset=my_mem_malloc(memx,size);  	   	 	   
    if(offset==0XFFFFFFFF)return NULL;  
    else return (void*)((u32)malloc_cortol.membase[memx]+offset);  
}  

//重新分配内存(外部调用)
//memx:所属内存块
//*ptr:旧内存首地址
//size:要分配的内存大小(字节)
//返回值:新分配到的内存首地址.
void *myrealloc(u8 memx,void *paddr,u32 size)  
{  
    u32 offset;    
    offset=my_mem_malloc(memx,size);   	
    if(offset==0XFFFFFFFF)return NULL;     
    else  
    {  									   
	    my_mem_cpy((void*)((u32)malloc_cortol.membase[memx]+offset),paddr,size);	//拷贝旧内存内容到新内存   
        myfree(memx,paddr);  											  		//释放旧内存
        return (void*)((u32)malloc_cortol.membase[memx]+offset);  				//返回新内存首地址
    }  
}








