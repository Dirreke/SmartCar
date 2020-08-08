#include "headfile.h"
#include "debug.h"

// int DEBUG_CHOICE = 1;
bool kaiji_flag = 0;
float Cam_P_New = 0.8;
bool ips_choice = 0;
// float speedgoal;
// float curvespeedgoal;
void Debug_Init(void)
{
    gpio_init(B10, GPI, 1, GPIO_PIN_CONFIG);
    gpio_init(D4, GPI, 1, GPIO_PIN_CONFIG);
    gpio_init(D27, GPI, 1, GPIO_PIN_CONFIG);
    gpio_init(C31, GPI, 1, GPIO_PIN_CONFIG);
    gpio_init(C27, GPI, 1, GPIO_PIN_CONFIG);
    gpio_init(C26, GPI, 1, GPIO_PIN_CONFIG);
    ips200_clear(WHITE);
}

void Debug_key(void)
{
    static int ips_num = 0;
    int page_num0 = 19;
    const int page_num1 = 49;
    int NB_Use_No = 0;
    static int page_num = 19;
    static bool qipao_flag;
    static bool guanpingmu_flag = 0;
    if (gpio_get(DEBUG_KEY0))
    {
        // Road = 0;
        if (qipao_flag == 0)
        {
            lib_speed_set(DEFAULT_SPEED);
            qipao_flag = 1;
            //barn_reset_flag = 0;
        }
        guanpingmu_flag = 0;

        return;
    }
    else
    {
        qipao_flag = 0;
        loop_time = 0;
        loop_distance = 0;
        ramp_out_time = -500;
        out_circle_time_temp = -500;
        if(!guanpingmu_flag)
        {
        Road3_flag = 0;
        Road = 3;    
        guanpingmu_flag = 1;
        }
        
        kaiji_flag = 1;
        lib_speed_set(0);
    }

    if(gpio_get(DEBUG_KEY5))
    {
        if(ips_choice)
        {
            ips_choice = 0;
            ips200_clear(WHITE);
            ips_num = 0;
            page_num = page_num0;
        }        
    }
    else
    {
        if(!ips_choice)
        {
            ips_num = 0;
            ips200_clear(WHITE);
            ips_choice = 1;
            page_num = page_num1;
        }
    }

    //·­Ò³
    if(ips_choice == 0)
    {
        if (!gpio_get(DEBUG_KEY2))
        {
            systick_delay_ms(50);
            if (!gpio_get(DEBUG_KEY2))
            {
                ips_num++;
                ips200_clear(WHITE);
                if (ips_num >= page_num)
                {
                    ips_num = 0;
                }
                return;
            }
        }

        if (!gpio_get(DEBUG_KEY1))
        {
            systick_delay_ms(50);
            if (!gpio_get(DEBUG_KEY1))
            {
                // ips_clear(IPS_WHITE);
                ips_num--;
                ips200_clear(WHITE);
                if (ips_num < 0)
                {
                    ips_num = page_num - 1;
                }
                return;
            }
        }
    }
    else
    {
        for(int i = 9 ; i >= 0 ; i--)
        {
            if(NB_starttime[i]>=0)
            {
                NB_Use_No = i+1;
            }
        }

        if (!gpio_get(DEBUG_KEY2))
        {
            systick_delay_ms(50);
            if (!gpio_get(DEBUG_KEY2))
            {
                ips_num++;
                if(ips_num > NB_Use_No*4+1 && ips_num < 41)
                {
                    ips_num = 41;
                }
                ips200_clear(WHITE);
                if (ips_num >= page_num)
                {
                    ips_num = 0;
                }
                return;
            }
        }

        if (!gpio_get(DEBUG_KEY1))
        {
            systick_delay_ms(50);
            if (!gpio_get(DEBUG_KEY1))
            {
                // ips_clear(IPS_WHITE);
                ips_num--;
                if(ips_num > NB_Use_No*4+1 && ips_num < 41)
                {
                    ips_num = NB_Use_No*4+1;
                }
                ips200_clear(WHITE);
                if (ips_num < 0)
                {
                    ips_num = page_num - 1;
                }
                return;
            }
        }
    }
    
    //ÏÔÊ¾
    if(ips_choice == 0)
    {
        ips_show_debug0(ips_num);
    }
    else
    {
        ips_show_debug1(ips_num);
    }

    // ips_show_debug(ips_num);

    //µ÷Õû
    if(ips_choice == 0)
    {
        if (!gpio_get(DEBUG_KEY3))
        {
            systick_delay_ms(50);
            if (!gpio_get(DEBUG_KEY3))
            {
                switch (19 - ips_num)
                {

                case 1:
                    competition_strategy += 1;
                    Para_Init();
                    break;
                case 2:
                    STRAIGHT_SPEED += 0.1;
                    break;
                case 3:
                    CURVE_SPEED += 0.1;
                    break;
                case 4:
                    UP_RAMP_SPEED += 0.1; //threshold_offset += 1;
                    break;
                case 5:
                    ON_RAMP_SPEED += 0.1; //PID_CAR_Diffcomp_CAM.P += 0.1;
                    break;
                case 6:
                    PRE_STOP_SPEED += 0.1;//PID_CAR_STRAIGHT_CAM.P += 0.01;
                    break;
                case 7:
                    RUSH_STOP_SPEED += 0.1;
                    break;
                case 8:
                    EMERGENCY_STOP_SPEED += 0.1;
                    break;
                case 9:
                    Road += 1;
                    switch (Road)
                    {
                    case 0:
                        Road0_flag = 0;
                        break;
                    case 1:
                        Road1_flag = 0;
                        break;
                    case 2:
                        Road2_flag = 0;
                        break;
                    case 3:
                        Road3_flag = 0;
                        break;
                    case 4:
                        Road4_flag = 0;
                    case 7:
                        Road7_flag = 0;
                        break;
                    default:
                        break;
                    }
                    break;
                case 10:
                    switch (Road)
                    {
                    case 0:
                        Road0_flag++;
                        break;
                    case 1:
                        Road1_flag++;
                        break;
                    case 2:
                        Road2_flag++;
                        break;
                    case 3:
                        Road3_flag++;
                        break;
                    case 4:
                        Road4_flag++;
                    case 7:
                        Road7_flag++;
                        break;
                    default:
                        break;
                    }
                    break;
                case 11:
                    ramp_ANGLE_3 += 0.5;
                    break;
                case 12:
                    If_Use_EM_Stop =! If_Use_EM_Stop;
                    break;
                case 13:
                    If_Use_EM_On_Ramp  =! If_Use_EM_On_Ramp;
                    break;
                case 14:
                    PID_CAR_STRAIGHT_CAM.D += 1;
                    break;
                case 15:
                    PID_diff0.P += 0.05;
                    break;
                case 16:
                    PID_diff.P += 0.05;
                    break;
                case 17:
                    car_straight_b += 0.01;
                    break;
                case 18:
                    car_straight_k += 0.01;
                    break;
                case 19:
                    barn_state = !barn_state;
                    break;
                default:
                    break;
                }
                return;
            }
        }
        if (!gpio_get(DEBUG_KEY4))
        {
            systick_delay_ms(50);
            if (!gpio_get(DEBUG_KEY4))
            {
                switch (19 - ips_num)
                {

                    case 1:
                        competition_strategy -= 1;
                        Para_Init();
                        break;
                    case 2:
                        STRAIGHT_SPEED -= 0.1;
                        break;
                    case 3:
                        CURVE_SPEED -= 0.1;
                        break;
                    case 4:
                        UP_RAMP_SPEED -= 0.1; //threshold_offset += 1;
                        break;
                    case 5:
                        ON_RAMP_SPEED -= 0.1; //PID_CAR_Diffcomp_CAM.P += 0.1;
                        break;
                    case 6:
                        PRE_STOP_SPEED -= 0.1;//PID_CAR_STRAIGHT_CAM.P += 0.01;
                        break;
                    case 7:
                        RUSH_STOP_SPEED -= 0.1;
                        break;
                    case 8:
                        EMERGENCY_STOP_SPEED -= 0.1;
                        break;
                    case 9:
                        Road -= 1;
                        switch (Road)
                        {
                        case 0:
                            Road0_flag = 0;
                            break;
                        case 1:
                            Road1_flag = 0;
                            break;
                        case 2:
                            Road2_flag = 0;
                            break;
                        case 3:
                            Road3_flag = 0;
                            break;
                        case 4:
                            Road4_flag = 0;
                        case 7:
                            Road7_flag = 0;
                            break;
                        default:
                            break;
                        }
                        break;
                    case 10:
                        switch (Road)
                        {
                        case 0:
                            Road0_flag--;
                            break;
                        case 1:
                            Road1_flag--;
                            break;
                        case 2:
                            Road2_flag--;
                            break;
                        case 3:
                            Road3_flag--;
                            break;
                        case 4:
                            Road4_flag--;
                        case 7:
                            Road7_flag--;
                            break;
                        default:
                            break;
                        }
                        break;
                    case 11:
                        ramp_ANGLE_3 -= 0.5;
                        break;
                    case 12:
                        If_Use_EM_Stop =! If_Use_EM_Stop;
                        break;
                    case 13:
                        If_Use_EM_On_Ramp  =! If_Use_EM_On_Ramp;
                        break;
                    case 14:
                        PID_CAR_STRAIGHT_CAM.D -= 1;
                        break;
                    case 15:
                        PID_diff0.P -= 0.05;
                        break;
                    case 16:
                        PID_diff.P -= 0.05;
                        break;
                    case 17:
                        car_straight_b -= 0.01;
                        break;
                    case 18:
                        car_straight_k -= 0.01;
                        break;
                    case 19:
                        barn_state = !barn_state;
                        break;

                    default:
                        break;
                }
                return;
            }
        }
    }
    else
    {
        DEBUG_KEY_adjust(ips_num);
    }       
}

void DEBUG_KEY_adjust(int ips_num)
{
    if (!gpio_get(DEBUG_KEY3))
    {
        systick_delay_ms(50);
        if (!gpio_get(DEBUG_KEY3))
        {
            if(ips_num == 0)
            {
                threshold_offset ++;
                return;
            }
            for(int i = 0;i < 10 ; ++i)
            {
                if(ips_num <= i*4+4)
                {
                    switch(ips_num % 4)
                    {
                        case 1:
                        NB_starttime[i] += 500;
                        NB_stoptime[i] = NB_starttime[i];
                        break;
                        case 2:
                        NB_stoptime[i] += 500;
                        break;
                        case 3:
                        NB_Road[i] ++;
                        break;
                        case 0:
                        NB_Road_flag[i] ++;
                        default:
                        break;
                    }
                    return;
                }
            }
            switch(ips_num)
            {

                case 42:
                    Road7_starttime += 500;
                    Road7_stoptime = Road7_starttime;
                    break;
                case 41:
                    Road7_stoptime += 500;
                    break;
                case 44:
                    Road1_starttime += 500;
                    Road1_stoptime = Road1_starttime;
                    break;
                case 43:
                    Road1_stoptime += 500;
                    break;
                case 46:
                    Road2_starttime += 500;
                    Road2_stoptime = Road2_starttime;
                    break;
                case 45:
                    Road2_stoptime += 500;
                    break;
                case 48:
                    Road4_starttime += 500;
                    Road4_stoptime = Road4_starttime;
                    break;
                case 47:
                    Road4_stoptime += 500;
                    break;
                default:
                    break;

            }
            return;
        }
    }
    if (!gpio_get(DEBUG_KEY4))
    {
        systick_delay_ms(50);
        if (!gpio_get(DEBUG_KEY4))
        {
            if(ips_num == 0)
            {
                threshold_offset --;
                return;
            }
            for(int i = 0;i < 10 ; ++i)
            {
                if(ips_num <= i*4+4)
                {
                    switch(ips_num % 4)
                    {
                        case 1:
                        NB_starttime[i] -= 500;
                        NB_stoptime[i] = NB_starttime[i];
                        break;
                        case 2:
                        NB_stoptime[i] -= 500;
                        break;
                        case 3:
                        NB_Road[i] --;
                        break;
                        case 0:
                        NB_Road_flag[i] --;
                        default:
                        break;
                    }
                    return;
                }
            }
            switch(ips_num)
            {

                case 42:
                    Road7_starttime -= 500;
                    Road7_stoptime = Road7_starttime;
                    break;
                case 41:
                    Road7_stoptime -= 500;
                    break;
                case 44:
                    Road1_starttime -= 500;
                    Road1_stoptime = Road1_starttime;
                    break;
                case 43:
                    Road1_stoptime -= 500;
                    break;
                case 46:
                    Road2_starttime -= 500;
                    Road2_stoptime = Road2_starttime;
                    break;
                case 45:
                    Road2_stoptime -= 500;
                    break;
                case 48:
                    Road4_starttime -= 500;
                    Road4_stoptime = Road4_starttime;
                    break;
                case 47:
                    Road4_stoptime -= 500;
                    break;
                default:
                    break;

            }
            return;
        }
    }

}

void ips_show_debug1(int ips_num)
{
    const int PAGE_X = 100;
    const int PAGE_NUM_X = 200;
    /** ips show image **/
    //ips200_clear(WHITE);
    ips200_displayimage032_zoom_gmy(Pixle[20], 80, 40, 320, 160);
    //systick_delay_ms(300);
    ips200_showstr(PAGE_X, 14, "page");
    ips200_showuint16(PAGE_NUM_X, 14, ips_num);
    
    if(ips_num == 0)
    {
        ips200_showstr(0, 12, "threshold_offset"); //ÏÔÊ¾×Ö·û´®
        ips200_showint32(0, 13, threshold_offset, 2);
        return;
    }
    for(int i = 0;i < 10 ; ++i)
    {
        if(ips_num <= i*4+4)
        {
            switch(ips_num % 4)
            {
                case 1:
                ips200_showstr(0, 12, "NB_starttime"); //ÏÔÊ¾×Ö·û´®
                ips200_showint32(150,12,i+1,2);
                ips200_showfloat(0, 13, NB_starttime[i]*0.001, 2, 2);
                break;
                case 2:
                ips200_showstr(0, 12, "NB_stoptime"); //ÏÔÊ¾×Ö·û´®
                ips200_showint32(150,12,i+1,2);
                ips200_showfloat(0, 13, NB_stoptime[i]*0.001, 2, 2);
                break;
                case 3:
                ips200_showstr(0, 12, "NB_Road"); //ÏÔÊ¾×Ö·û´®
                ips200_showint32(150,12,i+1,2);
                ips200_showint32(0, 13, NB_Road[i], 2);
                break;
                case 0:
                ips200_showstr(0, 12, "NB_Road_flag"); //ÏÔÊ¾×Ö·û´®
                ips200_showint32(150,12,i+1,2);
                ips200_showint32(0, 13, NB_Road_flag[i], 2);
                default:
                break;
            }
            return;
        }
    }
    switch(ips_num)
    {

        case 42:
            ips200_showstr(0, 12, "Road7_starttime"); //ÏÔÊ¾×Ö·û´®
            ips200_showfloat(0, 13, Road7_starttime*0.001, 2, 2);
            break;
        case 41:
            ips200_showstr(0, 12, "Road7_stoptime"); //ÏÔÊ¾×Ö·û´®
            ips200_showfloat(0, 13, Road7_stoptime*0.001, 2, 2);
            break;
        case 44:
            ips200_showstr(0, 12, "Road1_starttime"); //ÏÔÊ¾×Ö·û´®
            ips200_showfloat(0, 13, Road1_starttime*0.001, 2, 2);
            break;
        case 43:
            ips200_showstr(0, 12, "Road1_stoptime"); //ÏÔÊ¾×Ö·û´®
            ips200_showfloat(0, 13, Road1_stoptime*0.001, 2, 2);
            break;
        case 46:
            ips200_showstr(0, 12, "Road2_starttime"); //ÏÔÊ¾×Ö·û´®
            ips200_showfloat(0, 13, Road2_starttime*0.001, 2, 2);
            break;
        case 45:
            ips200_showstr(0, 12, "Road2_stoptime"); //ÏÔÊ¾×Ö·û´®
            ips200_showfloat(0, 13, Road2_stoptime*0.001, 2, 2);
            break;
        case 48:
            ips200_showstr(0, 12, "Road4_starttime"); //ÏÔÊ¾×Ö·û´®
            ips200_showfloat(0, 13, Road4_starttime*0.001, 2, 2);
            break;
        case 47:
            ips200_showstr(0, 12, "Road4_stoptime"); //ÏÔÊ¾×Ö·û´®
            ips200_showfloat(0, 13, Road4_stoptime*0.001, 2, 2);
            break;
        
        default:
            break;

    }
    
 }

void ips_show_debug0(int ips_num)
{
    const int PAGE_X = 100;
    const int PAGE_NUM_X = 200;
    /** ips show image **/
    //ips200_clear(WHITE);
    ips200_displayimage032_zoom(Image_Use[20], 80, 40, 320, 160);
    //systick_delay_ms(300);
    ips200_showstr(PAGE_X, 14, "page");
    ips200_showuint16(PAGE_NUM_X, 14, ips_num);
    switch (19 - ips_num)
    {

    /** pages **/
    /* Õý×Åµ÷ */

    case 1:
        ips200_showstr(0, 12, "competition_strategy");
        ips200_showint32(0, 13, competition_strategy, 3);
        break;
    case 2:
        ips200_showstr(0, 12, "straight_speed: "); //ÏÔÊ¾×Ö·û´®
        ips200_showfloat(0, 13, STRAIGHT_SPEED, 2, 2);
        break;
    case 3:
        ips200_showstr(0, 12, "CURVE_SPEED");
        ips200_showfloat(0, 13, CURVE_SPEED, 4, 2);
        break;
    case 4:
        ips200_showstr(0, 12, "up_ramp_speed");
        ips200_showfloat(0, 13, UP_RAMP_SPEED, 4, 2);
        break;

    case 5:
        ips200_showstr(0, 12, "on_ramp_speed");
        ips200_showfloat(0, 13, ON_RAMP_SPEED, 4, 2);
        break;

    case 6:
        ips200_showstr(0, 12, "pre_stop_speed");
        ips200_showfloat(0, 13, PRE_STOP_SPEED, 4, 2);
        break;
    case 7:
        ips200_showstr(0, 12, "rush_stop_speed");
        ips200_showfloat(0, 13, RUSH_STOP_SPEED, 4, 2);
        break;
    case 8:
        ips200_showstr(0, 12, "emergency_stop_speed");
        ips200_showfloat(0, 13, EMERGENCY_STOP_SPEED, 4, 2);
        break;
    case 9:
        ips200_showstr(0, 12, "Road");
        ips200_showint32(0, 13, Road, 3);
        break;
    case 10:
        ips200_showstr(0, 12, "Road       _flag");
        ips200_showint32(12, 12, Road, 3);
        switch (Road)
        {
        case 0:
            ips200_showint32(0, 13, Road0_flag, 3);
            break;
        case 1:
            ips200_showint32(0, 13, Road1_flag, 3);
            break;
        case 2:
            ips200_showint32(0, 13, Road2_flag, 3);
            break;
        case 3:
            ips200_showint32(0, 13, Road3_flag, 3);
            break;
        case 4:
            ips200_showint32(0, 13, Road4_flag, 3);
            break;
        case 7:
            ips200_showint32(0, 13, Road7_flag, 3);
            break;
        default:
            break;
        }
        break;

        /* µ¹×Åµ÷ */
    case 19:
        if (barn_state)
        {
            ips200_showstr(0, 12, "L_barn");
        }
        else
        {
            ips200_showstr(0, 12, "R_barn");
        }
        break;
    case 18:
        ips200_showstr(0, 12, "car_straight_k");
        ips200_showfloat(0, 13, car_straight_k, 4, 2); //PID_TURN_CAM_EXT.P, 4, 2);
        break;
    case 17:
        ips200_showstr(0, 12, "car_straight_b");
        ips200_showfloat(0, 13, car_straight_b, 4, 2); //PID_TURN_CAM_EXT.D, 4, 2);
        break;
    case 16:
        ips200_showstr(0, 12, "PID_diff.P");
        ips200_showfloat(0, 13, PID_diff.P, 4, 2);
        break;
    case 15:
        ips200_showstr(0, 12, "PID_diff0.P");
        ips200_showfloat(0, 13, PID_diff0.P, 4, 2);
        break;
    case 14:
        ips200_showstr(0, 12, "PID_CAR_STRAIGHT_CAM.D");
        ips200_showfloat(0, 13, PID_CAR_STRAIGHT_CAM.D, 4, 2);
        break;
    case 13:
        ips200_showstr(0, 12, "If_Use_EM_On_Ramp");
        ips200_showint32(0, 13, If_Use_EM_On_Ramp, 3); //PID_TURN_CAM_EXT.P, 4, 2);
        break;
    case 12:
        ips200_showstr(0, 12, "If_Use_EM_Stop");
        ips200_showint32(0, 13, If_Use_EM_Stop, 3); //PID_TURN_CAM_EXT.P, 4, 2);
        break;
    case 11:
        ips200_showstr(0, 12, "ramp_ANGLE_3");
        ips200_showfloat(0, 13, ramp_ANGLE_3, 4, 2);
        break;
    default:

        break;
    }
}
