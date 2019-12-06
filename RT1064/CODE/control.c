#include "headfile.h"


//PID控制类变量
//PID PID_SPEED,PID_TURN,PID_SPEED1,PID_SPEED2;


//电机输出量 
float  MotorOut;   
float MotorOut1,MotorOut2;

int16 qd1_result;
int16 qd2_result;
float CarSpeed1=0,CarSpeed2=0;
/////////////////////////////////////////////////////////解码器获取速度  未修正好

void Get_Speed()                     //获取电机的速度
{  
  
	static float Speed10 = 0,Speed11 = 0,Speed12 = 0,Speed13 = 0;
	static float Speed20 = 0,Speed21 = 0,Speed22 = 0,Speed23 = 0;
        
	qd1_result =  -qtimer_quad_get(QTIMER_1,QTIMER1_TIMER0_C0);
	qd2_result =  qtimer_quad_get(QTIMER_1,QTIMER1_TIMER2_C2);
        qtimer_quad_clear(QTIMER_1,QTIMER1_TIMER0_C0);
        qtimer_quad_clear(QTIMER_1,QTIMER1_TIMER2_C2);
        
	CarSpeed1=qd1_result/5700.0*500;
	CarSpeed2=qd2_result/5700.0*500;
        
	Speed13 = Speed12;Speed12 = Speed11;Speed11 = Speed10;Speed10 = CarSpeed1;
	Speed23 = Speed22;Speed22 = Speed21;Speed21 = Speed20;Speed20 = CarSpeed2;
	CarSpeed1 = (Speed10 + Speed11 + Speed12 + Speed13)/4;
	CarSpeed2 = (Speed20 + Speed21 + Speed22 + Speed23)/4;
        

}



void Moto_Out() 
{     
  	
/*
 
	//速度控制输出限幅
	if(PID_SPEED1.OUT>18000)//如果车子前倾，则车模的速度控制输出为正，反之为负
		PID_SPEED1.OUT=18000;
	if(PID_SPEED1.OUT<-18000)
		PID_SPEED1.OUT=-18000;
	if(PID_SPEED2.OUT>18000)//如果车子前倾，则车模的速度控制输出为正，反之为负
		PID_SPEED2.OUT=18000;
	if(PID_SPEED2.OUT<-18000)
		PID_SPEED2.OUT=-18000;

        MotorOut1=PID_SPEED1.OUT;
	MotorOut2=PID_SPEED2.OUT;
        

*/


        
        if(MotorOut1>=0) //正转
	{
		Motor_Duty(3, (uint32)(MotorOut1/100)*100);
		Motor_Duty(2, 0);
	}
	else   //反转
	{
		Motor_Duty(3, 0);
		Motor_Duty(2, (uint32)(-MotorOut1/100)*100);
	}        
        
        if(MotorOut2>=0)
	{       Motor_Duty(1, (uint32)(MotorOut2/100)*100);
		Motor_Duty(0, 0);

	}
	else{
                Motor_Duty(0, (uint32)(-MotorOut2/100)*100);
		Motor_Duty(1, 0);

	}

	
	
	
}
