#include "headfile.h"

void ICM_get(void);
void ICMx_filter(void);
void ICMy_filter(void);
void ICMz_filter(void);
int16 gyro_y_mean_filter(int D_new);
void gyro_y_init(void);
void gyro_y_integration(void);

extern float icm_gyro_y_offset;
extern float icm_gyro_y_angle;
extern float icm_gyro_y_float;
