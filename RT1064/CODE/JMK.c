#include "headfile.h"

//params init
int Road03_count = 0, Road04_count = 0;
global float centerAngle;

// 赛道识别
#if 0
char Road1_turnout = 1;
void Road_rec(void)
{

  // static int oldwhite=5000;
  // static uint8 Road1_cnt1=0;
  // static char Road1_flag1=0;
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////��ͨ������Բ��

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
    else if ((Lef_break_point < 20 && Rig_circle == 0 && Lef_circle == 1 && Rig_slope == 998 && Lef_slope != 998 && Rig[13] - Rig[11] < 5 && Rig[11] - Rig[9] < 5 && Rig[9] - Rig[7] < 5 && Rig[7] - Rig[5] < 5 && Rig[5] - Rig[3] < 5 && Rig[11] != 78 && New_Lef[50] == -MIDMAP && Rig_edge < 10)) //��Բ����������,�ұ��ߣ�ֱͨ����
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
    else if (Rig_break_point < 20 && Lef_circle == 0 && Rig_circle == 1 && Lef_slope == 998 && Rig_slope != 998 && Lef[3] - Lef[5] < 5 && Lef[5] - Lef[7] < 5 && Lef[7] - Lef[9] < 5 && Lef[9] - Lef[11] < 5 && Lef[11] - Lef[13] < 5 && Lef[11] != 2 && New_Rig[50] == MIDMAP && Lef_edge < 10) //��Բ�����ұ��ߣ�ͻ�������յ���ͻ����
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
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////��Բ������ͨ����

//if Road == 1 :left circle
  else if (Road == 1){
    if (Road1_flag == 0) //׼������Բ��
    {
      Road0_flag = 0;
      Road1_flag = 1;
      /*
      if(EM_Value_2+EM_Value_3>3.8)//����ʶ�����������߽���һ�߷�������
      {
        Road12_count++;
        if(Road12_count==2)
        {
          Road12_count=0;
          Road1_flag=1;//��ʾ�Ѿ�������Բ��
        }
        return;
      }
      */
      return;
    }
    else if (Road1_flag == 1) //����Բ��1/4
    {
      Road0_flag = 0;
      if (Lef_circle == 0 || (Lef_circle == 1 && Lef_break_point > 45)) //if(((Lef_circle==0||( Lef_circle ==1 && Lef_break_point>30)))&& Road1_turnin(EM_Value_2,EM_Value_3,3.8))//if(((Lef_circle==0||( Lef_circle ==1 && Lef_break_point>30))) && Rig_slope>=10)/ && Road1_turnin(EM_Value_2,EM_Value_3,3.8))    //Rig_slope<1 && (Lef_leap[0]==0||Lef_slope==999)&& Rig_leap[0]==0)
      {
        Road13_count++;
        if (Road13_count == 2) //2֡�� ����Բ����һ����
        {
          Road1_flag = 2;
          //        Road1_flag1 = 1;
          Road13_count = 0;
          return;
        }
        return;
      }
    }
    else if (Road1_flag == 2) //����Բ��2/4 ��ʼ���߽�����
    {
      Road0_flag = 0;
      Road14_count++;
      if (Road14_count == (int)(DIS_IN_CIRCLE * 10000 / (get_speed() * CAMERA_FPS)) + 1) //�궨����function.h
      {
        Road1_flag = 4;
        Road14_count = 0;
        return;
      }
      return;
    }
    else if (Road1_flag == 4) //����Բ���� ��ȡ������
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
    else if (Road1_flag == 3) //׼����Բ���ص�ֱ· �� ��ʼ����
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
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////��Բ������ͨ������ͬ����Բ����ע�ʹӼ�

//if Road == 2:right circle
  else if (Road == 2){
    if (Road2_flag == 0) //
    {
      Road0_flag = 0;
      Road2_flag = 1;
      /*
      if( EM_Value_2 +EM_Value_3 >3.8)//����ʶ�����������߽���һ�߷�������
      {
        Road22_count++;
        if(Road22_count==2)
        {
          Road22_count=0;
          Road2_flag=1;//��ʾ�Ѿ�������Բ��
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

 struct _pid{
     float err;                //定义偏差值
     float err_last;           //定义上一个偏差值
     float Kp,Ki,Kd;           //定义比例、积分、微分系数
     float voltage;            //定义电压值（控制执行器的变量）
     float integral;           //定义积分值
     float ActualAngle;        //输出转角
 };

 _pid pid_center;

 void PID_init_center(){
    printf("PID_init begin \n");
    pid_center.err=0.0;
    pid_center.err_last=0.0;
    pid_center.voltage=0.0;
    pid_center.integral=0.0;
    pid_center.Kp=0.2;
    pid_center.Ki=0.015;
    pid_center.Kd=0.2;
    printf("pid_center_init end \n");
}

float PID_realize_center(float err){
    pid_center.err=err;
    float incrementAngle=pid_center.Kp*(pid_center.err-pid_center.err_next)+pid_center.Ki*pid_center.err+pid_center.Kd*(pid_center.err-2*pid_center.err_next+pid_center.err_last);
    pid_center.ActualAngle+=incrementAngle;
    pid_center.err_last=pid_center.err_next;
    pid_center.err_next=pid_center.err;
    return pid_center.ActualAngle;
}

float car_center(){

  // PID_init_center();
  float car_center_dias=0;
  int car_center_start = 3;
  int car_center_end= 8;
  for(int i =car_center_start; i < car_center_end; ++i){
    car_center_dias += New_Mid[i];
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

void PID_init_straight(){
    printf("PID_init begin \n");
    pid_straight.err=0.0;
    pid_straight.err_last=0.0;
    pid_straight.voltage=0.0;
    pid_straight.integral=0.0;
    pid_straight.Kp=0.2;
    pid_straight.Ki=0.015;
    pid_straight.Kd=0.2;
    printf("pid_straight_init end \n");
}

int PID_realize_straight(float err){
    pid_straight.err=err;
    float incrementAngle=pid_straight.Kp*(pid_straight.err-pid_straight.err_next)+pid_straight.Ki*pid_straight.err+pid_straight.Kd*(pid_straight.err-2*pid_straight.err_next+pid_straight.err_last);
    pid_straight.ActualAngle+=incrementAngle;
    pid_straight.err_last=pid_straight.err_next;
    pid_straight.err_next=pid_straight.err;
    return (int) pid_straight.ActualAngle;
}

void car_straight_pid(){

  PID_init_straight();
  centerAngle=PID_realize_straight(theta);

}
