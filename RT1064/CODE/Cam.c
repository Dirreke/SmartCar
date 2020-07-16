#include "headfile.h"

float car_center(void)
{
  // PID_init_center();
  float car_center_dias_sum = 0;
  int car_center_start = 50;
  int car_center_end = 55;
  int cnt = 0;
  for (int i = car_center_start; i < car_center_end; ++i)
  {
    if (New_Mid[i] != 999)
    {
      car_center_dias_sum += New_Mid[i];
      cnt += 1;
    }
  }
  if (cnt == 0)
  {
    return 0;
  }
  return car_center_dias_sum / cnt;
  // centerAngle=PID_realize_center(car_center_dias);
}

/*************************************************************************
 *  函数名称：float Circle_angle_val1_calculate(void)
 *  功能说明：计算中线的曲率半径,并计算理想舵机打角
 *  参数说明：无
 *  函数返回：转向打角
 *  修改时间：2020.1.18
 *  备    注：其中12位参数等于去畸中压缩比例的数字,y2_origin为参数
             -angle_val1为正，车偏右，向左拐；
             -angle_val1为负，车偏左，向右拐；
            
 * ************************************************************************/

float car_straight(float car_dias)
{
  int i;
  float x2_temp;
  float sum1 = 0;
  float sum2 = 0;
  float temp_sin;
  float angle_val1;

  static const float y2_origin[60] = {13924, 13456, 12996, 12544, 12100, 11664, 11236, 10816, 10404, 10000, 9604, 9216, 8836, 8464, 8100, 7744, 7396, 7056, 6724, 6400, 6084, 5776, 5476, 5184, 4900, 4624, 4356, 4096, 3844, 3600, 3364, 3136, 2916, 2704, 2500, 2304, 2116, 1936, 1764, 1600, 1444, 1296, 1156, 1024, 900, 784, 676, 576, 484, 400, 324, 256, 196, 144, 100, 64, 36, 16, 4, 0};
  // 即y2_origin=(y*1.6)^2。
  for (i = 3; i < 58; i++)
  {
    if (New_Mid[i] != 999)
    {
      x2_temp = ((New_Mid[i] - car_dias) / UNDISTORT_XPK) * ((New_Mid[i] - car_dias) / UNDISTORT_XPK);
      sum1 += x2_temp;
      sum2 += (x2_temp + y2_origin[i]) * (New_Mid[i] - car_dias);
    }
  }
  if (sum2 == 0)
  {
    angle_val1 = 0;
  }
  else
  {
    temp_sin = sum1 / sum2 * 2 * CAR_LENGTH * UNDISTORT_PWK;
    temp_sin = (temp_sin > 1 ? 1 : (temp_sin < -1 ? -1 : temp_sin));
    // temp_sin = limit_f(temp_sin, -1, 1);
    angle_val1 = asin(temp_sin);
  }
  return -angle_val1;
}

PID PID_CAR_STRAIGHT_CAM;
PID PID_CAR_CENTER_CAM;

float Turn_Cam_Center_P_Table[11] = {0.2, 0.4, 0.45, 0.3, 0.2, 0.1, 0.2, 0.3, 0.45, 0.4, 0.2};//*0.5;
float Turn_Cam_Center_D_Table[11] = {0.2, 0.35, 0.5, 0.3, 0.2, 0.1, 0.2, 0.3, 0.5, 0.35, 0.2};//{0.2, 0.4, 0.45, 0.3, 0.2, 0.1, 0.2, 0.3, 0.45, 0.4, 0.2}*0.5;

float car_center_dias_Table[11] = {-180, -120, -75, -50, -30, 0, 30, 50, 75, 120, 180};

// float Turn_Cam_Straight_P_Table[11] = {0.5, 0.8, 0.7, 0.55, 0.4, 0.1, 0.4, 0.55, 0.7, 0.8, 0.5};
// float Turn_Cam_Straight_D_Table[11] = {0.15, 0.3, 0.6, 0.6, 0.4, 0.01, 0.4, 0.6, 0.6, 0.3, 0.15};
// float car_straight_dias_Table[11] = {-250, -180, -140, -100, -70, 0, 70, 100, 140, 180, 250};
float Turn_Cam_Straight_P_Table[11] = {0.58, 0.82, 0.84, 0.62, 0.45, 0.1, 0.45, 0.62, 0.84, 0.82, 0.58};
float Turn_Cam_Straight_D_Table[11] = {0.15, 0.25, 0.7, 0.6, 0.5, 0.01, 0.5, 0.6, 0.7, 0.25, 0.15};
float car_straight_dias_Table[11] = {-250, -180, -140, -100, -70, 0, 70, 100, 140, 180, 250};

float Turn_Cam_Center_P = 0;

float car_straight_dias;

float car_center_dias; //diff impose on angle set
void Turn_Cam_New(void)
{
  static float car_center_dias_old = 0;
  static float car_straight_dias_old = 0;
  float car_center_PWM;
  float car_straight_PWM;
  car_center_dias = car_center();
  //car_straight_angle = car_straight(car_center_dias);
  car_straight_dias = M_Slope_fig() * SERVO_RANGE / ANGLE_RANGE;
  /* 车正角度转换p表 */
  if (car_center_dias <= car_center_dias_Table[0])
  {
    PID_CAR_CENTER_CAM.P = Turn_Cam_Center_P_Table[0];
    PID_CAR_CENTER_CAM.D = Turn_Cam_Center_D_Table[0];
  }
  else if (car_center_dias >= car_center_dias_Table[10])
  {
    PID_CAR_CENTER_CAM.P = Turn_Cam_Center_P_Table[10];
    PID_CAR_CENTER_CAM.D = Turn_Cam_Center_D_Table[10];
  }
  else
  {
    for (int i = 0; i < 10; i++)
    {
      if (car_center_dias >= car_center_dias_Table[i] && car_center_dias < car_center_dias_Table[i + 1])
      {
        PID_CAR_CENTER_CAM.P = Turn_Cam_Center_P_Table[i] + (car_center_dias - car_center_dias_Table[i]) * (Turn_Cam_Center_P_Table[i + 1] - Turn_Cam_Center_P_Table[i]) / (car_center_dias_Table[i + 1] - car_center_dias_Table[i]); //线性
        PID_CAR_CENTER_CAM.D = Turn_Cam_Center_D_Table[i] + (car_center_dias - car_center_dias_Table[i]) * (Turn_Cam_Center_D_Table[i + 1] - Turn_Cam_Center_D_Table[i]) / (car_center_dias_Table[i + 1] - car_center_dias_Table[i]); //线性
      }
    }
  }

  car_center_PWM = PID_CAR_CENTER_CAM.P * car_center_dias+ PID_CAR_CENTER_CAM.D * (car_center_dias - car_center_dias_old);

  /* 车直模糊PD表 */

  if (car_straight_dias <= car_straight_dias_Table[0])
  {
    PID_CAR_STRAIGHT_CAM.P = Turn_Cam_Straight_P_Table[0];
    PID_CAR_STRAIGHT_CAM.D = Turn_Cam_Straight_D_Table[0];
  }
  else if (car_straight_dias >= car_straight_dias_Table[10])
  {
    PID_CAR_STRAIGHT_CAM.P = Turn_Cam_Straight_P_Table[10];
    PID_CAR_STRAIGHT_CAM.D = Turn_Cam_Straight_D_Table[10];
  }
  else
  {
    for (int i = 0; i < 10; i++)
    {
      if (car_straight_dias >= car_straight_dias_Table[i] && car_straight_dias < car_straight_dias_Table[i + 1])
      {
        PID_CAR_STRAIGHT_CAM.P = Turn_Cam_Straight_P_Table[i] + (car_straight_dias - car_straight_dias_Table[i]) * (Turn_Cam_Straight_P_Table[i + 1] - Turn_Cam_Straight_P_Table[i]) / (car_straight_dias_Table[i + 1] - car_straight_dias_Table[i]); //线性
        PID_CAR_STRAIGHT_CAM.D = Turn_Cam_Straight_D_Table[i] + (car_straight_dias - car_straight_dias_Table[i]) * (Turn_Cam_Straight_D_Table[i + 1] - Turn_Cam_Straight_D_Table[i]) / (car_straight_dias_Table[i + 1] - car_straight_dias_Table[i]);
        break;
      }
    }
  }


  car_straight_PWM = PID_CAR_STRAIGHT_CAM.P * car_straight_dias + PID_CAR_STRAIGHT_CAM.D * (car_straight_dias - car_straight_dias_old);

  car_straight_dias_old = car_straight_dias;
  car_center_dias_old = car_center_dias;
  //PID_realize_center(car_center_dias);
  //car_straight_PWM = PID_realize_straight(car_straight_angle * SERVO_RANGE / ANGLE_RANGE);

  Turn_Cam_Out = car_center_PWM + car_straight_PWM;
  //Servo_Duty(-Turn_Cam_Out);
}
float Mid_slope = 0;
float DEBUG_SLOPE = 0;
int DEBUG_MIDMAXMIN = 0;
float M_Slope_fig(void)
{
  int i;
  float xsum = 0, ysum = 0, xysum = 0, y2sum = 0;
  int max = -800, min = 0;
  int jump_change_point = 0;
  int jcp_old = FIG_AREA_NEAR;
  int count = 0, cnt_max = 0;
  int long_start = FIG_AREA_NEAR, long_end = 0;
  do //这个do后面是算跳变点的，要是不连续算个p斜率，取最长连续段算个斜率p
  {
    jump_change_point = gmyshuoqianbuchulai(jcp_old - 2);

    for (i = jump_change_point; i < jcp_old; i++)
    {
      if (i <= FIG_AREA_NEAR && i >= FIG_AREA_FAR && New_Mid[i] != 999)
      {
        count++;
      }
    }
    if (count > cnt_max)
    {
      cnt_max = count;
      long_start = jcp_old;
      long_end = jump_change_point;
    }
    count = 0;
    jcp_old = jump_change_point;
  } while (jump_change_point != FIG_AREA_FAR);
  {                                         //如果看到这段注释 下面是算斜率 可以折起来了
    for (i = long_end; i < long_start; i++) //从下往上搜的 start大，end小
    {
      if (i <= FIG_AREA_NEAR && i >= FIG_AREA_FAR && New_Mid[i] != 999)
      {
        if (New_Mid[i] > max)
        {
          max = New_Mid[i];
        }
        if (New_Mid[i] < min)
        {
          min = New_Mid[i];
        }
        xsum += New_Mid[i];
        ysum += i;
        xysum += New_Mid[i] * i;
        y2sum += i * i;
        count++;
      }
    }
    //cnt_max = count;
    DEBUG_MIDMAXMIN = abs(max - min);
    if (abs(max - min) > 20)
    {
      if (count * xysum - xsum * ysum)
      {
        Mid_slope = -(count * y2sum - ysum * ysum) / (count * xysum - xsum * ysum); // -(count * xysum - xsum * ysum) / (count * x2sum - xsum * xsum);
      }
      else
      {
        Mid_slope = 999;
      }
    }
    else
    {
      Mid_slope = 998;
    }
    DEBUG_SLOPE = Mid_slope;
  }
  { //如果看到这段注释 下面没有神奇操作了 斜率算的不对 return弧度！！好了折起来吧/* 是对斜率进行了一些神奇操作 */
    // if (Mid_slope != 999)
    // {
    //   if (Mid_slope >= 1.4 || Mid_slope <= -1.4)
    //   {
    //     Mid_slope = 998;
    //   }
    //   else if (Mid_slope >= 1.2 || Mid_slope <= -1.2)
    //   {
    //     Mid_slope = pow(Mid_slope, 3);
    //   }
    // }
    if (Mid_slope == 999 || Mid_slope == 998)
    {
      return 0;
    }
    else
    {
      return atan(Mid_slope * UNDISTORT_PYK * UNDISTORT_XPK) > 0 ? 1.57 - atan(Mid_slope * UNDISTORT_PYK * UNDISTORT_XPK) : -1.57 + atan(-Mid_slope * UNDISTORT_PYK * UNDISTORT_XPK);
    }
  }
}

int gmyshuoqianbuchulai(int temp)
{
  int dis = 0, turn = 0;
  if (temp <= FIG_AREA_FAR)
  {
    return FIG_AREA_FAR;
  }
  turn = temp;
  for (int i = turn; i > FIG_AREA_FAR; --i) //从仿制版改成了山寨版 折起来吧
  {
    dis = abs(New_Mid[i + 1] - New_Mid[i]);
    if (dis <= 15)
    {
      turn = i;
    }
    else
    {
      break;
    }
  }
  return turn;
}