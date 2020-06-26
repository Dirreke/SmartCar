#include "headfile.h"

int Allwhitestart; //全白行开始行
int Allwhiteend;   //全白行结束行

int Lef_circle;
int Rig_circle; //左右边线寻找环状黑线标志位
int Lef_break_point;
int Rig_break_point; //左右环状黑线拐弯点

int start_stop_line_flag = 0;
int barn_line = Fir_row;
int start_stop_line = 0;

int Road = 0; //道路类型标志位
int Road0_flag = 0;
int Road1_flag = 0;
int Road2_flag = 0;
int Road7_flag = 0;
int EM_Road = 0;
bool ganhuangguan_flag=0;
int ganhuangguan_flag_cnt1=0;

int turn_stop = 0; //转弯用
float Cam_offset = 0;

/*************************************************************************
*  函数名称：void camera_dispose_main(void)
*  功能说明：图像处理主函数
*  参数说明：无
*  函数返回：无
*  修改时间：2019.5.31
*  备    注：
*************************************************************************/

void camera_dispose_main(void) //摄像头处理主函数
{
    Get_Use_Image(); //图像预处理
    sobel();
    Pic_noi_elim();      //图像简单去噪点
    Pic_DrawLRside();    //寻找左右边线
    Pic_undistort(1, 1); //图像去畸变
    Pic_particular();
    LR_Slope_fig();    //左右边线斜率计算
    Allwhite_find();   //查找全白行//注释Allwhitestart2.Allwhiteend2
    Pic_find_circle(); //寻找环状黑线及拐点
    start_stop_find();
    Road_rec(); //利用左右边线斜率识别赛道
    // start_stop_rec();    //识别起跑线
    Threshold_change();
    Pic_Fix_Line();      //补线处理
    Pic_DrawMid();       //计算去畸前中心线-仅上位机用
    Pic_DrawMid_und();   //计算去畸后中线
    Pic_offset_fig();    //offset计算//注释Cam_offset2
    Pic_offset_filter(); //offset滤波

    Get_pic_with_edge(); //获得带边线灰度图
    Turn_Cam();
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
    int i;
    int Allwhiterow[LCDH]; //全白行，1表示全白，否则为0
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
    while (Allwhiterow[i] == 1 && i > Fir_row)
    {
        i = i - 1;
    }
    Allwhiteend = i;
}

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
*  函数名称：void start_stop_find(void)
*  功能说明：起跑线识别
*  参数说明：无
*  函数返回：无
*  修改时间：2020.06.17
*  备    注：
*************************************************************************/
void start_stop_find(void)
{
    start_stop_line_flag = 0;
    static int start_waited = 0;
    int tiaobian1 = 0;

    int Rig_end = 0, Lef_end = 0, Road_end = 0;

    // if (Road == 0 && Road0_flag == 0)
    // {
    start_waited = 601;
    if (start_waited == 601)
    {
        start_waited = 601;
    }
    else
    {
        return;
    }
    for (int i = Fir_row + 3; i < Last_row - 4; ++i)
    {
        if (Rig[i] >= Last_col)
        {
            continue;
        }
        if (Rig[i] > 40 && Rig[i + 2] - Rig[i] < 5 && Rig[i + 3] - Rig[i + 1] < 5 && Rig[i + 4] - Rig[i + 2] < 5 &&
            Rig[i + 2] - Rig[i] >= 0 && Rig[i + 3] - Rig[i + 1] >= 0 && Rig[i + 4] - Rig[i + 2] >= 0)
        {
            Rig_end = i;
            break;
        }
    }
    for (int i = Fir_row + 3; i < Last_row - 4; ++i)
    {
        if (Lef[i] <= Fir_col)
        {
            continue;
        }
        if (Lef[i] < 40 && Lef[i] - Lef[i + 2] < 5 && Lef[i + 1] - Lef[i + 3] < 5 && Lef[i + 2] - Lef[i + 4] < 5 &&
            Lef[i] - Lef[i + 2] >= 0 && Lef[i + 1] - Lef[i + 3] >= 0 && Lef[i + 2] - Lef[i + 4] >= 0)
        {
            Lef_end = i;
            break;
        }
    }
    Road_end = (Lef_end < Rig_end ? Rig_end : Lef_end);
    if (Road_end > 30)
    {
        return;
    }
#ifdef TL2barn
    for (int i = Rig_end; i < Last_row - 7; ++i)
    {
        if (Rig[i] > 40 && Rig[i + 2] - Rig[i] < 5 && Rig[i + 3] - Rig[i + 1] < 5 &&
            Rig[i + 2] - Rig[i] > 0 && Rig[i + 3] - Rig[i + 1] > 0 &&
            Rig[i + 3] - Rig[i + 7] > 25)
        {
            tiaobian1 = i;
            break;
        }
        // if (Rig[i] > 40 && Rig[i + 2] - Rig[i] < 5 && Rig[i + 3] - Rig[i + 1] < 5 && Rig[i + 4] - Rig[i + 2] < 5 &&
        //     Rig[i + 2] - Rig[i] > 0 && Rig[i + 3] - Rig[i + 1] > 0 && Rig[i + 4] - Rig[i + 2] > 0)
        // {
        //     continue;
        // }
        // if (Rig[i + 3] - Rig[i + 7] > 25)
        // {
        //     tiaobian1 = i;
        //     break;
        // }
    }
    if (tiaobian1 != 0)
    {
        for (int i = tiaobian1; i < Last_row - 4; ++i)
        {
            // if (Lef[i] - Fir_co > 5)
            // {
            //     return;
            // }
            if (Rig[i] > 40 && Rig[i] - Rig[i - 3] > 25 && Rig[i + 2] - Rig[i] < 5 && Rig[i + 3] - Rig[i + 1] < 5 && Rig[i + 4] - Rig[i + 2] < 5)
            {
                start_stop_line_flag = 1;
                start_stop_line = tiaobian1;
                break;
            }
            else
            {
                continue;
            }
        }
    }
    else
    {
        return;
    }
#endif
#ifdef TR2barn
    for (int i = Lef_end; i < Last_row - 7; ++i)
    {
        if (Lef[i] < 40 && Lef[i] - Lef[i + 2] < 5 && Lef[i + 1] - Lef[i + 3] < 5 &&
            Lef[i] - Lef[i + 2] > 0 && Lef[i + 1] - Lef[i + 3] > 0 &&
            Lef[i + 7] - Lef[i + 3] > 25)
        {
            tiaobian1 = i;
            break;
        }

        // if (Lef[i] < 40 && Lef[i] - Lef[i + 2] < 5 && Lef[i + 1] - Lef[i + 3] < 5 && Lef[i + 2] - Lef[i + 4] < 5 &&
        //     Lef[i] - Lef[i + 2] > 0 && Lef[i + 1] - Lef[i + 3] > 0 && Lef[i + 2] - Lef[i + 4] > 0)
        // {
        //     continue;
        // }
        // if (Lef[i + 7] - Lef[i + 3] > 25)
        // {
        // }
    }
    if (tiaobian1 != 0)
    {
        for (int i = tiaobian1; i < Last_row - 4; ++i)
        {
            // if (Last_col - Rig[i] > 5)
            // {
            //     return;
            // }
            if (Lef[i] < 40 && Lef[i - 3] - Lef[i] > 25 && Lef[i] - Lef[i + 2] < 5 && Lef[i + 1] - Lef[i + 3] < 5 && Lef[i + 2] - Lef[i + 4] < 5)
            {
                start_stop_line_flag = 1;
                start_stop_line = tiaobian1;
                break;
            }
            else
            {
                continue;
            }
        }
    }
    else
    {
        return;
    }
#endif
    // }
}

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
    static int Road00_count = 0;
    static int Road10_count = 0, Road20_count = 0, Road70_count = 0;
    //int i = 0;

    // static int oldwhite=5000;
    // static uint8 Road1_cnt1=0;
    // static char Road1_flag1=0;
    if ((Road == 1 && Road1_flag == 1) || (Road == 2 && Road2_flag == 1))
    {
        EM_Road = 1;
    }

    if (EM_Road == 1 && Road == 0 && Road0_flag == 0 && EM_Value_1 < 0.6 && EM_Value_4 < 0.6)
    {
        EM_Road = 0;
    }

    if (Lef_slope == 998 && Rig_slope == 998 && Road7_flag != 2 && Lef_edge < 12 && Rig_edge < 12)
    {
        Road0_count++;
        if (Road0_count >= 5)
        {
            Road = 0;
            EM_Road = 0;
        }
    }
    else
    {
        Road0_count = 0;
    }
    //起跑线
    if (Road == 0 && Road0_flag == 0)
    {

        if (start_stop_line_flag == 1)
        {
            Road70_count++;
            if (Road70_count > 2)
            {
                Road70_count = 0;
                Road = 7;
                Road7_flag = 0;
                for (int i = ((start_stop_line - 10) > Fir_row ? (start_stop_line - 10) : Fir_row); i < Last_row - 2; ++i)
                {
                    if (Lef[i - 4] - Lef[i - 2] < 5 && Lef[i - 3] - Lef[i - 1] < 5 && Lef[i - 2] - Lef[i] < 5)
                    {
                        continue;
                    }
                    if ((Lef[i] - Fir_col < 5 && Lef[i + 1] - Fir_col < 5 && Lef[i + 2] - Fir_col < 5) || Lef[i + 2] - Lef[i - 1] > 15)
                    {
                        barn_line = i - 1;
                        break;
                    }
                    barn_line = start_stop_line - 5;
                }
                barn_line = (barn_line > (Fir_row + 5) ? barn_line : (Fir_row + 5));
                return;
            }
        }
        else
        {
            Road70_count = 0;
        }
    }

    if (Road == 0)
    {

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////普通赛道→圆环
        if ((Lef_break_point < 45 && Road == 0 && Rig_circle == 0 && Lef_circle == 1 && Lef_slope != 998 && Rig_slope == 998 &&
             Rig[39] - Rig[37] < 5 && Rig[37] - Rig[35] < 5 && Rig[35] - Rig[33] < 5 && Rig[33] - Rig[31] < 5 && Rig[31] - Rig[29] < 5 && Rig[29] - Rig[27] < 5 && Rig[27] - Rig[25] < 5 && Rig[25] - Rig[23] < 5 &&
             //  (New_Lef[54] == -MIDMAP || New_Lef[55] == -MIDMAP || New_Lef[56] == -MIDMAP)&&
             Rig_edge < 15)) //左圆环：左边线,右边线：直通到底//&& Rig[11] != 78
        {
            Road10_count++;
            if (Road10_count == 2)
            {
                Road = 1;
                Road1_flag = 0;
                Road10_count = 0;
            }
            return;
        }
        else if (Rig_break_point < 45 && Road == 0 && Lef_circle == 0 && Rig_circle == 1 && Rig_slope != 998 && Lef_slope == 998 &&
                 Lef[25] - Lef[27] < 5 && Lef[27] - Lef[29] < 5 && Lef[29] - Lef[31] < 5 && Lef[31] - Lef[33] < 5 && Lef[33] - Lef[35] < 5 && Lef[35] - Lef[37] < 5 && Lef[37] - Lef[39] < 5 && Lef[23] - Lef[25] < 5 &&
                 //     (New_Rig[54] == MIDMAP || New_Rig[55] == MIDMAP || New_Rig[56] == MIDMAP) &&
                 Lef_edge < 15) //右圆环：右边线：突变点→拐点→突变点//&& Lef[11] != 2
        {
            Road20_count++;
            if (Road20_count == 2)
            {
                Road = 2;
                Road2_flag = 0;
                Road20_count = 0;
            }
            return;
        }
        //弯道状态机
        else if (((Rig_slope > -0.5 && Rig_slope != 998 && Rig_slope != 999) || Road0_flag == 4) && Rig_slope != 998) //左转弯//(Lef_break_point > 35 && Lef_circle == 1 && Rig_circle == 0)
        {

            TurnLeft_Process();
            if (Road0_flag == 4)
            {
                return;
            }
        }
        else if ((Lef_slope < 0.5 || Road0_flag == 5) && Lef_slope != 998) //右转弯//(Rig_break_point > 35 && Rig_circle == 1 && Lef_circle == 0)
        {
            TurnRight_Process();
            if (Road0_flag == 5)
            {
                return;
            }
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////左圆环→普通赛道
    else if (Road == 1)
    {
        Road0_flag = 0;
        Road1_zhuangtaiji();
        return;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////右圆环→普通赛道，同上左圆环，注释从简
    else if (Road == 2)
    {
        Road0_flag = 0;
        Road2_zhuangtaiji();
        return;
    }
    else if (Road == 7)
    {
        Road0_flag = 0;
        Road7_zhuangtaiji();
        return;
    }

    if (Road == 0)
    {
        // if (whitecnt > 2300 && ((Lef_edge > 7 && Rig_edge > 7) || Lef_edge > 15 || Rig_edge > 15) && Allwhitestart > 35 &&
        //     40 - Lef[Fir_row] <= 15 && 40 - Lef[Fir_row + 1] <= 15 &&
        //     Lef[Fir_row] - Lef[Fir_row + 2] < 5 Rig[Fir_row] - 40 <= 15 && Rig[Fir_row + 1] - 40 <= 15)
        // {
        //     Road0_flag = 3; //十字前后补
        // }
        if (whitecnt > 1700 && ((Lef_edge > 7 && Rig_edge > 7) || Lef_edge > 15 || Rig_edge > 15) && Allwhitestart <= 45 && Allwhitestart > (Fir_row + 5) && Allwhitestart - Allwhiteend > 3)
        {
            Road0_flag = 1;
        }
        else if ((Allwhitestart > 45 && Allwhiteend < 52) && Allwhitestart - Allwhiteend > 3 && whitecnt > 2000)
        {
            Road0_flag = 2;
        }

        else
        {
            Road00_count++;
            if (Road00_count == 2)
            {
                Road00_count = 0;
                Road0_flag = 0;
            }
        }
    }
}

/*************************************************************************
*  函数名称：void TurnLeft_Process(void)
*  功能说明：左转进程
*  参数说明：无
*  函数返回：无
*  修改时间：2020.06.17
*  备    注：
*************************************************************************/
void TurnLeft_Process(void)
{
    static int turn_stop_flag = 0, Road04_count = 0, Road00_count = 0;
    int temp = 40;
    int dis = 0, dis1 = 0;
    for (int i = Fir_row; i < 40; ++i)
    {
        if (Rig[i] < 40 && Rig[i + 1] <= 40 && Rig[i + 2] >= 40 && Rig[i + 3] > 40 &&
            Rig[i + 5] - Rig[i + 3] < 7 && Rig[i + 7] - Rig[i + 5] < 7 && Rig[i + 9] - Rig[i + 7] < 7 && Rig[i + 11] - Rig[i + 9] < 7 &&
            Rig[i + 2] - Rig[i] < 10 &&
            Rig[i + 5] - Rig[i + 3] > 0 && Rig[i + 7] - Rig[i + 5] > 0 && Rig[i + 9] - Rig[i + 7] > 0 && Rig[i + 11] - Rig[i + 9] > 0 &&
            Rig[i + 2] - Rig[i] > 0)
        //可能较严，（出现连续边线为40）
        {
            temp = i;
            break;
        }
    }

    if (temp == 40 && turn_stop_flag == 1)
    {
        Road00_count++;
        if (Road00_count == 2)
        {
            Road00_count = 0;
            Road0_flag = 0;
            turn_stop_flag = 0;
            return;
        }
    }
    else
    {
        Road00_count = 0;
    }
    dis = Rig[temp + 1] - Rig[temp];
    for (int i = temp; i > Fir_row; --i)
    {
        if (Rig[i - 1] > 40)
        {
            turn_stop = i + 1;
            break;
        }
        dis1 = Rig[i] - Rig[i - 1];
        if (dis1 < 0)
        {
            turn_stop = i;
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
            turn_stop = i;
            break;
        }
    }

    if (turn_stop < 28)
    {
        turn_stop_flag = 1;
    }
    if (Road0_flag != 4 && Rig[turn_stop] < 34 && dis > 4)
    {
        Road04_count++;
        if (Road04_count == 2)
        {
            Road0_flag = 4; //turn left flag
            threshold_offset2 = -5;
            Road04_count = 0; //reset
        }
    }
    else
    {
        Road04_count = 0;
    }

    return;
}

/*************************************************************************
*  函数名称：void TurnRight_Process(void)
*  功能说明：左转进程
*  参数说明：无
*  函数返回：无
*  修改时间：2020.06.17
*  备    注：
*************************************************************************/
void TurnRight_Process(void)
{
    static int turn_stop_flag = 0, Road05_count = 0, Road00_count = 0;
    int temp = 40;
    int dis = 0, dis1 = 0;
    for (int i = Fir_row; i < 40; ++i)
    {
        if (Lef[i] > 40 && Lef[i + 1] >= 40 && Lef[i + 2] <= 40 && Lef[i + 3] < 40 &&
            Lef[i + 3] - Lef[i + 5] < 7 && Lef[i + 5] - Lef[i + 7] < 7 && Lef[i + 7] - Lef[i + 9] < 7 && Lef[i + 9] - Lef[i + 11] < 7 &&
            Lef[i] - Lef[i + 2] < 10 &&
            Lef[i + 3] - Lef[i + 5] > 0 && Lef[i + 5] - Lef[i + 7] > 0 && Lef[i + 7] - Lef[i + 9] > 0 && Lef[i + 9] - Lef[i + 11] > 0 &&
            Lef[i] - Lef[i + 2] > 0)
        {
            temp = i;
            break;
        }
    }
    if (temp == 40 && turn_stop_flag == 1)
    {
        Road00_count++;
        if (Road00_count == 3)
        {
            Road00_count = 0;
            Road0_flag = 0;
            turn_stop_flag = 0;
            return;
        }
        else
        {
            Road00_count = 0;
        }
    }
    dis = Lef[temp] - Lef[temp + 1];
    for (int i = temp; i > Fir_row; --i)
    {
        if (Lef[i - 1] < 40)
        {
            turn_stop = i + 4;
            break;
        }
        dis1 = Lef[i - 1] - Lef[i];
        if (dis1 < 0)
        {
            turn_stop = i;
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
            turn_stop = i;
            break;
        }
    }
    if (turn_stop < 28)
    {
        turn_stop_flag = 1;
    }
    if (Road0_flag != 5 && Lef[turn_stop] > 45 && dis > 4)
    {
        Road05_count++;
        if (Road05_count == 2)
        {
            Road0_flag = 5; //turn left flag
            threshold_offset2 = -5;
            Road05_count = 0; //reset
        }
    }
    else
    {
        Road05_count = 0;
    }
    return;
}

/*************************************************************************
*  函数名称：void Road1_zhuangtaiji(void)
*  功能说明：左圆环状态机
*  参数说明：无
*  函数返回：无
*  修改时间：2020.06.17
*  备    注：
*************************************************************************/
void Road1_zhuangtaiji(void)
{
    static int Road11_count = 0, Road12_count = 0, Road13_count = 0, Road14_count = 0, Road15_count = 0, Road16_count = 0;
    int dis = 0, dis1 = 0;
    if (Road1_flag == 0) //准备进左圆环
    {
        Road0_flag = 0;
        if (EM_Value_2 + EM_Value_3 > 4.3) //弯内识别：左右两边仅有一边发生丢线
        {
            Road11_count++;
            if (Road11_count > 1)
            {
                Road11_count = 0;
                Road1_flag = 1; //表示已经进入左圆环
            }
        }
        else
        {
            Road11_count = 0;
        }
    }
    else if (Road1_flag == 1) //进左圆环1/4
    {
        if (Lef_circle_point != 0)
        {
            Road12_count++;
            if (Road12_count > 1)
            {
                Road1_flag = 2;
            }
        }
        else
        {
            Road12_count = 0;
        }
        // for (int i = 35; i > Fir_row; i--)
        // {
        //     if (Last_col - Rig[i] < 2 || Last_col - Rig[i - 1] < 2)
        //     {
        //         continue;
        //     }
        //     if (i < 28)
        //     {
        //         Road12_count++;
        //         if (Road12_count > 1) //2帧后 进左圆环第一弯道
        //         {
        //             Road1_flag = 2;
        //             Road12_count = 0;
        //         }
        //     }
        //     else
        //     {
        //         Road12_count = 0;
        //     }
        //     break;
        // }
        // if (Lef_circle == 0 || (Lef_circle == 1 && Lef_break_point > 45)) //if(((Lef_circle==0||( Lef_circle ==1 && Lef_break_point>30)))&& Road1_turnin(EM_Value_2,EM_Value_3,3.8))//if(((Lef_circle==0||( Lef_circle ==1 && Lef_break_point>30))) && Rig_slope>=10)/ && Road1_turnin(EM_Value_2,EM_Value_3,3.8))    //Rig_slope<1 && (Lef_leap[0]==0||Lef_slope==999)&& Rig_leap[0]==0)
        // {
        //     Road12_count++;
        //     if (Road12_count > 1) //2帧后 进左圆环第一弯道
        //     {
        //         Road1_flag = 2;
        //         Road12_count = 0;
        //     }
        // }
        // else
        // {
        //     Road12_count = 0;
        // }
    }
    else if (Road1_flag == 2) //进左圆环2/4 开始补线进弯道
    {
        // Road14_count++;
        for (int i = Last_row; i > Fir_row; i--)
        {
            if (Last_col - Rig[i] < 2 || Last_col - Rig[i - 1] < 2)
            {
                continue;
            }
            if (i > 50 && Rig[i] - Rig[i - 2] < 8 && Rig[i - 2] - Rig[i - 4] < 8 && Rig[i] - Rig[i - 2] > 0 && Rig[i - 2] - Rig[i - 4] > 0)
            {
                Road14_count++;
                //
                if (Road14_count > 1)
                {
                    Road1_flag = 4;
                    Road14_count = 0;
                    break;
                }
            }
            else
            {
                Road14_count = 0;
                break;
            }
        }
        // if (Road14_count > (int)(DIS_IN_CIRCLE * 10000 / (SpeedGoal * CAMERA_FPS)) + 1) //宏定义在function.h
        // {
        //     Road1_flag = 4;
        //     Road14_count = 0;
        // }
    }
    else if (Road1_flag == 4) //进入圆环内 ，取消补线
    {
        dis = Rig[40 + 1] - Rig[40];
        for (int i = 40; i > Fir_row; --i)
        {
            dis1 = Rig[i] - Rig[i - 1];
            if (dis1 < 0)
            {
                turn_stop = i;
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
                turn_stop = i;
                break;
            }
        }
        if (Allwhitestart > 29 && Allwhiteend < 30) //Rig_circlr 不好，改!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        {
            Road13_count++;
            if (Road13_count > 2)
            {
                Road1_flag = 3;
                Road13_count = 0;
            }
        }
        else
        {
            Road13_count = 0;
        }
    }
    else if (Road1_flag == 3) //准备出圆环，右边线补线
    {
        if (Allwhitestart >= 45)
        {
            Road15_count++;
            if (Road15_count > 1)
            {
                Road15_count = 0;
                Road1_flag = 5;
            }
        }
        else
        {
            Road15_count = 0;
        }
    }
    else if (Road1_flag == 5) //右边线已经不能补线，电磁等方法跑
    {
        // if ((Rig_slope > -0.02 && Rig_slope < 0) || (Pixle[58][74] == 1 && Pixle[57][74] == 1 && Pixle[56][74] == 1 && Pixle[55][74] == 1 && Pixle[54][74] == 1 && Pixle[53][74] == 1)) //|| Lef_edge < 20))
        if (((Rig_slope < -0.1 || Rig_slope == 998) && Allwhiteend > 45) || Allwhiteend == Fir_row)
        {
            Road16_count++;
            if (Road16_count > 3)
            {
                Road16_count = 0;
                // Road = 0;
                Road0_flag = 0;
                Road1_flag = 0;
            }
        }
        else
        {
            Road16_count = 0;
        }
    }
    return;
}
/*************************************************************************
*  函数名称：void Road1_zhuangtaiji(void)
*  功能说明：右圆环状态机
*  参数说明：无
*  函数返回：无
*  修改时间：2020.06.17
*  备    注：
*************************************************************************/

void Road2_zhuangtaiji(void)
{
    static int Road21_count = 0, Road22_count = 0, Road23_count = 0, Road24_count = 0, Road25_count = 0, Road26_count = 0;
    int dis = 0, dis1 = 0;
    if (Road2_flag == 0) //
    {
        if (EM_Value_2 + EM_Value_3 > 4.3) //弯内识别：左右两边仅有一边发生丢线
        {
            Road21_count++;
            if (Road21_count > 1)
            {
                Road21_count = 0;
                Road2_flag = 1; //表示已经进入左圆环
            }
        }
        else
        {
            Road21_count = 0;
        }
    }
    else if (Road2_flag == 1) //
    {
        if (Rig_circle_point != 0)
        {
            Road22_count++;
            if (Road22_count > 1)
            {
                Road2_flag = 2;
            }
        }
        else
        {
            Road22_count = 0;
        }
        // for (int i = 25; i > Fir_row; i--)
        // {
        //     if (Lef[i] - Fir_col < 2 || Lef[i - 1] - Fir_col < 2)
        //     {
        //         continue;
        //     }
        //     if (i < 28)
        //     {
        //         Road22_count++;
        //         if (Road22_count > 1) //2帧后 进左圆环第一弯道
        //         {
        //             Road2_flag = 2;
        //             Road22_count = 0;
        //         }
        //     }
        //     else
        //     {
        //         Road22_count = 0;
        //     }
        //     break;
        // }

        // if ((Rig_circle == 0 || (Rig_circle == 1 && Rig_break_point > 45)) && Rig_slope >= 10) //if(((Rig_circle==0||( Rig_circle ==1 && Rig_break_point>30))) && Rig_slope>=10)/ && Road1_turnin(EM_Value_2,EM_Value_3,3.4))
        // {
        //     Road22_count++;
        //     if (Road22_count > 1) //
        //     {
        //         Road2_flag = 2;
        //         Road22_count = 0;
        //     }
        // }
        // else
        // {
        //     Road22_count = 0;
        // }
    }
    else if (Road2_flag == 2) //
    {
        // Road24_count++;
        for (int i = Last_row; i > Fir_row; i--)
        {
            if (Lef[i] - Fir_col < 2 || Lef[i - 1] - Fir_col < 2)
            {
                continue;
            }
            if (i > 50 && Lef[i - 2] - Lef[i] < 8 && Lef[i - 4] - Lef[i - 2] < 8 && Lef[i - 2] - Lef[i] > 0 && Lef[i - 4] - Lef[i - 2] > 0)
            {
                Road24_count++;
                if (Road24_count > 1)
                {
                    Road24_count = 0;
                    Road2_flag = 4;
                    break;
                }
            }
            else
            {
                Road24_count = 0;
                break;
            }
        }

        // if (Road24_count == (int)(DIS_IN_CIRCLE * 10000 / (SpeedGoal * CAMERA_FPS)) + 1)
        // {
        //     Road24_count = 0;
        //     Road2_flag = 4;
        // }
    }
    else if (Road2_flag == 4)
    {
        dis = Lef[40] - Lef[40 + 1];
        for (int i = 40; i > Fir_row; --i)
        {
            dis1 = Lef[i - 1] - Lef[i];
            if (dis1 < 0)
            {
                turn_stop = i;
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
                turn_stop = i;
                break;
            }
        }

        if (Allwhitestart > 29 && Allwhiteend < 30)
        {
            Road23_count++;
            if (Road23_count > 2)
            {
                Road23_count = 0;
                Road2_flag = 3;
            }
        }
        else
        {
            Road23_count = 0;
        }
    }
    else if (Road2_flag == 3)
    {
        if (Allwhitestart >= 45)
        {
            Road25_count++;
            if (Road25_count > 1)
            {
                Road25_count = 0;
                Road2_flag = 5;
            }
        }
        else
        {
            Road25_count = 0;
        }
    }
    else if (Road2_flag == 5)
    {
        // Road0_flag = 0;
        // if ((Lef_slope > 0 && Lef_slope < 0.02) || (Pixle[58][5] == 1 && Pixle[57][5] == 1 && Pixle[56][5] == 1 && Pixle[55][5] == 1 && Pixle[54][5] == 1 && Pixle[53][5] == 1)) //|| Lef_edge < 20))
        if (((Lef_slope > 0.1 || Lef_slope == 998) && Allwhiteend > 45) || Allwhiteend == Fir_row)
        {
            Road26_count++;
            if (Road26_count > 3)
            {
                Road26_count = 0;
                Road = 0;

                Road2_flag = 0;
            }
        }
        else
        {
            Road26_count = 0;
        }
    }
}

/*************************************************************************
*  函数名称：void Road7_zhuangtaiji(void)
*  功能说明：起跑线状态机
*  参数说明：无
*  函数返回：无
*  修改时间：2020.06.17
*  备    注：
*************************************************************************/
void Road7_zhuangtaiji(void)
{
    static int Road73_count = 0;
    static int Road74_count = 0;
    static int stop_line = Fir_row;
    int Black_line = 0;
    if (Road7_flag == 0 || Road7_flag == 1) //等待转弯
    {
        for (int i = barn_line - 5; i < Last_row; i++)
        {
#ifdef TL2barn
            if (Lef[i] - Fir_col < 5 && Lef[i + 1] - Fir_col < 5 && Lef[i + 2] - Fir_col < 5)
#endif
#ifdef TR2barn
                if (Last_col - Rig[i] < 5 && Last_col - Rig[i + 1] < 5 && Last_col - Rig[i + 2] < 5)
#endif
                {
                    barn_line = ((i - 2) > Fir_row + 5 ? (i - 2) : Fir_row + 5);
                    break;
                }
        }
        if (barn_line < 26)
        {
            Road7_flag = 0; //等待转弯，远景行补线
        }
        else if (barn_line < 37)
        {
            Road7_flag = 1;
        }
        else
        {
            Road = 7;
            Road7_flag = 2; //开始转弯
        }
    }
    else if (Road7_flag == 2) //开始转弯
    {
        //Road73_count++;
#ifdef TL2barn
        Servo_Duty(SERVO_RANGE);
#endif
#ifdef TR2barn
        Servo_Duty(-SERVO_RANGE);
#endif
        // if (Road73_count >= (int)(10 * 100 / (get_speed() * CAMERA_FPS)) + 1)
        // {
        // Road7_flag = 3;
        // }
        if (Lef_slope == 998 && Rig_slope == 998 && (Lef_edge < 10 && Rig_edge < 10) || (EM_Value_1 < 0.2 && EM_Value_2 < 0.15 && EM_Value_2 < 0.15 && EM_Value_4 < 0.2))
        {
            Road73_count++;
            if (Road73_count > 1)
            {
                Road73_count = 0;
                Road7_flag = 3;
            }
        }
    }
    else if (Road7_flag == 3) //转弯结束停车
    {
        Road73_count = 0;
        // Road74_count++;
        for (int i = Last_row; i > stop_line - 5; i--)
        {
            if (Pixle[i][8] + Pixle[i][72] + Pixle[i][30] + Pixle[i][35] + Pixle[i][40] + Pixle[i][45] + Pixle[i][50] < 3)
            {
                Black_line++;
                if (Black_line > 2)
                {
                    stop_line = i - 2;
                    if (stop_line > 35) // || Road74_count >= (int)(30 * 100 / (get_speed() * CAMERA_FPS)) + 1)
                    {
                        Road74_count++;
                        if (Road74_count > 1)
                        {
                            Road7_flag = 4; //停车
                            Road74_count = 0;
                            SpeedGoal = 0;
                            stop_line = Fir_row;
                        }
                    }
                    break;
                }
            }
            else
            {
                Black_line = 0;
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
}
