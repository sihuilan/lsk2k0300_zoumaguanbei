#ifndef CODE_INC_ATTITUDE_CALCULATION_H
#define CODE_INC_ATTITUDE_CALCULATION_H


#include <math.h>

//#ifndef PI
//#define PI          3.1415926f
//#endif /*PI*/

#define PERIODS     0.005f  //采样周期

typedef struct{
    float q0;
    float q1;
    float q2;
    float q3;
}QuaterInfo;

typedef struct{
    float Pitch;
    float Roll;
    float Yaw;
}EulerianAngles;


typedef struct{
    float Xdata;
    float Ydata;
    float Zdata;
}AttitudeDatatypedef;

typedef struct
{
    float GYROX;
    float GYROY;
    float GYROZ;
    float ACCX;
    float ACCY;
    float ACCZ;
    float Last_ACCX;
    float Last_ACCY;
    float Last_ACCZ;
}ICM20602Datatypedef;

extern ICM20602Datatypedef    ICM20602;
extern EulerianAngles EulerAngle; //欧拉角

void IMU_AHRSupdate_noMagnetic(ICM20602Datatypedef *ICM);
void IMU_quaterToEulerianAngles(void);

#endif