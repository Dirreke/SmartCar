#include"headfile.h"








extern int Allwhitestart;
extern int Allwhiteend;

extern int Road;
extern int Road1_flag;
extern int Road2_flag;
extern int Road0_flag;
extern int Road7_flag;
extern int Road3_flag;
extern int Road4_flag;

extern int EM_Road;
extern bool ganhuangguan_flag;
extern bool barn_reset_flag;

extern volatile int turn_stop;//ת����ֹ��

extern int Lef_circle;
extern int Rig_circle;
extern int Lef_break_point;
extern int Rig_break_point;
extern bool Lef_circle_pre_flag;
extern bool Rig_circle_pre_flag;
// extern int Lef_break_point_und = 0;
// extern int Rig_break_point_und = 0;

extern bool crossing_flag;

extern int start_stop_line_flag;
extern int start_stop_line;
extern int barn_line;
extern int start_line;



extern int stop_line;

void Allwhite_find(void);
void Pic_find_circle(void);
void Pic_find_circle_pre(void);
void break_point_find_und(bool L,bool R);
void start_stop_find(void);
void mag_find(void);
void crossing_find(void);
void Road_rec(void);

void TurnLeft_Process(void);
void TurnRight_Process(void);
void Road1_zhuangtaiji(void);
void Road2_zhuangtaiji(void);
void Road3_zhuangtaiji(void);
void Road4_zhuangtaiji(void);
void Road7_zhuangtaiji(void);
extern bool crossing_flag_flag;
extern void crossing_find2(void);
