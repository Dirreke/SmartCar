#include "headfile.h"

void Motor_Init(void)
{
  //��ʼ��PWM1 MODULE0 ��ͨ��B ����ΪB23 Ƶ��50hz ռ�ձ�Ϊ �ٷ�֮100*5000/PWM_DUTY_MAX   PWM_DUTY_MAX��fsl_pwm.h�ļ��� Ĭ��Ϊ50000
  //ÿһ��ͨ��ֻ����һ���������PWM
  //���ռ�ձ�ֵPWM_DUTY_MAX ������fsl_pwm.h�ļ����޸� �Ѹ�Ϊ20000

  pwm_init(PWM1_MODULE3_CHB_D1, 13000, 0);
  pwm_init(PWM1_MODULE3_CHA_D0, 13000, 0);
  pwm_init(PWM1_MODULE0_CHB_D13, 13000, 0);
  pwm_init(PWM1_MODULE0_CHA_D12, 13000, 0);
}

//�������   1.5ms/20ms * PWM_DUTY_MAX��PWM_DUTY_MAX��PWM����ռ�ձ�ʱ���ֵ�� PWM_DUTY_MAX��fsl_pwm.h�ļ��� �Ѹ�Ϊ20000
void Servo_Init(void)
{
  pwm_init(PWM4_MODULE2_CHA_C30, 50, SERVO_MIDDLE); //SERVOMIDDLE ��Properties�ж���
}

void Motor_Duty(uint16 Motno, uint32 duty)
{
  //��һ��Ϊ0--500--1000
  //if(duty<1001)
  {
    switch (Motno)
    {
    case Mot0:
      pwm_duty(PWM1_MODULE3_CHA_D0, duty); //�����
      break;
    case Mot1:
      pwm_duty(PWM1_MODULE3_CHB_D1, duty); //�����
      break;
    case Mot2:
      pwm_duty(PWM1_MODULE0_CHA_D12, duty); //�����
      break;
    case Mot3:
      pwm_duty(PWM1_MODULE0_CHB_D13, duty); //�����
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
  //��һ��Ϊ2500--12500
  //if((duty>2500)&&(duty<12500))
  //{
  pwm_duty(PWM4_MODULE2_CHA_C30, duty);
  /*
        //������λ�ö��λ��   ��0.5ms - 2.5ms��ms/20ms * 50000��50000��PWM����ռ�ձ�ʱ���ֵ��
        //�����СֵΪ1250   ���ֵΪ6250
        //�����һ�� ��һ����Ϊ����Сת�������򣬲�Ҫת�����ڼ���λ�ã��еĶ���ڼ���λ�����׿�ס
        duty += 10;
        if((6250-100) < duty) duty = (1250+100);
        
        //���ƶ���ﵽָ��λ��
        pwm_duty(S_MOTOR_PIN,duty);
        
        if((1250+100) >= duty)  systick_delay_ms(1000); //����6250��λ��   ��ת��1250��λ��ʱ   Ӧ��ʹ�ýϳ�����ʱ�ȵ�����ﵽָ��λ��
        else                    systick_delay_ms(3);    //

*/
  //}
}
