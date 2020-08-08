#include "headfile.h"
#include "debug.h"

// int DEBUG_CHOICE = 1;
bool kaiji_flag = 0;
float Cam_P_New = 0.8;
float stop_time = 16;
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
    const int page_num0 = 20;
    const int page_num1 = 30;
    int page_num = page_num0;
    static bool qipao_flag;
    static bool guanpingmu_flag = 0;
    if (gpio_get(DEBUG_KEY0))
    {
        // Road = 0;
        if (qipao_flag == 0)
        {
            lib_speed_set(DEFAULT_SPEED);
            qipao_flag = 1;
            barn_reset_flag = 0;
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
            ips_num = 0;
            page_num = page_num0;
        }        
    }
    else
    {
        if(!ips_choice)
        {
            ips_num = 0;
            ips_choice = 1;
            page_num = page_num1;
        }
    }

    //翻页
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

    //显示
        if(ips_choice == 0)
{
    ips_show_debug0(ips_num);
}
else
{
    ips_show_debug1(ips_num);
}

    // ips_show_debug(ips_num);

    //调整
    if(ips_choice == 0)
    {
        if (!gpio_get(DEBUG_KEY3))
        {
            systick_delay_ms(50);
            if (!gpio_get(DEBUG_KEY3))
            {
                switch (ips_num)
                {
                case 2:
                    PID_CAR_CENTER_CAM.P += 0.1;
                    break;
                case 3:
                    DEFAULT_SPEED += 0.1;
                    STRAIGHT_SPEED += 0.1;
                    break;
                case 4:
                    SPEED_MOTOR_SCALE_HIGH += 100; //threshold_offset += 1;
                    break;
                case 14:
                    CURVE_SPEED += 0.1; //PID_CAR_Diffcomp_CAM.P += 0.1;
                    break;
                case 6:
                    //PID_TURN_CAM_EXT.P += 0.1;
                    car_straight_b += 0.01;//PID_CAR_STRAIGHT_CAM.P += 0.01;
                    break;
                case 7:
                    PID_SPEED.P += 1;
                    //PID_TURN_CAM_EXT.D += 0.1;
                    //PID_CAR_STRAIGHT_CAM.D += 0.01;
                    break;
                case 8:
                    PID_SPEED.I += 1;
                    //DEBUG_CHOICE++;
                    break;
                case 9:
                    PID_CAR_STRAIGHT_CAM.D += 0.1;
                    break;
                case 10:
                    PID_diff.P += 0.1;
                    break;
                case 11:
                    PID_diff0.P += 0.1;
                    break;
                case 12:
                    barn_state = !barn_state;
                    break;
                case 13:
                    competition_strategy += 1;
                    Para_Init();
                    break;
                case 5:
                    car_straight_k += 0.01;//Cam_P_New += 0.01;
                    break;
                case 0:
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
                case 1:
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
                case 15:
                    threshold_offset += 1;
                    break;
                case 16:
                    Sobel_Threshold_FarFar += 1;
                    break;
                case 17:
                    stop_time += 0.1;
                    break;
                case 18:
                    Sobel_Threshold_Far += 1;
                    break;
                case 19:
                    Sobel_Threshold_Near += 1;
                    break;
                    /* 

                case 16:
                    Sobel_Threshold_Far += 1;
                    break;
                case 17:
                    Sobel_Threshold_Near += 1;
                    break; */

                /* 舵机PD仅调参用
                case 2:
                    Turn_Cam_P_Table0[0] += 0.01;
                    break;
                case 3:
                    Turn_Cam_D_Table0[0] += 0.01;
                    break;
                case 4:
                    Turn_Cam_P_Table0[1] += 0.01;
                    break;
                case 5:
                    Turn_Cam_D_Table0[1] += 0.01;
                    break;
                case 6:
                    Turn_Cam_P_Table0[2] += 0.01;
                    break;
                case 7:
                    Turn_Cam_D_Table0[2] += 0.01;
                    break;
                case 8:
                    Turn_Cam_P_Table0[3] += 0.01;
                    break;
                case 9:
                    Turn_Cam_D_Table0[3] += 0.01;
                    break;
                case 10:
                    Turn_Cam_P_Table0[4] += 0.01;
                    break;
                case 11:
                    Turn_Cam_D_Table0[4] += 0.01;
                    break;
                case 12:
                    Turn_Cam_P_Table0[5] += 0.01;
                    break;
                case 13:
                    Turn_Cam_D_Table0[5] += 0.01;
                    break;
                case 14:
                    Turn_Cam_P_Table0[6] += 0.01;
                    break;
                case 15:
                    Turn_Cam_D_Table0[6] += 0.01;
                    break;
                case 16:
                    Turn_Cam_P_Table0[7] += 0.01;
                    break;
                case 17:
                    Turn_Cam_D_Table0[7] += 0.01;
                    break;
                case 18:
                    Turn_Cam_P_Table0[8] += 0.01;
                    break;
                case 19:
                    Turn_Cam_D_Table0[8] += 0.01;
                    break;
                case 20:
                    Turn_Cam_P_Table0[9] += 0.01;
                    break;
                case 21:
                    Turn_Cam_D_Table0[9] += 0.01;
                    break;
                case 22:
                    Turn_Cam_P_Table0[10] += 0.01;
                    break;
                case 23:
                    Turn_Cam_D_Table0[10] += 0.01;
                    break;
                case 24:
                    Turn_Cam_P_Table0[11] += 0.01;
                    break;
                case 25:
                    Turn_Cam_D_Table0[11] += 0.01;
                    break;
                case 26:
                    Turn_Cam_P_Table0[12] += 0.01;
                    break;
                case 27:
                    Turn_Cam_D_Table0[12] += 0.01;
                    break;
                case 28:
                    Turn_Cam_P_Table0[13] += 0.01;
                    break;
                case 29:
                    Turn_Cam_D_Table0[13] += 0.01;
                    break;
                case 30:
                    Turn_Cam_P_Table0[14] += 0.01;
                    break;
                case 31:
                    Turn_Cam_D_Table0[14] += 0.01;
                    break;
    */
                default:
                    // if (ips_num < 21)
                    // {
                    //     Turn_Cam_Straight_P_Table[ips_num - 10] += 0.05;
                    // }
                    // else if (ips_num < 32)
                    // {
                    //     Turn_Cam_Straight_D_Table[ips_num - 21] += 0.05;
                    // }
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
                switch (ips_num)
                {
                case 2:
                    PID_CAR_CENTER_CAM.P -= 0.1;
                    break;
                case 3:
                    //DEFAULT_SPEED -= 0.1;
                    STRAIGHT_SPEED -= 0.1;
                    break;
                case 4:
                    SPEED_MOTOR_SCALE_HIGH -= 100; //threshold_offset -= 1;
                    break;
                case 14:
                    CURVE_SPEED -= 0.1; //PID_CAR_Diffcomp_CAM.P -= 0.1;
                    break;
                case 6:
                    //PID_TURN_CAM_EXT.P -= 0.1;
                    car_straight_b -= 0.01;//PID_CAR_STRAIGHT_CAM.P -= 0.01;
                    break;
                case 7:
                    PID_SPEED.P -= 1;
                    //PID_TURN_CAM_EXT.D -= 0.1;
                    //PID_CAR_STRAIGHT_CAM.D -= 0.01;
                    break;
                case 8:
                    PID_SPEED.I -= 1;
                    //DEBUG_CHOICE--;
                    break;
                case 9:
                    PID_CAR_STRAIGHT_CAM.D -= 0.1;
                    break;
                case 10:
                    PID_diff.P -= 0.1;
                    break;
                case 11:
                    PID_diff0.P -= 0.1;
                    break;
                case 12:
                    barn_state = !barn_state;
                    break;
                case 13:
                    competition_strategy -= 1;
                    Para_Init();
                    break;
                case 5:
                    car_straight_k -= 0.01;//Cam_P_New -= 0.01;
                    break;
                case 0:
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
                case 1:
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
                        break;
                    case 7:
                        Road7_flag--;
                        break;
                    default:
                        break;
                    }
                    break;

                case 15:
                    threshold_offset -= 1;
                    break;
                case 16:
                    Sobel_Threshold_FarFar -= 1;
                    break;
                case 17:
                    stop_time -= 0.1;
                    break;
                case 18:
                    Sobel_Threshold_Far -= 1;
                    break;
                case 19:
                    Sobel_Threshold_Near -= 1;
                    break;
                    /*             case 15:
                    threshold_offset -= 1;
                    break;

                case 17:
                    Sobel_Threshold_Near -= 1;
                    break; */

                    /* 舵机PD仅调参用
                case 2:
                    Turn_Cam_P_Table0[0] -= 0.01;
                    break;

                case 3:
                    Turn_Cam_D_Table0[0] -= 0.01;
                    break;
                case 4:
                    Turn_Cam_P_Table0[1] -= 0.01;
                    break;
                case 5:
                    Turn_Cam_D_Table0[1] -= 0.01;
                    break;
                case 6:
                    Turn_Cam_P_Table0[2] -= 0.01;
                    break;
                case 7:
                    Turn_Cam_D_Table0[2] -= 0.01;
                    break;
                case 8:
                    Turn_Cam_P_Table0[3] -= 0.01;
                    break;
                case 9:
                    Turn_Cam_D_Table0[3] -= 0.01;
                    break;
                case 10:
                    Turn_Cam_P_Table0[4] -= 0.01;
                    break;
                case 11:
                    Turn_Cam_D_Table0[4] -= 0.01;
                    break;
                case 12:
                    Turn_Cam_P_Table0[5] -= 0.01;
                    break;
                case 13:
                    Turn_Cam_D_Table0[5] -= 0.01;
                    break;
                case 14:
                    Turn_Cam_P_Table0[6] -= 0.01;
                    break;
                case 15:
                    Turn_Cam_D_Table0[6] -= 0.01;
                    break;
                case 16:
                    Turn_Cam_P_Table0[7] -= 0.01;
                    break;
                case 17:
                    Turn_Cam_D_Table0[7] -= 0.01;
                    break;
                case 18:
                    Turn_Cam_P_Table0[8] -= 0.01;
                    break;
                case 19:
                    Turn_Cam_D_Table0[8] -= 0.01;
                    break;
                case 20:
                    Turn_Cam_P_Table0[9] -= 0.01;
                    break;
                case 21:
                    Turn_Cam_D_Table0[9] -= 0.01;
                    break;
                case 22:
                    Turn_Cam_P_Table0[10] -= 0.01;
                    break;
                case 23:
                    Turn_Cam_D_Table0[10] -= 0.01;
                    break;
                case 24:
                    Turn_Cam_P_Table0[11] -= 0.01;
                    break;
                case 25:
                    Turn_Cam_D_Table0[11] -= 0.01;
                    break;
                case 26:
                    Turn_Cam_P_Table0[12] -= 0.01;
                    break;
                case 27:
                    Turn_Cam_D_Table0[12] -= 0.01;
                    break;
                case 28:
                    Turn_Cam_P_Table0[13] -= 0.01;
                    break;
                case 29:
                    Turn_Cam_D_Table0[13] -= 0.01;
                    break;
                case 30:
                    Turn_Cam_P_Table0[14] -= 0.01;
                    break;
                case 31:
                    Turn_Cam_D_Table0[14] -= 0.01;
                    break;
                    */
                default:
                    // if (ips_num < 21)
                    // {
                    //     Turn_Cam_Straight_P_Table[ips_num - 10] -= 0.05;
                    // }
                    // else if (ips_num < 32)
                    // {
                    //     Turn_Cam_Straight_D_Table[ips_num - 21] -= 0.05;
                    // }
                    break;
                }
                return;
            }
        }
    }
    else
    {
        ;
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
    switch (ips_num)
    {

    /** pages **/
    case 2:                                          //阈值
        ips200_showstr(0, 12, "PID_CENTER_CAM.P: "); //显示字符串
        //ips200_showfloat(0, 13, 66.6667, 2, 4);    //显示一个浮点数并去除整数部分无效0
        //ips200_showuint16(0,1,666);                //显示一个16位无符号整数
        //ips200_showint32(0,3,-666,3);              //显示一个32位有符号数并去除无效0
        ips200_showfloat(0, 13, PID_CAR_CENTER_CAM.P, 3, 2);
        break;
    case 3:
        ips200_showstr(0, 12, "straight_speed: "); //显示字符串
        ips200_showfloat(0, 13, STRAIGHT_SPEED, 2, 2);
        break;
    case 4:
        ips200_showstr(0, 12, "SPEED_MOTOR_SCALE_HIGH //threshold_offset");
        ips200_showfloat(0, 13, SPEED_MOTOR_SCALE_HIGH, 4, 2);
        break;
    case 14:
        ips200_showstr(0, 12, "CURVE_SPEED");
        ips200_showfloat(0, 13, CURVE_SPEED, 4, 2);
        break;
    case 6:
        ips200_showstr(0, 12, "fuck b");
        ips200_showfloat(0, 13, car_straight_b, 4, 2); //PID_TURN_CAM_EXT.P, 4, 2);
        break;
    case 7:
        ips200_showstr(0, 12, "motor p CamStraightD");
        ips200_showfloat(0, 13, PID_SPEED.P, 4, 2); //PID_TURN_CAM_EXT.D, 4, 2);
        break;
    case 8:
        ips200_showstr(0, 12, "motor i DEBUG_CHOICE");
        ips200_showfloat(0, 13, PID_SPEED.I, 4, 2); //PID_TURN_CAM_EXT.D, 4, 2);
        //ips200_showint32(0, 13, PID_SPEED.I, 3);
        break;
    case 9:
        ips200_showstr(0, 12, "PID_CAR_STRAIGHT_CAM.D");
        ips200_showfloat(0, 13, PID_CAR_STRAIGHT_CAM.D, 4, 2);
        break;
    case 10:
        ips200_showstr(0, 12, "PID_diff.P");
        ips200_showfloat(0, 13, PID_diff.P, 4, 2);
        break;
    case 11:
        ips200_showstr(0, 12, "PID_diff0.P");
        ips200_showfloat(0, 13, PID_diff0.P, 4, 2);
        break;
    case 12:
        if (barn_state)
        {
            ips200_showstr(0, 12, "L_barn");
        }
        else
        {
            ips200_showstr(0, 12, "R_barn");
        }
        break;
    case 13:
        ips200_showstr(0, 12, "competition_strategy");
        ips200_showint32(0, 13, competition_strategy, 3);
        break;
    case 5:
        ips200_showstr(0, 12, "fuck k");
        ips200_showfloat(0, 13, car_straight_k, 4, 2);
        break;
    case 0:
        ips200_showstr(0, 12, "Road");
        ips200_showint32(0, 13, Road, 3);
        break;
    case 1:
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

    case 15:
        ips200_showstr(0, 12, "threshold_offset: ");
        ips200_showint32(0, 13, threshold_offset, 3);
        break;
    case 16:
        ips200_showstr(0, 12, "Sobel_Threshold_FarFar: ");
        ips200_showuint8(0, 13, Sobel_Threshold_FarFar);
        break;
    case 17:
        ips200_showstr(0, 12, "tingche shijian");
        ips200_showfloat(0, 13, stop_time, 4, 2);
        
        break;
    case 18:
        ips200_showstr(0, 12, "Sobel_Threshold_Far");
        ips200_showfloat(0, 13, Sobel_Threshold_Far, 4, 2);
                break;
            case 19:
        ips200_showstr(0, 12, "Sobel_Threshold_Near");
        ips200_showfloat(0, 13, Sobel_Threshold_Near, 4, 2);
                break;
        /*     
    case 17:
        ips200_showstr(0, 12, "Sobel_Threshold_Near: ");
        ips200_showuint8(0, 13, Sobel_Threshold_Near);
        break; */

    default:
        // ips_show_debug_pd(ips_num);
        // ips200_showint32(0, 11, ips_num, 2);
        // if (ips_num < 21)
        // {
        //     ips200_showstr(0, 12, "P");
        //     ips200_showfloat(0, 13, Turn_Cam_Straight_P_Table[ips_num - 10], 4, 2);
        // }
        // else if (ips_num < 32)
        // {
        //     ips200_showstr(0, 12, "D");
        //     ips200_showfloat(0, 13, Turn_Cam_Straight_D_Table[ips_num - 21], 4, 2);
        // }
        break;
    }
}


void ips_show_debug1(int ips_num)
{
    const int PAGE_X = 100;
    const int PAGE_NUM_X = 200;
    /** ips show image **/
    //ips200_clear(WHITE);
    ips200_displayimage032_zoom(Image_Use[20], 80, 40, 320, 160);
    //systick_delay_ms(300);
    ips200_showstr(PAGE_X, 14, "page");
    ips200_showuint16(PAGE_NUM_X, 14, ips_num);
    switch (ips_num)
    {

    /** pages **/
    case 2:                                          //阈值
        ips200_showstr(0, 12, "PID_CENTER_CAM.P: "); //显示字符串
        //ips200_showfloat(0, 13, 66.6667, 2, 4);    //显示一个浮点数并去除整数部分无效0
        //ips200_showuint16(0,1,666);                //显示一个16位无符号整数
        //ips200_showint32(0,3,-666,3);              //显示一个32位有符号数并去除无效0
        ips200_showfloat(0, 13, PID_CAR_CENTER_CAM.P, 3, 2);
        break;
    case 3:
        ips200_showstr(0, 12, "straight_speed: "); //显示字符串
        ips200_showfloat(0, 13, STRAIGHT_SPEED, 2, 2);
        break;
    case 4:
        ips200_showstr(0, 12, "SPEED_MOTOR_SCALE_HIGH //threshold_offset");
        ips200_showfloat(0, 13, SPEED_MOTOR_SCALE_HIGH, 4, 2);
        break;
    case 14:
        ips200_showstr(0, 12, "CURVE_SPEED");
        ips200_showfloat(0, 13, CURVE_SPEED, 4, 2);
        break;
    case 6:
        ips200_showstr(0, 12, "fuck b");
        ips200_showfloat(0, 13, car_straight_b, 4, 2); //PID_TURN_CAM_EXT.P, 4, 2);
        break;
    case 7:
        ips200_showstr(0, 12, "motor p CamStraightD");
        ips200_showfloat(0, 13, PID_SPEED.P, 4, 2); //PID_TURN_CAM_EXT.D, 4, 2);
        break;
    case 8:
        ips200_showstr(0, 12, "motor i DEBUG_CHOICE");
        ips200_showfloat(0, 13, PID_SPEED.I, 4, 2); //PID_TURN_CAM_EXT.D, 4, 2);
        //ips200_showint32(0, 13, PID_SPEED.I, 3);
        break;
    case 9:
        ips200_showstr(0, 12, "PID_CAR_STRAIGHT_CAM.D");
        ips200_showfloat(0, 13, PID_CAR_STRAIGHT_CAM.D, 4, 2);
        break;
    case 10:
        ips200_showstr(0, 12, "PID_diff.P");
        ips200_showfloat(0, 13, PID_diff.P, 4, 2);
        break;
    case 11:
        ips200_showstr(0, 12, "PID_diff0.P");
        ips200_showfloat(0, 13, PID_diff0.P, 4, 2);
        break;
    case 12:
        if (barn_state)
        {
            ips200_showstr(0, 12, "L_barn");
        }
        else
        {
            ips200_showstr(0, 12, "R_barn");
        }
        break;
    case 13:
        ips200_showstr(0, 12, "competition_strategy");
        ips200_showint32(0, 13, competition_strategy, 3);
        break;
    case 5:
        ips200_showstr(0, 12, "fuck k");
        ips200_showfloat(0, 13, car_straight_k, 4, 2);
        break;
    case 0:
        ips200_showstr(0, 12, "Road");
        ips200_showint32(0, 13, Road, 3);
        break;
    case 1:
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

    case 15:
        ips200_showstr(0, 12, "threshold_offset: ");
        ips200_showint32(0, 13, threshold_offset, 3);
        break;
    case 16:
        ips200_showstr(0, 12, "Sobel_Threshold_FarFar: ");
        ips200_showuint8(0, 13, Sobel_Threshold_FarFar);
        break;
    case 17:
        ips200_showstr(0, 12, "tingche shijian");
        ips200_showfloat(0, 13, stop_time, 4, 2);
        
        break;
    case 18:
        ips200_showstr(0, 12, "Sobel_Threshold_Far");
        ips200_showfloat(0, 13, Sobel_Threshold_Far, 4, 2);
                break;
            case 19:
        ips200_showstr(0, 12, "Sobel_Threshold_Near");
        ips200_showfloat(0, 13, Sobel_Threshold_Near, 4, 2);
                break;
        /*     
    case 17:
        ips200_showstr(0, 12, "Sobel_Threshold_Near: ");
        ips200_showuint8(0, 13, Sobel_Threshold_Near);
        break; */

    default:
        // ips_show_debug_pd(ips_num);
        // ips200_showint32(0, 11, ips_num, 2);
        // if (ips_num < 21)
        // {
        //     ips200_showstr(0, 12, "P");
        //     ips200_showfloat(0, 13, Turn_Cam_Straight_P_Table[ips_num - 10], 4, 2);
        // }
        // else if (ips_num < 32)
        // {
        //     ips200_showstr(0, 12, "D");
        //     ips200_showfloat(0, 13, Turn_Cam_Straight_D_Table[ips_num - 21], 4, 2);
        // }
        break;
    }
}

/*
void ips_show_debug_pd(int ips_num)
{
    int index = ips_num / 2 - 1;
    ips200_showstr(0, 11, "index:");
    ips200_showuint16(100, 11, index);
    if (ips_num % 2)
    {
        ips200_showstr(0, 12, "Turn_Cam_D_Table: ");
        ips200_showfloat(0, 13, Turn_Cam_D_Table0[index], 2, 3);
    }
    else
    {
        ips200_showstr(0, 12, "Turn_Cam_P_Table: ");
        ips200_showfloat(0, 13, Turn_Cam_P_Table0[index], 2, 3);
    }
}
*/