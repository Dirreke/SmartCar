/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		IAR 8.3 or MDK 5.24
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/

//整套推荐IO查看Projecct文件夹下的TXT文本

//打开新的工程或者工程移动了位置务必执行以下操作
//第一步 关闭上面所有打开的文件
//第二步 project  clean  等待下方进度条走完

// ！！！ 必看 ！！！！
// ！！！ 必看 ！！！！
// ！！！ 必看 ！！！！
//*********************************************************************************
//*********************************************************************************
//*********************************************************************************
//
// 请注意本RT1064主板例程的2.0寸屏幕引脚与开源库中的引脚定义并不相同，
// 本主板例程为了方便使用双摄，将数据端口分配为两组不同IO内的连续4个引脚。
// 若自己绘制主板，且IO数量充足时，请尽量使用同一组IO内连续8个引脚，例如 C8-C15，可以提高显示性能。
//
//*********************************************************************************
//*********************************************************************************
//*********************************************************************************

#include "headfile.h"

uint8 image_head[4] = {0x00, 0xff, 0x01, 0x01};

uint32 use_time;

int main(void)
{
    int temp_flag = 0;
    DisableGlobalIRQ();
    /** board init **/
    board_init();          //务必保留，本函数用于初始化MPU 时钟 调试串口
    systick_delay_ms(100); //延时100ms，等待主板其他外设上电成功

    /** program init **/
    //seekfree_wireless_init();  //初始化无线串口模块
    Debug_Init();
    uart_init(USART_1, 1500000, UART1_TX_B12, UART1_RX_B13);
    mt9v03x_csi_init(); //初始化摄像头	使用CSI接口
    ips200_init();      //初始化2.0寸IPS屏幕
    Motor_Init();       //电机初始化
    Servo_Init();       //舵机初始化
    pit_init();         //中断初始化，每2ms控制电机一次（中断的时间也许可以改到更小一些，因为主频有所提升）
    pit_interrupt_ms(PIT_CH0, PIT_TIME);
    qtimer_AB_init(); //解码器初始化
    EM_Init();        //ADC初始化
    gpio_init(C25, GPI, 1, GPIO_PIN_CONFIG);
    gpio_interrupt_init(C25, FALLING, GPIO_INT_CONFIG); //初始化GPIO C23 中断模式 使用默认引脚配置GPIO_INT_CONFIG

    NVIC_SetPriority(GPIO2_Combined_16_31_IRQn, 0); //设置中断优先级 范围0-15 越小优先级越高
    gyro_y_init();                                  //陀螺仪校正初始化
    Para_Init(); //各个变量初始化

    EnableGlobalIRQ(0);
    /** main loop **/

    while (1)
    {
        if (temp_flag < 1)
        {
            loop_time = -999;
        }

        if (mt9v03x_csi_finish_flag) //图像采集完成
        {
            mt9v03x_csi_finish_flag = 0; //清除采集完成标志位
            mag_find();
            ICM_main();
            camera_dispose_main();

            Send_Data();
            feisu_flag = 0;
            loop_time2 = 0;
            if (temp_flag < 100)
            {
                loop_time = 0;
                temp_flag++;
            }
            Dubug_key();

            //使用缩放显示函数，根据原始图像大小 以及设置需要显示的大小自动进行缩放或者放大显示。
            //总钻风采集到的图像分辨率为 188*120 ，2.0寸IPS屏显示分辨率为 320*240 ，图像拉伸全屏显示。
            //上位机显示的图像分辨率为 80*60 ， 采集第15-174列 ， 1-120行 ， 每两行取一行。
            //ips200_displayimage032_zoom(mt9v03x_csi_image[0], MT9V03X_CSI_W, MT9V03X_CSI_H, 320, 240);	//屏幕显示摄像头图像
            //seekfree_wireless_send_buff(image_head,sizeof(image_head));//由于sizeof计算字符串的长度包含了最后一个\0，因此需要减一。数组不用
            //seekfree_wireless_send_buff(*Image_Use,80*60);//Image_Use后80*60
        }

        //更改占空比为  百分之100*2000/PWM_DUTY_MAX  PWM_DUTY_MAX在fsl_pwm.h文件中 默认为50000
        //      占空比对应关系  0：D0  1：D1  2：D12  3：D13

        /*
        Motor_Duty(0,12500);
        Motor_Duty(1,0);

        Motor_Duty(2,0);
        Motor_Duty(3,12500);
        */

        //舵机

        //Servo_Duty(2500);
    }
}
