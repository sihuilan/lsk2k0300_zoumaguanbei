#ifndef _zf_common_headfile_h_
#define _zf_common_headfile_h_



#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include <cmath> 
#include <algorithm>
#include <fcntl.h>
#include <signal.h>
#include <cstring>
#include <pthread.h>

#include <sys/wait.h>

#include <opencv2/imgproc/imgproc.hpp>  // for cv::cvtColor
#include <opencv2/highgui/highgui.hpp> // for cv::VideoCapture
#include <opencv2/opencv.hpp>

#include <iostream> // for std::cerr
#include <fstream>  // for std::ofstream
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <vector>

using namespace cv;

//===================================================芯片 SDK 底层===================================================

//===================================================芯片 SDK 底层===================================================

//====================================================开源库公共层====================================================
// #include "zf_common_clock.h"
// // #include "zf_common_debug.h"
#include "zf_common_font.h"
#include "zf_common_function.h"
// #include "zf_common_interrupt.h"
// #include "zf_common_fifo.h"
#include "zf_common_typedef.h"
//====================================================开源库公共层====================================================

//===================================================芯片外设驱动层===================================================
// #include "zf_driver_adc.h"
#include "zf_driver_delay.h"
// #include "zf_driver_dvp.h"
// #include "zf_driver_encoder.h"
// #include "zf_driver_exti.h"
// #include "zf_driver_flash.h"
#include "zf_driver_gpio.h"
#include "zf_driver_file.h"
#include "zf_driver_encoder.h"
#include "zf_driver_pwm.h"
#include "zf_driver_adc.h"
#include "zf_driver_pit.h"
// //#include "zf_driver_iic.h"
// #include "zf_driver_pit.h"
// #include "zf_driver_pwm.h"
// //#include "zf_driver_soft_iic.h"
// //#include "zf_driver_soft_spi.h"
// #include "zf_driver_spi.h"
// #include "zf_driver_timer.h"
// #include "zf_driver_uart.h"
// #include "zf_driver_usb_cdc.h"
// #include "zf_driver_usb_cdc.h"
#include "zf_driver_udp.h"
#include "zf_driver_tcp_client.h"
//===================================================芯片外设驱动层===================================================

//===================================================外接设备驱动层===================================================
#include "zf_device_ips200_fb.h"
#include "zf_device_uvc.h"
#include "zf_device_imu_core.h"
//#include "zf_device_imu660ra.h"
// #include "zf_device_imu660rb.h"
// #include "zf_device_imu963ra.h"
#include "icm20602.h"

// #include "zf_device_uvc.h"
// #include "zf_device_camera.h"
// #include "zf_device_icm20602.h"
// #include "zf_device_ips114.h"
// #include "zf_device_tft180.h"
// #include "zf_device_ips200.h"
// #include "zf_device_mt9v03x_dvp.h"
// #include "zf_device_mpu6050.h"
// #include "zf_device_type.h"
// #include "zf_device_wireless_uart.h"
// #include "zf_device_oled.h"
// #include "zf_device_scc8660_dvp.h"
// #include "zf_device_bluetooth_ch9141.h"
// #include "zf_device_wireless_ch573.h"
// #include "zf_device_wireless_uart.h"
// #include "zf_device_virtual_oscilloscope.h"
// #include "zf_device_w25q32.h"
// #include "zf_device_k24c02.h"
// #include "zf_device_aht20.h"
// #include "zf_device_wifi_uart.h"
// #include "zf_device_imu660ra.h"
// #include "zf_device_imu963ra.h"
// #include "zf_device_key.h"
// #include "zf_device_gnss.h"
// #include "zf_device_dl1a.h"
// #include "zf_device_dm1xa.h"
// #include "zf_device_wifi_spi.h"
// #include "zf_device_dl1b.h"
// #include "zf_device_ble6a20.h"



//=================================================模型推理文件===================================================

#include "ncnn.h"
#include "net.h" // for ncnn
#include "ncnn_infer.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>  // for cv::cvtColor
#include <opencv2/highgui/highgui.hpp> // for cv::VideoCapture
#include <iostream> // for std::cerr
#include <fstream>  // for std::ofstream
#include <thread>
#include <chrono>
#include <atomic>

//===================================================应用组件层===================================================
#include "seekfree_assistant.h"
#include "seekfree_assistant_interface.h"
//===================================================应用组件层===================================================

//=================================================用户自定义文件===================================================
#include "Init.h"
#include "Motor_Control.h"
#include "Speed_Control.h"
#include "Car_Control.h"
#include "Parameter.h"
#include "Display.h"
#include "Save_Date.h"
#include "AngleCalculate.h"
#include "Attitude_Calculation.h"
#include "Seekfree_Help.h"

#include "Binary_Image.h"
#include "Image.h"
#include "ImageCircle.h"
#include "ImageDirection_Control.h"
#include "RoadControl.h"
#include "RoadRecognition.h"

#include "Cross_Control.h"
#include "find_jiaodian.h"
#include "Ring_Control.h"


#endif