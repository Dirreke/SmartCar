#include "headfile.h"
#include "math.h"
extern char get_diff_state(void);

//PID���������
PID PID_SPEED, PID_TURN, PID_SPEED1, PID_SPEED2;

/***********************************************************
����ͷת����Ʋ��ֱ���
************************************************************/

#define cam_offset_range 235

//�������� 
float MotorOut;
float MotorOut1, MotorOut2;

int16 qd1_result;
int16 qd2_result;
float CarSpeed1 = 0, CarSpeed2 = 0;
/////////////////////////////////////////////////////////????????????  ��??????

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

void Get_Speed() //��ȡ������ٶ�
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
������n������lower-higher֮��
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
***����������Ƕȣ��ȣ�
***���������P
***˵����
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
  float angle_val; // ������ʾʵ��ת��Ƕ�
  float diff_K0;   // ������=���ٱȾ��٣������ָ�һ��
#endif

  //���ٿ��� ������궨����DIFF_ON���͸��ݲ��ټ���p������pΪ0
#ifdef DIFF0
#ifdef DIFF_ON
  if (get_diff_state() == DIFF_ON_VAL)
  {
    //���ز�����Para�ж���

    angle_local = -((int)(Cam_offset)*45) / cam_offset_range;
    angle_local = (int)my_limit(angle_local, 45); //ͨ������ͷ����õ���ת��ƫ��ǵõ����޵�ת���

    lib_active_diff_input(angle_local);       //ͨ������pid �õ�������active_diff_val
    p = 1.0 * p_get((float)abs(angle_local)); //�������p
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
  {                                                         //��ת
    speedTarget1 = lib_get_speed(LIB_TIRE_LEFT) * (1 + p);  //��೵��
    speedTarget2 = lib_get_speed(LIB_TIRE_RIGHT) * (1 - p); //�Ҳ೵��
  }
  else
  {
    speedTarget1 = lib_get_speed(LIB_TIRE_LEFT) * (1 - p);  //��೵��
    speedTarget2 = lib_get_speed(LIB_TIRE_RIGHT) * (1 + p); //�Ҳ೵��
  }
#endif

#ifdef DIFF1
#ifdef DIFF_ON
  if (get_diff_state() == DIFF_ON_VAL)
  {
    //���ز�����Para�ж���

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
  speedTarget1 = get_speed() * (1 + diff_K0 / 2); //��೵��
  speedTarget2 = get_speed() * (1 - diff_K0 / 2); //�Ҳ೵��

#endif

  /////б�¿��ƣ���ʱ��û�м���
  // lib_speed_utility();

  ///////////////////////////////////////////////
  //PID�㷨��ʼ
  ///////////////////////////////////////////////

  //speedTarget1�ǲ����趨���ٶ�ֵ��setspeed��ֱ���趨��ֵ�������������ԣ������ٺ���ò�����
  //P����
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

//�ٶȿ���
void Speed_Control_Output(void) //2ms����һ��
{
  //	float fValue1,fValue2;
  PID_SPEED1.OUT += SpeedControlOutNew1;
  PID_SPEED2.OUT += SpeedControlOutNew2;
}

void Moto_Out()
{

  //�ٶȿ�������޷�
  if (PID_SPEED1.OUT > 18000) //�������ǰ�㣬��ģ���ٶȿ������Ϊ������֮Ϊ��
    PID_SPEED1.OUT = 18000;
  if (PID_SPEED1.OUT < -18000)
    PID_SPEED1.OUT = -18000;
  if (PID_SPEED2.OUT > 18000) //�������ǰ�㣬��ģ���ٶȿ������Ϊ������֮Ϊ��
    PID_SPEED2.OUT = 18000;
  if (PID_SPEED2.OUT < -18000)
    PID_SPEED2.OUT = -18000;

  MotorOut1 = PID_SPEED1.OUT;
  MotorOut2 = PID_SPEED2.OUT;

  if (MotorOut1 >= 0) //��ת
  {
    Motor_Duty(3, 0);
    Motor_Duty(2, (uint32)(MotorOut1 / 100) * 100);
  }
  else //��ת
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
ת��PDģ������------����ͷ����
�������������ͷ����ƫ��ֵ
�������������ͷ����ת��PD
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
      Turn_Cam_P = Turn_Cam_P_Table0[i] + (Cam_offset - Cam_Offset_Table0[i]) * (Turn_Cam_P_Table0[i + 1] - Turn_Cam_P_Table0[i]) / (Cam_Offset_Table0[i + 1] - Cam_Offset_Table0[i]); //����
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
***����ͷת����Ƴ��򣬸�������ƫ���������������
***��������������ٶȣ�����ƫ����
***��������������ٶȣ������ٶ�
***˵����ƫ����offsetΪ��˵�����������������ƫ��
         ƫ����offsetΪ��˵�����������������ƫ��
*********************************************/
void Turn_Cam()
{
  static float Cam_offset_old = 0;
  TurnFuzzyPD_Cam();

  //0.768=0.8*1.2*0.8
  Turn_Cam_P *= 0.768; //0.85;//0.7
  Turn_Cam_D *= 0.768; //6.5

  Turn_Cam_Out = Turn_Cam_P * Cam_offset + Turn_Cam_D * (Cam_offset - Cam_offset_old); //ת��PID����

  Cam_offset_old = Cam_offset;

  // Servo_Duty((uint32)(sever_middle - 0.8 * Turn_Cam_Out));
  Servo_Duty(-Turn_Cam_Out);
}
