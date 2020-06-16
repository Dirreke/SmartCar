#include"headfile.h"








extern int Allwhitestart;
extern int Allwhiteend;

extern int Road;
extern int Road1_flag;
extern int Road2_flag;
extern int Road0_flag;
extern int Road7_flag;

extern int turn_stop;//转弯终止线

extern float Lef_slope;
extern float Rig_slope;
extern int Lef_circle;
extern int Rig_circle;
extern int Lef_break_point;
extern int Rig_break_point;

extern int start_stop_line;

extern float Cam_offset;

void camera_dispose_main(void);
void Allwhite_find(void);
void Pic_find_circle(void);

void Road_rec(void);
void start_stop_rec(void);

void Threshold_change(void);

void Pic_Fix_Line(void);
void fangyuejie(void);

void Pic_offset_fig(void);
void Pic_offset_filter(void);



