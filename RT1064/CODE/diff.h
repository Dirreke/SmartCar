#ifndef _DIFF_H_
#define _DIFF_H_

//user macros controlling EM_ANGLE
//#define USE_EM_ANGLE
#define DIFF_ON

static enum diff_state_def{
  DIFF_ON_VAL,
  DIFF_OFF_VAL,
} diff_state;


//static int angle;

//int angle_get(void);
//void angle_set(int a);
//void uniform_angle_set(int a,int max);

void diff_on(void);
void diff_off(void);

//char get_diff_state(void);

#endif
