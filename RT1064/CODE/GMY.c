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
                else//结尾断点
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
                else//结尾断点
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
                else//结尾断点
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
                flag=0;
            }
            else if (flag == 1)
            {
                slope_temp = New_Mid[i] - New_Mid[i - 1];
                for (j = inter_point + 1; j > 1; j--)
                {
                    New_Mid[i - j] = (int)(New_Mid[i] - slope_temp * j + 0.5);
                }
                flag=0;
            }
        }
    }
    return;
}
/*************************************************************************
 *  函数名称：float Circle_angle_val1_calculate(void)
 *  功能说明：计算中线的曲率半径,并计算理想舵机打角
 *  参数说明：无
 *  函数返回：转向打角
 *  修改时间：2020.1.18
 *  备    注：其中12位参数等于去畸中压缩比例的数字,y2_origin为参数
             angle_val1为负，车偏右，向左拐；
             angle_val1为正，车偏左，向右拐；
 * ************************************************************************/
#define CAR_LENGTH 148.5149
float Circle_R_calculate(void)
{
    int i;
    float x2_temp;
    float sum1=0;
    float sum2=0;
    float temp_sin;
    float angle_val1;
    static const float y2_origin[58]={467856,451584,435600,419904,404496,389376,374544,360000,345744,331776,318096,304704,291600,278784,266256,254016,242064,230400,219024,207936,197136,186624,176400,166464,156816,147456,138384,129600,121104,112896,104976,97344,90000,82944,76176,69696,63504,57600,51984,46656,41616,36864,32400,28224,24336,20736,17424,14400,11664,9216,7056,5184,3600,2304,1296,576,144,0};
    //认为控制行高（近）行为第0行，成比例反转，即y2_origin=((control_line_min-y)*12)^2，暂定控制行3-57。

    for(i=3;i<58;i++)
    {
        x2_temp=New_Mid[i]^2;
        sum1+=x2_temp;
        sum2+=(x2_temp+y2_origin[i])*New_Mid[i];
    }
    if (sum2 == 0)
    {
        angle_val1=0
    }
    else
    {
        temp_sin=sum1/sum2*2*CAR_LENGTH;
        temp_sin=limit_f(temp_sin, -1, 1);
        angle_val1=asin(temp_sin);
    }
    return angle_val1;
}
/************************************************************************
 * search.c 1459-
 * 修改，减少取景行
 * 增加参数startpoint便于调整
 * 
 * **********************************************************************/
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
    int const startpoint = 11;
    int tempy;
    int tempx;
    int tempNewxR[60];
    int tempNewxL[60];
    int temp;
    int step;
    int Rig_New[60];
    int Lef_New[60];
    static const int tempNewy[60] = {707, 640, 583, 533, 490, 452, 419, 389, 362, 337, 315, 295, 277, 260, 244, 230, 216, 204, 192, 182, 171, 162, 153, 145, 137, 129, 122, 115, 109, 103, 97, 91, 86, 81, 76, 72, 67, 63, 59, 55, 51, 48, 44, 41, 38, 34, 31, 29, 26, 23, 20, 18, 15, 13, 11, 8, 6, 4, 2, 0};

    /*************************映射*******************************/
    if (R)
    {
        for (i = startpoint; i < 60; i++)
        {
            tempy = i;
            tempx = Rig[i];
            if (tempx >= Last_col)
                tempNewxR[i] = MIDMAP;
            else
                tempNewxR[i] = (int)((UNDISTORT_D * UNDISTORT_C + UNDISTORT_H) * 2 * (tempx - 39.5) / (-UNDISTORT_S * (29.5 - tempy) + UNDISTORT_D * UNDISTORT_C) + 0.5);

            if (tempNewxR[i] > MIDMAP)
                tempNewxR[i] = MIDMAP;
        }
    }
    if (L)
    {
        for (i = startpoint; i < 60; i++)
        {
            tempy = i;
            tempx = Lef[i];
            if (tempx <= Fir_col)
                tempNewxL[i] = -MIDMAP;
            else
                tempNewxL[i] = (int)((UNDISTORT_D * UNDISTORT_C + UNDISTORT_H) * 2 * (tempx - 39.5) / (-UNDISTORT_S * (29.5 - tempy) + UNDISTORT_D * UNDISTORT_C) + 0.5);

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
        temp = 12 * j;
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
