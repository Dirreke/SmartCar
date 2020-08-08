#include"headfile.h"








extern int Allwhitestart;
extern int Allwhiteend;
extern uint8 whiteline_start;
extern uint8 whiteline_end;
extern int Road;
extern int Road1_flag;
extern int Road2_flag;
extern int Road0_flag;
extern int Road7_flag;
extern int Road3_flag;
extern int Road4_flag;
extern bool Road0_flag0_flag;

// extern int EM_Road;
// extern bool ganhuangguan_flag;
// extern bool barn_reset_flag;

extern volatile int turn_stop;//◊™Õ‰÷’÷πœﬂ

extern int Lef_circle;
extern int Rig_circle;
extern int Lef_break_point;
extern int Rig_break_point;
extern bool Lef_circle_pre_flag;
extern bool Rig_circle_pre_flag;

extern bool Lef_innercurve ;
extern bool Rig_innercurve ;
extern uint8 Lef_innercurve_point ;
extern uint8 Rig_innercurve_point ;


// extern int Lef_break_point_und = 0;
// extern int Rig_break_point_und = 0;

extern bool crossing_flag;

extern int start_stop_line_flag;
extern int start_stop_line;
extern int barn_line;
extern int start_line;



extern int stop_line;
extern float ramp_ANGLE_3;

void Allwhite_find(void);
void Pic_find_circle(void);
void Pic_find_circle_pre(void);
void Pic_find_innercurve(void);
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
