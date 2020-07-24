#include "headfile.h"

#define ori_pic

uint16 Pic_new[4000];
int cnt;
int cntt = 1;
uint8 ch = 0;
float temp1 = 0;
float Variable[20];

void Send_Data(void)
{
  // if (Road0_flag == 4 || Road0_flag == 5)
  // {
  //   lib_speed_set(0);
  //   Road = 7;
  //   Road7_flag = 4;
  // }
  Pic_send_new2();
  Variable_update();
  Send_Begin();
  Send_Variable();
  Send_Img2();
}

/*************************************************************************
*  函数名称：void Send_Begin(void)
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2019.3.6
*  备    注：

*************************************************************************/
void Send_Begin(void)
{
  My_Put_Char(0x55);
  My_Put_Char(0xaa);
  My_Put_Char(0x11);
}

/*************************************************************************
*  函数名称：void Send_Img(void)
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2019.3.6
*  备    注：

*************************************************************************/

void Send_Img(void)
{

  uint16 i = 0, num;
  My_Put_Char(0x55);
  My_Put_Char(0xaa);
  My_Put_Char(0xff);
  My_Put_Char(0xa2);
  My_Put_Char(0x01); //小锟斤拷状态

  num = cnt + 2 + 180; //4+4*2
  //num = cnt+2+180;
  //num=cont+2+180+36;
  //统计将要传输的数据量 2是因为要传输关键字即0xf0和0xf2   (0xf2 所有数据结束，0xf0图像数据结束，180=60*3)
  //180是边线的位 36是变量的位 如果不传输就不要加上！

  My_Put_Char(BYTE0(num));
  My_Put_Char(BYTE1(num));
  for (i = 0; i < cnt; i++)
  {
    My_Put_Char(Pic_new[i]);
  }
  //    for(i=0;i< 60;i++)
  //    {
  //            My_Put_Char(Lef[i]);
  //            My_Put_Char(Rig[i]);
  //            My_Put_Char(0xff);
  //    }
  My_Put_Char(0xf0); //代表图像数据发完了
  /******************星号围起来的可以不传输*******************/

  ////////////////////////传输左右边线和计算得到的中线///////////
  for (i = 0; i < 60; i++)
  {
    My_Put_Char(Lef[i]); //New_Lef[i]/10+40);
  }
  for (i = 0; i < 60; i++)
  {
    My_Put_Char(Mid[i]);
  }
  for (i = 0; i < 60; i++)
  {
    My_Put_Char(Rig[i] + 1); //New_Rig[i]/10+40);
  }

  /*****************************************************/
  My_Put_Char(0xf2); //代表整个数据都发完了
}

void Pic_send_new(void)
{
  int i;
  int j;
  cnt = 0;
#ifdef ori_pic
  for (i = 0; i < 60; i++)
  {
    if (Pixle[i][0] == 1)
    {
      Pic_new[cnt] = 0;
      cnt++;
    }
    for (j = 1; j < 80; j++)
    {
      if (Pixle[i][j] != Pixle[i][j - 1])
      {
        Pic_new[cnt] = j;
        cnt++;
      }
    }
    Pic_new[cnt] = 0xff;
    cnt++;
  }
#endif

#ifdef und_pic
  for (i = 0; i < 60; i++)
  {
    if ((int)(New_Lef[i] + 450) * 0.089 <= 0)
    {
      Pic_new[cnt] = 0;
      cnt++;
      Pic_new[cnt] = 1;
      cnt++;
    }
    else
    {
      Pic_new[cnt] = (int)(New_Lef[i] + 450) * 0.089;
      cnt++;
      Pic_new[cnt] = (int)(New_Lef[i] + 450) * 0.089 + 1;
      cnt++;
    }

    Pic_new[cnt] = (int)(New_Rig[i] + 450) * 0.089;
    cnt++;
    Pic_new[cnt] = (int)(New_Rig[i] + 450) * 0.089 + 1;
    cnt++;
    Pic_new[cnt] = 0xff;
    cnt++;
  }
#endif
}
/*************************************************************************
*  函数名称：void My_Put_Char(void)
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2019.3.6
*  备    注：

*************************************************************************/

void My_Put_Char(char tmp)
{
  uart_putchar(USART_1, tmp); //根据实际的串口号来修改
}

void Send_Variable(void)
{

  My_Put_Char(0x55);
  My_Put_Char(0xaa);
  My_Put_Char(0xff);
  My_Put_Char(0x01);
  My_Put_Char(16);
  for (int i = 0; i < 16; i++)
  {
    temp1 = Variable[i];
    ch = BYTE0(temp1);
    My_Put_Char(ch);
    ch = BYTE1(temp1);
    My_Put_Char(ch);
    ch = BYTE2(temp1);
    My_Put_Char(ch);
    ch = BYTE3(temp1);
    My_Put_Char(ch);
  }
  My_Put_Char(0x01);
}

/*************************************************************************
*  函数名称：void Variable_update(void)
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2019.3.10
*  备    注：

*************************************************************************/

void Variable_update(void)
{
  // Variable[0] = Cam_offset;   //EM_Value_1;// Cam_offset;//cntt;//ToF_distance;
  // Variable[1] = Turn_Cam_Out;           //EM_Value_2; //Turn_Cam_Out;//cnt;//Turn_Cam_Out;//;//stat_slope;//gyroy_1;//Turn_Cam_Out;//Car_W;
  // Variable[2] = Road;                   //EM_Value_1;//Cam_offset;
  // if (Road == 0)
  // {
  //   Variable[3] = Road0_flag;
  // }
  // else if (Road == 1)
  // {
  //   Variable[3] = Road1_flag;
  // }
  // else if (Road == 2)
  // {
  //   Variable[3] = Road2_flag;
  // }
  // else if (Road == 3)
  // {
  //   Variable[3] = Road3_flag;
  // }
  // else if (Road == 4)
  // {
  //   Variable[3] = Road4_flag;
  // }
  // else if (Road == 7)
  // {
  //   Variable[3] = Road7_flag;
  // }
  // Variable[4] = icm_gyro_y_w; //speedTarget1; //Allwhiteend;
  // Variable[5] = icm_gyro_y_angle; //
  // Variable[6] = SpeedGoal;
  // Variable[7] = CarSpeed1; //EM_Value_4;

  // Variable[8] = CarSpeed2; //EM_offset;
  // Variable[9] = Lef_slope; //speedTarget2;//Turn_Cam_Out;
  // Variable[10] = Rig_slope;
  // Variable[11] = EM_straight_offset; //Turn_EM_Out;  //Lef_break_point*100+Rig_break_point;
  // Variable[12] = Turn_EM_Out;
  // Variable[13] = Turn_Out; /*Rig_circle;*/ //Road6_flag ;//CarSpeed2;
  // Variable[14] = Allwhitestart * 100 + Allwhiteend;
  // Variable[15] = 100 + Lef_circle * 10 + Rig_circle; //whitecnt;//Road1_turnout;//limit_pos(EM_Value_1/1.5-EM_Value_2/3.5);//speedTarget1;//map_line[MIN(50,AllWhileStartLine)];

  Variable[0] = crossing_flag_flag;//EM_Value_4;//crossing_flag_flag;                 //Mid_intercept;//MotorOut1_add;      //Cam_offset;   //EM_Value_1;// Cam_offset;//cntt;//ToF_distance;
  Variable[1] = Turn_Out;                          //EM_Value_2; //Turn_Cam_Out;//cnt;//Turn_Cam_Out;//;//stat_slope;//gyroy_1;//Turn_Cam_Out;//Car_W;
  Variable[2] = Allwhitestart * 100 + Allwhiteend; //EM_Value_1;//Cam_offset;
  Variable[3] = CarSpeed1;
  Variable[4] = CarSpeed2;    //EM_Value_1; //speedTarget1; //Allwhiteend;
  Variable[5] = speedTarget1; //EM_Value_2;
  Variable[6] = car_straight_dias;//speedTarget2; //EM_Value_3;
  Variable[7] = car_center_dias;//EM_Value_2 + EM_Value_3;
  if (Road == 0)
  {
    Variable[8] = Road0_flag;
  }
  else if (Road == 1)
  {
    Variable[8] = Road1_flag;
  }
  else if (Road == 2)
  {
    Variable[8] = Road2_flag;
  }
  else if (Road == 3)
  {
    Variable[8] = Road3_flag;
  }
  else if (Road == 4)
  {
    Variable[8] = Road4_flag;
  }
  else if (Road == 7)
  {
    Variable[8] = Road7_flag;
  }
  if (Road > 0)
  {
    Variable[8] += Road * 10;
  }
  Variable[9] = Lef_slope;  //EM_offset;
  Variable[10] = Rig_slope; //speedTarget2;//Turn_Cam_Out;
  Variable[11] = EM_Value_1;//EM_Value_1;//MotorOut1;
  Variable[12] = EM_Value_2;//MotorOut2;         //Turn_EM_Out;  //Lef_break_point*100+Rig_break_point;
  Variable[13] = EM_Value_3;
  Variable[14] = EM_Value_4;//speed_change_flag;//car_center_dias;   //100 + Lef_circle * 10 + Rig_circle; /*Rig_circle;*/ //Road6_flag ;//CarSpeed2;
  Variable[15] = loop_time;//EM_Value_1;         // Mid_slope; //Allwhitestart * 100 + Allwhiteend;              //EM_Value_2+EM_Value_3; //whitecnt;//Road1_turnout;//limit_pos(EM_Value_1/1.5-EM_Value_2/3.5);//speedTarget1;//map_line[MIN(50,AllWhileStartLine)];
}

void Pic_send_new2(void)
{
  int i;
  int j;
  cnt = 0;
#ifdef ori_pic
  for (i = 0; i < 20; ++i)
  {
    j = (int)(i * 2.5 + 5);
    if (New_Lef[j] < -395)
    {
      Pic_new[cnt] = 0;
      cnt++;
    }
    else if (New_Lef[j] > 395)
    {
      Pic_new[cnt] = 79;
      cnt++;
    }
    else
    {
      Pic_new[cnt] = (New_Lef[j] + 395) / 10;
      cnt++;
    }

    if (New_Rig[j] < -395)
    {
      Pic_new[cnt] = 0;
      cnt++;
    }
    else if (New_Rig[j] > 395)
    {
      Pic_new[cnt] = 79;
      cnt++;
    }
    else
    {
      Pic_new[cnt] = (New_Rig[j] + 395) / 10;
      cnt++;
    }
    Pic_new[cnt] = 0xff;
    cnt++;
  }
  for (i = 20; i < 60; i++)
  {
    if (Pixle[i][0] == 1)
    {
      Pic_new[cnt] = 0;
      cnt++;
    }
    for (j = 1; j < 80; j++)
    {
      if (Pixle[i][j] != Pixle[i][j - 1])
      {
        Pic_new[cnt] = j;
        cnt++;
      }
    }
    Pic_new[cnt] = 0xff;
    cnt++;
  }
#endif

#ifdef und_pic
  for (i = 0; i < 60; i++)
  {
    if ((int)(New_Lef[i] + 450) * 0.089 <= 0)
    {
      Pic_new[cnt] = 0;
      cnt++;
      Pic_new[cnt] = 1;
      cnt++;
    }
    else
    {
      Pic_new[cnt] = (int)(New_Lef[i] + 450) * 0.089;
      cnt++;
      Pic_new[cnt] = (int)(New_Lef[i] + 450) * 0.089 + 1;
      cnt++;
    }

    Pic_new[cnt] = (int)(New_Rig[i] + 450) * 0.089;
    cnt++;
    Pic_new[cnt] = (int)(New_Rig[i] + 450) * 0.089 + 1;
    cnt++;
    Pic_new[cnt] = 0xff;
    cnt++;
  }
#endif
}

/*************************************************************************
*  函数名称：void Send_Img(void)
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2019.3.6
*  备    注：

*************************************************************************/

void Send_Img2(void)
{

  uint16 i = 0, num;
  uint16 j = 0;
  My_Put_Char(0x55);
  My_Put_Char(0xaa);
  My_Put_Char(0xff);
  My_Put_Char(0xa2);
  My_Put_Char(0x01); //小锟斤拷状态

  num = cnt + 2 + 180; //4+4*2
  //num = cnt+2+180;
  //num=cont+2+180+36;
  //统计将要传输的数据量 2是因为要传输关键字即0xf0和0xf2   (0xf2 所有数据结束，0xf0图像数据结束，180=60*3)
  //180是边线的位 36是变量的位 如果不传输就不要加上！

  My_Put_Char(BYTE0(num));
  My_Put_Char(BYTE1(num));
  for (i = 0; i < cnt; i++)
  {
    My_Put_Char(Pic_new[i]);
  }
  //    for(i=0;i< 60;i++)
  //    {
  //            My_Put_Char(Lef[i]);
  //            My_Put_Char(Rig[i]);
  //            My_Put_Char(0xff);
  //    }
  My_Put_Char(0xf0); //代表图像数据发完了
  /******************星号围起来的可以不传输*******************/

  ////////////////////////传输左右边线和计算得到的中线///////////
  for (i = 0; i < 20; ++i)
  {
    j = (int)(i * 2.5 + 5);
    if (New_Lef[j] < -395)
    {
      My_Put_Char(0);
    }
    else if (New_Lef[j] > 395)
    {
      My_Put_Char(79);
    }
    else
    {
      My_Put_Char((New_Lef[j] + 395) / 10);
    }
  }
  for (i = 20; i < 60; i++)
  {
    My_Put_Char(Lef[i]); //New_Lef[i]/10+40);
  }

  for (i = 0; i < 20; ++i)
  {
    j = (int)(i * 2.5 + 5);
    if (New_Mid[j] < -395)
    {
      My_Put_Char(0);
    }
    else if (New_Mid[j] > 395)
    {
      My_Put_Char(79);
    }
    else
    {
      My_Put_Char((New_Mid[j] + 395) / 10);
    }
  }
  for (i = 20; i < 60; i++)
  {
    My_Put_Char(Mid[i]);
  }

  for (i = 0; i < 20; ++i)
  {
    j = (int)(i * 2.5 + 5);
    if (New_Rig[j] < -395)
    {
      My_Put_Char(0);
    }
    else if (New_Rig[j] > 395)
    {
      My_Put_Char(79);
    }
    else
    {
      My_Put_Char((New_Rig[j] + 395) / 10);
    }
  }

  for (i = 20; i < 60; i++)
  {
    My_Put_Char(Rig[i] + 1); //New_Rig[i]/10+40);
  }

  /*****************************************************/
  My_Put_Char(0xf2); //代表整个数据都发完了
}
