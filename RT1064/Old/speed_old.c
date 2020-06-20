#include "headfile.h"

static float speed;
#ifdef DIFF0
static int active_diff_val;		//>0:active turn left;<0 active turn right
static float diff_bias = 0.3;
#endif

#define diff_throttle_ratio 2.0   //lib_speed_utility

typedef enum{
  ramp_speed_,
  ramp_speed_hold_,
  ramp_speed_reset_,
  off
} utility_typedef;

static struct {
  //function type
  utility_typedef utility_type;
  //control block
  char start;
  char stop;
  char unused;
  
  //RAMP CONTROL
  int ramp_count;
  int ramp_reload_val;
  float ramp_to;
  float ramp_step;
  int ramp_count_back;
  
  
} utility_s;
#ifdef DIFF0
static struct {
  float p;
  float d;
} diff_pid;

//basic speed ctl
/*0****************************************************
***左右轮速度
***输入参数：LIB_TIRE_LEFT/LIB_TIRE_RIGHT
***输出参数：速度
***说明：(active_diff_val)*speed/active_diff_val_range，右轮比左轮快。
          diff_bias+speed 速度+偏置
********************************************/

float lib_get_speed(tire_type a)
{
  return (a==LIB_TIRE_LEFT)? (diff_bias+speed-(float)(active_diff_val)*speed/2.0/active_diff_val_range):(diff_bias+speed+(float)(active_diff_val)*speed/2.0/active_diff_val_range);
}
#endif

void lib_set_speed(float a)
{
  utility_s.start=0;
  utility_s.stop=1;
  speed = a;
  #ifdef DIFF0
  diff_bias = -active_diff_val*speed/active_diff_val_range/diff_throttle_ratio;//a/5;
  #endif 
  
}
#ifdef DIFF1
float get_speed()
{
  return speed;
}
#endif 

#ifdef DIFF0
float get_speed()
{
  return speed+diff_bias;
}
//active diff function
void lib_active_diff_init(void){
	lib_active_diff_set(0);
	diff_pid.p=1;
	diff_pid.d=0.01;
}

void lib_active_diff_set(int a){
	active_diff_val = a;
}


void lib_active_diff_input(int a)
{
  static int last_diff;
  lib_active_diff_set((int)(diff_pid.p * a + (a - last_diff) * diff_pid.d));
  last_diff = a;
}


void lib_active_diff_set_p(float a){
  diff_pid.p=a;
}

void lib_active_diff_set_d(float a){
  diff_pid.d=a;
}
#endif

/////////////////////////////////////////
//speed lib init
void lib_speed_init(void){
    utility_s.ramp_count=0;
    utility_s.ramp_step=0;
    utility_s.ramp_to=0;
    utility_s.start=0;
    utility_s.stop=1;
    utility_s.utility_type=off;
    utility_s.ramp_reload_val=0;
}

void lib_speed_utility(void){
  
  if(utility_s.start==1){
    switch(utility_s.utility_type){
    case ramp_speed_:{
      if(utility_s.ramp_count==0){
        //utility_s.stop=1;
        //utility_s.start=0;
        utility_s.utility_type=off;
      }
      else{
        speed=(utility_s.ramp_to-utility_s.ramp_step*utility_s.ramp_count);
        #ifdef DIFF0
        diff_bias = -active_diff_val*speed/active_diff_val_range/diff_throttle_ratio;//a/5;
        #endif
        utility_s.ramp_count = utility_s.ramp_count - 1;
      }
    }break;
    case ramp_speed_hold_:{
      //utility_s.ramp_count = utility_s.ramp_reload_val;
      if(utility_s.ramp_reload_val>=utility_s.ramp_count) utility_s.ramp_count+=utility_s.ramp_count_back;          //decrease speed by 2 steps
      utility_s.utility_type=ramp_speed_;               //go back to speed up mode
      speed=(utility_s.ramp_to-utility_s.ramp_step*utility_s.ramp_count);
      #ifdef DIFF0
      diff_bias = -active_diff_val*speed/active_diff_val_range/diff_throttle_ratio;//a/5;
      #endif
    }break;
    case ramp_speed_reset_:{
       utility_s.ramp_count=utility_s.ramp_reload_val;
        speed=(utility_s.ramp_to-utility_s.ramp_step*utility_s.ramp_count);
        #ifdef DIFF0
        diff_bias = -active_diff_val*speed/active_diff_val_range/diff_throttle_ratio;//a/5;
        #endif
        utility_s.utility_type=ramp_speed_;
    }break;
    case off:break;
    default: break;
    }
  }
}









