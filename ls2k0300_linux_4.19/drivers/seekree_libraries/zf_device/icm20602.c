#include <linux/spi/spi.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/errno.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/regmap.h>
#include <linux/iio/iio.h>
#include <linux/iio/sysfs.h>
#include <linux/iio/buffer.h>
#include <linux/iio/trigger.h>
#include <linux/iio/triggered_buffer.h>
#include <linux/iio/trigger_consumer.h>
#include <linux/unaligned/be_byteshift.h>

#include "icm20602.h"



//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ICM20602 写寄存器
// 参数说明     reg             寄存器地址
// 参数说明     data            数据
// 返回参数     void
// 使用示例     icm20602_write_register(ICM20602_PWR_MGMT_1, 0x80);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
#define icm20602_write_register(reg, data)      (regmap_write(dev->regmap, reg, data))
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ICM20602 读寄存器
// 参数说明     reg             寄存器地址
// 返回参数     uint8           数据
// 使用示例     icm20602_read_register(ICM20602_WHO_AM_I);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
#define icm20602_read_register(reg)\
({\
    uint8 ret = 0; \
    uint32 data = 0; \
    ret = regmap_read(dev->regmap, reg, &data); \
    data; \
})
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ICM20602 读数据
// 参数说明     reg             寄存器地址
// 参数说明     data            数据缓冲区
// 参数说明     len             数据长度
// 返回参数     void
// 使用示例     icm20602_read_registers(ICM20602_ACCEL_XOUT_H, dat, 6);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
#define icm20602_read_registers(reg, data, len)     (regmap_bulk_read(dev->regmap, reg, data, len))
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     ICM20602 自检
// 参数说明     void
// 返回参数     uint8           1-自检失败 0-自检成功
// 使用示例     icm20602_self_check(dev);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static uint8 icm20602_self_check (struct imu_dev_struct *dev)
{
    uint8 dat = 0, return_state = 0;
    uint16 timeout_count = 0;
    
    while(0x12 != dat)                                                          // 判断 ID 是否正确
    {
        if(timeout_count ++ > ICM20602_TIMEOUT_COUNT)
        {
            return_state =  1;
            break;
        }
        dat = icm20602_read_register(ICM20602_WHO_AM_I);


    }
    return return_state;
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 ICM20602 加速度计数据
// 参数说明     void
// 返回参数     void
// 使用示例     icm20602_get_acc(dev, IIO_MOD_X, &val);                          
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
int16 icm20602_get_acc (struct imu_dev_struct *dev, int axis)
{
    uint8_t dat[2];

    // axis_x = 1
    // axis_y = 2
    // axis_z = 3

    icm20602_read_registers(ICM20602_ACCEL_XOUT_H + (axis - IIO_MOD_X) * 2, dat, 2); 
    // 将读取到的两个字节数据合并为一个16位整数

    return (int16_t)(((uint16_t)dat[0] << 8 | dat[1]));
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 ICM20602 陀螺仪数据
// 参数说明     void
// 返回参数     void
// 使用示例     icm20602_get_gyro();                                            
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
int16 icm20602_get_gyro (struct imu_dev_struct *dev, int axis)
{
    uint8 dat[2];

    // axis_x = 1
    // axis_y = 2
    // axis_z = 3

    icm20602_read_registers(ICM20602_GYRO_XOUT_H+ (axis - IIO_MOD_X) * 2, dat, 2);

    // 将读取到的两个字节数据合并为一个16位整数
    return (int16_t)(((uint16_t)dat[0] << 8 | dat[1]));
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     初始化 ICM20602
// 参数说明     dev             ICM20602设备结构体指针，指向要初始化的设备实例
// 返回参数     uint8           1-初始化失败 0-初始化成功
// 使用示例     icm20602_init(dev);
// 备注信息     对ICM20602设备进行初始化操作，包括自检、配置寄存器
//-------------------------------------------------------------------------------------------------------------------
uint8 icm20602_init(struct imu_dev_struct *dev)
{
    uint8 ret = DEV_ICM20602;
    uint8 val = 0x0;
    uint16 timeout_count = 0;
    mdelay(10);
    do
    {
        if(icm20602_self_check(dev))
        {
            // 如果程序在输出了断言信息 并且提示出错位置在这里
            // 那么就是 ICM20602 自检出错并超时退出了
            // 检查一下接线有没有问题 如果没问题可能就是坏了
            ret = DEV_NO_FIND;
            break;
        }
        
        icm20602_write_register(ICM20602_PWR_MGMT_1, 0x80);                     // 复位设备
        mdelay(2);
        do
        {                                                                       // 等待复位成功
            val = icm20602_read_register(ICM20602_PWR_MGMT_1);
            if(timeout_count ++ > ICM20602_TIMEOUT_COUNT)
            {
                // 如果程序在输出了断言信息 并且提示出错位置在这里
                // 那么就是 ICM20602 自检出错并超时退出了
                // 检查一下接线有没有问题 如果没问题可能就是坏了
                ret = DEV_NO_FIND;
                break;
            }
        }while(0x41 != val);
        if(DEV_NO_FIND == ret)
        {
            break;
        }
        icm20602_write_register(ICM20602_PWR_MGMT_1,     0x01);                 // 时钟设置
        icm20602_write_register(ICM20602_PWR_MGMT_2,     0x00);                 // 开启陀螺仪和加速度计
        icm20602_write_register(ICM20602_CONFIG,         0x01);                 // 176HZ 1KHZ
        icm20602_write_register(ICM20602_SMPLRT_DIV,     0x07);                 // 采样速率 SAMPLE_RATE = INTERNAL_SAMPLE_RATE / (1 + SMPLRT_DIV)


        // ICM20602_ACCEL_CONFIG寄存器
        // 设置为:0x00 加速度计量程为:±2g          获取到的加速度计数据 除以16384      可以转化为带物理单位的数据，单位：g(m/s^2)
        // 设置为:0x08 加速度计量程为:±4g          获取到的加速度计数据 除以8192       可以转化为带物理单位的数据，单位：g(m/s^2)
        // 设置为:0x10 加速度计量程为:±8g          获取到的加速度计数据 除以4096       可以转化为带物理单位的数据，单位：g(m/s^2)
        // 设置为:0x18 加速度计量程为:±16g         获取到的加速度计数据 除以2048       可以转化为带物理单位的数据，单位：g(m/s^2)
       

        switch(ICM20602_ACC_SAMPLE_DEFAULT)
        {
            default:
            {
                dev_warn(&dev->spi->dev, "ICM20602_ACC_SAMPLE_DEFAULT set error.");
                ret = DEV_NO_FIND;
            }break;
            case ICM20602_ACC_SAMPLE_SGN_2G:
            {
                icm20602_write_register(ICM20602_ACCEL_CONFIG, 0x00);
                //icm20602_transition_factor[0] = 16393;
            }break;
            case ICM20602_ACC_SAMPLE_SGN_4G:
            {
                icm20602_write_register(ICM20602_ACCEL_CONFIG, 0x08);
                //icm20602_transition_factor[0] = 8197;
            }break;
            case ICM20602_ACC_SAMPLE_SGN_8G:
            {
                icm20602_write_register(ICM20602_ACCEL_CONFIG, 0x10);
                //icm20602_transition_factor[0] = 4098;
            }break;
            case ICM20602_ACC_SAMPLE_SGN_16G:
            {
                icm20602_write_register(ICM20602_ACCEL_CONFIG, 0x18);
                //icm20602_transition_factor[0] = 2049;
            }break;
        }
        if(ret == DEV_NO_FIND)
        {
            break;
        }

        icm20602_write_register(ICM20602_ACCEL_CONFIG_2, 0x03);
        
        // ICM20602_GYR_CONFIG寄存器
        // 设置为:0x00 陀螺仪量程为:±250 dps     获取到的陀螺仪数据除以131.2         可以转化为带物理单位的数据，单位为：°/s
        // 设置为:0x08 陀螺仪量程为:±500 dps     获取到的陀螺仪数据除以65.6          可以转化为带物理单位的数据，单位为：°/s
        // 设置为:0x10 陀螺仪量程为:±1000dps     获取到的陀螺仪数据除以32.8          可以转化为带物理单位的数据，单位为：°/s
        // 设置为:0x18 陀螺仪量程为:±2000dps     获取到的陀螺仪数据除以16.4          可以转化为带物理单位的数据，单位为：°/s
        switch(ICM20602_GYRO_SAMPLE_DEFAULT)
        {
            default:
            {
                dev_warn(&dev->spi->dev, "ICM20602_GYRO_SAMPLE_DEFAULT set error.");
                ret = DEV_NO_FIND;
            }break;
            case ICM20602_GYRO_SAMPLE_SGN_250DPS:
            {
                icm20602_write_register(ICM20602_GYRO_CONFIG, 0x00);
                //icm20602_transition_factor[1] = 114.3;
            }break;
            case ICM20602_GYRO_SAMPLE_SGN_500DPS:
            {
                icm20602_write_register(ICM20602_GYRO_CONFIG, 0x08);
                //icm20602_transition_factor[1] = 57.1;
            }break;
            case ICM20602_GYRO_SAMPLE_SGN_1000DPS:
            {
                icm20602_write_register(ICM20602_GYRO_CONFIG, 0x10);
                //icm20602_transition_factor[1] = 28.6;
            }break;
            case ICM20602_GYRO_SAMPLE_SGN_2000DPS:
            {
                icm20602_write_register(ICM20602_GYRO_CONFIG, 0x18);
                //icm20602_transition_factor[1] = 14.3;
            }break;
        }
    }while(0);
return ret;
}
