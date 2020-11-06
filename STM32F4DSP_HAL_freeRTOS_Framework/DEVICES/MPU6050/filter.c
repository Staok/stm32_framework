#include "filter.h"	

/**************************************************************************
函数功能：简易卡尔曼滤波
入口参数：加速度、角速度和上次计算出的角度 接着计算结果的angleAnddot，angleAnddot[0]为本次计算得到的角度，angleAnddot[1]为本次计算得到的角速度
返回  值：无
**************************************************************************/
void Kalman_Filter(float Accel,float Gyro, float angle, float angleAnddot[2])		
{
	float Q_angle=0.001f;// 过程噪声的协方差
	float Q_gyro=0.003f;//0.03 过程噪声的协方差 过程噪声的协方差为一个一行两列矩阵
	float R_angle=0.5f;// 测量噪声的协方差 既测量偏差
	float dt=0.005f;//                 
	float  C_0 = 1.0f;
	float Q_bias, Angle_err;
	float PCt_0, PCt_1, E;
	float K_0, K_1, t_0, t_1;
	float Pdot[4] ={0.0f,0.0f,0.0f,0.0f};
	float PP[2][2] = { { 1.0f, 0.0f },{ 0.0f, 1.0f } };
	
	angle+=(Gyro - Q_bias) * dt; //先验估计
	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分

	Pdot[1]=-PP[1][1];
	Pdot[2]=-PP[1][1];
	Pdot[3]=Q_gyro;
	PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分
	PP[0][1] += Pdot[1] * dt;   // =先验估计误差协方差
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;
	
	Angle_err = Accel - angle;	//zk-先验估计
	
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //后验估计误差协方差
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
		
	angleAnddot[0]	+= K_0 * Angle_err;	 //后验估计
	Q_bias	+= K_1 * Angle_err;	 //后验估计
	angleAnddot[1]   = Gyro - Q_bias;	 //输出值(后验估计)的微分=角速度
	
}

/**************************************************************************
函数功能：一阶互补滤波
入口参数：加速度、角速度和上次计算出的角度
返回  值：本次计算得出的角度
**************************************************************************/
float first_order_filter_for_mpu(float* angle_m, float* gyro_m, float* angle)
{
	float K1 = 0.02; 
	*angle = K1 * (*angle_m) + (1.0f - K1) * ((*angle) + (*gyro_m) * 0.005f);
	return *angle;
}

/**************************************************************************
函数功能：互补融合-清华滤波方案
入口参数：加速度、角速度和上次计算出的角度
返回  值：本次计算得出的角度
**************************************************************************/
float QingHua_AngleCal(float* angle_m, float* gyro_m, float* angle)
{
	float Gt = 1.0f,Td = 10.0f; //TODO:严格来说，Td应该等于两次本函数被调用之间的时间间隔，如果gyro_m的单位为度/ms，则Td的单位也是ms
	*angle += (((*gyro_m) + ((*angle_m) - (*angle)) / Gt) * Td);
	return *angle;
}

