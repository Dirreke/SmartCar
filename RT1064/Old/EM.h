#ifndef _EM_H_
#define _EM_H_

#include "headfile.h"

extern float EM_Value_1;
extern float EM_Value_2;
extern float EM_Value_3;
extern float EM_Value_4;
extern float EM_center_offset;
extern float EM_straight_offset;

void EM_main(void);
void EM_Init(void);
void EM_Get(void);
void EM_center_offset_fig(void);
void EM_center_offset_filter(void);
#endif