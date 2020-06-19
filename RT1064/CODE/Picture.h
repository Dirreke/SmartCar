#include"headfile.h"

#define CAMERA_W 80
#define CAMERA_H 60

#define LCDH 60
#define LCDW 80

#define MIDMAP 650  //undistort1

#define FIG_AREA_NEAR   54//50//350/*(59-tempNewy(50)/Bili)(Bili=12)
#define FIG_AREA_FAR   5//28//550   //逆透视变换后的图像计算区域

#define ROAD_HALF_WIDTH  150  //赛道半宽
#define ROAD_WIDTH  300  //赛道宽

#define START_LINE 19

#define Fir_row      START_LINE+1        //以第7行作为最小有效行
#define Last_row     (LCDH-2)    //以该行为最大有效行
#define Fir_col      2         //以第7列作为最小有效列
#define Last_col     (LCDW-3)    //以该列为最大有效列


#if 0
//图像DFS搜索算法起始搜索区域
#define seed_down	55
#define seed_up		40
#define seed_left	30
#define seed_right	50

#define LEFT        0
#define RIGHT       79
#define TOP         0
#define BOTTOM      59
#endif 


#define Sobel_Near_Far_Line 54
#define Sobel_Far_FarFar_Line 30
extern uint8 Sobel_Threshold_FarFar;
extern uint8 Sobel_Threshold_Far; 
extern uint8 Sobel_Threshold_Near;   

__ramfunc void Get_Use_Image(void);

uint8_t GetOSTU(uint8_t tmImage[CAMERA_H][CAMERA_W]);
void sobel(void); //Sobel边沿检测

void Pic_noi_elim(void);

void Pic_DrawLRside(void);
void Pic_undistort(int L,int R);

void Pic_particular(void);

void LR_Slope_fig(void);
float Slope(int F1x, int F1y, int F2x, int F2y);

void Threshold_change(void);
void Pic_Fix_Line(void);
void fangyuejie(void);

void Pic_DrawMid(void);
void Pic_DrawMid_und(void);

void Get_pic_with_edge(void);

#if 0
void Pic_seedfill(void);
void Pic_seedfill_grow(uint8 flag[CAMERA_H][CAMERA_W], int i, int j);
#endif 

extern uint8 Image_Use[CAMERA_H][CAMERA_W];
extern uint16 Pixle[CAMERA_H][CAMERA_W]; //二值化后用于OLED显uint16氖???//u16

extern int threshold_offset;
extern int threshold_offset2;

extern int whitecnt;

extern int Lef[LCDH];     //道路左分离点的纵坐标
extern int Rig[LCDH];     //道路右分离点的纵坐标
extern int Mid[LCDH]; //道路中心点的纵坐标

extern int New_Lef[60];
extern int New_Rig[60]; //用于存储逆透视变换后的横坐标
extern int New_Mid[60];

extern int Lef_edge, Rig_edge;

extern float Lef_slope, Rig_slope;

