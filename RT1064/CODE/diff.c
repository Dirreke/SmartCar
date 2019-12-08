#include "headfile.h"
//得到差速状态
char get_diff_state(void){
  return diff_state;
}
//开关差速
void diff_on(void){
  diff_state = DIFF_ON_VAL;
}

void diff_off(void){
  diff_state = DIFF_OFF_VAL;
}
