#ifndef __FUNCTION_H__
#define __FUNCTION_H__


#define PIT_TIME 2
#define SERVO_MIDDLE 1335  //1.36*20000/20;
#define SERVO_RANGE 180 //190 limit
#define CAMERA_FPS 100
#define DIS_IN_CIRCLE 0.9
/**********************search.c去畸变***********************************/
#define MAP_K0 300  //去畸变后赛道宽，此可用于调整，Road_Width为标准宽度
#define MAP_B 10   //畸变常数
//仅用于0-55行
//56-60行放在serch.c中 K0_Table
//MAP_K0/(y+MAP_B)

/*************************search.c去畸变2 *****************************/
#define UNDISTORT_D 30.0
#define UNDISTORT_H 120.0
#define UNDISTORT_C 0.8855
#define UNDISTORT_S 0.4647

/********************** conrtol.c差速 ********************************/
#define ANGLE_RANGE 3.1415926/4 //舵机最大偏转角，单位：弧度
#define CAR_DIFF_K 0//小车两轮宽比两轮长，暂设为0，此时无差速


/***************************差速两套**********************************/
// speed.h中宏定义
// DIFF0原，DIFF1改
/********************************************************************/
/**************************去畸变两套*********************************/
// speed.h中宏定义
// undistort0原，undistort1改
/*********************************************************************/
void Para_Init();
extern float SetSpeed1,SetSpeed2;

#endif