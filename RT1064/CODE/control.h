#ifndef _CONTROL_H_
#define _CONTROL_H_
#include "headfile.h"
//////////////////added para
//int brake_line=50;
/////////////////////////////////////////
typedef struct PID{float P,pout,I,iout,D,dout,OUT;}PID;
extern  PID PID_SPEED,PID_TURN,PID_SPEED1,PID_SPEED2;
extern float CarSpeed1;
extern float CarSpeed2;

extern float Turn_Cam_P_Table0[15];//仅调参用
extern float Turn_Cam_D_Table0[15];//仅调参用


//摄像头转弯变量
extern float offset ;

extern float Turn_Cam_Out;
extern float Turn_Cam_P;
extern float Turn_Cam_D;

float limit_f(float n, float lower, float higher);
float my_limit(float a, float limit);

void Get_Speed();
void Moto_Out();
void Speed_Control();
void Speed_Control_Output();
//摄像头转弯程序
void TurnFuzzyPD_Cam(void);
void Turn_Cam();

//电磁
int EM_angle_get(float lp,float l, float r,float rp);
float limit_pos(float a);
float EM_length_err_get(float l,float r,float pl, float pr);
float PD_section(float err);
float PI_section(float err,float pl,float pr);
float non_linear(float a);

#endif
