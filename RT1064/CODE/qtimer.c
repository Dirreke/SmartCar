#include "headfile.h"





void qtimer_AB_init(void)
{
     //��ʼ�� QTIMER_1 A��ʹ��QTIMER1_TIMER0_D0 B��ʹ��QTIMER1_TIMER1_D1
    qtimer_quad_init(QTIMER_1,QTIMER1_TIMER0_C0,QTIMER1_TIMER1_C1);
    //��ʼ�� QTIMER_1 A��ʹ��QTIMER1_TIMER2_D2 B��ʹ��QTIMER1_TIMER3_D3
    qtimer_quad_init(QTIMER_1,QTIMER1_TIMER2_C2,QTIMER1_TIMER3_C24);
}


