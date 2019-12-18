#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#define DIFF1

#define PIT_TIME 2
#define SERVO_MIDDLE 1335  //1.36*20000/20;
#define SERVO_RANGE 180 //190 limit
#define CAMERA_FPS 100
#define DIS_IN_CIRCLE 0.9
/**search.c去畸变**/
#define MAP_K0 300  //去畸变后赛道宽
#define MAP_B 10   //畸变常数
//仅用于0-55行
//56-60行放在serch.c中 K0_Table
//MAP_K0/(y+MAP_B)
/** conrtol.c差速 **/
#define ANGLE_RANGE 3.1415926/4 //舵机最大偏转角，单位：弧度
#define CAR_DIFF_K 0//小车两轮宽比两轮长，暂设为0，此时无差速





void Para_Init();
extern float SetSpeed1,SetSpeed2;

#endif