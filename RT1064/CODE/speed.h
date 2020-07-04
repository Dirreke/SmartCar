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
char set_diff_state(char val);
void diff_on(void);
void diff_off(void);
float speed_mean_filter1(float D_new);
float speed_mean_filter2(float D_new);
void lib_speed_set(float a);

