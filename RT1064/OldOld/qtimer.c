#include "headfile.h"





void qtimer_AB_init(void)
{
     //初始化 QTIMER_1 A相使用QTIMER1_TIMER0_D0 B相使用QTIMER1_TIMER1_D1
    qtimer_quad_init(QTIMER_1,QTIMER1_TIMER0_C0,QTIMER1_TIMER1_C1);
    //初始化 QTIMER_1 A相使用QTIMER1_TIMER2_D2 B相使用QTIMER1_TIMER3_D3
    qtimer_quad_init(QTIMER_1,QTIMER1_TIMER2_C2,QTIMER1_TIMER3_C24);
}


