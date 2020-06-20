#include "headfile.h"

float CarSpeed1 = 0, CarSpeed2 = 0;
float SpeedGoal = 0;


/*************************************************************************
*  函数名称：void qtimer_AB_init(void)
*  功能说明：编码器初始化
*  参数说明：无
*  函数返回：无
*  修改时间：2020.06.20
*  备    注：
*************************************************************************/
void qtimer_AB_init(void)
{
     //初始化 QTIMER_1 A相使用QTIMER1_TIMER0_D0 B相使用QTIMER1_TIMER1_D1
    qtimer_quad_init(QTIMER_1,QTIMER1_TIMER0_C0,QTIMER1_TIMER1_C1);
    //初始化 QTIMER_1 A相使用QTIMER1_TIMER2_D2 B相使用QTIMER1_TIMER3_D3
    qtimer_quad_init(QTIMER_1,QTIMER1_TIMER2_C2,QTIMER1_TIMER3_C24);
}

/*************************************************************************
*  函数名称：void Get_Speed(void) 
*  功能说明：获取电机的速度
*  参数说明：无
*  函数返回：无
*  修改时间：2020.06.20
*  备    注：
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























//得到差速状态
char get_diff_state(void){
  return diff_state;
}
//开关差速
void diff_on(void){
  diff_state = DIFF_ON_VAL;
}

void diff_off(void){
  diff_state = DIFF_OFF_VAL;
}
