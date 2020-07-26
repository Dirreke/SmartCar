
#include "headfile.h"
float car_straight_dias;
float car_center_dias;
PID PID_CAR_STRAIGHT_CAM;
PID PID_CAR_CENTER_CAM;
void Turn_Cam_New(void)
{
  PID_CAR_STRAIGHT_CAM.P = (((CarSpeed1 + CarSpeed2) * 0.5 - 2.3) > 0 ? ((CarSpeed1 + CarSpeed2) * 0.5 - 2.3) * 0.75 + 0.6 : 0.6); //2.8 0.95 2.5 0.6
  //PID_CAR_STRAIGHT_CAM.D = 0;
  static float car_straight_dias_old = 0;
  car_straight_dias = M_Slope_fig() * SERVO_DIVIDE_ANGLE_SCALE;
  Straight_offset_filter();
  car_center_dias = car_center();
  Center_offset_filter();

  if (fabs(car_center_dias) < 10)
  {
    car_center_dias = 0;
  }

  if (fabs(Mid_slope) < 1)
  {
    Turn_Cam_Out = car_straight_dias * PID_CAR_STRAIGHT_CAM.P + (car_straight_dias - car_straight_dias_old) * PID_CAR_STRAIGHT_CAM.D;
  }
  else if (fabs(Mid_slope) < 3)
  {
    Turn_Cam_Out = car_straight_dias * PID_CAR_STRAIGHT_CAM.P + car_center_dias * PID_CAR_CENTER_CAM.P;
  }
  else
  {
    Turn_Cam_Out = car_center_dias * PID_CAR_CENTER_CAM.P;
  }

  car_straight_dias_old = car_straight_dias;
}
float Mid_slope = 0;

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

  if (cnt_max < 18) //点太少直接返回上一帧
  {
    return atan(Mid_slope * UNDISTORT_PYK * UNDISTORT_XPK) > 0 ? 1.57 - atan(Mid_slope * UNDISTORT_PYK * UNDISTORT_XPK) : -1.57 + atan(-Mid_slope * UNDISTORT_PYK * UNDISTORT_XPK); //延续上一帧
  }

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
  }
  { //如果看到这段注释 下面没有神奇操作了 斜率算的不对 return弧度！！好了折起来吧/* 是对斜率进行了一些神奇操作 */

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

/*************************************************************************
*  函数名称：void Car_diff_comp(void)
*  功能说明：chasu buchang
*  参数说明：无
*  函数返回：无
*  修改时间：2020.7.16
*  备    注：

*************************************************************************/
float car_diffcomp_dias;
float Car_diff_comp(void)
{

  if (CarSpeed1 + CarSpeed2 > 3)
    return Turn_Out - atan(2 * (CarSpeed1 - CarSpeed2) / ((CarSpeed1 + CarSpeed2) * CAR_DIFF_K)) * SERVO_DIVIDE_ANGLE_SCALE;
  else
    return 0;
}
PID PID_CAR_Diffcomp_CAM;
void Turn_diff_comp()
{
  float car_diffcomp_PWM;
  static float car_diffcomp_dias_old = 0;
  // PID_CAR_Diffcomp_CAM.P = 0.2;
  // PID_CAR_Diffcomp_CAM.D = 0;
  car_diffcomp_dias = Car_diff_comp();

  car_diffcomp_PWM = PID_CAR_Diffcomp_CAM.P * car_diffcomp_dias + PID_CAR_Diffcomp_CAM.D * (car_diffcomp_dias - car_diffcomp_dias_old);

  car_diffcomp_dias_old = car_diffcomp_dias;
  Turn_Out = Turn_Out + car_diffcomp_PWM;
}
/*************************************************************************
*  函数名称：void Car_center(void)
*  功能说明：chezhi滤波
*  参数说明：无
*  函数返回：无
*  修改时间：2020.7.16
*  备    注：

*************************************************************************/
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
*  函数名称：void Straight_offset_filter(void)
*  功能说明：chezhi滤波
*  参数说明：无
*  函数返回：无
*  修改时间：2020.7.16
*  备    注：

*************************************************************************/

void Straight_offset_filter(void)
{
  static float Straight_offset_filter[4] = {0, 0, 0, 0}; //offset滤波数组
  Straight_offset_filter[3] = Straight_offset_filter[2];
  Straight_offset_filter[2] = Straight_offset_filter[1];
  Straight_offset_filter[1] = Straight_offset_filter[0];
  Straight_offset_filter[0] = car_straight_dias;
  car_straight_dias = Straight_offset_filter[0] * 0.4 + Straight_offset_filter[1] * 0.3 + Straight_offset_filter[2] * 0.2 + Straight_offset_filter[3] * 0.1;
}

/*************************************************************************
*  函数名称：void Center_offset_filter(void)
*  功能说明：chezheng滤波
*  参数说明：无
*  函数返回：无
*  修改时间：2020.7.16
*  备    注：

*************************************************************************/

void Center_offset_filter(void)
{
  static float Center_offset_filter[4] = {0, 0, 0, 0}; //offset滤波数组
  Center_offset_filter[3] = Center_offset_filter[2];
  Center_offset_filter[2] = Center_offset_filter[1];
  Center_offset_filter[1] = Center_offset_filter[0];
  Center_offset_filter[0] = car_center_dias;
  car_center_dias = Center_offset_filter[0] * 0.5 + Center_offset_filter[1] * 0.2 + Center_offset_filter[2] * 0.2 + Center_offset_filter[3] * 0.1;
}