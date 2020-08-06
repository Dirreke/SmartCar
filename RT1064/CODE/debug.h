#include "headfile.h"
#define DEBUG_KEY0 D4
#define DEBUG_KEY1 C27
#define DEBUG_KEY2 C31
#define DEBUG_KEY3 C26
#define DEBUG_KEY4 D27
#define DEBUG_KEY5 B10

extern bool kaiji_flag;
extern int DEBUG_CHOICE;
// extern float speedgoal;
// extern float curvespeedgoal;

void Debug_Init(void);
void Debug_key(void);

void ips_show_debug(int ips_num);
void ips_show_debug_pd(int ips_num);

extern int DDDebug;
extern float Cam_P_New;