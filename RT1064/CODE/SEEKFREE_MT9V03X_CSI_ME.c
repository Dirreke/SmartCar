/*********************************************************************************************************************

 * @file       		����ͷ������
 * @date       		2019-11-20
 * @note		

 ********************************************************************************************************************/

#include "headfile.h"
#include "SEEKFREE_MT9V03X_CSI_ME.h"

//������ض�����noncache������Ϊ����ͷ���ݰ�������DMA��ɵģ������ں���˵��������֪���������������ʲôʱ�򱻸��¹���
//�����������noncache��������������ͷ���ݲɼ���ɺ��ں�ȥ������������ʱ�򣬶�ȡ��������ֱ�ӳ�cache�����ȡ�Ķ�����ȥ��ȡ���º������
//�Ӷ���ɲ�ͬ�������⡣
                                                  
__ramfunc void Get_Use_Image(void);

/* AT_NONCACHEABLE_SECTION_ALIGN(uint8 image[MT9V03X_CSI_H][MT9V03X_CSI_W],4); */
uint8 Image_Use[CAMERA_H][CAMERA_W];
uint8 imgbuff_process[CAMERA_SIZE];               //���崦��ͼ���õ�����                        //
uint8 GaveValue;
uint8_t Threshold;                  //OSTU��򷨼����ͼ����ֵ
uint16 Pixle[CAMERA_H][CAMERA_W];              //��ֵ��������OLED��uint16����?//u16
uint16 FINAL[59];
uint16 adj_9034[5]={20,80,6,10,0};    //kkkl ���� ��С�ع�� �������� thresh*  plus

__ramfunc void Get_Use_Image(void)
{
	int i = 0,j = 0;
	int row = 0;
	for(i = 20; i  < 80; i+=1)  //120�У�ÿ2�вɼ�һ�У�15  74
	{
		for(j = 15;j < 174; j+=2)  //188��ȡ94��
		{        
			Image_Use[row][(j-15)>>1] = mt9v03x_csi_image[i][j];   
                 
		}     
                row+=1;
	}  
}
/*************************************************************** 
* 
* �������ƣ�uint8_t GetOSTU(uint8_t tmImage[IMAGEH][IMAGEW]) 
* ����˵��������ֵ��С 
* ����˵���� 
* �������أ���ֵ��С 
* �޸�ʱ�䣺2018��3��27�� 
* �� ע�� 
�ο���https://blog.csdn.net/zyzhangyue/article/details/45841255
      https://www.cnblogs.com/moon1992/p/5092726.html
      https://www.cnblogs.com/zhonghuasong/p/7250540.html     
Ostu������������������ͨ��ͳ������ͼ���ֱ��ͼ������ʵ��ȫ����ֵT���Զ�ѡȡ�����㷨����Ϊ��
1) �ȼ���ͼ���ֱ��ͼ������ͼ�����е����ص㰴��0~255��256��bin��ͳ������ÿ��bin�����ص�����
2) ��һ��ֱ��ͼ��Ҳ����ÿ��bin�����ص����������ܵ����ص�
3) i��ʾ�������ֵ��Ҳ��һ���Ҷȼ�����0��ʼ����
4) ͨ����һ����ֱ��ͼ��ͳ��0~i �Ҷȼ�������(��������ֵ�ڴ˷�Χ�����ؽ���ǰ������) ��ռ����ͼ��ı���w0����ͳ��ǰ�����ص�ƽ���Ҷ�u0��ͳ��i~255�Ҷȼ�������(��������ֵ�ڴ˷�Χ�����ؽ�����������) ��ռ����ͼ��ı���w1����ͳ�Ʊ������ص�ƽ���Ҷ�u1��
5) ����ǰ�����غͱ������صķ��� g = w0*w1*(u0-u1) (u0-u1)
6) i++��ת��4)��ֱ��iΪ256ʱ��������
7�������g��Ӧ��iֵ��Ϊͼ���ȫ����ֵ
ȱ��:OSTU�㷨�ڴ�����ղ����ȵ�ͼ���ʱ��Ч�������Բ��ã���Ϊ���õ���ȫ��������Ϣ��
***************************************************************/ 
uint8_t GetOSTU(uint8_t tmImage[CAMERA_H][CAMERA_W]) 
{ 
  int16_t i,j; 
  uint32_t Amount = 0; 
  uint32_t PixelBack = 0; 
  uint32_t PixelIntegralBack = 0; 
  uint32_t PixelIntegral = 0; 
  int32_t PixelIntegralFore = 0; 
  int32_t PixelFore = 0; 
  double OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // ��䷽��; 
  int16_t MinValue, MaxValue; 
  uint8_t Threshold = 0;
  uint16 HistoGram[256];              //  

  for (j = 0; j < 256; j++)  HistoGram[j] = 0; //��ʼ���Ҷ�ֱ��ͼ 
  
  for (j = 0; j < CAMERA_H; j++) 
  { 
    for (i = 0; i < CAMERA_W; i++) 
    { 
      HistoGram[tmImage[j][i]]++; //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
    } 
  } 
  
  for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++) ;        //��ȡ��С�Ҷȵ�ֵ
  for (MaxValue = 255; MaxValue > MinValue && HistoGram[MaxValue] == 0; MaxValue--) ; //��ȡ���Ҷȵ�ֵ
      
  if (MaxValue == MinValue)     return MaxValue;         // ͼ����ֻ��һ����ɫ    
  if (MinValue + 1 == MaxValue)  return MinValue;        // ͼ����ֻ�ж�����ɫ
    
  for (j = MinValue; j <= MaxValue; j++)    Amount += HistoGram[j];        //  ��������
    
  PixelIntegral = 0;
  for (j = MinValue; j <= MaxValue; j++)
  {
    PixelIntegral += HistoGram[j] * j;//�Ҷ�ֵ����
  }
  SigmaB = -1;
  for (j = MinValue; j < MaxValue; j++)
  {
    PixelBack = PixelBack + HistoGram[j];    //ǰ�����ص���
    PixelFore = Amount - PixelBack;         //�������ص���
    OmegaBack = (double)PixelBack / Amount;//ǰ�����ذٷֱ�
    OmegaFore = (double)PixelFore / Amount;//�������ذٷֱ�
    PixelIntegralBack += HistoGram[j] * j;  //ǰ���Ҷ�ֵ
    PixelIntegralFore = PixelIntegral - PixelIntegralBack;//�����Ҷ�ֵ
    MicroBack = (double)PixelIntegralBack / PixelBack;   //ǰ���ҶȰٷֱ�
    MicroFore = (double)PixelIntegralFore / PixelFore;   //�����ҶȰٷֱ�
    Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//������䷽��
    if (Sigma > SigmaB)                    //����������䷽��g //�ҳ������䷽���Լ���Ӧ����ֵ
    {
      SigmaB = Sigma;
      Threshold = j;
    }
  }
  return Threshold;                        //���������ֵ;
} 

void Get_01_Value(void)
{
  int i = 0,j = 0;
  for(i = 0; i < LCDH; i++)
  {
    for(j = 0; j < LCDW; j++)
    {                                
      
      if(Image_Use[i][j] > 110+camera_offset) //�����ֵ   ��ֵԽ����ʾ������Խ�࣬��ǳ��ͼ��Ҳ����ʾ����    
        Pixle[i][j] =1;        
      else                                        
        Pixle[i][j] =0;
    }    
  }
}

void swap(uint8_t *p,int a,int b){
	uint8_t temp;
	temp=p[a];
	p[a]=p[b];
	p[b]=temp;
}


void sobel()//Sobel���ؼ��
{
        
	int tempx=0,tempy=0,i=0,j=0;
        double tempsqrt=0; 
        uint8 threshold;
        threshold=GetOSTU(Image_Use);
	for(i=1;i <LCDH-1; i++)
            for(j=1;j<LCDW-1;j++)
            {
                
                if(Image_Use[i][j]<threshold + threshold_offset)
                {
                  Pixle[i][j] = 0;
                if(j == 40)
                  FINAL[i] = 0;
                  continue;
                }
		tempx=-Image_Use[i-1][j-1]-2*Image_Use[i][j-1]-Image_Use[i+1][j-1]+Image_Use[i-1][j+1]+2*Image_Use[i][j+1]+Image_Use[i+1][j+1];				
		tempy= Image_Use[i+1][j-1]+2*Image_Use[i+1][j]+Image_Use[i+1][j+1]-Image_Use[i-1][j-1]-2*Image_Use[i-1][j]-Image_Use[i-1][j+1];
                tempsqrt=sqrt(tempx*tempx+tempy*tempy);
                if(i<Sobel_Near_Far_Line && tempsqrt > Sobel_Threshold_Far && Image_Use[i][j]<200)
                {   Pixle[i][j] = 0;
                if(j == 40)
                    FINAL[i] = 0;
                }
                else if(i>=Sobel_Near_Far_Line && tempsqrt > Sobel_Threshold_Near && Image_Use[i][j]<200)
                {   Pixle[i][j] = 0;
                if(j == 40)
                    FINAL[i] = 0;
                }
                else
                {   Pixle[i][j] = 1;
                if(j == 40)
                    FINAL[i] = 1;
                }
            }
	
}
/*************************************************************************
*  �������ƣ�void Pic_particular()
*  ����˵����ͼ�����⴦��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2019.7.7
*  ��    ע��
*************************************************************************/
int Lef_edge=0,Rig_edge=0;
void Pic_particular(void)
{
  int i;
  Lef_edge=0;
  Rig_edge=0;
  for(i=Last_row;i>Fir_row-1;i--)
  {
    if(New_Lef[i]==-MIDMAP)
      Lef_edge+=1;
    if(New_Rig[i]==MIDMAP)
      Rig_edge+=1;
  }
  for(i=Last_row;i>Fir_row-1;i--)//ƽ��ֵ�������߻���
  {
      Mid[i]=(int)((New_Lef[i]+New_Rig[i])/20)+40;
  }
}

/*************************************************************************
*  �������ƣ�void Pic_main()
*  ����˵����ͼ����������
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2019.5.31
*  ��    ע��
*************************************************************************/

int circle_set = 350;
int circle_set2 = -350;
int circle_flag = 0;

void camera_dispose_main(void)//����ͷ����������
{  
     // static char Road3_cnt = 0 ; 
      Get_Use_Image(); //ͼ��Ԥ����
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
     if(Road == 6 && Road6_flag != 2)//||(Cam_Block_State ==1))
      { 
         //sobel��Ե���
        Get_01_Value();
      
      }
      else
      {
       sobel();
       if(Road ==1 && Road1_flag == 4)//||Road ==2)
       { 
          //Pic_seedfill();///���������㷨
       }
       Pic_noi_elim();//ͼ���ȥ���
      }
      Pic_DrawMid();//Ѱ�����ұ���  
      Cam_End_Detect();
      Pic_undistort();//ͼ��ȥ����
      Pic_particular();
      LR_Slope_fig();//���ұ���б�ʼ���
      Allwhite_find();//����ȫ����
      Pic_find_circle();//Ѱ�һ�״���߼��յ�
      Pic_find_leap();//Ѱ��ͻ���
      Pic_Block_Rec();
      Cam_Break_Rec();
     // Road_rec();//�������ұ���б��ʶ������
      Pic_Fix_Line();//���ߴ���
      //Pic_offset_fig();//offset����
      //Pic_offset_filter();//offset�˲� 
      
      
      //if(Road == 0)
      //{
       
        //}
     }
      
      

 
