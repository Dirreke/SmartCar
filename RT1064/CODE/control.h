#ifndef _CONTROL_H_
#define _CONTROL_H_
#include "headfile.h"
//////////////////added para
//int brake_line=50;
/////////////////////////////////////////
typedef struct PID{float P,pout,I,iout,D,dout,OUT;}PID;
extern  PID PID_SPEED,PID_TURN,PID_SPEED1,PID_SPEED2;

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


#endif
