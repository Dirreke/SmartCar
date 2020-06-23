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
#if 0
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
#endif

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

    // static const float y2_origin[60] = {55696, 53824, 51984, 50176, 48400, 46656, 44944, 43264, 41616, 40000, 38416, 36864, 35344, 33856, 32400, 30976, 29584, 28224, 26896, 25600, 24336, 23104, 21904, 20736, 19600, 18496, 17424, 16384, 15376, 14400, 13456, 12544, 11664, 10816, 10000, 9216, 8464, 7744, 7056, 6400, 5776, 5184, 4624, 4096, 3600, 3136, 2704, 2304, 1936, 1600, 1296, 1024, 784, 576, 400, 256, 144, 64, 16, 0}
    // // //认为控制行高（近）行为第0行，成比例反转，即y2_origin=((control_line_min-y)*4)^2，暂定控制行3-57。

    static const float y2_origin[60] = {8911.36, 8611.84, 8317.44, 8028.16, 7744, 7464.96, 7191.04, 6922.24, 6658.56, 6400, 6146.56, 5898.24, 5655.04, 5416.96, 5184, 4956.16, 4733.44, 4515.84, 4303.36, 4096, 3893.76, 3696.64, 3504.64, 3317.76, 3136, 2959.36, 2787.84, 2621.44, 2460.16, 2304, 2152.96, 2007.04, 1866.24, 1730.56, 1600, 1474.56, 1354.24, 1239.04, 1128.96, 1024, 924.16, 829.44, 739.84, 655.36, 576, 501.76, 432.64, 368.64, 309.76, 256, 207.36, 163.84, 125.44, 92.16, 64, 40.96, 23.04, 10.24, 2.56, 0};
    // // 即y2_origin=(y*1.6)^2。
    // static const float y2_origin[60] = {23839.36,23347.84,22861.44,22380.16,21904,21432.96,20967.04,20506.24,20050.56,19600,19154.56,18714.24,18279.04,17848.96,17424,17004.16,16589.44,16179.84,15775.36,15376,14981.76,14592.64,14208.64,13829.76,13456,13087.36,12723.84,12365.44,12012.16,11664,11320.96,10983.04,10650.24,10322.56,10000,9682.56,9370.24,9063.04,8760.96,8464,8172.16,7885.44,7603.84,7327.36,7056,6789.76,6528.64,6272.64,6021.76,5776,5535.36,5299.84,5069.44,4844.16,4624,4408.96,4199.04,3994.24,3794.56,3600}
    // // // 即y2_origin=(y*1.6+车中心)^2。认为第一行距车中心20cm,,即60像素。

    for (i = 3; i < 58; i++)
    {
        if (New_Mid != 999)
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
        temp_sin = limit_f(temp_sin, -1, 1);
        angle_val1 = asin(temp_sin);
    }
    return -angle_val1;
}

#if 0 /************************************************************************ \
       * search.c 1459-                                                         \
       * 修改，减少取景行                                               \
       * 增加参数startpoint便于调整                                     \
       *                                                                        \
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
#endif

#if 0
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
#endif
#if 0
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
            if (Pixle[i][Rig[i + 1]] == 0 || (Pixle[i][Rig[i + 1]] == 1 && Pixle[i][Rig[i + 1] - 1] == 0 && Pixle[i][Rig[i + 1] + 1] == 0)) //向内查找10个
            {
                for (j = Rig[i + 1]; j > Rig[i + 1] - 10 && j > Lef[i + 1] + 5; j--)
                {
                    if (Pixle[i][j - 1] == 1 && Pixle[i][j - 2] == 1) //两白
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
                if (Pixle[i][j - 1] == 1 && Pixle[i][j - 2] == 1 && Pixle[i][j - 3] == 1 && Pixle[i][j - 4] == 1)
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

#endif

void Turn_Cam_New()
{
    float car_center_dias;
    float car_straight_angle;
    int car_center_PWM;
    int car_straight_PWM;
    car_center_dias = car_center();
    car_straight_angle = car_straight(car_center_dias);
    car_center_PWM = PID_realize_center(car_center_dias);
    car_straight_PWM = PID_realize_straight(car_straight_angle * SERVO_RANGE / ANGLE_RANGE);
    Turn_Cam_Out = car_center_PWM + car_straight_PWM;
    Servo_Duty(-Turn_Cam_Out);
}
#if 0
int turn_stop=0;

void part()
{
    int dis=0;int dis1=0;
    int i=0;
    if (0)
    {
        ;
    }
    else if ((Rig_break_point > 45 && ((Lef_circle == 1 && Rig_circle == 0) || Road0_flag == 3) && Rig_slope != 998)) //左转弯
    {
        for (i = Fir_row; i < 40; ++i)
        {
            if (Rig[i] < 40 && Rig[i + 1] < 40 && Rig[i + 2] > 40 && Rig[i + 3] > 40 &&
                Rig[i + 5] - Rig[i + 3] < 5 && Rig[i + 7] - Rig[i + 5] < 5 && Rig[i + 9] - Rig[i + 7] < 5 && Rig[i + 11] - Rig[i + 9] < 5)
            {
                break;
            }
        }
        dis = Rig[i + 1] - Rig[i];
        for (i; i > Fir_row; --i)
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
            else if (dis1 < 2 * dis)
            {
                dis = Rig[i] - Rig[i - 1];
                continue;
            }
            else
            {
                break;
            }
        }
        turn_stop = i;
        if (Rig[turn_stop] < 34 && dis > 4)
        {
            Road03_count++;
            if (Road03_count == 2)
            {
                Road0_flag = 3;   //turn left flag
                Road03_count = 0; //reset
            }
            return;
        }
    }
    else if ((Lef_break_point > 45 && ((Rig_circle == 1 && Lef_circle == 0) || Road0_flag == 4) && Lef_slope != 998)) //右转弯
    {
        for (i = Fir_row; i < 40; ++i)
        {
            if (Lef[i] > 40 && Lef[i + 1] > 40 && Lef[i + 2] < 40 && Lef[i + 3] < 40 &&
                Lef[i + 3] - Lef[i + 5] < 5 && Lef[i + 5] - Lef[i + 7] < 5 && Lef[i + 7] - Lef[i + 9] < 5 && Lef[i + 9] - Lef[i + 11] < 5)
            {
                break;
            }
        }
        dis = Lef[i] - Lef[i + 1];
        for (i; i > Fir_row; --i)
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
            else if (dis1 < 2 * dis)
            {
                dis = Lef[i - 1] - Lef[i];
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
            Road04_count++;
            if (Road04_count == 2)
            {
                Road0_flag = 4;   //turn left flag
                Road04_count = 0; //reset
            }
            return;
        }
    }
}
#endif
#if 0
/*************************************************************************
*  函数名称：void start_stop_rec(void)
*  功能说明：起跑线识别与等待
*  参数说明：无
*  函数返回：无
*  修改时间：2020.06.17
*  备    注：
*************************************************************************/
void start_stop_rec(int TL, int TR)
{
    static int start_waited = 0;
    static int turn_cnt = 0, stop_cnt = 0;
    static int stop_line = Fir_row;
    float stop_flag_cnt = 0;
    int stop_line_cnt = 0, stop_black_cnt = 0, stop_all_cnt = 0, count = 0;
    int j, temp;
    int avr_mid = 0;
    int Black_line = 0;
    int road_half_width_original[40] = {40, 40, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4};
    int Rig_end, Lef_end, Road_end;

    if (Road == 0 && Road0_flag == 0)
    {
        start_waited = 601;
        if (start_waited == 601)
        {
            start_waited = 601;
        }
        else
        {
            return;
        }

        for (int i = temp; i < Last_row; ++i)
        {
            if (Rig[i] <= 40 || Rig[i + 1] <= 40)
            {
                continue;
            }
        }
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
}
/*************************************************************************
*  函数名称：void start_stop_rec(void)
*  功能说明：起跑线识别与等待
*  参数说明：无
*  函数返回：无
*  修改时间：2020.06.17
*  备    注：
*************************************************************************/
int start_stop_line_flag = 0;
void start_stop_find(void)
{
    static int start_waited = 0;
    int tiaobian1 = 0;

    int Rig_end, Lef_end, Road_end;

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
    for (int i = Fir_row; i < Last_row - 4; ++i)
    {
        if (Rig[i] > Last_col)
        {
            continue;
        }
        if (Rig[i] > 40 && Rig[i + 2] - Rig[i] < 5 && Rig[i + 3] - Rig[i + 1] < 5 && Rig[i + 4] - Rig[i + 2] < 5)
        {
            Rig_end = i;
            break;
        }
    }
    for (int i = Fir_row; i < Last_row - 4; ++i)
    {
        if (Lef[i] < Fir_col)
        {
            continue;
        }
        if (Lef[i] < 40 && Lef[i] - Lef[i + 2] < 5 && Lef[i + 1] - Lef[i + 3] < 5 && Lef[i + 2] - Lef[i + 4] < 5)
        {
            Lef_end = i;
            break;
        }
    }
    Road_end = (Lef_end > Rig_end ? Lef_end : Rig_end);
    if (Road_end < 30)
    {
        return;
    }
#ifdef TL2barn
    for (int i = Rig_end; i < Last_row - 7; ++i)
    {
        if (Rig[i] > 40 && Rig[i + 2] - Rig[i] < 5 && Rig[i + 3] - Rig[i + 1] < 5 && Rig[i + 4] - Rig[i + 2] < 5)
        {
            continue;
        }
        if (Rig[i + 7] - Rig[i + 3] > 25)
        {
            tiaobian1 = i;
            break;
        }
    }
    if (tiaobian1)
    {
        for (int i = tiaobian1; i < Last_row - 4; ++i)
        {
            if (Lef[i] - Fir_col > 5)
            {
                return;
            }
            if (Rig[i] > 40 && Rig[i + 2] - Rig[i] < 5 && Rig[i + 3] - Rig[i + 1] < 5 && Rig[i + 4] - Rig[i + 2] < 5)
            {
                ;
            }
            else
            {
                continue;
            }

            if (Rig[i] - Rig[i - 3] > 25)
            {
                start_stop_line_flag = 1;
                start_stop_line = tiaobian1;
                break;
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
        if (Lef[i] < 40 && Lef[i] - Lef[i + 2] < 5 && Lef[i + 1] - Lef[i + 3] < 5 && Lef[i + 2] - Lef[i + 4] < 5)
        {
            continue;
        }
        if (Lef[i + 3] - Lef[i + 7] > 25)
        {
            tiaobian1 = i;
            break;
        }
    }
    if (tiaobian1)
    {
        for (int i = tiaobian1; i < Last_row - 4; ++i)
        {
            if (Last_col - Rig[i] > 5)
            {
                return;
            }
            if (Lef[i] < 40 && Lef[i] - Lef[i + 2] < 5 && Lef[i + 1] - Lef[i + 3] < 5 && Lef[i + 2] - Lef[i + 4] < 5)
            {
                ;
            }
            else
            {
                continue;
            }

            if (Lef[i - 3] - Lef[i] > 25)
            {
                start_stop_line_flag = 1;
                start_stop_line = tiaobian1;
                break;
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
int barn_line = 0;
void part(void)
{
    static int Road70_count = 0;
    if (Road == 0 && Road0_flag == 0)
    {

        if (start_stop_line_flag = 1)
        {
            Road70_count++;
            if (Road70_count > 2)
            {
                Road70_cnt = 0;
                Road = 7;
                Road7_flag = 0;
                for (int i = start_stop_line - 10; i > Last_col - 2; ++i)
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
                return;
            }
        }
    }
}
#endif
#if 0
void Road7_zhuangtaiji(void)
{
    static int Road73_count = 0;
    static int Road74_count = 0;
    if (Road7_flag == 0 || Road7_flag == 1) //等待转弯
    {
        for (int i = barn_line - 10; i < Last_row; i++)
        {
#ifdef TL2barn
            if (Lef[i] - Fir_col < 5 && Lef[i + 1] - Fir_col < 5 && Lef[i + 2] - Fir_col < 5)
#endif
#ifdef TR2barn
                if (Last_col - Rig[i] < 5 && Last_col - Rig[i + 1] < 5 && Last_col - Rig[i + 2] < 5)
#endif
                {
                    barn_line = i - 2;
                    break;
                }
        }
        if (barn_line < 30)
        {
            Road7_flag = 0; //等待转弯，远景行补线
        }
        else if (barn_line < 40)
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
        Road73_count++;
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
        if (Lef_sope = 998 &&Rig_slope = 998)
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
        stop_line = Fir_row;
        for (int i = Last_row; i > stop_line; i--)
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
                            lib_set_speed(0);
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
#endif
#if 0
    void Road1_zhuangtaiji(void)
    {
        static int Road11_count = 0, Road12_count = 0, Road13_count = 0, Road14_count = 0, Road15_count = 0, Road16_count = 0;
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
            if (Lef_circle == 0 || (Lef_circle == 1 && Lef_break_point > 45)) //if(((Lef_circle==0||( Lef_circle ==1 && Lef_break_point>30)))&& Road1_turnin(EM_Value_2,EM_Value_3,3.8))//if(((Lef_circle==0||( Lef_circle ==1 && Lef_break_point>30))) && Rig_slope>=10)/ && Road1_turnin(EM_Value_2,EM_Value_3,3.8))    //Rig_slope<1 && (Lef_leap[0]==0||Lef_slope==999)&& Rig_leap[0]==0)
            {
                Road12_count++;
                if (Road12_count > 1) //2帧后 进左圆环第一弯道
                {
                    Road1_flag = 2;
                    Road12_count = 0;
                }
            }
        }
        else if (Road1_flag == 2) //进左圆环2/4 开始补线进弯道
        {
            Road14_count++;
            if (Road14_count == (int)(DIS_IN_CIRCLE * 10000 / (get_speed() * CAMERA_FPS)) + 1) //宏定义在function.h
            {
                Road1_flag = 4;
                Road14_count = 0;
            }
        }
        else if (Road1_flag == 4) //进入圆环内 ，取消补线
        {
            dis = Rig[40 + 1] - Rig[40];
            for (int i = 40; i > Fir_row; --i)
            {
                dis1 = Rig[i] - Rig[i - 1];
                if (dis1 < 0)
                {
                    turn_stop = i break;
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
                    turn_stop = i break;
                }
            }
            if (Rig_circle && whitecnt > 1200) //Rig_circlr 不好，改!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            {
                Road13_count++;
                if (Road13_count == 3)
                {
                    Road1_flag = 3;
                    Road13_count = 0;
                }
            }
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
            // if ((Rig_slope > -0.02 && Rig_slope < 0) || (Pixle[58][74] == 1 && Pixle[57][74] == 1 && Pixle[56][74] == 1 && Pixle[55][74] == 1 && Pixle[54][74] == 1 && Pixle[53][74] == 1)) //|| Lef_edge < 20))
            if (((Rig_slope < -0.1 || Rig_slope == 998) && Allwhiteend > 45) || Allwhiteend == Fir_row)
            {
                Road16_count++;
                if (Road16_count == 4)
                {
                    Road16_count = 0;
                    Road = 0;
                    Road0_flag = 0;
                    Road1_flag = 0;
                }
            }
        }
        return;
    }

void Road2_zhuagntaiji(void)
{
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
        if ((Rig_circle == 0 || (Rig_circle == 1 && Rig_break_point > 45)) && Rig_slope >= 10) //if(((Rig_circle==0||( Rig_circle ==1 && Rig_break_point>30))) && Rig_slope>=10)/ && Road1_turnin(EM_Value_2,EM_Value_3,3.4))
        {
            Road22_count++;
            if (Road22_count > 1) //
            {
                Road2_flag = 2;
                Road22_count = 0;
            }
        }
        else
        {
            Road22_count = 0;
        }
    }
    else if (Road2_flag == 2) //
    {
        Road24_count++;
        if (Road24_count == (int)(DIS_IN_CIRCLE * 10000 / (get_speed() * CAMERA_FPS)) + 1)
        {
            Road24_count = 0;
            Road2_flag = 4;
        }
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

        if (Lef_circle && whitecnt > 1200)
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

#endif

#if 0 
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

void TurnLeft_Process(void)
{
    static int turn_stop_flag = 0, Road04_count = 0;
    int temp = 40;
    int dis = 0, dis1 = 0;
    for (int i = Fir_row; i < 40; ++i)
    {
        if (Rig[i] < 40 && Rig[i + 1] <= 40 && Rig[i + 2] >= 40 && Rig[i + 3] > 40 &&
            Rig[i + 5] - Rig[i + 3] < 7 && Rig[i + 7] - Rig[i + 5] < 7 && Rig[i + 9] - Rig[i + 7] < 7 && Rig[i + 11] - Rig[i + 9] < 7)
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


void TurnRight_process()
{
    static int turn_stop_flag = 0, Road05_count = 0,Road00_count = 0;
    int temp = 40;
    int dis = 0, dis1 = 0;
    for (int i = Fir_row; i < 40; ++i)
    {
        if (Lef[i] > 40 && Lef[i + 1] >= 40 && Lef[i + 2] <= 40 && Lef[i + 3] < 40 &&
            Lef[i + 3] - Lef[i + 5] < 7 && Lef[i + 5] - Lef[i + 7] < 7 && Lef[i + 7] - Lef[i + 9] < 7 && Lef[i + 9] - Lef[i + 11] < 7)
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
#endif
#if 1

//弯道状态机阈值用0.5试试

/*************************************************************************
*  函数名称：void Fix_line()
*  功能说明：岔路补线处理
*  参数说明：无
*  函数返回：无
*  修改时间：2020.06.23
*  备    注：十字：近远景布线，远景补线
            弯道：圆环弯道部分：将无用信息行（弯道以外）置1和78
            入环，分别按1/2和2/3计算斜率补线
            出环，按2计算斜率补线

*************************************************************************/

void Pic_Fix_Line(void)
{
    float slope;
    // float slope2;
    static float slope_static;
    // int i;
    // int j;
    // int k;
    int xtemp, ytemp, get_flag = 0;
    static int xtemp_static, ytemp_static;
    //static float stat_slope;
    //static float stat_slope2;
    //static char road1_flag1 = 1; //0表示已计算完进圆环斜率，1表示已经出圆环，再次进圆环时计算补线斜率
    //static char road2_flag1 = 1;
    //bool road1_flag2 = 0, road2_flag2 = 0;
    //static bool road1_flag3 = 0, road2_flag3 = 0;
    // static bool road1_flag4, road2_flag4;
    static bool Road_flag1 = 0, Road_flag2 = 0;
    if (Road == 0)
    {
        if (Road0_flag == 1)
        {
            for (int i = Fir_row; i < Allwhiteend; ++i)
            {
                if (Lef[i] <= Fir_col + 15)
                {
                    continue;
                }
                if (Lef[i] - Lef[i + 2] < 5 && Lef[i + 1] - Lef[i + 3] < 5 && Lef[i] - Lef[i + 2] > 0 && Lef[i + 1] - Lef[i + 3] > 0)
                {
                    xtemp = Lef[i];
                    ytemp = i;
                    get_flag = 1;
                    break;
                }
            }
            if (get_flag == 1)
            {
                for (int i = 55; i > Allwhitestart; i--)
                {
                    if (Lef[i] <= Fir_col)
                    {
                        continue;
                    }

                    if (Lef[i - 2] - Lef[i] < 5 && Lef[i - 3] - Lef[i - 1] < 5 && Lef[i - 2] - Lef[i] > 0 && Lef[i - 3] - Lef[i - 1] > 0)
                    {
                        slope = Slope(Lef[i], i, xtemp, ytemp); //Slope(int F1x,int F1y,int F2x,int F2y)
                        if (slope != 999)
                        {
                            for (int j = ytemp; j < 55; j++)
                            {
                                Lef[j] = (int)(Lef[i] - (i - j) / slope);
                            }
                            Pic_undistort(1, 0);
                            break;
                        }
                    }
                }
            }
            else
            {
                for (int i = 55; i > Allwhitestart; i--)
                {
                    if (abs(Lef[i] - Fir_col) < 5)
                    {
                        continue;
                    }
                    if (Lef[i - 3] - Lef[i - 1] < 5 && Lef[i - 5] - Lef[i - 3] < 5 && Lef[i - 3] - Lef[i - 1] > 0 && Lef[i - 5] - Lef[i - 3] > 0)
                    {
                        slope = Slope(Lef[i], i, Lef[i - 5], i - 5); //Slope(int F1x,int F1y,int F2x,int F2y)
                        if (slope != 999)
                        {
                            for (int j = i + 1; j > Fir_row + 5; j--)
                            {
                                Lef[j] = (int)(Lef[i] - (i - j) / slope);
                            }
                            Pic_undistort(1, 0);
                            break;
                        }
                    }
                }
            }
            get_flag = 0;
            for (int i = Fir_row; i < Allwhiteend; ++i)
            {
                if (Rig[i] >= Last_col - 15)
                {
                    continue;
                }
                if (Rig[i + 2] - Rig[i] < 5 && Rig[i + 3] - Rig[i + 1] < 5 && Rig[i + 2] - Rig[i] > 0 && Rig[i + 3] - Rig[i + 1] > 0)
                {
                    xtemp = Rig[i];
                    ytemp = i;
                    get_flag = 1;
                    break;
                }
            }
            if (get_flag == 1)
            {
                for (int i = 55; i > Allwhitestart; i--)
                {
                    if (Rig[i] >= Last_col)
                    {
                        continue;
                    }

                    if (Rig[i] - Rig[i - 2] < 5 && Rig[i - 1] - Rig[i - 3] < 5 && Rig[i] - Rig[i - 2] > 0 && Rig[i - 1] - Rig[i - 3] > 0)
                    {
                        slope = Slope(Rig[i], i, xtemp, ytemp); //Slope(int F1x,int F1y,int F2x,int F2y)
                        if (slope != 999)
                        {
                            for (int j = ytemp; j < 55; j++)
                            {
                                Rig[j] = (int)(Rig[i] - (i - j) / slope);
                            }
                            Pic_undistort(0, 1);
                            break;
                        }
                    }
                }
            }
            else
            {
                for (int i = 55; i > Allwhitestart; i--)
                {
                    if (Last_col - Rig[i] < 5)
                    {
                        continue;
                    }
                    if (Rig[i - 1] - Rig[i - 3] < 5 && Rig[i - 3] - Rig[i - 5] < 5 && Rig[i - 1] - Rig[i - 3] > 0 && Rig[i - 3] - Rig[i - 5] > 0)
                    {
                        slope = Slope(Rig[i], i, Rig[i - 5], i - 5); //Slope(int F1x,int F1y,int F2x,int F2y)
                        if (slope != 999)
                        {
                            for (int j = i + 1; j > Fir_row + 5; j--)
                            {
                                Rig[j] = (int)(Rig[i] - (i - j) / slope);
                            }
                            Pic_undistort(0, 1);
                            break;
                        }
                    }
                }
            }
        }
        else if (Road0_flag == 2)
        {
            for (int i = Fir_row; i < Allwhiteend; ++i)
            {
                if (Lef[i] <= Fir_col + 15)
                {
                    continue;
                }
                if (Lef[i] - Lef[i + 2] < 5 && Lef[i + 2] - Lef[i + 4] < 5 && Lef[i] - Lef[i + 2] > 0 && Lef[i + 2] - Lef[i + 4] > 0)
                {
                    xtemp = Lef[i];
                    ytemp = i;
                    get_flag = 1;
                    break;
                }
            }
            if (get_flag == 1)
            {
                for (int i = Allwgiteend - 1; i > ytemp; --i)
                {
                    if (Lef[i] - Fir_col < 5)
                    {
                        continue;
                    }
                    if (Lef[i - 2] - Lef[i] < 5 && Lef[i - 4] - Lef[i - 2] < 5 && Lef[i - 5] - Lef[i - 3] < 5 &&
                        Lef[i - 2] - Lef[i] > 0 && Lef[i - 4] - Lef[i - 2] > 0 && Lef[i - 5] - Lef[i - 3] > 0)
                    {
                        slope = Slope(Lef[i], i, xtemp, ytemp); //Slope(int F1x,int F1y,int F2x,int F2y)
                        if (slope != 999)
                        {
                            for (int j = i + 1; j < 55; j++)
                            {
                                Lef[j] = (int)(Lef[i] - (i - j) / slope);
                            }
                            Pic_undistort(1, 0);
                            break;
                        }
                    }
                }
            }
            // for (int i = Fir_row + 5; i < Allwhiteend; i++)
            // {
            //     if (abs(Lef[i] - Fir_col) < 15)
            //         continue;
            //     slope = Slope(Lef[i], i, 4, 54); //Slope(int F1x,int F1y,int F2x,int F2y)
            //     if (slope != 999)
            //     {
            //         for (int j = i + 1; j < 55; j++)
            //         {
            //             Lef[j] = (int)(Lef[i] - (i - j) / slope);
            //         }
            //         Pic_undistort(1, 0);
            //         break;
            //     }
            // }
            get_flag = 0;
            for (int i = Fir_row; i < Allwhiteend; ++i)
            {
                if (Rig[i] >= Last_col - 15)
                {
                    continue;
                }
                if (Rig[i + 2] - Rig[i] < 5 && Rig[i + 4] - Rig[i + 2] < 5 && Rig[i + 2] - Rig[i] > 0 && Rig[i + 4] - Rig[i + 2] > 0)
                {
                    xtemp = Rig[i];
                    ytemp = i;
                    get_flag = 1;
                    break;
                }
            }
            if (get_flag == 1)
            {
                for (int i = Allwgiteend - 1; i > ytemp; --i)
                {
                    if (Rig[i] >= Last_col - 5)
                    {
                        continue;
                    }
                    if (Rig[i] - Rig[i - 2] < 5 && Rig[i - 2] - Rig[i - 4] < 5 && Rig[i - 3] - Rig[i - 5] < 5 &&
                        Rig[i] - Rig[i - 2] > 0 && Rig[i - 2] - Rig[i - 4] > 0 && Rig[i - 3] - Rig[i - 5] > 0)
                    {
                        slope = Slope(Rig[i], i, xtemp, ytemp); //Slope(int F1x,int F1y,int F2x,int F2y)
                        if (slope != 999)
                        {
                            for (int j = i + 1; j < 55; j++)
                            {
                                Rig[j] = (int)(Rig[i] - (i - j) / slope);
                            }
                            Pic_undistort(0, 1);
                            break;
                        }
                    }
                }
            }

            // for (int i = Fir_row + 5; i < Allwhiteend; i++)
            // {
            //     if (abs(Rig[i] - Last_col) < 15)
            //         continue;
            //     slope = Slope(Rig[i], i, 75, 54); //Slope(int F1x,int F1y,int F2x,int F2y)
            //     if (slope != 999)
            //     {
            //         for (int j = i + 1; j < 55; j++)
            //         {
            //             Rig[j] = (int)(Rig[i] - (i - j) / slope);
            //         }
            //         Pic_undistort(0, 1);
            //         break;
            //     }
            // }
            return;
        }
        // else if (Road0_flag == 3)
        // {
        //     for (int i = 55; i > Fir_row + 15; i--)
        //     {
        //         if (abs(Lef[i] - Fir_col) < 5)
        //             continue;
        //         slope = Slope(Lef[i], i, Lef[Fir_row + 1], Fir_row + 1); //Slope(int F1x,int F1y,int F2x,int F2y)
        //         if (slope != 999 && slope < 0)
        //         {
        //             for (int j = i + 1; j > Fir_row + 1; j--)
        //             {
        //                 Lef[j] = (int)(Lef[i] - (i - j) / slope);
        //             }
        //             Pic_undistort(1, 0);
        //             break;
        //         }
        //     }
        //     for (int i = 55; i > Fir_row + 15; i--)
        //     {
        //         if (abs(Rig[i] - Last_col) < 5)
        //             continue;
        //         slope = Slope(Rig[i], i, Rig[Fir_row + 1], Fir_row + 1); //Slope(int F1x,int F1y,int F2x,int F2y)
        //         if (slope != 999 && slope > 0)
        //         {
        //             for (int j = i + 1; j > Fir_row + 5; j--)
        //             {
        //                 Rig[j] = (int)(Rig[i] - (i - j) / slope);
        //             }
        //             Pic_undistort(0, 1);
        //             break;
        //         }
        //     }

        //     return;
        // }
        else if (Road0_flag == 4)
        {
            for (int i = turn_stop; i >= Fir_row; --i)
            {
                Rig[i] = 78;
            }
            Pic_undistort(0, 1);
            return;
        }
        else if (Road0_flag == 5)
        {
            for (int i = turn_stop; i >= Fir_row; --i)
            {
                Lef[i] = 1;
            }
            Pic_undistort(1, 0);
            return;
        }
    }

    //左圆环补线处理
    else if (Road == 1)
    {
        if (Road1_flag == 1)
        {
            if (Road_flag1 == 0)
            {
                slope_static = 999;
                Road_flag1 = 1;
            }
            // road1_flag2 = 0;
            for (int i = Fir_row; i < Last_row - 20; i++)
            {
                if (Lef[i] - Fir_col < 2)
                {
                    continue;
                }
                if (Lef[i - 4] - Lef[i - 2] < 5 && Lef[i - 2] - Lef[i] < 5 && Lef[i] - Lef[i + 1] > 15 && Pixle[i + 2][Lef[i] - 5] == 1)
                {

                    slope_static = Slope(Lef[i], i, 79, 54);
                    xtemp_static = Lef[i];
                    ytemp_static = i;
                    // road1_flag2 = 1;
                    break;
                }
            }
            if (slope_static != 999)
            {
                for (int k = Fir_row + 3; k < ytemp_static; k++)
                {
                    // Rig[k] = (int)((xtemp_static - (ytemp_static - k) / slope_static) / 2) + xtemp_static / 2;
                    Rig[k] = (int)((k - ytemp_static) / slope_static / 2) + xtemp_static;
                }
                for (int k = ytemp_static; k < 55; k++)
                {
                    Rig[k] = (int)(xtemp_static - (ytemp_static - k) / slope_static);
                }

                Pic_undistort(0, 1);
            }
        }

        else if (Road1_flag == 2)
        {
            Road_flag1 = 0;
            if (Road_flag2 == 0)
            {
                slope_static = 999;
                Road_flag2 = 1;
            }
            if (Lef_circle_point != 0)
            {
                slope_static = Slope(Rig[Lef_circle_point], Lef_circle_point, 79, 54);
                xtemp_static = Rig[Lef_circle_point];
                ytemp_static = Lef_circle_point;
            }
            if (slope_static != 999)
            {
                for (int k = ytemp_static; k < 55; k++)
                {
                    // Rig[k] = (int)((xtemp_static - (ytemp_static - k) / slope_static )* 2 / 3) + xtemp_static / 3;
                    Rig[k] = (int)((k - ytemp_static) * 2 / slope_static / 3) + xtemp_static;
                }
                Pic_undistort(0, 1);
            }

            // for (int i = Last_row - 13; i > Fir_row; i--)
            // {
            //     if (road1_flag1)
            //     {
            //         if (abs(Lef[i] - Fir_col) < 25)
            //             continue;
            //         stat_slope = Slope(Lef[i], i, 75, 54);
            //         road1_flag1 = 0;
            //     }

            //     if (stat_slope != 999)
            //     {
            //         for (int k = Fir_row + 3; k < 55; k++)
            //         {
            //             Rig[k] = (int)(75 - (54 - k) / stat_slope);
            //         }
            //         Pic_undistort(0, 1);
            //         break;
            //     }
            // }
        }
        else if (Road1_flag == 4)
        {
            Road_flag2 = 0;
            //road1_flag3 = 0;
            //road1_flag4 = 0;
            // road1_flag1 = 1; //表示已经出圆环，再次进圆环时计算补线斜率
            for (int i = turn_stop; i >= Fir_row; --i)
            {
                Rig[i] = 78;
            }
            Pic_undistort(0, 1);
        }
        else if (Road1_flag == 3)
        {
            for (int j = Last_row - 3; j > Fir_row; j--)
            {

                if (abs(Rig[j] - Last_col) < 2 || Rig[j] - Rig[j - 2] >= 5 || Rig[j - 2] - Rig[j - 4] >= 5 || Rig[j - 4] - Rig[j - 6] >= 5 || Rig[j] - Rig[j - 2] < 0 || Rig[j - 2] - Rig[j - 4] < 0 || Rig[j - 4] - Rig[j - 6] < 0)
                    continue;
                slope = Slope(Rig[j - 1], j - 1, Rig[j - 6], j - 6);

                if (slope != 999)
                {
                    for (int k = j - 7; k > Fir_row + 5; k--)
                    {
                        // Rig[k] = (int)(Rig[j] - (j - k) / slope);
                        // Rig[k] = (int)((Rig[j - 6] - (j - 6 - k) / slope) * 2) - Rig[j - 6];
                        Rig[k] = (int)((k + 6 - j) * 2 / slope) + Rig[j - 6];
                    }
                    Pic_undistort(0, 1);
                    break;
                }
            }
        }
        // else if (Road1_flag == 5)
        // {
        //   ;
        // }
    }
    else if (Road == 2)
    {
        if (Road2_flag == 1)
        {
            if (Road_flag1 == 0)
            {
                slope_static = 999;
                Road_flag1 = 1;
            }
            // road2_flag2 = 0;
            for (int i = Fir_row; i < Last_row - 20; i++)
            {
                if (Last_col - Rig[i] < 2)
                {
                    continue;
                }
                if (Rig[i] - Rig[i - 2] < 5 && Rig[i - 2] - Rig[i - 4] < 5 && Rig[i + 1] - Rig[i] > 15 && Pixle[i + 2][Rig[i] + 5] == 1)
                {
                    slope_static = Slope(Rig[i], i, 0, 54);
                    xtemp_static = Rig[i];
                    ytemp_static = i;
                    // road2_flag2 = 1;
                    // road2_flag3 = 1;
                    break;
                }
            }
            // if (Rig_circle_point == 0)
            // {
            //     road2_flag2 = 1;
            // }
            if (slope_static != 999)
            {
                for (int k = Fir_row + 3; k < ytemp_static; k++)
                {
                    Lef[k] = (int)((k - ytemp_static) / slope_static / 2) + xtemp_static;
                }
                for (int k = ytemp_static; k < 55; k++)
                {
                    Lef[k] = (int)(xtemp_static - (ytemp_static - k) / slope_static);
                }
                Pic_undistort(1, 0);
            }
        }
        else if (Road2_flag == 2)
        {
            Road_flag1 = 0;
            if (Road_flag2 == 0)
            {
                slope_static = 999;
                Road_flag2 = 1;
            }
            if (Rig_circle_point != 0)
            {
                slope_static = Slope(Lef[Rig_circle_point], Rig_circle_point, 1, 54);
                xtemp_static = Lef[Rig_circle_point];
                ytemp_static = Rig_circle_point;
            }

            if (slope_static != 999)
            {
                for (int k = ytemp_static; k < 55; k++)
                {
                    // Lef[k] = (int)(4 - (54 - k) / slope_static);
                    Lef[k] = (int)((k - ytemp_static) * 2 / slope_static / 3) + xtemp_static;
                }
                Pic_undistort(1, 0);
            }

            // for (int i = Last_row - 13; i > Fir_row; i--)
            // {
            //     if (road2_flag1)
            //     {
            //         if (abs(Rig[i] - Last_col) < 25)
            //             continue;
            //         stat_slope2 = Slope(Rig[i], i, 4, 54);
            //         road2_flag1 = 0;
            //     }

            //     if (stat_slope2 != 999)
            //     {
            //         for (int k = Fir_row + 3; k < 55; k++)
            //         {
            //             Lef[k] = (int)(15 - (57 - k) / stat_slope2);
            //         }
            //         Pic_undistort(1, 0);
            //         break;
            //     }
            // }
        }
        else if (Road2_flag == 4)
        {
            Road_flag2 = 0;
            // road2_flag3 = 0;
            // road2_flag4 = 0;
            // road2_flag1 = 1;
            for (int i = turn_stop; i >= Fir_row; --i)
            {
                Lef[i] = 1;
            }
            Pic_undistort(1, 0);
        }
        else if (Road2_flag == 3)
        {
            for (int j = Last_row - 3; j > Fir_row; j--)
            {

                if (abs(Lef[j] - Fir_col) < 2 || Lef[j - 2] - Lef[j] >= 5 || Lef[j - 4] - Lef[j - 2] >= 5 || Lef[j - 6] - Lef[j - 4] >= 5 || Lef[j - 2] - Lef[j] < 0 || Lef[j - 4] - Lef[j - 2] < 0 || Lef[j - 6] - Lef[j - 4] < 0)
                    continue;
                slope = Slope(Lef[j - 1], j - 1, Lef[j - 6], j - 6);

                if (slope != 999)
                {
                    for (int k = j - 6; k > Fir_row + 5; k--)
                    {
                        // Lef[k] = (int)(Lef[j] - (j - k) / slope2);
                        Lef[k] = (int)((k + 6 - j) * 2 / slope) - Lef[j - 6];
                    }
                    Pic_undistort(1, 0);
                    break;
                }
            }
        }
        // else if (Road2_flag == 5)
        // {
        //   ;
        // }
    }
    else if (Road == 7)
    {
        if (Road7_flag == 0)
        {
            get_flag = 0;
            for (int i = Fir_row + 2; i < start_stop_line; i++)
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

                for (int i = start_stop_line + 5; i < Last_row; i++)
                {
                    if (abs(Lef[i] - Fir_col) < 4 || Lef[i] - Lef[i + 2] > 5 || Lef[i] - Lef[i + 2] < 0)
                        continue;

                    slope = Slope(Lef[i], i, xtemp, ytemp); //Slope(int F1x,int F1y,int F2x,int F2y)
                    if (slope != 999)
                    {
                        for (int j = ytemp; j < 55; j++)
                        {
                            Lef[j] = (int)(Lef[i] - (i - j) / slope);
                        }
                        Pic_undistort(1, 0);
                        break;
                    }
                }
            }

            get_flag = 0;
            for (int i = Fir_row + 2; i < start_stop_line; i++)
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
                for (int i = start_stop_line + 5; i < Last_row; i++)
                {
                    if (abs(Rig[i] - Fir_col) < 4 || Rig[i] < 40 || Rig[i + 2] - Rig[i] > 5 || Rig[i + 2] - Rig[i] < 0)
                        continue;

                    slope = Slope(Rig[i], i, xtemp, ytemp); //Slope(int F1x,int F1y,int F2x,int F2y)
                    if (slope != 999)
                    {
                        for (int j = ytemp; j < 55; j++)
                        {
                            Rig[j] = (int)(Rig[i] - (i - j) / slope);
                        }
                        Pic_undistort(0, 1);
                        break;
                    }
                }
            }
        }
        else if (Road7_flag == 1)
        {

            // get_flag = 0;
            for (int i = Fir_row + 5; i < start_stop_line; i++)
            {
                if (abs(Lef[i] - Fir_col) < 15 || Lef[i] - Lef[i + 2] > 5 || Lef[i] - Lef[i + 2] < 0)
                    continue;
                // xtemp = Lef[i];
                // ytemp = i;
                slope = Slope(4, 54, Lef[i], i); //Slope(int F1x,int F1y,int F2x,int F2y)
                if (slope != 999)
                {
                    for (int j = ytemp; j < 55; j++)
                    {
                        Lef[j] = (int)(Lef[i] - (i - j) / slope);
                    }
                    Pic_undistort(1, 0);
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
            for (int i = Fir_row + 5; i < start_stop_line; i++)
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
                    for (int j = ytemp; j < 55; j++)
                    {
                        Rig[j] = (int)(Rig[i] - (i - j) / slope);
                    }
                    Pic_undistort(0, 1);
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
            //
        }
    }
    fangyuejie();
}

#endif
#if 1

void Pic_send_new2(void)
{
    int i;
    int j;
    cnt = 0;
#ifdef ori_pic
    for (i = 0 : i < 20; ++i)
    {
        j = i * 2.5 + 5;
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
    for (i = 0 : i < 20; ++i)
    {
        j = i * 2.5 + 5;
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
            My_Put_Char(New_Lef[j]);
        }
    }
    for (i = 20; i < 60; i++)
    {
        My_Put_Char(Lef[i]); //New_Lef[i]/10+40);
    }

    for (i = 0 : i < 20; ++i)
    {
        j = i * 2.5 + 5;
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
            My_Put_Char(New_Mid[j]);
        }
    }
    for (i = 20; i < 60; i++)
    {
        My_Put_Char(Mid[i]);
    }

    for (i = 0 : i < 20; ++i)
    {
        j = i * 2.5 + 5;
        if (New_Rig[j] < -395)
        {
            My_Put_Char(0);
        }
        else if (New_Rig[j] > 385)
        {
            My_Put_Char(79);
        }
        else
        {
            My_Put_Char(New_Rig[j]+1);
        }
    }

    for (i = 20; i < 60; i++)
    {
        My_Put_Char(Rig[i] + 1); //New_Rig[i]/10+40);
    }

    /*****************************************************/
    My_Put_Char(0xf2); //代表整个数据都发完了
}

#endif
#if 1

/*************************************************************************
*  函数名称：void Turn_Cam(void)
*  功能说明：摄像头转弯控制程序，根据中心偏移量计算舵机输出量
*  参数说明：中心偏移量,PD 
*  函数返回：舵机打角
*  修改时间：2020.6.20
*  备    注：偏移量offset为负说明车身相对赛道中心偏左
             偏移量offset为正说明车身相对赛道中心偏右
*************************************************************************/

PID PID_TURN_CAM_EXT;
void Turn_Cam(void)
{
    PID PID_TURN_CAM;
    static float Cam_offset_old = 0;
    PID_TURN_CAM = TurnFuzzyPD_Cam();

    //0.768=0.8*1.2*0.8
    PID_TURN_CAM.P *= PID_TURN_CAM_EXT.P; //0.85;//0.7
    PID_TURN_CAM.D *= PID_TURN_CAM_EXT.D;

    Turn_Cam_Out = PID_TURN_CAM.P * Cam_offset + PID_TURN_CAM.D * (Cam_offset - Cam_offset_old); //转向PID控制

    Cam_offset_old = Cam_offset;
    if (Road == 1 && Road1_flag == 5)
    {
        if (Turn_Cam_Out > -0.5 * SERVO_RANGE)
        {
            Turn_Cam_Out = -0.5 * SERVO_RANGE;
        }
    }
    else if (Road == 2 && Road2_flag == 5)
    {
        if (Turn_Cam_Out < 0.5 * SERVO_RANGE)
        {
            Turn_Cam_Out = 0.5 * SERVO_RANGE;
        }
    }
    Servo_Duty(-Turn_Cam_Out);
}
/*************************************************************************
*  函数名称：void TurnFuzzyPD_Cam(void)
*  功能说明：转弯PD模糊函数------摄像头控制
*  参数说明：PD 表
*  函数返回：摄像头控制转弯PD
*  修改时间：2020.6.20
*  备    注：
*************************************************************************/
PID TurnFuzzyPD_Cam(void)
{
    PID PID_TURN_CAM;
    static const float Cam_Offset_Table0[21] = {-140, -130, -110, -100, -80, -60, -50, -40, -30, -20, 0, 20, 30, 40, 50, 60, 80, 100, 110, 130, 140};
    static const float Turn_Cam_P_Table0[21] = {1.29, 1.20, 1.15, 1.20, 1.20, 1.30, 1.35, 1.60, 1.5, 1.45, 0.3, 1.45, 1.5, 1.60, 1.35, 1.30, 1.20, 1.20, 1.15, 1.20, 1.29};
    static const float Turn_Cam_D_Table0[21] = {1.29, 1.20, 1.15, 1.20, 1.20, 1.30, 1.35, 1.60, 1.5, 1.45, 0.01, 1.45, 1.5, 1.60, 1.35, 1.30, 1.20, 1.20, 1.15, 1.20, 1.29};

    if (Cam_offset <= Cam_Offset_Table0[0])
    {
        PID_TURN_CAM.P = Turn_Cam_P_Table0[0];
        PID_TURN_CAM.D = Turn_Cam_D_Table0[0];
    }
    else if (Cam_offset >= Cam_Offset_Table0[20])
    {
        PID_TURN_CAM.P = Turn_Cam_P_Table0[20];
        PID_TURN_CAM.D = Turn_Cam_D_Table0[20];
    }
    else
    {
        for (int i = 0; i < 20; i++)
        {
            if (Cam_offset >= Cam_Offset_Table0[i] && Cam_offset < Cam_Offset_Table0[i + 1])
            {
                PID_TURN_CAM.P = Turn_Cam_P_Table0[i] + (Cam_offset - Cam_Offset_Table0[i]) * (Turn_Cam_P_Table0[i + 1] - Turn_Cam_P_Table0[i]) / (Cam_Offset_Table0[i + 1] - Cam_Offset_Table0[i]); //线性
                PID_TURN_CAM.D = Turn_Cam_D_Table0[i] + (Cam_offset - Cam_Offset_Table0[i]) * (Turn_Cam_D_Table0[i + 1] - Turn_Cam_D_Table0[i]) / (Cam_Offset_Table0[i + 1] - Cam_Offset_Table0[i]);
                break;
            }
        }
    }
    return PID_TURN_CAM;
}



#endif 
#if 1
slope*PYK*XPK;
theta-90
150/sin(theta)
150/sin(arctan(slope*PYK*XPK));

#endif 