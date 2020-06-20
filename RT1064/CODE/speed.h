#include "headfile.h"
extern float CarSpeed1;
extern float CarSpeed2;
extern float SpeedGoal;

void qtimer_AB_init(void);
void Get_Speed(void);
















//²îËÙ
static enum diff_state_def{
  DIFF_ON_VAL,
  DIFF_OFF_VAL,
} diff_state;

char get_diff_state(void);
void diff_on(void);
void diff_off(void);