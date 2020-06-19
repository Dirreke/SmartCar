#include "headfile.h"


uint8 Sobel_Threshold_FarFar = 70;
uint8 Sobel_Threshold_Far = 80;
uint8 Sobel_Threshold_Near = 100;   

void Para_Init()
{
        threshold_offset = -5;
        
  	PID_SPEED.P=0.29;//2.9;//0.50
	PID_SPEED.I=0.25;//0.0014;
	PID_SPEED.D=0.000;//0.030
  	// PID_TURN.P=0.0102*325;//0.0118;//0.0112;//
	// PID_TURN.D=-0.0042*325;//-0.0053;//-0.004//3.7ÊÔ¸Ä-0.0035;
        PID_TURN_CAM_EXT.P = 0.85;
        PID_TURN_CAM_EXT.P = 0.8;
        MotorOut1=0;
        MotorOut2=0;

//diff.h and speed.h init
        lib_speed_init();

        lib_set_speed(2);
#ifdef DIFF0
        lib_active_diff_init();
        lib_active_diff_set_p(0.1);
        lib_active_diff_set_d(0.1);
#endif
        //¿ª²îËÙ
        // diff_on();
       diff_off();
  
  
        
}