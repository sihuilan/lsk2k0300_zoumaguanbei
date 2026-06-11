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

//-------------------------------------------------------------------------------------------------------------------
//  @brief      车体角度姿态采集
//-------------------------------------------------------------------------------------------------------------------
void Attitude_Geting(void)
{
   
    if(IsGyroOffsetReset == true)
    {
        GyroOffset_init();
        return;
    }


    // float Pitch = 0;

    icm20602_get_acc();
    icm20602_get_gyro();

    // 对加速度使用一阶低通滤波
    ICM20602.ACCX = static_cast<float>(icm20602_acc_transition(icm20602_acc_x)) / 4096;
    ICM20602.ACCY = static_cast<float>(icm20602_acc_transition(icm20602_acc_y)) / 4096;
    ICM20602.ACCZ = static_cast<float>(icm20602_acc_transition(icm20602_acc_z)) / 4096;
    //陀螺仪原始数据
    ICM20602.GYROX = static_cast<float>(icm20602_gyro_x) * 0.0609756f / 57.3f;
    ICM20602.GYROY = static_cast<float>(icm20602_gyro_y - GyroOffset.Ydata) * 0.0609756f / 57.3f;
    ICM20602.GYROZ = static_cast<float>(icm20602_gyro_z - GyroOffset.Zdata) * 0.0609756f / 57.3f;

    // 四元数转欧拉角
    IMU_quaterToEulerianAngles();

    /*把四元数转换成欧拉角*/
    //    CarInfo.Pitch = EulerAngle.Pitch * 57.3f;
    //    CarInfo.Roll  = EulerAngle.Roll * 57.3f;
    //    CarInfo.Yaw   = EulerAngle.Yaw * 57.3f;

    //    CarInfo.PitchRate  = -(ICM20602.GYROY-GyroOffset.Ydata) * 57.3f;
    //    if(fabs(CarInfo.PitchRate) < 0.2)   CarInfo.PitchRate = 0;
    //    CarInfo.Pitch += CarInfo.PitchRate * PERIODMS/1000 ;
    // CarInfo.Pitch = EulerAngle.Pitch * 57.3f;
    CarInfo.PitchRate = ICM20602.GYROY * 57.3f; 
    if(fabs(CarInfo.PitchRate) < 0.3)   CarInfo.PitchRate = 0;
    CarInfo.Pitch += CarInfo.PitchRate * PERIODMS/1000;//0.003;       
    // for(int i=0; i<10; i++){
    //     Pitch  += EulerAngle.Pitch * 57.3f;
    // }
    //    CarInfo.Pitch  = Pitch/10;
    //    CarInfo.Roll  = EulerAngle.Roll * 57.3f;
    CarInfo.YawRate = ICM20602.GYROZ * 57.3f;
    if(fabs(CarInfo.YawRate) < 0.2)   CarInfo.YawRate = 0;
    CarInfo.Yaw += (CarInfo.YawRate * PERIODMS/1000)*2;//0.003;
    // if(std::abs(y_error)<0.01){     
    // error_sumr+=y_error;
    // CarInfo.Yaw-=error_sumr;    
    // } 
    // y_error=CarInfo.Yaw-CarInfo.Last_Yaw;
    // CarInfo.YawRate = -CarInfo.YawRate;
    
    // CarInfo.Last_Yaw = CarInfo.Yaw;

    // 摄像头
    // 进圆环
    if (RoadFlag.circle) {
        // RoadFlag.circleAngle += CarInfo.YawRate * PERIODMS/1000*1.2; //0.003 
    if(RoadFlag.circle!=1&&RoadFlag.circle!=2&&RoadFlag.circle!=3&&RoadFlag.circle!=4 )
    {
         RoadFlag.circleAngle+= (CarInfo.YawRate * PERIODMS/1000)*2;   
    //             if(std::abs(y_error)<0.006){
    //     error_sumr+=y_error;
    //     RoadFlag.circleAngle-=error_sumr;
    // } 
    //     y_error=RoadFlag.circleAngle-CarInfo.Last_Yaw;
    //     CarInfo.Last_Yaw = RoadFlag.circleAngle;
    }
                                                                        //后面的参数越大，圆环转的角度越少，反之越大（这里的参考数据都是在每3ms进入一次中断得到的）  
                                                                        //开显示屏3.0  //2.0
    } else {
        RoadFlag.circleAngle = 0;
    }
    if(RoadFlag.AI)
    {
        RoadFlag.AIAngle += (CarInfo.YawRate * PERIODMS/1000)*2;  
    }
    else
    {
        RoadFlag.AIAngle=0;
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
    