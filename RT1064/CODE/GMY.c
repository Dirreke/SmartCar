#include "headfile.h"
#include "math.h"

/*************************************************************************
 *  函数名称：void Pic_DrawMid_und(void)
 *  功能说明：计算去畸变后中线
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020.1.18
 *  备    注：Cam_offset为负，车偏右，向左拐；
             Cam_offset为正，车偏左，向右拐；
 * ************************************************************************/
int New_Mid[60];

void Pic_DrawMid_und(void)
{
    int i, j;
    int inter_point = 0;
    int flag = 0;
    int inter_count = 0;
    float slope_temp;

    if ((Road0_flag == 3 && Road == 0) || Road == 1)
    {
        for (i = 0; i < 60; i++)
        {

            if (New_Rig[i] != MIDMAP)
            {
                New_Mid[i] = New_Rig[i] - ROAD_HALF_WIDTH;

                if (flag == 0 || inter_count == 0)
                {
                    inter_count = 1; //第一个点直接continue，flag==0意味着每断点直接continue
                    continue;
                }
            }
            else
            {
                inter_point++; //for 插值
                if (flag == 1)
                {
                    if (i = 59)
                    {
                        for (j = 0; j < 60; j++)
                        {
                            New_Mid[j] = 999;
                        }
                    }
                    continue;
                }
                if (i == 0) //开头断点，下一个有值点后插值
                {
                    flag = 1;
                    continue;
                }
                else if (i != 59) //中间断点，直接进插值
                {
                    flag = 2;
                    continue;
                }
                else //结尾断点
                {
                    flag = 3;
                }
            }

            if (flag == 2) //插值 内插
            {
                slope_temp = (New_Mid[i] - New_Mid[i - inter_point - 1]) / (inter_point + 1);
                for (j = inter_point; j > 0; j--)
                {
                    New_Mid[i - j] = (int)(New_Mid[i] - slope_temp * j + 0.5);
                }
                flag = 0;
            }
            else if (flag == 3) //外推
            {
                slope_temp = New_Mid[i - inter_point] - New_Mid[i - inter_point - 1];
                for (j = inter_point - 1; j >= 0; j--)
                {
                    New_Mid[i - j] = (int)(New_Mid[i - inter_point] + slope_temp * (inter_point - j) + 0.5);
                }
                flag = 0;
            }
            else if (flag == 1) //外推
            {
                slope_temp = New_Mid[i] - New_Mid[i - 1];
                for (j = inter_point + 1; j > 1; j--)
                {
                    New_Mid[i - j] = (int)(New_Mid[i] - slope_temp * j + 0.5);
                }
                flag = 0;
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
                if (flag == 0 || inter_count == 0)
                {
                    inter_count = 1;
                    continue;
                }
            }

            else
            {
                inter_point++; //for 插值
                if (flag == 1)
                {
                    if (i = 59)
                    {
                        for (j = 0; j < 60; j++)
                        {
                            New_Mid[j] = 999;
                        }
                    }
                    continue;
                }
                if (i == 0) //开头断点，下一个有值点后插值
                {
                    flag = 1;
                    continue;
                }
                else if (i != 59) //中间断点，直接进插值
                {
                    flag = 2;
                    continue;
                }
                else //结尾断点
                {
                    flag = 3;
                }
            }

            if (flag == 2) //插值
            {
                slope_temp = (New_Mid[i] - New_Mid[i - inter_point - 1]) / (inter_point + 1);
                for (j = inter_point; j > 0; j--)
                {
                    New_Mid[i - j] = (int)(New_Mid[i] - slope_temp * j + 0.5);
                }
                flag = 0;
            }
            else if (flag == 3)
            {
                slope_temp = New_Mid[i - inter_point] - New_Mid[i - inter_point - 1];
                for (j = inter_point - 1; j >= 0; j--)
                {
                    New_Mid[i - j] = (int)(New_Mid[i - inter_point] + slope_temp * (inter_point - j) + 0.5);
                }
                flag = 0;
            }
            else if (flag == 1)
            {
                slope_temp = New_Mid[i] - New_Mid[i - 1];
                for (j = inter_point + 1; j > 1; j--)
                {
                    New_Mid[i - j] = (int)(New_Mid[i] - slope_temp * j + 0.5);
                }
                flag = 0;
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
                if (flag == 0 || inter_count == 0)
                {
                    inter_count = 1;
                    continue;
                }
            }
            else if (New_Lef[i] == -MIDMAP && New_Rig[i] != MIDMAP)
            {
                New_Mid[i] = New_Rig[i] - ROAD_HALF_WIDTH;
                if (flag == 0 || inter_count == 0)
                {
                    inter_count = 1;
                    continue;
                }
            }
            else if (New_Lef[i] != -MIDMAP && New_Rig[i] == MIDMAP)
            {
                New_Mid[i] = New_Lef[i] + ROAD_HALF_WIDTH;
                if (flag == 0 || inter_count == 0)
                {
                    inter_count = 1;
                    continue;
                }
            }
            else
            {
                inter_point++; //for 插值
                if (flag == 1)
                {
                    if (i = 59)
                    {
                        for (j = 0; j < 60; j++)
                        {
                            New_Mid[j] = 999;
                        }
                    }
                    continue;
                }
                if (i == 0) //开头断点，下一个有值点后插值
                {
                    flag = 1;
                    continue;
                }
                else if (i != 59) //中间断点，直接进插值
                {
                    flag = 2;
                    continue;
                }
                else //结尾断点
                {
                    flag = 3;
                }
            }

            if (flag == 2) //插值
            {
                slope_temp = (New_Mid[i] - New_Mid[i - inter_point - 1]) / (inter_point + 1);
                for (j = inter_point; j > 0; j--)
                {
                    New_Mid[i - j] = (int)(New_Mid[i] - slope_temp * j + 0.5);
                }
                flag = 0;
            }
            else if (flag == 3)
            {
                slope_temp = New_Mid[i - inter_point] - New_Mid[i - inter_point - 1];
                for (j = inter_point - 1; j >= 0; j--)
                {
                    New_Mid[i - j] = (int)(New_Mid[i - inter_point] + slope_temp * (inter_point - j) + 0.5);
                }
                flag = 0;
            }
            else if (flag == 1)
            {
                slope_temp = New_Mid[i] - New_Mid[i - 1];
                for (j = inter_point + 1; j > 1; j--)
                {
                    New_Mid[i - j] = (int)(New_Mid[i] - slope_temp * j + 0.5);
                }
                flag = 0;
            }
        }
    }
    return;
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
/*************************************************************************
 *  函数名称：void Pic_DrawMid_und(void)
 *  功能说明：计算去畸变后中线无插值
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020.5.31
 *  备    注：
 * ************************************************************************/
int New_Mid[60];

void Pic_DrawMid_und(void)
{
    int i, j;
    int inter_point = 0;
    int flag = 0;
    int inter_count = 0;
    float slope_temp;

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
>>>>>>> 222cb59... 0531-GMY_chongxiehenduohanshu_jiaruzuixiaoerchengzhuanwan

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

>>>>>>> 0501c7b... GMY -zhaohui diushi daima
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
#define CAR_LENGTH 148.5149
float car_straight(float car_dias)
{
    int i;
    float x2_temp;
    float sum1 = 0;
    float sum2 = 0;
    float temp_sin;
    float angle_val1;
    // static const float y2_origin[58] = {467856, 451584, 435600, 419904, 404496, 389376, 374544, 360000, 345744, 331776, 318096, 304704, 291600, 278784, 266256, 254016, 242064, 230400, 219024, 207936, 197136, 186624, 176400, 166464, 156816, 147456, 138384, 129600, 121104, 112896, 104976, 97344, 90000, 82944, 76176, 69696, 63504, 57600, 51984, 46656, 41616, 36864, 32400, 28224, 24336, 20736, 17424, 14400, 11664, 9216, 7056, 5184, 3600, 2304, 1296, 576, 144, 0};
    // //认为控制行高（近）行为第0行，成比例反转，即y2_origin=((control_line_min-y)*12)^2，暂定控制行3-57。
    static const float y2_origin[60] = {55696, 53824, 51984, 50176, 48400, 46656, 44944, 43264, 41616, 40000, 38416, 36864, 35344, 33856, 32400, 30976, 29584, 28224, 26896, 25600, 24336, 23104, 21904, 20736, 19600, 18496, 17424, 16384, 15376, 14400, 13456, 12544, 11664, 10816, 10000, 9216, 8464, 7744, 7056, 6400, 5776, 5184, 4624, 4096, 3600, 3136, 2704, 2304, 1936, 1600, 1296, 1024, 784, 576, 400, 256, 144, 64, 16, 0};
    // //认为控制行高（近）行为第0行，成比例反转，即y2_origin=((control_line_min-y)*4)^2，暂定控制行3-57。

    for (i = 3; i < 58; i++)
    {
        if (New_Mid != 999)
        {
            x2_temp = ((New_Mid[i] - car_dias) / UNDISTORT_XPK) ^ 2;
            sum1 += x2_temp;
            sum2 += (x2_temp + y2_origin[i]) * New_Mid[i];
        }
    }
    if (sum2 == 0)
    {
        angle_val1 = 0;
    }
    else
    {
        temp_sin = sum1 / sum2 * 2 * CAR_LENGTH * UNDISTORT_PWK;
        temp_sin = limit_f(temp_sin, -1, 1);
        angle_val1 = asin(temp_sin);
    }
    return -angle_val1;
}
<<<<<<< HEAD
=======

<<<<<<< HEAD


>>>>>>> 0501c7b... GMY -zhaohui diushi daima
=======
>>>>>>> 222cb59... 0531-GMY_chongxiehenduohanshu_jiaruzuixiaoerchengzhuanwan
/************************************************************************
 * search.c 1459-
 * 修改，减少取景行
 * 增加参数startpoint便于调整
 * 
 * **********************************************************************/
#define UNDISTORT_PYK 4
#define UNDISTORT_XPK 1.235946609885535
#ifdef undistort1
/*************************************************************************
 *  函数名称：void Pic_undistort(int L, int R)
 *  功能说明：图像去畸变
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2019.12.28
 *  备    注：对Lef、Rig进行映射处理
 *            12为参数！！！
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
    int temp;
    int step;
    int Rig_New[60];
    int Lef_New[60];
    ,
        static const int tempNewy[60] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1188, 807, 602, 475, 387, 324, 276, 238, 208, 182, 161, 144, 128, 115, 103, 93, 84, 75, 68, 61, 55, 49, 44, 39, 35, 31, 27, 24, 20, 17, 14, 12, 9, 7, 4, 2, 0, 0};

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
                tempNewxR[i] = (int)((UNDISTORT_XPK*(UNDISTORT_D * UNDISTORT_C + UNDISTORT_H) * 2 * (tempx - 39.5) / (-UNDISTORT_S * (29.5 - tempy) + UNDISTORT_D * UNDISTORT_C) + 0.5);

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
                    Rig_New[59 - j] = (tempNewxR[i] - tempNewxR[i + 1]) / step * (temp - tempNewy[i + 1]) + tempNewxR[i + 1];
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
                    Lef_New[59 - j] = (tempNewxL[i] - tempNewxL[i + 1]) / step * (temp - tempNewy[i + 1]) + tempNewxL[i + 1];
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

//起跑线
int start_waited = 0;
int start_stop_line = 0;
void start_stop_rec(void)
{
    int i;
    int stop_line = 59;
    int Black_line = 0;
    int start_stop_line_flag;
    if (Road == 0 && Road0_flag != 0 && Road0_flag != 1) //如果前40行找不到边线，认为是断路或者起跑线
    {
        for (i = Fir_row; i < 60; i++) //自上而下寻找有边线的开始行
        {
            if (Lef[i] == 1 && Rig[i] == 78)
            {
                continue;
            }
            else
            {
                stop_line = i;
                break;
            }
        }
        start_waited++;
        if (start_waited > 600)
        {
            start_waited = 601;
            if (stop_line < 20 && Lef_slope != 999 && Rig_slope != 999)
            {
                start_stop_line_flag = 0;
                for (i = Fir_row + 5; i < 50; i++)
                {
                    if (Lef[i] == 1 && Rig[i] == 78)
                    {
                        if (start_stop_line_flag == 0)
                        {
                            start_stop_line = i;
                            start_stop_line_flag = 1;
                        }
                        if (Pixle[i][8] + Pixle[i][72] + Pixle[i][30] + Pixle[i][35] + Pixle[i][40] + Pixle[i][45] + Pixle[i][50] < 2)
                        {
                            Black_line++;
                        }
                        if (Black_line > 3)
                        {
                            Road = 7;
                            return;
                        }
                    }
                    else
                    {
                        start_stop_line_flag = 0;
                        Black_line = 0;
                    }
                }
            }
        }
    }
    else if (Road == 7)
    {
        start_stop_line_flag = 0;
        for (i = start_stop_line; i < 50; i++)
        {
            if (Lef[i] == 1 && Rig[i] == 78)
            {
                if (start_stop_line_flag == 0)
                {
                    start_stop_line = i;
                    start_stop_line_flag = 1;
                }
                if (Pixle[i][8] + Pixle[i][72] + Pixle[i][30] + Pixle[i][35] + Pixle[i][40] + Pixle[i][45] + Pixle[i][50] < 2)
                {
                    Black_line++;
                }
                if (Black_line > 3)
                {
                    return;
                }
            }
            else
            {
                start_stop_line_flag = 0;
                Black_line = 0;
            }
        }
    }
}

//起跑线2.
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
    static int start_stop_line = 0;
    static int start_waited = 0;
    static int turn_cnt = 0, stop_cnt = 0;
    static int stop_line = Fir_row;
    int stop_line_cnt = 0, stop_black_cnt = 0, stop_all_cnt = 0, stop_flag_cnt = 0, count = 0;
    int i, j;
    int avr_mid = 0;
    int Black_line = 0;
    int road_half_width_original[40] = {35, 34, 34, 33, 32, 31, 30, 30, 29, 28, 27, 26, 25, 25, 24, 23, 22, 21, 20, 20, 19, 18, 17, 16, 15, 14, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 0};

    if (Road == 0 && Road0_flag != 1 && Road0_flag != 2) //进起跑线
    {
        start_waited++;
        if (start_waited > 600 && Lef_edge > 15)
        {
            for (i = Fir_row + 10; i < 60; i++) //自上而下寻找有边线的开始行
            {
                if (Lef[i] > Fir_col)
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
                for (i = start_stop_line - 5; i < start_stop_line - 10; --i) //计算平均中间位置
                {
                    if (Lef[i] > Fir_col && Rig[i] < Last_col)
                    {
                        avr_mid += (int)((Lef[i] + Rig[i]) / 2.0 + 0.5);
                    }
                    else if (Lef[i] <= Fir_col && Rig[i] < Last_col)
                    {
                        avr_mid += Rig[i] - road_half_width_original[Last_row - i];
                    }
                    else if (Lef[i] > Fir_col && Rig[i] >= Last_col)
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

                for (i = start_stop_line; i <= Last_row; ++i) //搜黑白线
                {
                    for (j = avr_mid - road_half_width_original[Last_row - i]; j <= avr_mid + road_half_width_original[Last_row - i]; ++j)
                    {
                        if (Pixle[i][j] == 0)
                        {
                            stop_black_cnt++;
                        }
                        stop_all_cnt++;
                    }
                    if (stop_black_cnt > stop_all_cnt / 2)
                    {
                        stop_flag_cnt += 0.35;
                    }
                    else if (stop_black_cnt > stop_all_cnt / 3)
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
    else if (Road == 7 & Road7_flag == 0) //等待转弯
    {
        for (i = start_stop_line; i < Last_row; i++)
        {
            if (Lef[i] > Fir_col)
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
        if (start_stop_line > 35)
        {
            Road = 7;
            Road7_flag = 1; //开始转弯
        }
    }
    else if (Road7_flag == 1) //开始转弯
    {
        turn_cnt++;
        Servo_Duty(-SERVO_RANGE);
        if (turn_cnt >= (int)(5 * 100 / (get_speed() * CAMERA_FPS)) + 1)
        {
            Road7_flag = 2;
        }
    }
    else if (Road7_flag == 2) //转弯结束停车
    {
        stop_cnt++;
        stop_line = Fir_row;
        for (i = Last_row; i < stop_line; i++)
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
    int i = 0, j = 0, k = 0;
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
        for (j = Middle; j < Last_col + 1; j++) //末行处理
        {
            if (Pixle[i][j] == 1 && Pixle[i][j - 1] == 1 && Pixle[i][j - 2] == 1 && Pixle[i][j - 3] == 1 && Pixle[i][j - 4] == 1 && Pixle[i][j - 5] == 1 && Pixle[i][j - 6] == 1 && Pixle[i][j + 1] == 0 && Pixle[i][j + 2] == 0)
            {
                Rig[i] = j;
                break;
            }
        }
        for (j = Middle; j > Fir_col - 1; j--)
        {
            if (Pixle[i][j] == 1 && Pixle[i][j + 1] == 1 && Pixle[i][j + 2] == 1 && Pixle[i][j + 3] == 1 && Pixle[i][j + 4] == 1 && Pixle[i][j + 5] == 1 && Pixle[i][j + 6] == 1 && Pixle[i][j - 1] == 0 && Pixle[i][j - 2] == 0)
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
    for (i; i > Fir_row - 1; i--) //从底层向上绘线
    {
        search_flag1 = 0, search_flag2 = 0;
        Side_flag = 0;

        if (Rig[i + 1] != 78)
        {
            if (Pixle[i][Rig[i + 1]] == 0 || (Pixle[i][Rig[i + 1]] == 1 && Pixle[i][Rig[i + 1] - 1] == 0 && Pixle[i][Rig[i + 1] + 1] == 0))//向内查找10个
            {
                for (j = Rig[i + 1]; j > Rig[i + 1] - 10 && j > Lef[i + 1] + 5; j--)
                {
                    if (Pixle[i][j - 1] == 1 && Pixle[i][j - 2] == 1)//两白
                    {
                        Rig[i] = j;
                        Side_flag = 1;
                        break;
                    }
                }
                search_flag1 = 1;
            }
            else if (Pixle[i][Rig[i + 1] + 1] == 1)//向外查找8个
            {
                for (j = Rig[i + 1] + 1; j < Rig[i + 1] + 8 && j < Last_col + 1; j++)
                {
                    if (Pixle[i][j + 1] == 0 && Pixle[i][j + 2] == 0)                     //两黑
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
            for (j = Rig[i + 2]; j > Rig[i + 1] - 10 && j > Lef[i + 1] + 5; j--)  //先搜内10个
            {
                if (Pixle[i][j - 1] == 1 && Pixle[i][j - 2] == 1 && Pixle[i][j - 3] == 1 && Pixle[i][j - 4] == 1)
                {
                    Rig[i] = j;
                    Side_flag = 1;
                    break;
                }
                search_flag1 = 1;
            }
            if (Side_flag == 0 && Pixle[i][Rig[i + 2] - 2] == 1 && Pixle[i][Rig[i + 2] - 1] == 1 && Pixle[i][Rig[i + 2]] == 1 && Pixle[i][Rig[i + 2] + 1] == 1)//搜外8个
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
        search_flag1 = 0, search_flag2 = 0;

        if (Lef[i + 1] != 78)
        {
            if (Pixle[i][Lef[i + 1]] == 0 || (Pixle[i][Lef[i + 1]] == 1 && Pixle[i][Lef[i + 1] - 1] == 0 && Pixle[i][Lef[i + 1] - 1] == 0))
            {
                for (j = Lef[i + 1]; j < Lef[i + 1] + 10 && j < Rig[i + 1] - 5; j++)
                {
                    if (Pixle[i][j + 1] == 1 && Pixle[i][j + 2] == 1)
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
        else if (Lef[i + 2] != 78)
        {
            for (j = Lef[i + 2]; j < Lef[i + 1] + 10 && j < Rig[i + 1] + 5; j++)
            {
                if (Pixle[i][j + 1] == 1 && Pixle[i][j + 2] == 1 && Pixle[i][j + 3] == 1 && Pixle[i][j + 4] == 1)
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
            for (j = Lef[i + 1] - 8; j > Fir_col - 1; j++)
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

void Turn_Cam_New()
{
    float car_center_dias;
    float car_straight_angle;
    int car_center_PWM;
    int car_straight_PWM;
    car_center_dias = car_center();
    car_straight_angle = Circle_R_calculate(car_center_dias);
    car_center_PWM = PID_realize_center(car_center_dias);
    car_straight_PWM = PID_realize_straight(car_straight_angle * SERVO_RANGE / ANGLE_RANGE);
    Turn_Cam_Out = car_center_PWM + car_straight_PWM;
    Servo_Duty(-Turn_Cam_Out);
}