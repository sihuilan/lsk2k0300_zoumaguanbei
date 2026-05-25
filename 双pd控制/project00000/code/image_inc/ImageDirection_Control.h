#ifndef CODE_IMAGE_INC_IMAGEDIRECTION_CONTROL_H_
#define CODE_IMAGE_INC_IMAGEDIRECTION_CONTROL_H_
 



#define     MIDDLE                  179             //图像中值

#define     OFFSET_MAX              100              //偏差极限值
// #define     OFFSET_MAX              75              //偏差极限值

#define     CURVE_OFFSET            30              //急弯、普通弯偏差分界

#define     L_P         10
#define     R_P         10

//extern float direP1 , direD1; //转向PD
extern uint8 curvetime;
extern float OutCurvature;
extern float ExpectCurvature;
extern int16 turn_PWM , turn_speed;
extern float offset, last_offset, Expect_v, Expect_w;
extern float P_term1,P_term2,term;
void imageDirectionPID(void);


 
 #endif

 