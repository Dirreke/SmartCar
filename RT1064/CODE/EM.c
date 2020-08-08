#include "headfile.h"
float EM_Value_1 = 0;

float EM_Value_2 = 0;
float EM_Value_3 = 0;
float EM_Value_4 = 0;

float EM_center_offset = 0;
bool If_Use_EM_Stop = 1;
bool If_Use_EM_On_Ramp = 1;
/*************************************************************************
*  �������ƣ�void EM_init(void)
*  ����˵������ų�ʼ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020.5.24
*  ��    ע��adc_init(ADCN_enum adcn, ADCCH_enum ch, ADCRES_enum resolution)
              ��ѡADCģ�顢ͨ�����ֱ��ʿɼ�zf_adc.h�ļ�

*************************************************************************/
void EM_Init(void)
{
  adc_init(ADC_1, ADC1_CH3_B14, ADC_12BIT); //ͬһ��ADCģ��ֱ���Ӧ������Ϊһ���ģ�������ò�һ���������һ����ʼ��ʱ���õķֱ�����Ч
  adc_init(ADC_1, ADC1_CH4_B15, ADC_12BIT);
  adc_init(ADC_1, ADC1_CH10_B21, ADC_12BIT);
  adc_init(ADC_1, ADC1_CH12_B23, ADC_12BIT);
}

/*************************************************************************
*  �������ƣ�void EM_get(void)
*  ����˵������ֵ�˲���ȡ��з�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020.5.24
*  ��    ע��adc_mean_filterΪADת����ֵ�˲�����
             ����������Ϊ��ֵ�˲�����
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
*  �������ƣ�void EM_offset_fig(void)
*  ����˵�����������ƫ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020.6.23
*  ��    ע��

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
*  �������ƣ�void EM_main(void)
*  ����˵�������������
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020.8.8
*  ��    ע��
*************************************************************************/
void EM_main(void)
{
  EM_center_offset_fig();
  EM_center_offset_filter();
  Turn_EM();
}
