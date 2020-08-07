/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		isr
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		IAR 8.3 or MDK 5.24
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/

#include "headfile.h"

void CSI_IRQHandler(void)
{
    CSI_DriverIRQHandler(); //调用SDK自带的中断函数 这个函数最后会调用我们设置的回调函数
    __DSB();                //数据同步隔离
}

uint16 loop_time = 0;
//float loop_time2 = 0;
//int feisu_flag = 0;
float loop_distance = 0;

void PIT_IRQHandler(void)
{
    if (PIT_FLAG_GET(PIT_CH0))
    {
        PIT_FLAG_CLEAR(PIT_CH0);
        wosijile += 16;
        if (Road != 3 && loop_time > 500 && Road7_flag != 2 && Road7_flag != 3)
        {
            if ((EM_Value_2 < 0.3 && EM_Value_3 < 0.3 && EM_Value_1 < 0.3 && EM_Value_4 < 0.3)||loop_time > stop_time*1000)
            //if(loop_time > 1500)
            {
                lib_speed_set(0);
            }
        }
        EM_Get();
        // if (DEBUG_CHOICE == 3)
        // {
        //     Kalman_Filter();
        // }
        // else if (DEBUG_CHOICE == 1 || DEBUG_CHOICE == 2)
        // {
        //     Turn_Servo();
        // }
        // Turn_diff_comp();
        if (Road == 4)
        {
            EM_main();
            Turn_Out = Turn_EM_Out;
            Servo_Duty(-Turn_Out);
        }
        if (gpio_get(DEBUG_KEY0))
        {
            loop_time += 2; //ms
            //loop_time2 += 0.002;
            loop_distance += CarSpeed * 2; //mm
        }

        ICM_main_isr();

        // ICM_get();
        Get_Speed();
        Turn_Cam_New();
        Turn_Servo_Normal();
        // lib_set_fun();
        SpeedTarget_fig();
        //    BBC();
        Speed_Control_New();
        BB_add();
        Moto_Out();
        Mean_Turn_Out();
        wosijile -=16;
    }

    __DSB();
}

// void GPIO2_Combined_16_31_IRQHandler(void)
// {

//     CLEAR_GPIO_FLAG(C23); //清除中断标志位

//     ganhuangguan_flag = 1;

//     gpio_interrupt_init(C23, NO_INT, GPIO_INT_CONFIG);
// }

void GPIO2_Combined_0_15_IRQHandler(void)
{
    mt9v03x_vsync();
    //scc8660_vsync();
}

/*
中断函数名称，用于设置对应功能的中断函数
Sample usage:当前启用了周期定时器中断
void PIT_IRQHandler(void)
{
    //务必清除标志位
    __DSB();
}
记得进入中断后清除标志位
CTI0_ERROR_IRQHandler
CTI1_ERROR_IRQHandler
CORE_IRQHandler
FLEXRAM_IRQHandler
KPP_IRQHandler
TSC_DIG_IRQHandler
GPR_IRQ_IRQHandler
LCDIF_IRQHandler
CSI_IRQHandler
PXP_IRQHandler
WDOG2_IRQHandler
SNVS_HP_WRAPPER_IRQHandler
SNVS_HP_WRAPPER_TZ_IRQHandler
SNVS_LP_WRAPPER_IRQHandler
CSU_IRQHandler
DCP_IRQHandler
DCP_VMI_IRQHandler
Reserved68_IRQHandler
TRNG_IRQHandler
SJC_IRQHandler
BEE_IRQHandler
PMU_EVENT_IRQHandler
Reserved78_IRQHandler
TEMP_LOW_HIGH_IRQHandler
TEMP_PANIC_IRQHandler
USB_PHY1_IRQHandler
USB_PHY2_IRQHandler
ADC1_IRQHandler
ADC2_IRQHandler
DCDC_IRQHandler
Reserved86_IRQHandler
Reserved87_IRQHandler
GPIO1_INT0_IRQHandler
GPIO1_INT1_IRQHandler
GPIO1_INT2_IRQHandler
GPIO1_INT3_IRQHandler
GPIO1_INT4_IRQHandler
GPIO1_INT5_IRQHandler
GPIO1_INT6_IRQHandler
GPIO1_INT7_IRQHandler
GPIO1_Combined_0_15_IRQHandler
GPIO1_Combined_16_31_IRQHandler
GPIO2_Combined_0_15_IRQHandler
GPIO2_Combined_16_31_IRQHandler
GPIO3_Combined_0_15_IRQHandler
GPIO3_Combined_16_31_IRQHandler
GPIO4_Combined_0_15_IRQHandler
GPIO4_Combined_16_31_IRQHandler
GPIO5_Combined_0_15_IRQHandler
GPIO5_Combined_16_31_IRQHandler
WDOG1_IRQHandler
RTWDOG_IRQHandler
EWM_IRQHandler
CCM_1_IRQHandler
CCM_2_IRQHandler
GPC_IRQHandler
SRC_IRQHandler
Reserved115_IRQHandler
GPT1_IRQHandler
GPT2_IRQHandler
PWM1_0_IRQHandler
PWM1_1_IRQHandler
PWM1_2_IRQHandler
PWM1_3_IRQHandler
PWM1_FAULT_IRQHandler
SEMC_IRQHandler
USB_OTG2_IRQHandler
USB_OTG1_IRQHandler
XBAR1_IRQ_0_1_IRQHandler
XBAR1_IRQ_2_3_IRQHandler
ADC_ETC_IRQ0_IRQHandler
ADC_ETC_IRQ1_IRQHandler
ADC_ETC_IRQ2_IRQHandler
ADC_ETC_ERROR_IRQ_IRQHandler
PIT_IRQHandler
ACMP1_IRQHandler
ACMP2_IRQHandler
ACMP3_IRQHandler
ACMP4_IRQHandler
Reserved143_IRQHandler
Reserved144_IRQHandler
ENC1_IRQHandler
ENC2_IRQHandler
ENC3_IRQHandler
ENC4_IRQHandler
TMR1_IRQHandler
TMR2_IRQHandler
TMR3_IRQHandler
TMR4_IRQHandler
PWM2_0_IRQHandler
PWM2_1_IRQHandler
PWM2_2_IRQHandler
PWM2_3_IRQHandler
PWM2_FAULT_IRQHandler
PWM3_0_IRQHandler
PWM3_1_IRQHandler
PWM3_2_IRQHandler
PWM3_3_IRQHandler
PWM3_FAULT_IRQHandler
PWM4_0_IRQHandler
PWM4_1_IRQHandler
PWM4_2_IRQHandler
PWM4_3_IRQHandler
PWM4_FAULT_IRQHandler
Reserved171_IRQHandler
GPIO6_7_8_9_IRQHandler*/
