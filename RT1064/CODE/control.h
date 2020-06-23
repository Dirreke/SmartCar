#include"headfile.h"

extern float Turn_Cam_Out;
extern float Turn_EM_Out;
extern float MotorOut1,MotorOut2;
extern float speedTarget1,speedTarget2;
extern PID PID_SPEED;



void Turn_Cam(void);
PID TurnFuzzyPD_Cam(void);
void SpeedTarget_fig(void);
void Speed_Control(void);

float EM_err_cal(float l,float r,float pl, float pr);
void TurnFuzzyPD_EM(void);
float PD_section(float err);
float PD_section1(float err);

//debug
extern PID PID_SPEED;
extern PID PID_TURN_CAM_EXT;
// float Turn_P;
// float Turn_D;