#ifndef _EM_H_
#define _EM_H_

#include "headfile.h"

extern float EM_Value_1;
extern float EM_Value_2;
extern float EM_Value_3;
extern float EM_Value_4;
extern float EM_center_offset;
extern float EM_straight_offset;
// extern int  EM_Road;
extern int EM_edge;

void EM_main(void);
void EM_Init(void);
void EM_Get(void);
// void EM_Curve_Rec(void);
// void EM_Ring_Rec(void);
// void EM_offset_fig(void);
// void EM_offset_filter(void);
void EM_center_offset_fig(void);
void EM_center_offset_filter(void);
void EM_straight_offset_fig(void);
void EM_straight_offset_filter(void);


// int limit_em(int n, int lower, int higher);
// float abs_f_em(float n);


#endif