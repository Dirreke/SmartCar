#ifndef _Data_Send_H
#define _Data_Send_H
#include "headfile.h"



extern void My_Put_Char(char tmp);
extern void Send_Begin(void);
extern void Send_Img(void);
extern void Pic_send_new(void);
extern void Send_Variable(void);
extern void Variable_update(void);
extern void Send_Data(void);
extern int New_Lef[60];
extern int New_Rig[60];
extern int New_Mid[60];
extern float Lef_slope;
extern float Rig_slope;
extern int Lef_circle;
extern int Rig_circle;
extern uint16 Pic_new[600];
extern int cnt;
extern int Road0_flag;
extern float Turn_P_EM;
extern int whitecnt;
extern float speedTarget1;
extern float speedTarget2;
extern float angle_dbg_1,angle_dbg_2;
extern PID PID_SPEED;
extern float gyroy_1;
extern int16 icm_gyro_y;
extern float stat_slope;
extern char Road1_turnout;
//extern float cam_off_dbg;
/*
 * ȡһ�����ݵĸ���λ
 */
#define BYTE0(Temp)       (*(char *)(&Temp))
#define BYTE1(Temp)       (*((char *)(&Temp) + 1))
#define BYTE2(Temp)       (*((char *)(&Temp) + 2))
#define BYTE3(Temp)       (*((char *)(&Temp) + 3))



#endif
