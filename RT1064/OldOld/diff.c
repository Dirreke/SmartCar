#include "headfile.h"
//�õ�����״̬
char get_diff_state(void){
  return diff_state;
}
//���ز���
void diff_on(void){
  diff_state = DIFF_ON_VAL;
}

void diff_off(void){
  diff_state = DIFF_OFF_VAL;
}
