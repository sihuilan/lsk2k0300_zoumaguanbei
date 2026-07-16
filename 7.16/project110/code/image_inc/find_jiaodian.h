#ifndef CODE_IMAGE_INC_FIND_JIAODIAN_CONTROL_H_
#define CODE_IMAGE_INC_FIND_JIAODIAN_CONTROL_H_


//左下拐点
extern int left_points_turnDown[4];
//右上拐点
extern int right_points_turnUp[4];
//右下拐点
extern int right_points_turnDown[4];
//左上拐点
extern int left_points_turnUp[4];

//最长白列左扫线上突变点
extern int left_up_mutation[4];
//最长白列右扫线上突变点
extern int right_up_mutation[4];
extern int Ring_left_mutation[4];

void Ring_Find_Point(void);
void dispose_line(void);
void jiaodian_check(void);

#endif
