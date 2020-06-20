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
    qtimer_quad_init(QTIMER_1,QTIMER1_TIMER0_C0,QTIMER1_TIMER1_C1);
    //��ʼ�� QTIMER_1 A��ʹ��QTIMER1_TIMER2_D2 B��ʹ��QTIMER1_TIMER3_D3
    qtimer_quad_init(QTIMER_1,QTIMER1_TIMER2_C2,QTIMER1_TIMER3_C24);
}

/*************************************************************************
*  �������ƣ�void Get_Speed(void) 
*  ����˵������ȡ������ٶ�
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020.06.20
*  ��    ע��
*************************************************************************/
void Get_Speed(void) 
{
  int16 qd1_result;
  int16 qd2_result;

  static float Speed10 = 0, Speed11 = 0, Speed12 = 0, Speed13 = 0;
  static float Speed20 = 0, Speed21 = 0, Speed22 = 0, Speed23 = 0;

  qd1_result = -qtimer_quad_get(QTIMER_1, QTIMER1_TIMER0_C0);
  qd2_result = qtimer_quad_get(QTIMER_1, QTIMER1_TIMER2_C2);
  qtimer_quad_clear(QTIMER_1, QTIMER1_TIMER0_C0);
  qtimer_quad_clear(QTIMER_1, QTIMER1_TIMER2_C2);

  CarSpeed1 = qd1_result / (5.7 * PIT_TIME);
  CarSpeed2 = qd2_result / (5.7 * PIT_TIME);

  Speed13 = Speed12;
  Speed12 = Speed11;
  Speed11 = Speed10;
  Speed10 = CarSpeed1;
  Speed23 = Speed22;
  Speed22 = Speed21;
  Speed21 = Speed20;
  Speed20 = CarSpeed2;
  CarSpeed1 = (Speed10 + Speed11 + Speed12 + Speed13) / 4;
  CarSpeed2 = (Speed20 + Speed21 + Speed22 + Speed23) / 4;
}























//�õ�����״̬
char get_diff_state(void){
  return diff_state;
}
//���ز���
void diff_on(void){
  diff_state = DIFF_ON_VAL;
}

void diff_off(void){
  diff_state = DIFF_OFF_VAL;
}
