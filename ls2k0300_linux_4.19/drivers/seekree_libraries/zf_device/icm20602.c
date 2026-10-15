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
#include <linux/ktime.h>
#include <linux/iio/iio.h>
#include <linux/iio/sysfs.h>
#include <linux/iio/buffer.h>
#include <linux/iio/trigger.h>
#include <linux/iio/triggered_buffer.h>
#include <linux/iio/trigger_consumer.h>
#include <linux/unaligned/be_byteshift.h>

#include "icm20602.h"

  //============ 宏定义必须放在使用它们的函数前面 ============
  #define icm20602_write_register(reg, data)      (regmap_write(dev->regmap, reg, data))

  #define icm20602_read_register(reg)\
  ({\
      uint8 ret = 0; \
      uint32 data = 0; \
      ret = regmap_read(dev->regmap, reg, &data); \
      data; \
  })

  #define icm20602_read_registers(reg, data, len)     (regmap_bulk_read(dev->regmap, reg, data, len))

  //============ 缓存：6 次独立读合并为 1 次 SPI 事务 ============
  static uint8_t  icm20602_buf[14];
  static ktime_t  icm20602_buf_time;
  #define ICM20602_CACHE_US  1000

  static void icm20602_refresh_buf(struct imu_dev_struct *dev)
  {
      ktime_t now = ktime_get();
      if (ktime_us_delta(now, icm20602_buf_time) > ICM20602_CACHE_US) {
          icm20602_read_registers(ICM20602_ACCEL_XOUT_H, icm20602_buf, 14);
          icm20602_buf_time = now;
      }
  }

  //-------------------------------------------------------------------------------------------------------------------
  static uint8 icm20602_self_check(struct imu_dev_struct *dev)
  {
      uint8 dat = 0, return_state = 0;
      uint16 timeout_count = 0;

      while (0x12 != dat) {
          if (timeout_count++ > ICM20602_TIMEOUT_COUNT) {
              return_state = 1;
              break;
          }
          dat = icm20602_read_register(ICM20602_WHO_AM_I);
      }
      return return_state;
  }

  //-------------------------------------------------------------------------------------------------------------------
  int16 icm20602_get_acc(struct imu_dev_struct *dev, int axis)
  {
      int offset;
      icm20602_refresh_buf(dev);
      offset = (axis - IIO_MOD_X) * 2;
      return (int16_t)((uint16_t)icm20602_buf[offset] << 8 | icm20602_buf[offset + 1]);
  }

  //-------------------------------------------------------------------------------------------------------------------
  int16 icm20602_get_gyro(struct imu_dev_struct *dev, int axis)
  {
      int offset;
      icm20602_refresh_buf(dev);
      offset = 8 + (axis - IIO_MOD_X) * 2;
      return (int16_t)((uint16_t)icm20602_buf[offset] << 8 | icm20602_buf[offset + 1]);
  }

  //-------------------------------------------------------------------------------------------------------------------
  uint8 icm20602_init(struct imu_dev_struct *dev)
  {
      uint8 ret = DEV_ICM20602;
      uint8 val = 0x0;
      uint16 timeout_count = 0;
      mdelay(10);

      do {
          if (icm20602_self_check(dev)) {
              ret = DEV_NO_FIND;
              break;
          }

          icm20602_write_register(ICM20602_PWR_MGMT_1, 0x80);
          mdelay(2);
          do {
              val = icm20602_read_register(ICM20602_PWR_MGMT_1);
              if (timeout_count++ > ICM20602_TIMEOUT_COUNT) {
                  ret = DEV_NO_FIND;
                  break;
              }
          } while (0x41 != val);
          if (DEV_NO_FIND == ret)
              break;

          icm20602_write_register(ICM20602_PWR_MGMT_1,   0x01);
          icm20602_write_register(ICM20602_PWR_MGMT_2,   0x00);
          icm20602_write_register(ICM20602_CONFIG,       0x01);
          icm20602_write_register(ICM20602_SMPLRT_DIV,   0x07);

          switch (ICM20602_ACC_SAMPLE_DEFAULT) {
              default:
                  dev_warn(&dev->spi->dev, "ICM20602_ACC_SAMPLE_DEFAULT set error.");
                  ret = DEV_NO_FIND;
                  break;
              case ICM20602_ACC_SAMPLE_SGN_2G:
                  icm20602_write_register(ICM20602_ACCEL_CONFIG, 0x00);
                  break;
              case ICM20602_ACC_SAMPLE_SGN_4G:
                  icm20602_write_register(ICM20602_ACCEL_CONFIG, 0x08);
                  break;
              case ICM20602_ACC_SAMPLE_SGN_8G:
                  icm20602_write_register(ICM20602_ACCEL_CONFIG, 0x10);
                  break;
              case ICM20602_ACC_SAMPLE_SGN_16G:
                  icm20602_write_register(ICM20602_ACCEL_CONFIG, 0x18);
                  break;
          }
          if (ret == DEV_NO_FIND)
              break;

          icm20602_write_register(ICM20602_ACCEL_CONFIG_2, 0x03);

          switch (ICM20602_GYRO_SAMPLE_DEFAULT) {
              default:
                  dev_warn(&dev->spi->dev, "ICM20602_GYRO_SAMPLE_DEFAULT set error.");
                  ret = DEV_NO_FIND;
                  break;
              case ICM20602_GYRO_SAMPLE_SGN_250DPS:
                  icm20602_write_register(ICM20602_GYRO_CONFIG, 0x00);
                  break;
              case ICM20602_GYRO_SAMPLE_SGN_500DPS:
                  icm20602_write_register(ICM20602_GYRO_CONFIG, 0x08);
                  break;
              case ICM20602_GYRO_SAMPLE_SGN_1000DPS:
                  icm20602_write_register(ICM20602_GYRO_CONFIG, 0x10);
                  break;
              case ICM20602_GYRO_SAMPLE_SGN_2000DPS:
                  icm20602_write_register(ICM20602_GYRO_CONFIG, 0x18);
                  break;
          }
      } while (0);

      return ret;
  }