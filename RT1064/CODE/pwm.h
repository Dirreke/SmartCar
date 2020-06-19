#include "headfile.h"
//ֻ�õ����1 �� ���3
#define MOTOR1_A   PWM2_MODULE3_CHA_D2   //����1�����תPWM����
#define MOTOR1_B   PWM1_MODULE3_CHA_D0   //����1�����תPWM����
                        
#define MOTOR2_A   PWM1_MODULE3_CHB_D1   //����2�����תPWM����
#define MOTOR2_B   PWM2_MODULE3_CHB_D3   //����2�����תPWM����
    
#define MOTOR3_A   PWM1_MODULE0_CHA_D12  //����3�����תPWM����
#define MOTOR3_B   PWM1_MODULE1_CHA_D14  //����3�����תPWM����
    
#define MOTOR4_A   PWM1_MODULE0_CHB_D13  //����4�����תPWM����
#define MOTOR4_B   PWM1_MODULE1_CHB_D15  //����4�����תPWM����


typedef enum
{
    Mot0,
    Mot1,
    Mot2,
    Mot3,
    Mot4,
    Mot5,
}Mot_CHn_e;



//���ռ�ձȵĿ���
void Motor_Init(void);
void Motor_Duty(uint16 Motno, uint32 duty);
//���ռ�ձȵĿ���
void Servo_Init(void);
void Servo_Duty(float duty);

void Moto_Out(float MotorOut1,float MotorOut2);
