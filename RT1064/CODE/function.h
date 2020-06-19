#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#define TL2barn

#define PIT_TIME 2
#define SERVO_MIDDLE 1320  //1.36*20000/20;1140-1500 ��1120������
#define SERVO_RANGE 180 //190 limit
#define CAMERA_FPS 100
#define DIS_IN_CIRCLE 1.1 //0.9
/**********************search.cȥ����***********************************/
#define MAP_K0 300  //ȥ������������˿����ڵ�����Road_WidthΪ��׼���
#define MAP_B 10   //���䳣��
//������0-55��
//56-60�з���serch.c�� K0_Table
//MAP_K0/(y+MAP_B)

/*************************search.cȥ����2 *****************************/
#define UNDISTORT_D 59.9915
#define UNDISTORT_H 21.7060
#define UNDISTORT_C 0.2172
#define UNDISTORT_S 0.9761//�����м��˸��ţ�����matlab�෴
#define UNDISTORT_PYK 1.6
#define UNDISTORT_XPK 2.2050
#define UNDISTORT_PWK 3.0187//���أ�ʵ��
/********************** conrtol.c���� ********************************/
#define ANGLE_RANGE 0.5585 //������ƫת�ǣ���λ������,32du
#define CAR_DIFF_K 0.7750//С�����ֿ�����ֳ�������Ϊ0����ʱ�޲��٣�����15.5������20
#define CAR_LENGTH 20//����cm



/***************************��������**********************************/
// speed.h�к궨��
// DIFF0ԭ��DIFF1��
/********************************************************************/
/**************************ȥ��������*********************************/
// speed.h�к궨��
// undistort0ԭ��undistort1��
/*********************************************************************/
void Para_Init();
typedef struct PID{float P,I,D,OUT;}PID;

#endif