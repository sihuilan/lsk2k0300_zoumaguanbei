#ifndef CODE_IMAGE_INC_ROADCONTROL_H_
#define CODE_IMAGE_INC_ROADCONTROL_H_

#include <atomic>


//寻单边参数
#define Left_Normal             110   //79   //寻左边参数   79/40为前瞻行21时的左右边界     84/33是前瞻行为23时的左右边界
#define Right_Normal            33   //40      //寻右边参数  86/34是前瞻行为22时的左右边界(91/30)

//十字补线参数
#define CROSS_END               28//34

extern std::atomic<bool> Go_Flag;
//extern _Bool StopCar;
extern uint8 gotime;

void AIControl(void);
void ZebraControl(void);
void RampControl(void);
void Obstacle_Control(void);


#endif