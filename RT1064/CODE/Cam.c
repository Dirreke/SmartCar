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
PID PID_CAR_Diffcomp_CAM;

float Turn_Cam_Center_P = 0;

float car_straight_dias;

float car_center_dias; //diff impose on angle set

float car_center_PWM;

void Turn_Cam_New(void)
{
  static float car_center_dias_old = 0;
  static float car_straight_dias_old = 0;
  float car_center_diff = 0;
  float car_straight_PWM;
  
  car_center_dias = car_center();
  //car_straight_angle = car_straight(car_center_dias);
  car_straight_dias = M_Slope_fig() * SERVO_RANGE / ANGLE_RANGE;

  Center_offset_filter();
  Straight_offset_filter();
  /* 车正角度转换p表 */

  Turn_Cam_Out = car_center_PWM + car_straight_PWM; //+ car_diffcomp_PWM;
  //Servo_Duty(-Turn_Cam_Out);
}
float Mid_slope = 0;
float Mid_intercept = 0;
float Mid_status = 0;
// float DEBUG_SLOPE = 0;
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
    // DEBUG_SLOPE = Mid_slope;
    Mid_intercept = (xsum + ysum / Mid_slope) / count; // x shi y;y shi x;-MId_slope^-1 shi k;Mid_intercept shi jieju;
    Mid_status = -30 / Mid_slope + Mid_intercept;
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
    return Turn_Out - atan(2 * (CarSpeed1 - CarSpeed2) / ((CarSpeed1 + CarSpeed2) * CAR_DIFF_K)) / ANGLE_RANGE * SERVO_RANGE;
  else
    return 0;
}
float Turn_Cam_Diffcomp_P_Table[11] = {0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4};
float Turn_Cam_Diffcomp_D_Table[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float car_Diffcomp_Table[11] = {-250, -180, -140, -100, -70, 0, 70, 100, 140, 180, 250};

void Turn_diff_comp()
{
  float car_diffcomp_PWM;
  static float car_diffcomp_dias_old = 0;
  car_diffcomp_dias = Car_diff_comp();
  /* 差速补偿PD表 */

  if (car_diffcomp_dias <= car_Diffcomp_Table[0])
  {
    PID_CAR_Diffcomp_CAM.P = Turn_Cam_Diffcomp_P_Table[0];
    PID_CAR_Diffcomp_CAM.D = Turn_Cam_Diffcomp_D_Table[0];
  }
  else if (car_diffcomp_dias >= car_Diffcomp_Table[10])
  {
    PID_CAR_Diffcomp_CAM.P = Turn_Cam_Diffcomp_P_Table[10];
    PID_CAR_Diffcomp_CAM.D = Turn_Cam_Diffcomp_D_Table[10];
  }
  else
  {
    for (int i = 0; i < 10; i++)
    {
      if (car_diffcomp_dias >= car_Diffcomp_Table[i] && car_diffcomp_dias < car_Diffcomp_Table[i + 1])
      {
        PID_CAR_Diffcomp_CAM.P = Turn_Cam_Diffcomp_P_Table[i] + (car_diffcomp_dias - car_Diffcomp_Table[i]) * (Turn_Cam_Diffcomp_P_Table[i + 1] - Turn_Cam_Diffcomp_P_Table[i]) / (car_Diffcomp_Table[i + 1] - car_Diffcomp_Table[i]); //线性
        PID_CAR_STRAIGHT_CAM.D = Turn_Cam_Diffcomp_D_Table[i] + (car_diffcomp_dias - car_Diffcomp_Table[i]) * (Turn_Cam_Diffcomp_D_Table[i + 1] - Turn_Cam_Diffcomp_D_Table[i]) / (car_Diffcomp_Table[i + 1] - car_Diffcomp_Table[i]);
        break;
      }
    }
  }

  car_diffcomp_PWM = PID_CAR_Diffcomp_CAM.P * car_diffcomp_dias + PID_CAR_Diffcomp_CAM.D * (car_diffcomp_dias - car_diffcomp_dias_old);

  car_diffcomp_dias_old = car_diffcomp_dias;
  Turn_Out = Turn_Out + car_diffcomp_PWM;
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