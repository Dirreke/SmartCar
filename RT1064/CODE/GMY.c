#include "headfile.h"
/*************************************************************************
*  函数名称：void Pic_DrawMid_und(void)
*  功能说明：计算去畸变后中线
*  参数说明：无
*  函数返回：无
*  修改时间：2020.1.18
*  备    注：Cam_offset为负，车偏右，向左拐；
             Cam_offset为正，车偏左，向右拐；

*************************************************************************/
int New_Mid[60];

void Pic_DrawMid_und(void)
{
    int i;
    int count = 0;
    int flag = 0;
    float slope_temp;

    for (i = 0; i < 60; i++)
    {
        if (New_Lef[i] != -MIDMAP && New_Rig[i] != MIDMAP) //Mid Calculaing
        {
            New_Mid[i] = (int)((New_Lef[i] + New_Rig[i]) / 2.0 + 0.5);
            flag = 1;
        }
        else if (New_Lef[i] == -MIDMAP && New_Rig[i] != MIDMAP)
        {
            New_Mid[i] = New_Rig[i] - ROAD_HALF_WIDTH;
            flag = 1;
        }
        else if (New_Lef[i] != -MIDMAP && New_Rig[i] == MIDMAP)
        {
            New_Mid[i] = New_Lef[i] + ROAD_HALF_WIDTH;
            flag = 1;
        }
        else if (flag == 1)
        {
            count++; //for 插值
            flag = 0;
        }

        if (count != 0 && flag == 1) //插值
        {
            slope_temp = (New_Mid[i] - New_Mid[i - count - 1]) / (count + 1);
            for (j = count; j > 0; j--)
            {
                New_Mid[i - j] = (int)(New_Mid[i] - slope_temp * j + 0.5);
            }
        }
    }
    /*
    //   int count = 0, count2 = 0;

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
    }
    if (Road0_flag && Road == 0)
    {
        Cam_offset *= 0.3;
    }

    if (Road1_flag == 3 || Road2_flag == 3)
        Cam_offset *= 0.8;

    if (count != 0)
    {
        Cam_offset = Cam_offset / count;
    }
    if (count2 != 0)
    {
        Cam_offset2 = Cam_offset2 / count2;
    }

    //Cam_offset = (0.4 * Cam_offset + 0.6 * Cam_offset2);
    if (Road == 3 || Road1_flag == 2 || Road2_flag == 2)
    {
        Cam_offset *= 1;
    }
    */
    return;
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

*************************************************************************/
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
    j = 58;
    Rig_New[0] = tempNewxR[0];
    Lef_New[0] = tempNewxL[0];
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





