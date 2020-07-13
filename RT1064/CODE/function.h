#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#define TR2barn

#define PIT_TIME 2
#define SERVO_MIDDLE 1320  //1.36*20000/20;1140-1500 （1120打死）
#define SERVO_RANGE 180 //190 limit
#define MOTOR_RANGE 18000
#define CAMERA_FPS 100
#define DIS_IN_CIRCLE 1.1 //0.9
/**********************search.c去畸变***********************************/
#define MAP_K0 300  //去畸变后赛道宽，此可用于调整，Road_Width为标准宽度
#define MAP_B 10   //畸变常数
//仅用于0-55行
//56-60行放在serch.c中 K0_Table
//MAP_K0/(y+MAP_B)

/*************************search.c去畸变2 *****************************/
#define UNDISTORT_D 59.9915
#define UNDISTORT_H 21.7060
#define UNDISTORT_C 0.2172
#define UNDISTORT_S 0.9761//程序中加了负号，故与matlab相反
#define UNDISTORT_PYK 2//1.6
#define UNDISTORT_XPK 2.2050
#define UNDISTORT_PWK 3.0187//像素：实际
/********************** conrtol.c差速 ********************************/
#define ANGLE_RANGE 0.5585 //舵机最大偏转角，单位：弧度,32du
#define CAR_DIFF_K 0.7750//小车两轮宽比两轮长，暂设为0，此时无差速，车宽15.5，车长20
#define CAR_LENGTH 20//车长cm
#define CAR_LENGTH_P 148.5149
/********************* EM.c ******************************************/
#define EM2EM1K2 4 // EM_Value_2/EM_Value_1的平方

#define FILTER_ARRAY_SIZE 20


/***************************差速两套**********************************/
// speed.h中宏定义
// DIFF0原，DIFF1改
/********************************************************************/
/**************************去畸变两套*********************************/
// speed.h中宏定义
// undistort0原，undistort1改
/*********************************************************************/
void Para_Init();
typedef struct PID{float P,I,D,OUT;}PID;

#endif