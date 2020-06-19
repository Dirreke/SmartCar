#include "headfile.h"
#include "math.h"
extern char get_diff_state(void);
//���
int EM_Road;
float EM_Turn_Control;

//PID���������
PID PID_SPEED, PID_TURN, PID_SPEED1, PID_SPEED2;

/***********************************************************
����ͷת����Ʋ��ֱ���
************************************************************/

#define Cam_offset_range 235

//��������
// float MotorOut;
float MotorOut1, MotorOut2;

float CarSpeed1 = 0, CarSpeed2 = 0;
/////////////////////////////////////////////////////////????????????  ��??????

/********************************
 * PD
 ********************************/
float Turn_Cam_Out;
float Turn_Cam_P = 0.85;
float Turn_Cam_D = 0.8;

float Turn_EM_Out;
float Turn_EM_P = 0.8;
float Turn_EM_D = 0.8;

// float Cam_Offset_Table0[15] = {-192, -160, -130, -90, -60, -40, -20, 0, 20, 40, 60, 90, 130, 160, 192};
//float Cam_Offset_Table0[15] = {-300, -250, -203, -141, -94, -63, -31, 0, 31, 63, 94, 141, 203, 250, 300};
//float Turn_Cam_P_Table0[15] = {0.625,   0.719,    0.73,   0.56,   0.32160,    0.18,   0.1,   0.08,   0.1,   0.18,    0.32,    0.56,     0.73,   0.719,   0.625 };
//*float Turn_Cam_P_Table0[15] = {0.95,   1.05,    0.8,     0.64,    0.48,     0.28,   0.18,    0.06,   0.18,   0.28,     0.48,      0.64,      0.8,   1.05,   0.95 };
//float Turn_Cam_P_Table0[15] = {0.95,   1.05,    0.8,     0.64,    0.48,     0.38,   0.28,    0.06,   0.28,   0.38,     0.48,      0.64,      0.8,   1.05,   0.95 };
// float Turn_Cam_P_Table0[15] = {0.95, 1.05, 0.9, 0.8, 0.68, 0.28, 0.18, 0.06, 0.18, 0.28, 0.68, 0.8, 0.9, 1.05, 0.95};
//new float ===P=== {1.60, }
//float Turn_Cam_P_Table0[15] = {0.85,   0.95,    0.7,     0.56,    0.2,     0.1,   0.08,    0.06,   0.08,   0.1,     0.2,      0.56,      0.7,   0.95,   0.85 };
//float Turn_D_Cam_Table0[15] = {0.65,   0.85,   0.89,    1,      1.3,      1.35,   1.5,   0.25,    1.5,    1.35,   1.3,    1,    0.89,    0.85,    0.65};//{0.432,  0.46,     0.476,   0.356,    0.80,    0.130,   0.056,    0.01,  0.056,    0.13,  0.26,     0.356,      0.476,     0.46,      0.432};
// float Turn_Cam_D_Table0[15] = {0.65, 0.85, 0.89, 1.4, 1.8, 2.0, 2.3, 0.2, 2.3, 2.0, 1.8, 1.4, 0.89, 0.85, 0.65}; //{0.432,  0.46,     0.476,   0.356,    0.80,    0.130,   0.056,    0.01,  0.056,    0.13,  0.26,     0.356,      0.476,     0.46,      0.432};

float Cam_Offset_Table0[23] = {-140, -130, -110, -100, -80, -60, -50, -40, -30, -20, 0, 20, 30, 40, 50, 60, 80, 100, 110, 130, 140};
// float Turn_Cam_P_Table0[23] = {1.29, 1.20, 1.15, 1.20, 1.20, 1.30, 1.35, 1.60, 1.5, 1.45, 1.45, 1.45, 1.5, 1.60, 1.35, 1.30, 1.20, 1.20, 1.15, 1.20, 1.29};
float Turn_Cam_P_Table0[23] = {1.29, 1.20, 1.15, 1.20, 1.20, 1.30, 1.35, 1.60, 1.5, 1.45, 0.3,1.45, 1.5, 1.60, 1.35, 1.30, 1.20, 1.20, 1.15, 1.20, 1.29};
float Turn_Cam_D_Table0[23] = {1.29, 1.20, 1.15, 1.20, 1.20, 1.30, 1.35, 1.60, 1.5, 1.45, 0.01, 1.45, 1.5, 1.60, 1.35, 1.30, 1.20, 1.20, 1.15, 1.20, 1.29};

float Turn_P=0.85;
float Turn_D=0.8;
void Get_Speed() //��ȡ������ٶ�
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
#ifdef DIFF0
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
#endif
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

    angle_local = -((int)(Cam_offset)*45) / Cam_offset_range;
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
    Motor_Duty(3, (uint32)(-MotorOut1 / 100) * 100);
    Motor_Duty(2, 0); //(uint32)(-MotorOut1/100)*100);
  }

  if (MotorOut2 >= 0)
  {
    Motor_Duty(1, 0);
    Motor_Duty(0, (uint32)(MotorOut2 / 100) * 100);
  }
  else
  {
    Motor_Duty(0, (uint32)(-MotorOut2 / 100) * 100); //(uint32)(-MotorOut2/100)*100);
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
  Turn_Cam_P *= Turn_P; //0.85;//0.7
  Turn_Cam_D *= Turn_D;

  Turn_Cam_Out = Turn_Cam_P * Cam_offset+ Turn_Cam_D * (Cam_offset - Cam_offset_old); //ת��PID����

  Cam_offset_old = Cam_offset;
  if (Road == 1 && Road1_flag == 5)
  {
    if (Turn_Cam_Out > -0.5 * SERVO_RANGE)
    {
      Turn_Cam_Out = -0.5 * SERVO_RANGE;
    }
  }
  else if (Road == 2 && Road2_flag == 5)
  {
    if (Turn_Cam_Out < 0.5 * SERVO_RANGE)
    {
      Turn_Cam_Out = 0.5 * SERVO_RANGE;
    }
  }
  // Servo_Duty((uint32)(sever_middle - 0.8 * Turn_Cam_Out));
  Servo_Duty(-Turn_Cam_Out);
}

/*********************************
ת��PDģ������------��ſ���
�����������ż���ƫ��ֵ
�����������ſ���ת��PD
***********************************/
/*
void TurnFuzzyPD_EM(void)
{
  int i=0;

         if(EM_Road==4)
        {
            if(EM_offset <= EM_Offset_Table[0])
              {
                Turn_P_EM = Turn_P_EM_Table0[0];
                Turn_D_EM = Turn_D_EM_Table0[0];
                return;
              }
              else if(EM_offset >= EM_Offset_Table[14])
              {
                Turn_P_EM = Turn_P_EM_Table0[14];
                Turn_D_EM = Turn_D_EM_Table0[14];
                return;
              }
            
        }
        else
        {
          if(EM_offset <= EM_Offset_Table[0])
              {
                Turn_P_EM = Turn_P_EM_Table1[0];
                Turn_D_EM = Turn_D_EM_Table1[0];
                return;
              }
              else if(EM_offset >= EM_Offset_Table[14])
              {
                Turn_P_EM = Turn_P_EM_Table1[14];
                Turn_D_EM = Turn_D_EM_Table1[14];
                return;
              }                                  
      for(i=0;i<14;i++)
      {
       
        }
          if(EM_offset >= EM_Offset_Table[i] && EM_offset < EM_Offset_Table[i+1])
          {
            if(EM_Road==4){  //���
              Turn_P_EM=Turn_P_EM_Table0[i]+(EM_offset-EM_Offset_Table[i])
                        *(Turn_P_EM_Table0[i+1]-Turn_P_EM_Table0[i])
                         /(EM_Offset_Table[i+1]-EM_Offset_Table[i]);
              Turn_D_EM=Turn_D_EM_Table0[i]+(EM_offset - EM_Offset_Table[i])
                      *(Turn_D_EM_Table0[i+1] - Turn_D_EM_Table0[i])
                       /(EM_Offset_Table[i+1] - EM_Offset_Table[i]);
            
              } 
            
            else{               //ֱ��
              Turn_P_EM=Turn_P_EM_Table1[i]+(EM_offset-EM_Offset_Table[i])
                        *(Turn_P_EM_Table1[i+1]-Turn_P_EM_Table1[i])
                         /(EM_Offset_Table[i+1]-EM_Offset_Table[i]);
              Turn_D_EM=Turn_D_EM_Table1[i]+(EM_offset - EM_Offset_Table[i])
                      *(Turn_D_EM_Table1[i+1] - Turn_D_EM_Table1[i])
                       /(EM_Offset_Table[i+1] - EM_Offset_Table[i]);
              
            
            }
            
          }
      }
    

}

*/
/*********************************************
***���ת����Ƴ��򣬸�������ƫ������������������ٶ�
***��������������ٶȣ�����ƫ����
***��������������ٶȣ������ٶ�
***˵����ƫ����offsetΪ��˵�����������������ƫ��
         ƫ����offsetΪ��˵�����������������ƫ��
*********************************************/
float KP = 17;
void Turn_EM(void)
{

  //TurnFuzzyPD_EM();
  // TurnFuzzySIN();
  //Turn_P_EM = 0.8;
  // Turn_I_EM = 0;
  //Turn_D_EM = 0;
  //start of I
  //if(abs_f(EM_offset)<50){
  //EM_offset_I *= 0.8;
  //  //}
  //  else{
  //    EM_offset_I += EM_offset*0.00043;
  //  }
  //  EM_offset_I = limit_f(EM_offset_I,-1.5,1.5);        //�ۼ�I�޷�
  //
  //end of I

  //start of remapping EM_offset
  //float EM_offset_remap_val;
  //EM_offset_remap_val=EM_offset_remap(EM_offset);
  //end of remappping EM_offset

  //start of foc control
  //caculate angle and length
  float mid_err;
  float ctl_out;
  //EM_Value_3 = EM_Value_3 - EM_Value_2/1.88;
  //EM_Value_3 = EM_Value_3<0? 0 : EM_Value_3;
  //angle=EM_angle_get(limit_pos(EM_Value_1/1.5-EM_Value_2/3.5),EM_Value_2,EM_Value_3,limit_pos(EM_Value_4/1.5-EM_Value_3/3.5));//���ﻻ�㴹ֱ��У�ƽ�е����������GMYע//�ƺ�angleû���õ�����GMYע
  //angle=-1.2*197/45*angle;		//inner loop gain Kt
  //  mid_err=EM_length_err_get(EM_Value_2,EM_Value_3,EM_Value_1,EM_Value_4);//�ú���ԭ����û�õ�pl,pr,,,,,lm,lrʹ��EM_angle_get����ȫ�ֱ�����ģ���������ʱ�޸�Ϊ��һ�С���GMYע
  mid_err = EM_length_err_get(EM_Value_2, EM_Value_3, limit_pos(EM_Value_1 / 1.5 - EM_Value_2 / 3.5), limit_pos(EM_Value_4 / 1.5 - EM_Value_3 / 3.5)); //��ƽ�У���ƽ�У���ֱ���Ҵ�ֱ����GMYע
  mid_err = 0.2 / 3.1415926 * 180 * mid_err;                                                                                                           //outer loop gain Kl		//full range @ 70
  ctl_out = PD_section(mid_err);                                                                                                                       //+angle;		//inner loop error
  ctl_out = my_limit(ctl_out, 45);
  ctl_out = 196 / 45.0 * ctl_out; //inner loop gain

  if (Road == 3)
  {
    ctl_out /= 3;
  }
  //now using pi control of remapped EM_offset
  EM_Turn_Control = PI_section(ctl_out, EM_Value_1, EM_Value_4); //EM_offset_remap_val;//10*KP*EM_SIN_Control;//0.8 *EM_offset
                                                                 //+ Turn_D_EM*(EM_offset - old_EM_offset);  //ת��PID���� + Turn_I_EM*EM_offset_I
                                                                 // old_EM_offset = EM_offset;

  Servo_Duty((uint32)(EM_Turn_Control)); //disable EM control servo
}

/*********************************
���ƫ��ʸ������
���������EM_Val_2 EM_Val_3
�������: ��&|A|
�����ĸ����    pl------l&r------pr
���踺��Ϊ��ƫ������Ϊ��
***********************************/
float PD_section(float err)
{
  static float p = 1.22;
  static float d = 1.42;
  static float last = 0;

  float sub;
  sub = err - last;
  last = err;
  return (p * err + d * sub);
}

float my_fabs(float a)
{
  return a > 0 ? a : -a;
}

float limit_pos(float a)
{
  return a > 0 ? a : 0;
}

#if 0
static char is_smallest(float* fp){
  char i;
  for(i=0;i<4;i++){
    if(fp[4]>fp[4-i] || fp[4]>fp[4+i]) return 0;
  }
  return 1;
}

static char is_larger_than(float* fp,float a){
  char i,count;
  count=0;
  for(i=0;i<9;i++){
    if(fp[i]>a) count++;
  }
  return count;
}
#endif
/*
static char is_small(float a){
  static float peak = 4.0;
  static float array[9];
  //array[9] = array[8];
  array[8] = array[7];
  array[7] = array[6];
  array[6] = array[5];
  array[5] = array[4];
  array[4] = array[3];
  array[3] = array[2];
  array[2] = array[1];
  array[1] = array[0];
  array[0] = a;
  
  return is_smallest(array)&&(is_larger_than(array,peak)>2);
}
*/
static char EM_ring_stage;

char is_small_stage_report(void)
{
  return EM_ring_stage;
}
#if 0
//float angle_dbg_1,angle_dbg_2;//����debug�á���GMYע
int EM_angle_get(float lp,float l, float r,float rp)//Ӧ�üӸ�˥�����ߡ�д�ɾ���ź���ɣ����������ѡ���GMYע//�ƺ�û�õ�����GMYע
{
  char flag;		//identify is left
  float ratio;
  int angle;
 float lm,rm;		//l and r 's magnitude  
 /*float cos_angle;	//cos of angle   */
  lm=(float)sqrt(l*l+lp*lp);
  rm=(float)sqrt(rp*rp+r*r);/*������ע�������Ҽ���Ƕȵķ�ʽ����GMYע*/

  //EM_ring_stage=is_small(lm+rm);
  
  //stop
  //if(lm<0.2&&rm<0.2)
    //Road=7;
  
  if(lm>rm){
	ratio=lp/l;
/*	cos_angle=l/lm;*/
  }else{
	ratio=rp/r;
/*	cos_angle=r/rm;*/
  }
  
  /*angle_dbg_1 = l/lm;*/
  /*angle_dbg_2 = r/rm;*/
	  
  if(fabs(l-r)<0.05){
	return 0;
  }else if(r>l.3)//����Ӧ���ǳ���ʱ��ƽ�е�е�ADֵ����GMYע
  {
	flag=1;
  }else{
	flag=0;
  }
  
  //static float tan[45] = {};		//assume approximate tanx=x when angle is smaller than 45
  angle= (int)(flag? -45*ratio:45*ratio);
  return angle;
}
//��һ���� cmΪ��λ
#endif

float EM_mid = 0;

float EM_length_err_get(float l, float r, float pl, float pr)
{
  float err;
  //static float max_length = 11.5*1.414/10 ;
  //static float delta = 16*16*2/16;

  //remaints of the reverse runing EM

  //static float x_get[20]={0.0,0.01,0.05,0.12,0.27,0.31,0.42,0.49,0.54,0.60,0.67,0.73,0.75,0.79,0.81,0.83};

  //float length;
  //float err;

  /*length = (float)sqrt(l*l+r*r);
  
  if(length > max_length){
	return 0;
  }else{
	err = length/max_length;
	err = 1 - err;
	
	//remap err:0-1 to cm dimention
	for(int i=1;i<16;i++){
	  if(err<x_get[i]){		//err is at x_get[i-1]~x_get[i]
		err=i-1+(err-x_get[i-1])/(x_get[i]-x_get[i-1]);
		break;
	  }
	}*/

  //now calculate the actuall distance dA
  float lm, rm;    //l and r 's magnitude  //��ȫ�ֱ�����Ϊ�ֲ���������������EM_angle_get ��ͬ����EM_angle_getʹ��ֵ�ƺ�û���õ�������GMYע
  float cos_angle; //cos of angle
  lm = (float)sqrt(l * l + pl * pl);
  rm = (float)sqrt(pr * pr + r * r);

  if (lm > rm)
  {
    cos_angle = l / lm;
  }
  else
  {
    cos_angle = r / rm;
  }
  err = my_fabs((1.0 / lm - 1.0 / rm) / cos_angle);

  if (Road1_flag == 3)
  {
    err = -1.21 * (2.0 - l);
    return err;
  }

  if (r < 1.3) //car is near right side
    return err;
  else //car is near left side
    return -err;
}
/*********************************
PID������
���������error
�������: PID���
***********************************/

float PI_section(float err, float pl, float pr)
{
  static float p = 1.0;
  static float i = 0.000;
  static float d = 0.00;
  static float i_limit = 196 * 500; //assume input limit of 196, limit ratio of 1000		valid i 0.0001
  static float err_i = 0;
  static float err_last = 0;
  float err_d;
  err_d = err - err_last;
  err_last = err;

  if (pl > 0.6 && pr > 0.6 && Road1_flag != 3)
  {
    err_i = 0;
    err = 0;
    err_d = 0;
  }

  err_i += err;
  err_i = my_limit(err_i, i_limit);

  return (p * err + i * err_i + d * err_d);
}

#if 0
/*********************************
���ƫ����ӳ��
�����������ż���ƫ��ֵ
�����������ſ���ref--���
***********************************/ 
float EM_offset_remap(float a){
	const float pa=20.0;
	const float pb=270.0;
	//static float turn_limit=197.0;
	float temp;
	char flag;
	if(a>0){
	  flag=1;
	  temp=a;
	}else{
	  flag=0;
	  temp=-a;
	}
	if(temp<pa){
	  return 0;
	}else if(temp<pb){
	  return flag? (non_linear((temp-pa)/(pb-pa)))*SERVO_RANGE:-(non_linear((temp-pa)/(pb-pa)))*SERVO_RANGE;
	}else{
	  return flag? SERVO_RANGE:-SERVO_RANGE;
	}
}
/*********************************
���ƫ����ӳ��--������͹����
�����������ż���ƫ��ֵ����ֵ��һ�����:0-1
���������������0-1��ӳ��
***********************************/ 

float a_exp_n(float a){
  return 0.9*a+0.1*a*a;
  return 0.1*a+0.2*a*a+0.7*a*a*a;
}

float non_linear(float a){
  //linaer solution1
  return 1-a_exp_n(1-a);
  //s-function solution2
	if(a<1/2){
	  return (1/2-8*4*a_exp_n(1/2-a));
	}else{
	  return (1/2+8*4*a_exp_n(a-1/2));
	}
}

#endif
