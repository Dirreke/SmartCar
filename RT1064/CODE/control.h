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


void Get_Speed();
void Moto_Out();

//摄像头转弯程序
void TurnFuzzyPD_Cam(void);
void Turn_Cam();


#endif
