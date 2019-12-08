#ifndef _SPEED_H_
#define _SPEED_H_

static float active_diff_val_range = 45.0;

typedef enum{
  LIB_TIRE_LEFT,
  LIB_TIRE_RIGHT
} tire_type;

float lib_get_speed(tire_type a);
//float get_speed(void);
void lib_set_speed(float a);

void lib_active_diff_set(int a);
//int lib_active_diff_get(void);
void lib_active_diff_init(void);
void lib_active_diff_input(int a);
void lib_active_diff_set_p(float a);
void lib_active_diff_set_d(float a);

void lib_speed_init(void);
void lib_speed_utility(void);
//void lib_set_ramp_speed(float a,float b,int ms);

#endif
