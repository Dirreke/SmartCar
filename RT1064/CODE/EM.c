#include "headfile.h"
float EM_Value_1 = 0;

float EM_Value_2 = 0;
float EM_Value_3 = 0;
float EM_Value_4 = 0;

float EM_center_offset = 0;
bool If_Use_EM_Stop = 1;
bool If_Use_EM_On_Ramp = 1;
/*************************************************************************
*  函数名称：void EM_init(void)
*  功能说明：电磁初始化
*  参数说明：无
*  函数返回：无
*  修改时间：2020.5.24
*  备    注：adc_init(ADCN_enum adcn, ADCCH_enum ch, ADCRES_enum resolution)
              可选ADC模块、通道、分辨率可见zf_adc.h文件

*************************************************************************/
void EM_Init(void)
{
  adc_init(ADC_1, ADC1_CH3_B14, ADC_12BIT); //同一个ADC模块分辨率应该设置为一样的，如果设置不一样，则最后一个初始化时设置的分辨率生效
  adc_init(ADC_1, ADC1_CH4_B15, ADC_12BIT);
  adc_init(ADC_1, ADC1_CH10_B21, ADC_12BIT);
  adc_init(ADC_1, ADC1_CH12_B23, ADC_12BIT);
}

/*************************************************************************
*  函数名称：void EM_get(void)
*  功能说明：均值滤波获取电感反馈量
*  参数说明：无
*  函数返回：无
*  修改时间：2020.5.24
*  备    注：adc_mean_filter为AD转换均值滤波函数
             参数第三项为均值滤波次数
*************************************************************************/
// int stop_cnt;
void EM_Get(void)
{
  EM_Value_1 = (int)adc_mean_filter(ADC_1, ADC1_CH3_B14, 5);
  EM_Value_2 = (int)adc_mean_filter(ADC_1, ADC1_CH4_B15, 5);
  EM_Value_4 = (int)adc_mean_filter(ADC_1, ADC1_CH10_B21, 5);
  EM_Value_3 = (int)adc_mean_filter(ADC_1, ADC1_CH12_B23, 5);

  EM_Value_1 = (float)(EM_Value_1 * 3.3 / 4096);
  EM_Value_2 = (float)(EM_Value_2 * 3.3 / 4096);
  EM_Value_3 = (float)(EM_Value_3 * 3.3 / 4096);
  EM_Value_4 = (float)(EM_Value_4 * 3.3 / 4096);
}

/*************************************************************************
*  函数名称：void EM_offset_fig(void)
*  功能说明：电磁中线偏差
*  参数说明：无
*  函数返回：无
*  修改时间：2020.6.23
*  备    注：

*************************************************************************/
void EM_center_offset_fig(void)
{
  if (EM_Value_2 < 2.85 && EM_Value_3 < 2.85)
  {
    EM_center_offset = ((5.05 * EM_Value_3 - 7.56) / (EM_Value_3 + 0.4901) - (5.05 * EM_Value_2 - 7.56) / (EM_Value_2 + 0.4901)) / 2;
  }
  else if (EM_Value_2 >= 2.85 && EM_Value_3 < 2.85)
  {
    EM_center_offset = (5.05 * EM_Value_3 - 7.56) / (EM_Value_3 + 0.4901);
  }
  else if (EM_Value_3 >= 2.85 && EM_Value_2 < 2.85)
  {
    EM_center_offset = -(5.05 * EM_Value_2 - 7.56) / (EM_Value_2 + 0.4901);
  }
  else
  {
    EM_center_offset = 999;
  }
}

void EM_center_offset_filter(void)
{
  static float EM_center_offset_buff[4];
  EM_center_offset_buff[3] = EM_center_offset_buff[2];
  EM_center_offset_buff[2] = EM_center_offset_buff[1];
  EM_center_offset_buff[1] = EM_center_offset_buff[0];
  EM_center_offset_buff[0] = EM_center_offset;
  EM_center_offset = EM_center_offset_buff[0] * 0.5 + EM_center_offset_buff[1] * 0.2 + EM_center_offset_buff[2] * 0.2 + EM_center_offset_buff[3] * 0.1;
}

/*************************************************************************
*  函数名称：void EM_main(void)
*  功能说明：电磁主函数
*  参数说明：无
*  函数返回：无
*  修改时间：2020.8.8
*  备    注：
*************************************************************************/
void EM_main(void)
{
  EM_center_offset_fig();
  EM_center_offset_filter();
  Turn_EM();
}
