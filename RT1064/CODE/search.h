#ifndef _Picpro_H
#define _Picpro_H
#define undistort1

#include "headfile.h"
void Pic_seedfill(void); 
char Final_line(void);
void Pic_seedfill_grow(uint8 flag[CAMERA_H][CAMERA_W],int i,int j);   //DFS搜索算法递归函数
extern float KP;
extern void Pic_DrawMid(void);//图像绘制中心线
extern void Pic_noi_elim(void);//图像噪点去除
extern void Road_rec(void);
extern void Pic_offset_fig(void);//offset计算
extern void Pic_offset_filter(void);//offset滤波处理
extern void Pic_main(void);
extern void Pic_Fix_Line(void);//补线程序
// extern void Pic_thresh_get(void);//没有用到
extern void LR_Slope_fig(void);
#ifdef undistort0
extern void Pic_undistort(void);//图像去畸变
extern void pixel_undistort(int x,int y,int LR);//像素去畸变
#endif
#ifdef undistort1
extern void Pic_undistort(int L ,int R);//图像去畸变
#endif
//extern void Pixel_undistort(int x,int y,int LR);//像素去畸变
extern void Pic_find_circle(void);//左右黑线寻找环状黑线
extern void Pic_find_leap(void);
extern void Allwhite_find(void);
extern void Pic_precal(void);
extern void grow(int Pic_precal_done[60][80],int Pic_precal_flag[60][80],int i,int j);
//extern void my_key_debug();
void Pic_Block_Rec(void);
extern float L_R_Slope(char LR);
void Cam_Break_Rec(void);
void Cam_End_Detect(void); //End记录从上至下最后一行边界行
void SideLine_Find(int find_mode);
int Pic_Pixle_jl(int row,int col);
int Pic_Pixle_jr(int row,int col);

extern float Slope(int F1x,int F1y,int F2x,int F2y);
extern  int Pic_cur_fig(int AX, int AY, int BX, int BY, int CX, int CY);

#define LCDH    60  //OLED显示的行数
#define LCDW    80  //OLED显示的列数

extern int Lef[LCDH]; //道路左分离点的纵坐标
extern int Rig[LCDH]; //道路右分离点的纵坐标
extern int Mid[LCDH]; //道路中心点的纵坐标

extern int Lef_leap[4];
extern int Rig_leap[4];//左右变线跳变点坐标
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
extern float Cam_offset;


//#define Car_Speed_Str     2200   //直道速度
//#define Car_Speed_Cur1    1800   //弯道速度1
//#define Car_Speed_Cur2    1950   //弯道速度2
//#define Car_Speed_Cur3    500   //弯道速度3

#define Car_Speed_Str     1   //直道速度
#define Car_Speed_Cur1    0.8   //弯道速度1
#define Car_Speed_Cur2    0.9   //弯道速度2
#define Car_Speed_Cur3    0.5   //弯道速度3

#ifdef undistort0
    #define MIDMAP 300  //undistort0

    #define FIG_AREA_NEAR   50//350
    #define FIG_AREA_FAR   28//550   //逆透视变换后的图像计算区域
    #define FIG_AREA_FAR2   20//600
    #define FIG_AREA_FAR3   15//700

    // #define FIG_AREA_NEAR1   100//350
    // #define FIG_AREA_FAR1   200//550   //逆透视变换后的图像计算区域
#endif 

#ifdef undistort1
    #define MIDMAP 860  //undistort1

    #define FIG_AREA_NEAR   57//50//350/*(59-tempNewy(50)/Bili)(Bili=12)
    #define FIG_AREA_FAR   44//28//550   //逆透视变换后的图像计算区域
    #define FIG_AREA_FAR2   44//20//600
    #define FIG_AREA_FAR3   39//15//700
#endif

#define ROAD_HALF_WIDTH  150  //赛道半宽
#define ROAD_WIDTH  300  //赛道宽

#define Fir_row      START_LINE+1        //以第7行作为最小有效行
#define Last_row     (LCDH-2)    //以该行为最大有效行
#define Fir_col      2         //以第7列作为最小有效列
#define Last_col     (LCDW-3)    //以该列为最大有效列
//#define Middle       40        //图片视野中心位置


#define Fir_col_cross      8        //以第7列作为最小有效列
#define Last_col_cross     (LCDW-8)    //以该列为最大有效列


#if 0
#define UNDISTORT_FX      1.1911        //内参矩阵和畸变系数
#define UNDISTORT_FY      1.1890
#define UNDISTORT_CX      77.1315   
#define UNDISTORT_CY      58.4910
#define UNDISTORT_K1      (-0.4077)  
#define UNDISTORT_K2      0.1326
#endif 

//图像DFS搜索算法起始搜索区域
#define seed_down	55
#define seed_up		40
#define seed_left	30
#define seed_right	50

#define LEFT        0
#define RIGHT       79
#define TOP         0
#define BOTTOM      59



//倒跑
extern float err;
extern int angle;
extern float offset_map[60];
extern int Lef_edge,Rig_edge;
extern int Road6_flag;
extern int ramp_cnt ;
extern int iii;
extern int block_flag;
#endif
#if 0
extern float zhidaosudu ;//直道速度
extern float xiaowandaosudu ;//小弯道速度
extern float dawandaosudu  ;//大弯道速度
extern float duanlusudu ;//断路速度
extern float luzhangsudu ;//路障速度
extern int Tof_thres;//障碍物检测阈值
#endif

extern int camera_offset ;//摄像头二值化阈值

extern int threshold_offset;