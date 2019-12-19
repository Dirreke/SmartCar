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
#define UNDISTORT_D 60.8582
#define UNDISTORT_H 73.3878
#define UNDISTORT_C 0.5594
#define UNDISTORT_S 0.8289//程序中加了负号，故与matlab相反

/********************** conrtol.c差速 ********************************/
#define ANGLE_RANGE 0.5585 //舵机最大偏转角，单位：弧度,32du
#define CAR_DIFF_K 0.7750//小车两轮宽比两轮长，暂设为0，此时无差速，车宽15.5，车长20


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