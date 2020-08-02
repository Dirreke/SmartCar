#include "headfile.h"

extern float Turn_Cam_Out;
extern float Turn_EM_Out;
extern float Turn_Out;
extern float MotorOut1, MotorOut2;
extern float MotorOut1_add;
extern float MotorOut2_add;

extern float speedTarget1, speedTarget2;
extern float mean_turn_out;
extern PID PID_SPEED;

extern float Turn_EM_Out1, Turn_EM_Out2, Turn_EM_Out;
extern PID PID_CENTER_EM, PID_STRAIGHT_EM;

extern bool speed_change_flag;
// extern bool road_change_flag;
extern int mix_choice;

extern bool a_flag1, a_flag2, d_flag1, d_flag2;
extern bool a_flag_pre;
extern uint8 diff_BB_flag;
// extern float DIFF_KKK;
// extern float DIFF_KK;
void Turn_Cam(void);
PID TurnFuzzyPD_Cam(void);

void lib_set_fun(void);
void SpeedTarget_fig(void);
// void Speed_Control(void);
void Speed_Control_New(void);

void Turn_EM(void);
void Turn_Servo_Normal(void);
// float EM_err_cal(float l,float r,float pl, float pr);
// void TurnFuzzyPD_EM(void);
// float PD_section(float err);
// float PD_section1(float err);

void Kalman_Filter(void);
void BBC(void);
void Mean_Turn_Out(void);

int BB_add_flag_set(void);
void BB_add(void);

void Road7_flag_shift(bool reset0);
void Road4_flag_shift(bool reset0);
void Road2_flag_shift(bool reset0);
void Road1_flag_shift(bool reset0);
void Road0_flag_shift(bool reset0);
void Road_shift(void);
void Curve_shift(void);

//debug
extern PID PID_SPEED, PID2_SPEED;
extern PID PID_diff;
extern PID PID_diff0;
// extern PID PID_TURN_CAM_EXT;
// float Turn_P;
// float Turn_D;
extern float SpeedE1, SpeedE2;
extern float SpeedEE1, SpeedEE2;

extern int ramp_out_time;
extern int out_circle_time_temp;
extern int curve_state;
extern int8 diff_flag1;
extern int8 diff_flag2;