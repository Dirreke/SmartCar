#include "headfile.h"    //20��=90cm


#define max_block(a, b) ((a) > (b) ? (a) : (b))

int Lef[LCDH]; //��·�������������
int Rig[LCDH]; //��·�ҷ�����������
int Lef_err[LCDH];//�����жϷ�����Ƿ�ɹ�����δ�ҵ�������1
int Rig_err[LCDH];
int Mid[LCDH]; //��·���ĵ��������
int Middle = 41;

float LR_slope;
float Cam_offset_filter[4] ={0,0,0};//offset�˲�����
float Cam_offset2;

int area[5]={Last_row-8,48,34,Last_row-33,Last_row};//���������ƫ��ı߽绮��

int Allwhiterow[LCDH];//ȫ���У�1��ʾȫ�ף�����Ϊ0
int Allwhitestart;//ȫ���п�ʼ��
int Allwhiteend;//ȫ���н�����
int Allwhitestart2;
int Allwhiteend2;
int Fix1x,Fix2x,Fix1y,Fix2y;
int Fix_line;//���߱�־λ
int Fix[LCDH];//���ڲ���


int Lef_circle;
int Rig_circle;//���ұ���Ѱ�һ�״���߱�־λ
int Lef_break_point;
int Rig_break_point;//���һ�״���߹����
int Lef_leap[4];
int Rig_leap[4];//���ұ������������

uint16 Lef_road=0;//���Ҳ�·��־λ������⵽�в�·������1
uint16 Rig_road=0;

int Road11_count=0,Road12_count=0,Road13_count=0,Road14_count=0,Road15_count=0;
int Road21_count=0,Road22_count=0,Road23_count=0,Road24_count=0,Road25_count=0;
int Road0_flag=0;
int Road1_flag=0;
int Road2_flag=0;

int Lef_start=Fir_row,Rig_start=Fir_row,Rig_sep1=Fir_row,Rig_sep2=Fir_row,Lef_sep1=Fir_row,Lef_sep2=Fir_row,Lef_lose,Rig_lose;//����������ʼ�㡢�Լ�������·��

float Lef_slope=0;
float Rig_slope=0;




int Road=0;//��·���ͱ�־λ
int Road3_state;
int cross_flag=0;
int unclear=0;
int New_Lef[60];
int New_Rig[60];//���ڴ洢��͸�ӱ任��ĺ�����
int Ca_offset=0;
int Cam_Break_Flag = 0;

extern float Cam_Turn_Control;
extern float EM_Turn_Control;
extern float Turn_P_EM; 
extern float Turn_I_EM;
extern float Turn_D_EM;
extern int  EM_Road;
extern int EM_Ring_State;
extern float EM_Value_23_Sum;
extern float EM_Road1_Cnt;
extern float EM_Road2_Cnt;
extern int16 qd1_result;

int Lef_End = 0;
int Rig_End = 0;
int Cam_End = 0;
int block_flag = 1;

/*���Բ���*/
float offset_map[60]={(float)31/77,(float)31/77,(float)31/9	,(float)31/9	 ,(float)31/10	,(float)31/11	,(float)31/12	 ,(float)31/13	,(float)31/14,(float)31/15,(float)31/16		 	
,(float)31/17,(float)31/17,(float)31/19,(float)31/19,(float)31/21	,(float)31/21,(float)31/23,(float)31/24	,(float)31/25	 ,(float)31/25	,(float)31/27,(float)31/28,(float)31/29	 	 	
,(float)31/30,(float)31/31,(float)31/31	,(float)31/33,(float)31/34,(float)31/34,(float)31/36,(float)31/36	,(float)31/36	,(float)31/38	,(float)31/40,(float)31/40,(float)31/42,(float)31/42	  	
,(float)31/42	,(float)31/44	 ,(float)31/46	 ,(float)31/46	 ,(float)31/48	  ,(float)31/48	,(float)31/48	,(float)31/50	,(float)31/51,(float)31/52,(float)31/53,(float)31/54,(float)31/54,(float)31/56,(float)31/56,(float)31/58,(float)31/58,(float)31/58,(float)31/60,(float)31/60,(float)31/61,(float)31/77};
int threshold_offset = 5;
float zhidaosudu = 2.5;//ֱ���ٶ�
float xiaowandaosudu = 2.3;//С����ٶ�
float dawandaosudu = 2.3;//������ٶ�
float duanlusudu =1.8;//��·�ٶ�
int camera_offset = 0;//����ͷ��ֵ����ֵ
int Tof_thres=150;//�ϰ�������ֵ
float luzhangsudu = 2.3;//·���ٶ�

/*************************************************************************
*  �������ƣ�my_key_debug()
*  ����˵�����������Ժ���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2019.3.6
*  ��    ע��
*************************************************************************/
/*
void my_key_debug()
{
  
  static int state = 0;
  static int32 ips_num = 0;

        
        state = SW_Read();
        switch(state)
        {
          
          case 0:
            ips_showint32(150,0,(int32)ips_num,2);
            if(ips_num>10)
            {
            ips_num = 0;
            }
            if(ips_num<0)
            {
            ips_num = 10;
            }
            ips_showfloat(0,0,(double)zhidaosudu,3,2);
            ips_showfloat(0,1,(double)xiaowandaosudu,3,2);
            ips_showfloat(0,2,(double)dawandaosudu,3,2);
            ips_showfloat(0,3,(double)duanlusudu,3,2);
            ips_showfloat(0,4,(double)luzhangsudu,3,2);
            ips_showfloat(0,5,(double)Block_Angle_Set,3,2);
            ips_showfloat(0,6,(double)Block_Angle_Set2,3,2);
            ips_showint32(0,7,(int32)camera_offset,3);
            ips_showint32(150,1,(int32)Tof_thres,3);
            ips_showint32(150,2,(int32)block_flag,3);
            ips_showint32(150,3,(int32)threshold_offset,2);
            
           
            
            if(!KEY_Read(KEY0))  
            {
              systick_delay_ms(50);
              
            if(!KEY_Read(KEY0))
             {
              ips_clear(IPS_WHITE);
              switch(ips_num)
            {       
              case 0:
                zhidaosudu+=0.1;
                break;
              case 1:
                xiaowandaosudu+=0.1;
                break;
              case 2:
                dawandaosudu+=0.1;
                break;
              case 3:
                duanlusudu+=0.1;
                break;
              case 4:
                luzhangsudu+=0.1;
                break;
              case 5:
                Block_Angle_Set+=1;
                break;
              case 6:
                Block_Angle_Set2+=1;
                break;
              case 7:
                camera_offset+=1;
                break;
              case 8:
                Tof_thres+=5;
                break;
              case 9:
                block_flag = 1;
                break;
              case 10:
                threshold_offset += 1;
                break;
              default:
                break;
              
            }
           
             }
             }
            if(!KEY_Read(KEY1))  
            {
            systick_delay_ms(50);
            if(!KEY_Read(KEY1))
            {
              ips_clear(IPS_WHITE);
              switch(ips_num)
            { 
              
              case 0:
                zhidaosudu-=0.1;
                break;
              case 1:
                xiaowandaosudu-=0.1;
                break;
              case 2:
                dawandaosudu-=0.1;
                break;
              case 3:
                duanlusudu-=0.1;
                break;
              case 4:
                luzhangsudu-=0.1;
                break;
              case 5:
                Block_Angle_Set-=1;
                break;
              case 6:
                Block_Angle_Set2-=1;
                break;
              case 7:
                camera_offset-=1;
                break;
              case 8:
                Tof_thres-=5;
                break;
              case 9:
                block_flag = -1;
                break;
              case 10:
                threshold_offset -= 1;
              default:
                break;
              
            }
            
            }
            }
            
            if(!KEY_Read(KEY2))  
            {
            systick_delay_ms(50);
            if(!KEY_Read(KEY2))
            {
              ips_clear(IPS_WHITE);
              ips_num++;
            }
            }
            
            if(!KEY_Read(KEY3))  
            {
            systick_delay_ms(50);
            if(!KEY_Read(KEY3))
            {
              ips_clear(IPS_WHITE);
              ips_num--;
            }
            }
            break;
            
          case 1:
            if(!KEY_Read(KEY0))  
            {
            systick_delay_ms(50);
            if(!KEY_Read(KEY0))
            {
            ips_clear(IPS_WHITE);
            }
            }
            ips_displayimage032_01Value(Pixle[0], CAMERA_W, CAMERA_H);
            break;
          case 2:
            return;
            break;
          case 3:
            return;
            break;
          default:
            break;
          
        
          
        }
        
}
*/
/*************************************************************************
*  �������ƣ�void Pic_seedfill(void)
*  ����˵����DFS������������㷨
*  ����˵������           
*  �������أ���Sobel��Ե��ȡ���ͼ���������Լ����ŵ�ȥ��
*  �޸�ʱ�䣺2019.5.27
*  ��    ע���Ӻ���grow()Ϊ�ݹ麯����
*************************************************************************/

void Pic_seedfill(void){
	uint8 flag[CAMERA_H][CAMERA_W]={0};
	int i,j;
	int seed_flag=0,seed_i=-1,seed_j=-1;
	
	//�趨��׼��(��)
	for(i=seed_down;i>=seed_up;i=i-3){
		for(j=seed_left;j<=seed_right;j=j+3){
			if(Pixle[i][j]==1){
				seed_flag=1; 
				seed_i=i;
				seed_j=j;
				break;
			}
		}
		if(seed_flag==1)
			break;
	}
	if(seed_flag==0)	//û�ҵ���׼��,����-1
		return;
        if(Lef_slope==999 && Rig_slope<900 && Road!=2)
        {
          seed_i=55;
          seed_j=10;
        }
        if(Rig_slope==999 && Lef_slope<900 && Road ==1)
        {
          seed_i=55;
          seed_j=10;
        }
	//�������
	Pic_seedfill_grow(flag,seed_i,seed_j);

	for(i=CAMERA_H-1;i>=0;i--)
		for(j=0;j<=CAMERA_W-1;j++){
			Pixle[i][j]=flag[i][j];
			if(flag[i][j]==2)
			 	Pixle[i][j]=0;
			// 	Bondary[i][j]=1;
		}			
			
	return;
}

void Pic_seedfill_grow(uint8 flag[CAMERA_H][CAMERA_W],int i,int j){
	//�ж� ����������� ������������ ���߽�����
	int grow_i,grow_j;
	
	for(grow_i=i-1;grow_i<=i+1;grow_i++)
		for(grow_j=j-1;grow_j<=j+1;grow_j++)
			if(Pixle[grow_i][grow_j]==1)
				flag[grow_i][grow_j]=1;
			else
				flag[grow_i][grow_j]=2;			//2����ԭͼ��Ϊ��ɫ0�������ѹ�

	// if(Pixle[i][j]==1)
	// 	flag[i][j]=1;

	//down
	if(Pixle[i+1][j]==1 && Pixle[i+2][j]==1 && flag[i+3][j]==0 && i<BOTTOM-3)	//������1���Ź�����������һ�¶���Ϊ�� 2������û�ѹ� 3������С�ڱ߽�  //����Ե����һ�л�����
		Pic_seedfill_grow(flag,i+3,j);
	
	//left
	if(Pixle[i][j-1]==1 && Pixle[i][j-2]==1 && flag[i][j-3]==0 && j>LEFT+3)
		Pic_seedfill_grow(flag,i,j-3);
	//right
	if(Pixle[i][j+1]==1 && Pixle[i][j+2]==1 && flag[i][j+3]==0 && j<RIGHT-3)
		Pic_seedfill_grow(flag,i,j+3);
	//up
	if(Pixle[i-1][j]==1 && Pixle[i-2][j]==1 && flag[i-3][j]==0 && i>TOP+3)
		Pic_seedfill_grow(flag,i-3,j);
	

	
	return;
	
}


/*************************************************************************
*  �������ƣ�void Pic_noi_elim()
*  ����˵������Ƭ�������
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2019.2.20
*  ��    ע��ĳ�������෴��Ϊ���
*************************************************************************/
int whitecnt=0;
void Pic_noi_elim(void)
{  
  int nr; //��
  int nc; //��
  whitecnt=0;
  for(nr=1; nr<LCDH-1; nr++)
  {  	    
    for(nc=1; nc<LCDW-1; nc=nc+1)
    {
      if((Pixle[nr][nc]==0)&&(Pixle[nr-1][nc]+Pixle[nr+1][nc]+Pixle[nr][nc+1]+Pixle[nr][nc-1]>2))         
      {
        Pixle[nr][nc]=1;
      }	
      else if((Pixle[nr][nc]==1)&&(Pixle[nr-1][nc]+Pixle[nr+1][nc]+Pixle[nr][nc+1]+Pixle[nr][nc-1]<2))         
      {
        Pixle[nr][nc]=0;
      }
      if(Pixle[nr][nc]==1)
        whitecnt+=1;
    }	  
  }  
}

/*************************************************************************
*  �������ƣ�void Pic_thresh_get()
*  ����˵������ֵ����ֵԤ��ȡ
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2019.3.15
*  ��    ע��
*************************************************************************/

void Pic_thresh_get(void)
{
  int i=0;
  int thresh_sum=0;
  for(i=area[2];i<area[1];i++)
  {
    thresh_sum=thresh_sum+Image_Use[i][Mid[i]];
  }
  Threshold=(int)(thresh_sum*1.0/(area[1]-area[2]))-20;
}

char Road1_turnin(float a,float b,float c)
{
  //static float c=3.8;
  if(a*b> c)
    return 1;
  else if((a-3 < 0.1 || 3-a < 0.1) && (b < c/3 && b>0))
    return 1;
  else if((b-3 < 0.1 || 3-b < 0.1) && (a < c/3 && a>0))
     return 1;
  return 0;
}
/*************************************************************************
*  �������ƣ�void Road_rec()
*  ����˵��������ʶ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2019.3.23
*  ��    ע��Road=0��ʾ����·������û�н���Բ�����£�
             3.23:Road=1��ʾ��Բ����Road=2��ʾ��Բ��

*************************************************************************/
/*
char Road1_turnout = 1;
void Road_rec(void)
{
  
  static int oldwhite=5000;
  static uint8 Road1_cnt1=0;
  static char Road1_flag1=0;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////��ͨ������Բ��  
  if((Lef_break_point<20 && Road==0&& Rig_circle==0 && Lef_circle==1 && Rig_slope==998 &&Lef_slope!=998 && Rig[13]-Rig[11]<5 && Rig[11]-Rig[9]<5 && Rig[9]-Rig[7]<5 && Rig[7]-Rig[5]<5&& Rig[5]-Rig[3]<5 && Rig[11]!=78 &&New_Lef[50]==-MIDMAP&&Rig_edge<10))//��Բ���������,�ұ��ߣ�ֱͨ����
  {
    Road0_flag = 0;
    Road11_count++;
    if(Road11_count==2 && Road1_flag==0)
    {
      Road=1;
      Road11_count=0;
      return;
    }
    return;
  }
  else if(Rig_break_point<20 && Road==0 && Lef_circle==0 && Rig_circle==1 && Lef_slope==998 && Rig_slope!=998 && Lef[3]-Lef[5]<5 && Lef[5]-Lef[7]<5 && Lef[7]-Lef[9]<5 && Lef[9]-Lef[11]<5 && Lef[11]-Lef[13]<5&&Lef[11]!=2 &&New_Rig[50]==MIDMAP&&Lef_edge<10)//��Բ�����ұ��ߣ�ͻ�����յ��ͻ���
  {
    Road0_flag = 0;
    Road21_count++;
    if(Road21_count==2 && Road2_flag==0)
    {
      Road=2;
      Road21_count=0;
      return;
    } 
    return;
  }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////��Բ������ͨ����
  else if(Road==1 && Road1_flag==0)
  {
    Road0_flag = 0;
    if(EM_Value_2+EM_Value_3>3.8)//����ʶ���������߽���һ�߷�������
    {
      Road12_count++;
      if(Road12_count==2)
      {
        Road12_count=0;
        Road1_flag=1;//��ʾ�Ѿ�������Բ��
      }
      return;
    }
    return;
  }
  else if(Road==1 && (Road1_flag==1))
  {
    Road0_flag = 0;
    if(((Lef_circle==0||( Lef_circle ==1 && Lef_break_point>30)))&& Road1_turnin(EM_Value_2,EM_Value_3,3.8))//if(((Lef_circle==0||( Lef_circle ==1 && Lef_break_point>30))) && Rig_slope>=10)/ && Road1_turnin(EM_Value_2,EM_Value_3,3.8))    //Rig_slope<1 && (Lef_leap[0]==0||Lef_slope==999)&& Rig_leap[0]==0)
    {
      Road13_count++;
      if(Road13_count==2)
      {
        Road1_flag = 2;
//        Road1_flag1 = 1;
        Road13_count=0;
        return;
      }
      return;
    }   
  }
  else if(Road==1 && Road1_flag==2)
  {
    Road0_flag = 0;
    if(1)//
    {
      Road14_count++;
      if(Road14_count==(int)(115/(get_speed()+1)))
      {
        Road1_flag = 4;
        Road14_count=0;
        return;
      }
    }
    return;
  }
    else if(Road==1 && Road1_flag==4)
  {
    Road0_flag = 0;
    if(Rig_circle&&/whitecnt>2700)//
    {
      Road14_count++;
      if(Road14_count==3)
      {
        Road1_flag = 3;
        Road14_count=0;
        return;
      }
    }
    return;
  }
  else if(Road==1 && Road1_flag==3)
  {
    Road0_flag = 0;
    if((Rig_slope>-0.02 && Rig_slope<0)||(Pixle[58][74] == 1 && Pixle[57][74] == 1 && Pixle[56][74] == 1 && Pixle[55][74] == 1&& Pixle[54][74]==1&&Pixle[53][74]==1 ))//|| Lef_edge < 20))
    {
      Road15_count++;
      if(Road15_count==4)
      {
        Road15_count=0;
        Road=0;
        //Road1_turnout=1;
        Road1_flag=0;
        return;
      }
    }
    return;
  }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////��Բ������ͨ����
  else if(Road==2 && Road2_flag==0)
  {
    Road0_flag = 0;
    if( EM_Value_2 +EM_Value_3 >3.8)//����ʶ���������߽���һ�߷�������
    {
      Road22_count++;
      if(Road22_count==2)
      {
        Road22_count=0;
        Road2_flag=1;//��ʾ�Ѿ�������Բ��
      }
      return;
    }
    return;
  }
  else if(Road==2 && Road2_flag==1)
  {
    Road0_flag = 0;
    if(((Rig_circle==0||( Rig_circle ==1 && Rig_break_point>30))) && Rig_slope>=10)/ && Road1_turnin(EM_Value_2,EM_Value_3,3.4))
    {
      Road23_count++;
      if(Road23_count==2)
      {
        
        Road2_flag=2;
        Road23_count=0;
        return;
      }
    }
    return;
  }
  else if(Road==2 && Road2_flag==2)
  {
    Road0_flag = 0;
    if(1)
    {
      Road24_count++;
      if(Road24_count==(int)(115/(get_speed()+1)))
      {
        Road24_count=0;
        Road2_flag=4;
        return;
      }
    }
    return;
  }
  else if(Road==2 && Road2_flag==4)
  {
    Road0_flag = 0;
    if(whitecnt>2700)
    {
      Road25_count++;
      if(Road25_count==3)
      {
        Road25_count=0;
        Road2_flag=3;
        return;
      }
    }
    return;
  }
  else if(Road==2 && Road2_flag==3)
  {
      Road0_flag = 0;
    if((Lef_slope>0 && Lef_slope<0.02)||(Pixle[58][5] == 1 && Pixle[57][5] == 1 && Pixle[56][5] == 1 && Pixle[55][5] == 1&& Pixle[54][5]==1&&Pixle[53][5]==1 ))//|| Lef_edge < 20))
    {
      Road25_count++;
      if(Road25_count==4)
      {
        Road25_count=0;
        Road=0;
        //Road2_turnout=1;
        Road2_flag=0;
        return;
      }
    }
    return;
  }

  if(Road==0 && whitecnt>2700 && ((Lef_edge>10 && Rig_edge>10)||Lef_edge>30||Rig_edge>30)&&Allwhitestart<40)
  {
      Road0_flag = 1;
  }
  else if((Allwhitestart>50 && Allwhiteend <40)||whitecnt>3000)
  {
      Road0_flag = 2;
  }
  else
  {
      Road0_flag = 0;
  }
}  
*/
/*************************************************************************
*  �������ƣ�void Slope_fig()
*  ����˵������������б�ʼ���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2019.3.21
*  ��    ע����С���˷����FIG_AREA�����ڲ�Ϊ0��б��

*************************************************************************/

void LR_Slope_fig()
{
  int i;
  float xsum=0,ysum=0,xysum=0,x2sum=0,count=0;
  Lef_slope=Rig_slope=0;
  int max=-800,min=0;
  for(i=0;i<60;i++)
  {
    if (i<=FIG_AREA_NEAR && i>=FIG_AREA_FAR3 && New_Lef[i]!=-MIDMAP)
    {
      if(New_Lef[i]>max)
      {
        max=New_Lef[i];
      }
      if(New_Lef[i]<min)
      {
        min=New_Lef[i];
      }
      xsum+=New_Lef[i];
      ysum+=i;
      xysum+=New_Lef[i]*i;
      x2sum+=New_Lef[i]*New_Lef[i];
      count++;
    }
  }
  if(abs(max-min)>50)
  {
      if(count*x2sum-xsum*xsum)
      {
        Lef_slope=-(count*xysum-xsum*ysum)/(count*x2sum-xsum*xsum);
      }
      else
      {
       Lef_slope=999;
      }
  }
  else
  {
    Lef_slope=998;
  }
  max=0;
  min=800;
  xsum=ysum=xysum=x2sum=count=0;
  for(i=0;i<60;i++)
  {
    if (i<=FIG_AREA_NEAR && i>=FIG_AREA_FAR3 && New_Rig[i]!=MIDMAP)
    {
      if(New_Rig[i]>max)
      {
        max=New_Rig[i];
      }
      if(New_Rig[i]<min)
      {
        min=New_Rig[i];
      }      
      xsum+=New_Rig[i];
      ysum+=i;
      xysum+=New_Rig[i]*i;
      x2sum+=New_Rig[i]*New_Rig[i];
      count++;
    }
  }
  if(abs(max-min)>50)
  {
    if(count*x2sum-xsum*xsum)
    {
      Rig_slope= - (count*xysum-xsum*ysum)/(count*x2sum-xsum*xsum);
    }
    else
    {
      Rig_slope=999;
    }
  }
  else
  {
    Rig_slope=998;
  }
    LR_slope=L_R_Slope(1)+L_R_Slope(2);
}


/*************************************************************************
*  �������ƣ�float Slope(int F1x,int F1y,int F2x,int F2y)
*  ����˵����б�ʺ���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2019.3.21
*  ��    ע����y/��x

*************************************************************************/

float Slope(int F1x,int F1y,int F2x,int F2y)
{
  float slope=0;
  if(F2y==F1y)
  {
    return 999;
  }
  slope=(F2y-F1y)*1.0/(F2x-F1x);
  return slope;
}

/*************************************************************************
*  �������ƣ�float L_R_Slope(char LR)
*  ����˵����б�ʺ���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2019.7.24
*  ��    ע��LR=1 �����  LR=2�ұ���

*************************************************************************/
float L_R_Slope(char LR)
{
  int8 i,j;
  float slope[15]={0};
  float sum=0;
  int8 row=0;
  if(LR == 1)
  {
    for(i=5;i<20;i++)
    {
       if(Lef[i]!=Fir_row)
       {
         slope[row++]=Slope(Lef[i],i,Lef[i+10],i+10);         
       }
    }
    for(j=row-1;j>=0;j--)
    {
      if(slope[j]!=999)
      {
        sum+=slope[j];
        
      }
      sum/=(row-1);
     // return sum;
    }
    
  }
  if(LR == 2)
  {
    for(i=5;i<20;i++)
    {
       if(Rig[i]!=Fir_row)
       {
         slope[row++]=Slope(Rig[i],i,Rig[i+10],i+10);         
       }
    }
    for(j=row-1;j>=0;j--)
    {
      if(slope[j]!=999)
      {
        sum+=slope[j];
        
      }
      sum/=(row-1);
     // return sum;
    }
      
  }
  return sum;
}
/*************************************************************************
*  �������ƣ�void Allwhite_find()
*  ����˵����ȫ���в���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2019.3.23
*  ��    ע��Ѱ��ȫ���еĿ�ʼ�кͽ�����

*************************************************************************/

void Allwhite_find(void)
{  
  int i;
  Allwhitestart=Allwhiteend=Allwhitestart2=Allwhiteend2=0;
  for(i=Fir_row;i<Last_row+1;i++)
  {
    Allwhiterow[i]=0;
    if(Road0_flag==1&&i<30)
    {
      if(abs(Lef[i]-8)<5 && abs(72-Rig[i])<5&&Pixle[i][40]==1&&Pixle[i][20]==1&&Pixle[i][60]==1)
      {
       Allwhiterow[i]=1;
      }
    }
    else if(abs(Lef[i]-Fir_col)<5 && abs(Last_col-Rig[i])<5&&Pixle[i][40]==1&&Pixle[i][20]==1&&Pixle[i][60]==1)
    {
       Allwhiterow[i]=1;
    }
  }
  Allwhitestart=Last_row;
  i=Last_row;
  while(Allwhiterow[i]!=1 && i>Fir_row)
  {
    i=i-1;
  }
  Allwhitestart=i;
  Allwhiteend=Fir_row;
  i=Allwhitestart;
  while(Allwhiterow[i]==1 && i>Fir_row)
  {
    i=i-1;
  }
  Allwhiteend=i;
  for(i=Allwhiteend-1;i>Fir_row;i--)
  {
    if(Allwhiterow[i]==1)
    {
      Allwhitestart2=i;
      break;
    }
  }
  if(Allwhitestart2!=0)
  {
    for(i=Allwhitestart2;i>Fir_row;i--)
    {
      if(Allwhiterow[i]!=1)
      {
        Allwhiteend2=i;
        break;
      }
    }
  }

    
}


/*************************************************************************
*  �������ƣ�void Fix_line()
*  ����˵������·���ߴ���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2019.3.6
*  ��    ע��3.6�գ���Ҫ����������ʮ��·��ƫ�����ĵ�·ʱ����������·���ߣ�
             ����ֱͨ������������в��ߣ�ͨ��ƽ�������ʮ�����ֿ���

*************************************************************************/

void Pic_Fix_Line(void)
{
  float slope;
  float slope2;
  int i;
  int j;
  int k;
  static float stat_slope;
  static float stat_slope2;
  //static char road1_flag3=1;
  static char road1_flag1=1;
  static char road2_flag1=1;
  if(Road0_flag==1&&Road==0)
  {
    for(i=55;i>15;i--)
    {
        if(abs(Lef[i]-Fir_col)<5)
          continue;
        slope=Slope(Lef[i],i,Lef[i-5],i-5);//Slope(int F1x,int F1y,int F2x,int F2y)
        if(slope!=999)
        {
          for(j=i+1;j>5;j--)
          {
            Lef[j]=(int)(Lef[i]-(i-j)/slope);
            pixel_undistort(Lef[j],j,0);  //x:Lef[i],      y:i   LR:0:��  1���� 
          }
          break;
        }
    }
     for(i=55;i>15;i--)
    {
      if(abs(Rig[i]-Last_col)<5 )
        continue;
        slope=Slope(Rig[i],i,Rig[i-5],i-5);//Slope(int F1x,int F1y,int F2x,int F2y)
        if(slope!=999)
        {
          for(j=i+1;j>5;j--)
          {
            Rig[j]=(int)(Rig[i]-(i-j)/slope);
            pixel_undistort(Rig[j],j,1);  //x:Lef[i],      y:i   LR:0:��  1���� 
          }
          break;
        }
      
    }
    return;
  }
  else if(Road0_flag==2&&Road==0)
  {
    for(i=15;i<Allwhiteend;i++)
    {
      if(abs(Lef[i]-Fir_col)<15)
         continue;
      slope=Slope(Lef[i],i,10,54);//Slope(int F1x,int F1y,int F2x,int F2y)
        if(slope!=999)
        {
          for(j=i+1;j<55;j++)
          {
            Lef[j]=(int)(Lef[i]-(i-j)/slope);
            pixel_undistort(Lef[j],j,0);  //x:Lef[i],      y:i   LR:0:��  1���� 
          }
          break;
        }
    }
     for(i=15;i<Allwhiteend;i++)
    {
      if(abs(Rig[i]-Last_col)<15)
        continue;
        slope=Slope(Rig[i],i,70,54);//Slope(int F1x,int F1y,int F2x,int F2y)
        if(slope!=999)
        {
          for(j=i+1;j<55;j++)
          {
            Rig[j]=(int)(Rig[i]-(i-j)/slope);
            pixel_undistort(Rig[j],j,1);  //x:Lef[i],      y:i   LR:0:��  1���� 
          }
          break;
        }
      
    }
    return;
   }
  
  
 //��Բ�����ߴ��� 
  if(Road==1 && Road1_flag==2)
  {
    for(i=0;i<30;i++)
    {
      if(road1_flag1)
      {     
      if(abs(Lef[i]-Fir_col)<20)
        continue;
        stat_slope=Slope(Lef[i],i,63,55);
        road1_flag1=0;
      }
      
      if(stat_slope!=999)
      {
        for(k=3;k<55;k++)
        {
          Rig[k]=(int)(60-(57-k)/stat_slope);
          pixel_undistort(Rig[k],k,1);
        }
        break;
      }
      
    }
    return;
  }
 else if(Road==1 && Road1_flag==4)
  {
    road1_flag1=1;
    return;
  }
  else if(Road==1 && Road1_flag==3)
  {
      for(j=55;j>Fir_row;j--)
    {

       if(abs(Rig[j]-Last_col)<2)
        continue;    
        slope=Slope(Rig[j-1],j-1,Rig[j-6],j-6);

        if(stat_slope!=999)
        {
          for(k=j+1;k>5;k--)
          {
            Rig[k]=(int)(64-(57-k)/slope);
            pixel_undistort(Rig[k],k,1);
          }
          break;
        }
     }
      return;
  }
  
 if(Road==2 && Road2_flag==2)
  {
    for(i=0;i<30;i++)
    {
      if(road2_flag1)
      {     
      if(abs(Rig[i]-Last_col)<20)
        continue;
        stat_slope2=Slope(Rig[i],i,15,55);
        road2_flag1=0;
      }
      
      if(stat_slope2!=999)
      {
        for(k=3;k<55;k++)
        {
          Lef[k]=(int)(15-(57-k)/stat_slope2);
          pixel_undistort(Lef[k],k,0);
        }
        break;
      }
      
    }
    return;
  }
  else if(Road==2 && Road2_flag==4)
  {
    road2_flag1=1;
    return;
  
  }
  else if(Road==2 && Road2_flag==3)
  {
      for(j=55;j>Fir_row;j--)
    {

       if(abs(Lef[j]-Fir_col)<2)
        continue;    
        slope2=Slope(Lef[j-1],j-1,Lef[j-6],j-6);

        if(slope2!=999)
        {
          for(k=j+1;k>5;k--)
          {
            Lef[k]=(int)(15-(57-k)/slope2);
            pixel_undistort(Lef[k],k,0);
          }
          break;
        }
     }
      return;
  
  }
  
}


/*************************************************************************
*  �������ƣ�void Pic_DrawMid()
*  ����˵��������������
*  ����˵�������ط�����������ʾӦ��ת������ʾӦ��ת
*  �������أ�������ǰ�������ڵľ�ֵ��Ԥ��ֵ��ƫ��
*  �޸�ʱ�䣺2019.2.27
*  ��    ע��Ѱ�ҵײ�ڰ�����㣬�����������ÿ�е�����㡣(�������ѵ㡢��Χ�ѵ����ַ�����
             ������ͼ������5���ú�
             ÿһ��ֻ�����������㡣
             Ȼ��������ƽ��ֵ����������
*************************************************************************/

void Pic_DrawMid(void)
{
  int i=0,j=0,k=0;
  for(i=0;i<LCDH;i++)//�������úڣ���ֹȫ�����޷���Ѱ����㣩
  {
    Rig[i]=78;
    Lef[i]=1;
    Mid[i]=39;
    for(j=0;j<Fir_col;j++)
    {
      Pixle[i][j]=0;
    }
    for(j=Last_col+1;j<LCDW;j++)
    {
      Pixle[i][j]=0;
    }
  }
//  for(i=0;i<Fir_row;i++)//���Ϸ��ú�
//  {
//    for(j=0;j<LCDW;j++)
//    {
//      Pixle[0][j]=0;
//    }
  
  for(j=Middle;j<Last_col+1;j++)//ĩ�д���
  {
    if(Pixle[Last_row][j]==1&&Pixle[Last_row][j-1]==1&&Pixle[Last_row][j-2]==1&&Pixle[Last_row][j-3]==1&&Pixle[Last_row][j-4]==1&&Pixle[Last_row][j-5]==1&&Pixle[Last_row][j-6]==1&&Pixle[Last_row][j+1]==0&&Pixle[Last_row][j+2]==0)
    {
      Rig[Last_row]=j+2;
      break;
    }
  }
  for(j=Middle;j>Fir_col-1;j--)
  {
    if(Pixle[Last_row][j]==1&&Pixle[Last_row][j+1]==1&&Pixle[Last_row][j+2]==1&&Pixle[Last_row][j+3]==1&&Pixle[Last_row][j+4]==1&&Pixle[Last_row][j+5]==1&&Pixle[Last_row][j+6]==1&&Pixle[Last_row][j-1]==0&&Pixle[Last_row][j-2]==0)
    {
      Lef[Last_row]=j;
      break;
    }
  }
  for(i=Last_row-1;i>Fir_row-1;i--)//�ӵײ����ϻ���
  {
    Lef_err[i]=Rig_err[i]=0;
    for(j=Rig[i+1]-10;j<Last_col+1;j++)//������������һ�����
    {
      if(Pixle[i][j]==1&&Pixle[i][j-1]==1&&Pixle[i][j-2]==1&&Pixle[i][j-3]==1&&Pixle[i][j-4]==1&&Pixle[i][j-5]==1&&Pixle[i][j-6]==1&&Pixle[i][j+1]==1&&Pixle[i][j+2]==0)
      {
        Rig[i]=j+2;
        break;
      }
      if(j==Last_col)//��û���ҵ�����㣬��ſ�Χ��������
      {
        Rig_err[i]=1;
        for(k=Lef[i+1]+5;k<Rig[i+1]-10;k++)
        {
          if(Pixle[i][k]==1&&Pixle[i][k-1]==1&&Pixle[i][k-2]==1&&Pixle[i][k-3]==1&&Pixle[i][k-4]==1&&Pixle[i][k-5]==1&&Pixle[i][k-6]==1&&Pixle[i][k+1]==0&&Pixle[i][k+2]==0)
          {
            Rig[i]=k+2;
            break;
          }          
        }
      }
    }
    for(j=Lef[i+1]+10;j>Fir_col-1;j--)
    {
      if(Pixle[i][j]==1&&Pixle[i][j+1]==1&&Pixle[i][j+2]==1&&Pixle[i][j+3]==1&&Pixle[i][j+4]==1&&Pixle[i][j+5]==1&&Pixle[i][j+6]==1&&Pixle[i][j-1]==0&&Pixle[i][j-2]==0)//��С��·��Ե�İ׽�����Ӱ��
      {
        Lef[i]=j;
        break;
      }
      if(j==Fir_col)//��û���ҵ�����ſ�Χ����
      {
        Lef_err[i]=1;
        for(k=Rig[i+1]-5;k>Lef[i+1]+10;k--)
        {
          if(Pixle[i][k]==1&&Pixle[i][k+1]==1&&Pixle[i][k+2]==1&&Pixle[i][k+3]==1&&Pixle[i][k+4]==1&&Pixle[i][k+5]==1&&Pixle[i][k+6]==1&&Pixle[i][k-1]==0&&Pixle[i][k-2]==0)//��С��·��Ե�İ׽�����Ӱ��
          {
            Lef[i]=k;
            break;
          }          
        }
      }
    }
  }
  
  for(i=Last_row;i>Fir_row-1;i--)//ƽ��ֵ�������߻���
  {
//    if(Road0_flag==1)
//    {
//     if(Lef[i]<=8)
//       Lef[i]=8;
//     if(Rig[i]>=72)
//       Rig[i]=72;
//    }
      Mid[i]=(int)((Lef[i]+Rig[i])/2);
  }
}

/*************************************************************************
*  �������ƣ�void Pic_offset_fig(void)
*  ����˵��������offset
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2019.3.21
*  ��    ע��Cam_offsetΪ������ƫ�ң�����գ�
             Cam_offsetΪ������ƫ�����ҹգ�

*************************************************************************/
/*
void Pic_offset_fig(void)
{
  int i;
  int count=0,count2=0;
  if(1)
  {
    for(i=0;i<60;i++)
    {    
//          if(Road==1&&(Road1_flag==0||Road1_flag==2))
//          {  
//              if(New_Rig[i]!=MIDMAP)
//              {
//              Cam_offset=Cam_offset+New_Rig[i]-ROAD_HALF_WIDTH;
//              count++;
//              }
//              continue;
//          }
          if(Road==1&&(Road1_flag==3||Road1_flag ==1))
          {
            if(New_Rig[i]!=MIDMAP)
            {
              Cam_offset=Cam_offset+New_Rig[i]-ROAD_HALF_WIDTH;
              count++;
            }
            continue;
          }
           if(Road==2&&(Road2_flag==3||Road2_flag ==1))
          {
            if(New_Lef[i]!=-MIDMAP)
            {
              Cam_offset=Cam_offset+New_Lef[i]+ROAD_HALF_WIDTH;
              count++;
            }
            continue;
          }
       
       else if (i<=FIG_AREA_NEAR && i>=FIG_AREA_FAR)
      {
        if(New_Lef[i]!=-MIDMAP && New_Rig[i]!=MIDMAP)
        {
          Cam_offset=Cam_offset+New_Lef[i]+New_Rig[i];
          count++;
          continue;
        }
        else if(New_Lef[i]==-MIDMAP && New_Rig[i]!=MIDMAP)
        {
          Cam_offset=Cam_offset+New_Rig[i]-ROAD_HALF_WIDTH;
          count++;
          continue;
        }
        else if(New_Lef[i]!=-MIDMAP && New_Rig[i]==MIDMAP)
        {
          Cam_offset=Cam_offset+New_Lef[i]+ROAD_HALF_WIDTH;
          count++;
          continue;
        }
        else
        {
          continue;
        }
      }
       else if ((i<=FIG_AREA_FAR && i>=FIG_AREA_FAR2))
      {
        if(New_Lef[i]!=-MIDMAP && New_Rig[i]!=MIDMAP)
        {
          Cam_offset2=Cam_offset2+New_Lef[i]+New_Rig[i];
          count2++;
          continue;
        }
        else if(New_Lef[i]==-MIDMAP && New_Rig[i]!=MIDMAP)
        {
          Cam_offset2=Cam_offset2+New_Rig[i]-ROAD_HALF_WIDTH;
          count2++;
          continue;
        }
        else if(New_Lef[i]!=-MIDMAP && New_Rig[i]==MIDMAP)
        {
          Cam_offset2=Cam_offset2+New_Lef[i]+ROAD_HALF_WIDTH;
          count2++;
          continue;
        }
        else
        {
          continue;
        }
      }
        
    }
    if(Road0_flag && Road==0)
    {
      Cam_offset *= 0.3;
    }
    
    if(Road1_flag == 3||Road2_flag ==3)
     Cam_offset *= 0.8; 

    if(count!=0)
    {
      Cam_offset=Cam_offset/count;
    } 
    if(count2!=0)
    {
      Cam_offset2=Cam_offset2/count2;
    }
    
    Cam_offset=(0.4*Cam_offset+0.6*Cam_offset2);
    if(Road == 3||Road1_flag==2 || Road2_flag ==2)
    {
      Cam_offset *= 1;
    }
    return;
  }
  else 
  {
    Cam_offset=0;
  }
}
*/


/*************************************************************************
*  �������ƣ�void Pic_offset_fig(void)
*  ����˵�����ֶμ����·ƫ��ֵ��ʵ���˲�
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2019.3.6
*  ��    ע��

*************************************************************************/
/*
void Pic_offset_filter(void)
{
  Cam_offset_filter[3]=Cam_offset_filter[2];
  Cam_offset_filter[2]=Cam_offset_filter[1];
  Cam_offset_filter[1]=Cam_offset_filter[0];
  Cam_offset_filter[0]=Cam_offset;
  Cam_offset=Cam_offset_filter[0]*0.5+Cam_offset_filter[1]*0.2+Cam_offset_filter[2]*0.2+Cam_offset_filter[3]*0.1;
  Cam_offset *= 1.1;
  //Cam_offset=Cam_offset_filter[0]*0.45+Cam_offset_filter[1]*0.15+Cam_offset_filter[2]*0.15+Cam_offset_filter[3]*0.05+Cam_offset2*0.2;
}

// measured remapping of y
//int map_y[60]={
// 235,223,212,201,191,181,172,163,154,146,138,130,123,116,109,103,97,92,86,81,76,72,67,63,59,55,52,48,45,42,39,37,34,32,29,27,25,23,21,20,18,17,15,14,13,11,10,9,8,7,6,5,5,4,3,3,2,1,1,0
//};
*/
int temx;
int mid_map[60];
//my analitical version of remapping x and y
void pixel_undistort(int x,int y,int LR)
{
  temx=x;
  if(temx==2)temx=0;
  if(temx==78)temx=79;
  if(LR==0){
	New_Lef[y]=(int)(10*(temx-39)*offset_map[y]);
        if(New_Lef[y]<-MIDMAP)
          New_Lef[y]=-MIDMAP;
  }else{
	New_Rig[y]=(int)(10*(temx-39)*offset_map[y]);
        if(New_Rig[y]>MIDMAP)
          New_Rig[y]=MIDMAP;
  }
}

/*************************************************************************
*  �������ƣ�void Pic_undistort(void)
*  ����˵����ͼ��ȥ����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2019.3.21
*  ��    ע����Lef��Rig����ӳ�䴦��

*************************************************************************/
  
void Pic_undistort(void)
{
  int i;
  for(i=0;i<60;i++)
  {
    New_Lef[i]=-MIDMAP;
    New_Rig[i]=MIDMAP;
  }
  
  for(i=0;i<=55;i++)
  {
    pixel_undistort(Lef[i],i,0);		//change to analitical version
    if(Lef[i]<=Fir_col)
    {
      New_Lef[i]=-MIDMAP;//��ʧ�����
    }          
    pixel_undistort(Rig[i],i,1);		//change to analitical version
    if(Rig[i]>=Last_col-1)
    {
      New_Rig[i]=MIDMAP;//��ʧ�ұ���
    }
  }
}

/*************************************************************************
*  �������ƣ�void Pic_find_circle(void)
*  ����˵����ͼ���һ�״
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2019.3.21
*  ��    ע����״���壺���ұ�����������Ч�п�ʼ�ȼ�С����

*************************************************************************/

void Pic_find_circle(void)
{
  int i;
  Lef_circle=Rig_circle=0;
  Lef_break_point=Rig_break_point=0;
  for(i=55;i>2;i--)//�ӷ�ȫ���п�ʼѰ��
  {
    if(Last_col-Rig[i]<2||Last_col-Rig[i-12]<2)//���ұ����뿪�ұ߽翪ʼѰ��
    {
      continue;
    }
    /*if(Rig[i-2]<=Rig[i] && Rig[i-4]<=Rig[i-2] && Rig[i-7]>=Rig[i-5] && 
       Rig[i-9]>=Rig[i-7] && Rig[i-11]>=Rig[i-9]&& Rig[i-8]!=Rig[i-2] && 
       abs(Rig[i-8]-Rig[i-6])<5 && abs(Rig[i-6]-Rig[i-4])<5 && abs(Rig[i-4]-Rig[i-2])<5 && abs(Rig[i-2]-Rig[i])<5 && Rig[i-5]<=67)
    {
      Rig_circle=1;
      Rig_break_point=i-5;
      break;
    }
    */
    
    
   if(Rig[i-2]<=Rig[i] && Rig[i-4]<=Rig[i-2] && Rig[i-6]<=Rig[i-4] && Rig[i-8]<=Rig[i-6] && Rig[i-10]<=Rig[i-8] && Rig[i-12]>Rig[i-10]&&Rig[i-11]>Rig[i-9] 
       &&abs(Rig[i-8]-Rig[i-6])<4 && abs(Rig[i-6]-Rig[i-4])<4 && abs(Rig[i-4]-Rig[i-2])<4 && abs(Rig[i-2]-Rig[i])<4)
    {
      Rig_circle=1;
      Rig_break_point=i;
      break;
    }
   
  }
  for(i=55;i>10;i--)
  {
    if(Lef[i]-Fir_col<2||Lef[i-12]-Fir_col<2)
    {
      continue;
    }
    /*
    if(Lef[i-2]>=New_Lef[i] && Lef[i-4]>=Lef[i-2] && Lef[i-7]>=New_Lef[i-5] && //Lef[i-8]<=Lef[i-6] && 
       Lef[i-9]<=Lef[i-7] && Lef[i-11]<=Lef[i-19] && Lef[i-8]!=Lef[i-2] && 
         abs(Lef[i-8]-Lef[i-6])<5 && abs(Lef[i-6]-Lef[i-4])<5 && abs(Lef[i-4]-Lef[i-2])<5 && abs(Lef[i-2]-New_Lef[i])<5)
    {
      Lef_circle=1;
      Lef_break_point=i-5;
      break;
    }
    */
    if(Lef[i-2]>=Lef[i] && Lef[i-4]>=Lef[i-2] && Lef[i-6]>=Lef[i-4] && Lef[i-8]>=Lef[i-6] && Lef[i-10]>=Lef[i-8] && Lef[i-12]<Lef[i-10]&&Lef[i-11]<Lef[i-9] 
       && abs(Lef[i-8]-Lef[i-6])<4 && abs(Lef[i-6]-Lef[i-4])<4 && abs(Lef[i-4]-Lef[i-2])<4 && abs(Lef[i-2]-Lef[i])<4)
    {
      Lef_circle=1;
      Lef_break_point=i;
      break;
    }
    
  }
}

/*************************************************************************
*  �������ƣ�void Pic_find_leap(void)
*  ����˵����ͼ����ͻ���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2019.3.23
*  ��    ע��ͻ��㶨�壺���ұ��߲������ĵ�

*************************************************************************/

void Pic_find_leap(void)
{
  int i;
  
  int count1=0,count2=0;
  for(i=0;i<4;i++)
  {
    Lef_leap[i]=0;
    Rig_leap[i]=0;
  }
  for(i=Last_row;i>=Fir_row+4;i--)
  {
    if(abs(Lef[i]-Lef[i-2])<5 && abs(Lef[i-4]-Lef[i-2])>6)//������������ƫ����
    {
      Lef_leap[count1]=i-3;
      count1++;
      if(count1==4)//ͼ����������4����ֹͣѰ��
      {
        break;
      }
    }
  }
  
  for(i=Last_row;i>=Fir_row+4;i--)
  {
    if(abs(Rig[i]-Rig[i-2])<5 && abs(Rig[i-4]-Rig[i-2])>6)
    {
      Rig_leap[count2]=i-3;
      count2++;
      if(count2==4)
      {
        break;
      }
    }
  }
 
 }

void Cam_End_Detect(void)  //End��¼�����������һ�б߽���
{
  int i;
  
  for(i=50;i>=Fir_row;i--)
  {
    if(Lef[i]==1 && Pixle[i][2] ==0)Lef_End = i;
    if(Rig[i]==78&& Pixle[i][77] ==0)Rig_End = i;
    
    if(Lef[i]==1 && Rig[i]==78)break;// || (Lef[i]==1 && Pixle[i][2] ==1) || (Rig[i]==78 && Pixle[i][77] ==1))break;
  } 
  Cam_End = max_block(Lef_End,Rig_End);
}



int cnt_duanlu=0;
int end;
int Road6_flag=0;
void Cam_Break_Rec(void)
{
  int i;
  Cam_Break_Flag = 0;
  end = max_block(Lef_End,Rig_End)+1;
 
  
    if(end >= 30 && Road ==0)
    {
//      if(((Pixle[end-15][36]==1 && Pixle[end-15][44]==1)||(Pixle[end-18][36]==1 && Pixle[end-18][44]==1)) && Road == 0 && Start_Runing_Line == 0 )
//      {
//        Start_Runing_Line = 1;
//      }
      //Cam_Break_Flag = 1;
      Road  = 6;
     
     
    }
    if(Road == 6 && Road6_flag == 0)
    {
      for(i=58;i>50;i--)
      {
        if(Lef[i] ==1 && Rig[i] ==78)
        {
          continue;
        }
        else
        {
          return;
        }
      }
      Road6_flag = 1;
      return;
      
    }
    else if(Road == 6 && Road6_flag == 1)
    {
      for(i=58;i>50;i--)
      {
        if(Lef[i]!=1 || Rig[i]!=78 )
        {
          continue;
        }
        else
        {
          return;
        }
      }
      Road6_flag = 2;
      return;
    }
    else if(Road == 6 && Road6_flag == 2)
  {
    cnt_duanlu++;
    if(cnt_duanlu > 10)
    {
      cnt_duanlu = 0;
      Road = 0;
      Road6_flag = 0;
    
    }
     return;
  }

}

  
/*************************************************************************
*  �������ƣ�void Pic_Block_Rec(void)
*  ����˵����·��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2019.5.8
*  ��    ע��

*************************************************************************/
int Cam_Block_Cnt=0;
int Cam_Block_State=0;
int Road6_flag;
int Block_wrong;
int ramp_cnt =0;
int cnt_ramp=0;
char STOP_flag=0;
int black_line;
int start_stop = 0;
void Pic_Block_Rec(void)//�ϰ���ʶ��
{
  int i;
  //int j;
  int STOP_line = 0;
  //static char flag5 = 1;
  
  
  
  if(Road == 3)
  {
    cnt_ramp++;
    if(cnt_ramp > 50)
    {
      Road = 0;
      cnt_ramp = 0;
      Cam_Block_State = 0;
      return;
    }
    return;
  }
  else if( Road == 0 && Cam_Block_State == 0)//���ǰ40���Ҳ������ߣ���Ϊ�Ƕ�·����������
  {
     if(icm_gyro_y<-3000)
        {
             Road =3;
             return;
        }
     
      for(i=1;i<60;i++)//���϶���Ѱ���б��ߵĿ�ʼ��
      {
        if(Lef[i]==1 && Rig[i]==78)
        {
          continue;
        }
        else
        {
          STOP_line = i;
          break;
        }
       }
     start_stop++;
     if(start_stop>600)
     {
       start_stop=601;
      for(i=15;i<40;i++)
      {
        if(Lef[i]==1 && Rig[i]==78)
        {
          black_line++;
          if(black_line > 3 && STOP_line < 5 && Lef_slope !=999 && Rig_slope != 999)
          { 
            Road = 7;
            return;
          }
          
        }
        else
        {
          black_line = 0;
        }
      }
     }
//      if(STOP_line < 30&&STOP_line> 15)
//      {
//        if(FINAL[STOP_line-10]==1 && FINAL[STOP_line-11]==1 && FINAL[STOP_line-12]==1 && FINAL[STOP_line-13]==1 && FINAL[STOP_line-14]==1 && FINAL[STOP_line+1] ==1 && FINAL[STOP_line+2] == 1 )
//        { Road=7;
//          return;
//        }
//      }
      if(STOP_line>30)
      { Road = 6;
        return;
      }
      if(STOP_line>5&&STOP_line<15 && Lef_slope!=999 &&Rig_slope!=999 && LR_slope <0.1 && LR_slope> -0.1) 
       Cam_Block_State = 1;
        return;
   }
  
/*
  else if(Cam_Block_State == 1)//�����С��100ʱ������Ϊ�����ϰ��ﻹ��1�׵ľ��룬��ʼ���п���ת�򣬽���״̬2��ʹ������ƫתԼ60��Ƕ�
  {
    
    
   if(ToF_distance > Tof_thres && flag5 ==1)//ToF_distance < 6000
    {
      flag5 = 0;
      Road = 5;//��·���ϰ����־λ 
      Turn_Angle_flag=1;
      Cam_Block_State = 2;
      Cam_Block_Cnt = 0;
      Turn_Angle=0;
      return;
    }
   else
     Cam_Block_State = 0;
    
  }
  
  else if(Cam_Block_State == 2)//������100*10ms = 1s�����״̬3,ʹ������60���ǰ��һ�����룬�뿪����
  {
    if( block_flag == 1)
    {
    if( (Block_Angle_Set - Turn_Angle) <1)//Cam_Block_Cnt == 100)
    {
      Cam_Block_State = 3;
      Cam_Block_Cnt = 0;
      Turn_Angle=0;
    }
    }
    else if(block_flag == -1)
    {
    if( (Block_Angle_Set + Turn_Angle) <1)//Cam_Block_Cnt == 100)
    {
      Cam_Block_State = 3;
      Cam_Block_Cnt = 0;
      Turn_Angle=0;
    }
    }
  }
  else if(Cam_Block_State == 3)//������100*10ms = 1s�����״̬5�����ֳ���Ƕ���ǰ�н�һ������
  {
    if( block_flag == 1)
    {
    if( (Turn_Angle+Block_Angle_Set2) <1)//Cam_Block_Cnt == 100)
    {
      Cam_Block_State = 4;
      Cam_Block_Cnt = 0;
      Turn_Angle=0;
    }
    }
    else if( block_flag == -1)
    {
    if( (Block_Angle_Set2 - Turn_Angle) <1)//Cam_Block_Cnt == 100)
    {
      Cam_Block_State = 4;
      Cam_Block_Cnt = 0;
      Turn_Angle=0;
    }
    }
  }  
  else if(Cam_Block_State == 4)//������100*10ms = 1s�����״̬7��ʹ�����ֽǶȻص�����
  {
    if( block_flag == 1)
    {
    if( (Block_Angle_Set - Turn_Angle) <1)//Cam_Block_Cnt == 100)
    {
      Cam_Block_State = 0;
      Road = 0;
      Turn_Angle_flag=0;
      Cam_Block_Cnt = 0;
    }
    }
    else if( block_flag == -1)
    {
    if( (Block_Angle_Set + Turn_Angle) <1)//Cam_Block_Cnt == 100)
    {
      Cam_Block_State = 0;
      Road = 0;
      Turn_Angle_flag=0;
      Cam_Block_Cnt = 0;
    } 
    }
    
  }
  
  */
 
  return ;
    
}
  

  


  

