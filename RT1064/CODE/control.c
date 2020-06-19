#include "headfile.h"

float Turn_Cam_Out;
float Turn_EM_Out;
float MotorOut1,MotorOut2;
float speedTarget1,speedTarget2;

/*************************************************************************
*  �������ƣ�void Turn_Cam(void)
*  ����˵��������ͷת����Ƴ��򣬸�������ƫ���������������
*  ����˵��������ƫ����,PD 
*  �������أ�������
*  �޸�ʱ�䣺2020.6.20
*  ��    ע��ƫ����offsetΪ��˵�����������������ƫ��
             ƫ����offsetΪ��˵�����������������ƫ��
*************************************************************************/

PID PID_TURN_CAM_EXT;
void Turn_Cam(void)
{
    PID PID_TURN_CAM;
    static float Cam_offset_old = 0;
    PID_TURN_CAM = TurnFuzzyPD_Cam();

    //0.768=0.8*1.2*0.8
    PID_TURN_CAM.P *= PID_TURN_CAM_EXT.P; //0.85;//0.7
    PID_TURN_CAM.D *= PID_TURN_CAM_EXT.D;

    Turn_Cam_Out = PID_TURN_CAM.P * Cam_offset + PID_TURN_CAM.D * (Cam_offset - Cam_offset_old); //ת��PID����

    Cam_offset_old = Cam_offset;
    if (Road == 1 && Road1_flag == 5)
    {
        if (Turn_Cam_Out > -0.5 * SERVO_RANGE)
        {
            Turn_Cam_Out = -0.5 * SERVO_RANGE;
        }
    }
    else if (Road == 2 && Road2_flag == 5)
    {
        if (Turn_Cam_Out < 0.5 * SERVO_RANGE)
        {
            Turn_Cam_Out = 0.5 * SERVO_RANGE;
        }
    }
    Servo_Duty(-Turn_Cam_Out);
}
/*************************************************************************
*  �������ƣ�void TurnFuzzyPD_Cam(void)
*  ����˵����ת��PDģ������------����ͷ����
*  ����˵����PD ��
*  �������أ�����ͷ����ת��PD
*  �޸�ʱ�䣺2020.6.20
*  ��    ע��
*************************************************************************/
PID TurnFuzzyPD_Cam(void)
{
    PID PID_TURN_CAM;
    static const float Cam_Offset_Table0[23] = {-140, -130, -110, -100, -80, -60, -50, -40, -30, -20, 0, 20, 30, 40, 50, 60, 80, 100, 110, 130, 140};
    static const float Turn_Cam_P_Table0[23] = {1.29, 1.20, 1.15, 1.20, 1.20, 1.30, 1.35, 1.60, 1.5, 1.45, 0.3, 1.45, 1.5, 1.60, 1.35, 1.30, 1.20, 1.20, 1.15, 1.20, 1.29};
    static const float Turn_Cam_D_Table0[23] = {1.29, 1.20, 1.15, 1.20, 1.20, 1.30, 1.35, 1.60, 1.5, 1.45, 0.01, 1.45, 1.5, 1.60, 1.35, 1.30, 1.20, 1.20, 1.15, 1.20, 1.29};

    if (Cam_offset <= Cam_Offset_Table0[0])
    {
        PID_TURN_CAM.P = Turn_Cam_P_Table0[0];
        PID_TURN_CAM.D = Turn_Cam_D_Table0[0];
    }
    else if (Cam_offset >= Cam_Offset_Table0[22])
    {
        PID_TURN_CAM.P = Turn_Cam_P_Table0[22];
        PID_TURN_CAM.D = Turn_Cam_D_Table0[22];
    }
    else
    {
        for (int i = 0; i < 22; i++)
        {
            if (Cam_offset >= Cam_Offset_Table0[i] && Cam_offset < Cam_Offset_Table0[i + 1])
            {
                PID_TURN_CAM.P = Turn_Cam_P_Table0[i] + (Cam_offset - Cam_Offset_Table0[i]) * (Turn_Cam_P_Table0[i + 1] - Turn_Cam_P_Table0[i]) / (Cam_Offset_Table0[i + 1] - Cam_Offset_Table0[i]); //����
                PID_TURN_CAM.D = Turn_Cam_D_Table0[i] + (Cam_offset - Cam_Offset_Table0[i]) * (Turn_Cam_D_Table0[i + 1] - Turn_Cam_D_Table0[i]) / (Cam_Offset_Table0[i + 1] - Cam_Offset_Table0[i]);
                break;
            }
        }
    }
    return PID_TURN_CAM;
}

/*************************************************************************
*  �������ƣ�void SpeedTarget_fig(void)
*  ����˵���������ٶ�Ŀ����
*  ����˵����
*  �������أ��ٶ�Ŀ����
*  �޸�ʱ�䣺2020.6.20
*  ��    ע��
*************************************************************************/
void SpeedTarget_fig(void)
{
  float angle_val; // ������ʾʵ��ת��Ƕ�
  float diff_K0;   // ������=���ٱȾ��٣������ָ�һ��

  if (get_diff_state() == DIFF_ON_VAL)
  {
    //���ز�����Para�ж���

    angle_val = Turn_Cam_Out / SERVO_RANGE * ANGLE_RANGE;
    diff_K0 = CAR_DIFF_K * tan(angle_val);
    //�ɴ�PD������
  }
  else if (get_diff_state() == DIFF_OFF_VAL)
  {
    diff_K0 = 0;
  }

  speedTarget1 = get_speed() * (1 + diff_K0 / 2); //��೵��
  speedTarget2 = get_speed() * (1 - diff_K0 / 2); //�Ҳ೵��
//����ɼ������²���
}

/*************************************************************************
*  �������ƣ�void Speed_Control(void)
*  ����˵�����ٶ�PI
*  ����˵����
*  �������أ����ռ�ձ�+������
*  �޸�ʱ�䣺2020.6.20
*  ��    ע��
*************************************************************************/
PID PID_SPEED;
void Speed_Control(void)
{
    int SpeedE1, SpeedE2;
    static int OldE1, OldE2;
    int SpeedEE1,SpeedEE2;
    float Speed_kP1,Speed_kP2,Speed_kI1,Speed_kI2;
    float SpeedControlOutE1,SpeedControlOutE2; 
    SpeedE1 = (int)((speedTarget1 - CarSpeed1) * 100.0);
    SpeedE2 = (int)((speedTarget2 - CarSpeed2) * 100.0);

    //kp
    Speed_kP1 = PID_SPEED.P;
    Speed_kP2 = PID_SPEED.P;
    //ki
    SpeedEE1 = SpeedE1 - OldE1;
    SpeedEE2 = SpeedE2 - OldE2;
////////////////////////////////////////////////////////////////////////////////
    if (abs(SpeedE1) < 15)
        Speed_kI1 = 0;
    else
        Speed_kI1 = PID_SPEED.I;
    if (abs(SpeedE2) < 15)
        Speed_kI2 = 0;
    else
        Speed_kI2 = PID_SPEED.I;
///////////////////////////////////////////////////////////////////////////////
    SpeedControlOutE1 = (Speed_kP1 * SpeedEE1 + Speed_kI1 * SpeedE1);
    SpeedControlOutE2 = (Speed_kP2 * SpeedEE2 + Speed_kI2 * SpeedE2);
    OldE1 = SpeedE1;
    OldE2 = SpeedE2;

    MotorOut1+=SpeedControlOutE1;
    MotorOut2+=SpeedControlOutE2;
    
    Moto_Out(MotorOut1,MotorOut2);// (uint32)(MotorOut1 / 100) * 100Ҫ�ȷ�+-
}