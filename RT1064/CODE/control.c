#include "headfile.h"
float Turn_Cam_Out;
float Turn_EM_Out;
float Turn_Out;
float MotorOut1, MotorOut2;
float speedTarget1, speedTarget2;
float Turn_P_EM;
float Turn_D_EM;

float Turn_EM_Out1 = 0, Turn_EM_Out2 = 0, Turn_EM_Out = 0;
PID PID_CENTER_EM, PID_STRAIGHT_EM;

int mix_choice = 0; //1���Ϊ��С��ǣ�2���Ϊ����ǣ�3��Ų����ţ�0������ģ�

/*************************************************************************
*  �������ƣ�void Turn_Cam(void)
*  ����˵��������ͷת����Ƴ��򣬸�������ƫ���������������
*  ����˵��������ƫ����,PD 
*  �������أ�������
*  �޸�ʱ�䣺2020.6.20
*  ��    ע��ƫ����offsetΪ��˵�����������������ƫ��
             ƫ����offsetΪ��˵�����������������ƫ��
*************************************************************************/

PID PID_TURN_CAM_EXT;
void Turn_Cam(void)
{
  PID PID_TURN_CAM;
  static float Cam_offset_old = 0;
  PID_TURN_CAM = TurnFuzzyPD_Cam();

  //0.768=0.8*1.2*0.8
  PID_TURN_CAM.P *= PID_TURN_CAM_EXT.P; //0.85;//0.7
  PID_TURN_CAM.D *= PID_TURN_CAM_EXT.D;

  Turn_Cam_Out = PID_TURN_CAM.P * Cam_offset + PID_TURN_CAM.D * (Cam_offset - Cam_offset_old); //ת��PID����

  Cam_offset_old = Cam_offset;

  //Servo_Duty(-Turn_Cam_Out);
}
/*************************************************************************
*  �������ƣ�void TurnFuzzyPD_Cam(void)
*  ����˵����ת��PDģ������------����ͷ����
*  ����˵����PD ��
*  �������أ�����ͷ����ת��PD
*  �޸�ʱ�䣺2020.6.20
*  ��    ע��
*************************************************************************/
PID TurnFuzzyPD_Cam(void)
{
  PID PID_TURN_CAM;
  static const float Cam_Offset_Table0[21] = {-140, -130, -110, -100, -80, -60, -50, -40, -30, -20, 0, 20, 30, 40, 50, 60, 80, 100, 110, 130, 140};
  static const float Turn_Cam_P_Table0[21] = {1.29, 1.20, 1.15, 1.20, 1.20, 1.30, 1.35, 1.60, 1.5, 1.45, 0.3, 1.45, 1.5, 1.60, 1.35, 1.30, 1.20, 1.20, 1.15, 1.20, 1.29};
  static const float Turn_Cam_D_Table0[21] = {1.29, 1.20, 1.15, 1.20, 1.20, 1.30, 1.35, 1.60, 1.5, 1.45, 0.01, 1.45, 1.5, 1.60, 1.35, 1.30, 1.20, 1.20, 1.15, 1.20, 1.29};

  if (Cam_offset <= Cam_Offset_Table0[0])
  {
    PID_TURN_CAM.P = Turn_Cam_P_Table0[0];
    PID_TURN_CAM.D = Turn_Cam_D_Table0[0];
  }
  else if (Cam_offset >= Cam_Offset_Table0[20])
  {
    PID_TURN_CAM.P = Turn_Cam_P_Table0[20];
    PID_TURN_CAM.D = Turn_Cam_D_Table0[20];
  }
  else
  {
    for (int i = 0; i < 20; i++)
    {
      if (Cam_offset >= Cam_Offset_Table0[i] && Cam_offset < Cam_Offset_Table0[i + 1])
      {
        PID_TURN_CAM.P = Turn_Cam_P_Table0[i] + (Cam_offset - Cam_Offset_Table0[i]) * (Turn_Cam_P_Table0[i + 1] - Turn_Cam_P_Table0[i]) / (Cam_Offset_Table0[i + 1] - Cam_Offset_Table0[i]); //����
        PID_TURN_CAM.D = Turn_Cam_D_Table0[i] + (Cam_offset - Cam_Offset_Table0[i]) * (Turn_Cam_D_Table0[i + 1] - Turn_Cam_D_Table0[i]) / (Cam_Offset_Table0[i + 1] - Cam_Offset_Table0[i]);
        break;
      }
    }
  }
  return PID_TURN_CAM;
}

/*********************************************
***�������ƣ����ת�����
***���������ƽ�е�С���ֱ���ֵ
***���������������
***˵����

*********************************************/
void Turn_EM(void)
{
  // float mid_err;
  // float mid_length;
  // float mid_length_err;
  // float EM_angle;
  // float EM_offset1;
  // float EM_Turn_Control1, EM_Turn_Control2, EM_Turn_Control;
  // float length_EM_23 = 0.17;
  static float EM_center_offset_last = 0;
  static float EM_straight_offset_last = 0;
  // EM_offset1 = mid_length_err / 8.5 * 350;
  // EM_straight_offset = EM_err_cal(EM_Value_2, EM_Value_3, EM_Value_1, EM_Value_4); //��ƽ�У���ƽ�У���ֱ���Ҵ�ֱ �õ�ת��
  // TurnFuzzyPD_EM();                                                      //����ƫ��дģ��PD
  // EM_offset1 *= Turn_P_EM;
  if (EM_center_offset == 999)
  {
    Turn_EM_Out1 = 0;
  }
  else
  {
    Turn_EM_Out1 = EM_center_offset * PID_CENTER_EM.P + (EM_center_offset - EM_center_offset_last) * PID_CENTER_EM.D;
    EM_center_offset_last = EM_center_offset;
  }

  Turn_EM_Out2 = EM_straight_offset * PID_STRAIGHT_EM.P + (EM_straight_offset - EM_straight_offset_last) * PID_STRAIGHT_EM.D;
  EM_straight_offset_last = EM_straight_offset;

  // EM_Turn_Control2 = PD_section1(EM_angle);

  Turn_EM_Out = 1.0 * Turn_EM_Out1 + 1.0 * Turn_EM_Out2;
}

/*********************************************
***�������ƣ�ת�����
***�������������ͷ��ǣ���Ŵ��
***���������������
***˵����
*********************************************/
void Turn_Servo()
{
  if (Road == 7)
  {
    if (Road7_flag == 2)
    {
#ifdef TL2barn
      Turn_Out = -SERVO_RANGE;
#endif
#ifdef TR2barn
      Turn_Out = SERVO_RANGE;
#endif
    }
    else if (Road7_flag == 3)
    {
      Turn_Out = Turn_Cam_Out;
    }
    else if (Road7_flag == 4)
    {
      Turn_Out = 0;
    }
  }
  
  else if (Road == 1)
  {
    if (Road1_flag == 5)
    {
      // if (Turn_Cam_Out > -0.5 * SERVO_RANGE)
      // {
      //   Turn_Cam_Out = -0.5 * SERVO_RANGE;
      // }
      Turn_Out = -SERVO_RANGE;
    }
    else if (Road1_flag == 4)
    {
      Turn_Out = Turn_EM_Out;
    }
    else
    {
      Turn_Out = Turn_Cam_Out;
    }
  }
  
  else if (Road == 2)
  {
    if (Road2_flag == 5)
    {
      // if (Turn_Cam_Out < 0.5 * SERVO_RANGE)
      // {
      //   Turn_Cam_Out = 0.5 * SERVO_RANGE;
      // }
      Turn_Out = SERVO_RANGE;
    }
    else if (Road2_flag == 4)
    {
      Turn_Out = Turn_EM_Out;
    }
    else
    {
      Turn_Out = Turn_Cam_Out;
    }
  }
  
  else if (Road == 3)
  {
    Turn_Out = Turn_Cam_Out;
  }
  
  else
  {
    if (Road0_flag == 1 || Road0_flag == 2)
    {
      Turn_Out = Turn_Cam_Out;
    }
    else if (EM_edge > 2)
    {
      Turn_Out = Turn_Cam_Out;
    }
    else
    {
      Turn_Out = Turn_EM_Out;
    }
  }

  Servo_Duty(-Turn_Out); //�������
}

/*************************************************************************
*  �������ƣ�void SpeedTarget_fig(void)
*  ����˵���������ٶ�Ŀ����
*  ����˵����
*  �������أ��ٶ�Ŀ����
*  �޸�ʱ�䣺2020.6.20
*  ��    ע��
*************************************************************************/
void SpeedTarget_fig(void)
{
  float angle_val; // ������ʾʵ��ת��Ƕ�
  float diff_K0;   // ������=���ٱȾ��٣������ָ�һ��

  if (get_diff_state() == DIFF_ON_VAL)
  {
    //���ز�����Para�ж���

    angle_val = Turn_Cam_Out / SERVO_RANGE * ANGLE_RANGE;
    diff_K0 = CAR_DIFF_K * tan(angle_val);
    //�ɴ�PD������
  }
  else if (get_diff_state() == DIFF_OFF_VAL)
  {
    diff_K0 = 0;
  }

  speedTarget1 = SpeedGoal * (1 + diff_K0 / 2); //��೵��
  speedTarget2 = SpeedGoal * (1 - diff_K0 / 2); //�Ҳ೵��
  //����ɼ������²���

  if (barn_reset_flag == 1)
  {
    if (CarSpeed1 >= 3 || CarSpeed2 >= 3)
    {
      SpeedGoal = 0;
    }
  }
}

/*************************************************************************
*  �������ƣ�void Speed_Control(void)
*  ����˵�����ٶ�PI
*  ����˵����
*  �������أ����ռ�ձ�+������
*  �޸�ʱ�䣺2020.6.20
*  ��    ע��
*************************************************************************/
PID PID_SPEED;
void Speed_Control(void)
{
  int SpeedE1, SpeedE2;
  static int OldE1, OldE2;
  int SpeedEE1, SpeedEE2;
  float Speed_kP1, Speed_kP2, Speed_kI1, Speed_kI2;
  float SpeedControlOutE1, SpeedControlOutE2;
  SpeedE1 = (int)((speedTarget1 - CarSpeed1) * 100.0);
  SpeedE2 = (int)((speedTarget2 - CarSpeed2) * 100.0);

  //kp
  Speed_kP1 = PID_SPEED.P;
  Speed_kP2 = PID_SPEED.P;
  //ki
  SpeedEE1 = SpeedE1 - OldE1;
  SpeedEE2 = SpeedE2 - OldE2;
  ////////////////////////////////////////////////////////////////////////////////
  if (abs(SpeedE1) < 15)
    Speed_kI1 = 0;
  else
    Speed_kI1 = PID_SPEED.I;
  if (abs(SpeedE2) < 15)
    Speed_kI2 = 0;
  else
    Speed_kI2 = PID_SPEED.I;
  ///////////////////////////////////////////////////////////////////////////////
  SpeedControlOutE1 = (Speed_kP1 * SpeedEE1 + Speed_kI1 * SpeedE1);
  SpeedControlOutE2 = (Speed_kP2 * SpeedEE2 + Speed_kI2 * SpeedE2);
  OldE1 = SpeedE1;
  OldE2 = SpeedE2;

  MotorOut1 += SpeedControlOutE1;
  MotorOut2 += SpeedControlOutE2;

  Moto_Out(MotorOut1, MotorOut2); // (uint32)(MotorOut1 / 100) * 100Ҫ�ȷ�+-
}