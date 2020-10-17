#ifndef __FILTER_H
#define __FILTER_H

void Kalman_Filter(float Accel,float Gyro, float angle, float angleAnddot[2]);		
float first_order_filter_for_mpu(float* angle_m, float* gyro_m, float* angle);
float QingHua_AngleCal(float* angle_m, float* gyro_m, float* angle);
#endif
