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

uint8 test1[4] ={0x00,0xff,0x01,0x01};
uint8 test2= 0xff;
uint8 test3= 0x00;



uint8 otsuThreshold(uint8 *image, uint16 col, uint16 row)
{
    #define GrayScale 256
    uint16 width = col;
    uint16 height = row;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j, pixelSum = width * height;
    uint8 threshold = 0;
    uint8* data = image;  //指向像素数据的指针
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }

    //统计灰度级中每个像素在整幅图像中的个数  
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            pixelCount[(int)data[i * width + j]]++;  //将像素值作为计数数组的下标
        }
    }

    //计算每个像素在整幅图像中的比例  
    float maxPro = 0.0;
    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
        if (pixelPro[i] > maxPro)
        {
            maxPro = pixelPro[i];
        }
    }

    //遍历灰度级[0,255]  
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
    for (i = 0; i < GrayScale; i++)     // i作为阈值
    {
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
        for (j = 0; j < GrayScale; j++)
        {
            if (j <= i)   //背景部分  
            {
                w0 += pixelPro[j];
                u0tmp += j * pixelPro[j];
            }
            else   //前景部分  
            {
                w1 += pixelPro[j];
                u1tmp += j * pixelPro[j];
            }
        }
        u0 = u0tmp / w0;
        u1 = u1tmp / w1;
        u = u0tmp + u1tmp;
        deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
        if (deltaTmp > deltaMax)
        {
            deltaMax = deltaTmp;
            threshold = i;
        }
    }

    return threshold;
}

uint8 image_threshold;  //图像阈值
uint32 use_time;     


int main(void)
{
    //int i;
    //uint8 *p;
    uint8 k;
    
    DisableGlobalIRQ();
    board_init();   		//务必保留，本函数用于初始化MPU 时钟 调试串口
    
	systick_delay_ms(100);	//延时100ms，等待主板其他外设上电成功
	
        seekfree_wireless_init();

	mt9v03x_csi_init();		//初始化摄像头	使用CSI接口
	ips200_init();			//初始化2.0寸IPS屏幕
   
    EnableGlobalIRQ(0);
    while(1)
    {
      
        if(mt9v03x_csi_finish_flag)			//图像采集完成
        {
			mt9v03x_csi_finish_flag = 0;	//清除采集完成标志位
                        
			camera_dispose_main();
			//使用缩放显示函数，根据原始图像大小 以及设置需要显示的大小自动进行缩放或者放大显示
			//总钻风采集到的图像分辨率为 188*120 ，2.0寸IPS屏显示分辨率为 320*240 ，图像拉伸全屏显示。
            ips200_displayimage032_zoom(mt9v03x_csi_image[0], MT9V03X_CSI_W, MT9V03X_CSI_H, 320, 240);	//显示摄像头图像
            image_threshold = otsuThreshold(mt9v03x_csi_image[0],MT9V03X_CSI_W,MT9V03X_CSI_H);  //大津法计算阈值
                        //Send_Data();
                   //发送二值化图像至上位机
            p = mt9v03x_csi_image[0];
       //seekfree_wireless_send_buff(test1,sizeof(test1));//由于sizeof计算字符串的长度包含了最后一个0，因此需要减一
            //seekfree_wireless_send_buff(*Image_Use,80*60);
              /*
        for(i=0; i<MT9V03X_CSI_W*MT9V03X_CSI_H; i++)
            {
                if(p[i]>image_threshold)    seekfree_wireless_send_buff(&test3,sizeof(test3));
                else                        seekfree_wireless_send_buff(&test3,sizeof(test3));
            }
        */
        }
        seekfree_wireless_send_buff(&k,1);
        k++;
/       /  if (k>=256)
  //  k=0;
        
    //systick_delay_ms(10); 
    //}
}





