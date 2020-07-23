#include "headfile.h"

// void ICM_get(void)
// {
//     // get_icm20602_gyro_spi();
//     //get_icm20602_accdata_spi();
//     // icm_gyro_y = gyro_y_mean_filter(icm_gyro_y);
//     gyro_y_integration();
//     //ICMx_filter();
//     //ICMy_filter();
//     //ICMz_filter();
// }

/*************************************************************************
*  函数名称：int16 gyro_y_init(int16 D_new) 
*  功能说明：陀螺仪初始化校正
*  参数说明：void
*  函数返回：偏置
*  修改时间：2020.07.01
*  备    注：循环读20帧，平均值，偏置，然后初始化进while
*************************************************************************/
float icm_gyro_y_offset = 0.0;
float icm_gyro_y_w;
float icm_gyro_y_angle = 0;

void gyro_y_init(void)
{
    icm20602_init_spi();
    int num = 0;
    int sum = 0;
    //int sumz = 0;

    while (num < 500)
    {
        get_icm20602_gyro_spi();

        systick_delay_ms(2);
        ++num;
        sum += icm_gyro_y;
    }
    icm_gyro_y_offset = -sum / 500.0;
}
#if 0
/*************************************************************************
*  函数名称：int16 gyro_y_mean_filter(int16 D_new) 
*  功能说明：陀螺仪均值滤波
*  参数说明：新数据
*  函数返回：滤波结果
*  修改时间：2020.07.01
*  备    注：均值滤波母函数迁移，类型改为int （+-32768 用short可能不太够
*************************************************************************/
int16 gyro_y_mean_filter(int D_new)
{
    static int D[16];
    static int num = 0;
    static int sum = 0;

    D_new += (int)icm_gyro_y_offset;
    if (num < 16)
    {
        D[num] =  D_new;
        ++num;
        sum += D_new;
        return (int16) (sum / num);
    }
    else
    {
        sum += D_new;
        num = num % 16 + 16;
        sum -= D[num - 16];
        D[num - 16] = D_new;
        num++;
        return (int16) (sum / 16);
    }
}
#endif
#if 0
/*************************************************************************
*  函数名称：void gyro_y_omega(void) 
*  功能说明：陀螺仪角度计算
*  参数说明：
*  函数返回：
*  修改时间：2020.07.01
*  备    注：y轴角速度积分 一开始写了int16不够，改了int，然后发现哪有算角度
            只有int的。。所以用float并换算吧（2000/2^15 = 0.06103515625）
*************************************************************************/
// float icm_gyro_y_angle = 0;
// float icm_gyro_y_float = 0;
void gyro_y_integration(void)
{
    int icm_gyro_y_float;
    icm_gyro_y_float = icm_gyro_y + icm_gyro_y_offset;
    icm_guro_y_w = icm_gyro_y_float * 0.06103515625
    // if(fabs(icm_gyro_y_float) < 100)
    // {
    //    icm_gyro_y = 0;
    // }
    // icm_gyro_y_angle += icm_gyro_y_float * 0.0001220703125;
}
#endif

/*************************************************************************
*  函数名称：void gyro_y_omega(void) 
*  功能说明：陀螺仪角度计算
*  参数说明：
*  函数返回：
*  修改时间：2020.07.01
*  备    注：y轴角速度积分 
            一开始写了int16不够，改了int，然后发现哪有算角度只有int的。。
            所以用float并换算吧（2000/2^15 = 0.06103515625）
*************************************************************************/
//float icm_gyro_y_angle = 0;
//float icm_gyro_y_float = 0;
void ICM_main(void)
{

    if (Road != 4)
    {
        get_icm20602_gyro_spi();
        icm_gyro_y_angle = 0;
        icm_gyro_y_w = (icm_gyro_y + icm_gyro_y_offset) * 0.06103515625;
    }
}

void ICM_main_isr(void)
{
    if (Road == 4)
    {
        get_icm20602_gyro_spi();
        icm_gyro_y_w = (icm_gyro_y + icm_gyro_y_offset) * 0.06103515625;
        icm_gyro_y_angle += icm_gyro_y_w * 0.002;
    }
}

#if 0
/*************************************************************************
*  函数名称：void ICMx_filter(void)
*  功能说明：ICMx 滤波
*  参数说明：无
*  函数返回：无
*  修改时间：2020.06.30
*  备    注：

*************************************************************************/

void ICMx_filter(void)
{
    static int ICMx_filter[4] = {0, 0, 0, 0}; //offset滤波数组
    ICMx_filter[3] = ICMx_filter[2];
    ICMx_filter[2] = ICMx_filter[1];
    ICMx_filter[1] = ICMx_filter[0];
    ICMx_filter[0] = icm_gyro_x;
    icm_gyro_x = (int)(ICMx_filter[0] * 0.5 + ICMx_filter[1] * 0.2 + ICMx_filter[2] * 0.2 + ICMx_filter[3] * 0.1);
}
/*************************************************************************
*  函数名称：void ICMy_filter(void)
*  功能说明：ICMy 滤波
*  参数说明：无
*  函数返回：无
*  修改时间：2020.06.30
*  备    注：

*************************************************************************/

void ICMy_filter(void)
{
    static int ICMy_filter[4] = {0, 0, 0, 0}; //offset滤波数组
    ICMy_filter[3] = ICMy_filter[2];
    ICMy_filter[2] = ICMy_filter[1];
    ICMy_filter[1] = ICMy_filter[0];
    ICMy_filter[0] = icm_gyro_y;
    icm_gyro_y = (int)(ICMy_filter[0] * 0.5 + ICMy_filter[1] * 0.2 + ICMy_filter[2] * 0.2 + ICMy_filter[3] * 0.1);
}
/*************************************************************************
*  函数名称：void ICMz_filter(void)
*  功能说明：ICMz 滤波
*  参数说明：无
*  函数返回：无
*  修改时间：2020.06.30
*  备    注：

*************************************************************************/

void ICMz_filter(void)
{
    static int ICMz_filter[4] = {0, 0, 0, 0}; //offset滤波数组
    ICMz_filter[3] = ICMz_filter[2];
    ICMz_filter[2] = ICMz_filter[1];
    ICMz_filter[1] = ICMz_filter[0];
    ICMz_filter[0] = icm_gyro_z;
    icm_gyro_z = (int)(ICMz_filter[0] * 0.5 + ICMz_filter[1] * 0.2 + ICMz_filter[2] * 0.2 + ICMz_filter[3] * 0.1);
}
#endif