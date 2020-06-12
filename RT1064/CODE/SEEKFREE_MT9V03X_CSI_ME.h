/*********************************************************************************************************************

 * @file       		摄像头总处理函数
 * @date       		2019-11-20
 * @note		

 ********************************************************************************************************************/


#ifndef _SEEKFREE_MT9V03X_CSI_ME_h
#define _SEEKFREE_MT9V03X_CSI_ME_h


#include "SEEKFREE_MT9V03X_CSI.h"

/** definition **/

#define Sobel_Near_Far_Line 30
extern uint8 Sobel_Threshold_FarFar;
extern uint8 Sobel_Threshold_Far; 
extern uint8 Sobel_Threshold_Near;   

#define IMAGEH  120  //行 HEIGHT 待采集摄像头图像高度行数
#define IMAGEW  188  //列 WIDTH  待采集摄像头图像宽度列数

#define CAMERA_W 80
#define CAMERA_H 60

#define LCDH 60
#define LCDW 80

#define MAX_ROW   60
#define MAX_COL   94

#define CAMERA_DMA_NUM  CAMERA_W*CAMERA_H/8
#define CAMERA_SIZE     CAMERA_DMA_NUM

/** varialbes **/
extern uint8_t Threshold;

/** declaration **/
extern uint8 Image_Use[CAMERA_H][CAMERA_W];
extern uint8 imgbuff_process[CAMERA_SIZE];                       //定义存储接收图像的数组
extern uint8 img_edg[CAMERA_SIZE*2];    
// extern int circle_flag ;
//extern uint8 LMR[3][CAMERA_H];
//extern uint8 new_img;
extern uint8 GaveValue;
extern int thresh;
extern uint16 Pixle[CAMERA_H][CAMERA_W]; 		//change to uint8
// extern uint16 FINAL[59];
void Get_pic_with_edge(void);
void Get_Use_Image(void);
// void Get_01_Value(void);
void Test_V032(void);

void Pic_particular(void);
void camera_dispose_main(void);
void camera_simple(void);


#endif