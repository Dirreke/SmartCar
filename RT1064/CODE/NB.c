#include"headfile.h"
bool Road1_judge_EN = 1;
bool Road2_judge_EN = 1;
bool Road4_judge_EN = 1;
bool Road7_judge_EN = 1;


float Road4_starttime = 0;
float Road4_stoptime = -500;
float Road1_starttime = 0;
float Road1_stoptime = -500;
float Road2_starttime = 0;
float Road2_stoptime = -500;
float Road7_starttime = 0;
float Road7_stoptime = -500;


void judge_EN_set(void)
{
    if (loop_time == 0)
    {
        Road1_judge_EN = 1;
        Road2_judge_EN = 1;
        Road4_judge_EN = 1;
        Road7_judge_EN = 1;
        return;
    }
    if (loop_time > Road1_starttime && (loop_time < Road1_stoptime || Road1_stoptime < Road1_starttime) && Road1_starttime >=0)
    {
        Road1_judge_EN = 1;
    }
    else
    {
        Road1_judge_EN = 0;
    }
    if (loop_time > Road2_starttime && (loop_time < Road2_stoptime || Road2_stoptime < Road2_starttime)&& Road2_starttime >=0)
    {
        Road2_judge_EN = 1;
    }
    else
    {
        Road2_judge_EN = 0;
    }
    if (loop_time > Road4_starttime && (loop_time < Road4_stoptime || Road4_stoptime < Road4_starttime)&& Road4_starttime >=0)
    {
        Road4_judge_EN = 1;
    }
    else
    {
        Road4_judge_EN = 0;
    }
    if (loop_time > Road7_starttime && (loop_time < Road7_stoptime || Road7_stoptime < Road7_starttime)&& Road7_starttime >=0)
    {
        Road7_judge_EN = 1;
    }
    else
    {
        Road7_judge_EN = 0;
    }
}

float NB_starttime[10] = {-500,-500,-500,-500,-500,-500,-500,-500,-500,-500};
float NB_stoptime[10];
int NB_Road[10];
int NB_Road_flag[10];

void NB_set(void)
{
    static bool set_flag[10];
    if (loop_time == 0)
    {
        for (int i = 0; i < 10; ++i)
        {
            set_flag[i] = 1;
        }
        return;
    }
    for (int i = 0; i < 10; ++i)
    {
        if (NB_starttime[i] < 0)
        {
            continue;
        }
        else if (loop_time > NB_starttime[i] && NB_stoptime[i] < NB_starttime[i] && set_flag[i])
        {
            set_flag[i] = 0;
            Road = NB_Road[i];
            if (Road == 0)
            {
                Road0_flag = NB_Road_flag[i];
                Road1_flag = 0;
                Road2_flag = 0;
                Road4_flag = 0;
                Road7_flag = 0;
                Road3_flag = 0;
            }
            else if (Road == 1)
            {
                Road0_flag = 0;
                Road1_flag = NB_Road_flag[i];
                Road2_flag = 0;
                Road4_flag = 0;
                Road7_flag = 0;
                Road3_flag = 0;
            }
            else if (Road == 2)
            {
                Road0_flag = 0;
                Road1_flag = 0;
                Road2_flag = NB_Road_flag[i];
                Road4_flag = 0;
                Road7_flag = 0;
                Road3_flag = 0;
            }
            else if (Road == 4)
            {
                Road0_flag = 0;
                Road1_flag = 0;
                Road2_flag = 0;
                Road4_flag = NB_Road_flag[i];;
                Road7_flag = 0;
                Road3_flag = 0;
            }
            else if (Road == 7)
            {
                Road0_flag = 0;
                Road1_flag = 0;
                Road2_flag = 0;
                Road4_flag = 0;
                Road7_flag = 0;
                Road3_flag = NB_Road_flag[i];;
            }

        }
        else if (loop_time > NB_starttime[i] && loop_time< NB_stoptime[i])
        {
            Road = NB_Road[i];
            if (Road == 0)
            {
                Road0_flag = NB_Road_flag[i];
                Road1_flag = 0;
                Road2_flag = 0;
                Road4_flag = 0;
                Road7_flag = 0;
                Road3_flag = 0;
            }
            else if (Road == 1)
            {
                Road0_flag = 0;
                Road1_flag = NB_Road_flag[i];
                Road2_flag = 0;
                Road4_flag = 0;
                Road7_flag = 0;
                Road3_flag = 0;
            }
            else if (Road == 2)
            {
                Road0_flag = 0;
                Road1_flag = 0;
                Road2_flag = NB_Road_flag[i];
                Road4_flag = 0;
                Road7_flag = 0;
                Road3_flag = 0;
            }
            else if (Road == 4)
            {
                Road0_flag = 0;
                Road1_flag = 0;
                Road2_flag = 0;
                Road4_flag = NB_Road_flag[i];;
                Road7_flag = 0;
                Road3_flag = 0;
            }
            else if (Road == 7)
            {
                Road0_flag = 0;
                Road1_flag = 0;
                Road2_flag = 0;
                Road4_flag = 0;
                Road7_flag = 0;
                Road3_flag = NB_Road_flag[i];;
            }
        }

    }
}