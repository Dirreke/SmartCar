#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#define TL2barn

#define PIT_TIME 2
#define SERVO_MIDDLE 1320 //1.36*20000/20;1140-1500 ��1120������
#define SERVO_RANGE 180   //190 limit
#define MOTOR_RANGE 18000
#define CAMERA_FPS 150
#define DIS_IN_CIRCLE 1.1 //0.9
/**********************search.cȥ����***********************************/
#define MAP_K0 300 //ȥ������������˿����ڵ�����Road_WidthΪ��׼���
#define MAP_B 10   //���䳣��
//������0-55��
//56-60�з���serch.c�� K0_Table
//MAP_K0/(y+MAP_B)

/*************************search.cȥ����2 *****************************/
#define UNDISTORT_D 59.9915
#define UNDISTORT_H 21.7060
#define UNDISTORT_C 0.2172
#define UNDISTORT_S 0.9761 //�����м��˸��ţ�����matlab�෴
#define UNDISTORT_PYK 2    //1.6
#define UNDISTORT_XPK 2.2050
#define UNDISTORT_PWK 3.0187 //���أ�ʵ��
/********************** conrtol.c���� ********************************/
#define ANGLE_RANGE 0.5585                     //������ƫת�ǣ���λ������,32du
#define SERVO_DIVIDE_ANGLE_SCALE 322.291853178 //�Ƕ�ת�����������
#define ANGLE_DIVIDE_SERVO_SCALE 0.003102778   //���������ת���Ƕ�
#define CAR_DIFF_K 0.7750                      //С�����ֿ�����ֳ�������Ϊ0����ʱ�޲��٣�����15.5������20
#define CAR_LENGTH 20                          //����cm
#define CAR_LENGTH_P 148.5149
/********************* EM.c ******************************************/
#define EM2EM1K2 4 // EM_Value_2/EM_Value_1��ƽ��

#define FILTER_ARRAY_SIZE 20
#define FILTER_ARRAY_SIZE_INVERSE 0.05

/***************************��������**********************************/
// speed.h�к궨��
// DIFF0ԭ��DIFF1��
/********************************************************************/
/**************************ȥ��������*********************************/
// speed.h�к궨��
// undistort0ԭ��undistort1��
/*********************************************************************/
void Para_Init();
typedef struct PID
{
    float P, I, D, OUT;
} PID;
void SPEED_INIT(void);
extern float DEFAULT_SPEED;
extern float STRAIGHT_SPEED;
extern float CURVE_SPEED;
extern float PRE_STOP_SPEED;
extern float RUSH_STOP_SPEED;
// extern float TURN_STOP_SPEED;
extern float EMERGENCY_STOP_SPEED;
extern float UP_RAMP_SPEED;
extern float ON_RAMP_SPEED;
extern float DOWN_RAMP_SPEED;
#define SPEED_MOTOR_SCALE_LOW 2500
extern int SPEED_MOTOR_SCALE_HIGH;// 3000





















#endif