#include "headfile.h"
extern float Turn_Cam_Center_P_Table[11];
extern float car_center_dias_Table[11];

extern float Turn_Cam_Center_P;
extern PID PID_CAR_STRAIGHT_CAM;
extern PID PID_CAR_CENTER_CAM;
extern float Mid_slope;
extern float car_straight_angle;
extern float car_center_dias;
float car_center(void);
float car_straight(float car_dias);
void Turn_Cam_New(void);
float M_Slope_fig(void);

extern float DEBUG_SLOPE;
extern int DEBUG_MIDMAXMIN;