#include "headfile.h"

float car_center(void)
{
  // PID_init_center();
  float car_center_dias = 0;
  int car_center_start = 45;
  int car_center_end = 55;
  int cnt = 0;
  for (int i = car_center_start; i < car_center_end; ++i)
  {
    if (New_Mid[i] != 999)
    {
      car_center_dias += New_Mid[i];
      cnt++;
    }
  }
  if (cnt == 0)
  {
    return 0;
  }
  return car_center_dias / cnt;
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

float Turn_Cam_Center_P_Table[11] = {2, 1.5, 1.3, 1, 1, 0.5, 1, 1, 1.3, 1.5, 2};
float car_center_dias_Table[11] = {-150, -130, -100, -70, -30, 0, 30, 70, 100, 130, 150};
float Turn_Cam_Center_P = 0;

float car_straight_angle;

float car_center_dias; //diff impose on angle set
void Turn_Cam_New(void)
{
  static float car_center_dias_old = 0;
  static float car_straight_angle_old = 0;
  float car_center_PWM;
  float car_straight_PWM;
  car_center_dias = car_center();
  //car_straight_angle = car_straight(car_center_dias);
  car_straight_angle = M_Slope_fig();
  /* 车正角度转换p表 */
  if (car_center_dias <= car_center_dias_Table[0])
  {
    Turn_Cam_Center_P = Turn_Cam_Center_P_Table[0];
  }
  else if (car_center_dias >= car_center_dias_Table[10])
  {
    Turn_Cam_Center_P = Turn_Cam_Center_P_Table[10];
  }
  else
  {
    for (int i = 0; i < 11; i++)
    {
      if (car_center_dias >= car_center_dias_Table[i] && car_center_dias < car_center_dias_Table[i + 1])
      {
        Turn_Cam_Center_P = Turn_Cam_Center_P_Table[i] + (car_center_dias - car_center_dias_Table[i]) * (Turn_Cam_Center_P_Table[i + 1] - Turn_Cam_Center_P_Table[i]) / (car_center_dias_Table[i + 1] - car_center_dias_Table[i]); //线性
        break;
      }
    }
  }
  car_center_dias *= Turn_Cam_Center_P;

  car_center_PWM = PID_CAR_CENTER_CAM.P * car_center_dias + PID_CAR_CENTER_CAM.D * (car_center_dias - car_center_dias_old);
  car_straight_PWM = (PID_CAR_STRAIGHT_CAM.P * car_straight_angle + PID_CAR_STRAIGHT_CAM.D * (car_straight_angle - car_straight_angle_old)) * SERVO_RANGE / ANGLE_RANGE;

  car_straight_angle_old = car_straight_angle;
  car_center_dias_old = car_center_dias;
  //PID_realize_center(car_center_dias);
  //car_straight_PWM = PID_realize_straight(car_straight_angle * SERVO_RANGE / ANGLE_RANGE);

  Turn_Cam_Out = car_center_PWM + car_straight_PWM;
  //Servo_Duty(-Turn_Cam_Out);
}
float Mid_slope = 0;
float M_Slope_fig(void)
{
  int i;
  float xsum = 0, ysum = 0, xysum = 0, x2sum = 0, count = 0;
  int max = -800, min = 0;
  for (i = 5; i < 45; i++)
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
      x2sum += New_Mid[i] * New_Mid[i];
      count++;
    }
  }
  if (abs(max - min) > 25)
  {
    if (count * x2sum - xsum * xsum)
    {
      Mid_slope = -(count * xysum - xsum * ysum) / (count * x2sum - xsum * xsum);
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
  if (Mid_slope != 999)
  {
    if (Mid_slope >= 1.4 || Mid_slope <= -1.4)
    {
      Mid_slope = 998;
    }
    else if (Mid_slope >= 1.2 || Mid_slope <= -1.2)
    {
      Mid_slope = pow(Mid_slope, 3);
    }
  }
  if (Mid_slope == 999 || Mid_slope == 998)
  {
    return 0;
  }

  else
  {
    return atan(Mid_slope * UNDISTORT_PYK * UNDISTORT_XPK) > 0 ? 1.57 - atan(Mid_slope * UNDISTORT_PYK * UNDISTORT_XPK) : -1.57 + atan(-Mid_slope * UNDISTORT_PYK * UNDISTORT_XPK);
  }
}
