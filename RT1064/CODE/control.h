#ifndef _CONTROL_H_
#define _CONTROL_H_
#include "headfile.h"
//////////////////added para
//int brake_line=50;
/////////////////////////////////////////
typedef struct PID{float P,pout,I,iout,D,dout,OUT;}PID;
extern  PID PID_SPEED,PID_TURN,PID_SPEED1,PID_SPEED2;


void Get_Speed();
void Moto_Out();


#endif
