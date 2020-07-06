#include "headfile.h"
#include <queue>
//params init
int Road03_count = 0, Road04_count = 0;
float centerAngle;

// 赛道识别
#if 0
char Road1_turnout = 1;
void Road_rec(void)
{

  // static int oldwhite=5000;
  // static uint8 Road1_cnt1=0;
  // static char Road1_flag1=0;
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////????????????

  //if road=0 : straight line/turn left/turn right
  if(Road == 0){
    if((Rig_break_point > 30 && Lef_circle == 0 && Rig_circle == 1 && Rig_slope != 998 )) //左转弯
    {
      Road03_count++;
      if (Road03_count == 2){
        Road0_flag = 3;//turn left flag
        Road03_count = 0;//reset
      }
    }
    else if ((Lef_break_point > 30 && Rig_circle == 0 && Lef_circle == 1 && Lef_slope != 998 )) //右转弯
    {
      Road04_count++;
      if (Road04_count == 2){
        Road0_flag = 4;//turn right flag
        Road04_count = 0;//reset
      }
    }
    else if ((Lef_break_point < 20 && Rig_circle == 0 && Lef_circle == 1 && Rig_slope == 998 && Lef_slope != 998 && Rig[13] - Rig[11] < 5 && Rig[11] - Rig[9] < 5 && Rig[9] - Rig[7] < 5 && Rig[7] - Rig[5] < 5 && Rig[5] - Rig[3] < 5 && Rig[11] != 78 && New_Lef[50] == -MIDMAP && Rig_edge < 10)) //?????????????,????????????
    {
      Road0_flag = 0;
      Road11_count++;
      if (Road11_count == 2 && Road1_flag == 0)
      {
        Road = 1;
        Road11_count = 0;
        return;
      }
      return;
    }
    else if (Rig_break_point < 20 && Lef_circle == 0 && Rig_circle == 1 && Lef_slope == 998 && Rig_slope != 998 && Lef[3] - Lef[5] < 5 && Lef[5] - Lef[7] < 5 && Lef[7] - Lef[9] < 5 && Lef[9] - Lef[11] < 5 && Lef[11] - Lef[13] < 5 && Lef[11] != 2 && New_Rig[50] == MIDMAP && Lef_edge < 10) //??????????????????????????????
    {
      Road0_flag = 0;
      Road21_count++;
      if (Road21_count == 2 && Road2_flag == 0)
      {
        Road = 2;
        Road21_count = 0;
        return;
      }
      return;
    }
  }
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////??????????????

//if Road == 1 :left circle
  else if (Road == 1){
    if (Road1_flag == 0) //??????????
    {
      Road0_flag = 0;
      Road1_flag = 1;
      /*
      if(EM_Value_2+EM_Value_3>3.8)//??????????????????????????????
      {
        Road12_count++;
        if(Road12_count==2)
        {
          Road12_count=0;
          Road1_flag=1;//???????????????
        }
        return;
      }
      */
      return;
    }
    else if (Road1_flag == 1) //???????1/4
    {
      Road0_flag = 0;
      if (Lef_circle == 0 || (Lef_circle == 1 && Lef_break_point > 45)) //if(((Lef_circle==0||( Lef_circle ==1 && Lef_break_point>30)))&& Road1_turnin(EM_Value_2,EM_Value_3,3.8))//if(((Lef_circle==0||( Lef_circle ==1 && Lef_break_point>30))) && Rig_slope>=10)/ && Road1_turnin(EM_Value_2,EM_Value_3,3.8))    //Rig_slope<1 && (Lef_leap[0]==0||Lef_slope==999)&& Rig_leap[0]==0)
      {
        Road13_count++;
        if (Road13_count == 2) //2??? ??????????????
        {
          Road1_flag = 2;
          //        Road1_flag1 = 1;
          Road13_count = 0;
          return;
        }
        return;
      }
    }
    else if (Road1_flag == 2) //???????2/4 ????????????
    {
      Road0_flag = 0;
      Road14_count++;
      if (Road14_count == (int)(DIS_IN_CIRCLE * 10000 / (get_speed() * CAMERA_FPS)) + 1) //??????function.h
      {
        Road1_flag = 4;
        Road14_count = 0;
        return;
      }
      return;
    }
    else if (Road1_flag == 4) //????????? ?????????
    {
      Road0_flag = 0;
      if (Rig_circle && whitecnt > 2700) //
      {
        Road14_count++;
        if (Road14_count == 3)
        {
          Road1_flag = 3;
          Road14_count = 0;
          return;
        }
      }
      return;
    }
    else if (Road1_flag == 3) //????????????· ?? ???????
    {
      Road0_flag = 0;
      if ((Rig_slope > -0.02 && Rig_slope < 0) || (Pixle[58][74] == 1 && Pixle[57][74] == 1 && Pixle[56][74] == 1 && Pixle[55][74] == 1 && Pixle[54][74] == 1 && Pixle[53][74] == 1)) //|| Lef_edge < 20))
      {
        Road15_count++;
        if (Road15_count == 4)
        {
          Road15_count = 0;
          Road = 0;
          //Road1_turnout=1;
          Road1_flag = 0;
          return;
        }
      }
      return;
    }
  }
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////???????????????????????????????

//if Road == 2:right circle
  else if (Road == 2){
    if (Road2_flag == 0) //
    {
      Road0_flag = 0;
      Road2_flag = 1;
      /*
      if( EM_Value_2 +EM_Value_3 >3.8)//??????????????????????????????
      {
        Road22_count++;
        if(Road22_count==2)
        {
          Road22_count=0;
          Road2_flag=1;//???????????????
        }
        return;
      }
      */
      return;
    }
    else if (Road2_flag == 1) //
    {
      Road0_flag = 0;
      if ((Rig_circle == 0 || (Rig_circle == 1 && Rig_break_point > 30)) && Rig_slope >= 10) //if(((Rig_circle==0||( Rig_circle ==1 && Rig_break_point>30))) && Rig_slope>=10)/ && Road1_turnin(EM_Value_2,EM_Value_3,3.4))
      {
        Road23_count++;
        if (Road23_count == 2) //
        {

          Road2_flag = 2;
          Road23_count = 0;
          return;
        }
      }
      return;
    }
    else if (Road2_flag == 2) //
    {
      Road0_flag = 0;
      Road24_count++;
      if (Road24_count == (int)(DIS_IN_CIRCLE * 10000 / (get_speed() * CAMERA_FPS)) + 1)
      {
        Road24_count = 0;
        Road2_flag = 4;
        return;
      }
      return;
    }
    else if (Road2_flag == 4)
    {
      Road0_flag = 0;
      if (whitecnt > 2700)
      {
        Road25_count++;
        if (Road25_count == 3)
        {
          Road25_count = 0;
          Road2_flag = 3;
          return;
        }
      }
      return;
    }
    else if (Road2_flag == 3)
    {
      Road0_flag = 0;
      if ((Lef_slope > 0 && Lef_slope < 0.02) || (Pixle[58][5] == 1 && Pixle[57][5] == 1 && Pixle[56][5] == 1 && Pixle[55][5] == 1 && Pixle[54][5] == 1 && Pixle[53][5] == 1)) //|| Lef_edge < 20))
      {
        Road25_count++;
        if (Road25_count == 4)
        {
          Road25_count = 0;
          Road = 0;
          //Road2_turnout=1;
          Road2_flag = 0;
          return;
        }
      }
      return;
    }
  }

  if (Road == 0 && whitecnt > 2700 && ((Lef_edge > 10 && Rig_edge > 10) || Lef_edge > 30 || Rig_edge > 30) && Allwhitestart < 40)
  {
    Road0_flag = 1;
  }
  else if ((Allwhitestart > 50 && Allwhiteend < 40) || whitecnt > 3000)
  {
    Road0_flag = 2;
  }
  else
  {
    Road0_flag = 0;
  }
}
#endif
/*************************************************************************
 *  函数名称：void car_center_pid()
 *  功能说明：车正 与中心线位置差pid
 *  参数说明：无 (pixel)
 *  函数返回：无
 *  修改时间：2019.12.28
 *  备    注：与车直合用
 * **********************************************************************/

struct _pid
{
  float err;          //定义偏差值
  float err_lastlast; //定义上上个偏差值
  float err_last;     //定义上个偏差值
  //float Kp, Ki, Kd;   //定义比例、积分、微分系数
  float Kp, Kd;      //定义比例、微分系数
  float voltage;     //定义电压值（控制执行器的变量）
  float integral;    //定义积分值
  float ActualAngle; //输出转角
};

_pid pid_center;

void PID_init_center()
{
  printf("PID_init begin \n");
  pid_center.err = 0.0;
  pid_center.err_last = 0.0;
  pid_center.err_lastlast = 0.0;
  pid_center.voltage = 0.0;
  pid_center.integral = 0.0;
  pid_center.Kp = 0.2;
  //pid_center.Ki = 0.015;
  pid_center.Kd = 0.2;
  printf("pid_center_init end \n");
}

float PID_realize_center(float err)
{
  pid_center.err = err;
  float incrementAngle = pid_center.Kp * (pid_center.err - pid_center.err_last) + pid_center.Kd * (pid_center.err - 2 * pid_center.err_last + pid_center.err_lastlast);
  //+ pid_center.Ki * pid_center.err //不用i调节
  pid_center.ActualAngle += incrementAngle;
  pid_center.err_lastlast = pid_center.err_last;
  pid_center.err_last = pid_center.err;
  return pid_center.ActualAngle;
}

float car_center()
{

  // PID_init_center();
  float car_center_dias = 0;
  int car_center_start = 3;
  int car_center_end = 8;
  for (int i = car_center_start; i < car_center_end; ++i)
  {
    if (New_Mid[i] != 999)
    {
      car_center_dias += New_Mid[i];
    }
  }
  car_center_dias /= (car_center_start - car_center_end);
  return car_center_dias;
  // centerAngle=PID_realize_center(car_center_dias);
}

/*************************************************************************
 *  函数名称：void car_straight_pid()
 *  功能说明：车直 与中线角度差pid
 *  参数说明：无 (theta)
 *  函数返回：无
 *  修改时间：2019.12.28
 *  备    注：给gmydl的车直加(打)pid(call)
 * **********************************************************************/
_pid pid_straight;

void PID_init_straight()
{
  printf("PID_init begin \n");
  pid_straight.err = 0.0;
  pid_center.err_last = 0.0;
  pid_straight.err_lastlast = 0.0;
  pid_straight.voltage = 0.0;
  pid_straight.integral = 0.0;
  pid_straight.Kp = 0.2;
  pid_straight.Ki = 0.015;
  pid_straight.Kd = 0.2;
  printf("pid_straight_init end \n");
}

int PID_realize_straight(float err)
{
  pid_straight.err = err;
  float incrementAngle = pid_straight.Kp * (pid_straight.err - pid_straight.err_last) + pid_straight.Kd * (pid_straight.err - 2 * pid_straight.err_last + pid_straight.err_lastlast);
  //+ pid_straight.Ki * pid_straight.err //不用i调节
  pid_straight.ActualAngle += incrementAngle;
  pid_straight.err_lastlast = pid_straight.err_last;
  pid_straight.err_last = pid_straight.err;
  return (int)pid_straight.ActualAngle;
}

void car_straight_pid()
{

  PID_init_straight();
  centerAngle = PID_realize_straight(atan(new_M_slope()));
}

/* 补control.c 39-45 */
void chuyuanhuan_bunengbuxian(void)
{
  float wall_slope = 0;
  int wall_dis = 0;
  int avr_x = 0;

  if (Road1_flag == 5) //出左环不能补线时
  {
    /* 黑线斜率约陡打角越小 */
    if (Rig[Allwhiteend] < 78) //正常情况
    {
      avr_x += Rig[Allwhiteend];
      for (int i = Allwhiteend - 1; i > Fir_row; --i)
      {
        if (Rig[i] >= 78 || Rig[i] - Rig[i + 1] > 0) //循环搜至找不到边线或不单调
        {
          avr_x += 2;                                                            //取最左
          avr_x /= Allwhiteend - i + 1;                                          //均值，取整，至少两点，Rig[Allwhiteend] … … 2
          wall_slope = Slope(avr_x, (i + Allwhiteend + 1) / 2, 78, Allwhiteend); //算斜率，应该不大于1
          break;
        }
        avr_x += Rig[i];
      }
    }
    else if (Lef[Allwhiteend] <= 2) //黑长直
    {
      wall_slope = 0.01;
    }
    else //歪的离谱，提前进5可能导致该状态
    {
      wall_slope = 0.01; //-999;//暂时与直线不区分
    }

    /* 黑线距离越近打角越大（以中间列为准） */
    for (int j = 0; j < 3 && Allwhiteend - j > Fir_row && Rig[Allwhiteend - j] < 78; ++j)
    {
      if (j == 2)
      {
        wall_dis == Rig[Allwhiteend - 1]; //38行打满晚了
      }
    }

    /* 咋整到一起呢 左环左转为负*/
    Turn_Cam_Out = -120 - (1 - wall_slope) * (wall_dis - 25) * 10;
  }

  if (Road2_flag == 5) //出右环不能补线时
  {
    /* 黑线斜率约陡打角越小 */
    if (Lef[Allwhiteend] < 78) //正常情况
    {
      avr_x += Lef[Allwhiteend];
      for (int i = Allwhiteend - 1; i > Fir_row; --i)
      {
        if (Lef[i] >= 78 || Lef[i] - Lef[i + 1] > 0) //循环搜至找不到边线或不单调
        {
          avr_x += 2;                                                            //取最左
          avr_x /= Allwhiteend - i + 1;                                          //均值，取整，至少两点，Rig[Allwhiteend] … … 2
          wall_slope = Slope(avr_x, (i + Allwhiteend + 1) / 2, 78, Allwhiteend); //算斜率，应该不大于1
          break;
        }
        avr_x += Lef[i];
      }
    }
    else if (Rig[Allwhiteend] <= 2) //黑长直
    {
      wall_slope = 0.01;
    }
    else //歪的离谱，提前进5可能导致该状态
    {
      wall_slope = 0.01; //-999;//暂时与直线不区分
    }

    /* 黑线距离越近打角越大（以中间列为准） */
    for (int j = 0; j < 3 && Allwhiteend - j > Fir_row && Lef[Allwhiteend - j] < 78; ++j)
    {
      if (j == 2)
      {
        wall_dis == Lef[Allwhiteend - 1]; //38行打满晚了
      }
    }

    /* 咋整到一起呢 右环右转为正*/
    Turn_Cam_Out = 120 + (1 - wall_slope) * (wall_dis - 25) * 10;
  }
}

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
#ifdef TL2barn
      Turn_Out = -SERVO_RANGE;
#endif
#ifdef TR2barn
      Turn_Out = SERVO_RANGE;
#endif
    }
    else if (Road7_flag == 3)
    {
      accuracy_EM = 0;
      accuracy_Cam = 1;
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
    accuracy_EM = 0;
    accuracy_Cam = 1;
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

float speed_mean_filter(float D_new)
{
  static float D[FILTER_ARRAY_SIZE];
  static int num = 0;
  static float sum = 0;

  if (num < FILTER_ARRAY_SIZE)
  {
    D[num] = D_new;
    ++num;
    sum += D_new;
    return sum / num;
  }
  else
  {
    sum += D_new;
    num = num % FILTER_ARRAY_SIZE + FILTER_ARRAY_SIZE;
    sum -= D[num - FILTER_ARRAY_SIZE];
    D[num - FILTER_ARRAY_SIZE] = D_new;
    return sum * 0.05;
  }
}

float Wheel_Differ_P = 6; //4;
float Wheel_Differ_I = 0; //1.5;
float Wheel_Differ_D = 2; //4;

float Wheel_Differ_Err_Table[9] = {-1.5, -1, -0.3, -0.05, 0, 0.05, 0.3, 1, 1.5};
float Wheel_Differ_P_Table0[9] = {8, 9, 25, 35, 6, 35, 25, 9, 8};
float Wheel_Differ_D_Table0[9] = {3, 3, 4, 6, 5, 6, 4, 3, 3};

float Wheel_Differ_P_Table1[9] = {7, 7, 15, 35, 6, 35, 15, 7, 7};
float Wheel_Differ_D_Table1[9] = {10, 10, 10, 10, 10, 10, 10, 10, 10}; //25 25 25 30 30 30 25 25 25

float Wheel_Differ_P_Table[9] = {7, 7, 15, 35, 6, 35, 15, 7, 7};
float Wheel_Differ_D_Table[9] = {10, 10, 10, 10, 10, 10, 10, 10, 10}; //25 25 25 30 30 30 25 25 25

Wheel_Differ_Err = SpeedE1 - SpeedE2;
if (Wheel_Differ_Err <= Wheel_Differ_Err_Table[0])
{
  Wheel_Differ_P = Wheel_Differ_P_Table1[0];
  Wheel_Differ_D = Wheel_Differ_D_Table1[0];
}
else if (Wheel_Differ_Err >= Wheel_Differ_Err_Table[8])
{
  Wheel_Differ_P = Wheel_Differ_P_Table1[8];
  Wheel_Differ_D = Wheel_Differ_D_Table1[8];
}
else
{
  for (int i = 0; i < 8; i++)
  {
    if (Wheel_Differ_Err >= Wheel_Differ_Err_Table[i] && Wheel_Differ_Err < Wheel_Differ_Err_Table[i + 1])
    {
      if (Wheel_Differ_Err == 0)
      {
        Wheel_Differ_Err = 0.01;
      }
      //Err = 0;Err = 0.01 ; P = (35*0.05-6*0)*(1-0/0.01)/(0.05-0)+0*6/0.01=35
      //Err=0.1;P=(15*0.3-35*0.05)*(1-0.05/0.1)/(0.3-0.05)+0.05*35/0.1=30
      //(P_high * E_high - P_low * E_low) / (E_high - E_low) * (1 - E_low / E_real) + P_low * E_low / E_real
      Wheel_Differ_P = (Wheel_Differ_P_Table[i + 1] * Wheel_Differ_Err_Table[i + 1] - Wheel_Differ_P_Table[i] * Wheel_Differ_Err_Table[i]) * (1 - Wheel_Differ_Err_Table[i] / Wheel_Differ_Err) / (Wheel_Differ_Err_Table[i + 1] - Wheel_Differ_Err_Table[i]) + Wheel_Differ_Err_Table[i] * Wheel_Differ_P_Table[i] / Wheel_Differ_Err;
      Wheel_Differ_D = Wheel_Differ_D_Table[i] + (Wheel_Differ_Err - Wheel_Differ_Err_Table[i]) * (Wheel_Differ_D_Table[i + 1] - Wheel_Differ_D_Table[i]) / (Wheel_Differ_Err_Table[i + 1] - Wheel_Differ_Err_Table[i]);
      break;
    }
  }
}

Wheel_Differ_PID_Out = Wheel_Differ_P * Wheel_Differ_Err + Wheel_Differ_D * (Wheel_Differ_Err - old_Wheel_Differ_Err);