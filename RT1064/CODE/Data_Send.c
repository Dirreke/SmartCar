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
*  函数名称：void Send_Begin(void)
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2019.3.6
*  备    注：

*************************************************************************/
void Send_Begin(void)
{
  My_Put_Char(0x55);
  My_Put_Char(0xaa);
  My_Put_Char(0x11);
}

/*************************************************************************
*  函数名称：void Send_Img(void)
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2019.3.6
*  备    注：

*************************************************************************/


void Send_Img(void)
{

    uint16 i=0,num;
    My_Put_Char(0x55);
    My_Put_Char(0xaa);
    My_Put_Char(0xff);
    My_Put_Char(0xa2);
    My_Put_Char(0x01); //小锟斤拷状态

    num = cnt + 2 +180;//4+4*2
    //num = cnt+2+180;
    //num=cont+2+180+36;
    //统计将要传输的数据量 2是因为要传输关键字即0xf0和0xf2   (0xf2 所有数据结束，0xf0图像数据结束，180=60*3)
    //180是边线的位 36是变量的位 如果不传输就不要加上！

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
    My_Put_Char(0xf0);  //代表图像数据发完了
    /******************星号围起来的可以不传输*******************/

    ////////////////////////传输左右边线和计算得到的中线///////////
    for(i=0;i<60;i++){
      My_Put_Char(Lef[i]);//New_Lef[i]/10+40);
    }
    for(i=0;i<60;i++){
      My_Put_Char(Mid[i]);
    }
    for(i=0;i<60;i++){
      My_Put_Char(Rig[i]+1);//New_Rig[i]/10+40);
    }

    /*****************************************************/
    My_Put_Char(0xf2); //代表整个数据都发完了


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
*  函数名称：void My_Put_Char(void)
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2019.3.6
*  备    注：

*************************************************************************/

void My_Put_Char(char tmp)
{
    uart_putchar(USART_1,tmp);//根据实际的串口号来修改
}

void Send_Variable(void){

  My_Put_Char(0x55);
  My_Put_Char(0xaa);
  My_Put_Char(0xff);
  My_Put_Char(0x01);
  My_Put_Char(16);
  for(int i=0;i<16;i++)
  {
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
*  函数名称：void Variable_update(void)
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2019.3.10
*  备    注：

*************************************************************************/

void Variable_update(void)
{
  Variable[0]= Cam_offset;//cntt;//ToF_distance;
  Variable[1]= Turn_Cam_Out;//cnt;//Turn_Cam_Out;//;//stat_slope;//gyroy_1;//Turn_Cam_Out;//Car_W;
  Variable[2]= Lef_break_point;//Allwhiteend;//CarSpeed1;//speedTarget1; //Allwhiteend;
  Variable[3]= Rig_break_point;//Allwhitestart;//CarSpeed2;//whitecnt;
  //  EM_Value_1=(float)(EM_Value_1*3.3/4096);
  // EM_Value_2=(float)(EM_Value_2*3.3/4096);
  // EM_Value_3=(float)(EM_Value_3*3.3/4096);
  // EM_Value_4=(float)(EM_Value_4*3.3/4096);
  Variable[4]= Road2_flag;//EM_Value_1;//Cam_offset;
  Variable[5]= Allwhitestart;
  Variable[6]= EM_Value_2;
  Variable[7]= Rig_slope;//EM_Value_4;
  Variable[8]= Lef_slope;//EM_offset;
  Variable[9]= Road;//speedTarget2;//Turn_Cam_Out;
  Variable[10]=Road0_flag+Road1_flag*10+Road7_flag*100;
  Variable[11]=CarSpeed1;
  Variable[12]=CarSpeed2;
  Variable[13]=Lef_circle;/*Rig_circle;*///Road6_flag ;//CarSpeed2;
  Variable[14]=Rig_circle;
  Variable[15]=whitecnt;//Road1_turnout;//limit_pos(EM_Value_1/1.5-EM_Value_2/3.5);//speedTarget1;//map_line[MIN(50,AllWhileStartLine)];
  // Variable[13]=EM_Value_2;//speedTarget2;//lib_active_diff_get();//map_line[MIN(50,AllWhileEndLine)];_
  // Variable[14]=EM_Value_3;//Cam_Block_State;//MotorOut1;//CarSpeed1;//(MotorOut1+MotorOut2)/2;
  // Variable[15]=ToF_distance;//EM_Value_4;//ToF_distance;//MotorOut2;//CarSpeed2;


}
