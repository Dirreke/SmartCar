#include "Data_Send.h"
#include "search.h"

#define ori_pic

uint16 Pic_new[600];
int cnt;
int cntt=1;
uint8 ch=0;
float temp1=0;
float Variable[20];


void Send_Data(void){
    Pic_send_new();
    Variable_update();
    Send_Begin();
    Send_Variable();
    Send_Img();
}





/*************************************************************************
*  �������ƣ�void Send_Begin(void)
*  ����˵����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2019.3.``````````````````````````````````````````
*  ��    ע��

*************************************************************************/
void Send_Begin(void)
{
  My_Put_Char(0x55);
  My_Put_Char(0xaa);
  My_Put_Char(0x11);
}

/*************************************************************************
*  �������ƣ�void Send_Img(void)
*  ����˵����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2019.3.6
*  ��    ע��

*************************************************************************/


void Send_Img(void)
{

    uint16 i=0,num;
    My_Put_Char(0x55);
    My_Put_Char(0xaa);
    My_Put_Char(0xff);
    My_Put_Char(0xa2);
    My_Put_Char(0x01); //С��״̬

    num = cnt + 2 +180;//4+4*2
    //num = cnt+2+180;
    //num=cont+2+180+36;
    //ͳ�ƽ�Ҫ������������ 2����ΪҪ�����ؼ��ּ�0xf0��0xf2   (0xf2 �������ݽ�����0xf0ͼ�����ݽ�����180=60*3)
    //180�Ǳ��ߵ�λ 36�Ǳ�����λ �����������Ͳ�Ҫ���ϣ�

    My_Put_Char(BYTE0(num));
    My_Put_Char(BYTE1(num));
    for(i=0;i<cnt;i++)
    {
      My_Put_Char(Pic_new[i]);
    }
//    for(i=0;i< 60;i++)
//    {
//            My_Put_Char(Lef[i]);
//            My_Put_Char(Rig[i]);
//            My_Put_Char(0xff);
//    }
    My_Put_Char(0xf0);  //����ͼ�����ݷ�����
    /******************�Ǻ�Χ�����Ŀ��Բ�����*******************/
    ////////////////////////�������ұ��ߺͼ����õ�������///////////
    for(i=0;i<60;i++){
      My_Put_Char(Lef[i]);//New_Lef[i]/10+40);
    }
    for(i=0;i<60;i++){
      My_Put_Char(Mid[i]);
    }
    for(i=0;i<60;i++){
      My_Put_Char(Rig[i]);//New_Rig[i]/10+40);
    }

    /*****************************************************/
    My_Put_Char(0xf2); //�����������ݶ�������

}

void Pic_send_new(void)
{
  int i;
  int j;
  cnt=0;
#ifdef ori_pic
  for(i=0;i<60;i++)
  {
    if(Pixle[i][0]==1)
    {
      Pic_new[cnt]=0;
      cnt++;
    }
    for(j=1;j<80;j++)
    {
      if(Pixle[i][j]!=Pixle[i][j-1])
      {
        Pic_new[cnt]=j;
        cnt++;
      }
    }
    Pic_new[cnt]=0xff;
    cnt++;
  }
#endif

#ifdef und_pic
    for(i=0;i<60;i++)
  {
    if((int)(New_Lef[i]+450)*0.089 <= 0)
    {
     Pic_new[cnt]=0;
     cnt++;
     Pic_new[cnt]=1;
     cnt++;
    }else{
    Pic_new[cnt]=(int)(New_Lef[i]+450)*0.089;
    cnt++;
    Pic_new[cnt]=(int)(New_Lef[i]+450)*0.089+1;
    cnt++;
    }

    Pic_new[cnt]=(int)(New_Rig[i]+450)*0.089;
    cnt++;
    Pic_new[cnt]=(int)(New_Rig[i]+450)*0.089+1;
    cnt++;
    Pic_new[cnt]=0xff;
    cnt++;
  }
#endif

}
/*************************************************************************
*  �������ƣ�void My_Put_Char(void)
*  ����˵����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2019.3.6
*  ��    ע��

*************************************************************************/

void My_Put_Char(char tmp)
{
    uart_putchar(USART_1,tmp);//����ʵ�ʵĴ��ں����޸�
}


void Send_Variable(void){

  My_Put_Char(0x55);
  My_Put_Char(0xaa);
  My_Put_Char(0xff);
  My_Put_Char(0x01);
  My_Put_Char(11);
  for(int i=0;i<11;i++){
            temp1=Variable[i];
            ch=BYTE0(temp1);
            My_Put_Char(ch);
            ch=BYTE1(temp1);
            My_Put_Char(ch);
            ch=BYTE2(temp1);
            My_Put_Char(ch);
            ch=BYTE3(temp1);
            My_Put_Char(ch);
  }
  My_Put_Char(0x01);

}



/*************************************************************************
*  �������ƣ�void Variable_update(void)
*  ����˵����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2019.3.10
*  ��    ע��

*************************************************************************/

void Variable_update(void)
{
  Variable[0]= Cam_offset;//cntt;//ToF_distance;
  Variable[1]= Turn_Cam_Out;//cnt;//Turn_Cam_Out;//;//stat_slope;//gyroy_1;//Turn_Cam_Out;//Car_W;
  Variable[2]= CarSpeed1;//speedTarget1; //Allwhiteend;
  Variable[3]= CarSpeed2;//whitecnt;
  Variable[4]= Road;//speedTarget2;//Turn_Cam_Out;
  Variable[5]= Lef_circle;//Cam_offset;
  Variable[6]= Rig_circle;
  Variable[7]= Road1_flag;
  Variable[8]= Road2_flag;
  Variable[9]= Lef_slope;
  Variable[10]=Rig_slope;
  //Variable[11]=Road;/*Rig_circle;*///Road6_flag ;//CarSpeed2;
/*
  Variable[12]=whitecnt;//Road1_turnout;//limit_pos(EM_Value_1/1.5-EM_Value_2/3.5);//speedTarget1;//map_line[MIN(50,AllWhileStartLine)];
  Variable[13]=EM_Value_2;//speedTarget2;//lib_active_diff_get();//map_line[MIN(50,AllWhileEndLine)];_
  Variable[14]=EM_Value_3;//Cam_Block_State;//MotorOut1;//CarSpeed1;//(MotorOut1+MotorOut2)/2;
  Variable[15]=ToF_distance;//EM_Value_4;//ToF_distance;//MotorOut2;//CarSpeed2;
  */

}
