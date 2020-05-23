#include "headfile.h"

void Motor_Init(void)
{
  //初始化PWM1 MODULE0 的通道B 引脚为B23 频率50hz 占空比为 百分之100*5000/PWM_DUTY_MAX   PWM_DUTY_MAX在fsl_pwm.h文件中 默认为50000
  //每一个通道只能有一个引脚输出PWM
  //最大占空比值PWM_DUTY_MAX 可以在fsl_pwm.h文件中修改 已改为20000

  pwm_init(PWM1_MODULE3_CHB_D1, 17000, 0);
  pwm_init(PWM1_MODULE3_CHA_D0, 17000, 0);
  pwm_init(PWM2_MODULE3_CHB_D3, 17000, 0);
  pwm_init(PWM2_MODULE3_CHA_D2, 17000, 0);
}

//舵机居中   1.5ms/20ms * PWM_DUTY_MAX（PWM_DUTY_MAX是PWM的满占空比时候的值） PWM_DUTY_MAX在fsl_pwm.h文件中 已改为20000

void Servo_Init(void)
{
  pwm_init(PWM4_MODULE2_CHA_C30, 50, SERVO_MIDDLE); //SERVOMIDDLE 在Properties中定义
}

void Motor_Duty(uint16 Motno, uint32 duty)
{
  //归一化为0--500--1000
  //if(duty<1001)
  {
    /** if new main_board **/
    /*
    switch (Motno)
    {
    case Mot0:
      pwm_duty(PWM2_MODULE3_CHB_D3, duty); //右电机，
      break;
    case Mot1:
      pwm_duty(PWM1_MODULE3_CHB_D1, duty); //右电机，
      break;
    case Mot2:
      pwm_duty(PWM1_MODULE3_CHA_D0, duty); //左电机，
      break;
    case Mot3:
      pwm_duty(PWM2_MODULE3_CHA_D2, duty); //左电机，
      break;
    default:
      break;
    }
    */
    /** if old main_board **/
    switch (Motno)
    {
    case Mot0:
      pwm_duty(PWM1_MODULE3_CHA_D0, duty); //R电机，
      break;
    case Mot1:
      pwm_duty(PWM2_MODULE3_CHA_D2, duty); //R电机，
      break;
    case Mot2:
      pwm_duty(PWM2_MODULE3_CHB_D3, duty); //L电机，
      break;
    case Mot3:
      pwm_duty(PWM1_MODULE3_CHB_D1, duty); //L电机，
      break;    
    default:
      break;
    }
  }
}

//中位修改放入Servo_Duty中。既输入量<+-SERVO_RANGE
void Servo_Duty(float duty)
{
  if (duty > SERVO_RANGE)
  {
    duty =  SERVO_MIDDLE + SERVO_RANGE;
  }
  else if (duty < 0- SERVO_RANGE)
  {
    duty = SERVO_MIDDLE  - SERVO_RANGE;
  }
  else
  {
    duty+=SERVO_MIDDLE ;
  }
  //归一化为2500--12500
  //if((duty>2500)&&(duty<12500))
  //{
  pwm_duty(PWM4_MODULE2_CHA_C30, (uint32)duty);
  /*
        //计算舵机位置舵机位置   （0.5ms - 2.5ms）ms/20ms * 50000（50000是PWM的满占空比时候的值）
        //舵机最小值为1250   最大值为6250
        //这里加一百 减一百是为了缩小转动的区域，不要转动过于极限位置，有的舵机在极限位置容易卡住
        duty += 10;
        if((6250-100) < duty) duty = (1250+100);
        
        //控制舵机达到指定位置
        pwm_duty(S_MOTOR_PIN,duty);
        
        if((1250+100) >= duty)  systick_delay_ms(1000); //当从6250的位置   回转到1250的位置时   应该使用较长的延时等到舵机达到指定位置
        else                    systick_delay_ms(3);    //

*/
  //}
}
