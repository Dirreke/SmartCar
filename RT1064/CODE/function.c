#include "headfile.h"

uint8 Sobel_Threshold_FarFar = 70;
uint8 Sobel_Threshold_Far = 80;
uint8 Sobel_Threshold_Near = 100;
uint8 competition_strategy = 0;

void Para_Init()
{
        switch (competition_strategy)
        {
        case 0:
                threshold_offset = 0;
                Road = 3;
                Road3_flag = 0;
                PID_SPEED.P = 50;    //2.9;//0.50
                PID_SPEED.I = 20;    //10;    //0.25;//0.0014;
                PID_SPEED.D = 0.000; //0.030
                                     //PID2_SPEED.P=100;//2.9;//0.50
                                     //PID2_SPEED.I=100;//0.25;//0.0014;
                                     // PID_TURN.P=0.0102*325;//0.0118;//0.0112;//
                                     // PID_TURN.D=-0.0042*325;//-0.0053;//-0.004//3.7试改-0.0035;
                // PID_TURN_CAM_EXT.P = 0.85;
                // PID_TURN_CAM_EXT.D = 1.8; //0.8;
                PID_CENTER_EM.P = 9;        //4
                PID_CENTER_EM.D = 12;       //9
                PID_STRAIGHT_EM.P = 0.7;    //0.7;//0.5;
                PID_STRAIGHT_EM.D = 0.6;    //0.6;//0.4;
                PID_CAR_CENTER_CAM.P = 0.5; //0.3;//0.5;
                PID_CAR_CENTER_CAM.D = 0;
                PID_CAR_STRAIGHT_CAM.P = 0.8; //0.7;//0.6;//0.7;
                PID_CAR_STRAIGHT_CAM.D = 0;
                //PID_CAR_Diffcomp_CAM.P = 0; //0.6;//0;//0.6;
                //PID_CAR_Diffcomp_CAM.D = 0;
                PID_diff.P = 0.5; //1;
                PID_diff0.P = 0.2;
                barn_reset_flag = 0;
                DEBUG_CHOICE = 2;
                
                MotorOut1 = 0;
                MotorOut2 = 0;
                break;

        default:
                threshold_offset = 0;
                Road = 3;
                Road3_flag = 0;
                PID_SPEED.P = 50;    //2.9;//0.50
                PID_SPEED.I = 20;    //10;    //0.25;//0.0014;
                PID_SPEED.D = 0.000; //0.030
                                     //PID2_SPEED.P=100;//2.9;//0.50
                                     //PID2_SPEED.I=100;//0.25;//0.0014;
                                     // PID_TURN.P=0.0102*325;//0.0118;//0.0112;//
                                     // PID_TURN.D=-0.0042*325;//-0.0053;//-0.004//3.7试改-0.0035;
                // PID_TURN_CAM_EXT.P = 0.85;
                // PID_TURN_CAM_EXT.D = 1.8; //0.8;
                PID_CENTER_EM.P = 9;        //4
                PID_CENTER_EM.D = 12;       //9
                PID_STRAIGHT_EM.P = 0.7;    //0.7;//0.5;
                PID_STRAIGHT_EM.D = 0.6;    //0.6;//0.4;
                PID_CAR_CENTER_CAM.P = 0.5; //0.3;//0.5;
                PID_CAR_CENTER_CAM.D = 0;
                PID_CAR_STRAIGHT_CAM.P = 0.8; //0.7;//0.6;//0.7;
                PID_CAR_STRAIGHT_CAM.D = 0;
                //PID_CAR_Diffcomp_CAM.P = 0; //0.6;//0;//0.6;
                //PID_CAR_Diffcomp_CAM.D = 0;
                PID_diff.P = 0.5; //1;
                PID_diff0.P = 0.2;
                barn_reset_flag = 0;
                DEBUG_CHOICE = 2;

                
                MotorOut1 = 0;
                MotorOut2 = 0;
                break;
        }

        //diff.h and speed.h init
        //        lib_speed_init();

        SPEED_INIT();
#ifdef DIFF0
        lib_active_diff_init();
        lib_active_diff_set_p(0.1);
        lib_active_diff_set_d(0.1);
#endif
        //开差速
        //diff_on();
        diff_off();
}

float DEFAULT_SPEED = 0;
float STRAIGHT_SPEED = 0;
float CURVE_SPEED = 0;
float PRE_STOP_SPEED = 0;
float RUSH_STOP_SPEED = 0;
float EMERGENCY_STOP_SPEED = 0;
float UP_RAMP_SPEED = 0;
float ON_RAMP_SPEED = 0;
float DOWN_RAMP_SPEED = 0;
int SPEED_MOTOR_SCALE_HIGH = 0;
bool barn_state = 1;

void SPEED_INIT(void)
{
        switch (competition_strategy)
        {
        case 0:
                DEFAULT_SPEED = 3.0;
                STRAIGHT_SPEED = 3.2;
                CURVE_SPEED = 2.8;
                PRE_STOP_SPEED = 2.5;
                RUSH_STOP_SPEED = 2.0;
                EMERGENCY_STOP_SPEED = 1.0;
                UP_RAMP_SPEED = 2.0;
                ON_RAMP_SPEED = 1.0;
                DOWN_RAMP_SPEED = DEFAULT_SPEED;

                SPEED_MOTOR_SCALE_HIGH = 2500;
                break;
        default:
                DEFAULT_SPEED = 2.8;
                STRAIGHT_SPEED = 2.8;
                CURVE_SPEED = 2.5;
                PRE_STOP_SPEED = 2.5;
                RUSH_STOP_SPEED = 2.0;
                EMERGENCY_STOP_SPEED = 1.0;
                UP_RAMP_SPEED = 2.0;
                ON_RAMP_SPEED = 1.0;
                DOWN_RAMP_SPEED = DEFAULT_SPEED;

                SPEED_MOTOR_SCALE_HIGH = 2500;
                break;
        }
}
