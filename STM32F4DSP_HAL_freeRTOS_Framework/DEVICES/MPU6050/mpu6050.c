#include "mpu6050.h"
#include "filter.h"

/**
  * 函数功能: 写数据到MPU6050寄存器
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */ 
void MPU6050_WriteReg(uint8_t reg_add,uint8_t reg_dat)
{
	SimuI2C_Start(&SimuI2C_Handle);
	
	SimuI2C_WriteByte(&SimuI2C_Handle,MPU6050_SLAVE_ADDRESS);
	SimuI2C_WaitAck(&SimuI2C_Handle);
	
	SimuI2C_WriteByte(&SimuI2C_Handle,reg_add);
	SimuI2C_WaitAck(&SimuI2C_Handle);
	
	SimuI2C_WriteByte(&SimuI2C_Handle,reg_dat);
	SimuI2C_WaitAck(&SimuI2C_Handle);
	
	SimuI2C_Stop(&SimuI2C_Handle);
}

/**
  * 函数功能: 从MPU6050寄存器读取数据
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */ 
void MPU6050_ReadData(uint8_t reg_add,unsigned char *Read,uint8_t num)
{
	unsigned char i;
	
	SimuI2C_Start(&SimuI2C_Handle);
	
	SimuI2C_WriteByte(&SimuI2C_Handle,MPU6050_SLAVE_ADDRESS);
	SimuI2C_WaitAck(&SimuI2C_Handle);
	
	SimuI2C_WriteByte(&SimuI2C_Handle,reg_add);
	SimuI2C_WaitAck(&SimuI2C_Handle);
	
	SimuI2C_Start(&SimuI2C_Handle);
	
	SimuI2C_WriteByte(&SimuI2C_Handle,MPU6050_SLAVE_ADDRESS + 1);
	SimuI2C_WaitAck(&SimuI2C_Handle);
	
	for(i=0;i<(num-1);i++){
		*Read=SimuI2C_ReadByte(&SimuI2C_Handle,1);
		Read++;
	}
	*Read=SimuI2C_ReadByte(&SimuI2C_Handle,0);
	SimuI2C_Stop(&SimuI2C_Handle);
}

unsigned char MPU6050_ID;
/**
  * 函数功能: 初始化MPU6050芯片
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */ 
void MPU6050_Init(void)
{
	int i=0,j=0;
	//在初始化之前要延时一段时间，若没有延时，则断电后再上电数据可能会出错
	HAL_Delay(10);
	
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	    //解除休眠状态
	MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);	    //陀螺仪采样率，1KHz
	MPU6050_WriteReg(MPU6050_RA_CONFIG , 0x06);	        //低通滤波器的设置，截止频率是1K，带宽是5K
	MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x00);	//配置加速度传感器工作在2G模式，不自检
	MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
	
	MPU6050_ReadData(MPU6050_RA_WHO_AM_I,&MPU6050_ID,1);    //读器件地址
	if(MPU6050_ID != 0x68)
			FaultASSERT("MPU6050 dectected error!\r\n");
	else 	return;
}

/**
  * 函数功能: 读取MPU6050的加速度数据
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */ 
void MPU6050ReadAcc(short *accData)
{
    uint8_t buf[6];
    MPU6050_ReadData(MPU6050_ACC_OUT, buf, 6);
    accData[0] = (buf[0] << 8) | buf[1];
    accData[1] = (buf[2] << 8) | buf[3];
    accData[2] = (buf[4] << 8) | buf[5];
}

/**
  * 函数功能: 读取MPU6050的角速度数据
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */ 
void MPU6050ReadGyro(short *gyroData)
{
    uint8_t buf[6];
    MPU6050_ReadData(MPU6050_GYRO_OUT,buf,6);
    gyroData[0] = (buf[0] << 8) | buf[1];
    gyroData[1] = (buf[2] << 8) | buf[3];
    gyroData[2] = (buf[4] << 8) | buf[5];
}

/**
  * 函数功能: 读取MPU6050的温度数据，转化成摄氏度
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */ 
void MPU6050_ReadTemp(short*Temperature)
{
	short temp3;
	uint8_t buf[2];
	
	MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //读取温度值
  temp3= (buf[0] << 8) | buf[1];
	*Temperature=(((double) (temp3 + 13200)) / 280)-13;
}


short MPU6050_Gyro[3];	//从MPU6050读出的陀螺仪原始数据
short MPU6050_Accel[3];	//从MPU6050读出的加速度计原始数据

float MPU6050_GyroF[3];	//从MPU6050读出的陀螺仪转换为单位g的数值
float MPU6050_AccelF[3];	//从MPU6050读出的加速度计转换为单位度/ms的数值

float MPU6050_Angle[3];			//计算得到的三轴角度
float MPU6050_AngleSpeed[3];	//计算得到的三轴角速度（对于非卡尔曼滤波，为直接用陀螺仪数据经过滤波的值）


/*定义使用哪一种互补滤波算法：
	1：Kalman_Filter
	2：first_order_filter_for_mpu
	3：QingHua_AngleCal
*/
u8 MPU6050_which_filter = 1;

#include "pid.h" /*使用一阶滤波*/
#include "sysVar.h" /*使用M_PI值*/
/**
  * 函数功能: 计算得到的三轴角度和角速度
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 这个函数必须10ms循环调用；加速度单位为g，陀螺仪单位为度/ms（试试，如果ms不行就换s，相应的清华滤波的Td改成1000）
  */ 
void MPU6050_GetAngle(float* Angle[3],float* AngleSpeed[3])
{
	float angleAnddot[3][2],buf[3][2];
	u8 i;
	
	/*读出原始数据*/
	MPU6050ReadAcc(MPU6050_Accel);
	MPU6050ReadGyro(MPU6050_Gyro);
	
	/*限幅*/
	for(i = 0;i < 3;i++)
	{
		if(MPU6050_Gyro[i] > )
		{
			
		}
		
		if(MPU6050_Accel[i] > )
		{
			
		}
	}
	
	/*把加速度计的值转为与陀螺仪同位置的角度值*/
	MPU6050_GyroF[0] = ((float)atan2(MPU6050_Gyro[1],MPU6050_Gyro[2]))*180.0f/M_PI;	//tan(θx) = tan(θyz) = Ry/Rz
	MPU6050_GyroF[1] = ((float)atan2(MPU6050_Gyro[0],MPU6050_Gyro[2]))*180.0f/M_PI;	//tan(θy) = tan(θxz) = Rx/Rz
	MPU6050_GyroF[2] = ((float)atan2(MPU6050_Gyro[0],MPU6050_Gyro[1]))*180.0f/M_PI;	//tan(θz) = tan(θxy) = Rx/Ry
	
	
	/*单位转换，加速度单位为g，陀螺仪单位为度/ms*/
	for(i = 0;i < 3;i++)
	{
		MPU6050_GyroF[i] = 		MPU6050_Gyro[i];
		MPU6050_AccelF[i] = 	MPU6050_Accel[i];
	}
	
	
	switch(MPU6050_which_filter)
	{
		case 1:
			for(i = 0;i < 3;i++)
			{	
				Kalman_Filter((float)MPU6050_Accel[i], (float)MPU6050_Gyro[i], MPU6050_Angle[i],angleAnddot[i]);
				MPU6050_Angle[i] = 		angleAnddot[i][0];
				MPU6050_AngleSpeed[i] = angleAnddot[i][1];
			}
			break;
		case 2:
			for(i = 0;i < 3;i++)
			{	
				buf[i][0] = (float)MPU6050_Accel[i]; //先转为float类型
				buf[i][1] = (float)MPU6050_Gyro[i];
				MPU6050_Angle[i] = first_order_filter_for_mpu(&buf[i][0], &buf[i][1],&MPU6050_Angle[i]);
				MPU6050_AngleSpeed[i] = FirstOrderLPF(MPU6050_GyroF[i]); //对于非卡尔曼滤波，为直接用陀螺仪数据经过滤波的值
			}
			break;
		case 3:
			for(i = 0;i < 3;i++)
			{	
				buf[i][0] = (float)MPU6050_Accel[i]; //先转为float类型
				buf[i][1] = (float)MPU6050_Gyro[i];
				MPU6050_Angle[i] = QingHua_AngleCal(&buf[i][0], &buf[i][1],&MPU6050_Angle[i]);
				MPU6050_AngleSpeed[i] = FirstOrderLPF(MPU6050_GyroF[i]); //对于非卡尔曼滤波，为直接用陀螺仪数据经过滤波的值
			}
			break;
		default:
			break;
	}
}



