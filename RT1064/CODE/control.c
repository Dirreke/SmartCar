#include "headfile.h"
float Turn_Cam_Out;
float Turn_EM_Out;
float Turn_Out;
float MotorOut1, MotorOut2;
float MotorOut1_add = 0;
float MotorOut2_add = 0;
float speedTarget1, speedTarget2;
float Turn_P_EM;
float Turn_D_EM;
bool speed_change_flag = 0;
// bool road_change_flag = 0;

int ramp_out_time = -500;
int out_circle_time_temp = -500;
float Turn_EM_Out1 = 0, Turn_EM_Out2 = 0, Turn_EM_Out = 0;
PID PID_CENTER_EM, PID_STRAIGHT_EM;

int mix_choice = 0; //1电磁为最小打角，2电磁为最大打角，3电磁不可信，0电磁中心；

/*************************************************************************
*  函数名称：void Turn_Cam(void)
*  功能说明：摄像头转弯控制程序，根据中心偏移量计算舵机输出量
*  参数说明：中心偏移量,PD 
*  函数返回：舵机打角
*  修改时间：2020.6.20
*  备    注：偏移量offset为负说明车身相对赛道中心偏左
             偏移量offset为正说明车身相对赛道中心偏右
*************************************************************************/
#if 0 
PID PID_TURN_CAM_EXT;
void Turn_Cam(void)
{
  PID PID_TURN_CAM;
  //static float Cam_offset_old = 0;
  PID_TURN_CAM = TurnFuzzyPD_Cam();
  float Turn_angle_PWM;
  static float Turn_angle_PWM_old = 0;

  //0.768=0.8*1.2*0.8
  // PID_TURN_CAM.P *= PID_TURN_CAM_EXT.P; //0.85;//0.7
  // PID_TURN_CAM.D *= PID_TURN_CAM_EXT.D;

  // Turn_Cam_Out = PID_TURN_CAM.P * Cam_offset + PID_TURN_CAM.D * (Cam_offset - Cam_offset_old); //转向PID控制
  // Cam_offset_old = Cam_offset;

  Turn_angle_PWM = PID_TURN_CAM.P * Cam_offset;
  Turn_Cam_Out = PID_TURN_CAM_EXT.P * Turn_angle_PWM + PID_TURN_CAM_EXT.D * PID_TURN_CAM.D * (Turn_angle_PWM - Turn_angle_PWM_old);
  Turn_angle_PWM_old = Turn_angle_PWM;
  //Servo_Duty(-Turn_Cam_Out);
}
/*************************************************************************
*  函数名称：void TurnFuzzyPD_Cam(void)
*  功能说明：转弯PD模糊函数------摄像头控制
*  参数说明：PD 表
*  函数返回：摄像头控制转弯PD
*  修改时间：2020.6.20
*  备    注：
*************************************************************************/
PID TurnFuzzyPD_Cam(void)
{
  PID PID_TURN_CAM;
  static const float Cam_Offset_Table0[21] = {-140, -130, -110, -100, -80, -60, -50, -40, -30, -20, 0, 20, 30, 40, 50, 60, 80, 100, 110, 130, 140};
  static const float Turn_Cam_P_Table0[21] = {1.29, 1.20, 1.15, 1.20, 1.20, 1.30, 1.35, 1.60, 1.5, 1.45, 0.3, 1.45, 1.5, 1.60, 1.35, 1.30, 1.20, 1.20, 1.15, 1.20, 1.29};
  // static const float Turn_Cam_D_Table0[21] = {1.29, 1.20, 1.15, 1.20, 1.20, 1.30, 1.35, 1.60, 1.5, 1.45, 0.01, 1.45, 1.5, 1.60, 1.35, 1.30, 1.20, 1.20, 1.15, 1.20, 1.29};
  static const float Turn_Cam_D_Table0[21] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0.5, 0.01, 0.5, 1, 1, 1, 1, 1, 1, 1, 1, 1};
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
        PID_TURN_CAM.P = Turn_Cam_P_Table0[i] + (Cam_offset - Cam_Offset_Table0[i]) * (Turn_Cam_P_Table0[i + 1] - Turn_Cam_P_Table0[i]) / (Cam_Offset_Table0[i + 1] - Cam_Offset_Table0[i]); //线性
        PID_TURN_CAM.D = Turn_Cam_D_Table0[i] + (Cam_offset - Cam_Offset_Table0[i]) * (Turn_Cam_D_Table0[i + 1] - Turn_Cam_D_Table0[i]) / (Cam_Offset_Table0[i + 1] - Cam_Offset_Table0[i]);
        break;
      }
    }
  }
  return PID_TURN_CAM;
}
#endif
/*********************************************
***函数名称：电磁转弯程序
***输入参数：平行电感、垂直电感值
***输出参数：舵机打角
***说明：

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
  // EM_straight_offset = EM_err_cal(EM_Value_2, EM_Value_3, EM_Value_1, EM_Value_4); //左平行，右平行，左垂直，右垂直 得到转角
  // TurnFuzzyPD_EM();                                                      //根据偏移写模糊PD
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
#if 0
/*********************************************
***函数名称：转弯程序
***输入参数：摄像头打角，电磁打角
***输出参数：舵机打角
***说明：
*********************************************/
void Turn_Servo()
{
  if (Road == 7)
  {
    if (Road7_flag == 2)
    {
      // #ifdef TL2barn
      //       Turn_Cam_Out = -SERVO_RANGE;
      //       Turn_Out = Turn_Cam_Out;
      // #endif
      // #ifdef TR2barn
      //       Turn_Cam_Out = SERVO_RANGE;
      //       Turn_Out = Turn_Cam_Out;
      // #endif
      if (barn_state)
      {
        Turn_Cam_Out = -SERVO_RANGE;
        Turn_Out = Turn_Cam_Out;
      }
      else
      {
        Turn_Cam_Out = SERVO_RANGE;
        Turn_Out = Turn_Cam_Out;
      }
    }
    else if (Road7_flag == 3)
    {
      Turn_Out = Turn_Cam_Out;
    }
    else if (Road7_flag == 4)
    {
      Turn_Out = Turn_Cam_Out;
    }
    else if (Road7_flag == 5)
    {
      Turn_Out = 0;
    }
  }

  else if (Road == 1)
  {
    if (Road1_flag == 5 || Road2_flag == 3)
    {
      // if (Turn_Cam_Out > -0.5 * SERVO_RANGE)
      // {
      //   Turn_Cam_Out = -0.5 * SERVO_RANGE;
      // }
      Turn_Cam_Out = mean_turn_out;
      Turn_Out = Turn_Cam_Out;
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
    if (Road2_flag == 5 || Road2_flag == 3)
    {
      // if (Turn_Cam_Out < 0.5 * SERVO_RANGE)
      // {
      //   Turn_Cam_Out = 0.5 * SERVO_RANGE;
      // }
      Turn_Cam_Out = mean_turn_out;
      Turn_Out = Turn_Cam_Out;
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
  else if (Road == 4)
  {
    Turn_Out `= Turn_EM_Out;
  }
  else if (Road == 3)
  {
    if (Road3_flag == 0)
    {
      Turn_Cam_Out = 0;
      Turn_Out = Turn_Cam_Out;
    }
    if (Road3_flag == 1)
    {
      // #ifdef TL2barn
      //       Turn_Cam_Out = -SERVO_RANGE;
      //       Turn_Out = Turn_Cam_Out;
      // #endif
      // #ifdef TR2barn
      //       Turn_Cam_Out = SERVO_RANGE;
      //       Turn_Out = Turn_Cam_Out;
      // #endif
      if (barn_state)
      {
        Turn_Cam_Out = -SERVO_RANGE;
        Turn_Out = Turn_Cam_Out;
      }
      else
      {
        Turn_Cam_Out = SERVO_RANGE;
        Turn_Out = Turn_Cam_Out;
      }
    }
    else
    {
      Turn_Out = Turn_Cam_Out;
    }
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
  //Turn_diff_comp();
  Servo_Duty(-Turn_Out); //舵机控制
}
#endif
/*************************************************************************
*  函数名称：void SpeedTarget_fig(void)
*  功能说明：计算速度目标量
*  参数说明：
*  函数返回：速度目标量
*  修改时间：2020.6.20
*  备    注：
*************************************************************************/
// float DIFF_KKK = 0;
// float DIFF_KK = 1;
PID PID_diff;
PID PID_diff0;
uint8 diff_BB_flag = 0;
void SpeedTarget_fig(void)
{
  float angle_val; // 用来表示实际转向角度
  float diff_K0;   // 差速率=差速比均速，左右轮各一半
  float Turn_Cam_Out_temp = Turn_Cam_Out;
  float PID_diff_P;
  angle_val = Turn_Cam_Out_temp * ANGLE_DIVIDE_SERVO_SCALE;
  diff_BB_flag = 0;

  // Turn_Cam_Out_temp = (Turn_Cam_Out > 490) ? 490 : ((Turn_Cam_Out < -490) ? -490 : Turn_Cam_Out);
  /* 开关差速 diff_K0计算 */
  // if (get_diff_state() == DIFF_ON_VAL)
  // {
  { //Turn_Cam_Out的限幅，死区
    //开关差速在Para中定义
    // if (fabs(Turn_Cam_Out_temp) < 46)
    // {
    //   angle_val = 0;
    //   diff_off();
    // }
    if (fabs(Turn_Cam_Out_temp) > 300)
    {
      PID_diff_P = 2;
      // diff_BB_flag = 1;
      diff_on();
    }
    else if (Turn_Cam_Out_temp > 250)
    {
      PID_diff_P = 1;
      diff_on();
      // diff_BB_flag = 1;
    }
    else if (fabs(Turn_Cam_Out_temp) > 190) //262.5 * PID_CAR_STRAIGHT_CAM.P) //SERVO_RANGE)
    {
      // angle_val = ANGLE_RANGE; //((Turn_Cam_Out_temp - SERVO_RANGE) * DIFF_KKK + SERVO_RANGE) * ANGLE_DIVIDE_SERVO_SCALE;
      diff_on();
      PID_diff_P = PID_diff.P;
    }
    else
    {
      angle_val = Turn_Cam_Out_temp * ANGLE_DIVIDE_SERVO_SCALE; // * DIFF_KK;
      diff_on();
      PID_diff_P = PID_diff0.P;
    }
    // angle_val = (fabs(Turn_Cam_Out_temp) < 46) ? 0 : Turn_Cam_Out_temp / SERVO_RANGE * ANGLE_RANGE;
    // angle_val = (Turn_Cam_Out_temp > SERVO_RANGE) ? ((Turn_Cam_Out_temp - SERVO_RANGE) * DIFF_KKK + SERVO_RANGE) / SERVO_RANGE * ANGLE_RANGE : Turn_Cam_Out_temp / SERVO_RANGE * ANGLE_RANGE;
    // angle_val = (Turn_Cam_Out_temp < -SERVO_RANGE) ? ((Turn_Cam_Out_temp + SERVO_RANGE) * DIFF_KKK - SERVO_RANGE) / SERVO_RANGE * ANGLE_RANGE : Turn_Cam_Out_temp / SERVO_RANGE * ANGLE_RANGE;
    // if (angle_val > 1.5)
    // {
    //   angle_val = 1.5;
    // }
    // else if (angle_val < -1.5)
    // {
    //   angle_val = -1.5;
    // }
  }
  //可串PD控制器
  { //特定道路关差速（出库、坡)
    if (Road == 4 || Road == 3)
    {
      diff_K0 = 0;
    }
    else
    {
      diff_K0 = CAR_DIFF_K * tan(angle_val);
    }
  }
  // }
  if (get_diff_state() == DIFF_OFF_VAL)
  {
    diff_K0 = 0;
  }
  /* 入库差速增益补偿 */
  if (Road == 7 && Road7_flag == 2)
  {
    diff_K0 *= 2;
  }
  else
  {
    diff_K0 = diff_K0 * PID_diff_P;
  }

  /* 左右轮目标速度计算 */
  speedTarget1 = SpeedGoal * (1 + diff_K0 / 2); //左侧车轮
  speedTarget2 = SpeedGoal * (1 - diff_K0 / 2); //右侧车轮

  //后面可加上下坡部分

  // if (barn_reset_flag == 1)
  // {
  //   if (CarSpeed1 >= 3 || CarSpeed2 >= 3)
  //   {
  //     //SpeedGoal = 0;
  //   }
  // }
}
#if 0
void lib_set_fun(void)
{
  static bool ss_flag;
  static bool tt_flag = 1;
  if (!gpio_get(DEBUG_KEY0))
  {
    return;
  }
  if (Road == 7)
  {
    if ((Road7_flag == 0 || Road7_flag == 1 || Road7_flag == 2 || Road7_flag == 6) && tt_flag)
    {
      lib_speed_set(2.5);
      if (Road7_flag == 2)
      {
        lib_speed_set(1.5);
      }
      diff_on();
      tt_flag = 0;
    }
    else if (Road7_flag == 4)
    {
      lib_speed_set(0);
    }
    else if (Road7_flag == 6)
    {
      lib_speed_set(1.0);
    }
  }
  else if (Road == 0)
  {
    if (Road0_flag == 4 && ss_flag)
    {
      lib_speed_set(curvespeedgoal);
      speed_change_flag = 1;
      ss_flag = 0;
    }
    else if (Road0_flag == 5 && ss_flag)
    {
      lib_speed_set(curvespeedgoal);
      speed_change_flag = 1;
      ss_flag = 0;
    }
    else if (Road0_flag == 0)
    {
      lib_speed_set(speedgoal);
      ss_flag = 1;
      tt_flag = 1;
    }
  }
  else if (Road == 1 || Road == 2)
  {
    // speed_
  }

  if (Road != 3 && loop_time > 500)
  {
    if (EM_Value_2 < 0.3 && EM_Value_3 < 0.3 && EM_Value_1 < 0.3 && EM_Value_4 < 0.3)
    {
      lib_speed_set(0);
    }
  }
}
#endif
/*************************************************************************
*  函数名称：void Speed_Control(L_flag)
*  功能说明：速度PI+bang
*  参数说明：1左0右
*  函数返回：电机占空比//电机输出放到外面了
*  修改时间：2020.7.10
*  备    注：
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
  /* 计算速度偏差 */
  SpeedE1 = speedTarget1 - CarSpeed1;
  SpeedE2 = speedTarget2 - CarSpeed2;
  SpeedGoalE1 = SpeedGoal - CarSpeed1;
  SpeedGoalE2 = SpeedGoal - CarSpeed2;
  /* 变设定速大bang */
  if (speed_change_flag)
  {
    //停车减速bang
    if (SpeedGoal == 0)
    {
      MotorOut1 = 0;
      MotorOut2 = 0;
      //return;
      Lef_pp = 1;
      Rig_pp = 1;
    }
    //加速bang
    else
    {
      Lef_BB = 1;
      Rig_BB = 1;
    }
    speed_change_flag = 0;
  }

  if (SpeedGoalE1 > 1)
  {
    //即使速度目标不改变但speede1>1.5也直接进BB
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
  if (SpeedGoalE2 > 1)
  {
    //即使速度目标不改变但speede2>1.5也直接进BB
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

  if (SpeedGoalE1 < -1)
  {
    //同上
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
    //同上
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
  //弯道不bang
  if (fabs(Turn_Out) > 100 && fabs(Turn_Out) < 250)
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
  else if (fabs(Turn_Out) >= 250 && (diff_flag1 < 0 || diff_flag2 < 0) && CarSpeed1 < 6 && CarSpeed2 < 6) //差速bang，Turn_Out>250 标志位，给18000和0的Bang)
  {
    diff_flag1 = 3;
    diff_flag2 = 3;
  }

  /* 速度控制 */
  // d_flag1 = 0;
  /******* 左轮 *******/
  //刹车BBC
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
  //加速BBC
  else if (a_flag1)
  {
    //不刹车时
    if (speedTarget1 > 0) //DEBUG!!! (speedTarget1 + SpeedGoal)/2 ?
    {
      //加速状态速度小于设定速度5000bang
      if (CarSpeed1 < speedTarget1 * 1.0)
      {
        MotorOut1 = MOTOR_RANGE;//speedTarget1 * 5000; //speedTarget1
      }
      //加速状态速度稍大于设定速度6帧2500huifu
      else if (CarSpeed1 < speedTarget1 * 1.1) //0.8
      {
        cnt1++;
        if (cnt1 > 5)
        {
          a_flag1 = 0;
          MotorOut1 = speedTarget1 * 2500;
        }
      }
      //加速状态速度更大直接2500huifu
      else
      {
        a_flag1 = 0;
        MotorOut1 = speedTarget1 * 2500;
      }
    }
    //刹车时
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
  //减速BBC
  else if (d_flag1)
  {
    if (speedTarget1 > 0)
    {
      if (CarSpeed1 > speedTarget1 * 1.0 + 0.15)
      {
        MotorOut1 = -MOTOR_RANGE;//speedTarget1 * -3000;
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
    { //速度偏差E1小 剪除积分作用10帧
      if (SpeedE1 < 0.15 && SpeedE1 > -0.15 && frame_flag1 == 0)
      {
        /* 首次进入置位，开始数帧 */
        frame_flag1 = 1;
        frame1 = 0;
      }
      if (frame_flag1)
      {
        frame1++;
      }
      if (frame1 <= 10 && frame_flag1 == 1)
      {
        /* 小于10帧 且开始数帧*/
        Speed_kI1 = 0;
      }
      else
      {
        /* 继续数帧 不重复进入置位 I不为0 */
        frame1 = 11;
        Speed_kI1 = PID_SPEED.I;
      }
    }
    SpeedEE1 = SpeedE1 - OldE1;
    { // 速度变化EE1大 剪除比例作用
      if (SpeedEE1 > 0.1 || SpeedEE1 < -0.1)
      {
        Speed_kP1 = 0;
      }
      else
      {
        Speed_kP1 = PID_SPEED.P;
      }
    }
    { // 增量PI
      SpeedControlOutE1 = (Speed_kP1 * SpeedEE1 + Speed_kI1 * SpeedE1);
      MotorOut1 += SpeedControlOutE1;
    }
  }

  if (diff_flag1 > 0)
  //差速 舵机打死 BBC
  {
    if (Turn_Out >= 250)
    {
      MotorOut1 = MOTOR_RANGE;
    }
    else if (Turn_Out <= -250)
    {
      MotorOut1 = 0;
    }
    diff_flag1--;
    if (diff_flag1 == 0)
    {
      MotorOut1 = SpeedGoal * SPEED_MOTOR_SCALE_LOW;
      diff_flag1 = -1;
    }
  }
  // d_flag2 = 0;
  /******* 右轮 *******/
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
      if (CarSpeed2 < speedTarget2 * 1.0)
      {
        MotorOut2 = MOTOR_RANGE;//speedTarget2 * 5000;
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
      if (CarSpeed2 > speedTarget2 * 1.0 + 0.15)
      {
        MotorOut2 = -MOTOR_RANGE;//speedTarget2 * -3000;
      }
      else if (CarSpeed2 > speedTarget2 * 1.0)
      {;
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
      /* 首次进入置位，开始数帧 */
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
      /* 小于10帧 且开始数帧*/
      Speed_kI2 = 0;
    }
    else
    {
      /* 继续数帧 不重复进入置位 I不为0 */
      frame2 = 11;
      Speed_kI2 = PID_SPEED.I;
    }
    SpeedControlOutE2 = (Speed_kP2 * SpeedEE2 + Speed_kI2 * SpeedE2);
    MotorOut2 += SpeedControlOutE2;
  }

  if (diff_flag2 > 0)
  //差速 舵机打死 BBC
  {
    if (Turn_Out >= 250)
    {
      MotorOut2 = 0;
    }
    else if (Turn_Out <= -250)
    {
      MotorOut2 = MOTOR_RANGE;
    }
    diff_flag2--;
    if (diff_flag2 == 0)
    {
      MotorOut2 = SpeedGoal * SPEED_MOTOR_SCALE_LOW;
      diff_flag2 = -1;
    }
  }

  OldE1 = SpeedE1;
  OldE2 = SpeedE2;
}
#if 0
void Kalman_Filter(void)
{
  /* 控制向量uk：舵机打角变化dδ */
  /* 控制矩阵B：角度速度耦合矩阵 */
  /* 状态向量xk：当前状态（frenet坐标系，n：车与中心线法向距离，μ：车与中心线切线夹角） */
  /* 速度方向β：tanδ = 2 * tanβ */
  /* 状态方程： nk = nk+1 + vxsinμ + vycosμ */
  /* 状态方程： μk = μk-1 + r - κ(s)ds*/
  /* 状态转移矩阵Fk：  */

  float accuracy_EM = 0;
  float accuracy_Cam = 0;
  float K = 0;
  accuracy_EM = 1;

  if (Road == 7)
  {
    if (Road7_flag == 2)
    {
      // #ifdef TL2barn
      //       Turn_Cam_Out = -SERVO_RANGE;
      //       Turn_Out = Turn_Cam_Out;
      // #endif
      // #ifdef TR2barn
      //       Turn_Cam_Out = SERVO_RANGE;
      //       Turn_Out = Turn_Cam_Out;
      // #endif
      if (barn_state)
      {
        Turn_Cam_Out = -SERVO_RANGE;
        Turn_Out = Turn_Cam_Out;
      }
      else
      {
        Turn_Cam_Out = SERVO_RANGE;
        Turn_Out = Turn_Cam_Out;
      }
    }
    else if (Road7_flag == 3)
    {
      accuracy_EM = 0;
      accuracy_Cam = 1;
    }
    else if (Road7_flag == 4)
    {
      accuracy_Cam = 1;
      accuracy_EM = 0;
    }
    else if (Road7_flag == 5)
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
      Servo_Duty(-Turn_Out); //舵机控制
      return;
    }
    else if (Road1_flag == 4)
    {
      if (EM_edge > 0)
      {
        accuracy_EM = 0;
      }
      else
      {
        accuracy_EM = 0.3;
      }
      accuracy_Cam = 1;
    }
    else
    {
      accuracy_EM = 0;
      accuracy_Cam = 1;
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
      Servo_Duty(-Turn_Out); //舵机控制
      return;
    }
    else if (Road2_flag == 4)
    {
      if (EM_edge > 0)
      {
        accuracy_EM = 0;
      }
      else
      {
        accuracy_EM = 0.3;
      }
      accuracy_Cam = 1;
    }
    else
    {
      accuracy_EM = 0;
      accuracy_Cam = 1;
    }
  }

  else if (Road == 3)
  {
    if (Road3_flag == 1)
    {
      // #ifdef TL2barn
      //       Turn_Out = -SERVO_RANGE;
      // #endif
      // #ifdef TR2barn
      //       Turn_Out = SERVO_RANGE;
      // #endif
      if (barn_state)
      {

        Turn_Out = -SERVO_RANGE;
      }
      else
      {
        Turn_Out = SERVO_RANGE;
      }
      Servo_Duty(-Turn_Out); //舵机控制
      return;
    }
    else
    {
      accuracy_EM = 0;
      accuracy_Cam = 1;
    }
  }

  else
  {
    if (Road0_flag == 1 || Road0_flag == 2)
    {
      accuracy_EM = 0;
      accuracy_Cam = 1;
    }
    else if (EM_edge > 2)
    {
      accuracy_EM = 0;
      accuracy_Cam = 1;
    }
    else
    {
      accuracy_EM = 0.3;
      accuracy_Cam = 0.7;
    }
  }

  K = accuracy_EM / (accuracy_Cam + accuracy_EM);
  Turn_Out = (1 - K) * Turn_Cam_Out + K * Turn_EM_Out;
  Servo_Duty(-Turn_Out); //舵机控制
}
#endif
/*************************************************************************
*  函数名称：void Mean_Turn_Out(void)
*  功能说明：转向角度平均值
*  参数说明：
*  函数返回：
*  修改时间：2020.7.3
*  备    注：用于圆环
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
***函数名称：转弯程序
***输入参数：摄像头打角，电磁打角
***输出参数：舵机打角
***说明：
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
  Servo_Duty(-Turn_Out); //舵机控制
  // Turn_state = (int) Turn_Out /50;
  //   if(fabs(Turn_Out) > 100 && fabs(Turn_Out) < 250)
  //   {
  // jmksaytomain_flag = 1;
  //   }
  //   if()
}

// void Turn_state_set(void)
// {
//   if(Turn_Out>200)
//   {

//   }

// }

/*************************************************************************
*  函数名称：void Road_shift(void)
*  功能说明：
*  参数说明：
*  函数返回：
*  修改时间：2020.7.26
*  备    注：用于
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
*  函数名称：void Road0_flag_shift(void)
*  功能说明：
*  参数说明：
*  函数返回：
*  修改时间：2020.7.26
*  备    注：用于
*************************************************************************/
bool a_flag_pre = 0;

void Road0_flag_shift(bool reset0)
{
  static int Road0_flag_old = -1;

  if (!reset0)
  {
    if (Road0_flag == Road0_flag_old)
    {
      if (Road0_flag <= 2 && a_flag_pre)
      {
        if (fabs(Turn_Out) < 50)
        {
          if (CarSpeed < SpeedGoal - 0.3)
          {
            a_flag1 = 1;
            a_flag2 = 1;
          }
          else
          {
            MotorOut1 = SpeedGoal * SPEED_MOTOR_SCALE_HIGH;
            MotorOut2 = MotorOut1; //SpeedGoal * SPEED_MOTOR_SCALE_HIGH;
          }
          a_flag_pre = 0;
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
    if (fabs(Turn_Out) > 50)
    {
      a_flag_pre = 1;
    }
    else
    {
      if (CarSpeed < SpeedGoal - 0.3)
      {
        a_flag1 = 1;
        a_flag2 = 1;
      }
      else
      {
        MotorOut1 = SpeedGoal * SPEED_MOTOR_SCALE_HIGH;
        MotorOut2 = MotorOut1; //SpeedGoal * SPEED_MOTOR_SCALE_HIGH;
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
*  函数名称：void Road1_flag_shift(void)
*  功能说明：
*  参数说明：
*  函数返回：
*  修改时间：2020.7.26
*  备    注：用于
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
*  函数名称：void Road2_flag_shift(void)
*  功能说明：
*  参数说明：
*  函数返回：
*  修改时间：2020.7.26
*  备    注：用于
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
*  函数名称：void Road4_flag_shift(void)
*  功能说明：
*  参数说明：
*  函数返回：
*  修改时间：2020.7.26
*  备    注：用于
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
    lib_speed_set(UP_RAMP_SPEED);
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
*  函数名称：void Road7_flag_shift(void)
*  功能说明：
*  参数说明：
*  函数返回：
*  修改时间：2020.7.26
*  备    注：用于
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
*  函数名称：void Curve_shift(void)
*  功能说明：
*  参数说明：
*  函数b返回：
*  修改时间：2020.7.26
*  备    注：用于
*************************************************************************/
int curve_state = 0;
void Curve_shift(void)
{
  bool enter_curve_flag = 0;
  bool in_curve_flag = 0;
  bool out_curve_flag = 0;
  static float old_slope = 0;

  if (fabs(Turn_Cam_Out) > 70 && fabs(Mid_slope) > 0.3 && fabs(Mid_slope) < 0.56 && fabs(Mid_slope) - fabs(old_slope) > 0)
  {
    enter_curve_flag = 1;
  }
  else if (fabs(Mid_slope) < 0.25 && enter_curve_flag)
  {
    enter_curve_flag = 0;
    in_curve_flag = 1;
  }
  else if (in_curve_flag && fabs(Mid_slope) > 0.3 && fabs(Mid_slope) < 0.65 && fabs(Mid_slope) - fabs(old_slope) > 0)
  {
    out_curve_flag = 1;
    in_curve_flag = 0;
  }
  else
  {
    out_curve_flag = 0;
    in_curve_flag = 0;
    enter_curve_flag = 0;
  }
  curve_state = enter_curve_flag * 100 + in_curve_flag * 10 + out_curve_flag;
}

/*************************************************************************
*  函数名称：int8 BB_add_flag_set(void)
*  功能说明：转向BB置位
*  参数说明：
*  函数返回：
*  修改时间：2020.7.13
*  备    注：用于
*************************************************************************/
int BB_add_flag_set(void)
{

  static int BB_add_flag = 0;
  float Turn_Out_Table[4] = {200, 100, 50, 0};
  // float Speed12_diff[4] = {0.3, 0.1, 0, -0.2};
  // float Speed12_diff2[4] = {0, -0.2, -0.3, -0.5};
  // float Speed12_diff_stop[4] = {0.5, 0.3, 0.1, 0};
  float Speed12_diff[4] = {0.12, 0.04, 0, -0.08};
  float Speed12_diff2[4] = {0, -0.08, -0.12, -0.2};
  float Speed12_diff_stop[4] = {0.2, 0.12, 0.04, 0};

  float speed_diff;
  static bool fuhao;
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
        fabs(Turn_Out) <= Turn_Out_Table[BB_add_flag % 10 - 1] || /*BBadd_flag==1时用这个*/
        (Turn_Out < 0 ^ fuhao) ||
        speed_diff > Speed12_diff_stop[BB_add_flag % 10 - 1])
    {
      BB_add_flag = 0; //如果满足条件4，不会add_flag，其他条件重新判bang，都不满足维持上一帧
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
      if (fabs(Turn_Out) > Turn_Out_Table[i])
      {
        if (speed_diff < Speed12_diff[i])
        {
          BB_add_flag += i + 1;
        }
        // else if (i == 3 && -speed_diff < Speed12_diff[i] )
        // {
          //BB_add_flag += i + 1;
          //BB_add_flag += 100 - 200 * (BB_add_flag / 100);
        // }

        if (speed_diff < Speed12_diff2[i])
        {
          BB_add_flag += 10;
        }
        else if (i == 3 && -speed_diff < Speed12_diff2[i] )
        {
          BB_add_flag += i + 1;
          BB_add_flag += 100 - 200 * (BB_add_flag / 100);
        }
        break;
      }
    }
  }

  // if (BB_add_flag == -1) //标志位置位
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
*  函数名称：  void BB_add(void)
*  功能说明：转向BB
*  参数说明：
*  函数返回：
*  修改时间：2020.7.13
*  备    注：用于
*************************************************************************/
int DDDebug;
void BB_add(void)
{
  int BB_add_flag = 0;
  int temp;  //2000;
  int temp2; //500;
  BB_add_flag = BB_add_flag_set();
  DDDebug = BB_add_flag;
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

  if (BB_add_flag / 100 == 1) //左转
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
