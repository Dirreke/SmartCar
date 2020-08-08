#include "headfile.h"
float Turn_Cam_Out = 0;
float Turn_EM_Out = 0;
float Turn_Out = 0;
float MotorOut1 = 0, MotorOut2 = 0;
float MotorOut1_add = 0;
float MotorOut2_add = 0;
float speedTarget1 = 0, speedTarget2 =0;
float Turn_P_EM;
float Turn_D_EM;
bool speed_change_flag = 0;

int ramp_out_time = -500;
int out_circle_time_temp = -500;
PID PID_CENTER_EM;

/*********************************************
***�������ƣ����ת�����
***���������ƽ�е�С���ֱ���ֵ
***���������������
***˵����

*********************************************/
void Turn_EM(void)
{
  static float EM_center_offset_last = 0;
  if (EM_center_offset == 999)
  {
    Turn_EM_Out = 0;
  }
  else
  {
    Turn_EM_Out = EM_center_offset * PID_CENTER_EM.P + (EM_center_offset - EM_center_offset_last) * PID_CENTER_EM.D;
    EM_center_offset_last = EM_center_offset;
  }
}
/*************************************************************************
*  �������ƣ�void SpeedTarget_fig(void)
*  ����˵���������ٶ�Ŀ����
*  ����˵����
*  �������أ��ٶ�Ŀ����
*  �޸�ʱ�䣺2020.8.8
*  ��    ע��
*************************************************************************/
// float DIFF_KKK = 0;
// float DIFF_KK = 1;
PID PID_diff;
PID PID_diff0;
uint8 diff_BB_flag = 0;
void SpeedTarget_fig(void)
{
  float angle_val; // ������ʾʵ��ת��Ƕ�
  float diff_K0;   // ������=���ٱȾ��٣������ָ�һ��
  float Turn_Cam_Out_temp = Turn_Cam_Out;
  float PID_diff_P;
  angle_val = Turn_Cam_Out_temp * ANGLE_DIVIDE_SERVO_SCALE;
  diff_BB_flag = 0;

  { //Turn_Cam_Out���޷�������
    if (fabs(Turn_Cam_Out_temp) > CAR_DIFF_SERVO_RANGE + 50) //300
    {
      PID_diff_P = 2;
      diff_on();
    }
    else if (Turn_Cam_Out_temp > CAR_DIFF_SERVO_RANGE)
    {
      PID_diff_P = 1;
      diff_on();
    }
    else if (fabs(Turn_Cam_Out_temp) > SERVO_RANGE + 10) //262.5 * PID_CAR_STRAIGHT_CAM.P) //SERVO_RANGE)
    {
      diff_on();
      PID_diff_P = PID_diff.P;
    }
    else
    {
      diff_on();
      PID_diff_P = PID_diff0.P;
    }


    if(Road == 4 || (Road == 3 && Road3_flag == 0))
    {
        PID_diff_P = 0;
    }
    else if((Road == 7 && Road7_flag == 2) || (Road == 3 && Road3_flag == 1))
    {
        PID_diff_P = 2;
    }
    
  }
  diff_K0 = CAR_DIFF_K * tan(angle_val);

  // }
  if (get_diff_state() == DIFF_OFF_VAL)
  {
    diff_K0 = 0;
  }
  else
  {
    diff_K0 = CAR_DIFF_K * tan(angle_val) * PID_diff_P;
  }
  
  /* ������Ŀ���ٶȼ��� */
  speedTarget1 = SpeedGoal * (1 + diff_K0 / 2); //��೵��
  speedTarget2 = SpeedGoal * (1 - diff_K0 / 2); //�Ҳ೵��

  //����ɼ������²���

}
/*************************************************************************
*  �������ƣ�void Speed_Control(L_flag)
*  ����˵�����ٶ�PI+bang
*  ����˵����1��0��
*  �������أ����ռ�ձ�//�������ŵ�������
*  �޸�ʱ�䣺2020.8.8
*  ��    ע��
*************************************************************************/
PID PID_SPEED, PID2_SPEED;

float SpeedE1, SpeedE2;
float SpeedEE1, SpeedEE2;
float SpeedGoalE1, SpeedGoalE2;
bool a_flag1 = 0, a_flag2 = 0, d_flag1 = 0, d_flag2 = 0;
int8 diff_flag1 = -1, diff_flag2 = -1;
void Speed_Control_New(void)
{

  static float OldE1, OldE2;

  float Speed_kP1, Speed_kP2, Speed_kI1, Speed_kI2;
  float SpeedControlOutE1, SpeedControlOutE2;

  static uint8 Lef_pp_cnt = 0, Rig_pp_cnt = 0;
  static int cnt1, cnt2;
  static int frame1 = 0, frame2 = 0;
  static bool frame_flag1 = 0, frame_flag2 = 0;
  static bool Lef_pp = 0, Rig_pp = 0;
  static bool Lef_BB = 0, Rig_BB = 0;
  static float Turn_Out_old;
  /* �����ٶ�ƫ�� */
  SpeedE1 = speedTarget1 - CarSpeed1;
  SpeedE2 = speedTarget2 - CarSpeed2;
  SpeedGoalE1 = SpeedGoal - CarSpeed1;
  SpeedGoalE2 = SpeedGoal - CarSpeed2;
  /* ���趨�ٴ�bang */
  if (speed_change_flag)
  {
    //ͣ������bang
    if (SpeedGoal == 0)
    {
      MotorOut1 = 0;
      MotorOut2 = 0;
      //return;
      Lef_pp = 1;
      Rig_pp = 1;
    }
    //����bang
    else
    {
      Lef_BB = 1;
      Rig_BB = 1;
    }
    speed_change_flag = 0;
  }

  if (SpeedGoalE1 > 1 && ((Road0_flag < 3 && Road == 0 )|| Road == 3))
  {
    //��ʹ�ٶ�Ŀ�겻�ı䵫speede1>1.5Ҳֱ�ӽ�BB
    if (SpeedGoalE1 > 1.5)
    {
      a_flag1 = 1;
      Lef_BB = 0;
    }
    else if (Lef_BB)
    {
      a_flag1 = 1;
      Lef_BB = 0;
    }
    cnt1 = 0;
    frame_flag1 = 0;
    frame1 = 0;
  }
  if (SpeedGoalE2 > 1 && ((Road0_flag < 3 && Road == 0 )|| Road == 3))
  {
    //��ʹ�ٶ�Ŀ�겻�ı䵫speede2>1.5Ҳֱ�ӽ�BB
    if (SpeedGoalE2 > 1.5)
    {
      a_flag2 = 1;
      Rig_BB = 0;
    }
    else if (Rig_BB)
    {
      a_flag2 = 1;
      Rig_BB = 0;
    }
    cnt2 = 0;
    frame_flag2 = 0;
    frame2 = 0;
  }

  if (SpeedGoalE1 < -1 && ((Road0_flag < 3 && Road == 0 )|| Road == 4 || Road == 7))
  {
    //ͬ��
    if (SpeedGoalE1 < -1.5)
    {
      d_flag1 = 1;
      Lef_BB = 0;
    }
    else if (Lef_BB)
    {
      d_flag1 = 1;
      Lef_BB = 0;
    }
    cnt1 = 0;
    frame_flag1 = 0;
    frame1 = 0;
  }
  if (SpeedGoalE2 < -1)
  {
    //ͬ��
    if (SpeedGoalE2 < -1.5)
    {
      d_flag2 = 1;
      Rig_BB = 0;
    }
    if (Rig_BB)
    {
      d_flag2 = 1;
      Rig_BB = 0;
    }
    cnt2 = 0;
    frame_flag2 = 0;
    frame2 = 0;
  }
  //�����bang
  if (fabs(Turn_Out) > 100 && fabs(Turn_Out) < CAR_DIFF_SERVO_RANGE)
  {

    if (a_flag1 == 1)
    {
      MotorOut1 = SpeedGoal * 2500;
      a_flag1 = 0;
    }
    if (a_flag2 == 1)
    {
      MotorOut2 = SpeedGoal * 2500;
      a_flag2 = 0;
    }
    if (d_flag1 == 1)
    {
      if (CarSpeed < SpeedGoal)
      {
        d_flag1 = 0;
        MotorOut1 = SpeedGoal * 2500;
      }
    }
    if (d_flag2 == 1)
    {
      if (CarSpeed < SpeedGoal)
      {
        d_flag2 = 0;
        MotorOut2 = SpeedGoal * 2500;
      }
    }
  }
  else if (fabs(Turn_Out) >= CAR_DIFF_SERVO_RANGE && (fabs(Turn_Out_old)-fabs(Turn_Out)<10||((Turn_Out>0 ^ Turn_Out_old>0) == 0))&& (diff_flag1 < 0 || diff_flag2 < 0) && CarSpeed1 < 5 && CarSpeed2 < 5) //����bang��Turn_Out>250 ��־λ����18000��0��Bang)
  {
    diff_flag1 = 3;
    diff_flag2 = 3;
  }
  Turn_Out_old = Turn_Out;

  /* �ٶȿ��� */
  // d_flag1 = 0;
  /******* ���� *******/
  //ɲ��BBC
  if (Lef_pp)
  {
    if (CarSpeed1 > SpeedGoal)
    {
      MotorOut1 = -MOTOR_RANGE;
    }
    else
    {
      Lef_pp_cnt++;
      if (Lef_pp_cnt > 254)
      {
        Lef_pp = 0;
        Lef_pp_cnt = 0;
      }
      MotorOut1 = 0;
    }
  }
  //����BBC
  else if (a_flag1)
  {
    //��ɲ��ʱ
    if (speedTarget1 > 0) //DEBUGDEBUGDEBUG!!! (speedTarget1 + SpeedGoal)/2 ?
    {
      //����״̬�ٶ�С���趨�ٶ�5000bang
      if (CarSpeed1 < speedTarget1 * 0.9) //1.0)
      {
        MotorOut1 = MOTOR_RANGE; //speedTarget1 * 5000; //speedTarget1
      }
      //����״̬�ٶ��Դ����趨�ٶ�6֡2500huifu
      else if (CarSpeed1 < speedTarget1 * 1.0 + 0.1) //0.8
      {
        cnt1++;
        if (cnt1 > 5)
        {
          a_flag1 = 0;
          MotorOut1 = speedTarget1 * 2500;
        }
      }
      //����״̬�ٶȸ���ֱ��2500huifu
      else
      {
        a_flag1 = 0;
        MotorOut1 = speedTarget1 * 2500;
      }
    }
    //ɲ��ʱ
    else
    {
      if (CarSpeed1 < speedTarget1 * 1.0)
      {
        MotorOut1 = speedTarget1 * -3000; //speedTarget1
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
  }
  //����BBC
  else if (d_flag1)
  {
    if (speedTarget1 > 0)
    {
      if (CarSpeed1 > speedTarget1 * 1.1) //0 + 0.15)
      {
        MotorOut1 = -MOTOR_RANGE; //speedTarget1 * -3000;
      }
      else if (CarSpeed1 > speedTarget1 * 1.0)
      {
        cnt1++;
        if (cnt1 > 5)
        {
          d_flag1 = 0;
          MotorOut1 = speedTarget1 * 2500;
        }
      }
      else //if(speedTarget>0)//////
      {
        d_flag1 = 0;
        MotorOut1 = speedTarget1 * 2500;
      }
    }
    else
    {
      if (CarSpeed1 > speedTarget1 * 1.0)
      {
        MotorOut1 = speedTarget1 * 5000;
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
      else //if(speedTarget>0)//////
      {
        d_flag1 = 0;
        MotorOut1 = speedTarget1 * 2500;
      }
    }
  }
  //PI
  else
  {
    { //�ٶ�ƫ��E1С ������������10֡
      if (SpeedE1 < 0.15 && SpeedE1 > -0.15 && frame_flag1 == 0)
      {
        /* �״ν�����λ����ʼ��֡ */
        frame_flag1 = 1;
        frame1 = 0;
      }
      if (frame_flag1)
      {
        frame1++;
      }
      if (frame1 <= 10 && frame_flag1 == 1)
      {
        /* С��10֡ �ҿ�ʼ��֡*/
        Speed_kI1 = 0;
      }
      else
      {
        /* ������֡ ���ظ�������λ I��Ϊ0 */
        frame1 = 11;
        Speed_kI1 = PID_SPEED.I;
      }
    }
    SpeedEE1 = SpeedE1 - OldE1;
    { // �ٶȱ仯EE1�� ������������
      if (SpeedEE1 > 0.1 || SpeedEE1 < -0.1)
      {
        Speed_kP1 = 0;
      }
      else
      {
        Speed_kP1 = PID_SPEED.P;
      }
    }
    { // ����PI
      SpeedControlOutE1 = (Speed_kP1 * SpeedEE1 + Speed_kI1 * SpeedE1);
      MotorOut1 += SpeedControlOutE1;
    }
  }

  if (diff_flag1 > 0)
  //���� ������� BBC
  {
    if (Turn_Out >= CAR_DIFF_SERVO_RANGE)
    {
      MotorOut1 = MOTOR_RANGE;
    }
    else if (Turn_Out <= -CAR_DIFF_SERVO_RANGE)
    {
      MotorOut1 = 0;
    }
    else
    {
      MotorOut1 = SpeedGoal * SPEED_MOTOR_SCALE_LOW;
      diff_flag1 = -1;
    }
    
    // diff_flag1--;
    // if (diff_flag1 == 0)
    // {
    //   MotorOut1 = SpeedGoal * SPEED_MOTOR_SCALE_LOW;
    //   diff_flag1 = -1;
    // }
  }
  // d_flag2 = 0;
  /******* ���� *******/
  if (Rig_pp)
  {
    if (CarSpeed2 > SpeedGoal)
    {
      MotorOut2 = -MOTOR_RANGE;
    }
    else
    {
      Rig_pp_cnt++;
      if (Rig_pp_cnt > 254)
      {
        Rig_pp = 0;
        Rig_pp_cnt = 0;
      }
      MotorOut2 = 0;
    }
  }
  else if (a_flag2)
  {
    if (speedTarget2 > 0)
    {
      if (CarSpeed2 < speedTarget2 * 0.9) //1.0)
      {
        MotorOut2 = MOTOR_RANGE; //speedTarget2 * 5000;
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
    else
    {
      if (CarSpeed2 < speedTarget2 * 1.0)
      {
        MotorOut2 = speedTarget2 * -3000;
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
  }
  else if (d_flag2)
  {
    if (speedTarget2 > 0)
    {
      if (CarSpeed2 > speedTarget2 * 1.1) //1.0 + 0.15)
      {
        MotorOut2 = -MOTOR_RANGE; //speedTarget2 * -3000;
      }
      else if (CarSpeed2 > speedTarget2 * 1.0)
      {
        ;
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
      if (CarSpeed2 > speedTarget2 * 1.0)
      {
        MotorOut2 = speedTarget2 * 5000;
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
  }
  else
  {
    if (SpeedE2 < 0.15 && SpeedE2 > -0.15 && frame_flag2 == 0)
    {
      /* �״ν�����λ����ʼ��֡ */
      frame_flag2 = 1;
      frame2 = 0;
    }

    if (frame_flag2)
    {
      frame2++;
    }
    SpeedEE2 = SpeedE2 - OldE2;
    if (SpeedEE2 > 0.1 || SpeedEE2 < -0.1)
    {
      Speed_kP2 = 0;
    }
    else
    {
      Speed_kP2 = PID_SPEED.P;
    }
    if (frame2 <= 10 && frame_flag2 == 1)
    {
      /* С��10֡ �ҿ�ʼ��֡*/
      Speed_kI2 = 0;
    }
    else
    {
      /* ������֡ ���ظ�������λ I��Ϊ0 */
      frame2 = 11;
      Speed_kI2 = PID_SPEED.I;
    }
    SpeedControlOutE2 = (Speed_kP2 * SpeedEE2 + Speed_kI2 * SpeedE2);
    MotorOut2 += SpeedControlOutE2;
  }

  if (diff_flag2 > 0)
  //���� ������� BBC
  {
    if (Turn_Out >= CAR_DIFF_SERVO_RANGE)
    {
      MotorOut2 = 0;
    }
    else if (Turn_Out <= -CAR_DIFF_SERVO_RANGE)
    {
      MotorOut2 = MOTOR_RANGE;
    }
    else
    {
      MotorOut2 = SpeedGoal * SPEED_MOTOR_SCALE_LOW;
      diff_flag2 = -1;
    }
    // diff_flag2--;
    // if (diff_flag2 == 0)
    // {
    //   MotorOut2 = SpeedGoal * SPEED_MOTOR_SCALE_LOW;
    //   diff_flag2 = -1;
    // }
  }

  OldE1 = SpeedE1;
  OldE2 = SpeedE2;
}
/*************************************************************************
*  �������ƣ�void Mean_Turn_Out(void)
*  ����˵����ת��Ƕ�ƽ��ֵ
*  ����˵����
*  �������أ�
*  �޸�ʱ�䣺2020.7.3
*  ��    ע������Բ��
*************************************************************************/
float mean_turn_out = 0;
void Mean_Turn_Out(void)
{
  static float turn_sum = 0;
  static int turn_num = 0;

  if ((Road == 1 && Road1_flag == 4) || (Road == 2 && Road2_flag == 4))
  {
    turn_sum += Turn_Cam_Out;
    turn_num++;
    mean_turn_out = turn_sum / turn_num;
  }

  if ((Road == 1 && Road1_flag == 5) || (Road == 2 && Road2_flag == 5))
  {
    turn_sum = 0;
    turn_num = 0;
  }
}

/*********************************************
***�������ƣ�ת�����
***�������������ͷ��ǣ���Ŵ��
***���������������
***˵����
*********************************************/
void Turn_Servo_Normal()
{
  if (Road == 4)
  {
    Turn_Cam_Out = 0;
    return;
  }
  else if (Road == 7)
  {
    if (Road7_flag == 2 || Road7_flag == 6)
    {
      if (barn_state)
      {
        Turn_Cam_Out = -SERVO_RANGE;
      }
      else
      {
        Turn_Cam_Out = SERVO_RANGE;
      }
    }
    else if (Road7_flag == 5)
    {
      Turn_Cam_Out = 0;
    }
  }
  else if (Road == 1)
  {
    if (Road1_flag == 5 || Road1_flag == 3)
    {
      Turn_Cam_Out = mean_turn_out;
    }
  }
  else if (Road == 2)
  {
    if (Road2_flag == 5 || Road2_flag == 3)
    {
      Turn_Cam_Out = mean_turn_out;
    }
  }
  else if (Road == 3)
  {
    if (Road3_flag == 0)
    {
      Turn_Cam_Out = 0;
    }
    if (Road3_flag == 1)
    {
      if (barn_state)
      {
        Turn_Cam_Out = -SERVO_RANGE;
      }
      else
      {
        Turn_Cam_Out = SERVO_RANGE;
      }
    }
  }

  Turn_Out = Turn_Cam_Out;
  //Turn_diff_comp();
  Servo_Duty(-Turn_Out); //�������

}



/*************************************************************************
*  �������ƣ�void Road_shift(void)
*  ����˵����
*  ����˵����
*  �������أ�
*  �޸�ʱ�䣺2020.7.26
*  ��    ע������
*************************************************************************/
void Road_shift(void)
{
  static int Road_old = 0;
  if (Road == Road_old)
  {
    switch (Road)
    {
    case 0:
      Road0_flag_shift(0);
      break;
    case 1:
      Road1_flag_shift(0);
      break;
    case 2:
      Road2_flag_shift(0);
      break;
    case 3:
      return;
    case 4:
      Road4_flag_shift(0);
      break;
    case 7:
      Road7_flag_shift(0);
      break;
    default:
      break;
    }
    return;
  }
  // Road_flag_change_flag = 1
  switch (Road_old)
  {
  case 0:
    Road0_flag_shift(1);
    break;
  case 1:
    Road1_flag_shift(1);
    break;
  case 2:
    Road2_flag_shift(1);
    break;
  case 4:
    Road4_flag_shift(1);
    break;
  case 7:
    Road7_flag_shift(1);
    break;
  default:
    break;
  }

  switch (Road)
  {
  case 0:

    Road0_flag_shift(0);
    break;
  case 1:
    Road1_flag_shift(0);
    break;
  case 2:
    Road2_flag_shift(0);
    break;
  case 4:
    Road4_flag_shift(0);
    break;
  case 7:
    Road7_flag_shift(0);
    break;
  default:
    break;
  }
  Road_old = Road;
}

/*************************************************************************
*  �������ƣ�void Road0_flag_shift(void)
*  ����˵����
*  ����˵����
*  �������أ�
*  �޸�ʱ�䣺2020.7.26
*  ��    ע������
*************************************************************************/
// bool a_flag_pre = 0;

void Road0_flag_shift(bool reset0)
{
  static int Road0_flag_old = -1;

  if (!reset0)
  {
    if (Road0_flag == Road0_flag_old)
    {
      if (Road0_flag <= 2 && !Road0_flag0_flag)
      {
        if (fabs(Turn_Out) < 100)
        {
          if (CarSpeed1 < SpeedGoal - 0.3)
          {
            a_flag1 = 1;
          }
          else
          {
            MotorOut1 = SpeedGoal * SPEED_MOTOR_SCALE_HIGH;
          }

          if (CarSpeed2 < SpeedGoal - 0.3)
          {
            a_flag2 = 1;
          }
          else
          {
            MotorOut2 = SpeedGoal * SPEED_MOTOR_SCALE_HIGH;
          }
          // a_flag_pre = 0;
          Road0_flag0_flag = 1;
        }
      }
      return;
    }
  }
  switch (Road0_flag_old)
  {
  case -1:

    break;
  case 0:

  case 1:

  case 2:
    break;
  case 4:
    // goto: case 5
  case 5:

    lib_speed_set(DEFAULT_SPEED);
    MotorOut1 = SpeedGoal * SPEED_MOTOR_SCALE_LOW;
    MotorOut2 = MotorOut1; //SpeedGoal * SPEED_MOTOR_SCALE_LOW;
    break;
  default:
    break;
  }
  if (reset0)
  {
    Road0_flag_old = -1;
    return;
  }
  switch (Road0_flag)
  {
  case 0:
    lib_speed_set(STRAIGHT_SPEED);
    if (fabs(Turn_Out) > 100)
    {
      // a_flag_pre = 1;
      Road0_flag0_flag = 0;
    }
    else
    {
      Road0_flag0_flag = 1;
      if (CarSpeed1 < SpeedGoal - 0.3)
      {
        a_flag1 = 1;
      }
      else
      {
        MotorOut1 = SpeedGoal * SPEED_MOTOR_SCALE_HIGH;
      }

      if (CarSpeed2 < SpeedGoal - 0.3)
      {
        a_flag2 = 1;
      }
      else
      {
        MotorOut2 = SpeedGoal * SPEED_MOTOR_SCALE_HIGH;
      }
    }

    break;
  case 1:
    //goto case 2:
  case 2:
    lib_speed_set(STRAIGHT_SPEED);
    break;
  case 4:
    //goto case 5:
  case 5:
    lib_speed_set(CURVE_SPEED);
    if (CarSpeed > SpeedGoal + 0.3)
    {
      d_flag1 = 1;
      d_flag2 = 1;
    }
    else
    {
      MotorOut1 = SpeedGoal * SPEED_MOTOR_SCALE_HIGH;
      MotorOut2 = MotorOut1; //SpeedGoal * SPEED_MOTOR_SCALE_HIGH;
    }

    // MotorOut1 = SpeedGoal * SPEED_MOTOR_SCALE_HIGH;
    // MotorOut2 = MotorOut1; //SpeedGoal * SPEED_MOTOR_SCALE_HIGH;
    break;
  default:
    break;
  }
  Road0_flag_old = Road0_flag;
}
/*************************************************************************
*  �������ƣ�void Road1_flag_shift(void)
*  ����˵����
*  ����˵����
*  �������أ�
*  �޸�ʱ�䣺2020.7.26
*  ��    ע������
*************************************************************************/

void Road1_flag_shift(bool reset0)
{
  static int Road1_flag_old = -1;
  if (!reset0)
  {

    if (Road1_flag == Road1_flag_old)
      return;
  }
  switch (Road1_flag_old)
  {
  case -1:

    break;
  case 0:

  case 1:

  case 2:

  case 3:

  case 4:
  case 5:
    break;
  case 6:
    out_circle_time_temp = loop_time;
    break;
  default:
    break;
  }
  if (reset0)
  {
    Road1_flag_old = -1;
    MotorOut1 = SpeedGoal * SPEED_MOTOR_SCALE_LOW;
    MotorOut2 = MotorOut1; //SpeedGoal * SPEED_MOTOR_SCALE_LOW;
    return;
  }
  switch (Road1_flag)
  {
  case 0:
    lib_speed_set(STRAIGHT_SPEED);
    break;
  case 1:
    lib_speed_set(CURVE_SPEED);
    MotorOut1 = SpeedGoal * SPEED_MOTOR_SCALE_HIGH;
    MotorOut2 = MotorOut1; //SpeedGoal * SPEED_MOTOR_SCALE_LOW;
    break;
  case 2:
    //goto case 4:
  case 4:
    lib_speed_set(CURVE_SPEED);
    break;
  case 3:

  case 5:

  case 6:

  default:
    break;
  }
  Road1_flag_old = Road1_flag;
}
/*************************************************************************
*  �������ƣ�void Road2_flag_shift(void)
*  ����˵����
*  ����˵����
*  �������أ�
*  �޸�ʱ�䣺2020.7.26
*  ��    ע������
*************************************************************************/

void Road2_flag_shift(bool reset0)
{
  static int Road2_flag_old = -1;
  if (!reset0)
  {
    if (Road2_flag == Road2_flag_old)
      return;
  }
  switch (Road2_flag_old)
  {
  case -1:

    break;
  case 0:

  case 1:

  case 2:

  case 3:

  case 4:
  case 5:
    break;
  case 6:
    out_circle_time_temp = loop_time;
    break;
  default:
    break;
  }
  if (reset0)
  {
    Road2_flag_old = -1;
    MotorOut1 = SpeedGoal * SPEED_MOTOR_SCALE_LOW;
    MotorOut2 = MotorOut1; //SpeedGoal * SPEED_MOTOR_SCALE_LOW;
    return;
  }
  switch (Road2_flag)
  {
  case 0:
    lib_speed_set(STRAIGHT_SPEED);
    break;
  case 1:
    lib_speed_set(CURVE_SPEED);
    MotorOut1 = SpeedGoal * SPEED_MOTOR_SCALE_HIGH;
    MotorOut2 = MotorOut1; //SpeedGoal * SPEED_MOTOR_SCALE_LOW;
    break;
  case 2:
    //goto case 4:
  case 4:
    lib_speed_set(CURVE_SPEED);
    break;
  case 3:

  case 5:

  case 6:

  default:
    break;
  }
  Road2_flag_old = Road2_flag;
}

/*************************************************************************
*  �������ƣ�void Road4_flag_shift(void)
*  ����˵����
*  ����˵����
*  �������أ�
*  �޸�ʱ�䣺2020.7.26
*  ��    ע������
*************************************************************************/

void Road4_flag_shift(bool reset0)
{
  static int Road4_flag_old = -1;
  if (!reset0)
  {

    if (Road4_flag == Road4_flag_old)
      return;
  }
  switch (Road4_flag_old)
  {
  case -1:

    break;
  case 0:

  case 1:

  case 2:

  case 3:

  default:
    break;
  }
  if (reset0)
  {
    Road4_flag_old = -1;
    ramp_out_time = loop_time;
    // ramp_reset_flag = 0;
    return;
  }
  switch (Road4_flag)
  {
  case 0:
    break;
  case 1:
    lib_speed_set(UP_RAMP_SPEED);
    break;
  case 2:
    lib_speed_set(ON_RAMP_SPEED);
    break;
  case 3:
    lib_speed_set(DOWN_RAMP_SPEED);
    break;
  default:
    break;
  }
  Road4_flag_old = Road4_flag;
}
/*************************************************************************
*  �������ƣ�void Road7_flag_shift(void)
*  ����˵����
*  ����˵����
*  �������أ�
*  �޸�ʱ�䣺2020.7.26
*  ��    ע������
*************************************************************************/
void Road7_flag_shift(bool reset0)
{
  static int Road7_flag_old = -1;
  if (!reset0)
  {
    if (Road7_flag == Road7_flag_old)
      return;
  }
  switch (Road7_flag_old)
  {
  case 0:

  case 1:

  case 2:

  case 3:

  case 4:
  case 5:

  case 6:

  default:
    break;
  }
  if (reset0)
  {
    Road7_flag_old = -1;
    return;
  }
  switch (Road7_flag)
  {
  case 0:
    lib_speed_set(PRE_STOP_SPEED);
    break;
  case 1:
    lib_speed_set(RUSH_STOP_SPEED);
    break;
  case 6:
    lib_speed_set(EMERGENCY_STOP_SPEED);
    break;
  case 2:
    lib_speed_set(EMERGENCY_STOP_SPEED);
    break;
  case 3:
  lib_speed_set(STOP_SPEED);
    break;
  case 4:
    lib_speed_set(0);
    break;
  case 5:
  default:
    break;
  }
  Road7_flag_old = Road7_flag;
}


/*************************************************************************
*  �������ƣ�int8 BB_add_flag_set(void)
*  ����˵����ת��BB��λ
*  ����˵����
*  �������أ�
*  �޸�ʱ�䣺2020.7.13
*  ��    ע������
*************************************************************************/
int BB_add_flag_set(void)
{

  static int BB_add_flag = 0;
  float Turn_Out_Table[4] = {200, 100, 50, 0};
  // float Speed12_diff[4] = {0.3, 0.1, 0, -0.2};
  // float Speed12_diff2[4] = {0, -0.2, -0.3, -0.5};
  // float Speed12_diff_stop[4] = {0.5, 0.3, 0.1, 0.1};
  float Speed12_diff[4] = {0.12, 0.04, 0, -0.08};
  float Speed12_diff2[4] = {0, -0.08, -0.12, -0.16};
  float Speed12_diff_stop[4] = {0.12, 0.04, 0, 0.04};//{0.2, 0.12, 0.04, 0.04}; //0.08 = 0.1 / 2.5

  float speed_diff;
  float diff_stop_offset = 0.04; //= 0.1 / 2.5
  static bool fuhao;
  static bool diff_4_sign;
  if (CarSpeed < 1)
  {
    return 0;
  }
  if (Turn_Out < 0)
  {
    speed_diff = (CarSpeed2 - CarSpeed1) / CarSpeed;
  }
  else
  {
    speed_diff = (CarSpeed1 - CarSpeed2) / CarSpeed;
  }
  if (BB_add_flag % 10 != 0)
  {
    if ((BB_add_flag % 10 > 1 /*fangyuejie*/ && fabs(Turn_Out) >= Turn_Out_Table[BB_add_flag % 10 - 2]) ||
        fabs(Turn_Out) <= Turn_Out_Table[BB_add_flag % 10 - 1] || /*BBadd_flag==1ʱ�����*/
        (Turn_Out < 0 ^ fuhao) ||
        (BB_add_flag % 10 < 4 && speed_diff > Speed12_diff_stop[BB_add_flag % 10 - 1]) ||
        (BB_add_flag % 10 == 4 && speed_diff > diff_stop_offset * (diff_4_sign > 0 ? 1 : -1) + Speed12_diff_stop[BB_add_flag % 10 - 1]))
    {
      BB_add_flag = 0; //�����������4������add_flag����������������bang����������ά����һ֡
    }
  }
  fuhao = Turn_Out < 0;
  if (BB_add_flag % 10 == 0)
  {

    if (Turn_Out < 0)
    {
      BB_add_flag = 100;
    }
    else
    {
      BB_add_flag = 0;
    }

    for (int i = 0; i < 4; ++i)
    {
      if (fabs(Turn_Out) >= Turn_Out_Table[i])
      {
        if (speed_diff < Speed12_diff[i])
        {
          BB_add_flag += i + 1;
          if (i == 3)
          {
            diff_4_sign = speed_diff > 0;
          }
        }
        // else if (i == 3 && -speed_diff < Speed12_diff[i] )
        // {
        //BB_add_flag += i + 1;
        //BB_add_flag += 100 - 200 * (BB_add_flag / 100);
        // }

        if (speed_diff <= Speed12_diff2[i] && (Road != 0 || Road0_flag != 0))
        {
          BB_add_flag += 10;
        }
        else if (i == 3 && -speed_diff <= Speed12_diff2[i])
        {
          BB_add_flag += i + 1;
          BB_add_flag += 100 - 200 * (BB_add_flag / 100);
          diff_4_sign = speed_diff > 0;
        }

        /* -50~50�Ƕ�ʱ ****
        ��ת�����: ���ֿ� speed_diff������Ҫ��bang���� ֻ��Сbang �ָ�����0.1+0.1   *0.4
                    ���ֿ� speed_diff������Сbang����              �ָ�����0.1-0.1
        ��ת�Ҽ���: ���ֿ� speed_diff������Ҫ��bang���� ֻ��Сbang �ָ�����0.1+0.1
                    ���ֿ� speed_diff������Сbang����              �ָ�����0.1-0.1
         */
        break;
      }
    }
  }

  // if (BB_add_flag == -1) //��־λ��λ
  // {
  // if (Turn_Out < -100 || Road0_flag == 4 || (Road1_flag > 1 && Road1_flag < 6) || ((Road1_flag == 1 || Road1_flag == 6) && Turn_Out < -50))
  // {
  //   if (CarSpeed1 - CarSpeed2 > 0.1)
  //   {
  //     BB_add_flag = 4;
  //   }
  //   if (CarSpeed1 - CarSpeed2 > 0.5)
  //   {
  //     BB_add_flag = 14;
  //   }
  // }
  // else if (Turn_Out > 100 || Road0_flag == 5 || (Road2_flag > 1 && Road2_flag < 6) || ((Road2_flag == 1 || Road2_flag == 6) && Turn_Out > 50))
  // {
  //   if (CarSpeed2 - CarSpeed1 > 0.1)
  //   {
  //     BB_add_flag = 5;
  //   }
  //   if (CarSpeed2 - CarSpeed1 > 0.5)
  //   {
  //     BB_add_flag = 15;
  //   }
  // }
  // else if (Turn_Out < 50 && Road == 0)
  // {
  //   if (CarSpeed1 - CarSpeed2 > 0.3)
  //   {
  //     BB_add_flag = 0;
  //   }
  //   if (CarSpeed1 - CarSpeed2 > 0.6)
  //   {
  //     BB_add_flag = 10;
  //   }
  // }
  // else if (Turn_Out > -50 && Road == 0)
  // {
  //   if (CarSpeed2 - CarSpeed1 > 0.3)
  //   {
  //     BB_add_flag = 1;
  //   }
  //   if (CarSpeed2 - CarSpeed1 > 0.6)
  //   {
  //     BB_add_flag = 11;
  //   }
  // }

  // else if (Road == 0 && Road0_flag < 3)
  // {
  //   if (CarSpeed1 - CarSpeed2 > 0.5 && Turn_Out < 50)
  //   {
  //     BB_add_flag = 0;
  //   }
  //   else if (CarSpeed2 - CarSpeed1 > 0.5 && Turn_Out > -50)
  //   {
  //     BB_add_flag = 1;
  //   }
  // }
  // }

  // if (BB_add_flag % 10 == 1)
  // {
  //   if (speed_diff < 0.2 || Turn_Out > 70) //70budui DEBUG
  //     BB_add_flag = 0;
  // }
  // else if (BB_add_flag % 10 == 2)
  // {
  //   if (CarSpeed2 - CarSpeed1 < 0.2 || Turn_Out < -70)
  //     BB_add_flag = 0;
  // }
  // else if (BB_add_flag % 10 == 3)
  // {
  //   if (CarSpeed2 - CarSpeed1 > 0.1)
  //     BB_add_flag = 0;
  // }
  // else if (BB_add_flag % 10 == 4)
  // {
  //   if (CarSpeed1 - CarSpeed2 > 0.1)
  //   {
  //     BB_add_flag = 0;
  //   }
  // }
  return BB_add_flag;
}
/*************************************************************************
*  �������ƣ�  void BB_add(void)
*  ����˵����ת��BB
*  ����˵����
*  �������أ�
*  �޸�ʱ�䣺2020.7.13
*  ��    ע������
*************************************************************************/
// int DDDebug;
void BB_add(void)
{
  int BB_add_flag = 0;
  int temp;  //2000;
  int temp2; //500;
  BB_add_flag = BB_add_flag_set();
  // DDDebug = BB_add_flag;
  if (BB_add_flag % 10 == 0)
  {
    MotorOut1_add = 0;
    MotorOut2_add = 0;
    return;
  }
  if (BB_add_flag % 100 / 10 == 0)
  {
    temp = 1000; //2000;
    temp2 = 500; //500;
  }
  else if (BB_add_flag % 100 / 10 == 1)
  {
    temp = 2000;  //2000;
    temp2 = 1000; //500;
  }

  if (BB_add_flag / 100 == 1) //��ת
  {
    MotorOut1_add = -temp2;
    MotorOut2_add = temp;
  }
  else if (BB_add_flag / 100 == 0)
  {
    MotorOut1_add = temp;
    MotorOut2_add = -temp2;
  }
}
