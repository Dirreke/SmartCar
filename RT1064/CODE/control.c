#include "headfile.h"
#include "math.h"
extern char get_diff_state(void);

//PID控制类变量
PID PID_SPEED, PID_TURN, PID_SPEED1, PID_SPEED2;

/***********************************************************
摄像头转向控制部分变量
************************************************************/

#define cam_offset_range 235

//电机输出量 
float MotorOut;
float MotorOut1, MotorOut2;

int16 qd1_result;
int16 qd2_result;
float CarSpeed1 = 0, CarSpeed2 = 0;
/////////////////////////////////////////////////////////????????????  δ??????

/********************************
 * PD
 ********************************/
float Turn_Cam_Out;
float Turn_Cam_P = 2;
float Turn_Cam_D = 1.5;

float Cam_Offset_Table0[15] = {-192, -160, -130, -90, -60, -40, -20, 0, 20, 40, 60, 90, 130, 160, 192};
//float Cam_Offset_Table0[15] = {-300, -250, -203, -141, -94, -63, -31, 0, 31, 63, 94, 141, 203, 250, 300};
//float Turn_Cam_P_Table0[15] = {0.625,   0.719,    0.73,   0.56,   0.32160,    0.18,   0.1,   0.08,   0.1,   0.18,    0.32,    0.56,     0.73,   0.719,   0.625 };
//*float Turn_Cam_P_Table0[15] = {0.95,   1.05,    0.8,     0.64,    0.48,     0.28,   0.18,    0.06,   0.18,   0.28,     0.48,      0.64,      0.8,   1.05,   0.95 };
//float Turn_Cam_P_Table0[15] = {0.95,   1.05,    0.8,     0.64,    0.48,     0.38,   0.28,    0.06,   0.28,   0.38,     0.48,      0.64,      0.8,   1.05,   0.95 };
float Turn_Cam_P_Table0[15] = {0.95, 1.05, 0.9, 0.8, 0.68, 0.28, 0.18, 0.06, 0.18, 0.28, 0.68, 0.8, 0.9, 1.05, 0.95};

//float Turn_Cam_P_Table0[15] = {0.85,   0.95,    0.7,     0.56,    0.2,     0.1,   0.08,    0.06,   0.08,   0.1,     0.2,      0.56,      0.7,   0.95,   0.85 };
//float Turn_D_Cam_Table0[15] = {0.65,   0.85,   0.89,    1,      1.3,      1.35,   1.5,   0.25,    1.5,    1.35,   1.3,    1,    0.89,    0.85,    0.65};//{0.432,  0.46,     0.476,   0.356,    0.80,    0.130,   0.056,    0.01,  0.056,    0.13,  0.26,     0.356,      0.476,     0.46,      0.432};
float Turn_Cam_D_Table0[15] = {0.65, 0.85, 0.89, 1.4, 1.8, 2.0, 2.3, 0.2, 2.3, 2.0, 1.8, 1.4, 0.89, 0.85, 0.65}; //{0.432,  0.46,     0.476,   0.356,    0.80,    0.130,   0.056,    0.01,  0.056,    0.13,  0.26,     0.356,      0.476,     0.46,      0.432};

void Get_Speed() //获取电机的速度
{

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

float speedTarget1, speedTarget2;
float SetSpeed1 = 0, SetSpeed2 = 0;
float SpeedControlOutNew1 = 0;
float SpeedControlOutOld1 = 0;

float SpeedControlOutNew2 = 0;
float SpeedControlOutOld2 = 0;

int SpeedE, SpeedI1 = 0, SpeedI2 = 0, SpeedD, SpeedD_offset;
int SpeedE1, SpeedE2;
int OldE1, OldE2;
float Speed_kP1, Speed_kP2, Speed_kI1, Speed_kI2, Speed_kD, Speed_kD_offset;

/*0***********************************
将输入n限制在lower-higher之间
***********************************/
float limit_f(float n, float lower, float higher)
{
  if (n < lower)
  {
    return lower;
  }
  else if (n > higher)
  {
    return higher;
  }
  else
  {
    return n;
  }
}

float my_limit(float a, float limit)
{
  a = limit_f(a, -limit, limit);
  return a;
}
////////////////////////////////////////////

/*0********************************************
***
***输入参数：角度（度）
***输出参数：P
***说明：
           w
          ---
          2.0
 p= --------------------
     w          l
    --- + ------------
    2.0    angle(rad)

w=15,l=20
*********************************************/
float p_get(float angle)
{
  static float w = 15;
  static float l = 20;

  float p;

  p = (w / 2.0) / (w / 2.0 + l / (angle / 180.0 * 3.1415926));

  return p;
}

void Speed_Control(void)

{
#ifdef DIFF0
  float p;
  int angle_local;
#endif
/**GMY**/
#ifdef DIFF1
  float angle_val; // 用来表示实际转向角度
  float diff_K0;   // 差速率=差速比均速，左右轮各一半
#endif

  //差速控制 ，如果宏定义了DIFF_ON，就根据差速计算p，否则p为0
#ifdef DIFF0
#ifdef DIFF_ON
  if (get_diff_state() == DIFF_ON_VAL)
  {
    //开关差速在Para中定义

    angle_local = -((int)(Cam_offset)*45) / cam_offset_range;
    angle_local = (int)my_limit(angle_local, 45); //通过摄像头计算得到的转向偏差角得到设限的转向角

    lib_active_diff_input(angle_local);       //通过差速pid 得到反馈量active_diff_val
    p = 1.0 * p_get((float)abs(angle_local)); //计算比例p
  }
  else if (get_diff_state() == DIFF_OFF_VAL)
  {
    p = 0;
    lib_active_diff_input(0);
  }

#else
  p = 0;
#endif
  if (angle_local < 0)
  {                                                         //右转
    speedTarget1 = lib_get_speed(LIB_TIRE_LEFT) * (1 + p);  //左侧车轮
    speedTarget2 = lib_get_speed(LIB_TIRE_RIGHT) * (1 - p); //右侧车轮
  }
  else
  {
    speedTarget1 = lib_get_speed(LIB_TIRE_LEFT) * (1 - p);  //左侧车轮
    speedTarget2 = lib_get_speed(LIB_TIRE_RIGHT) * (1 + p); //右侧车轮
  }
#endif

#ifdef DIFF1
#ifdef DIFF_ON
  if (get_diff_state() == DIFF_ON_VAL)
  {
    //开关差速在Para中定义

    angle_val = Turn_Cam_Out / SERVO_RANGE * ANGLE_RANGE;
    diff_K0 = CAR_DIFF_K * tan(angle_val);
  }
  else if (get_diff_state() == DIFF_OFF_VAL)
  {
    diff_K0 = 0;
  }

#else
  diff_K0 = 0;
#endif
  speedTarget1 = get_speed() * (1 + diff_K0 / 2); //左侧车轮
  speedTarget2 = get_speed() * (1 - diff_K0 / 2); //右侧车轮

#endif

  /////斜坡控制，暂时还没有加上
  // lib_speed_utility();

  ///////////////////////////////////////////////
  //PID算法开始
  ///////////////////////////////////////////////

  //speedTarget1是差速设定的速度值，setspeed是直接设定的值，可以用来调试，开差速后就用不到了
  //P分量
  OldE1 = SpeedE1;
  OldE2 = SpeedE2;
  SpeedE1 = (int)((speedTarget1 - CarSpeed1) * 100.0);
  SpeedE2 = (int)((speedTarget2 - CarSpeed2) * 100.0);

  //kp
  Speed_kP1 = PID_SPEED.P;
  Speed_kP2 = PID_SPEED.P;
  //ki
  SpeedI1 = SpeedE1 - OldE1;
  SpeedI2 = SpeedE2 - OldE2;
  if (abs(SpeedE1) < 15)
    Speed_kI1 = 0;
  else
    Speed_kI1 = PID_SPEED.I;
  if (abs(SpeedE2) < 15)
    Speed_kI2 = 0;
  else
    Speed_kI2 = PID_SPEED.I;

  SpeedControlOutNew1 = (Speed_kP1 * SpeedI1 + Speed_kI1 * SpeedE1);
  SpeedControlOutNew2 = (Speed_kP2 * SpeedI2 + Speed_kI2 * SpeedE2);
}

//速度控制
void Speed_Control_Output(void) //2ms运行一次
{
  //	float fValue1,fValue2;
  PID_SPEED1.OUT += SpeedControlOutNew1;
  PID_SPEED2.OUT += SpeedControlOutNew2;
}

void Moto_Out()
{

  //速度控制输出限幅
  if (PID_SPEED1.OUT > 18000) //如果车子前倾，则车模的速度控制输出为正，反之为负
    PID_SPEED1.OUT = 18000;
  if (PID_SPEED1.OUT < -18000)
    PID_SPEED1.OUT = -18000;
  if (PID_SPEED2.OUT > 18000) //如果车子前倾，则车模的速度控制输出为正，反之为负
    PID_SPEED2.OUT = 18000;
  if (PID_SPEED2.OUT < -18000)
    PID_SPEED2.OUT = -18000;

  MotorOut1 = PID_SPEED1.OUT;
  MotorOut2 = PID_SPEED2.OUT;

  if (MotorOut1 >= 0) //正转
  {
    Motor_Duty(3, 0);
    Motor_Duty(2, (uint32)(MotorOut1 / 100) * 100);
  }
  else //反转
  {
    Motor_Duty(3, 0);
    Motor_Duty(2, 0); //(uint32)(-MotorOut1/100)*100);
  }

  if (MotorOut2 >= 0)
  {
    Motor_Duty(1, 0);
    Motor_Duty(0, (uint32)(MotorOut2 / 100) * 100);
  }
  else
  {
    Motor_Duty(0, 0); //(uint32)(-MotorOut2/100)*100);
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

  Turn_Cam_Out = Turn_Cam_P * Cam_offset + Turn_Cam_D * (Cam_offset - Cam_offset_old); //转向PID控制

  Cam_offset_old = Cam_offset;

  // Servo_Duty((uint32)(sever_middle - 0.8 * Turn_Cam_Out));
  Servo_Duty(-Turn_Cam_Out);
}
