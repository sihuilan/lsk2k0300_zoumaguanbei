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
float icm20602_acc_transition (int16 acc_value)
{
    float acc_data = 0;
    switch(ICM20602_ACC_SAMPLE_DEFAULT)
    {
        case ICM20602_ACC_SAMPLE_SGN_2G: acc_data = (float)acc_value / 16384; break;      // 0x00 加速度计量程为:±2g          获取到的加速度计数据 除以16384      可以转化为带物理单位的数据，单位：g(m/s^2)
        case ICM20602_ACC_SAMPLE_SGN_4G: acc_data = (float)acc_value / 8192;  break;      // 0x08 加速度计量程为:±4g          获取到的加速度计数据 除以8192       可以转化为带物理单位的数据，单位：g(m/s^2)
        case ICM20602_ACC_SAMPLE_SGN_8G: acc_data = (float)acc_value / 4096;  break;      // 0x10 加速度计量程为:±8g          获取到的加速度计数据 除以4096       可以转化为带物理单位的数据，单位：g(m/s^2)
        case ICM20602_ACC_SAMPLE_SGN_16G: acc_data = (float)acc_value / 2048;  break;      // 0x18 加速度计量程为:±16g         获取到的加速度计数据 除以2048       可以转化为带物理单位的数据，单位：g(m/s^2)
        default: break;
    }
    return acc_data;
}
float icm20602_gyro_transition (int16 gyro_value)
{
    float gyro_data = 0;
    switch(ICM20602_GYRO_SAMPLE_DEFAULT)
    {
        case ICM20602_GYRO_SAMPLE_SGN_250DPS: gyro_data = (float)gyro_value / 131.2f;  break;  //  0x00 陀螺仪量程为:±250 dps     获取到的陀螺仪数据除以131           可以转化为带物理单位的数据，单位为：°/s
        case ICM20602_GYRO_SAMPLE_SGN_500DPS: gyro_data = (float)gyro_value / 65.6f;   break;  //  0x08 陀螺仪量程为:±500 dps     获取到的陀螺仪数据除以65.5          可以转化为带物理单位的数据，单位为：°/s
        case ICM20602_GYRO_SAMPLE_SGN_1000DPS: gyro_data = (float)gyro_value / 32.8f;   break;  //  0x10 陀螺仪量程为:±1000dps     获取到的陀螺仪数据除以32.8          可以转化为带物理单位的数据，单位为：°/s
        case ICM20602_GYRO_SAMPLE_SGN_2000DPS: gyro_data = (float)gyro_value / 16.4f;   break;  //  0x18 陀螺仪量程为:±2000dps     获取到的陀螺仪数据除以16.4          可以转化为带物理单位的数据，单位为：°/s
        default: break;
    }
    return gyro_data;
}