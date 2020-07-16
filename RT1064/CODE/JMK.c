#include "headfile.h"
#include <queue>
//params init
int Road03_count = 0, Road04_count = 0;
float centerAngle;

// ����ʶ��
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
    if((Rig_break_point > 30 && Lef_circle == 0 && Rig_circle == 1 && Rig_slope != 998 )) //��ת��
    {
      Road03_count++;
      if (Road03_count == 2){
        Road0_flag = 3;//turn left flag
        Road03_count = 0;//reset
      }
    }
    else if ((Lef_break_point > 30 && Rig_circle == 0 && Lef_circle == 1 && Lef_slope != 998 )) //��ת��
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
    else if (Road1_flag == 3) //????????????�� ?? ???????
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
 *  �������ƣ�void car_center_pid()
 *  ����˵�������� ��������λ�ò�pid
 *  ����˵������ (pixel)
 *  �������أ���
 *  �޸�ʱ�䣺2019.12.28
 *  ��    ע���복ֱ����
 * **********************************************************************/

struct _pid
{
  float err;          //����ƫ��ֵ
  float err_lastlast; //�������ϸ�ƫ��ֵ
  float err_last;     //�����ϸ�ƫ��ֵ
  //float Kp, Ki, Kd;   //�������������֡�΢��ϵ��
  float Kp, Kd;      //����������΢��ϵ��
  float voltage;     //������ѹֵ������ִ�����ı�����
  float integral;    //��������ֵ
  float ActualAngle; //����ת��
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
  //+ pid_center.Ki * pid_center.err //����i����
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
 *  �������ƣ�void car_straight_pid()
 *  ����˵������ֱ �����߽ǶȲ�pid
 *  ����˵������ (theta)
 *  �������أ���
 *  �޸�ʱ�䣺2019.12.28
 *  ��    ע����gmydl�ĳ�ֱ��(��)pid(call)
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
  //+ pid_straight.Ki * pid_straight.err //����i����
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

/* ��control.c 39-45 */
void chuyuanhuan_bunengbuxian(void)
{
  float wall_slope = 0;
  int wall_dis = 0;
  int avr_x = 0;

  if (Road1_flag == 5) //���󻷲��ܲ���ʱ
  {
    /* ����б��Լ������ԽС */
    if (Rig[Allwhiteend] < 78) //��������
    {
      avr_x += Rig[Allwhiteend];
      for (int i = Allwhiteend - 1; i > Fir_row; --i)
      {
        if (Rig[i] >= 78 || Rig[i] - Rig[i + 1] > 0) //ѭ�������Ҳ������߻򲻵���
        {
          avr_x += 2;                                                            //ȡ����
          avr_x /= Allwhiteend - i + 1;                                          //��ֵ��ȡ�����������㣬Rig[Allwhiteend] �� �� 2
          wall_slope = Slope(avr_x, (i + Allwhiteend + 1) / 2, 78, Allwhiteend); //��б�ʣ�Ӧ�ò�����1
          break;
        }
        avr_x += Rig[i];
      }
    }
    else if (Lef[Allwhiteend] <= 2) //�ڳ�ֱ
    {
      wall_slope = 0.01;
    }
    else //�������ף���ǰ��5���ܵ��¸�״̬
    {
      wall_slope = 0.01; //-999;//��ʱ��ֱ�߲�����
    }

    /* ���߾���Խ������Խ�������м���Ϊ׼�� */
    for (int j = 0; j < 3 && Allwhiteend - j > Fir_row && Rig[Allwhiteend - j] < 78; ++j)
    {
      if (j == 2)
      {
        wall_dis == Rig[Allwhiteend - 1]; //38�д�������
      }
    }

    /* զ����һ���� ������תΪ��*/
    Turn_Cam_Out = -120 - (1 - wall_slope) * (wall_dis - 25) * 10;
  }

  if (Road2_flag == 5) //���һ����ܲ���ʱ
  {
    /* ����б��Լ������ԽС */
    if (Lef[Allwhiteend] < 78) //��������
    {
      avr_x += Lef[Allwhiteend];
      for (int i = Allwhiteend - 1; i > Fir_row; --i)
      {
        if (Lef[i] >= 78 || Lef[i] - Lef[i + 1] > 0) //ѭ�������Ҳ������߻򲻵���
        {
          avr_x += 2;                                                            //ȡ����
          avr_x /= Allwhiteend - i + 1;                                          //��ֵ��ȡ�����������㣬Rig[Allwhiteend] �� �� 2
          wall_slope = Slope(avr_x, (i + Allwhiteend + 1) / 2, 78, Allwhiteend); //��б�ʣ�Ӧ�ò�����1
          break;
        }
        avr_x += Lef[i];
      }
    }
    else if (Rig[Allwhiteend] <= 2) //�ڳ�ֱ
    {
      wall_slope = 0.01;
    }
    else //�������ף���ǰ��5���ܵ��¸�״̬
    {
      wall_slope = 0.01; //-999;//��ʱ��ֱ�߲�����
    }

    /* ���߾���Խ������Խ�������м���Ϊ׼�� */
    for (int j = 0; j < 3 && Allwhiteend - j > Fir_row && Lef[Allwhiteend - j] < 78; ++j)
    {
      if (j == 2)
      {
        wall_dis == Lef[Allwhiteend - 1]; //38�д�������
      }
    }

    /* զ����һ���� �һ���תΪ��*/
    Turn_Cam_Out = 120 + (1 - wall_slope) * (wall_dis - 25) * 10;
  }
}

void Kalman_Filter(void)
{
  /* ��������uk���������Ǳ仯d�� */
  /* ���ƾ���B���Ƕ��ٶ����Ͼ��� */
  /* ״̬����xk����ǰ״̬��frenet����ϵ��n�����������߷������룬�̣��������������߼нǣ� */
  /* �ٶȷ����£�tan�� = 2 * tan�� */
  /* ״̬���̣� nk = nk+1 + vxsin�� + vycos�� */
  /* ״̬���̣� ��k = ��k-1 + r - ��(s)ds*/
  /* ״̬ת�ƾ���Fk��  */

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
      Servo_Duty(-Turn_Out); //��������
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
      Servo_Duty(-Turn_Out); //��������
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
  Servo_Duty(-Turn_Out); //��������
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
    num = num % FILTER_A
                    num = num % FILTER_ARRAY_SIZE + FILTER_ARRAY_SIZE;
    sum -= D[num - FILTER_ARRAY_SIZE];
    D[num - FILTER_ARRAY_SIZE] = D_new;
    return sum * 0.05;
  }
}

float Mid_slope = 0;
float M_Slope_fig(void)
{

  float xsum = 0, ysum = 0, xysum = 0, x2sum = 0, count = 0;
  int max = -800, min = 0;
  const int M_slope_start = 5, M_slope_end = 45;
  for (int i = M_slope_start; i < M_slope_end; i++)
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

float Related_calculate(float xsum, float ysum, const int M_slope_start, const int M_slope_end)
{
  float xmean = 0, ymean = 0, xmean2sum = 0, ymean2sum = 0;
  float xysigma = 0, rsum = 0;
  int count = M_slope_end - M_slope_start;
  xmean = xsum / count;
  ymean = ysum / count;
  for (int i = M_slope_start; i < M_slope_end; i++)
  {
    xmean2sum += (New_Mid[i] - xmean) * (New_Mid[i] - xmean);
    ymean2sum += (i - ymean) * (i - ymean);
  }
  xmean2sum /= count;
  ymean2sum /= count;
  xysigma = 1 / sqrt(xmean2sum * ymean2sum);
  for (int i = M_slope_start; i < M_slope_end; i++)
  {
    rsum += (New_Mid[i] - xmean) * (i - ymean) * xysigma;
  }
  return rsum / count;
}

float Turn_Cam_Center_P_Table[11] = {0.2, 0.3, 0.4, 0.3, 0.15, 0.05, 0.15, 0.3, 0.4, 0.3, 0.2}; // {0.15, 0.2, 0.3, 0.2, 0.15, 0.05, 0.15, 0.2, 0.3, 0.2, 0.15}; //*0.5;
float Turn_Cam_Center_D_Table[11] = {0.8, 1, 1.4, 1.2, 0.6, 0, 0.6, 1.2, 1.4, 1, 0.8};          //{0.2, 0.4, 0.45, 0.3, 0.2, 0.1, 0.2, 0.3, 0.45, 0.4, 0.2}*0.5;

float car_center_dias_Table[11] = {-45, -25, -15, -10, -5, 0, 5, 10, 15, 25, 45}; //{-180, -120, -75, -50, -15, 0, 15, 50, 75, 120, 180};

// float Turn_Cam_Straight_P_Table[11] = {0.5, 0.8, 0.7, 0.55, 0.4, 0.1, 0.4, 0.55, 0.7, 0.8, 0.5};
// float Turn_Cam_Straight_D_Table[11] = {0.15, 0.3, 0.6, 0.6, 0.4, 0.01, 0.4, 0.6, 0.6, 0.3, 0.15};
// float car_straight_dias_Table[11] = {-250, -180, -140, -100, -70, 0, 70, 100, 140, 180, 250};
float Turn_Cam_Straight_P_Table[11] = {0.58, 0.82, 0.84, 0.57, 0.4, 0.1, 0.4, 0.57, 0.84, 0.82, 0.58};
float Turn_Cam_Straight_D_Table[11] = {0.4, 0.6, 0.7, 0.5, 0.4, 0.01, 0.4, 0.5, 0.7, 0.6, 0.4};
float car_straight_dias_Table[11] = {-250, -180, -140, -100, -70, 0, 70, 100, 140, 180, 250};

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
  car_center_diff = car_center_dias - car_center_dias_old;
  /* 车正角度转换p表 */
  if (car_center_diff <= car_center_dias_Table[0])
  {
    PID_CAR_CENTER_CAM.P = Turn_Cam_Center_P_Table[0];
    PID_CAR_CENTER_CAM.D = Turn_Cam_Center_D_Table[0];
  }
  else if (car_center_diff >= car_center_dias_Table[10])
  {
    PID_CAR_CENTER_CAM.P = Turn_Cam_Center_P_Table[10];
    PID_CAR_CENTER_CAM.D = Turn_Cam_Center_D_Table[10];
  }
  else
  {
    for (int i = 0; i < 10; i++)
    {
      if (car_center_diff >= car_center_dias_Table[i] && car_center_diff < car_center_dias_Table[i + 1])
      {
        PID_CAR_CENTER_CAM.P = Turn_Cam_Center_P_Table[i] + (car_center_diff - car_center_dias_Table[i]) * (Turn_Cam_Center_P_Table[i + 1] - Turn_Cam_Center_P_Table[i]) / (car_center_dias_Table[i + 1] - car_center_dias_Table[i]); //线性
        PID_CAR_CENTER_CAM.D = Turn_Cam_Center_D_Table[i] + (car_center_diff - car_center_dias_Table[i]) * (Turn_Cam_Center_D_Table[i + 1] - Turn_Cam_Center_D_Table[i]) / (car_center_dias_Table[i + 1] - car_center_dias_Table[i]); //线性
      }
    }
  }
