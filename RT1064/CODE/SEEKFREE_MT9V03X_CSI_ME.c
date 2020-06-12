/*********************************************************************************************************************

 * @file       		摄像头处理函数
 * @date       		2019-11-20
 * @note

 ********************************************************************************************************************/

#include "headfile.h"
#include "SEEKFREE_MT9V03X_CSI_ME.h"

//数据务必定义在noncache区域，因为摄像头数据搬移是由DMA完成的，对于内核来说，它并不知道这个数组内容在什么时候被更新过，
//如果不定义在noncache区域，则会造成摄像头数据采集完成后，内核去访问这个数组的时候，读取的数据是直接冲cache里面读取的而不是去获取更新后的内容
//从而造成不同步的问题。

__ramfunc void Get_Use_Image(void);

/* AT_NONCACHEABLE_SECTION_ALIGN(uint8 image[MT9V03X_CSI_H][MT9V03X_CSI_W],4); */
uint8 Image_Use[CAMERA_H][CAMERA_W];
uint8 imgbuff_process[CAMERA_SIZE]; //定义处理图像用的数组                        //
uint8 GaveValue;
uint8_t Threshold;                //OSTU大津法计算的图像阈值
uint16 Pixle[CAMERA_H][CAMERA_W]; //二值化后用于OLED显uint16氖???//u16
uint16 FINAL[59];
uint16 adj_9034[5] = {20, 80, 6, 10, 0}; //kkkl 亮度 最小曝光度 数字增益 thresh*  plus

__ramfunc void Get_Use_Image(void)
{
  int i = 0, j = 0;
  int row = 0;

  for (i = 20; i < 80; i += 1) //60行
  {
    for (j = 15; j < 174; j += 2) //188，取94列
    {
      Image_Use[row][(j - 15) >> 1] = mt9v03x_csi_image[i][j];
    }
    row += 1;
  }
}
/***************************************************************
*
* 函数名称：uint8_t GetOSTU(uint8_t tmImage[IMAGEH][IMAGEW])
* 功能说明：求阈值大小
* 参数说明：
* 函数返回：阈值大小
* 修改时间：2018年3月27日
* 备 注：
参考：https://blog.csdn.net/zyzhangyue/article/details/45841255
      https://www.cnblogs.com/moon1992/p/5092726.html
      https://www.cnblogs.com/zhonghuasong/p/7250540.html
Ostu方法又名最大类间差方法，通过统计整个图像的直方图特性来实现全局阈值T的自动选取，其算法步骤为：
1) 先计算图像的直方图，即将图像所有的像素点按照0~255共256个bin，统计落在每个bin的像素点数量
2) 归一化直方图，也即将每个bin中像素点数量除以总的像素点
3) i表示分类的阈值，也即一个灰度级，从0开始迭代
4) 通过归一化的直方图，统计0~i 灰度级的像素(假设像素值在此范围的像素叫做前景像素) 所占整幅图像的比例w0，并统计前景像素的平均灰度u0；统计i~255灰度级的像素(假设像素值在此范围的像素叫做背景像素) 所占整幅图像的比例w1，并统计背景像素的平均灰度u1；
5) 计算前景像素和背景像素的方差 g = w0*w1*(u0-u1) (u0-u1)
6) i++；转到4)，直到i为256时结束迭代
7）将最大g相应的i值作为图像的全局阈值
缺陷:OSTU算法在处理光照不均匀的图像的时候，效果会明显不好，因为利用的是全局像素信息。
***************************************************************/
uint8_t GetOSTU(uint8_t tmImage[CAMERA_H][CAMERA_W])
{
  int16_t i, j;
  uint32_t Amount = 0;
  uint32_t PixelBack = 0;
  uint32_t PixelIntegralBack = 0;
  uint32_t PixelIntegral = 0;
  int32_t PixelIntegralFore = 0;
  int32_t PixelFore = 0;
  double OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // 类间方差;
  int16_t MinValue, MaxValue;
  uint8_t Threshold = 0;
  uint16 HistoGram[256]; //

  for (j = 0; j < 256; j++)
    HistoGram[j] = 0; //初始化灰度直方图

  for (j = START_LINE; j < CAMERA_H; j++)
  {
    for (i = 0; i < CAMERA_W; i++)
    {
      HistoGram[tmImage[j][i]]++; //统计灰度级中每个像素在整幅图像中的个数
    }
  }

  for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++)
    ; //获取最小灰度的值
  for (MaxValue = 255; MaxValue > MinValue && HistoGram[MaxValue] == 0; MaxValue--)
    ; //获取最大灰度的值

  if (MaxValue == MinValue)
    return MaxValue; // 图像中只有一个颜色
  if (MinValue + 1 == MaxValue)
    return MinValue; // 图像中只有二个颜色

  for (j = MinValue; j <= MaxValue; j++)
    Amount += HistoGram[j]; //  像素总数

  PixelIntegral = 0;
  for (j = MinValue; j <= MaxValue; j++)
  {
    PixelIntegral += HistoGram[j] * j; //灰度值总数
  }
  SigmaB = -1;
  for (j = MinValue; j < MaxValue; j++)
  {
    PixelBack = PixelBack + HistoGram[j];                                              //前景像素点数
    PixelFore = Amount - PixelBack;                                                    //背景像素点数
    OmegaBack = (double)PixelBack / Amount;                                            //前景像素百分比
    OmegaFore = (double)PixelFore / Amount;                                            //背景像素百分比
    PixelIntegralBack += HistoGram[j] * j;                                             //前景灰度值
    PixelIntegralFore = PixelIntegral - PixelIntegralBack;                             //背景灰度值
    MicroBack = (double)PixelIntegralBack / PixelBack;                                 //前景灰度百分比
    MicroFore = (double)PixelIntegralFore / PixelFore;                                 //背景灰度百分比
    Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore); //计算类间方差
    if (Sigma > SigmaB)                                                                //遍历最大的类间方差g //找出最大类间方差以及对应的阈值
    {
      SigmaB = Sigma;
      Threshold = j;
    }
  }
  return Threshold; //返回最佳阈值;
}
#if 0
void Get_01_Value(void)
{
  int i = 0, j = 0;
  for (i = 0; i < LCDH; i++)
  {
    for (j = 0; j < LCDW; j++)
    {

      if (Image_Use[i][j] > 110 + camera_offset) //大津法阈值   数值越大，显示的内容越多，较浅的图像也能显示出来
        Pixle[i][j] = 1;
      else
        Pixle[i][j] = 0;
    }
  }
}
#endif
void swap(uint8_t *p, int a, int b)
{
  uint8_t temp;
  temp = p[a];
  p[a] = p[b];
  p[b] = temp;
}

void sobel() //Sobel边沿检测
{

  int tempx = 0, tempy = 0, i = 0, j = 0;
  double tempsqrt = 0;
  uint8 threshold;
  uint8 Sobel_Threshold;
  threshold = GetOSTU(Image_Use);
  for (i = Fir_row; i < LCDH - 1; i++)
  {
    for (j = 1; j < LCDW - 1; j++)
    {

      if (Image_Use[i][j] < threshold + threshold_offset + threshold_offset2)
      {
        Pixle[i][j] = 0;
        // if (j == 40)
        // FINAL[i] = 0;
        continue;
      }
      tempx = -Image_Use[i - 1][j - 1] - 2 * Image_Use[i][j - 1] - Image_Use[i + 1][j - 1] + Image_Use[i - 1][j + 1] + 2 * Image_Use[i][j + 1] + Image_Use[i + 1][j + 1];
      tempy = Image_Use[i + 1][j - 1] + 2 * Image_Use[i + 1][j] + Image_Use[i + 1][j + 1] - Image_Use[i - 1][j - 1] - 2 * Image_Use[i - 1][j] - Image_Use[i - 1][j + 1];
      tempsqrt = sqrt(tempx * tempx + tempy * tempy);
      if (i < 30)
      {
        Sobel_Threshold = Sobel_Threshold_FarFar;
      }
      else if(i < 54)
      {
        Sobel_Threshold = Sobel_Threshold_Far;
      }else
      {
        Sobel_Threshold = Sobel_Threshold_Near;
      }
      if (tempsqrt > Sobel_Threshold && Image_Use[i][j] < 200) //i < Sobel_Near_Far_Line &&
      {
        Pixle[i][j] = 0;
        // if (j == 40)
        // FINAL[i] = 0;
      }
      else
      {
        Pixle[i][j] = 1;
        // if (j == 40)
        // FINAL[i] = 1;
      }
    }
  }
}
/*************************************************************************
*  函数名称：void Pic_particular()
*  功能说明：图像特殊处理
*  参数说明：无
*  函数返回：无
*  修改时间：2019.7.7
*  备    注：
*************************************************************************/
int Lef_edge = 0, Rig_edge = 0;
void Pic_particular(void)
{
  int i;
  Lef_edge = 0;
  Rig_edge = 0;
  for (i = 59; i > 0; i--)
  {
    if (New_Lef[i] == -MIDMAP)
      Lef_edge += 1;
    if (New_Rig[i] == MIDMAP)
      Rig_edge += 1;
  }
  // for (i = 59; i > 0; i--) //平均值法中心线绘制
  // {
  //   Mid[i] = (int)((New_Lef[i] + New_Rig[i]) / 20) + 40;
  // }
}

/*************************************************************************
*  函数名称：void Get_pic_with_edge()
*  功能说明：带边线灰度图
*  参数说明：无
*  函数返回：无
*  修改时间：2019.12.12
*  备    注：
*************************************************************************/
void Get_pic_with_edge()
{
  int i = 0;
  for (i = 0; i < 60; i++)
  {
    Image_Use[i][Lef[i]] = 0xFF;
    Image_Use[i][Mid[i]] = 0xFF;
    Image_Use[i][Rig[i]] = 0xFF;
  }
}

/*************************************************************************
*  函数名称：void camera_dispose_main(void)
*  功能说明：图像处理主函数
*  参数说明：无
*  函数返回：无
*  修改时间：2019.5.31
*  备    注：
*************************************************************************/

// int circle_set = 350;
// int circle_set2 = -350;
// int circle_flag = 0;

void camera_dispose_main(void) //摄像头处理主函数
{
  // static char Road3_cnt = 0 ;
  Get_Use_Image(); //图像预处理
  // if (Road == 0)
  // {
  //   circle_flag = 0;
  // }
  // if (Road == 1)
  // {
  //   if (circle_set < 1) //Cam_Block_Cnt == 100)
  //   {
  //     Road = 0;
  //   }
  // }
  // if (Road == 2)
  // {
  //   if (circle_set < 1) //Cam_Block_Cnt == 100)
  //   {
  //     Road = 0;
  //   }
  // }

  /*
     if(Road == 0)
     {
       Turn_Angle = 0;
       circle_flag = 0;
     }
     if(Road == 1)
     {
       if( (circle_set - Turn_Angle) <1)//Cam_Block_Cnt == 100)
    {
      Road = 0;
    }
     }
     if(Road == 2)
     {
       if( (circle_set + Turn_Angle) <1)//Cam_Block_Cnt == 100)
    {
      Road = 0;
    }

     }
     */
  /*
  if (Road == 6 && Road6_flag != 2) //||(Cam_Block_State ==1))
  {
    //sobel边缘检测
    Get_01_Value();
  }
  else
  {
*/
  sobel();
  // if (Road == 1 && Road1_flag == 4) //||Road ==2)
  // {
  //Pic_seedfill();///种子搜索算法
  //    }
  Pic_noi_elim(); //图像简单去噪点
  // }
  Pic_DrawLRside(); //寻找左右边线
                    // Cam_End_Detect();
#ifdef undistort0
  Pic_undistort(); //图像去畸变
#endif
#ifdef undistort1
  Pic_undistort(1, 1); //图像去畸变
#endif
  Pic_particular();
  LR_Slope_fig();    //左右边线斜率计算
  Allwhite_find();   //查找全白行//注释Allwhitestart2.Allwhiteend2
  Pic_find_circle(); //寻找环状黑线及拐点
  // Pic_find_leap();   //寻找突变点//没有用到，似乎在旧的圆环状态机中使用 search.c中，函数变量都注释掉了
  //Pic_Block_Rec();
  //Cam_Break_Rec();
  Road_rec();          //利用左右边线斜率识别赛道
  start_stop_rec();    //识别起跑线
  Pic_Fix_Line();      //补线处理
  Pic_DrawMid();       //计算去畸前中心线-仅上位机用
  Pic_DrawMid_und();   //计算去畸后中线
  Pic_offset_fig();    //offset计算//注释Cam_offset2
  Pic_offset_filter(); //offset滤波

  Get_pic_with_edge(); //获得带边线灰度图

  //if(Road == 0)
  //{

  //}
}

void camera_simple(void) //摄像头处理主函数
{

  Get_Use_Image(); //图像预处理
  sobel();
  Pic_DrawLRside(); //寻找左右边线
#ifdef undistort0
  Pic_undistort(); //图像去畸变
#endif
#ifdef undistort1
  Pic_undistort(1, 1); //图像去畸变
#endif
  LR_Slope_fig(); //左右边线斜率计算

  Pic_DrawMid();       //计算去畸前中心线
  Pic_DrawMid_und();   //计算去畸后中线
  Pic_offset_fig();    //offset计算//注释Cam_offset2
  Pic_offset_filter(); //offset滤波

  Get_pic_with_edge(); //获得带边线灰度图
}
