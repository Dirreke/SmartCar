#include "headfile.h"
extern PID PID_CAR_Diffcomp_CAM;
extern PID PID_CAR_STRAIGHT_CAM;
extern PID PID_CAR_CENTER_CAM;
extern float Mid_slope;
extern float car_straight_dias;
extern float car_center_dias;
extern float car_diffcomp_dias;
extern float car_diffcomp_PWM;
void Turn_Cam_New(void);
float M_Slope_fig(void);
int gmyshuoqianbuchulai(int temp);
void Straight_offset_filter(void);
float Car_diff_comp(void);
void Turn_diff_comp(void);
float car_center(void);
void Center_offset_filter(void);
void Turn_Cam_dias(void);