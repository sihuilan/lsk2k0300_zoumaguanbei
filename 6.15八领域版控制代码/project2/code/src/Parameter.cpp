#include "zf_common_headfile.h"
//#include "Parameter.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      参数初始化
//-------------------------------------------------------------------------------------------------------------------
void DefaultInit() {
    //-----CarInfo相关参数初始化
    CarInfo.distance = 0;

    //-----RoadFlag相关参数初始化
    RoadFlag.protect = false;
    RoadFlag.blind = false;
    RoadFlag.cross = false;
    RoadFlag.ramp = false;
    RoadFlag.circle = false;
    RoadFlag.zebraLine = 6; // 6 8 5
    RoadFlag.AI = 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      浮点型变量限幅
//-------------------------------------------------------------------------------------------------------------------
float ValueLimitfloat(float value, float max, float min) {
    
    if(value > max) value = max;
    else if(value < min) value = min;
    return value;
   }



//-------------------------------------------------------------------------------------------------------------------
//  @brief      整型变量限幅
//-------------------------------------------------------------------------------------------------------------------
int16_t ValueLimitint(int16_t value, int16_t max, int16_t min) {
    
   if(value > max) value = max;
    else if(value < min) value = min;
    return value;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      增幅限幅
//-------------------------------------------------------------------------------------------------------------------
float ChangeLimit(float value, float lastvalue, float increaseLimit, float decreaseLimit) {
    float diff = value - lastvalue;
    if (diff > increaseLimit) {
        value = lastvalue + increaseLimit;
    } else if (diff < decreaseLimit) {
        value = lastvalue + decreaseLimit;
    }
    return value;
}

int ChangeLimitint(int value, int lastvalue, int increaseLimit, int decreaseLimit) {
    int diff = value - lastvalue;
    if (diff > increaseLimit) {
        value = lastvalue + increaseLimit;
    } else if (diff < decreaseLimit) {
        value = lastvalue + decreaseLimit;
    }
    return value;
}    
