#include "icm20602.h"
#include "zf_device_imu_core.h"

int16 icm20602_acc_x, icm20602_acc_y, icm20602_acc_z;  
int16 icm20602_gyro_x, icm20602_gyro_y, icm20602_gyro_z;

void icm20602_get_acc(void)
{
	icm20602_acc_x = imu_get_raw(imu_file_path[ACC_X_RAW]);
	icm20602_acc_y = imu_get_raw(imu_file_path[ACC_Y_RAW]);
	icm20602_acc_z = imu_get_raw(imu_file_path[ACC_Z_RAW]);
}

void icm20602_get_gyro(void)
{
	icm20602_gyro_x = imu_get_raw(imu_file_path[GYRO_X_RAW]);
	icm20602_gyro_y = imu_get_raw(imu_file_path[GYRO_Y_RAW]);
	icm20602_gyro_z = imu_get_raw(imu_file_path[GYRO_Z_RAW]);
}
