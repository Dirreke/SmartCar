#include "headfile.h"
#include "debug.h"

void Debug_Init(void)
{
    gpio_init(D4, GPI, 1, PULLUP_22K);
    gpio_init(D27, GPI, 1, PULLUP_22K);
    gpio_init(C31, GPI, 1, PULLUP_22K);
    gpio_init(C27, GPI, 1, PULLUP_22K);
    gpio_init(C26, GPI, 1, PULLUP_22K);
}

void Dubug_key(void)
{
    static int ips_num = 0;
    const int page_num = 2;
    if (gpio_get(DEBUG_KEY0))
    {
        return;
    }
    //显示
    ips_show_debug(ips_num);
    //翻页
    if (!gpio_get(DEBUG_KEY1))
    {
        systick_delay_ms(50);
        if (!gpio_get(DEBUG_KEY1))
        {
            ips_num++;
            if (ips_num >= page_num)
            {
                ips_num = 0;
            }
            return;
        }
    }

    if (!gpio_get(DEBUG_KEY2))
    {
        systick_delay_ms(50);
        if (!gpio_get(DEBUG_KEY2))
        {
            // ips_clear(IPS_WHITE);
            ips_num--;
            if (ips_num < 0)
            {
                ips_num = page_num - 1;
            }
            return;
        }
    }
    //调整
    if (!gpio_get(DEBUG_KEY3))
    {
        systick_delay_ms(50);
        if (!gpio_get(DEBUG_KEY3))
        {
            switch (ips_num)
            {
            case 0:
                threshold_offset += 1;
                break;
            case 1:
                lib_set_speed(get_speed() + 0.1);
                break;
            /* 舵机PD仅调参用*/
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
            switch (ips_num)
            {
            case 0:
                threshold_offset -= 1;
                break;
            case 1:
                lib_set_speed(get_speed() - 0.1);
                break;
                /* 舵机PD仅调参用*/
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
            default:
                break;
            }
            return;
        }
    }
}

void ips_show_debug(int ips_num)
{
    const int PAGE_X = 100;
    const int PAGE_NUM_X = 200;
    /** ips show image **/
    ips200_clear(WHITE);
    ips200_displayimage032_zoom(Image_Use[0], 80, 40, 320, 160);

    ips200_showstr(PAGE_X, 14, "page");
    ips200_showuint16(PAGE_NUM_X, 14, ips_num);
    switch (ips_num)
    {

    /** pages **/
    case 0:                                          //阈值
        ips200_showstr(0, 12, "threshold_offset: "); //显示字符串
        //ips200_showfloat(0, 13, 66.6667, 2, 4);    //显示一个浮点数并去除整数部分无效0
        //ips200_showuint16(0,1,666);                //显示一个16位无符号整数
        //ips200_showint32(0,3,-666,3);              //显示一个32位有符号数并去除无效0
        ips200_showint32(0, 13, threshold_offset, 3);
        break;
    case 1:
        ips200_showstr(0, 12, "speed: "); //显示字符串
        ips200_showfloat(0, 13, get_speed(), 2, 2);
        break;
    default:
        ips_show_debug_pd(ips_num);
        break;
    }
}
void ips_show_debug_pd(int ips_num)
{
    int index = ips_num / 2 - 1;
    ips200_showstr(0, 11, "index:");
    ips200_showuint16(100, 11, index);
    if (ips_num % 2)
    {
        ips200_showstr(0, 12, "Turn_Cam_P_Table: ");
        ips200_showfloat(0, 13, Turn_Cam_P_Table0[index], 2, 3);
    }
    else
    {
        ips200_showstr(0, 12, "Turn_Cam_D_Table: ");
        ips200_showfloat(0, 13, Turn_Cam_D_Table0[index], 2, 3);
    }
}