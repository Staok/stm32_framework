#include "NetCard.h"

#if	SYS_SUPPORT_LWIP

//NetCard_PHY初始化
//返回值:0,成功;
//    其他,失败
u8 NetCard_PHY_Init(void)
{
	return 0;
}

extern ETH_HandleTypeDef ETH_Handler;
//读取PHY寄存器值
u32 NetCard_PHY_ReadPHY(u16 reg)
{
    u32 regval;
    HAL_ETH_ReadPHYRegister(&ETH_Handler,reg,&regval);
    return regval;
}
//向NetCard_PHY指定寄存器写入值
//reg:要写入的寄存器
//value:要写入的值
void NetCard_PHY_WritePHY(u16 reg,u16 value)
{
    u32 temp=value;
    HAL_ETH_ReadPHYRegister(&ETH_Handler,reg,&temp);
}
//得到8720的速度模式
//返回值:
//001:10M半双工
//101:10M全双工
//010:100M半双工
//110:100M全双工
//其他:错误
u8 NetCard_PHY_Get_Speed(void)
{
	u8 speed;
	speed=((NetCard_PHY_ReadPHY(31)&0x1C)>>2); //从LAN8720的31号寄存器中读取网络速度和双工模式
	return speed;
}

#endif







