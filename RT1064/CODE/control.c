#include "headfile.h"
float Turn_Cam_Out;
float Turn_EM_Out;
float Turn_Out;
float MotorOut1, MotorOut2;
float speedTarget1, speedTarget2;
float Turn_P_EM;
float Turn_D_EM;
bool speed_change_flag = 0;

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
      if (DEBUG_CHOICE == 1)
      {
        Turn_Out = Turn_EM_Out;
      }
      else if (DEBUG_CHOICE == 2)
      {
        Turn_Out = Turn_Cam_Out;
      }
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
      if (DEBUG_CHOICE == 1)
      {
        Turn_Out = Turn_EM_Out;
      }
      else if (DEBUG_CHOICE == 2)
      {
        Turn_Out = Turn_Cam_Out;
      }
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
      if (DEBUG_CHOICE == 1)
      {
        Turn_Out = Turn_EM_Out;
      }
      else if (DEBUG_CHOICE == 2)
      {
        Turn_Out = Turn_Cam_Out;
      }
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
      //SpeedGoal = 0;
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
PID PID_SPEED, PID2_SPEED;

float SpeedE1, SpeedE2;
float SpeedEE1, SpeedEE2;
#if 0
void Speed_Control(void)
{

  static float OldE1, OldE2;

  float Speed_kP1, Speed_kP2, Speed_kI1, Speed_kI2;
  float SpeedControlOutE1, SpeedControlOutE2;
  static bool a_flag1 = 1, a_flag2 = 1, d_flag1 = 1, d_flag2 = 1;
  SpeedE1 = speedTarget1 - CarSpeed1;
  SpeedE2 = speedTarget2 - CarSpeed2;

  //kp
  if (fabs(SpeedE1) < 0.3)
  {
    Speed_kP1 = PID2_SPEED.P;
    Speed_kI1 = PID2_SPEED.I;
  }
  else
  {
    Speed_kP1 = PID_SPEED.P;
    Speed_kI1 = PID_SPEED.I;
  }
  if (fabs(SpeedE2) < 0.3)
  {
    Speed_kP2 = PID2_SPEED.P;
    Speed_kI2 = PID2_SPEED.I;
  }
  else
  {
    Speed_kP2 = PID_SPEED.P;
    Speed_kI2 = PID_SPEED.I;
  }
  if (fabs(SpeedEE1) > 0.1)
  {
    Speed_kP1 = 0;
  }
  if (fabs(SpeedEE2) > 0.1)
  {
    Speed_kP2 = 0;
  }

  //ki
  SpeedEE1 = SpeedE1 - OldE1;
  SpeedEE2 = SpeedE2 - OldE2;

  SpeedControlOutE1 = (Speed_kP1 * SpeedEE1 + Speed_kI1 * SpeedE1);
  SpeedControlOutE2 = (Speed_kP2 * SpeedEE2 + Speed_kI2 * SpeedE2);

  OldE1 = SpeedE1;
  OldE2 = SpeedE2;

  MotorOut1 += SpeedControlOutE1;
  MotorOut2 += SpeedControlOutE2;

  // if (a_flag1 == 0 && SpeedE1 < -0.2)
  // {
  //   a_flag1 = 1;
  //   MotorOut1 = speedTarget1 * 2000;
  // }
  // else if (SpeedE1 > 1)
  // {
  //   a_flag1 = 0;
  // }
  // if (a_flag2 == 0 && SpeedE2 < -0.2)
  // {
  //   a_flag2 = 1;
  //   MotorOut2 = speedTarget2 * 2000;
  // }
  // else if (SpeedE2 > 1)
  // {
  //   a_flag2 = 0;
  // }

  // if (d_flag1 == 0 && SpeedE1 > 0.1)
  // {
  //   d_flag1 = 1;
  //   MotorOut1 = speedTarget1 * 2000;
  // }
  // else if (SpeedE1 < -1)
  // {
  //   d_flag1 = 0;
  // }
  // if (d_flag2 == 0 && SpeedE2 > 0.2)
  // {
  //   d_flag2 = 1;
  //   MotorOut2 = speedTarget2 * 2500;
  // }
  // else if (SpeedE2 < -1)
  // {
  //   d_flag2 = 0;
  // }

  Moto_Out(); // (uint32)(MotorOut1 / 100) * 100Ҫ�ȷ�+-
}

void BBC(void)
{
  static float OldE1, OldE2;

  float Speed_kP1, Speed_kP2, Speed_kI1, Speed_kI2;
  float SpeedControlOutE1, SpeedControlOutE2;

  static bool a_flag1 = 0, a_flag2 = 0, d_flag1 = 0, d_flag2 = 0, l_flag1 = 1, l_flag2 = 1;
  static int cnt1, cnt2;
  static int frame1 = 0, frame2 = 0;
  static bool frame_flag1 = 0, frame_flag2 = 0;
  static int linear_flag1 = 2, linear_flag2 = 2;

  if (frame_flag1)
  {
    frame1++;
  }
  if (frame_flag2)
  {
    frame2++;
  }

  SpeedE1 = speedTarget1 - CarSpeed1;
  SpeedE2 = speedTarget2 - CarSpeed2;

  Speed_kP1 = PID_SPEED.P;
  Speed_kI1 = PID_SPEED.I;
  // //kp
  // if (fabs(SpeedE1) < 0.3)
  // {
  //   Speed_kP1 = PID2_SPEED.P;
  //   Speed_kI1 = PID2_SPEED.I;
  // }
  // else
  // {
  //   Speed_kP1 = PID_SPEED.P;
  //   Speed_kI1 = PID_SPEED.I;
  // }
  // if (fabs(SpeedE2) < 0.3)
  // {
  //   Speed_kP2 = PID2_SPEED.P;
  //   Speed_kI2 = PID2_SPEED.I;
  // }
  // else
  // {
  //   Speed_kP2 = PID_SPEED.P;
  //   Speed_kI2 = PID_SPEED.I;
  // }
  if (fabs(SpeedEE1) > 0.1)
  {
    Speed_kP1 = 0;
  }
  if (fabs(SpeedEE2) > 0.1)
  {
    Speed_kP2 = 0;
  }
  /******* ���� *******/
  if (SpeedE1 < 15 && frame_flag1 == 0)
  {
    /* �״ν�����λ����ʼ��֡ */
    Speed_kI1 = 0;
    frame_flag1 = 1;
    frame1 = 0;
  }

  if (frame1 < 10 && frame_flag1 == 1)
  {
    /* С��10֡ �ҿ�ʼ��֡*/
    Speed_kI1 = 0;
  }
  else
  {
    /* ������֡ ���ظ�������λ I��Ϊ0 */
    frame1 = 0;
  }

  if (SpeedE1 > 1)
  {
    /* ����1ʱ��λ ��������λ ֡������*/
    frame_flag1 = 0;
    frame1 = 0;
  }
  /******* ���� *******/
  if (SpeedE2 < 15 && frame_flag2 == 0)
  {
    /* �״ν�����λ����ʼ��֡ */
    Speed_kI2 = 0;
    frame_flag2 = 1;
    frame2 = 0;
  }

  if (frame2 < 10 && frame_flag2 == 1)
  {
    /* С��10֡ �ҿ�ʼ��֡*/
    Speed_kI2 = 0;
  }
  else
  {
    /* ������֡ ���ظ�������λ I��Ϊ0 */
    frame2 = 0;
  }

  if (SpeedE2 > 1)
  {
    /* ����1ʱ��λ ��������λ ֡������*/
    frame_flag2 = 0;
    frame2 = 0;
  }

  //ki
  SpeedEE1 = SpeedE1 - OldE1;
  SpeedEE2 = SpeedE2 - OldE2;

  SpeedControlOutE1 = (Speed_kP1 * SpeedEE1 + Speed_kI1 * SpeedE1);
  SpeedControlOutE2 = (Speed_kP2 * SpeedEE2 + Speed_kI2 * SpeedE2);

  OldE1 = SpeedE1;
  OldE2 = SpeedE2;

  MotorOut1 += SpeedControlOutE1;
  MotorOut2 += SpeedControlOutE2;
  // if (!linear_flag1 || !l_flag1)
  // {
  //   MotorOut1 += SpeedControlOutE1;
  //   l_flag1 = 0;
  // }
  // else
  // {
  //   MotorOut1 -= 50;
  // }

  // if (!linear_flag2 || !l_flag2)
  // {
  //   MotorOut2 += SpeedControlOutE2;
  //   l_flag2 = 0;
  // }
  // else
  // {
  //   MotorOut2 -= 50;
  // }

  if (SpeedE1 > 1)
  {
    a_flag1 = 1;
    cnt1 = 0;
  }
  if (SpeedE2 > 1)
  {
    a_flag2 = 1;
    cnt2 = 0;
  }

  if (SpeedE1 < -1)
  {
    d_flag1 = 1;
    cnt1 = 0;
  }
  if (SpeedE2 < -1)
  {
    d_flag2 = 1;
    cnt2 = 0;
  }

  if (a_flag1 == 1)
  {
    if (CarSpeed1 < speedTarget1 * 1.0)
    {
      MotorOut1 = speedTarget1 * 5000; //speedTarget1
    }
    else if (CarSpeed1 < speedTarget1 * 1.1) //0.8
    {
      cnt1++;
      if (cnt1 > 5)
      {
        a_flag1 = 0;
        MotorOut1 = speedTarget1 * 2500;
      }
    }
    else
    {
      a_flag1 = 0;
      MotorOut1 = speedTarget1 * 2500;
    }
  }
  else if (d_flag1 == 1)
  {
    if (CarSpeed1 > speedTarget1 * 1.0)
    {
      MotorOut1 = speedTarget1 * -3000;
    }
    else if (CarSpeed1 > speedTarget1 * 0.8)
    {
      cnt1++;
      if (cnt1 > 5)
      {
        d_flag1 = 0;
        MotorOut1 = speedTarget1 * 2500;
      }
    }
    else
    {
      d_flag1 = 0;
      MotorOut1 = speedTarget1 * 2500;
    }
  }
  else
  {
    ; //MotorOut1 = 3500;
  }

  if (a_flag2 == 1)
  {
    if (CarSpeed2 < speedTarget2 * 1.0)
    {
      MotorOut2 = speedTarget2 * 5000;
    }
    else if (CarSpeed2 < speedTarget2 * 1.1)
    {
      cnt2++;
      if (cnt2 > 5)
      {
        a_flag2 = 0;
        MotorOut2 = speedTarget2 * 2500;
      }
    }
    else
    {
      a_flag2 = 0;
      MotorOut2 = speedTarget2 * 2500;
    }
  }
  else if (d_flag2 == 1)
  {

    if (CarSpeed2 > speedTarget2 * 1.0)
    {
      MotorOut2 = speedTarget2 * -3000;
    }
    else if (CarSpeed2 > speedTarget2 * 0.8)
    {
      cnt2++;
      if (cnt2 > 5)
      {
        d_flag2 = 0;
        MotorOut2 = speedTarget2 * 2500;
      }
    }
    else
    {
      d_flag2 = 0;
      MotorOut2 = speedTarget2 * 2500;
    }
  }
  else
  {
    // MotorOut2 = 3500;
  }
  // if (!a_flag1)
  // {
  //   /* bbc֮�󣬳���ʱ */
  //   if (SpeedE1 < 0)
  //   {
  //     linear_flag1 = 1;
  //   }
  //   /* bbc֮�󣬵���ʱ ����20%��� */
  //   if (SpeedE1 > speedTarget1 * 0.1 && linear_flag1 == 1)
  //   {
  //     linear_flag1 = 0;
  //     l_flag1 = 0;
  //   }
  // }
  // if (!a_flag2)
  // {
  //   /* bbc֮�󣬳���ʱ */
  //   if (SpeedE2 < 0)
  //   {
  //     linear_flag2 = 1;
  //   }
  //   /* bbc֮�󣬵���ʱ ����20%��� */
  //   if (SpeedE2 > speedTarget2 * 0.1 && linear_flag2 == 1)
  //   {
  //     linear_flag2 = 0;
  //     l_flag2 = 0;
  //   }
  // }

  Moto_Out();
}
#endif
void Speed_Control_New(void)
{
  static float OldE1, OldE2;

  float Speed_kP1, Speed_kP2, Speed_kI1, Speed_kI2;
  float SpeedControlOutE1, SpeedControlOutE2;

  static bool a_flag1 = 0, a_flag2 = 0, d_flag1 = 0, d_flag2 = 0;
  static int cnt1, cnt2;
  static int frame1 = 0, frame2 = 0;
  static bool frame_flag1 = 0, frame_flag2 = 0;
  static bool park_flag = 0;
  static bool Lef_BB = 0, Rig_BB = 0;

  if (speed_change_flag == 1)
  {
    Lef_BB = 1;
    Rig_BB = 1;
    speed_change_flag = 0;
  }

  if (frame_flag1)
  {
    frame1++;
  }
  if (frame_flag2)
  {
    frame2++;
  }

  SpeedE1 = speedTarget1 - CarSpeed1;
  SpeedE2 = speedTarget2 - CarSpeed2;

  Speed_kP1 = PID_SPEED.P;
  Speed_kI1 = PID_SPEED.I;
  Speed_kP2 = PID_SPEED.P;
  Speed_kI2 = PID_SPEED.I;

  SpeedEE1 = SpeedE1 - OldE1;
  SpeedEE2 = SpeedE2 - OldE2;

  if (SpeedEE1 > 0.1 || SpeedEE1 < -0.1)
  {
    Speed_kP1 = 0;
  }
  if (SpeedEE2 > 0.1 || SpeedEE2 < -0.1)
  {
    Speed_kP2 = 0;
  }
  /******* ���� *******/
  if (SpeedE1 < 0.15 && SpeedE1 > -0.15 &&frame_flag1 == 0)
  {
    /* �״ν�����λ����ʼ��֡ */
    Speed_kI1 = 0;
    frame_flag1 = 1;
    frame1 = 0;
  }

  if (frame1 < 10 && frame_flag1 == 1)
  {
    /* С��10֡ �ҿ�ʼ��֡*/
    Speed_kI1 = 0;
  }
  else
  {
    /* ������֡ ���ظ�������λ I��Ϊ0 */
    frame1 = 11;
  }

  /******* ���� *******/
  if (SpeedE2 < 0.15 && SpeedE2>-0.15 && frame_flag2 == 0)
  {
    /* �״ν�����λ����ʼ��֡ */
    Speed_kI2 = 0;
    frame_flag2 = 1;
    frame2 = 0;
  }

  if (frame2 < 10 && frame_flag2 == 1)
  {
    /* С��10֡ �ҿ�ʼ��֡*/
    Speed_kI2 = 0;
  }
  else
  {
    /* ������֡ ���ظ�������λ I��Ϊ0 */
    frame2 = 11;
  }

  //ki

  SpeedControlOutE1 = (Speed_kP1 * SpeedEE1 + Speed_kI1 * SpeedE1);
  SpeedControlOutE2 = (Speed_kP2 * SpeedEE2 + Speed_kI2 * SpeedE2);

  OldE1 = SpeedE1;
  OldE2 = SpeedE2;

  MotorOut1 += SpeedControlOutE1;
  MotorOut2 += SpeedControlOutE2;
  if (SpeedE1 > 1)
  {
    a_flag1 = 1;
    cnt1 = 0;
    frame_flag1 = 0;
    frame1 = 0;
  }
  if (SpeedE2 > 1)
  {
    a_flag2 = 1;
    cnt2 = 0;
    frame_flag2 = 0;
    frame2 = 0;
  }

  if (SpeedE1 < -1)
  {
    d_flag1 = 1;
    cnt1 = 0;
    frame_flag1 = 0;
    frame1 = 0;
  }
  if (SpeedE2 < -1)
  {
    d_flag2 = 1;
    cnt2 = 0;
    frame_flag2 = 0;
    frame2 = 0;
  }
  if (SpeedGoal == 0)
  {
    park_flag = 1;
    a_flag1 = 0;
    a_flag2 = 0;
    d_flag1 = 0;
    d_flag2 = 0;
  }
  if (park_flag == 1)
  {
    if (CarSpeed1 > SpeedGoal &&Lef_BB)
    {
      MotorOut1 = -MOTOR_RANGE;
    }
    else
    {
      Lef_BB = 0;
      park_flag = 0;
      MotorOut1 = 0;
    }
    if (CarSpeed2 > SpeedGoal && Rig_BB)
    {
      MotorOut2 = -MOTOR_RANGE;
    }
    else
    {
      Rig_BB = 0;
      park_flag = 0;
      MotorOut2 = 0;
    }
  }
  if (a_flag1 == 1 && Lef_BB)
  {
    if (CarSpeed1 < speedTarget1 * 1.0)
    {
      MotorOut1 = speedTarget1 * 5000; //speedTarget1
    }
    else if (CarSpeed1 < speedTarget1 * 1.1) //0.8
    {
      cnt1++;
      if (cnt1 > 5)
      {
        a_flag1 = 0;
        Lef_BB = 0;
        MotorOut1 = speedTarget1 * 2500;
      }
    }
    else
    {
      a_flag1 = 0;
      Lef_BB = 0;
      MotorOut1 = speedTarget1 * 2500;
    }
  }
  else if (d_flag1 == 1 && Lef_BB)
  {
    if (CarSpeed1 > speedTarget1 * 1.0)
    {
      MotorOut1 = speedTarget1 * -3000;
    }
    else if (CarSpeed1 > speedTarget1 * 0.8)
    {
      cnt1++;
      if (cnt1 > 5)
      {
        d_flag1 = 0;
        Lef_BB = 0;
        MotorOut1 = speedTarget1 * 2500;
      }
    }
    else
    {
      d_flag1 = 0;
      Lef_BB = 0;
      MotorOut1 = speedTarget1 * 2500;
    }
  }

  if (a_flag2 == 1 && Rig_BB)
  {
    if (CarSpeed2 < speedTarget2 * 1.0)
    {
      MotorOut2 = speedTarget2 * 5000;
    }
    else if (CarSpeed2 < speedTarget2 * 1.1)
    {
      cnt2++;
      if (cnt2 > 5)
      {
        a_flag2 = 0;
        Rig_BB = 0;
        MotorOut2 = speedTarget2 * 2500;
      }
    }
    else
    {
      a_flag2 = 0;
      Rig_BB = 0;
      MotorOut2 = speedTarget2 * 2500;
    }
  }
  else if (d_flag2 == 1 && Rig_BB)
  {

    if (CarSpeed2 > speedTarget2 * 1.0)
    {
      MotorOut2 = speedTarget2 * -3000;
    }
    else if (CarSpeed2 > speedTarget2 * 0.8)
    {
      cnt2++;
      if (cnt2 > 5)
      {
        d_flag2 = 0;
        Rig_BB = 0;
        MotorOut2 = speedTarget2 * 2500;
      }
    }
    else
    {
      d_flag2 = 0;
      Rig_BB = 0;
      MotorOut2 = speedTarget2 * 2500;
    }
  }

  Moto_Out();
}

void Kalman_Filter(void)
{
  /* ��������uk�������Ǳ仯d�� */
  /* ���ƾ���B���Ƕ��ٶ���Ͼ��� */
  /* ״̬����xk����ǰ״̬��frenet����ϵ��n�����������߷�����룬�̣��������������߼нǣ� */
  /* �ٶȷ���£�tan�� = 2 * tan�� */
  /* ״̬���̣� nk = nk+1 + vxsin�� + vycos�� */
  /* ״̬���̣� ��k = ��k-1 + r - ��(s)ds*/
  /* ״̬ת�ƾ���Fk��  */

  float accuracy_EM = 0;
  float accuracy_Cam = 0;
  float K = 0;
  accuracy_EM = 1;
  if (Road0_flag == 4 || Road0_flag == 5)
  {
    lib_speed_set(0);
    ;
    Road = 7;
    Road7_flag = 4;
  }
  Servo_Duty(-Turn_Cam_Out);
  //   if (Road == 7)
  //   {
  //     if (Road7_flag == 2)
  //     {
  // #ifdef TL2barn
  //       Turn_Out = -SERVO_RANGE;
  // #endif
  // #ifdef TR2barn
  //       Turn_Out = SERVO_RANGE;
  // #endif
  //     }
  //     else if (Road7_flag == 3)
  //     {
  //       accuracy_EM = 0;
  //       accuracy_Cam = 1;
  //     }
  //     else if (Road7_flag == 4)
  //     {
  //       Turn_Out = 0;
  //     }
  //   }

  //   else if (Road == 1)
  //   {
  //     if (Road1_flag == 5)
  //     {
  //       // if (Turn_Cam_Out > -0.5 * SERVO_RANGE)
  //       // {
  //       //   Turn_Cam_Out = -0.5 * SERVO_RANGE;
  //       // }
  //       Turn_Out = -SERVO_RANGE;
  //       Servo_Duty(-Turn_Out); //�������
  //       return;
  //     }
  //     else if (Road1_flag == 4)
  //     {
  //       if (EM_edge > 0)
  //       {
  //         accuracy_EM = 0;
  //       }
  //       else
  //       {
  //         accuracy_EM = 0.3;
  //       }
  //       accuracy_Cam = 1;
  //     }
  //     else
  //     {
  //       accuracy_EM = 0;
  //       accuracy_Cam = 1;
  //     }
  //   }

  //   else if (Road == 2)
  //   {
  //     if (Road2_flag == 5)
  //     {
  //       // if (Turn_Cam_Out < 0.5 * SERVO_RANGE)
  //       // {
  //       //   Turn_Cam_Out = 0.5 * SERVO_RANGE;
  //       // }
  //       Turn_Out = SERVO_RANGE;
  //       Servo_Duty(-Turn_Out); //�������
  //       return;
  //     }
  //     else if (Road2_flag == 4)
  //     {
  //       if (EM_edge > 0)
  //       {
  //         accuracy_EM = 0;
  //       }
  //       else
  //       {
  //         accuracy_EM = 0.3;
  //       }
  //       accuracy_Cam = 1;
  //     }
  //     else
  //     {
  //       accuracy_EM = 0;
  //       accuracy_Cam = 1;
  //     }
  //   }

  //   else if (Road == 3)
  //   {
  //     accuracy_EM = 0;
  //     accuracy_Cam = 1;
  //   }

  //   else
  //   {
  //     if (Road0_flag == 1 || Road0_flag == 2)
  //     {
  //       accuracy_EM = 0;
  //       accuracy_Cam = 1;
  //     }
  //     else if (EM_edge > 2)
  //     {
  //       accuracy_EM = 0;
  //       accuracy_Cam = 1;
  //     }
  //     else
  //     {
  //       accuracy_EM = 0.3;
  //       accuracy_Cam = 0.7;
  //     }
  //   }

  //   K = accuracy_EM / (accuracy_Cam + accuracy_EM);
  //   Turn_Out = (1 - K) * Turn_Cam_Out + K * Turn_EM_Out;
  //   Servo_Duty(-Turn_Out); //�������
}
