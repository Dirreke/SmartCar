#include "headfile.h"

void Motor_Init(void)
{
  //初始化PWM1 MODULE0 的通道B 引脚为B23 频率50hz 占空比为 百分之100*5000/PWM_DUTY_MAX   PWM_DUTY_MAX在fsl_pwm.h文件中 默认为50000
  //每一个通道只能有一个引脚输出PWM
  //最大占空比值PWM_DUTY_MAX 可以在fsl_pwm.h文件中修改 已改为20000

  pwm_init(PWM1_MODULE3_CHB_D1, 13000, 0);
  pwm_init(PWM1_MODULE3_CHA_D0, 13000, 0);
  pwm_init(PWM1_MODULE0_CHB_D13, 13000, 0);
  pwm_init(PWM1_MODULE0_CHA_D12, 13000, 0);
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
    switch (Motno)
    {
    case Mot0:
      pwm_duty(PWM1_MODULE3_CHA_D0, duty); //电机，
      break;
    case Mot1:
      pwm_duty(PWM1_MODULE3_CHB_D1, duty); //电机，
      break;
    case Mot2:
      pwm_duty(PWM1_MODULE0_CHA_D12, duty); //电机，
      break;
    case Mot3:
      pwm_duty(PWM1_MODULE0_CHB_D13, duty); //电机，
      break;
    default:
      break;
    }
  }
}
void Servo_Duty(uint32 duty)
{
  if (duty > SERVO_MIDDLE + SERVO_RANGE)
  {
    duty = SERVO_MIDDLE + SERVO_RANGE;
  }
  else if (duty < SERVO_MIDDLE - SERVO_RANGE)
  {
    duty = SERVO_MIDDLE - SERVO_RANGE;
  }
  //归一化为2500--12500
  //if((duty>2500)&&(duty<12500))
  //{
  pwm_duty(PWM4_MODULE2_CHA_C30, duty);
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
