#ifndef _Picpro_H
#define _Picpro_H
#include "headfile.h"
void Pic_seedfill(void); 
char Final_line(void);
void Pic_seedfill_grow(uint8 flag[CAMERA_H][CAMERA_W],int i,int j);   //DFS�����㷨�ݹ麯��
extern float KP;
extern void Pic_DrawMid(void);//ͼ�����������
extern void Pic_noi_elim(void);//ͼ�����ȥ��
extern void Road_rec(void);
extern void Pic_offset_fig(void);//offset����
extern void Pic_offset_filter(void);//offset�˲�����
extern void Pic_main(void);
extern void Pic_Fix_Line(void);//���߳���
extern void Pic_thresh_get(void);
extern void LR_Slope_fig(void);
extern void Pic_undistort(void);//ͼ��ȥ����
extern void pixel_undistort(int x,int y,int LR);//����ȥ����
//extern void Pixel_undistort(int x,int y,int LR);//����ȥ����
extern void Pic_find_circle(void);//���Һ���Ѱ�һ�״����
extern void Pic_find_leap(void);
extern void Allwhite_find(void);
extern void Pic_precal(void);
extern void grow(int Pic_precal_done[60][80],int Pic_precal_flag[60][80],int i,int j);
//extern void my_key_debug();
void Pic_Block_Rec(void);
extern float L_R_Slope(char LR);
void Cam_Break_Rec(void);
void Cam_End_Detect(void); //End��¼�����������һ�б߽���
void SideLine_Find(int find_mode);
int Pic_Pixle_jl(int row,int col);
int Pic_Pixle_jr(int row,int col);

extern float Slope(int F1x,int F1y,int F2x,int F2y);
extern  int Pic_cur_fig(int AX, int AY, int BX, int BY, int CX, int CY);

#define LCDH    60  //OLED��ʾ������
#define LCDW    80  //OLED��ʾ������

extern int Lef[LCDH]; //��·�������������
extern int Rig[LCDH]; //��·�ҷ�����������
extern int Mid[LCDH]; //��·���ĵ��������

extern int Lef_leap[4];
extern int Rig_leap[4];//���ұ������������
extern int Fix_line;
extern int Road;
extern int Road1_flag;
extern int Road2_flag;
extern int Allwhitestart;
extern int Allwhiteend;
extern int Allwhitestart2;
extern int Allwhiteend2;

extern float Lef_slope;
extern float Rig_slope;
extern float LR_slope;
extern int Lef_circle;
extern int Rig_circle;
extern int Lef_break_point;
extern int Rig_break_point;
extern int Cam_Block_State;
extern int Middle;
extern int end;


//#define Car_Speed_Str     2200   //ֱ���ٶ�
//#define Car_Speed_Cur1    1800   //����ٶ�1
//#define Car_Speed_Cur2    1950   //����ٶ�2
//#define Car_Speed_Cur3    500   //����ٶ�3

#define Car_Speed_Str     1   //ֱ���ٶ�
#define Car_Speed_Cur1    0.8   //����ٶ�1
#define Car_Speed_Cur2    0.9   //����ٶ�2
#define Car_Speed_Cur3    0.5   //����ٶ�3

#define MIDMAP 300

#define FIG_AREA_NEAR   50//350
#define FIG_AREA_FAR   28//550   //��͸�ӱ任���ͼ���������
#define FIG_AREA_FAR2   20//600
#define FIG_AREA_FAR3   15//700

#define FIG_AREA_NEAR1   100//350
#define FIG_AREA_FAR1   200//550   //��͸�ӱ任���ͼ���������

#define ROAD_HALF_WIDTH  150  //�������

#define Fir_row      2        //�Ե�7����Ϊ��С��Ч��
#define Last_row     (LCDH-2)    //�Ը���Ϊ�����Ч��
#define Fir_col      2         //�Ե�7����Ϊ��С��Ч��
#define Last_col     (LCDW-3)    //�Ը���Ϊ�����Ч��
//#define Middle       40        //ͼƬ��Ұ����λ��


#define Fir_col_cross      8        //�Ե�7����Ϊ��С��Ч��
#define Last_col_cross     (LCDW-8)    //�Ը���Ϊ�����Ч��



#define UNDISTORT_FX      1.1911        //�ڲξ���ͻ���ϵ��
#define UNDISTORT_FY      1.1890
#define UNDISTORT_CX      77.1315   
#define UNDISTORT_CY      58.4910
#define UNDISTORT_K1      (-0.4077)  
#define UNDISTORT_K2      0.1326

//ͼ��DFS�����㷨��ʼ��������
#define seed_down	55
#define seed_up		40
#define seed_left	30
#define seed_right	50

#define LEFT        0
#define RIGHT       79
#define TOP         0
#define BOTTOM      59



//����
extern float err;
extern int angle;
extern float offset_map[60];
extern int Lef_edge,Rig_edge;
extern int Road6_flag;
extern int ramp_cnt ;
extern int iii;
extern int block_flag;
#endif
extern float zhidaosudu ;//ֱ���ٶ�
extern float xiaowandaosudu ;//С����ٶ�
extern float dawandaosudu  ;//������ٶ�
extern float duanlusudu ;//��·�ٶ�
extern int camera_offset ;//����ͷ��ֵ����ֵ
extern int Tof_thres;//�ϰ�������ֵ
extern float luzhangsudu ;//·���ٶ�
extern int threshold_offset;