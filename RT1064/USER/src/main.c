/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.24
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/


//�����Ƽ�IO�鿴Projecct�ļ����µ�TXT�ı�



//���µĹ��̻��߹����ƶ���λ�����ִ�����²���
//��һ�� �ر��������д򿪵��ļ�
//�ڶ��� project  clean  �ȴ��·�����������



							// ������ �ؿ� ��������
							// ������ �ؿ� ��������
							// ������ �ؿ� ��������
//*********************************************************************************
//*********************************************************************************
//*********************************************************************************
//
// ��ע�ⱾRT1064�������̵�2.0����Ļ�����뿪Դ���е����Ŷ��岢����ͬ��
// ����������Ϊ�˷���ʹ��˫�㣬�����ݶ˿ڷ���Ϊ���鲻ͬIO�ڵ�����4�����š�
// ���Լ��������壬��IO��������ʱ���뾡��ʹ��ͬһ��IO������8�����ţ����� C8-C15�����������ʾ���ܡ�
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
    uint8* data = image;  //ָ���������ݵ�ָ��
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }

    //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���  
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            pixelCount[(int)data[i * width + j]]++;  //������ֵ��Ϊ����������±�
        }
    }

    //����ÿ������������ͼ���еı���  
    float maxPro = 0.0;
    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
        if (pixelPro[i] > maxPro)
        {
            maxPro = pixelPro[i];
        }
    }

    //�����Ҷȼ�[0,255]  
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
    for (i = 0; i < GrayScale; i++)     // i��Ϊ��ֵ
    {
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
        for (j = 0; j < GrayScale; j++)
        {
            if (j <= i)   //��������  
            {
                w0 += pixelPro[j];
                u0tmp += j * pixelPro[j];
            }
            else   //ǰ������  
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

uint8 image_threshold;  //ͼ����ֵ
uint32 use_time;     


int main(void)
{
    //int i;
    //uint8 *p;
    uint8 k;
    
    DisableGlobalIRQ();
    board_init();   		//��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���
    
	systick_delay_ms(100);	//��ʱ100ms���ȴ��������������ϵ�ɹ�
	
        seekfree_wireless_init();

	mt9v03x_csi_init();		//��ʼ������ͷ	ʹ��CSI�ӿ�
	ips200_init();			//��ʼ��2.0��IPS��Ļ
   
    EnableGlobalIRQ(0);
    while(1)
    {
      
        if(mt9v03x_csi_finish_flag)			//ͼ��ɼ����
        {
			mt9v03x_csi_finish_flag = 0;	//����ɼ���ɱ�־λ
                        
			camera_dispose_main();
			//ʹ��������ʾ����������ԭʼͼ���С �Լ�������Ҫ��ʾ�Ĵ�С�Զ��������Ż��߷Ŵ���ʾ
			//�����ɼ�����ͼ��ֱ���Ϊ 188*120 ��2.0��IPS����ʾ�ֱ���Ϊ 320*240 ��ͼ������ȫ����ʾ��
            ips200_displayimage032_zoom(mt9v03x_csi_image[0], MT9V03X_CSI_W, MT9V03X_CSI_H, 320, 240);	//��ʾ����ͷͼ��
            image_threshold = otsuThreshold(mt9v03x_csi_image[0],MT9V03X_CSI_W,MT9V03X_CSI_H);  //��򷨼�����ֵ
                        //Send_Data();
                   //���Ͷ�ֵ��ͼ������λ��
            p = mt9v03x_csi_image[0];
       //seekfree_wireless_send_buff(test1,sizeof(test1));//����sizeof�����ַ����ĳ��Ȱ��������һ��0�������Ҫ��һ
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





