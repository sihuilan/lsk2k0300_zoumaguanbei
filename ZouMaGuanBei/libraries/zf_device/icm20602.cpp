#include "icm20602.h"
#include "zf_device_imu_core.h"

int16 icm20602_acc_x, icm20602_acc_y, icm20602_acc_z;
int16 icm20602_gyro_x, icm20602_gyro_y, icm20602_gyro_z;

void icm20602_get_acc(void)
{
	icm20602_acc_x = imu_get_raw_fast(ACC_X_RAW);
	icm20602_acc_y = imu_get_raw_fast(ACC_Y_RAW);
	icm20602_acc_z = imu_get_raw_fast(ACC_Z_RAW);
}

void icm20602_get_gyro(void)
{
	icm20602_gyro_x = imu_get_raw_fast(GYRO_X_RAW);
	icm20602_gyro_y = imu_get_raw_fast(GYRO_Y_RAW);
	icm20602_gyro_z = imu_get_raw_fast(GYRO_Z_RAW);
}
float icm20602_acc_transition (int16 acc_value)
{
    float acc_data = 0;
    switch(ICM20602_ACC_SAMPLE_DEFAULT)
    {
        case ICM20602_ACC_SAMPLE_SGN_2G: acc_data = (float)acc_value / 16384; break;
        case ICM20602_ACC_SAMPLE_SGN_4G: acc_data = (float)acc_value / 8192;  break;
        case ICM20602_ACC_SAMPLE_SGN_8G: acc_data = (float)acc_value / 4096;  break;
        case ICM20602_ACC_SAMPLE_SGN_16G: acc_data = (float)acc_value / 2048;  break;
        default: break;
    }
    return acc_data;
}
float icm20602_gyro_transition (int16 gyro_value)
{
    float gyro_data = 0;
    switch(ICM20602_GYRO_SAMPLE_DEFAULT)
    {
        case ICM20602_GYRO_SAMPLE_SGN_250DPS: gyro_data = (float)gyro_value / 131.2f;  break;
        case ICM20602_GYRO_SAMPLE_SGN_500DPS: gyro_data = (float)gyro_value / 65.6f;   break;
        case ICM20602_GYRO_SAMPLE_SGN_1000DPS: gyro_data = (float)gyro_value / 32.8f;   break;
        case ICM20602_GYRO_SAMPLE_SGN_2000DPS: gyro_data = (float)gyro_value / 16.4f;   break;
        default: break;
    }
    return gyro_data;
}