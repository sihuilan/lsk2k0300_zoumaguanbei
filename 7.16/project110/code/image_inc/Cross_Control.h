#ifndef CODE_IMAGE_INC_CROSS_CONTROL_H_
#define CODE_IMAGE_INC_CROSS_CONTROL_H_


typedef enum CrossRoad_State {
    no_crossRoad = 0,
    front_crossRoad_1,
    front_crossRoad_2,
    front_crossRoad_3,
    in_crossRoad_1
}CrossRoad_State;

// static 
int16 limit_a_b(int16 x, int a, int b);
void calculate_s_i(uint8 start, uint8 end, uint16 *border, float *slope_rate, float *intercept);
int check_CrossRoad(void);
int check_In_CrossRoad(void);
void in_crossRoad_pullLine(void);
void crossRoad_processing(void);

void left_plus_right_2midline(void);
void leftLine_plus_halfWide(void);
void rightLine_plus_halfWide(void);

#endif