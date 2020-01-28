#include "headfile.h"

//params init
int Road03_count = 0, Road04_count = 0;


// 赛道识别
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
      if (Lef_circle == 0 || (Lef_circle == 1 && Lef_break_point > 30)) //if(((Lef_circle==0||( Lef_circle ==1 && Lef_break_point>30)))&& Road1_turnin(EM_Value_2,EM_Value_3,3.8))//if(((Lef_circle==0||( Lef_circle ==1 && Lef_break_point>30))) && Rig_slope>=10)/ && Road1_turnin(EM_Value_2,EM_Value_3,3.8))    //Rig_slope<1 && (Lef_leap[0]==0||Lef_slope==999)&& Rig_leap[0]==0)
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
