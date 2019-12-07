#include "headfile.h"


//PID控制类变量
//PID PID_SPEED,PID_TURN,PID_SPEED1,PID_SPEED2;


//电机输出量 
// float  MotorOut;   
float MotorOut1,MotorOut2;

int16 qd1_result;
int16 qd2_result;
float CarSpeed1=0,CarSpeed2=0;
/////////////////////////////////////////////////////////解码器获取速度  未修正好


/********************************
 * 摄像头变量
 ********************************/
float Cam_Turn_Control;
float Turn_Cam_P = 2;
float Turn_Cam_D = 1.5;




float Cam_Offset_Table0[15] = {-192, -160, -130, -90, -60, -40, -20, 0, 20, 40, 60, 90, 130, 160, 192};
//float Turn_Cam_P_Table0[15] = {0.625,   0.719,    0.73,   0.56,   0.32160,    0.18,   0.1,   0.08,   0.1,   0.18,    0.32,    0.56,     0.73,   0.719,   0.625 };
//*float Turn_Cam_P_Table0[15] = {0.95,   1.05,    0.8,     0.64,    0.48,     0.28,   0.18,    0.06,   0.18,   0.28,     0.48,      0.64,      0.8,   1.05,   0.95 };
//float Turn_Cam_P_Table0[15] = {0.95,   1.05,    0.8,     0.64,    0.48,     0.38,   0.28,    0.06,   0.28,   0.38,     0.48,      0.64,      0.8,   1.05,   0.95 };
float Turn_Cam_P_Table0[15] = {0.95, 1.05, 0.9, 0.8, 0.68, 0.28, 0.18, 0.06, 0.18, 0.28, 0.68, 0.8, 0.9, 1.05, 0.95};

//float Turn_Cam_P_Table0[15] = {0.85,   0.95,    0.7,     0.56,    0.2,     0.1,   0.08,    0.06,   0.08,   0.1,     0.2,      0.56,      0.7,   0.95,   0.85 };
//float Turn_D_Cam_Table0[15] = {0.65,   0.85,   0.89,    1,      1.3,      1.35,   1.5,   0.25,    1.5,    1.35,   1.3,    1,    0.89,    0.85,    0.65};//{0.432,  0.46,     0.476,   0.356,    0.80,    0.130,   0.056,    0.01,  0.056,    0.13,  0.26,     0.356,      0.476,     0.46,      0.432};
float Turn_Cam_D_Table0[15] = {0.65, 0.85, 0.89, 1.4, 1.8, 2.0, 2.3, 0.2, 2.3, 2.0, 1.8, 1.4, 0.89, 0.85, 0.65}; //{0.432,  0.46,     0.476,   0.356,    0.80,    0.130,   0.056,    0.01,  0.056,    0.13,  0.26,     0.356,      0.476,     0.46,      0.432};



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


/*********************************
转弯PD模糊函数------摄像头控制
输入参数：摄像头计算偏差值
输出参数：摄像头控制转弯PD
***********************************/

void TurnFuzzyPD_Cam(void)
{
  int i = 0;
  /***********fuzzy***********/
  // if (1)
  //{
  for (i = 0; i < 14; i++)
  {
    if (Cam_offset >= Cam_Offset_Table0[i] && Cam_offset < Cam_Offset_Table0[i + 1])
    {
      Turn_Cam_P = Turn_Cam_P_Table0[i] + (Cam_offset - Cam_Offset_Table0[i]) * (Turn_Cam_P_Table0[i + 1] - Turn_Cam_P_Table0[i]) / (Cam_Offset_Table0[i + 1] - Cam_Offset_Table0[i]); //线性
      Turn_Cam_D = Turn_Cam_D_Table0[i] + (Cam_offset - Cam_Offset_Table0[i]) * (Turn_Cam_D_Table0[i + 1] - Turn_Cam_D_Table0[i]) / (Cam_Offset_Table0[i + 1] - Cam_Offset_Table0[i]);
      break;
    }
  }

  if (Cam_offset <= Cam_Offset_Table0[0])
  {
    Turn_Cam_P = Turn_Cam_P_Table0[0];
    Turn_Cam_D = Turn_Cam_D_Table0[0];
  }
  else if (Cam_offset >= Cam_Offset_Table0[14])
  {
    Turn_Cam_P = Turn_Cam_P_Table0[14];
    Turn_Cam_D = Turn_Cam_D_Table0[14];
  }
  // }
}

/*********************************************
***摄像头转弯控制程序，根据中心偏移量计算舵机输出量
***输入参数：车身速度，中心偏移量
***输出参数：左轮速度，右轮速度
***说明：偏移量offset为负说明车身相对赛道中心偏左
         偏移量offset为正说明车身相对赛道中心偏右
*********************************************/
void Turn_Cam()
{
  static float Cam_offset_old = 0;
  TurnFuzzyPD_Cam();
  
  //0.768=0.8*1.2*0.8
  Turn_Cam_P *= 0.768; //0.85;//0.7
  Turn_Cam_D *= 0.768; //6.5

  Turn_Cam_Out = Turn_Cam_P * Cam_offset + Turn_Cam_D * (Cam_offset - Cam_offset_old); //转向PD控制(Kp*C+Kd*delta C)


  Cam_offset_old = Cam_offset;

  // Servo_Duty((uint32)(sever_middle - 0.8 * Turn_Cam_Out));
  Servo_Duty(-Turn_Cam_Out);
}
