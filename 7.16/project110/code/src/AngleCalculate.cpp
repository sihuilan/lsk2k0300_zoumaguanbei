#include "zf_common_headfile.h"
// #include "AngleCalculate.h"

uint8 IsMagnet;

std::atomic<bool> IsGyroOffsetReset(true);
ICM20602Datatypedef    ICM20602;
AttitudeDatatypedef    GyroOffset;

int32 temp;                                      //记录当前偏航角，用于特殊元素记录打角


//-------------------------------------------------------------------------------------------------------------------
//  @brief      陀螺仪零飘初始化
//-------------------------------------------------------------------------------------------------------------------
static void GyroOffset_init(void)
{
    static uint16 Count = 0;

    if(Count == 0)
    {
        GyroOffset.Xdata = 0;
        GyroOffset.Ydata = 0;
        GyroOffset.Zdata = 0;
    }

    if(Count == 100)
    {
        GyroOffset.Xdata *= 0.01;
        GyroOffset.Ydata *= 0.01;
        GyroOffset.Zdata *= 0.01;
        IsGyroOffsetReset = false;
        Count = 0;
    }
    else
    {
        icm20602_get_acc();
        icm20602_get_gyro();

        GyroOffset.Xdata += icm20602_gyro_x;
        GyroOffset.Ydata += icm20602_gyro_y;
        GyroOffset.Zdata += icm20602_gyro_z;
        Count++;
    }
}

// //-------------------------------------------------------------------------------------------------------------------
// //  @brief      车体角度姿态采集
// //-------------------------------------------------------------------------------------------------------------------
// void Attitude_Geting(void)
// {
   
//     if(IsGyroOffsetReset == true)
//     {
//         GyroOffset_init();
//         return;
//     }


//     // float Pitch = 0;

//     icm20602_get_acc();
//     icm20602_get_gyro();

//     // 对加速度使用一阶低通滤波
//     ICM20602.ACCX = static_cast<float>(icm20602_acc_transition(icm20602_acc_x)) / 4096;
//     ICM20602.ACCY = static_cast<float>(icm20602_acc_transition(icm20602_acc_y)) / 4096;
//     ICM20602.ACCZ = static_cast<float>(icm20602_acc_transition(icm20602_acc_z)) / 4096;
//     //陀螺仪原始数据
//     ICM20602.GYROX = static_cast<float>(icm20602_gyro_x) * 0.0609756f / 57.3f;
//     ICM20602.GYROY = static_cast<float>(icm20602_gyro_y - GyroOffset.Ydata) * 0.0609756f / 57.3f;
//     ICM20602.GYROZ = static_cast<float>(icm20602_gyro_z - GyroOffset.Zdata) * 0.0609756f / 57.3f;

//     // 四元数转欧拉角
//     IMU_quaterToEulerianAngles();

//     CarInfo.PitchRate = ICM20602.GYROY * 57.3f; 
//     if(fabs(CarInfo.PitchRate) < 0.3)   CarInfo.PitchRate = 0;
//     CarInfo.Pitch += CarInfo.PitchRate * PERIODMS/1000;//0.003;       

//     CarInfo.YawRate = ICM20602.GYROZ * 57.3f;
//     if(fabs(CarInfo.YawRate) < 0.2)   CarInfo.YawRate = 0;

//     atomic_yaw_rate.store(CarInfo.YawRate, std::memory_order_relaxed);
    
//     CarInfo.Yaw += (CarInfo.YawRate * PERIODMS/1000)*2;//0.003;


//     if (RoadFlag.AI) {
//          RoadFlag.AIAngle+= (CarInfo.YawRate * PERIODMS/1000)*2;   
//     } else {
//         RoadFlag.AIAngle = 0;
//     }

//     // 摄像头
//     // 进圆环
//     if (RoadFlag.circle) {
//         // RoadFlag.circleAngle += CarInfo.YawRate * PERIODMS/1000*1.2; //0.003 
//     if(RoadFlag.circle!=1&&RoadFlag.circle!=2&&RoadFlag.circle!=3&&RoadFlag.circle!=4 )
//     {
//          RoadFlag.circleAngle += (CarInfo.YawRate * PERIODMS/1000)*2;   
//     //             if(std::abs(y_error)<0.006){
//     //     error_sumr+=y_error;
//     //     RoadFlag.circleAngle-=error_sumr;
//     // } 
//     //     y_error=RoadFlag.circleAngle-CarInfo.Last_Yaw;
//     //     CarInfo.Last_Yaw = RoadFlag.circleAngle;
//     }
//                                                                         //后面的参数越大，圆环转的角度越少，反之越大（这里的参考数据都是在每3ms进入一次中断得到的）  
//                                                                         //开显示屏3.0  //2.0
//     } else {
//         RoadFlag.circleAngle = 0;
//     }
// }
#include <chrono> // 确保在文件开头引入了此头文件

//-------------------------------------------------------------------------------------------------------------------
//  @brief      车体角度姿态采集（优化版）
//-------------------------------------------------------------------------------------------------------------------
void Attitude_Geting(void)
{
    if(IsGyroOffsetReset == true)
    {
        GyroOffset_init();
        return;
    }

    // 1. ?? 动态计算两次调用的真实时间差 dt (解决 Linux 系统调度抖动)
    static auto last_time = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    float dt = std::chrono::duration<float>(now - last_time).count();
    last_time = now;

    // 异常保护：若第一次运行或间隔过长，则使用理论值
    if(dt <= 0.0f || dt > 0.1f) {
        dt = static_cast<float>(PERIODMS) / 1000.0f;
    }

    // 获取传感器数据
    icm20602_get_acc();
    icm20602_get_gyro();

    // 2. ?? X轴也增加零偏校准减除
    ICM20602.ACCX = static_cast<float>(icm20602_acc_transition(icm20602_acc_x)) / 4096.0f;
    ICM20602.ACCY = static_cast<float>(icm20602_acc_transition(icm20602_acc_y)) / 4096.0f;
    ICM20602.ACCZ = static_cast<float>(icm20602_acc_transition(icm20602_acc_z)) / 4096.0f;

    // 转换为弧度制（供 Pitch/Roll 的四元数解算使用）
    ICM20602.GYROX = static_cast<float>(icm20602_gyro_x - GyroOffset.Xdata) * 0.0609756f / 57.3f;
    ICM20602.GYROY = static_cast<float>(icm20602_gyro_y - GyroOffset.Ydata) * 0.0609756f / 57.3f;
    ICM20602.GYROZ = static_cast<float>(icm20602_gyro_z - GyroOffset.Zdata) * 0.0609756f / 57.3f;

    // 更新四元数以计算 Pitch (俯仰角)
    IMU_quaterToEulerianAngles();

    CarInfo.PitchRate = ICM20602.GYROY * 57.3f; 
    if(fabs(CarInfo.PitchRate) < 0.3f)   CarInfo.PitchRate = 0.0f;
    CarInfo.Pitch += CarInfo.PitchRate * dt;       

    // 3. ?? 【核心改动】使用纯净、未被互补滤波污染的 Z轴原始角速度计算 Yaw
    // 0.0609756f 是 2000dps 量程下的 LSB 转换系数。
    // 如果你的 ICM20602 初始化改为了 1000dps，请将此系数改为 0.03048f！
    float pure_yaw_rate = static_cast<float>(icm20602_gyro_z - GyroOffset.Zdata) * 0.0609756f;
    
    // 消除静态死区微小噪声
    if(fabs(pure_yaw_rate) < 0.2f) {
        pure_yaw_rate = 0.0f;
    }
    CarInfo.YawRate = pure_yaw_rate;

    // 存储原子变量供其他线程使用
    atomic_yaw_rate.store(CarInfo.YawRate, std::memory_order_relaxed);
    
    // 4. ?? 严格使用真实的 dt 积分，彻底删掉原本不科学的 "*2"
    CarInfo.Yaw += CarInfo.YawRate * dt;

    if (RoadFlag.AI) {
        RoadFlag.AIAngle += CarInfo.YawRate * dt;   
    } else {
        RoadFlag.AIAngle = 0;
    }

    // 圆环角度积分
    if (RoadFlag.circle) {
        if(RoadFlag.circle != 1 && RoadFlag.circle != 2 && RoadFlag.circle != 3 && RoadFlag.circle != 4)
        {
            RoadFlag.circleAngle += CarInfo.YawRate * dt;   
        }
    } else {
        RoadFlag.circleAngle = 0;
    }
}
uint8 imu_self_check(void){

    if(DEV_IMU660RA == imu_type)
    {
        printf("IMU DEV IS IMU660RA\r\n");
    }
    else if(DEV_IMU660RB == imu_type)
    {
        printf("IMU DEV IS IMU660RB\r\n");
    }
    else if(DEV_IMU963RA == imu_type)
    {
        printf("IMU DEV IS IMU963RA\r\n");
    }
    else if(DEV_ICM20602 == imu_type)
    {
        printf("IMU DEV IS ICM20602\r\n");
    }    
    else
    {
        printf("NO FIND IMU DEV\r\n");
    }
    return 0;
}
    