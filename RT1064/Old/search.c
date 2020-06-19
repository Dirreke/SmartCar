#include "headfile.h" //20行=90cm

#define max_block(a, b) ((a) > (b) ? (a) : (b))

int threshold_offset = -5;
int threshold_offset2 = 0;

int Lef[LCDH];     //道路左分离点的纵坐标
int Rig[LCDH];     //道路右分离点的纵坐标
// int Lef_err[LCDH]; //用于判断分离点是否成功，若找到，则置1
// int Rig_err[LCDH];
int Mid[LCDH]; //道路中心点的纵坐标
int Middle = 41;

float LR_slope;
// float Cam_offset2;

// int area[5] = {Last_row - 8, 48, 34, Last_row - 33, Last_row}; //分区域计算偏差的边界划分


int Allwhitestart;     //全白行开始行
int Allwhiteend;       //全白行结束行
// int Allwhitestart2;
// int Allwhiteend2;
// int Fix1x, Fix2x, Fix1y, Fix2y;
// int Fix_line;  //补线标志位
// int Fix[LCDH]; //用于补线

int Lef_circle;
int Rig_circle; //左右边线寻找环状黑线标志位
int Lef_break_point;
int Rig_break_point; //左右环状黑线拐弯点
// int Lef_leap[4];
// int Rig_leap[4]; //左右变线跳变点坐标

// uint16 Lef_road = 0; //左右岔路标志位，若检测到有岔路，则置1
// uint16 Rig_road = 0;

int Road0_flag = 0;
int Road1_flag = 0;
int Road2_flag = 0;
int Road7_flag = 0;

int start_stop_line = 0;
// int Lef_start = Fir_row, Rig_start = Fir_row, Rig_sep1 = Fir_row, Rig_sep2 = Fir_row, Lef_sep1 = Fir_row, Lef_sep2 = Fir_row, Lef_lose, Rig_lose; //左右赛道起始点、以及两个岔路点
int turn_stop = 0; //转弯用

float Lef_slope = 0;
float Rig_slope = 0;

float Cam_offset = 0;

int Road = 0; //道路类型标志位
// int Road3_state;
// int cross_flag = 0;
// int unclear = 0;
int New_Lef[60];
int New_Rig[60]; //用于存储逆透视变换后的横坐标
int New_Mid[60];
// int Cam_offset = 0;
// int Cam_Break_Flag = 0;

/*
extern float EM_Turn_Control;
extern float Turn_P_EM;
extern float Turn_I_EM;
extern float Turn_D_EM;
extern int  EM_Road;
extern int EM_Ring_State;
extern float EM_Value_23_Sum;
extern float EM_Road1_Cnt;
extern float EM_Road2_Cnt;
*/
// extern int16 qd1_result;

// int Lef_End = 0;
// int Rig_End = 0;
// int Cam_End = 0;
// int block_flag = 1;

/*调试参数*/
/*
float offset_map[60]={(float)31/77,(float)31/77,(float)31/9	,(float)31/9	 ,(float)31/10	,(float)31/11	,(float)31/12	 ,(float)31/13	,(float)31/14,(float)31/15,(float)31/16
,(float)31/17,(float)31/17,(float)31/19,(float)31/19,(float)31/21	,(float)31/21,(float)31/23,(float)31/24	,(float)31/25	 ,(float)31/25	,(float)31/27,(float)31/28,(float)31/29
,(float)31/30,(float)31/31,(float)31/31	,(float)31/33,(float)31/34,(float)31/34,(float)31/36,(float)31/36	,(float)31/36	,(float)31/38	,(float)31/40,(float)31/40,(float)31/42,(float)31/42
,(float)31/42	,(float)31/44	 ,(float)31/46	 ,(float)31/46	 ,(float)31/48	  ,(float)31/48	,(float)31/48	,(float)31/50	,(float)31/51,(float)31/52,(float)31/53,(float)31/54,(float)31/54,(float)31/56,(float)31/56,(float)31/58,(float)31/58,(float)31/58,(float)31/60,(float)31/60,(float)31/61,(float)31/77};
*/
// float K0_Table[5] = {ROAD_WIDTH / 65, ROAD_WIDTH / 66, ROAD_WIDTH / 67, ROAD_WIDTH / 68, ROAD_WIDTH / 69};

float zhidaosudu = 2.5;     //直道速度
float xiaowandaosudu = 2.3; //小弯道速度
float dawandaosudu = 2.3;   //大弯道速度
// float duanlusudu = 1.8;     //断路速度
// int camera_offset = 0; //摄像头二值化阈值
// int Tof_thres = 150;        //障碍物检测阈值
// float luzhangsudu = 2.3;    //路障速度

/*************************************************************************
*  函数名称：my_key_debug()
*  功能说明：按键调试函数
*  参数说明：无
*  函数返回：无
*  修改时间：2019.3.6
*  备    注：
*************************************************************************/
/*
void my_key_debug()
{

  static int state = 0;
  static int32 ips_num = 0;


        state = SW_Read();
        switch(state)
        {

          case 0:
            ips_showint32(150,0,(int32)ips_num,2);
            if(ips_num>10)
            {
            ips_num = 0;
            }
            if(ips_num<0)
            {
            ips_num = 10;
            }
            ips_showfloat(0,0,(double)zhidaosudu,3,2);
            ips_showfloat(0,1,(double)xiaowandaosudu,3,2);
            ips_showfloat(0,2,(double)dawandaosudu,3,2);
            ips_showfloat(0,3,(double)duanlusudu,3,2);
            ips_showfloat(0,4,(double)luzhangsudu,3,2);
            ips_showfloat(0,5,(double)Block_Angle_Set,3,2);
            ips_showfloat(0,6,(double)Block_Angle_Set2,3,2);
            ips_showint32(0,7,(int32)camera_offset,3);
            ips_showint32(150,1,(int32)Tof_thres,3);
            ips_showint32(150,2,(int32)block_flag,3);
            ips_showint32(150,3,(int32)threshold_offset,2);



            if(!KEY_Read(KEY0))
            {
              systick_delay_ms(50);

            if(!KEY_Read(KEY0))
             {
              ips_clear(IPS_WHITE);
              switch(ips_num)
            {
              case 0:
                zhidaosudu+=0.1;
                break;
              case 1:
                xiaowandaosudu+=0.1;
                break;
              case 2:
                dawandaosudu+=0.1;
                break;
              case 3:
                duanlusudu+=0.1;
                break;
              case 4:
                luzhangsudu+=0.1;
                break;
              case 5:
                Block_Angle_Set+=1;
                break;
              case 6:
                Block_Angle_Set2+=1;
                break;
              case 7:
                camera_offset+=1;
                break;
              case 8:
                Tof_thres+=5;
                break;
              case 9:
                block_flag = 1;
                break;
              case 10:
                threshold_offset += 1;
                break;
              default:
                break;

            }

             }
             }
            if(!KEY_Read(KEY1))
            {
            systick_delay_ms(50);
            if(!KEY_Read(KEY1))
            {
              ips_clear(IPS_WHITE);
              switch(ips_num)
            {

              case 0:
                zhidaosudu-=0.1;
                break;
              case 1:
                xiaowandaosudu-=0.1;
                break;
              case 2:
                dawandaosudu-=0.1;
                break;
              case 3:
                duanlusudu-=0.1;
                break;
              case 4:
                luzhangsudu-=0.1;
                break;
              case 5:
                Block_Angle_Set-=1;
                break;
              case 6:
                Block_Angle_Set2-=1;
                break;
              case 7:
                camera_offset-=1;
                break;
              case 8:
                Tof_thres-=5;
                break;
              case 9:
                block_flag = -1;
                break;
              case 10:
                threshold_offset -= 1;
              default:
                break;

            }

            }
            }

            if(!KEY_Read(KEY2))
            {
            systick_delay_ms(50);
            if(!KEY_Read(KEY2))
            {
              ips_clear(IPS_WHITE);
              ips_num++;
            }
            }

            if(!KEY_Read(KEY3))
            {
            systick_delay_ms(50);
            if(!KEY_Read(KEY3))
            {
              ips_clear(IPS_WHITE);
              ips_num--;
            }
            }
            break;

          case 1:
            if(!KEY_Read(KEY0))
            {
            systick_delay_ms(50);
            if(!KEY_Read(KEY0))
            {
            ips_clear(IPS_WHITE);
            }
            }
            ips_displayimage032_01Value(Pixle[0], CAMERA_W, CAMERA_H);
            break;
          case 2:
            return;
            break;
          case 3:
            return;
            break;
          default:
            break;



        }

}
*/
#if 0
/*************************************************************************
*  函数名称：void Pic_seedfill(void)
*  功能说明：DFS搜索种子填充算法
*  参数说明：无
*  函数返回：将Sobel边缘提取后的图像进行填充以及干扰的去除
*  修改时间：2019.5.27
*  备    注：子函数grow()为递归函数。
*************************************************************************/

void Pic_seedfill(void)
{
  uint8 flag[CAMERA_H][CAMERA_W] = {0};
  int i, j;
  int seed_flag = 0, seed_i = -1, seed_j = -1;

  //设定基准团(点)
  for (i = seed_down; i >= seed_up; i = i - 3)
  {
    for (j = seed_left; j <= seed_right; j = j + 3)
    {
      if (Pixle[i][j] == 1)
      {
        seed_flag = 1;
        seed_i = i;
        seed_j = j;
        break;
      }
    }
    if (seed_flag == 1)
      break;
  }
  if (seed_flag == 0) //没找到基准团,返回-1
    return;
  if (Lef_slope == 999 && Rig_slope < 900 && Road != 2)
  {
    seed_i = 55;
    seed_j = 10;
  }
  if (Rig_slope == 999 && Lef_slope < 900 && Road == 1)
  {
    seed_i = 55;
    seed_j = 10;
  }
  //生长填充
  Pic_seedfill_grow(flag, seed_i, seed_j);

  for (i = CAMERA_H - 1; i >= 0; i--)
    for (j = 0; j <= CAMERA_W - 1; j++)
    {
      Pixle[i][j] = flag[i][j];
      if (flag[i][j] == 2)
        Pixle[i][j] = 0;
      // 	Bondary[i][j]=1;
    }

  return;
}

void Pic_seedfill_grow(uint8 flag[CAMERA_H][CAMERA_W], int i, int j)
{
  //判定 深度优先搜索 方向下左右上 带边界条件
  int grow_i, grow_j;

  for (grow_i = i - 1; grow_i <= i + 1; grow_i++)
    for (grow_j = j - 1; grow_j <= j + 1; grow_j++)
      if (Pixle[grow_i][grow_j] == 1)
        flag[grow_i][grow_j] = 1;
      else
        flag[grow_i][grow_j] = 2; //2表明原图像为黑色0，且已搜过

  // if(Pixle[i][j]==1)
  // 	flag[i][j]=1;

  //down
  if (Pixle[i + 1][j] == 1 && Pixle[i + 2][j] == 1 && flag[i + 3][j] == 0 && i < BOTTOM - 3) //条件：1、九宫格下中心下一下二均为白 2、下三没搜过 3、下三小于边界  //会忽略掉最后一行或两行
    Pic_seedfill_grow(flag, i + 3, j);

  //left
  if (Pixle[i][j - 1] == 1 && Pixle[i][j - 2] == 1 && flag[i][j - 3] == 0 && j > LEFT + 3)
    Pic_seedfill_grow(flag, i, j - 3);
  //right
  if (Pixle[i][j + 1] == 1 && Pixle[i][j + 2] == 1 && flag[i][j + 3] == 0 && j < RIGHT - 3)
    Pic_seedfill_grow(flag, i, j + 3);
  //up
  if (Pixle[i - 1][j] == 1 && Pixle[i - 2][j] == 1 && flag[i - 3][j] == 0 && i > TOP + 3)
    Pic_seedfill_grow(flag, i - 3, j);

  return;
}
#endif
/*************************************************************************
*  函数名称：void Pic_noi_elim()
*  功能说明：照片噪点消除
*  参数说明：无
*  函数返回：无
*  修改时间：2019.2.20
*  备    注：某点三面相反视为噪点
*************************************************************************/
int whitecnt = 0;
void Pic_noi_elim(void)
{
  int nr; //行
  int nc; //列
  whitecnt = 0;
  for (nr = Fir_row; nr < LCDH - 1; nr++)
  {
    for (nc = 1; nc < LCDW - 1; nc = nc + 1)
    {
      if ((Pixle[nr][nc] == 0) && (Pixle[nr - 1][nc] + Pixle[nr + 1][nc] + Pixle[nr][nc + 1] + Pixle[nr][nc - 1] > 2))
      {
        Pixle[nr][nc] = 1;
      }
      else if ((Pixle[nr][nc] == 1) && (Pixle[nr - 1][nc] + Pixle[nr + 1][nc] + Pixle[nr][nc + 1] + Pixle[nr][nc - 1] < 2))
      {
        Pixle[nr][nc] = 0;
      }
      if (Pixle[nr][nc] == 1)
        whitecnt += 1;
    }
  }
}
#if 0
/*************************************************************************
*  函数名称：void Pic_thresh_get()
*  功能说明：二值化阈值预获取
*  参数说明：无
*  函数返回：无
*  修改时间：2019.3.15
*  备    注：没有用到
*************************************************************************/

void Pic_thresh_get(void)
{
  int i = 0;
  int thresh_sum = 0;
  for (i = area[2]; i < area[1]; i++)
  {
    thresh_sum = thresh_sum + Image_Use[i][Mid[i]];
  }
  Threshold = (int)(thresh_sum * 1.0 / (area[1] - area[2])) - 20;
}

#endif

char Road1_turnin(float a, float b, float c)
{
  //static float c=3.8;
  if (a * b > c)
    return 1;
  else if ((a - 3 < 0.1 || 3 - a < 0.1) && (b < c / 3 && b > 0))
    return 1;
  else if ((b - 3 < 0.1 || 3 - b < 0.1) && (a < c / 3 && a > 0))
    return 1;
  return 0;
}
#if 0
/*************************************************************************
*  函数名称：void Road_rec()
*  功能说明：赛道识别
*  参数说明：无
*  函数返回：无
*  修改时间：2019.3.23
*  备    注：Road=0表示正常路况（即没有进入圆环及坡）
             3.23:Road=1表示左圆环，Road=2表示右圆环

*************************************************************************/

char Road1_turnout = 1;
void Road_rec(void)
{
  static int Road00_count = 0, Road03_count = 0, Road04_count = 0;
  static int Road11_count = 0, Road12_count = 0, Road13_count = 0, Road14_count = 0, Road15_count = 0;
  static int Road21_count = 0, Road22_count = 0, Road23_count = 0, Road24_count = 0, Road25_count = 0;
  // static int oldwhite=5000;
  // static uint8 Road1_cnt1=0;
  // static char Road1_flag1=0;
  if (Road == 0)
  {

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////普通赛道→圆环
    if ((Lef_break_point < 45 && Road == 0 && Rig_circle == 0 && Lef_circle == 1 &&  Lef_slope != 998 && Rig_slope == 998 &&
         Rig[39] - Rig[37] < 5 && Rig[37] - Rig[35] < 5 && Rig[35] - Rig[33] < 5 && Rig[33] - Rig[31] < 5 && Rig[31] - Rig[29] < 5 && Rig[29] - Rig[27] < 5 && Rig[27] - Rig[25] < 5 && Rig[25] - Rig[23] < 5 &&
        //  (New_Lef[54] == -MIDMAP || New_Lef[55] == -MIDMAP || New_Lef[56] == -MIDMAP)&& 
             Rig_edge < 10)) //左圆环：左边线,右边线：直通到底//&& Rig[11] != 78
    {
      Road0_flag = 0;
      Road11_count++;
      if (Road11_count == 2 && Road1_flag == 0)
      {
        Road = 1;
        Road11_count = 0;
      }
      return;
    }
    else if (Rig_break_point < 45 && Road == 0 && Lef_circle == 0 && Rig_circle == 1 &&Rig_slope != 998 &&  Lef_slope == 998 && 
             Lef[25] - Lef[27] < 5 && Lef[27] - Lef[29] < 5 && Lef[29] - Lef[31] < 5 && Lef[31] - Lef[33] < 5 && Lef[33] - Lef[35] < 5 && Lef[35] - Lef[37] < 5 && Lef[37] - Lef[39] < 5 &&  Lef[23] - Lef[25] < 5 &&
          //     (New_Rig[54] == MIDMAP || New_Rig[55] == MIDMAP || New_Rig[56] == MIDMAP) && 
                 Lef_edge < 10) //右圆环：右边线：突变点→拐点→突变点//&& Lef[11] != 2
    {
      Road0_flag = 0;
      Road21_count++;
      if (Road21_count == 2 && Road2_flag == 0)
      {
        Road = 2;
        Road21_count = 0;
      }
      return;
    }
    //弯道
    else if ((Rig_break_point > 40 && Lef_circle == 0 && Rig_circle == 1 && Rig_slope != 998)) //左转弯
    {
      Road03_count++;
      if (Road03_count == 2)
      {
        Road0_flag = 3;   //turn left flag
        Road03_count = 0; //reset
      }
      return;
    }
    else if ((Lef_break_point > 40 && Rig_circle == 0 && Lef_circle == 1 && Lef_slope != 998)) //右转弯
    {
      Road04_count++;
      if (Road04_count == 2)
      {
        Road0_flag = 4;   //turn right flag
        Road04_count = 0; //reset
      }
      return;
    }
  }
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////左圆环→普通赛道
  else if (Road == 1)
  {
    if (Road1_flag == 0) //准备进左圆环
    {
      Road0_flag = 0;
      // Road1_flag = 1;
      if (EM_Value_2 + EM_Value_3 > 4.3) //弯内识别：左右两边仅有一边发生丢线
      {
        Road12_count++;
        if (Road12_count == 2)
        {
          Road12_count = 0;
          Road1_flag = 1; //表示已经进入左圆环
        }
        return;
      }
      return;
    }
    else if (Road1_flag == 1) //进左圆环1/4
    {
      Road0_flag = 0;
      if (Lef_circle == 0 || (Lef_circle == 1 && Lef_break_point > 45)) //if(((Lef_circle==0||( Lef_circle ==1 && Lef_break_point>30)))&& Road1_turnin(EM_Value_2,EM_Value_3,3.8))//if(((Lef_circle==0||( Lef_circle ==1 && Lef_break_point>30))) && Rig_slope>=10)/ && Road1_turnin(EM_Value_2,EM_Value_3,3.8))    //Rig_slope<1 && (Lef_leap[0]==0||Lef_slope==999)&& Rig_leap[0]==0)
      {
        Road13_count++;
        if (Road13_count == 2) //2帧后 进左圆环第一弯道
        {
          Road1_flag = 2;
          //        Road1_flag1 = 1;
          Road13_count = 0;
          return;
        }
        return;
      }
    }
    else if (Road1_flag == 2) //进左圆环2/4 开始补线进弯道
    {
      Road0_flag = 0;
      Road14_count++;
      if (Road14_count == (int)(DIS_IN_CIRCLE * 10000 / (get_speed() * CAMERA_FPS)) + 1) //宏定义在function.h
      {
        Road1_flag = 4;
        Road14_count = 0;
        return;
      }
      return;
    }
    else if (Road1_flag == 4) //进入圆环内 ，取消补线
    {
      Road0_flag = 0;
      if (Rig_circle && whitecnt > 1200) //
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
    else if (Road1_flag == 3) //准备出圆环回到直路 ， 开始补线
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
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////右圆环→普通赛道，同上左圆环，注释从简
  else if (Road == 2)
  {
    if (Road2_flag == 0) //
    {
      Road0_flag = 0;
      // Road2_flag = 1;
      if (EM_Value_2 + EM_Value_3 > 4.3) //弯内识别：左右两边仅有一边发生丢线
      {
        Road22_count++;
        if (Road22_count == 2)
        {
          Road22_count = 0;
          Road2_flag = 1; //表示已经进入左圆环
        }
      }

      return;
    }
    else if (Road2_flag == 1) //
    {
      Road0_flag = 0;
      if ((Rig_circle == 0 || (Rig_circle == 1 && Rig_break_point > 45)) && Rig_slope >= 10) //if(((Rig_circle==0||( Rig_circle ==1 && Rig_break_point>30))) && Rig_slope>=10)/ && Road1_turnin(EM_Value_2,EM_Value_3,3.4))
      {
        Road23_count++;
        if (Road23_count == 2) //
        {

          Road2_flag = 2;
          Road23_count = 0;
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
      }
      return;
    }
    else if (Road2_flag == 4)
    {
      Road0_flag = 0;
      if (whitecnt > 1200)
      {
        Road25_count++;
        if (Road25_count == 3)
        {
          Road25_count = 0;
          Road2_flag = 3;
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
        }
      }
      return;
    }
  }

  if (Road == 0 && whitecnt > 1700 && ((Lef_edge > 10 && Rig_edge > 10) || Lef_edge > 30 || Rig_edge > 30) && Allwhitestart < 40)
  {
    Road0_flag = 1;
  }
  else if ((Allwhitestart > 45 && Allwhiteend < 45) && whitecnt > 1900)
  {
    Road0_flag = 2;
  }
  else
  {
    Road00_count++;
    if (Road25_count == 3)
    {
      Road00_count = 0;
      Road0_flag = 0;
    }
  }
}

#endif
#if 1
/*************************************************************************
*  函数名称：void Road_rec()
*  功能说明：赛道识别
*  参数说明：无
*  函数返回：无
*  修改时间：2019.3.23
*  备    注：Road=0表示正常路况（即没有进入圆环及坡）
             3.23:Road=1表示左圆环，Road=2表示右圆环

*************************************************************************/

// char Road1_turnout = 1;
void Road_rec(void)
{
  static int Road0_count = 0;
  static int Road00_count = 0, Road03_count = 0, Road04_count = 0, Road05_count = 0;
  static int Road11_count = 0, Road12_count = 0, Road13_count = 0, Road14_count = 0, Road15_count = 0, Road16_count = 0;
  static int Road21_count = 0, Road22_count = 0, Road23_count = 0, Road24_count = 0, Road25_count = 0, Road26_count = 0;
  static int turn_stop_flag = 0;
  int dis = 0, dis1 = 0;
  int i = 0;

  // static int oldwhite=5000;
  // static uint8 Road1_cnt1=0;
  // static char Road1_flag1=0;
  if (Lef_slope == 998 && Rig_slope == 998)
  {
    Road0_count++;
    if (Road0_count >= 5)
    {
      Road = 0;
    }
  }
  else
  {
    Road0_count = 0;
  }

  if (Road == 0)
  {

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////普通赛道→圆环
    if ((Lef_break_point < 45 && Road == 0 && Rig_circle == 0 && Lef_circle == 1 && Lef_slope != 998 && Rig_slope == 998 &&
         Rig[39] - Rig[37] < 5 && Rig[37] - Rig[35] < 5 && Rig[35] - Rig[33] < 5 && Rig[33] - Rig[31] < 5 && Rig[31] - Rig[29] < 5 && Rig[29] - Rig[27] < 5 && Rig[27] - Rig[25] < 5 && Rig[25] - Rig[23] < 5 &&
         //  (New_Lef[54] == -MIDMAP || New_Lef[55] == -MIDMAP || New_Lef[56] == -MIDMAP)&&
         Rig_edge < 10)) //左圆环：左边线,右边线：直通到底//&& Rig[11] != 78
    {
      Road0_flag = 0;
      Road11_count++;
      if (Road11_count == 2 && Road1_flag == 0)
      {
        Road = 1;
        Road11_count = 0;
      }
      return;
    }
    else if (Rig_break_point < 45 && Road == 0 && Lef_circle == 0 && Rig_circle == 1 && Rig_slope != 998 && Lef_slope == 998 &&
             Lef[25] - Lef[27] < 5 && Lef[27] - Lef[29] < 5 && Lef[29] - Lef[31] < 5 && Lef[31] - Lef[33] < 5 && Lef[33] - Lef[35] < 5 && Lef[35] - Lef[37] < 5 && Lef[37] - Lef[39] < 5 && Lef[23] - Lef[25] < 5 &&
             //     (New_Rig[54] == MIDMAP || New_Rig[55] == MIDMAP || New_Rig[56] == MIDMAP) &&
             Lef_edge < 10) //右圆环：右边线：突变点→拐点→突变点//&& Lef[11] != 2
    {
      Road0_flag = 0;
      Road21_count++;
      if (Road21_count == 2 && Road2_flag == 0)
      {
        Road = 2;
        Road21_count = 0;
      }
      return;
    }
    //弯道状态机
    else if (((Lef_break_point > 35 && Lef_circle == 1 && Rig_circle == 0) || Road0_flag == 4) && Rig_slope != 998) //左转弯
    {

      for (i = Fir_row; i < 40; ++i)
      {
        if (Rig[i] < 40 && Rig[i + 1] <= 40 && Rig[i + 2] >= 40 && Rig[i + 3] > 40 &&
            Rig[i + 5] - Rig[i + 3] < 7 && Rig[i + 7] - Rig[i + 5] < 7 && Rig[i + 9] - Rig[i + 7] < 7 && Rig[i + 11] - Rig[i + 9] < 7)
        //可能较严，（出现连续边线为40）
        {
          break;
        }
      }
      if (i == 40 && turn_stop_flag == 1)
      {
        Road00_count++;
        if (Road00_count == 2)
        {
          Road00_count = 0;
          Road0_flag = 0;
          turn_stop_flag = 0;
          threshold_offset2 = -5;
          return;
        }
      }
      else
      {
        Road00_count = 0;
      }
      dis = Rig[i + 1] - Rig[i];
      for (; i > Fir_row; --i)
      {
        if (Rig[i - 1] > 40)
        {
          break;
        }
        dis1 = Rig[i] - Rig[i - 1];
        if (dis1 < 0)
        {
          break;
        }
        else if (dis1 < dis)
        {
          continue;
        }
        else if (dis1 <= 2 * dis + 1)
        {
          dis = dis1;
          continue;
        }
        else
        {
          break;
        }
      }
      turn_stop = i;
      if (turn_stop < 28)
      {
        turn_stop_flag = 1;
      }
      if (Rig[turn_stop] < 34 && dis > 4)
      {
        Road04_count++;
        if (Road04_count == 2)
        {
          Road0_flag = 4; //turn left flag
          threshold_offset2 = -5;
          Road04_count = 0; //reset
        }
        return;
      }
      return;
    }
    else if (((Rig_break_point > 35 && Rig_circle == 1 && Lef_circle == 0) || Road0_flag == 5) && Lef_slope != 998) //右转弯
    {
      for (i = Fir_row; i < 40; ++i)
      {
        if (Lef[i] > 40 && Lef[i + 1] >= 40 && Lef[i + 2] <= 40 && Lef[i + 3] < 40 &&
            Lef[i + 3] - Lef[i + 5] < 7 && Lef[i + 5] - Lef[i + 7] < 7 && Lef[i + 7] - Lef[i + 9] < 7 && Lef[i + 9] - Lef[i + 11] < 7)
        {
          break;
        }
      }
      if (i == 40 && turn_stop_flag == 1)
      {
        Road00_count++;
        if (Road00_count == 3)
        {
          Road00_count = 0;
          Road0_flag = 0;
          threshold_offset2 = -5;
          turn_stop_flag = 0;
          return;
        }
        else
        {
          Road00_count = 0;
        }
      }
      dis = Lef[i] - Lef[i + 1];
      for (; i > Fir_row; --i)
      {
        if (Lef[i - 1] < 40)
        {
          break;
        }
        dis1 = Lef[i - 1] - Lef[i];
        if (dis1 < 0)
        {
          break;
        }
        else if (dis1 < dis)
        {
          continue;
        }
        else if (dis1 <= 2 * dis + 1)
        {
          dis = dis1;
          continue;
        }
        else
        {
          break;
        }
      }
      turn_stop = i;
      if (Lef[turn_stop] > 45 && dis > 4)
      {
        Road05_count++;
        if (Road05_count == 2)
        {
          Road0_flag = 5; //turn left flag
          threshold_offset2 = -5;
          Road05_count = 0; //reset
        }
        return;
      }
      return;
    }
  }
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////左圆环→普通赛道
  else if (Road == 1)
  {
    if (Road1_flag == 0) //准备进左圆环
    {
      Road0_flag = 0;
      // Road1_flag = 1;
      if (EM_Value_2 + EM_Value_3 > 4.3) //弯内识别：左右两边仅有一边发生丢线
      {
        Road12_count++;
        if (Road12_count == 2)
        {
          Road12_count = 0;
          Road1_flag = 1; //表示已经进入左圆环
        }
        return;
      }
      return;
    }
    else if (Road1_flag == 1) //进左圆环1/4
    {
      // Road0_flag = 0;
      if (Lef_circle == 0 || (Lef_circle == 1 && Lef_break_point > 45)) //if(((Lef_circle==0||( Lef_circle ==1 && Lef_break_point>30)))&& Road1_turnin(EM_Value_2,EM_Value_3,3.8))//if(((Lef_circle==0||( Lef_circle ==1 && Lef_break_point>30))) && Rig_slope>=10)/ && Road1_turnin(EM_Value_2,EM_Value_3,3.8))    //Rig_slope<1 && (Lef_leap[0]==0||Lef_slope==999)&& Rig_leap[0]==0)
      {
        Road13_count++;
        if (Road13_count == 2) //2帧后 进左圆环第一弯道
        {
          Road1_flag = 2;
          //        Road1_flag1 = 1;
          Road13_count = 0;
          return;
        }
        return;
      }
    }
    else if (Road1_flag == 2) //进左圆环2/4 开始补线进弯道
    {
      // Road0_flag = 0;
      Road14_count++;
      if (Road14_count == (int)(DIS_IN_CIRCLE * 10000 / (get_speed() * CAMERA_FPS)) + 1) //宏定义在function.h
      {
        Road1_flag = 4;
        Road14_count = 0;
        return;
      }
      return;
    }
    else if (Road1_flag == 4) //进入圆环内 ，取消补线
    {
      i = 40;
      dis = Rig[i + 1] - Rig[i];
      for (; i > Fir_row; --i)
      {
        if (Rig[i - 1] > 40)
        {
          break;
        }
        dis1 = Rig[i] - Rig[i - 1];
        if (dis1 < 0)
        {
          break;
        }
        else if (dis1 < dis)
        {
          continue;
        }
        else if (dis1 <= 2 * dis + 1)
        {
          dis = dis1;
          continue;
        }
        else
        {
          break;
        }
      }
      turn_stop = i;
      // Road0_flag = 0;
      if (Rig_circle && whitecnt > 1200) //Rig_circlr 不好，改!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
    else if (Road1_flag == 3) //准备出圆环，右边线补线
    {
      if (Allwhitestart >= 45)
      {
        Road15_count++;
        if (Road15_count == 2)
        {
          Road15_count = 0;
          Road1_flag = 5;
        }
      }
    }
    else if (Road1_flag == 5) //右边线已经不能补线，电磁等方法跑
    {
      // Road0_flag = 0;
      // if ((Rig_slope > -0.02 && Rig_slope < 0) || (Pixle[58][74] == 1 && Pixle[57][74] == 1 && Pixle[56][74] == 1 && Pixle[55][74] == 1 && Pixle[54][74] == 1 && Pixle[53][74] == 1)) //|| Lef_edge < 20))
      if (((Rig_slope < -0.1 || Rig_slope == 998) && Allwhiteend > 45) || Allwhiteend == Fir_row)
      {
        Road16_count++;
        if (Road16_count == 4)
        {
          Road16_count = 0;
          Road = 0;
          //Road1_turnout=1;
          Road1_flag = 0;
          return;
        }
      }
      return;
    }
  }
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////右圆环→普通赛道，同上左圆环，注释从简
  else if (Road == 2)
  {
    if (Road2_flag == 0) //
    {
      Road0_flag = 0;
      // Road2_flag = 1;
      if (EM_Value_2 + EM_Value_3 > 4.3) //弯内识别：左右两边仅有一边发生丢线
      {
        Road22_count++;
        if (Road22_count == 2)
        {
          Road22_count = 0;
          Road2_flag = 1; //表示已经进入左圆环
        }
      }

      return;
    }
    else if (Road2_flag == 1) //
    {
      // Road0_flag = 0;
      if ((Rig_circle == 0 || (Rig_circle == 1 && Rig_break_point > 45)) && Rig_slope >= 10) //if(((Rig_circle==0||( Rig_circle ==1 && Rig_break_point>30))) && Rig_slope>=10)/ && Road1_turnin(EM_Value_2,EM_Value_3,3.4))
      {
        Road23_count++;
        if (Road23_count == 2) //
        {

          Road2_flag = 2;
          Road23_count = 0;
        }
      }
      return;
    }
    else if (Road2_flag == 2) //
    {
      // Road0_flag = 0;
      Road24_count++;
      if (Road24_count == (int)(DIS_IN_CIRCLE * 10000 / (get_speed() * CAMERA_FPS)) + 1)
      {
        Road24_count = 0;
        Road2_flag = 4;
      }
      return;
    }
    else if (Road2_flag == 4)
    {
      i = 40;
      dis = Lef[i] - Lef[i + 1];
      for (; i > Fir_row; --i)
      {
        if (Lef[i - 1] < 40)
        {
          break;
        }
        dis1 = Lef[i - 1] - Lef[i];
        if (dis1 < 0)
        {
          break;
        }
        else if (dis1 < dis)
        {
          continue;
        }
        else if (dis1 <= 2 * dis + 1)
        {
          dis = dis1;
          continue;
        }
        else
        {
          break;
        }
      }
      turn_stop = i;
      // Road0_flag = 0;
      if (Lef_circle && whitecnt > 1200)
      {
        Road25_count++;
        if (Road25_count == 3)
        {
          Road25_count = 0;
          Road2_flag = 3;
        }
      }
      return;
    }

    else if (Road2_flag == 3)
    {
      if (Allwhitestart >= 45)
      {
        Road26_count++;
        if (Road26_count == 2)

        {
          Road26_count = 0;
          Road2_flag = 5;
        }
      }
    }
    else if (Road2_flag == 5)
    {
      // Road0_flag = 0;
      // if ((Lef_slope > 0 && Lef_slope < 0.02) || (Pixle[58][5] == 1 && Pixle[57][5] == 1 && Pixle[56][5] == 1 && Pixle[55][5] == 1 && Pixle[54][5] == 1 && Pixle[53][5] == 1)) //|| Lef_edge < 20))
      if (((Lef_slope > 0.1 || Lef_slope == 998) && Allwhiteend > 45) || Allwhiteend == Fir_row)
      {
        Road26_count++;
        if (Road26_count == 4)
        {
          Road26_count = 0;
          Road = 0;
          //Road2_turnout=1;
          Road2_flag = 0;
        }
      }
      return;
    }
  }

  if (Road == 0 && whitecnt > 1700 && ((Lef_edge > 10 && Rig_edge > 10) || Lef_edge > 30 || Rig_edge > 30) && Allwhitestart <= 45 && Allwhitestart > (Fir_row + 5))
  {
    Road0_flag = 1;
    threshold_offset2 = -8; //十字降低阈值
  }
  else if ((Allwhitestart > 45 && Allwhiteend < 45) && whitecnt > 2000)
  {
    Road0_flag = 2;
    threshold_offset2 = -15;
  }
  else
  {
    Road00_count++;
    if (Road00_count == 2)
    {
      Road00_count = 0;
      Road0_flag = 0;
      threshold_offset2 = 0;
    }
  }
}

#endif
/*************************************************************************
*  函数名称：void Slope_fig()
*  功能说明：赛道左右斜率计算
*  参数说明：无
*  函数返回：无
*  修改时间：2019.3.21
*  备    注：最小二乘法拟合FIG_AREA区域内不为0的斜率

*************************************************************************/

void LR_Slope_fig()
{
  int i;
  float xsum = 0, ysum = 0, xysum = 0, x2sum = 0, count = 0;
  Lef_slope = 0;
  Rig_slope = 0;
  int max = -800, min = 0;
  for (i = 0; i < 60; i++)
  {
    if (i <= FIG_AREA_NEAR && i >= FIG_AREA_FAR3 && New_Lef[i] != -MIDMAP)
    {
      if (New_Lef[i] > max)
      {
        max = New_Lef[i];
      }
      if (New_Lef[i] < min)
      {
        min = New_Lef[i];
      }
      xsum += New_Lef[i];
      ysum += i;
      xysum += New_Lef[i] * i;
      x2sum += New_Lef[i] * New_Lef[i];
      count++;
    }
  }
  if (abs(max - min) > 25)
  {
    if (count * x2sum - xsum * xsum)
    {
      Lef_slope = -(count * xysum - xsum * ysum) / (count * x2sum - xsum * xsum);
    }
    else
    {
      Lef_slope = 999;
    }
  }
  else
  {
    Lef_slope = 998;
  }
  max = 0;
  min = 800;
  xsum = 0;
  ysum = 0;
  xysum = 0;
  x2sum = 0;
  count = 0;
  for (i = 0; i < 60; i++)
  {
    if (i <= FIG_AREA_NEAR && i >= FIG_AREA_FAR3 && New_Rig[i] != MIDMAP)
    {
      if (New_Rig[i] > max)
      {
        max = New_Rig[i];
      }
      if (New_Rig[i] < min)
      {
        min = New_Rig[i];
      }
      xsum += New_Rig[i];
      ysum += i;
      xysum += New_Rig[i] * i;
      x2sum += New_Rig[i] * New_Rig[i];
      count++;
    }
  }
  if (abs(max - min) > 50)
  {
    if (count * x2sum - xsum * xsum)
    {
      Rig_slope = -(count * xysum - xsum * ysum) / (count * x2sum - xsum * xsum);
    }
    else
    {
      Rig_slope = 999;
    }
  }
  else
  {
    Rig_slope = 998;
  }
  LR_slope = L_R_Slope(1) + L_R_Slope(2);
}

/*************************************************************************
*  函数名称：float Slope(int F1x,int F1y,int F2x,int F2y)
*  功能说明：斜率函数
*  参数说明：无
*  函数返回：无
*  修改时间：2019.3.21
*  备    注：△y/△x

*************************************************************************/

float Slope(int F1x, int F1y, int F2x, int F2y)
{
  float slope = 0;
  if (F2y == F1y)
  {
    return 999;
  }
  slope = (F2y - F1y) * 1.0 / (F2x - F1x);
  return slope;
}

/*************************************************************************
*  函数名称：float L_R_Slope(char LR)
*  功能说明：斜率函数
*  参数说明：无
*  函数返回：无
*  修改时间：2019.7.24
*  备    注：LR=1 左边线  LR=2右边线

*************************************************************************/
float L_R_Slope(char LR)
{
  int8 i, j;
  float slope[15] = {0};
  float sum = 0;
  int8 row = 0;
  if (LR == 1)
  {
    for (i = 5; i < 20; i++)
    {
      if (Lef[i] != Fir_row)
      {
        slope[row++] = Slope(Lef[i], i, Lef[i + 10], i + 10);
      }
    }
    for (j = row - 1; j >= 0; j--)
    {
      if (slope[j] != 999)
      {
        sum += slope[j];
      }
      sum /= (row - 1);
      // return sum;
    }
  }
  if (LR == 2)
  {
    for (i = 5; i < 20; i++)
    {
      if (Rig[i] != Fir_row)
      {
        slope[row++] = Slope(Rig[i], i, Rig[i + 10], i + 10);
      }
    }
    for (j = row - 1; j >= 0; j--)
    {
      if (slope[j] != 999)
      {
        sum += slope[j];
      }
      sum /= (row - 1);
      // return sum;
    }
  }
  return sum;
}
/*************************************************************************
*  函数名称：void Allwhite_find()
*  功能说明：全白行查找
*  参数说明：无
*  函数返回：无
*  修改时间：2019.3.23
*  备    注：寻找全白行的开始行和结束行

*************************************************************************/

void Allwhite_find(void)
{
  int Allwhiterow[LCDH]; //全白行，1表示全白，否则为0
  int i;
  // Allwhitestart = Allwhiteend = Allwhitestart2 = Allwhiteend2 = 0;
  Allwhitestart = 0;
  Allwhiteend = 0;

  for (i = Fir_row; i < Last_row - 7; i++)
  {
    Allwhiterow[i] = 0;
    if (Road0_flag == 1 && i < 35)
    {
      if (abs(Lef[i] - Fir_col) < 8 && abs(Last_col - Rig[i]) < 8 &&
          Pixle[i][10] + Pixle[i][14] + Pixle[i][18] + Pixle[i][22] > 2 &&
          Pixle[i][70] + Pixle[i][66] + Pixle[i][62] + Pixle[i][58] > 2 &&
          Pixle[i][34] + Pixle[i][38] + Pixle[i][42] + Pixle[i][46] > 2) //Pixle[i][10] + Pixle[i][20] + Pixle[i][30] + Pixle[i][40] + Pixle[i][50] + Pixle[i][60] + Pixle[i][70] > 5)
      {
        Allwhiterow[i] = 1;
      }
    }
    else if (abs(Lef[i] - Fir_col) < 5 && abs(Last_col - Rig[i]) < 5 &&
             Pixle[i][10] + Pixle[i][14] + Pixle[i][18] + Pixle[i][22] > 2 &&
             Pixle[i][70] + Pixle[i][66] + Pixle[i][62] + Pixle[i][58] > 2 &&
             Pixle[i][34] + Pixle[i][38] + Pixle[i][42] + Pixle[i][46] > 2)
    //Pixle[i][10] + Pixle[i][14] + Pixle[i][18] + Pixle[i][22] > 2 && Pixle[i][70] + Pixle[i][66] + Pixle[i][62] + Pixle[i][58] > 2)
    {
      Allwhiterow[i] = 1;
    }
  }
  Allwhitestart = Last_row;
  i = Last_row - 8;
  while (Allwhiterow[i] != 1 && i > Fir_row)
  {
    i = i - 1;
  }
  Allwhitestart = i;
  Allwhiteend = Fir_row;
  // i = Allwhitestart;
  while (Allwhiterow[i] == 1 && i > Fir_row)
  {
    i = i - 1;
  }
  Allwhiteend = i;
  /*
  for (i = Allwhiteend - 1; i > Fir_row; i--)
  {
    if (Allwhiterow[i] == 1)
    {
      Allwhitestart2 = i;
      break;
    }
  }
  if (Allwhitestart2 != 0)
  {
    for (i = Allwhitestart2; i > Fir_row; i--)
    {
      if (Allwhiterow[i] != 1)
      {
        Allwhiteend2 = i;
        break;
      }
    }
  }
  */
}

/*************************************************************************
*  函数名称：void Fix_line()
*  功能说明：岔路补线处理
*  参数说明：无
*  函数返回：无
*  修改时间：2019.3.6
*  备    注：3.6日：主要补线体现在十字路口偏离中心道路时将左右两边路补线，
             在普通直道与弯道不进行补线（通过平滑与否与十字区分开）

*************************************************************************/

void Pic_Fix_Line(void)
{
  float slope;
  float slope2;
  int i;
  int j;
  int k;
  int xtemp, ytemp, get_flag = 0;
  static float stat_slope;
  static float stat_slope2;
  //static char road1_flag3=1;
  static char road1_flag1 = 1; //0表示已计算完进圆环斜率，1表示已经出圆环，再次进圆环时计算补线斜率
  static char road2_flag1 = 1;
  if (Road == 0)
  {
    if (Road0_flag == 1)
    {
      for (i = 55; i > Fir_row + 15; i--)
      {
        if (abs(Lef[i] - Fir_col) < 5)
          continue;
        slope = Slope(Lef[i], i, Lef[i - 5], i - 5); //Slope(int F1x,int F1y,int F2x,int F2y)
        if (slope != 999)
        {
          for (j = i + 1; j > Fir_row + 5; j--)
          {
            Lef[j] = (int)(Lef[i] - (i - j) / slope);
#ifdef undistort0
            pixel_undistort(Lef[j], j, 0); //x:Lef[i],      y:i   LR:0:左  1：右
#endif
          }
#ifdef undistort1
          Pic_undistort(1, 0);
#endif
          break;
        }
      }
      for (i = 55; i > Fir_row + 15; i--)
      {
        if (abs(Rig[i] - Last_col) < 5)
          continue;
        slope = Slope(Rig[i], i, Rig[i - 5], i - 5); //Slope(int F1x,int F1y,int F2x,int F2y)
        if (slope != 999)
        {
          for (j = i + 1; j > Fir_row + 5; j--)
          {
            Rig[j] = (int)(Rig[i] - (i - j) / slope);
#ifdef undistort0
            pixel_undistort(Rig[j], j, 1); //x:Lef[i],      y:i   LR:0:左  1：右
#endif
          }
#ifdef undistort1
          Pic_undistort(0, 1);
#endif
          break;
        }
      }

      return;
    }
    else if (Road0_flag == 2)
    {
      for (i = Fir_row + 5; i < Allwhiteend; i++)
      {
        if (abs(Lef[i] - Fir_col) < 15)
          continue;
        slope = Slope(Lef[i], i, 4, 54); //Slope(int F1x,int F1y,int F2x,int F2y)
        if (slope != 999)
        {
          for (j = i + 1; j < 55; j++)
          {
            Lef[j] = (int)(Lef[i] - (i - j) / slope);
#ifdef undistort0
            pixel_undistort(Lef[j], j, 0); //x:Lef[i],      y:i   LR:0:左  1：右
#endif
          }
#ifdef undistort1
          Pic_undistort(1, 0); //
#endif
          break;
        }
      }
      for (i = Fir_row + 5; i < Allwhiteend; i++)
      {
        if (abs(Rig[i] - Last_col) < 15)
          continue;
        slope = Slope(Rig[i], i, 75, 54); //Slope(int F1x,int F1y,int F2x,int F2y)
        if (slope != 999)
        {
          for (j = i + 1; j < 55; j++)
          {
            Rig[j] = (int)(Rig[i] - (i - j) / slope);
#ifdef undistort0
            pixel_undistort(Rig[j], j, 1); //x:Lef[i],      y:i   LR:0:左  1：右
#endif
          }
#ifdef undistort1
          Pic_undistort(0, 1); //
#endif
          break;
        }
      }

      return;
    }
    else if (Road0_flag == 4)
    {
      for (i = turn_stop; i >= Fir_row; --i)
      {
        Rig[i] = 78;
#ifdef undistort0
        pixel_undistort(Rig[j], j, 1); //x:Lef[i],      y:i   LR:0:左  1：右
#endif
      }
#ifdef undistort1
      Pic_undistort(0, 1);
#endif
      return;
    }
    else if (Road0_flag == 5)
    {
      for (i = turn_stop; i >= Fir_row; --i)
      {
        Lef[i] = 1;
#ifdef undistort0
        pixel_undistort(Rig[j], j, 1); //x:Lef[i],      y:i   LR:0:左  1：右
#endif
      }
#ifdef undistort1
      Pic_undistort(1, 0);
#endif
      return;
    }
  }
  //左圆环补线处理
  if (Road == 1)
  {
    if (Road1_flag == 2)
    {
      for (i = Last_row - 13; i > Fir_row; i--)
      {
        if (road1_flag1)
        {
          if (abs(Lef[i] - Fir_col) < 25)
            continue;
          stat_slope = Slope(Lef[i], i, 75, 54);
          road1_flag1 = 0;
        }

        if (stat_slope != 999)
        {
          for (k = Fir_row + 3; k < 55; k++)
          {
            Rig[k] = (int)(60 - (57 - k) / stat_slope);
#ifdef undistort0
            pixel_undistort(Rig[k], k, 1);
#endif
          }
#ifdef undistort1
          Pic_undistort(0, 1); //
#endif
          break;
        }
      }

      return;
    }
    else if (Road1_flag == 4)
    {
      road1_flag1 = 1; //表示已经出圆环，再次进圆环时计算补线斜率
      for (i = turn_stop; i >= Fir_row; --i)
      {
        Rig[i] = 78;
#ifdef undistort0
        pixel_undistort(Rig[j], j, 1); //x:Lef[i],      y:i   LR:0:左  1：右
#endif
      }
#ifdef undistort1
      Pic_undistort(0, 1);
#endif
      return;
    }
    else if (Road1_flag == 3)
    {
      for (j = Last_row + 3; j > Fir_row; j--)
      {

        if (abs(Rig[j] - Last_col) < 2)
          continue;
        slope = Slope(Rig[j - 1], j - 1, Rig[j - 6], j - 6);

        if (stat_slope != 999)
        {
          for (k = j + 1; k > Fir_row + 5; k--)
          {
            Rig[k] = (int)(64 - (57 - k) / slope);
#ifdef undistort0
            pixel_undistort(Rig[k], k, 1);
#endif
          }
#ifdef undistort1
          Pic_undistort(0, 1); //
#endif
          break;
        }
      }

      return;
    }
    // else if (Road1_flag == 5)
    // {
    //   ;
    // }
  }
  if (Road == 2)
  {
    if (Road2_flag == 2)
    {
      for (i = Last_row - 13; i > Fir_row; i--)
      {
        if (road2_flag1)
        {
          if (abs(Rig[i] - Last_col) < 25)
            continue;
          stat_slope2 = Slope(Rig[i], i, 4, 54);
          road2_flag1 = 0;
        }

        if (stat_slope2 != 999)
        {
          for (k = Fir_row + 3; k < 55; k++)
          {
            Lef[k] = (int)(15 - (57 - k) / stat_slope2);
#ifdef undistort0
            pixel_undistort(Lef[k], k, 0);
#endif
          }
#ifdef undistort1
          Pic_undistort(1, 0); //
#endif
          break;
        }
      }

      return;
    }
    else if (Road2_flag == 4)
    {
      road2_flag1 = 1;
      for (i = turn_stop; i >= Fir_row; --i)
      {
        Lef[i] = 1;
#ifdef undistort0
        pixel_undistort(Rig[j], j, 1); //x:Lef[i],      y:i   LR:0:左  1：右
#endif
      }
#ifdef undistort1
      Pic_undistort(1, 0);
#endif
      return;
    }
    else if (Road2_flag == 3)
    {
      for (j = Last_row + 3; j > Fir_row; j--)
      {

        if (abs(Lef[j] - Fir_col) < 2)
          continue;
        slope2 = Slope(Lef[j - 1], j - 1, Lef[j - 6], j - 6);

        if (slope2 != 999)
        {
          for (k = j + 1; k > Fir_row + 5; k--)
          {
            Lef[k] = (int)(15 - (57 - k) / slope2);
#ifdef undistort0
            pixel_undistort(Lef[k], k, 0);
#endif
          }
#ifdef undistort1
          Pic_undistort(1, 0); //
#endif
          break;
        }
      }

      return;
    }
    // else if (Road2_flag == 5)
    // {
    //   ;
    // }
  }
  if (Road == 7)
  {
    if (Road7_flag == 0)
    {
      get_flag = 0;
      for (i = Fir_row + 2; i < start_stop_line; i++)
      {
        if (abs(Lef[i] - Fir_col) < 15 || Lef[i] - Lef[i + 2] > 5 || Lef[i] - Lef[i + 2] < 0)
          continue;
        xtemp = Lef[i];
        ytemp = i;
        get_flag = 1;
        break;
      }
      if (get_flag == 1)
      {

        for (i = start_stop_line + 5; i < Last_row; i++)
        {
          if (abs(Lef[i] - Fir_col) < 4 || Lef[i] - Lef[i + 2] > 5 || Lef[i] - Lef[i + 2] < 0)
            continue;

          slope = Slope(Lef[i], i, xtemp, ytemp); //Slope(int F1x,int F1y,int F2x,int F2y)
          if (slope != 999)
          {
            for (j = ytemp; j < 55; j++)
            {
              Lef[j] = (int)(Lef[i] - (i - j) / slope);
#ifdef undistort0
              pixel_undistort(Lef[j], j, 0); //x:Lef[i],      y:i   LR:0:左  1：右
#endif
            }
#ifdef undistort1
            Pic_undistort(1, 0); //
#endif
            break;
          }
        }
      }

      get_flag = 0;
      for (i = Fir_row + 2; i < start_stop_line; i++)
      {
        if (abs(Rig[i] - Last_col) < 15 || Rig[i + 2] - Rig[i] > 5 || Rig[i + 2] - Rig[i] < 0)
          continue;
        if (Rig[i] < 40)
          break;
        xtemp = Rig[i];
        ytemp = i;
        get_flag = 1;
        break;
      }
      if (get_flag == 1)
      {
        for (i = start_stop_line + 5; i < Last_row; i++)
        {
          if (abs(Rig[i] - Fir_col) < 4 || Rig[i] < 40 || Rig[i + 2] - Rig[i] > 5 || Rig[i + 2] - Rig[i] < 0)
            continue;

          slope = Slope(Rig[i], i, xtemp, ytemp); //Slope(int F1x,int F1y,int F2x,int F2y)
          if (slope != 999)
          {
            for (j = ytemp; j < 55; j++)
            {
              Rig[j] = (int)(Rig[i] - (i - j) / slope);
#ifdef undistort0
              pixel_undistort(Rig[j], j, 1); //x:Lef[i],      y:i   LR:0:左  1：右
#endif
            }
#ifdef undistort1
            Pic_undistort(0, 1); //
#endif
            break;
          }
        }
      }

      return;
    }
    else if (Road7_flag == 1)
    {

      // get_flag = 0;
      for (i = Fir_row + 5; i < start_stop_line; i++)
      {
        if (abs(Lef[i] - Fir_col) < 15 || Lef[i] - Lef[i + 2] > 5 || Lef[i] - Lef[i + 2] < 0)
          continue;
        // xtemp = Lef[i];
        // ytemp = i;
        slope = Slope(4, 54, Lef[i], i); //Slope(int F1x,int F1y,int F2x,int F2y)
        if (slope != 999)
        {
          for (j = ytemp; j < 55; j++)
          {
            Lef[j] = (int)(Lef[i] - (i - j) / slope);
#ifdef undistort0
            pixel_undistort(Lef[j], j, 0); //x:Lef[i],      y:i   LR:0:左  1：右
#endif
          }
#ifdef undistort1
          Pic_undistort(1, 0); //
#endif
          break;
        }
        // get_flag = 1;
      }
      // if (get_flag == 1)
      // {

      //   for (; i < start_stop_line; i++)
      //   {
      //     if (abs(Lef[i] - Fir_col) > 15 && Lef[i] - Lef[i + 2] < 5 && Lef[i] - Lef[i + 2] > 0)
      //       continue;

      // slope = Slope(Lef[i-1], i-1, xtemp, ytemp); //Slope(int F1x,int F1y,int F2x,int F2y)
      //     break;
      //   }
      // }

      // get_flag = 0;
      for (i = Fir_row + 5; i < start_stop_line; i++)
      {
        if (abs(Rig[i] - Last_col) < 15 || Rig[i + 2] - Rig[i] > 5 || Rig[i + 2] - Rig[i] < 0)
          continue;
        if (Rig[i] < 40)
          break;
        // xtemp = Rig[i];
        // ytemp = i;
        slope = Slope(75, 54, Rig[i], i); //Slope(int F1x,int F1y,int F2x,int F2y)
        if (slope != 999)
        {
          for (j = ytemp; j < 55; j++)
          {
            Rig[j] = (int)(Rig[i] - (i - j) / slope);
#ifdef undistort0
            pixel_undistort(Rig[j], j, 1); //x:Lef[i],      y:i   LR:0:左  1：右
#endif
          }
#ifdef undistort1
          Pic_undistort(0, 1); //
#endif
          break;
        }
        // get_flag = 1;
      }

      // if (get_flag == 1)
      // {
      //   for (; i < start_stop_line; i++)
      //   {
      //     if (abs(Rig[i] - Fir_col) > 15 && Rig[i] > 40 && Rig[i+2] - Rig[i] < 5 && Rig[i+2] - Rig[i] > 0)
      // continue;

      //     slope = Slope(Rig[i-1], i-1, xtemp, ytemp); //Slope(int F1x,int F1y,int F2x,int F2y)
      //     break;
      //   }
      // }

      return;
    }
  }
  // fangyuejie();
}
void fangyuejie(void)
{
  int i = 0;
  for (i = 0; i < 60; ++i)
  {
    if (Lef[i] < 1)
    {
      Lef[i] = 1;
    }
    if (Rig[i] < 1)
    {
      Rig[i] = 1;
    }
    if (Lef[i] > 78)
    {
      Lef[i] = 78;
    }
    if (Rig[i] > 78)
    {
      Rig[i] = 78;
    }
  }
}
#if 0
/*************************************************************************
*  函数名称：void Pic_DrawLRside(void)
*  功能说明：绘制左右边线线
*  参数说明：返回符号数，正表示应右转，负表示应左转
*  函数返回：中心线前置区域内的均值与预设值的偏差
*  修改时间：2019.2.27 2020.05.31改
*  备    注：寻找底层黑白跳变点，逐层向上搜索每行的跳变点。(向两边搜点、范围搜点两种方法）
             将整幅图的左右5列置黑
             每一行只检测两个跳变点。
             ////然后利用求平均值绘制中心线，单写
*************************************************************************/

void Pic_DrawLRside(void)
{
  int i = 0, j = 0, k = 0;
  int side_search_sw[3] = {2, 5, 10};
  int side_search_end;
  for (i = Fir_row; i < LCDH; i++) //将左右置黑（防止全白下无法找寻跳变点）
  {
    Rig[i] = 78;
    Lef[i] = 1;
    Mid[i] = 39;
    for (j = 0; j < Fir_col; j++)
    {
      Pixle[i][j] = 0;
    }
    for (j = Last_col + 1; j < LCDW; j++)
    {
      Pixle[i][j] = 0;
    }
  }
  //  for(i=0;i<Fir_row;i++)//将上方置黑
  //  {
  //    for(j=0;j<LCDW;j++)
  //    {
  //      Pixle[0][j]=0;
  //    }

  for (j = Middle; j < Last_col + 1; j++) //末行处理
  {
    if (Pixle[Last_row][j] == 1 && Pixle[Last_row][j - 1] == 1 && Pixle[Last_row][j - 2] == 1 && Pixle[Last_row][j - 3] == 1 && Pixle[Last_row][j - 4] == 1 && Pixle[Last_row][j - 5] == 1 && Pixle[Last_row][j - 6] == 1 && Pixle[Last_row][j + 1] == 0 && Pixle[Last_row][j + 2] == 0)
    {
      Rig[Last_row] = j;
      break;
    }
  }
  for (j = Middle; j > Fir_col - 1; j--)
  {
    if (Pixle[Last_row][j] == 1 && Pixle[Last_row][j + 1] == 1 && Pixle[Last_row][j + 2] == 1 && Pixle[Last_row][j + 3] == 1 && Pixle[Last_row][j + 4] == 1 && Pixle[Last_row][j + 5] == 1 && Pixle[Last_row][j + 6] == 1 && Pixle[Last_row][j - 1] == 0 && Pixle[Last_row][j - 2] == 0)
    {
      Lef[Last_row] = j;
      break;
    }
  }
  for (i = Last_row - 1; i > Fir_row - 1; i--) //从底层向上绘线
  {
    Lef_err[i] = 0;
    Rig_err[i] = 0;
    side_search_end = Last_col + 1;
    if (Rig[i + 1] != 78)
    {
      for (k = 0; k < 3; ++k)
      {
        if (Rig_err[i] == 1)
        {
          break;
        }
        for (j = Rig[i + 1] - side_search_sw[k]; j < side_search_end; j++) //向两边搜索下一跳变点
        {
          if (Pixle[i][j] == 1 && Pixle[i][j - 1] == 1 && Pixle[i][j - 2] == 1 && Pixle[i][j - 3] == 1 && Pixle[i][j - 4] == 1 && Pixle[i][j - 5] == 1 && Pixle[i][j - 6] == 1 && Pixle[i][j + 1] == 0 && Pixle[i][j + 2] == 0)
          {
            Rig[i] = j;
            Rig_err[i] = 1;
            break;
          }
        }
        side_search_end = Rig[i + 1] - side_search_sw[k];
      }
    }
    else if (Rig[i + 2] != 78)
    {
      for (k = 1; k < 3; ++k)
      {
        if (Rig_err[i] == 1)
        {
          break;
        }
        for (j = Rig[i + 2] - side_search_sw[k]; j < side_search_end; j++) //向两边搜索下一跳变点
        {
          if (Pixle[i][j] == 1 && Pixle[i][j - 1] == 1 && Pixle[i][j - 2] == 1 && Pixle[i][j - 3] == 1 && Pixle[i][j - 4] == 1 && Pixle[i][j - 5] == 1 && Pixle[i][j - 6] == 1 && Pixle[i][j + 1] == 0 && Pixle[i][j + 2] == 0)
          {
            Rig[i] = j;
            Rig_err[i] = 1;
            break;
          }
        }
        side_search_end = Rig[i + 2] - side_search_sw[k];
      }
    }
    if (Rig_err[i] == 0) //若没有找到跳变点，则放宽范围进行搜索
    {
      Rig_err[i] = 1;
      for (j = Lef[i + 1] + 5; j < side_search_end; j++)
      {
        if (Pixle[i][j] == 1 && Pixle[i][j - 1] == 1 && Pixle[i][j - 2] == 1 && Pixle[i][j - 3] == 1 && Pixle[i][j - 4] == 1 && Pixle[i][j - 5] == 1 && Pixle[i][j - 6] == 1 && Pixle[i][j + 1] == 0 && Pixle[i][j + 2] == 0)
        {
          Rig[i] = j + 1;
          break;
        }
      }
    }
    side_search_end = Fir_col - 1;
    if (Lef[i + 1] != 1)
    {
      for (k = 0; k < 3; ++k)
      {

        if (Lef_err[i] == 1)
        {
          break;
        }
        for (j = Lef[i + 1] + side_search_sw[k]; j > side_search_end; j--)
        {
          if (Pixle[i][j] == 1 && Pixle[i][j + 1] == 1 && Pixle[i][j + 2] == 1 && Pixle[i][j + 3] == 1 && Pixle[i][j + 4] == 1 && Pixle[i][j + 5] == 1 && Pixle[i][j + 6] == 1 && Pixle[i][j - 1] == 0 && Pixle[i][j - 2] == 0) //减小道路边缘的白胶带的影响
          {
            Lef[i] = j;
            Lef_err[i] = 1;
            break;
          }
        }
        side_search_end = Lef[i + 1] + side_search_sw[k];
      }
    }
    else if (Lef[i + 2] != 1)
    {
      for (k = 1; k < 3; ++k)
      {

        if (Lef_err[i] == 1)
        {
          break;
        }
        for (j = Lef[i + 2] + side_search_sw[k]; j > side_search_end; j--)
        {
          if (Pixle[i][j] == 1 && Pixle[i][j + 1] == 1 && Pixle[i][j + 2] == 1 && Pixle[i][j + 3] == 1 && Pixle[i][j + 4] == 1 && Pixle[i][j + 5] == 1 && Pixle[i][j + 6] == 1 && Pixle[i][j - 1] == 0 && Pixle[i][j - 2] == 0) //减小道路边缘的白胶带的影响
          {
            Lef[i] = j;
            Lef_err[i] = 1;
            break;
          }
        }
        side_search_end = Lef[i + 2] + side_search_sw[k];
      }
    }
    if (Lef_err[i] == 0) //若没有找到，则放宽范围搜索
    {
      Lef_err[i] = 1;
      for (j = Rig[i + 1] - 5; j > side_search_end; j--)
      {
        if (Pixle[i][j] == 1 && Pixle[i][j + 1] == 1 && Pixle[i][j + 2] == 1 && Pixle[i][j + 3] == 1 && Pixle[i][j + 4] == 1 && Pixle[i][j + 5] == 1 && Pixle[i][j + 6] == 1 && Pixle[i][j - 1] == 0 && Pixle[i][j - 2] == 0) //减小道路边缘的白胶带的影响
        {
          Lef[i] = j;
          break;
        }
      }
    }
  }
}
#endif
#if 1
/*************************************************************************
*  函数名称：void Pic_DrawLRside(void)
*  功能说明：绘制左右边线线
*  参数说明：返回符号数，正表示应右转，负表示应左转
*  函数返回：中心线前置区域内的均值与预设值的偏差
*  修改时间：2020.06.03
*  备    注：寻找底层黑白跳变点，逐层向上搜索每行的跳变点。(向两边搜点、范围搜点两种方法）
             将整幅图的左右5列置黑
             每一行只检测两个跳变点。
             ////然后利用求平均值绘制中心线，单写
*************************************************************************/

void Pic_DrawLRside(void)
{
  const int Middle = 41;
  int i = 0, j = 0;
  int search_flag1 = 0, search_flag2 = 0;
  int Side_flag;
  for (i = Fir_row; i < LCDH; i++) //将左右置黑（防止全白下无法找寻跳变点）
  {
    Rig[i] = 78;
    Lef[i] = 1;
    for (j = 0; j < Fir_col; j++)
    {
      Pixle[i][j] = 0;
    }
    for (j = Last_col + 1; j < LCDW; j++)
    {
      Pixle[i][j] = 0;
    }
  }
  //  for(i=0;i<Fir_row;i++)//将上方置黑
  //  {
  //    for(j=0;j<LCDW;j++)
  //    {
  //      Pixle[0][j]=0;
  //    }
  for (i = Last_row; i > Last_row - 5; i--)
  {
    for (j = Middle; j < Last_col; j++) //末行处理
    {
      if (Pixle[i][j] == 1 && Pixle[i][j - 1] == 1 && Pixle[i][j - 2] == 1 && Pixle[i][j - 3] == 1 && Pixle[i][j - 4] == 1 && Pixle[i][j - 5] == 1 && Pixle[i][j - 6] == 1 && Pixle[i][j + 1] == 0 && Pixle[i][j + 2] == 0 && Pixle[i][j + 3] == 0)
      {
        Rig[i] = j;
        break;
      }
    }
    for (j = Middle; j > Fir_col; j--)
    {
      if (Pixle[i][j] == 1 && Pixle[i][j + 1] == 1 && Pixle[i][j + 2] == 1 && Pixle[i][j + 3] == 1 && Pixle[i][j + 4] == 1 && Pixle[i][j + 5] == 1 && Pixle[i][j + 6] == 1 && Pixle[i][j - 1] == 0 && Pixle[i][j - 2] == 0 && Pixle[i][j - 3] == 0)
      {
        Lef[i] = j;
        break;
      }
    }
    if (Rig[i] != 78 && Lef[i] != 1)
    {
      break;
    }
  }
  for (--i; i > Fir_row - 1; i--) //从底层向上绘线
  {
    search_flag1 = 0;
    search_flag2 = 0;
    Side_flag = 0;

    if (Rig[i + 1] != 78)
    {
      if (Pixle[i][Rig[i + 1]] == 0 || (Pixle[i][Rig[i + 1]] == 1 && Pixle[i][Rig[i + 1] + 1] == 0)) //向内查找10个
      {
        for (j = Rig[i + 1]; j > Rig[i + 1] - 10 && j > Lef[i + 1] + 5; j--)
        {
          if (Pixle[i][j] == 1 && Pixle[i][j - 1] == 1) //两白
          {
            Rig[i] = j;
            Side_flag = 1;
            break;
          }
        }
        search_flag1 = 1;
      }
      else if (Pixle[i][Rig[i + 1] + 1] == 1) //向外查找8个
      {
        for (j = Rig[i + 1] + 1; j < Rig[i + 1] + 8 && j < Last_col + 1; j++)
        {
          if (Pixle[i][j + 1] == 0 && Pixle[i][j + 2] == 0) //两黑
          {
            Rig[i] = j;
            Side_flag = 1;
            break;
          }
        }
        search_flag2 = 1;
      }
    }
    else if (Rig[i + 2] != 78) //更严格的条件
    {
      for (j = Rig[i + 2]; j > Rig[i + 1] - 10 && j > Lef[i + 1] + 5; j--) //先搜内10个
      {
        if (Pixle[i][j] == 1 && Pixle[i][j - 1] == 1 && Pixle[i][j - 2] == 1 && Pixle[i][j - 3] == 1)
        {
          Rig[i] = j;
          Side_flag = 1;
          break;
        }
        search_flag1 = 1;
      }
      if (Side_flag == 0 && Pixle[i][Rig[i + 2] - 2] == 1 && Pixle[i][Rig[i + 2] - 1] == 1 && Pixle[i][Rig[i + 2]] == 1 && Pixle[i][Rig[i + 2] + 1] == 1) //搜外8个
      {
        for (j = Rig[i + 2] + 1; j < Rig[i + 2] + 8 && j < Last_col + 1; j++)
        {
          if (Pixle[i][j + 1] == 0 && Pixle[i][j + 2] == 0 && Pixle[i][j + 3] == 0)
          {
            Rig[i] = j;
            Side_flag = 1;
            break;
          }
        }
        search_flag2 = 1;
      }
    }

    if (Side_flag == 0) //若没有找到跳变点，则放宽范围进行搜索
    {
      Side_flag = 1;
      for (j = Lef[i + 1] + 5; j <= Rig[i + 1] - 10; j++)
      {
        if (Pixle[i][j] == 1 && Pixle[i][j - 1] == 1 && Pixle[i][j - 2] == 1 && Pixle[i][j - 3] == 1 && Pixle[i][j - 4] == 1 && Pixle[i][j - 5] == 1 && Pixle[i][j - 6] == 1 && Pixle[i][j + 1] == 0 && Pixle[i][j + 2] == 0)
        {
          Rig[i] = j;
          break;
        }
      }
      if (search_flag1 == 0)
      {
        for (j = Rig[i + 1] - 9; j < Rig[i + 1]; j++)
        {
          if (Pixle[i][j] == 1 && Pixle[i][j - 1] == 1 && Pixle[i][j - 2] == 1 && Pixle[i][j - 3] == 1 && Pixle[i][j - 4] == 1 && Pixle[i][j - 5] == 1 && Pixle[i][j - 6] == 1 && Pixle[i][j + 1] == 0 && Pixle[i][j + 2] == 0)
          {
            Rig[i] = j;
            break;
          }
        }
      }
      if (search_flag2 == 0)
      {
        for (j = Rig[i + 1]; j < Rig[i + 1] + 8; j++)
        {
          if (Pixle[i][j] == 1 && Pixle[i][j - 1] == 1 && Pixle[i][j - 2] == 1 && Pixle[i][j - 3] == 1 && Pixle[i][j - 4] == 1 && Pixle[i][j - 5] == 1 && Pixle[i][j - 6] == 1 && Pixle[i][j + 1] == 0 && Pixle[i][j + 2] == 0)
          {
            Rig[i] = j;
            break;
          }
        }
      }
      for (j = Rig[i + 1] + 8; j <= Last_col; j++)
      {
        if (Pixle[i][j] == 1 && Pixle[i][j - 1] == 1 && Pixle[i][j - 2] == 1 && Pixle[i][j - 3] == 1 && Pixle[i][j - 4] == 1 && Pixle[i][j - 5] == 1 && Pixle[i][j - 6] == 1 && Pixle[i][j + 1] == 0 && Pixle[i][j + 2] == 0)
        {
          Rig[i] = j;
          break;
        }
      }
    }
    search_flag1 = 0;
    search_flag2 = 0;
    Side_flag = 0;

    if (Lef[i + 1] != 1)
    {
      if (Pixle[i][Lef[i + 1]] == 0 || (Pixle[i][Lef[i + 1]] == 1 && Pixle[i][Lef[i + 1] - 1] == 0))
      {
        for (j = Lef[i + 1]; j < Lef[i + 1] + 10 && j < Rig[i + 1] - 5; j++)
        {
          if (Pixle[i][j] == 1 && Pixle[i][j + 1] == 1)
          {
            Lef[i] = j;
            Side_flag = 1;
            break;
          }
        }
        search_flag1 = 1;
      }
      else if (Pixle[i][Lef[i + 1] - 1] == 1)
      {
        for (j = Lef[i + 1] - 1; j > Lef[i + 1] - 8 && j > Fir_col - 1; j--)
        {
          if (Pixle[i][j - 1] == 0 && Pixle[i][j - 2] == 0)
          {
            Lef[i] = j;
            Side_flag = 1;
            break;
          }
        }
        search_flag2 = 1;
      }
    }
    else if (Lef[i + 2] != 1)
    {
      for (j = Lef[i + 2]; j < Lef[i + 1] + 10 && j < Rig[i + 1] + 5; j++)
      {
        if (Pixle[i][j] == 1 && Pixle[i][j + 1] == 1 && Pixle[i][j + 2] == 1 && Pixle[i][j + 3] == 1)
        {
          Lef[i] = j;
          Side_flag = 1;
          break;
        }
        search_flag1 = 1;
      }
      if (Side_flag == 0 && Pixle[i][Lef[i + 2] + 2] == 1 && Pixle[i][Lef[i + 2] + 1] == 1 && Pixle[i][Lef[i + 2]] == 1 && Pixle[i][Lef[i + 2] - 1] == 1)
      {
        for (j = Lef[i + 2] - 1; j > Lef[i + 2] - 8 && j > Fir_col - 1; j--)
        {
          if (Pixle[i][j - 1] == 0 && Pixle[i][j - 2] == 0 && Pixle[i][j - 3] == 0)
          {
            Lef[i] = j;
            Side_flag = 1;
            break;
          }
        }
        search_flag2 = 1;
      }
    }

    if (Side_flag == 0) //若没有找到跳变点，则放宽范围进行搜索
    {
      Side_flag = 1;
      for (j = Rig[i + 1] - 5; j >= Lef[i + 1] + 10; j--)
      {
        if (Pixle[i][j] == 1 && Pixle[i][j + 1] == 1 && Pixle[i][j + 2] == 1 && Pixle[i][j + 3] == 1 && Pixle[i][j + 4] == 1 && Pixle[i][j + 5] == 1 && Pixle[i][j + 6] == 1 && Pixle[i][j - 1] == 0 && Pixle[i][j - 2] == 0)
        {
          Lef[i] = j;
          break;
        }
      }
      if (search_flag1 == 0)
      {
        for (j = Lef[i + 1] + 9; j > Lef[i + 1]; j--)
        {
          if (Pixle[i][j] == 1 && Pixle[i][j + 1] == 1 && Pixle[i][j + 2] == 1 && Pixle[i][j + 3] == 1 && Pixle[i][j + 4] == 1 && Pixle[i][j + 5] == 1 && Pixle[i][j + 6] == 1 && Pixle[i][j - 1] == 0 && Pixle[i][j - 2] == 0)
          {
            Lef[i] = j;
            break;
          }
        }
      }
      if (search_flag2 == 0)
      {
        for (j = Lef[i + 1]; j > Lef[i + 1] - 8; j--)
        {
          if (Pixle[i][j] == 1 && Pixle[i][j + 1] == 1 && Pixle[i][j + 2] == 1 && Pixle[i][j + 3] == 1 && Pixle[i][j + 4] == 1 && Pixle[i][j + 5] == 1 && Pixle[i][j + 6] == 1 && Pixle[i][j - 1] == 0 && Pixle[i][j - 2] == 0)
          {
            Lef[i] = j;
            break;
          }
        }
      }
      for (j = Lef[i + 1] - 8; j > Fir_col - 1; j--)
      {
        if (Pixle[i][j] == 1 && Pixle[i][j + 1] == 1 && Pixle[i][j + 2] == 1 && Pixle[i][j + 3] == 1 && Pixle[i][j + 4] == 1 && Pixle[i][j + 5] == 1 && Pixle[i][j + 6] == 1 && Pixle[i][j - 1] == 0 && Pixle[i][j - 2] == 0)
        {
          Lef[i] = j;
          break;
        }
      }
    }
  }
}
#endif
/*************************************************************************
*  函数名称：void Pic_DrawMid(void)
*  功能说明：绘制左右边线线
*  参数说明：返回符号数，正表示应右转，负表示应左转
*  函数返回：中心线前置区域内的均值与预设值的偏差
*  修改时间：2020.05.31
*  备    注：寻找底层黑白跳变点，逐层向上搜索每行的跳变点。(向两边搜点、范围搜点两种方法）
             将整幅图的左右5列置黑
             每一行只检测两个跳变点。
             ////然后利用求平均值绘制中心线，单写
*************************************************************************/

void Pic_DrawMid(void)
{
  int i = 0;
  int road_half_width_original[40] = {40, 40, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4};
  if ((Road0_flag == 3 && Road == 0) || Road == 1)
  {
    for (i = Fir_row; i < Last_row + 1; i++)
    {

      if (Rig[i] < Last_col)
      {
        Mid[i] = Rig[i] - road_half_width_original[Last_row - i];
      }
      else
      {
        Mid[i] = 40;
      }
    }
  }
  else if ((Road0_flag == 4 && Road == 0) || Road == 2)
  {
    for (i = Fir_row; i < Last_row + 1; i++)
    {

      if (Lef[i] > Fir_col)
      {
        Mid[i] = Lef[i] + road_half_width_original[Last_row - i];
      }
      else
      {
        Mid[i] = 40;
      }
    }
  }
  else
  {
    for (i = Fir_row; i < Last_row + 1; i++)
    {

      if (Lef[i] > Fir_col && Rig[i] < Last_col) //Mid Calculaing
      {
        Mid[i] = (int)((Lef[i] + Rig[i]) / 2.0 + 0.5);
      }
      else if (Lef[i] <= Fir_col && Rig[i] < Last_col)
      {
        Mid[i] = Rig[i] - road_half_width_original[Last_row - i];
      }
      else if (Lef[i] > Fir_col && Rig[i] >= Last_col)
      {
        Mid[i] = Lef[i] + road_half_width_original[Last_row - i];
      }
      else
      {
        Mid[i] = 40;
      }
    }
  }
  return;
}
/*************************************************************************
 *  函数名称：void Pic_DrawMid_und(void)
 *  功能说明：计算去畸变后中线无插值
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020.5.31
 *  备    注：
 * ************************************************************************/

void Pic_DrawMid_und(void)
{
  int i;

  if ((Road0_flag == 3 && Road == 0) || Road == 1)
  {
    for (i = 0; i < 60; i++)
    {

      if (New_Rig[i] != MIDMAP)
      {
        New_Mid[i] = New_Rig[i] - ROAD_HALF_WIDTH;
      }
      else
      {
        New_Mid[i] = 999;
      }
    }
  }
  else if ((Road0_flag == 4 && Road == 0) || Road == 2)
  {
    for (i = 0; i < 60; i++)
    {

      if (New_Lef[i] != MIDMAP)
      {
        New_Mid[i] = New_Lef[i] + ROAD_HALF_WIDTH;
      }
      else
      {
        New_Mid[i] = 999;
      }
    }
  }
  else
  {
    for (i = 0; i < 60; i++)
    {

      if (New_Lef[i] != -MIDMAP && New_Rig[i] != MIDMAP) //Mid Calculaing
      {
        New_Mid[i] = (int)((New_Lef[i] + New_Rig[i]) / 2.0 + 0.5);
      }
      else if (New_Lef[i] == -MIDMAP && New_Rig[i] != MIDMAP)
      {
        New_Mid[i] = New_Rig[i] - ROAD_HALF_WIDTH;
      }
      else if (New_Lef[i] != -MIDMAP && New_Rig[i] == MIDMAP)
      {
        New_Mid[i] = New_Lef[i] + ROAD_HALF_WIDTH;
      }
      else
      {
        New_Mid[i] = 999;
      }
    }
  }
  return;
}
/*************************************************************************
*  函数名称：void Pic_offset_fig(void)
*  功能说明：计算offset
*  参数说明：无
*  函数返回：无
*  修改时间：2020.5.31_GMY
*  备    注：Cam_offset为负，车偏右，向左拐；
             Cam_offset为正，车偏左，向右拐；

*************************************************************************/
void Pic_offset_fig(void)
{
  int i;
  int count = 0; // count2 = 0;

  for (i = FIG_AREA_FAR; i < FIG_AREA_NEAR; ++i)
  {
    if (New_Mid[i] != 999)
    {
      Cam_offset = Cam_offset + New_Mid[i];
      count++;
    }
  }
  if (count != 0)
  {
    Cam_offset = Cam_offset / count;
  }

  if ((Road0_flag == 1 || Road0_flag == 2) && Road == 0)
  {
    Cam_offset *= 0.3;
  }

  else if (Road1_flag == 3 || Road2_flag == 3)
    Cam_offset *= 0.8;

  // if (Road == 3 || Road1_flag == 2 || Road2_flag == 2)
  // {
  //   Cam_offset *= 1;
  // }
  return;
}

#if 0 //整理代替
/*************************************************************************
*  函数名称：void Pic_offset_fig(void)
*  功能说明：计算offset
*  参数说明：无
*  函数返回：无
*  修改时间：2019.3.21
*  备    注：Cam_offset为负，车偏右，向左拐；
             Cam_offset为正，车偏左，向右拐；

*************************************************************************/

void Pic_offset_fig(void)
{
  int i;
  int count = 0; // count2 = 0;

  for (i = 0; i < 60; i++)
  {
    
    //  if(Road==1&&(Road1_flag==0||Road1_flag==2))
    //  {
    //      if(New_Rig[i]!=MIDMAP)
    //      {
    //      Cam_offset=Cam_offset+New_Rig[i]-ROAD_HALF_WIDTH;
    //      count++;
    //      }
    //      continue;
    //  }
    if (Road == 1 && (Road1_flag == 3 || Road1_flag == 1))
    {
      if (New_Rig[i] != MIDMAP)
      {
        Cam_offset = Cam_offset + New_Rig[i] - ROAD_HALF_WIDTH;
        count++;
      }
      continue;
    }
    else if (Road == 2 && (Road2_flag == 3 || Road2_flag == 1))
    {
      if (New_Lef[i] != -MIDMAP)
      {
        Cam_offset = Cam_offset + New_Lef[i] + ROAD_HALF_WIDTH;
        count++;
      }
      continue;
    }

    else if (i <= FIG_AREA_NEAR && i >= FIG_AREA_FAR)
    {
      if (New_Lef[i] != -MIDMAP && New_Rig[i] != MIDMAP)
      {
        Cam_offset = Cam_offset + New_Lef[i] + New_Rig[i];
        count++;
        continue;
      }
      else if (New_Lef[i] == -MIDMAP && New_Rig[i] != MIDMAP)
      {
        Cam_offset = Cam_offset + New_Rig[i] - ROAD_HALF_WIDTH;
        count++;
        continue;
      }
      else if (New_Lef[i] != -MIDMAP && New_Rig[i] == MIDMAP)
      {
        Cam_offset = Cam_offset + New_Lef[i] + ROAD_HALF_WIDTH;
        count++;
        continue;
      }
      else
      {
        continue;
      }
    }
    /*
    else if ((i <= FIG_AREA_FAR && i >= FIG_AREA_FAR2))
    {
      if (New_Lef[i] != -MIDMAP && New_Rig[i] != MIDMAP)
      {
        Cam_offset2 = Cam_offset2 + New_Lef[i] + New_Rig[i];
        count2++;
        continue;
      }
      else if (New_Lef[i] == -MIDMAP && New_Rig[i] != MIDMAP)
      {
        Cam_offset2 = Cam_offset2 + New_Rig[i] - ROAD_HALF_WIDTH;
        count2++;
        continue;
      }
      else if (New_Lef[i] != -MIDMAP && New_Rig[i] == MIDMAP)
      {
        Cam_offset2 = Cam_offset2 + New_Lef[i] + ROAD_HALF_WIDTH;
        count2++;
        continue;
      }
      else
      {
        continue;
      }
    }
    */
  }
  if ((Road0_flag==1 || Road0_flag==2) && Road == 0)
  {
    Cam_offset *= 0.3;
  }

  if (Road1_flag == 3 || Road2_flag == 3)
    Cam_offset *= 0.8;

  if (count != 0)
  {
    Cam_offset = Cam_offset / count;
  }
  /*
  if (count2 != 0)
  {
    Cam_offset2 = Cam_offset2 / count2;
  }
*/
  //Cam_offset = (0.4 * Cam_offset + 0.6 * Cam_offset2);
  if (Road == 3 || Road1_flag == 2 || Road2_flag == 2)
  {
    Cam_offset *= 1;
  }
  return;
}

#endif
/*************************************************************************
*  函数名称：void Pic_offset_filter(void)
*  功能说明：分段计算道路偏差值及实现滤波
*  参数说明：无
*  函数返回：无
*  修改时间：2019.3.6
*  备    注：

*************************************************************************/

void Pic_offset_filter(void)
{
  static float Cam_offset_filter[4] = {0, 0, 0, 0}; //offset滤波数组
  Cam_offset_filter[3] = Cam_offset_filter[2];
  Cam_offset_filter[2] = Cam_offset_filter[1];
  Cam_offset_filter[1] = Cam_offset_filter[0];
  Cam_offset_filter[0] = Cam_offset;
  Cam_offset = Cam_offset_filter[0] * 0.5 + Cam_offset_filter[1] * 0.2 + Cam_offset_filter[2] * 0.2 + Cam_offset_filter[3] * 0.1;
  //Cam_offset=Cam_offset_filter[0]*0.45+Cam_offset_filter[1]*0.15+Cam_offset_filter[2]*0.15+Cam_offset_filter[3]*0.05+Cam_offset2*0.2;
}

// measured remapping of y
//int map_y[60]={
// 235,223,212,201,191,181,172,163,154,146,138,130,123,116,109,103,97,92,86,81,76,72,67,63,59,55,52,48,45,42,39,37,34,32,29,27,25,23,21,20,18,17,15,14,13,11,10,9,8,7,6,5,5,4,3,3,2,1,1,0
//};

int temx;
// int mid_map[60];

#ifdef undistort0

//my analitical version of remapping x and y
void pixel_undistort(int x, int y, int LR)
{
  int K0; //变比
  temx = x;
  if (temx == 2)
    temx = 0;
  if (temx == 78)
    temx = 79;
  if (y > 55)
  {
    K0 = (int)K0_Table[y - 56];
  }
  else
  {
    K0 = MAP_K0 / (y + MAP_B);
  }

  if (LR == 0)
  {
    New_Lef[y] = (int)((temx - 39) * K0); //offset_map[y]);
    if (New_Lef[y] < -MIDMAP)
      New_Lef[y] = -MIDMAP;
  }
  else
  {
    New_Rig[y] = (int)((temx - 39) * K0); //offset_map[y]);
    if (New_Rig[y] > MIDMAP)
      New_Rig[y] = MIDMAP;
  }
}

/*************************************************************************
*  函数名称：void Pic_undistort(void)
*  功能说明：图像去畸变
*  参数说明：无
*  函数返回：无
*  修改时间：2019.3.21
*  备    注：对Lef、Rig进行映射处理

*************************************************************************/
void Pic_undistort(void)
{
  int i;
  for (i = 0; i < 60; i++)
  {
    New_Lef[i] = -MIDMAP;
    New_Rig[i] = MIDMAP;
  }

  for (i = 0; i <= 55; i++)
  {

    if (Lef[i] <= Fir_col)
    {
      New_Lef[i] = -MIDMAP; //丢失左边线
    }
    else
    {
      pixel_undistort(Lef[i], i, 0); //change to analitical version
    }
    if (Rig[i] >= Last_col - 1)
    {
      New_Rig[i] = MIDMAP; //丢失右边线
    }
    else
    {
      pixel_undistort(Rig[i], i, 1); //change to analitical version
    }
  }
}

#endif
#ifdef undistort1
/*************************************************************************
 *  函数名称：void Pic_undistort(int L, int R)
 *  功能说明：图像去畸变
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020.05.30
 *  备    注：对Lef、Rig进行映射处理
 * **********************************************************************/
void Pic_undistort(int L, int R)
{
  int i = 0;
  int j = 0;
  int const startpoint = 29;
  int tempy;
  int tempx;
  int tempNewxR[60];
  int tempNewxL[60];
  float temp;
  int step;
  int Rig_New[60];
  int Lef_New[60];
  static const int tempNewy[60] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 261, 217, 184, 158, 138, 121, 107, 96, 86, 77, 70, 63, 57, 52, 47, 43, 39, 35, 32, 29, 26, 24, 21, 19, 17, 15, 13, 12, 10, 8, 7, 6, 4, 3, 2, 1, 0, 0};

  /*************************映射*******************************/
  if (R)
  {
    for (i = startpoint; i < 59; i++)
    {
      tempy = i;
      tempx = Rig[i];
      if (tempx >= Last_col)
        tempNewxR[i] = MIDMAP;
      else
        tempNewxR[i] = (int)(UNDISTORT_XPK * (UNDISTORT_D * UNDISTORT_C + UNDISTORT_H) * 2 * (tempx - 39.5) / (-UNDISTORT_S * (29.5 - tempy) + UNDISTORT_D * UNDISTORT_C) + 0.5);

      if (tempNewxR[i] > MIDMAP)
        tempNewxR[i] = MIDMAP;
    }
  }
  if (L)
  {
    for (i = startpoint; i < 59; i++)
    {
      tempy = i;
      tempx = Lef[i];
      if (tempx <= Fir_col)
        tempNewxL[i] = -MIDMAP;
      else
        tempNewxL[i] = (int)(UNDISTORT_XPK * (UNDISTORT_D * UNDISTORT_C + UNDISTORT_H) * 2 * (tempx - 39.5) / (-UNDISTORT_S * (29.5 - tempy) + UNDISTORT_D * UNDISTORT_C) + 0.5);

      if (tempNewxL[i] < -MIDMAP)
        tempNewxL[i] = -MIDMAP;
    }
  }
  /************************插值+压缩+倒序*************************/
  i = startpoint;
  j = 59; //59，不补最远行，58，补最远行需+下面两行代码
  // Rig_New[0] = tempNewxR[0];
  // Lef_New[0] = tempNewxL[0];
  while (j >= 0)
  {
    temp = UNDISTORT_PYK * j;
    if (tempNewy[i] >= temp && tempNewy[i + 1] <= temp)
    {
      step = tempNewy[i] - tempNewy[i + 1];
      if (R)
      {
        if (tempNewxR[i] != MIDMAP && tempNewxR[i + 1] != MIDMAP)
        {
          Rig_New[59 - j] = (int)((tempNewxR[i] - tempNewxR[i + 1]) * (temp - tempNewy[i + 1]) / step + tempNewxR[i + 1] + 0.5);
        }
        else
        {
          Rig_New[59 - j] = MIDMAP;
        }
      }
      if (L)
      {
        if (tempNewxL[i] != -MIDMAP && tempNewxL[i + 1] != -MIDMAP)
        {
          Lef_New[59 - j] = (int)((tempNewxL[i] - tempNewxL[i + 1]) * (temp - tempNewy[i + 1]) / step + tempNewxL[i + 1] + 0.5);
        }
        else
        {
          Lef_New[59 - j] = -MIDMAP;
        }
      }
      j--;
    }
    else
    {
      i++;
    }
  }
  /************************滤波*************************/

  if (L)
  {

    for (i = 0; i < 60; i++)
    {
      New_Lef[i] = Lef_New[i];
    }
    i = 0;
    while (i < 57)
    {
      if (Lef_New[i + 2] == -MIDMAP)
      {
        i = i + 3;
        continue;
      }
      else if (Lef_New[i + 1] == -MIDMAP)
      {
        i = i + 2;
        continue;
      }
      else if (Lef_New[i] == -MIDMAP)
      {
        i = i + 1;
        continue;
      }

      New_Lef[i + 1] = (Lef_New[i] + Lef_New[i + 1] + Lef_New[i + 2]) / 3;
      if (Lef_New[i + 3] == -MIDMAP)
      {
        i = i + 4;
        continue;
      }

      while (i < 57 && Lef_New[i + 3] != -MIDMAP)
      {

        New_Lef[i + 2] = (Lef_New[i] + Lef_New[i + 1] + Lef_New[i + 2] + Lef_New[i + 3]) / 4;
        i = i + 1;
      }
      i = i + 4;
    }
  }
  if (R)
  {
    for (i = 0; i < 60; i++)
    {
      New_Rig[i] = Rig_New[i];
    }
    i = 0;
    while (i < 57)
    {
      if (Rig_New[i + 2] == MIDMAP)
      {
        i = i + 3;
        continue;
      }
      else if (Rig_New[i + 1] == MIDMAP)
      {
        i = i + 2;
        continue;
      }
      else if (Rig_New[i] == MIDMAP)
      {
        i = i + 1;
        continue;
      }

      New_Rig[i + 1] = (Rig_New[i] + Rig_New[i + 1] + Rig_New[i + 2]) / 3;
      if (Rig_New[i + 3] == MIDMAP)
      {
        i = i + 4;
        continue;
      }

      while (i < 57 && Rig_New[i + 3] != MIDMAP)
      {

        New_Rig[i + 2] = (Rig_New[i] + Rig_New[i + 1] + Rig_New[i + 2] + Rig_New[i + 3]) / 4;
        i = i + 1;
      }
      i = i + 4;
    }
  }
}

#endif

/*************************************************************************
*  函数名称：void Pic_find_circle(void)
*  功能说明：图像找环状
*  参数说明：无
*  函数返回：无
*  修改时间：2019.3.21
*  备    注：环状定义：如右边线列数从有效行开始先减小后变大。

*************************************************************************/

void Pic_find_circle(void)
{
  int i;
  Lef_circle = 0;
  Rig_circle = 0;
  Lef_break_point = 0;
  Rig_break_point = 0;
  for (i = 55; i > Fir_row + 12; i--) //从非全白行开始寻找
  {
    if (Last_col - Rig[i] < 2) //从右边线离开右边界开始寻找
    {
      continue;
    }
    /*if(Rig[i-2]<=Rig[i] && Rig[i-4]<=Rig[i-2] && Rig[i-7]>=Rig[i-5] &&
       Rig[i-9]>=Rig[i-7] && Rig[i-11]>=Rig[i-9]&& Rig[i-8]!=Rig[i-2] &&
       abs(Rig[i-8]-Rig[i-6])<5 && abs(Rig[i-6]-Rig[i-4])<5 && abs(Rig[i-4]-Rig[i-2])<5 && abs(Rig[i-2]-Rig[i])<5 && Rig[i-5]<=67)
    {
      Rig_circle=1;
      Rig_break_point=i-5;
      break;
    }
    */

    if (Rig[i - 2] <= Rig[i] && Rig[i - 4] <= Rig[i - 2] && Rig[i - 6] <= Rig[i - 4] && Rig[i - 8] <= Rig[i - 6] && Rig[i - 10] <= Rig[i - 8] &&
        Rig[i - 12] > Rig[i - 10] && Rig[i - 11] > Rig[i - 9] && Rig[i - 13] > Rig[i - 11] && Rig[i - 14] > Rig[i - 12] && Rig[i - 15] > Rig[i - 13] &&
        abs(Rig[i - 10] - Rig[i - 8]) < 4 && abs(Rig[i - 8] - Rig[i - 6]) < 4 && abs(Rig[i - 6] - Rig[i - 4]) < 4 && abs(Rig[i - 4] - Rig[i - 2]) < 4 && abs(Rig[i - 2] - Rig[i]) < 4 &&
        abs(Rig[i - 12] - Rig[i - 10]) < 5 && abs(Rig[i - 11] - Rig[i - 9]) < 5 && abs(Rig[i - 13] > Rig[i - 11]) < 5 && abs(Rig[i - 14] > Rig[i - 12]) < 8 && abs(Rig[i - 15] > Rig[i - 13]) < 8)
    {
      Rig_circle = 1;
      Rig_break_point = i;
      break;
    }
  }
  for (i = 55; i > Fir_row + 12; i--)
  {
    if (Lef[i] - Fir_col < 2)
    {
      continue;
    }
    /*
    if(Lef[i-2]>=New_Lef[i] && Lef[i-4]>=Lef[i-2] && Lef[i-7]>=New_Lef[i-5] && //Lef[i-8]<=Lef[i-6] &&
       Lef[i-9]<=Lef[i-7] && Lef[i-11]<=Lef[i-19] && Lef[i-8]!=Lef[i-2] &&
         abs(Lef[i-8]-Lef[i-6])<5 && abs(Lef[i-6]-Lef[i-4])<5 && abs(Lef[i-4]-Lef[i-2])<5 && abs(Lef[i-2]-New_Lef[i])<5)
    {
      Lef_circle=1;
      Lef_break_point=i-5;
      break;t
    }
    */
    if (Lef[i - 2] >= Lef[i] && Lef[i - 4] >= Lef[i - 2] && Lef[i - 6] >= Lef[i - 4] && Lef[i - 8] >= Lef[i - 6] && Lef[i - 10] >= Lef[i - 8] &&
        Lef[i - 12] < Lef[i - 10] && Lef[i - 11] < Lef[i - 9] && Lef[i - 13] < Lef[i - 11] && Lef[i - 14] < Lef[i - 12] && Lef[i - 15] < Lef[i - 13] &&
        abs(Lef[i - 10] - Lef[i - 8]) < 4 && abs(Lef[i - 8] - Lef[i - 6]) < 4 && abs(Lef[i - 6] - Lef[i - 4]) < 4 && abs(Lef[i - 4] - Lef[i - 2]) < 4 && abs(Lef[i - 2] - Lef[i]) < 4 &&
        abs(Lef[i - 12] - Lef[i - 10]) < 5 && abs(Lef[i - 11] - Lef[i - 9]) < 5 && abs(Lef[i - 13] - Lef[i - 11]) < 5 && abs(Lef[i - 14] - Lef[i - 12]) < 8 && abs(Lef[i - 15] - Lef[i - 13]) < 8)
    {
      Lef_circle = 1;
      Lef_break_point = i;
      break;
    }
  }
}

/*************************************************************************
*  函数名称：void start_stop_rec(void)
*  功能说明：起跑线识别与等待
*  参数说明：无
*  函数返回：无
*  修改时间：2020.05.31
*  备    注：
*************************************************************************/
void start_stop_rec(void)
{
  static int start_waited = 0;
  static int turn_cnt = 0, stop_cnt = 0;
  static int stop_line = Fir_row;
  float stop_flag_cnt = 0;
  int stop_line_cnt = 0, stop_black_cnt = 0, stop_all_cnt = 0, count = 0;
  int j;
  int avr_mid = 0;
  int Black_line = 0;
  int road_half_width_original[40] = {40, 40, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4};

  if (Road == 0 && Road0_flag == 0) //进起跑线
  {
    start_waited = 601; //要改回去，否则无法出库
    if (start_waited > 600 && Lef_edge > 10)
    {
      start_waited = 601;
      for (int i = Fir_row + 10; i < Last_row; i++) //自上而下寻找有边线的开始行
      {
        if (Lef[i] > Fir_col + 5)
        {
          stop_line_cnt = 0;
          continue;
        }
        else
        {
          stop_line_cnt++;
          if (stop_line_cnt >= 3)
          {
            start_stop_line = i - 2;
            break;
          }
        }
      }

      if (start_stop_line != 0)
      {
        count = 5;
        for (int i = start_stop_line - 5; i > start_stop_line - 10; --i) //计算平均中间位置
        {
          if (Lef[i] > Fir_col + 5 && Rig[i] < Last_col - 5)
          {
            avr_mid += (int)((Lef[i] + Rig[i]) / 2.0 + 0.5);
          }
          else if (Lef[i] <= Fir_col + 5 && Rig[i] < Last_col - 5)
          {
            avr_mid += Rig[i] - road_half_width_original[Last_row - i];
          }
          else if (Lef[i] > Fir_col + 5 && Rig[i] >= Last_col - 5)
          {
            avr_mid += Lef[i] + road_half_width_original[Last_row - i];
          }
          else
          {
            count--;
            continue;
          }
        }
        if (count == 0)
        {
          start_stop_line = 0;
          return;
        }
        avr_mid /= count;
        for (int i = start_stop_line; i <= Last_row; ++i) //搜黑白线
        {
          for (j = avr_mid - road_half_width_original[Last_row - i]; j <= avr_mid + road_half_width_original[Last_row - i]; ++j)
          {
            if (Pixle[i][j] == 0)
            {
              stop_black_cnt++;
            }
            stop_all_cnt++;
          }
          if (stop_black_cnt > stop_all_cnt / 1.5)
          {
            stop_flag_cnt += 0.35;
          }
          else if (stop_black_cnt > stop_all_cnt / 2)
          {
            stop_flag_cnt += 0.2;
          }
          if (stop_flag_cnt >= 1)
          {
            Road = 7;
            return;
          }
        }
        start_stop_line = 0;
      }
    }
  }

  else if (Road == 7 & Road7_flag == 0 || Road7_flag == 1) //等待转弯
  {
    for (int i = start_stop_line - 5; i < Last_row; i++)
    {
      if (Lef[i] > Fir_col + 5)
      {
        stop_line_cnt = 0;
        continue;
      }
      else
      {
        stop_line_cnt++;
        if (stop_line_cnt >= 3)
        {
          start_stop_line = i - 2;
          break;
        }
      }
    }
    if (start_stop_line > 30)
    {
      Road = 7;
      Road7_flag = 1; //等待转弯，远景行补线
    }
    if (start_stop_line > 40)
    {
      Road = 7;
      Road7_flag = 2; //开始转弯
    }
  }
  else if (Road7_flag == 2) //开始转弯
  {
    turn_cnt++;
    Servo_Duty(SERVO_RANGE);
    if (turn_cnt >= (int)(10 * 100 / (get_speed() * CAMERA_FPS)) + 1)
    {
      Road7_flag = 3;
    }
  }
  else if (Road7_flag == 3) //转弯结束停车
  {
    turn_cnt = 0;
    stop_cnt++;
    stop_line = Fir_row;
    for (int i = Last_row; i < stop_line; i++)
    {

      if (Pixle[i][8] + Pixle[i][72] + Pixle[i][30] + Pixle[i][35] + Pixle[i][40] + Pixle[i][45] + Pixle[i][50] < 2)
      {
        Black_line++;
        if (Black_line >= 3)
        {
          stop_line = i - 2;
          break;
        }
      }
      else
      {
        Black_line = 0;
      }
    }
    if (stop_line > 35 || stop_cnt >= (int)(30 * 100 / (get_speed() * CAMERA_FPS)) + 1)
    {
      Road = 8; //停车
      lib_set_speed(0);
    }
  }
  return;
}

#if 0
/*************************************************************************
*  函数名称：void Pic_find_leap(void)
*  功能说明：图像找突变点
*  参数说明：无
*  函数返回：无
*  修改时间：2019.3.23
*  备    注：突变点定义：左右边线不连续的点
//没有用到，似乎在旧的圆环状态机中使用。变量也注释掉了
*************************************************************************/
/*
void Pic_find_leap(void)
{
  int i;

  int count1 = 0, count2 = 0;
  for (i = 0; i < 4; i++)
  {
    Lef_leap[i] = 0;
    Rig_leap[i] = 0;
  }
  for (i = Last_row; i >= Fir_row + 4; i--)
  {
    if (abs(Lef[i] - Lef[i - 2]) < 5 && abs(Lef[i - 4] - Lef[i - 2]) > 6) //！！条件可能偏宽松
    {
      Lef_leap[count1] = i - 3;
      count1++;
      if (count1 == 4) //图像跳变点大于4个则停止寻找
      {
        break;
      }
    }
  }

  for (i = Last_row; i >= Fir_row + 4; i--)
  {
    if (abs(Rig[i] - Rig[i - 2]) < 5 && abs(Rig[i - 4] - Rig[i - 2]) > 6)
    {
      Rig_leap[count2] = i - 3;
      count2++;
      if (count2 == 4)
      {
        break;
      }
    }
  }
}
*/
/*
void Cam_End_Detect(void) //End记录从上至下最后一行边界行
{
  int i;

  for (i = 50; i >= Fir_row; i--)
  {
    if (Lef[i] == 1 && Pixle[i][2] == 0)
      Lef_End = i;
    if (Rig[i] == 78 && Pixle[i][77] == 0)
      Rig_End = i;

    if (Lef[i] == 1 && Rig[i] == 78)
      break; // || (Lef[i]==1 && Pixle[i][2] ==1) || (Rig[i]==78 && Pixle[i][77] ==1))break;
  }
  Cam_End = max_block(Lef_End, Rig_End);
}
*/

int cnt_duanlu = 0;
int end;
int Road6_flag = 0;
void Cam_Break_Rec(void)
{
  int i;
  Cam_Break_Flag = 0;
  end = max_block(Lef_End, Rig_End) + 1;

  if (end >= 30 && Road == 0)
  {
    //      if(((Pixle[end-15][36]==1 && Pixle[end-15][44]==1)||(Pixle[end-18][36]==1 && Pixle[end-18][44]==1)) && Road == 0 && Start_Runing_Line == 0 )
    //      {
    //        Start_Runing_Line = 1;
    //      }
    //Cam_Break_Flag = 1;
    Road = 6;
  }
  if (Road == 6 && Road6_flag == 0)
  {
    for (i = 58; i > 50; i--)
    {
      if (Lef[i] == 1 && Rig[i] == 78)
      {
        continue;
      }
      else
      {
        return;
      }
    }
    Road6_flag = 1;
    return;
  }
  else if (Road == 6 && Road6_flag == 1)
  {
    for (i = 58; i > 50; i--)
    {
      if (Lef[i] != 1 || Rig[i] != 78)
      {
        continue;
      }
      else
      {
        return;
      }
    }
    Road6_flag = 2;
    return;
  }
  else if (Road == 6 && Road6_flag == 2)
  {
    cnt_duanlu++;
    if (cnt_duanlu > 10)
    {
      cnt_duanlu = 0;
      Road = 0;
      Road6_flag = 0;
    }
    return;
  }
}

/*************************************************************************
*  函数名称：void Pic_Block_Rec(void)
*  功能说明：路障
*  参数说明：无
*  函数返回：无
*  修改时间：2019.5.8
*  备    注：不用了

*************************************************************************/

int Cam_Block_Cnt = 0;
int Cam_Block_State = 0;
int Road6_flag;
int Block_wrong;
int ramp_cnt = 0;
int cnt_ramp = 0;
char STOP_flag = 0;
int black_line;
int start_stop = 0;
void Pic_Block_Rec(void) //障碍物识别
{
  int i;
  //int j;
  int STOP_line = 0;
  //static char flag5 = 1;

  if (Road == 3)
  {
    cnt_ramp++;
    if (cnt_ramp > 50)
    {
      Road = 0;
      cnt_ramp = 0;
      Cam_Block_State = 0;
      return;
    }
    return;
  }
  else if (Road == 0 && Cam_Block_State == 0) //如果前40行找不到边线，认为是断路或者起跑线
  {
    if (icm_gyro_y < -3000)
    {
      Road = 3;
      return;
    }

    for (i = 1; i < 60; i++) //自上而下寻找有边线的开始行
    {
      if (Lef[i] == 1 && Rig[i] == 78)
      {
        continue;
      }
      else
      {
        STOP_line = i;
        break;
      }
    }
    start_stop++;
    if (start_stop > 600)
    {
      start_stop = 601;
      for (i = 15; i < 40; i++)
      {
        if (Lef[i] == 1 && Rig[i] == 78)
        {
          black_line++;
          if (black_line > 3 && STOP_line < 5 && Lef_slope != 999 && Rig_slope != 999)
          {
            Road = 7;
            return;
          }
        }
        else
        {
          black_line = 0;
        }
      }
    }
    //      if(STOP_line < 30&&STOP_line> 15)
    //      {
    //        if(FINAL[STOP_line-10]==1 && FINAL[STOP_line-11]==1 && FINAL[STOP_line-12]==1 && FINAL[STOP_line-13]==1 && FINAL[STOP_line-14]==1 && FINAL[STOP_line+1] ==1 && FINAL[STOP_line+2] == 1 )
    //        { Road=7;
    //          return;
    //        }
    //      }
    if (STOP_line > 30)
    {
      Road = 6;
      return;
    }
    if (STOP_line > 5 && STOP_line < 15 && Lef_slope != 999 && Rig_slope != 999 && LR_slope < 0.1 && LR_slope > -0.1)
      Cam_Block_State = 1;
    return;
  }

  /*
  else if(Cam_Block_State == 1)//当测距小于100时，则认为车离障碍物还有1米的距离，开始进行开环转向，进入状态2，使车向右偏转约60°角度
  {


   if(ToF_distance > Tof_thres && flag5 ==1)//ToF_distance < 6000
    {
      flag5 = 0;
      Road = 5;//断路与障碍物标志位
      Turn_Angle_flag=1;
      Cam_Block_State = 2;
      Cam_Block_Cnt = 0;
      Turn_Angle=0;
      return;
    }
   else
     Cam_Block_State = 0;

  }

  else if(Cam_Block_State == 2)//开环进100*10ms = 1s后进入状态3,使车保持60°角前进一定距离，离开赛道
  {
    if( block_flag == 1)
    {
    if( (Block_Angle_Set - Turn_Angle) <1)//Cam_Block_Cnt == 100)
    {
      Cam_Block_State = 3;
      Cam_Block_Cnt = 0;
      Turn_Angle=0;
    }
    }
    else if(block_flag == -1)
    {
    if( (Block_Angle_Set + Turn_Angle) <1)//Cam_Block_Cnt == 100)
    {
      Cam_Block_State = 3;
      Cam_Block_Cnt = 0;
      Turn_Angle=0;
    }
    }
  }
  else if(Cam_Block_State == 3)//开环进100*10ms = 1s后进入状态5，保持车身角度向前行进一定距离
  {
    if( block_flag == 1)
    {
    if( (Turn_Angle+Block_Angle_Set2) <1)//Cam_Block_Cnt == 100)
    {
      Cam_Block_State = 4;
      Cam_Block_Cnt = 0;
      Turn_Angle=0;
    }
    }
    else if( block_flag == -1)
    {
    if( (Block_Angle_Set2 - Turn_Angle) <1)//Cam_Block_Cnt == 100)
    {
      Cam_Block_State = 4;
      Cam_Block_Cnt = 0;
      Turn_Angle=0;
    }
    }
  }
  else if(Cam_Block_State == 4)//开环进100*10ms = 1s后进入状态7，使车保持角度回到赛道
  {
    if( block_flag == 1)
    {
    if( (Block_Angle_Set - Turn_Angle) <1)//Cam_Block_Cnt == 100)
    {
      Cam_Block_State = 0;
      Road = 0;
      Turn_Angle_flag=0;
      Cam_Block_Cnt = 0;
    }
    }
    else if( block_flag == -1)
    {
    if( (Block_Angle_Set + Turn_Angle) <1)//Cam_Block_Cnt == 100)
    {
      Cam_Block_State = 0;
      Road = 0;
      Turn_Angle_flag=0;
      Cam_Block_Cnt = 0;
    }
    }

  }

  */

  return;
}
#endif