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

    for (i = 1; i < 60; i++)
    {
        if (New_Lef[i] != -MIDMAP && New_Rig[i] != MIDMAP) //Mid Calculaing
        {
            New_Mid[i] = New_Lef[i] + New_Rig[i];
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
        else
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