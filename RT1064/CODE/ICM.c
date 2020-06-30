#include "headfile.h"

void ICM_get(void)
{
    get_icm20602_gyro_spi();
    get_icm20602_accdata_spi();
    //ICMx_filter();
    //ICMy_filter();
    //ICMz_filter();
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