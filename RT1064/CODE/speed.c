#include "headfile.h"

float CarSpeed1 = 0, CarSpeed2 = 0;
float SpeedGoal = 0;

/*************************************************************************
*  �������ƣ�void qtimer_AB_init(void)
*  ����˵������������ʼ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020.06.20
*  ��    ע��
*************************************************************************/
void qtimer_AB_init(void)
{
  //��ʼ�� QTIMER_1 A��ʹ��QTIMER1_TIMER0_D0 B��ʹ��QTIMER1_TIMER1_D1
  qtimer_quad_init(QTIMER_1, QTIMER1_TIMER0_C0, QTIMER1_TIMER1_C1);
  //��ʼ�� QTIMER_1 A��ʹ��QTIMER1_TIMER2_D2 B��ʹ��QTIMER1_TIMER3_D3
  qtimer_quad_init(QTIMER_1, QTIMER1_TIMER2_C2, QTIMER1_TIMER3_C24);
}

/*************************************************************************
*  �������ƣ�void Get_Speed(void) 
*  ����˵������ȡ������ٶ�
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020.06.20
*  ��    ע��
*************************************************************************/
float CarSpeed;
void Get_Speed(void)
{
  int16 qd1_result;
  int16 qd2_result;

  /*   static float Speed10 = 0, Speed11 = 0, Speed12 = 0, Speed13 = 0;
  static float Speed20 = 0, Speed21 = 0, Speed22 = 0, Speed23 = 0; */

  qd1_result = -qtimer_quad_get(QTIMER_1, QTIMER1_TIMER0_C0);
  qd2_result = qtimer_quad_get(QTIMER_1, QTIMER1_TIMER2_C2);
  qtimer_quad_clear(QTIMER_1, QTIMER1_TIMER0_C0);
  qtimer_quad_clear(QTIMER_1, QTIMER1_TIMER2_C2);

  CarSpeed1 = qd1_result * QTIMER_PIT_SCALE;// / (11.4 * PIT_TIME);//(5.7 * PIT_TIME );
  CarSpeed2 = qd2_result * QTIMER_PIT_SCALE;// / (11.4 * PIT_TIME); 1/22.8

  CarSpeed1 = speed_mean_filter1(CarSpeed1);
  CarSpeed2 = speed_mean_filter2(CarSpeed2);
  CarSpeed = (CarSpeed1 + CarSpeed2) * 0.5;
  /*   Speed13 = Speed12;
  Speed12 = Speed11;
  Speed11 = Speed10;
  Speed10 = CarSpeed1;
  Speed23 = Speed22;
  Speed22 = Speed21;
  Speed21 = Speed20;
  Speed20 = CarSpeed2;
  CarSpeed1 = (Speed10 + Speed11 + Speed12 + Speed13) / 4;
  CarSpeed2 = (Speed20 + Speed21 + Speed22 + Speed23) / 4; */
}
/*************************************************************************
*  �������ƣ�float speed_mean_filter(float D_new) 1��2��
*  ����˵������ֵ�˲�
*  ����˵����������
*  �������أ��˲����
*  �޸�ʱ�䣺2020.06.29
*  ��    ע��
*************************************************************************/
float speed_mean_filter1(float D_new)
{
  static float D[FILTER_ARRAY_SIZE];
  static int num = 0;
  static float sum = 0;

  if (num < FILTER_ARRAY_SIZE)
  {
    D[num] = D_new;
    ++num;
    sum += D_new;
    return sum / num;
  }
  else
  {
    sum += D_new;
    num = num % FILTER_ARRAY_SIZE + FILTER_ARRAY_SIZE;
    sum -= D[num - FILTER_ARRAY_SIZE];
    D[num - FILTER_ARRAY_SIZE] = D_new;
    num++;
    return sum * FILTER_ARRAY_SIZE_INVERSE;
  }
}

float speed_mean_filter2(float D_new)
{
  static float D[FILTER_ARRAY_SIZE];
  static int num = 0;
  static float sum = 0;

  if (num < FILTER_ARRAY_SIZE)
  {
    D[num] = D_new;
    ++num;
    sum += D_new;
    return sum / num;
  }
  else
  {
    sum += D_new;
    num = num % FILTER_ARRAY_SIZE + FILTER_ARRAY_SIZE;
    sum -= D[num - FILTER_ARRAY_SIZE];
    D[num - FILTER_ARRAY_SIZE] = D_new;
    num++;
    return sum * FILTER_ARRAY_SIZE_INVERSE;
  }
}

void lib_speed_set(float a)
{
  if (a - SpeedGoal >= 0.1 || a - SpeedGoal <= -0.1)
  {
    speed_change_flag = 1;
  }
  SpeedGoal = a;
  // speed_change_flag = 1;
}

//�õ�����״̬
char get_diff_state(void)
{
  return diff_state;
}

//���ز���
void diff_on(void)
{
  diff_state = DIFF_ON_VAL;
}

void diff_off(void)
{
  diff_state = DIFF_OFF_VAL;
}
