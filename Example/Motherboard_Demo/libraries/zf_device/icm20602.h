#ifndef _icm20602_h
#define _icm20602_h

#include "zf_common_typedef.h"


extern int16 icm20602_acc_x, icm20602_acc_y, icm20602_acc_z;  
extern int16 icm20602_gyro_x, icm20602_gyro_y, icm20602_gyro_z;

void icm20602_get_acc(void);
void icm20602_get_gyro(void);

#endif