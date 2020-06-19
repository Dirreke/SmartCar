#include "headfile.h"
//只用到电机1 和 电机3
#define MOTOR1_A   PWM2_MODULE3_CHA_D2   //定义1电机正转PWM引脚
#define MOTOR1_B   PWM1_MODULE3_CHA_D0   //定义1电机反转PWM引脚
                        
#define MOTOR2_A   PWM1_MODULE3_CHB_D1   //定义2电机正转PWM引脚
#define MOTOR2_B   PWM2_MODULE3_CHB_D3   //定义2电机反转PWM引脚
    
#define MOTOR3_A   PWM1_MODULE0_CHA_D12  //定义3电机正转PWM引脚
#define MOTOR3_B   PWM1_MODULE1_CHA_D14  //定义3电机反转PWM引脚
    
#define MOTOR4_A   PWM1_MODULE0_CHB_D13  //定义4电机正转PWM引脚
#define MOTOR4_B   PWM1_MODULE1_CHB_D15  //定义4电机反转PWM引脚


typedef enum
{
    Mot0,
    Mot1,
    Mot2,
    Mot3,
    Mot4,
    Mot5,
}Mot_CHn_e;



//电机占空比的控制
void Motor_Init(void);
void Motor_Duty(uint16 Motno, uint32 duty);
//舵机占空比的控制
void Servo_Init(void);
void Servo_Duty(float duty);

void Moto_Out(float MotorOut1,float MotorOut2);
