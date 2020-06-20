#include "headfile.h"
#include "EM.h"
float EM_Value_1 = 0;
float EM_Value_10 = 0;  //电磁1本次的值
float EM_Value_11 = 0;  //电磁1上次的值
float EM_Value_12 = 0;  //电磁1上上次的值
float EM_Value_2 = 0;
float EM_Value_3 = 0;
float EM_Value_4 = 0;
float EM_Value_40 = 0; 
float EM_Value_41 = 0; 
float EM_Value_42 = 0; 

float EM_offset_buff[4]={0};
float EM_offset = 0 ;
int Curve_Cnt=0;
int EM_Peak_Time_Cnt = 0;
int EM_Ring_State = 0;
float EM_Value_23_Sum = 0;
float EM_Ring_Min = 6;
float EM_Ring_Max = 0;
float Lef_EM_Sum = 0;
float Rig_EM_Sum = 0;
float EM_2_Max = 0;
float EM_3_Max = 0;
float EM_Road2_Cnt = 0;
float EM_Road1_Cnt = 0;


int limit_em(int n, int lower, int higher){
  if(n<lower){
    return lower;
  }
  else if(n>higher){
    return higher;
  }
  else{
    return n;
  }
}

float abs_f_em(float n){
  if(n>=0) return n;
  else return -n;
}






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
  adc_init(ADC_1,ADC1_CH3_B14 ,ADC_12BIT);//同一个ADC模块分辨率应该设置为一样的，如果设置不一样，则最后一个初始化时设置的分辨率生效
  adc_init(ADC_1,ADC1_CH4_B15 ,ADC_12BIT);
  adc_init(ADC_1,ADC1_CH10_B21,ADC_12BIT);
  adc_init(ADC_1,ADC1_CH12_B23,ADC_12BIT);
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
  EM_Value_1=(int)adc_mean_filter(ADC_1,ADC1_CH3_B14 ,5);
  EM_Value_2=(int)adc_mean_filter(ADC_1,ADC1_CH4_B15 ,5);
  EM_Value_4=(int)adc_mean_filter(ADC_1,ADC1_CH10_B21,5);
  EM_Value_3=(int)adc_mean_filter(ADC_1,ADC1_CH12_B23,5);


  EM_Value_1=(float)(EM_Value_1*3.3/4096);
  EM_Value_2=(float)(EM_Value_2*3.3/4096);
  EM_Value_3=(float)(EM_Value_3*3.3/4096);
  EM_Value_4=(float)(EM_Value_4*3.3/4096);
//   if(EM_Value_1 < 0.1 && EM_Value_2 < 0.1 && EM_Value_3 < 0.1 && EM_Value_4 < 0.1 && Road != 5 && Road != 3)
//   {
//     stop_cnt++;
   
//   }
//   else
//   {
//     stop_cnt = 0;
//   }
//   if(stop_cnt > 5)
//   {
//     Road = 8;
//   }

}

void EM_Curve_Rec(void)//弯道识别
{
  if(EM_Road == 0 )//从直道到弯道的判断依据为：连续采集到五次1,4电感之差大于0.6 且电磁1增大的同时电磁4减小 或电磁1减小的同时电磁4增大
  {
    if(abs_f_em(EM_Value_1 - EM_Value_4)>0.4 && abs_f_em(EM_Value_2 - EM_Value_3)>0.3)
    {
      Curve_Cnt++;
      //电磁1的电感值增大4电感值减小或 1减小4增大
      EM_Value_12=EM_Value_11;
      EM_Value_11=EM_Value_10;
      EM_Value_10=EM_Value_1;

      EM_Value_42=EM_Value_41;
      EM_Value_41=EM_Value_40;
      EM_Value_40=EM_Value_4;

      if(Curve_Cnt == 5 && ((EM_Value_1>EM_Value_12&& EM_Value_4<EM_Value_42)||(EM_Value_4>EM_Value_42&& EM_Value_1<EM_Value_12)))
      {
        EM_Road = 4;//EM_Road=4表示弯道
        Curve_Cnt = 0;
      }
    }
    else
    {
      Curve_Cnt = 0;
    }
  }
  else if(EM_Road == 4)//从弯道到直道的判断依据为：连续采集到30次采集到1,4电感之差小于0.6，则认为进入直道
  {
    if(abs_f_em(EM_Value_1 - EM_Value_4)<0.1)
    {
      Curve_Cnt++;
      if(Curve_Cnt == 3)
      {
        EM_Road = 0;   //EM_Road=0表示直道
        Curve_Cnt = 0;
      }
    }
    else
    {
      Curve_Cnt = 0;
    }
  }
}

void EM_Ring_Rec(void){
  EM_Peak_Time_Cnt++;
  EM_Peak_Time_Cnt = limit_em(EM_Peak_Time_Cnt,0,1000);

  EM_Value_23_Sum = EM_Value_2 + EM_Value_3;
  if(EM_Value_23_Sum>5.2 && EM_Ring_State == 0 && (EM_Road==0 || EM_Road==4) )//2+3之和大于3.8，开始进行圆环状态机
  {
     EM_Ring_State = 1;
     EM_Ring_Max = EM_Value_23_Sum;
     EM_Peak_Time_Cnt = 0;
     Lef_EM_Sum = 0;
     Rig_EM_Sum = 0;
     if(EM_Value_4>EM_Value_1  )
    {
      EM_Road2_Cnt += 1000 ; 
    }
    else if(EM_Value_1>EM_Value_4  )
    {
      EM_Road1_Cnt += 1000 ;
    }
  }
  else if(EM_Ring_State == 1)//状态1下，若2+3大于峰值-0.2，则保持状态1，否则跳到状态2
  {

    if(EM_Value_4>EM_Value_1 && EM_Value_4>1.2 && abs_f_em(EM_offset)<100)
    {
      EM_Road2_Cnt += 200 - abs_f_em(EM_offset); 
    }
    else if(EM_Value_1>EM_Value_4 && EM_Value_1>1.2 && abs_f_em(EM_offset)<100)
    {
      EM_Road1_Cnt += 200 - abs_f_em(EM_offset);
    }
    if( EM_Value_23_Sum>EM_Ring_Max-0.4 )
    {
      if(EM_Value_23_Sum>EM_Ring_Max)
      {
        EM_Ring_Max = EM_Value_23_Sum;
        EM_2_Max = EM_Value_2;
        EM_3_Max = EM_Value_3;
      }
    }
    else
    {

      EM_Ring_State = 2;
      EM_Ring_Max = 0;
      EM_Ring_Min = EM_Value_23_Sum;
    }
  }
  else if(EM_Ring_State == 2) //状态2下，若2+3小于最小值+0.2，则保持状态2，否则跳到状态3
  {

    if(EM_Value_4>EM_Value_1 && EM_Value_4>1.2 && abs_f_em(EM_offset)<100)
    {
      EM_Road2_Cnt += 200 - abs_f_em(EM_offset); 
    }
    else if(EM_Value_1>EM_Value_4 && EM_Value_1>1.2 && abs_f_em(EM_offset)<100)
    {
      EM_Road1_Cnt += 200 - abs_f_em(EM_offset);
    }
    if( EM_Value_23_Sum<EM_Ring_Min+0.1)
    {
      if(EM_Value_23_Sum<EM_Ring_Min)
      {
        EM_Ring_Min = EM_Value_23_Sum;
      }
    }
    else
    {
      if(EM_Road1_Cnt > EM_Road2_Cnt)
      {
        EM_Road = 1;
        EM_Road1_Cnt = 0;
        EM_Road2_Cnt = 0;
      }
      else 
      {
        EM_Road = 2;
        EM_Road1_Cnt = 0;
        EM_Road2_Cnt = 0;
      }

      EM_Ring_State = 3;
      EM_Peak_Time_Cnt = 0;
      EM_Ring_Min = 6;
    }
  }
  else if(EM_Ring_State == 3)//状态3下，若2+3小于固定值3，则表示已经进入了圆环
  {

    if((EM_Value_23_Sum < 3.5 && EM_Road == 1 && EM_Value_1>1 && EM_Value_4<0.2)||(EM_Value_23_Sum < 3.5 && EM_Road == 2 && EM_Value_4>1 && EM_Value_1<0.2))
    {
      EM_Ring_State = 4;
    }
  }
  else if(EM_Ring_State == 4)//状态4下，若2+3大于固定值3.5，则表示又一次进入了圆环岔道处，且处于刚出环状态5
  {
    if(EM_Value_23_Sum >5)
    {
      EM_Ring_Max = EM_Value_23_Sum;
      if(EM_Road == 1 && EM_Value_1>EM_Value_4 && EM_Value_1>0.5)
      {
        EM_Ring_State = 5;
      }
      else if(EM_Road == 2 && EM_Value_1<EM_Value_4 && EM_Value_4>0.5)
      {
        EM_Ring_State = 5;
      }
    }
  }
  else if(EM_Ring_State == 5)//状态5下，若2+3小于第一个峰值减0.2，则认为已经下坡，进入状态6
  {
    if( EM_Value_23_Sum>EM_Ring_Max-0.2 )
    {
      if(EM_Value_23_Sum>EM_Ring_Max)
      {
        EM_Ring_Max = EM_Value_23_Sum;
      }
    }
    else
    {
      EM_Ring_State = 6;
      EM_Ring_Max = 0;
      EM_Ring_Min = EM_Value_23_Sum;
    }
  }
  else if(EM_Ring_State == 6)//状态6下，若2+3大于波谷+0.3，则认为爬第二个坡，进入状态7
  {
    if( EM_Value_23_Sum<EM_Ring_Min+0.3)
    {
      if(EM_Value_23_Sum<EM_Ring_Min)
      {
        EM_Ring_Min = EM_Value_23_Sum;
      }
    }
    else
    {
      EM_Ring_State = 7;
      EM_Ring_Min = 6;
    }
  }
  else if(EM_Ring_State == 7)
  {
    if( EM_Value_23_Sum < 3.2 && EM_Value_1 < 1 && EM_Value_4 < 1)
    {
      EM_Ring_State = 0;
      EM_Road = 0;
    }
  }

}










/*************************************************************************
*  函数名称：void EM_offset_fig(void)
*  功能说明：电磁中线偏差
*  参数说明：无
*  函数返回：无
*  修改时间：2020.5.24
*  备    注：

*************************************************************************/
void EM_offset_fig(void){
        EM_offset=(EM_Value_3-EM_Value_2)*100;
//可加限幅
       return ;
        
}

void EM_offset_filter(void)
{
  EM_offset_buff[3]=EM_offset_buff[2];
  EM_offset_buff[2]=EM_offset_buff[1];
  EM_offset_buff[1]=EM_offset_buff[0];
  EM_offset_buff[0]=EM_offset;
  EM_offset=EM_offset_buff[0]*0.5+EM_offset_buff[1]*0.2+EM_offset_buff[2]*0.2+EM_offset_buff[3]*0.1;
}
/*************************************************************************
*  函数名称：void EM_main(void)
*  功能说明：电磁主函数
*  参数说明：无
*  函数返回：无
*  修改时间：2019.5.13
*  备    注：
*************************************************************************/
void EM_main(void)
{
  EM_Get();
  // EM_Ring_Rec();
  // //EM_Ramp_Rec();
  // EM_Curve_Rec();
  // //EM_Speed_Control();
  // EM_offset_fig();
  // //Block_By_Pass();
  // EM_offset_filter();
}
