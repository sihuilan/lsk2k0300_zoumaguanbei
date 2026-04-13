#include "zf_common_headfile.h"

float I_ex, I_ey, I_ez;  // 误差积分
float Kp = 0.17;   // 加速度计(磁力计)的收敛速率比例增益50
float Ki = 0.001;//0.2;   //陀螺仪收敛速率的积分增益 0.2
QuaterInfo Q_info = {1, 0, 0, 0};  // 全局四元数
EulerianAngles EulerAngle; //欧拉角

//-------------------------------------------------------------------------------------------------------------------
// @brief   求倒数平方根
//-------------------------------------------------------------------------------------------------------------------
//static float invSqrt(float x) {
//    float halfx = 0.5f * x;
//    float y = x;
//    long i = *(long*)&y;
//    i = 0x5f375a86 - (i >> 1);// 0x5f3759df
//
//    y = *(float*)&i;
//    y = y * (1.5f - (halfx * y * y));
//    y = y * (1.5f - (halfx * y * y));
//    return y;
//}

//-------------------------------------------------------------------------------------------------------------------
// @brief   互补滤波算法进行姿态解算
//-------------------------------------------------------------------------------------------------------------------
void IMU_AHRSupdate_noMagnetic(ICM20602Datatypedef *ICM)
{
    float halfT = 0.5 * PERIODS;
    float vx, vy, vz;    //当前的机体坐标系上的重力单位向量
    float ex, ey, ez;    //四元数计算值与加速度计测量值的误差

    //四元数
    float q0 = Q_info.q0;
    float q1 = Q_info.q1;
    float q2 = Q_info.q2;
    float q3 = Q_info.q3;

    float q0q0 = q0 * q0;
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
//    float q0q3 = q0 * q3;
    float q1q1 = q1 * q1;
//    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;

    // 正常静止状态为-g 反作用力。
     if(ICM->ACCX * ICM->ACCY * ICM->ACCZ == 0) // 加计处于自由落体状态时(此时g = 0)不进行姿态解算，因为会产生分母无穷大的情况
         return;

    //对加速度数据进行归一化 得到单位加速度
    float norm = sqrtf(ICM->ACCX*ICM->ACCX + ICM->ACCY*ICM->ACCY + ICM->ACCZ*ICM->ACCZ);
    ICM->ACCX /=  norm;
    ICM->ACCY /=  norm;
    ICM->ACCZ /=  norm;

    // 载体坐标系下重力在三个轴上的分量
    vx = 2 * (q1q3 - q0q2);
    vy = 2 * (q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;

    // g^b 与 a^b 做向量叉乘，得到陀螺仪的校正补偿向量e的系数
    ex = ICM->ACCY * vz - ICM->ACCZ * vy;
    ey = ICM->ACCZ * vx - ICM->ACCX * vz;
    ez = ICM->ACCX * vy - ICM->ACCY * vx;

    //用叉乘误差来做PI修正陀螺零偏，
    //通过调节 Kp，Ki 两个参数，
    //可以控制加速度计修正陀螺仪积分姿态的速度。
    I_ex += halfT * ex;   // integral error scaled by Ki
    I_ey += halfT * ey;
    I_ez += halfT * ez;

    // 使用PI控制器消除向量积误差(陀螺仪漂移误差)
    ICM->GYROX = ICM->GYROX + Kp * ex + Ki * I_ex;
    ICM->GYROY = ICM->GYROY + Kp * ey + Ki * I_ey;
    ICM->GYROZ = ICM->GYROZ + Kp * ez + Ki * I_ez;

    /*数据修正完成，下面是四元数微分方程*/

    //四元数微分方程，其中halfT为测量周期的1/2，gx gy gz为陀螺仪角速度，以下都是已知量，这里使用了一阶龙哥库塔求解四元数微分方程
    q0 = q0 + (-q1 * ICM->GYROX - q2 * ICM->GYROY - q3 * ICM->GYROZ) * halfT;
    q1 = q1 + ( q0 * ICM->GYROX + q2 * ICM->GYROZ - q3 * ICM->GYROY) * halfT;
    q2 = q2 + ( q0 * ICM->GYROY - q1 * ICM->GYROZ + q3 * ICM->GYROX) * halfT;
    q3 = q3 + ( q0 * ICM->GYROZ + q1 * ICM->GYROY - q2 * ICM->GYROX) * halfT;

    // 单位化四元数在空间旋转时不会拉伸，仅有旋转角度，下面算法类似线性代数里的正交变换
    norm = sqrtf(q0*q0 + q1*q1 + q2*q2 + q3*q3);

    // 用全局变量记录上一次计算的四元数值
    Q_info.q0 = q0 / norm;
    Q_info.q1 = q1 / norm;
    Q_info.q2 = q2 / norm;
    Q_info.q3 = q3 / norm;
}

/*把四元数转换成欧拉角*/
void IMU_quaterToEulerianAngles(void)
{
    IMU_AHRSupdate_noMagnetic(&ICM20602);

    float q0 = Q_info.q0;
    float q1 = Q_info.q1;
    float q2 = Q_info.q2;
    float q3 = Q_info.q3;
    EulerAngle.Pitch = asin(-2*q1*q3 + 2*q0*q2) * 180 / 3.1415926f; // pitch
//    EulerAngle.Roll = atan2(2*q2*q3 + 2*q0*q1, -2*q1*q1 - 2*q2*q2 + 1) * 180 / 3.1415926f; // roll
//    EulerAngle.Yaw = atan2(2*q1*q2 + 2*q0*q3, -2*q2*q2 - 2*q3*q3 + 1) * 180 / 3.1415926f; // yaw
}
