#include "headfile.h"

int Allwhitestart; //ȫ���п�ʼ��
int Allwhiteend;   //ȫ���н�����

int Lef_circle;
int Rig_circle; //���ұ���Ѱ�һ�״���߱�־λ
int Lef_break_point;
int Rig_break_point; //���һ�״���߹����

bool barn_reset_flag; //�ɻɹܼ������������ش���ʱ

int start_stop_line_flag = 0;
int barn_line = Fir_row;
int start_stop_line = 0;
int start_line = 0;
int Road = 0; //��·���ͱ�־λ
int Road0_flag = 0;
int Road1_flag = 0;
int Road2_flag = 0;
int Road7_flag = 0;
int Road3_flag = 0;
int Road4_flag = 0;
bool ganhuangguan_flag = 0;

int turn_stop = 0; //ת����

/*************************************************************************
*  �������ƣ�void Allwhite_find()
*  ����˵����ȫ���в���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2019.3.23
*  ��    ע��Ѱ��ȫ���еĿ�ʼ�кͽ�����

*************************************************************************/

void Allwhite_find(void)
{
    int i;
    int Allwhiterow[LCDH]; //ȫ���У�1��ʾȫ�ף�����Ϊ0
    Allwhitestart = 0;
    Allwhiteend = 0;

    for (i = Fir_row; i < Last_row - 7; i++)
    {
        Allwhiterow[i] = 0;
        if (Road0_flag == 1 && i < 35)
        {
            if (abs(Lef[i] - Fir_col) < 8 && abs(Last_col - Rig[i]) < 8 &&
                Pixle[i][10] + Pixle[i][14] + Pixle[i][18] + Pixle[i][22] > 2 &&
                Pixle[i][70] + Pixle[i][66] + Pixle[i][62] + Pixle[i][58] > 2 &&
                Pixle[i][34] + Pixle[i][38] + Pixle[i][42] + Pixle[i][46] > 2) //Pixle[i][10] + Pixle[i][20] + Pixle[i][30] + Pixle[i][40] + Pixle[i][50] + Pixle[i][60] + Pixle[i][70] > 5)
            {
                Allwhiterow[i] = 1;
            }
        }
        else if (abs(Lef[i] - Fir_col) < 5 && abs(Last_col - Rig[i]) < 5 &&
                 Pixle[i][10] + Pixle[i][14] + Pixle[i][18] + Pixle[i][22] > 2 &&
                 Pixle[i][70] + Pixle[i][66] + Pixle[i][62] + Pixle[i][58] > 2 &&
                 Pixle[i][34] + Pixle[i][38] + Pixle[i][42] + Pixle[i][46] > 2)
        //Pixle[i][10] + Pixle[i][14] + Pixle[i][18] + Pixle[i][22] > 2 && Pixle[i][70] + Pixle[i][66] + Pixle[i][62] + Pixle[i][58] > 2)
        {
            Allwhiterow[i] = 1;
        }
    }
    Allwhitestart = Last_row;
    i = Last_row - 8;
    while (Allwhiterow[i] != 1 && i > Fir_row)
    {
        i = i - 1;
    }
    Allwhitestart = i;
    Allwhiteend = Fir_row;
    while (Allwhiterow[i] == 1 && i > Fir_row)
    {
        i = i - 1;
    }
    Allwhiteend = i;
}

/*************************************************************************
*  �������ƣ�void Pic_find_circle(void)
*  ����˵����ͼ���һ�״
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2019.3.21
*  ��    ע����״���壺���ұ�����������Ч�п�ʼ�ȼ�С����

*************************************************************************/

void Pic_find_circle(void)
{
    int i;
    Lef_circle = 0;
    Rig_circle = 0;
    Lef_break_point = 0;
    Rig_break_point = 0;
    for (i = 55; i > Fir_row + 12; i--) //�ӷ�ȫ���п�ʼѰ��
    {
        if (Last_col - Rig[i] < 2) //���ұ����뿪�ұ߽翪ʼѰ��
        {
            continue;
        }
        /*if(Rig[i-2]<=Rig[i] && Rig[i-4]<=Rig[i-2] && Rig[i-7]>=Rig[i-5] &&
       Rig[i-9]>=Rig[i-7] && Rig[i-11]>=Rig[i-9]&& Rig[i-8]!=Rig[i-2] &&
       abs(Rig[i-8]-Rig[i-6])<5 && abs(Rig[i-6]-Rig[i-4])<5 && abs(Rig[i-4]-Rig[i-2])<5 && abs(Rig[i-2]-Rig[i])<5 && Rig[i-5]<=67)
    {
      Rig_circle=1;
      Rig_break_point=i-5;
      break;
    }
    */

        if (Rig[i - 2] <= Rig[i] && Rig[i - 4] <= Rig[i - 2] && Rig[i - 6] <= Rig[i - 4] && Rig[i - 8] <= Rig[i - 6] && Rig[i - 10] <= Rig[i - 8] &&
            Rig[i - 12] > Rig[i - 10] && Rig[i - 11] > Rig[i - 9] && Rig[i - 13] > Rig[i - 11] && Rig[i - 14] > Rig[i - 12] && Rig[i - 15] > Rig[i - 13] &&
            abs(Rig[i - 10] - Rig[i - 8]) < 4 && abs(Rig[i - 8] - Rig[i - 6]) < 4 && abs(Rig[i - 6] - Rig[i - 4]) < 4 && abs(Rig[i - 4] - Rig[i - 2]) < 4 && abs(Rig[i - 2] - Rig[i]) < 4 &&
            abs(Rig[i - 12] - Rig[i - 10]) < 5 && abs(Rig[i - 11] - Rig[i - 9]) < 5 && abs(Rig[i - 13] > Rig[i - 11]) < 5 && abs(Rig[i - 14] > Rig[i - 12]) < 8 && abs(Rig[i - 15] > Rig[i - 13]) < 8)
        {
            Rig_circle = 1;
            Rig_break_point = i;
            break;
        }
    }
    for (i = 55; i > Fir_row + 12; i--)
    {
        if (Lef[i] - Fir_col < 2)
        {
            continue;
        }
        /*
    if(Lef[i-2]>=New_Lef[i] && Lef[i-4]>=Lef[i-2] && Lef[i-7]>=New_Lef[i-5] && //Lef[i-8]<=Lef[i-6] &&
       Lef[i-9]<=Lef[i-7] && Lef[i-11]<=Lef[i-19] && Lef[i-8]!=Lef[i-2] &&
         abs(Lef[i-8]-Lef[i-6])<5 && abs(Lef[i-6]-Lef[i-4])<5 && abs(Lef[i-4]-Lef[i-2])<5 && abs(Lef[i-2]-New_Lef[i])<5)
    {
      Lef_circle=1;
      Lef_break_point=i-5;
      break;t
    }
    */
        if (Lef[i - 2] >= Lef[i] && Lef[i - 4] >= Lef[i - 2] && Lef[i - 6] >= Lef[i - 4] && Lef[i - 8] >= Lef[i - 6] && Lef[i - 10] >= Lef[i - 8] &&
            Lef[i - 12] < Lef[i - 10] && Lef[i - 11] < Lef[i - 9] && Lef[i - 13] < Lef[i - 11] && Lef[i - 14] < Lef[i - 12] && Lef[i - 15] < Lef[i - 13] &&
            abs(Lef[i - 10] - Lef[i - 8]) < 4 && abs(Lef[i - 8] - Lef[i - 6]) < 4 && abs(Lef[i - 6] - Lef[i - 4]) < 4 && abs(Lef[i - 4] - Lef[i - 2]) < 4 && abs(Lef[i - 2] - Lef[i]) < 4 &&
            abs(Lef[i - 12] - Lef[i - 10]) < 5 && abs(Lef[i - 11] - Lef[i - 9]) < 5 && abs(Lef[i - 13] - Lef[i - 11]) < 5 && abs(Lef[i - 14] - Lef[i - 12]) < 8 && abs(Lef[i - 15] - Lef[i - 13]) < 8)
        {
            Lef_circle = 1;
            Lef_break_point = i;
            break;
        }
    }
}

/*************************************************************************
*  �������ƣ�void start_stop_find(void)
*  ����˵����������ʶ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020.06.17
*  ��    ע��
*************************************************************************/
void start_stop_find(void)
{
    start_stop_line_flag = 0;

    int tiaobian1 = 0;

    int Rig_end = 0, Lef_end = 0, Road_end = 0;

    // if (Road == 0 && Road0_flag == 0)
    // {
    if (barn_reset_flag == 0)
    {
        return;
    }
    for (int i = Fir_row + 3; i < Last_row - 4; ++i)
    {
        if (Rig[i] >= Last_col)
        {
            continue;
        }
        if (Rig[i] > 40 && Rig[i + 2] - Rig[i] < 5 && Rig[i + 3] - Rig[i + 1] < 5 && Rig[i + 4] - Rig[i + 2] < 5 &&
            Rig[i + 2] - Rig[i] >= 0 && Rig[i + 3] - Rig[i + 1] >= 0 && Rig[i + 4] - Rig[i + 2] >= 0)
        {
            Rig_end = i;
            break;
        }
    }
    for (int i = Fir_row + 3; i < Last_row - 4; ++i)
    {
        if (Lef[i] <= Fir_col)
        {
            continue;
        }
        if (Lef[i] < 40 && Lef[i] - Lef[i + 2] < 5 && Lef[i + 1] - Lef[i + 3] < 5 && Lef[i + 2] - Lef[i + 4] < 5 &&
            Lef[i] - Lef[i + 2] >= 0 && Lef[i + 1] - Lef[i + 3] >= 0 && Lef[i + 2] - Lef[i + 4] >= 0)
        {
            Lef_end = i;
            break;
        }
    }
    Road_end = (Lef_end < Rig_end ? Rig_end : Lef_end);
    if (Road_end > 30)
    {
        return;
    }

    int jump_p_count = 0;
    for (int i = Fir_row; i < 45; ++i)
    {
        if (jump_p[i] >= 5)
        {
            jump_p_count++;
            if (jump_p_count >= 3)
            {
                start_stop_line_flag = 1;
                start_stop_line = i - 3;
                return;
            }
        }
        else
        {
            jump_p_count = 0;
        }
    }

#ifdef TL2barn
    for (int i = Rig_end; i < Last_row - 7; ++i)
    {
        if (Rig[i] > 40 && Rig[i + 2] - Rig[i] < 5 && Rig[i + 3] - Rig[i + 1] < 5 &&
            Rig[i + 2] - Rig[i] > 0 && Rig[i + 3] - Rig[i + 1] > 0 &&
            Rig[i + 3] - Rig[i + 7] > 25)
        {
            tiaobian1 = i;
            break;
        }
        // if (Rig[i] > 40 && Rig[i + 2] - Rig[i] < 5 && Rig[i + 3] - Rig[i + 1] < 5 && Rig[i + 4] - Rig[i + 2] < 5 &&
        //     Rig[i + 2] - Rig[i] > 0 && Rig[i + 3] - Rig[i + 1] > 0 && Rig[i + 4] - Rig[i + 2] > 0)
        // {
        //     continue;
        // }
        // if (Rig[i + 3] - Rig[i + 7] > 25)
        // {
        //     tiaobian1 = i;
        //     break;
        // }
    }
    if (tiaobian1 != 0)
    {
        for (int i = tiaobian1; i < Last_row - 4; ++i)
        {
            // if (Lef[i] - Fir_co > 5)
            // {
            //     return;
            // }
            if (Rig[i] > 40 && Rig[i] - Rig[i - 3] > 25 && Rig[i + 2] - Rig[i] < 5 && Rig[i + 3] - Rig[i + 1] < 5 && Rig[i + 4] - Rig[i + 2] < 5)
            {
                start_stop_line_flag = 1;
                start_stop_line = tiaobian1;
                return;
            }
            else
            {
                continue;
            }
        }
    }
#endif
#ifdef TR2barn
    for (int i = Lef_end; i < Last_row - 7; ++i)
    {
        if (Lef[i] < 40 && Lef[i] - Lef[i + 2] < 5 && Lef[i + 1] - Lef[i + 3] < 5 &&
            Lef[i] - Lef[i + 2] > 0 && Lef[i + 1] - Lef[i + 3] > 0 &&
            Lef[i + 7] - Lef[i + 3] > 25)
        {
            tiaobian1 = i;
            break;
        }

        // if (Lef[i] < 40 && Lef[i] - Lef[i + 2] < 5 && Lef[i + 1] - Lef[i + 3] < 5 && Lef[i + 2] - Lef[i + 4] < 5 &&
        //     Lef[i] - Lef[i + 2] > 0 && Lef[i + 1] - Lef[i + 3] > 0 && Lef[i + 2] - Lef[i + 4] > 0)
        // {
        //     continue;
        // }
        // if (Lef[i + 7] - Lef[i + 3] > 25)
        // {
        // }
    }
    if (tiaobian1 != 0)
    {
        for (int i = tiaobian1; i < Last_row - 4; ++i)
        {
            // if (Last_col - Rig[i] > 5)
            // {
            //     return;
            // }
            if (Lef[i] < 40 && Lef[i - 3] - Lef[i] > 25 && Lef[i] - Lef[i + 2] < 5 && Lef[i + 1] - Lef[i + 3] < 5 && Lef[i + 2] - Lef[i + 4] < 5)
            {
                start_stop_line_flag = 1;
                start_stop_line = tiaobian1;
                return;
            }
            else
            {
                continue;
            }
        }
    }
#endif

    // }
}

/*************************************************************************
*  �������ƣ�void mag_find(void)
*  ����˵�����ɻɹ�ʶ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020.06.26
*  ��    ע��Road=0��ʾ����·������û�н���Բ�����£�
             3.23:Road=1��ʾ��Բ����Road=2��ʾ��Բ��

*************************************************************************/
void mag_find(void)
{
    static int start_waited = 0;
    if (barn_reset_flag == 0)
    {
        start_waited++;
    }
    if (start_waited >= 600)
    {
        gpio_interrupt_init(C25, FALLING, GPIO_INT_CONFIG);
        start_waited = 0;
        barn_reset_flag = 1;
    }
    if (ganhuangguan_flag == 1)
    {
        if (barn_reset_flag == 0)
        {
            ganhuangguan_flag = 0;
            Road = 3;
            Road3_flag = 1;
        }
        else
        {
            ganhuangguan_flag = 0;
            Road = 7;
            Road7_flag = 2;
        }
    }

    return;
}
// char Road1_turnout = 1;
void Road_rec(void)
{
    static int Road0_count = 0;
    static int Road00_count = 0;
    static int Road10_count = 0, Road20_count = 0, Road70_count = 0, Road40_count = 0;

    //��ֱ·
    if (Road != 0)
    {
        if (Lef_slope == 998 && Rig_slope == 998 && Road7_flag != 2 &&
            ((Lef_edge < 10 && Rig_edge < 10) || (Lef_edge < 2 && Rig_edge < 15) || (Rig_edge < 2 && Lef_edge < 15)))
        {
            Road0_count++;
            if (Road0_count >= 5)
            {
                Road = 0;
                Road0_flag = 0;
            }
        }
        else
        {
            Road0_count = 0;
        }
    }
    //������
    if (Road == 0 && Road0_flag == 0)
    {
        if (start_stop_line_flag == 1)
        {
            Road70_count++;
            if (Road70_count > 2)
            {
                Road70_count = 0;
                Road = 7;
                Road7_flag = 0;
                for (int i = ((start_stop_line - 10) > Fir_row ? (start_stop_line - 10) : Fir_row); i < Last_row - 2; ++i)
                {
                    if (Lef[i - 4] - Lef[i - 2] < 5 && Lef[i - 3] - Lef[i - 1] < 5 && Lef[i - 2] - Lef[i] < 5)
                    {
                        continue;
                    }
                    if ((Lef[i] - Fir_col < 5 && Lef[i + 1] - Fir_col < 5 && Lef[i + 2] - Fir_col < 5) || Lef[i + 2] - Lef[i - 1] > 15)
                    {
                        barn_line = i - 1;
                        break;
                    }
                    barn_line = start_stop_line - 5;
                }
                barn_line = (barn_line > (Fir_row + 5) ? barn_line : (Fir_row + 5));
                return;
            }
        }
        else
        {
            Road70_count = 0;
        }
    }

    if (Road == 0)
    {
        if (icm_gyro_y_w < -30*(CarSpeed1+CarSpeed2) && icm_gyro_y_w < -30)
        {
            Road40_count++;
            if (Road40_count > 1)
            {
                Road = 4;
                Road4_flag = 0;
                return;
            }
        }
        else
        {
            Road40_count = 0;
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////��ͨ������Բ��
        if (Rig_circle == 0 && Lef_circle == 1 && Lef_slope != 998 &&
            Rig[39] - Rig[37] < 5 && Rig[37] - Rig[35] < 5 && Rig[35] - Rig[33] < 5 && Rig[33] - Rig[31] < 5 && Rig[31] - Rig[29] < 5 && Rig[29] - Rig[27] < 5 && Rig[27] - Rig[25] < 5 && Rig[25] - Rig[23] < 5 && Lef_break_point < 45 && Rig_slope >= 0 && EM_Value_2 + EM_Value_3 > 4.2)
        //  (New_Lef[54] == -MIDMAP || New_Lef[55] == -MIDMAP || New_Lef[56] == -MIDMAP)&&// && Rig_edge <= 20)
        //��Բ���������,�ұ��ߣ�ֱͨ����//&& Rig[11] != 78
        {
            Road10_count++;
            if (Road10_count == 2)
            {
                Road = 1;
                Road1_flag = 1;
                Road10_count = 0;
            }
            return;
        }
        else
        {
            Road10_count = 0;
        }
        if (Lef_circle == 0 && Rig_circle == 1 && Rig_slope != 998 &&
            Lef[25] - Lef[27] < 5 && Lef[27] - Lef[29] < 5 && Lef[29] - Lef[31] < 5 && Lef[31] - Lef[33] < 5 && Lef[33] - Lef[35] < 5 && Lef[35] - Lef[37] < 5 && Lef[37] - Lef[39] < 5 && Lef[23] - Lef[25] < 5 && Rig_break_point < 45 && (Lef_slope <= 0 || Lef_slope == 998) && EM_Value_2 + EM_Value_3 > 4.2)
        //     (New_Rig[54] == MIDMAP || New_Rig[55] == MIDMAP || New_Rig[56] == MIDMAP) &&// && Lef_edge <= 20)
        //��Բ�����ұ��ߣ�ͻ�����յ��ͻ���//&& Lef[11] != 2
        {
            Road20_count++;
            if (Road20_count == 2)
            {
                Road = 2;
                Road2_flag = 1;
                Road20_count = 0;
            }
            return;
        }
        else
        {
            Road20_count = 0;
        }

        //���״̬��
        // else if (((Rig_slope > -0.5 && Rig_slope != 998 && Rig_slope != 999) || Road0_flag == 4) && Rig_slope != 998) //��ת��//(Lef_break_point > 35 && Lef_circle == 1 && Rig_circle == 0)
        if ((Rig_slope > -0.5 && Rig_slope < 0.05) || Road0_flag == 4)
        {

            TurnLeft_Process();
            if (Road0_flag == 4)
            {
                return;
            }
        }
        // else if ((Lef_slope < 0.5 || Road0_flag == 5) && Lef_slope != 998) //��ת��//(Rig_break_point > 35 && Rig_circle == 1 && Lef_circle == 0)
        else if ((Lef_slope < 0.5 && Lef_slope > -0.05) || Road0_flag == 5)
        {
            TurnRight_Process();
            if (Road0_flag == 5)
            {
                return;
            }
        }
        //ʮ��״̬��
        if (whitecnt > 1700 && ((Lef_edge > 4 && Rig_edge > 4) || Lef_edge > 12 || Rig_edge > 12) && Allwhitestart <= 45 && Allwhitestart > (Fir_row + 5) && Allwhitestart - Allwhiteend > 3)
        {
            Road0_flag = 1;
        }
        else if ((Allwhitestart > 45 && Allwhiteend < 52) && Allwhitestart - Allwhiteend > 3 && whitecnt > 2000)
        {
            Road0_flag = 2;
        }

        else
        {
            Road00_count++;
            if (Road00_count == 2)
            {
                Road00_count = 0;
                Road0_flag = 0;
            }
        }
    }
    //��Բ��״̬��
    else if (Road == 1)
    {
        Road0_flag = 0;
        Road1_zhuangtaiji();
    }
    //��Բ��״̬��
    else if (Road == 2)
    {
        Road0_flag = 0;
        Road2_zhuangtaiji();
    }
    //���״̬��
    else if (Road == 3)
    {
        Road0_flag = 0;
        Road3_zhuangtaiji();
    }
    else if(Road == 4)
    {
        Road0_flag = 0;
        Road4_zhuangtaiji();
    }
    else if (Road == 7)
    {
        Road0_flag = 0;
        Road7_zhuangtaiji();
    }
}

/*************************************************************************
*  �������ƣ�void TurnLeft_Process(void)
*  ����˵������ת����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020.06.17
*  ��    ע��
*************************************************************************/
void TurnLeft_Process(void)
{
    static int Road04_count = 0, Road00_count = 0; //turn_stop_flag = 0,
    int temp = 40;
    int dis = 0, dis1 = 0;
    for (int i = Fir_row; i < 40; ++i)
    {
        if (Rig[i] < 40 && Rig[i + 1] <= 40 && Rig[i + 2] >= 40 && Rig[i + 3] > 40 &&
            Rig[i + 5] - Rig[i + 3] < 7 && Rig[i + 7] - Rig[i + 5] < 7 && Rig[i + 9] - Rig[i + 7] < 7 && Rig[i + 11] - Rig[i + 9] < 7 &&
            Rig[i + 2] - Rig[i] < 10 &&
            Rig[i + 5] - Rig[i + 3] > 0 && Rig[i + 7] - Rig[i + 5] > 0 && Rig[i + 9] - Rig[i + 7] > 0 && Rig[i + 11] - Rig[i + 9] > 0 &&
            Rig[i + 2] - Rig[i] > 0)
        //���ܽ��ϣ���������������Ϊ40��
        {
            temp = i;
            break;
        }
    }

    if (temp == 40) //&& turn_stop_flag == 1)
    {
        Road00_count++;
        if (Road00_count == 2)
        {
            Road00_count = 0;
            Road0_flag = 0;
            // turn_stop_flag = 0;
            return;
        }
    }
    else
    {
        Road00_count = 0;
    }
    dis = Rig[temp + 1] - Rig[temp];
    for (int i = temp; i > Fir_row; --i)
    {
        if (Rig[i - 1] > 40)
        {
            turn_stop = i + 1;
            break;
        }
        dis1 = Rig[i] - Rig[i - 1];
        if (dis1 < 0)
        {
            turn_stop = i;
            break;
        }
        else if (dis1 < dis)
        {
            continue;
        }
        else if (dis1 <= 2 * dis + 1)
        {
            dis = dis1;
            continue;
        }
        else
        {
            turn_stop = i;
            break;
        }
    }

    // if (turn_stop < 28)
    // {
    //     turn_stop_flag = 1;
    // }
    if (Road0_flag != 4 && Rig[turn_stop] < 34 && dis > 4)
    {
        Road04_count++;
        if (Road04_count == 2)
        {
            Road = 0;
            Road0_flag = 4; //turn left flag
            threshold_offset2 = -5;
            Road04_count = 0; //reset
        }
    }
    else
    {
        Road04_count = 0;
    }

    return;
}

/*************************************************************************
*  �������ƣ�void TurnRight_Process(void)
*  ����˵������ת����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020.06.17
*  ��    ע��
*************************************************************************/
void TurnRight_Process(void)
{
    static int Road05_count = 0, Road00_count = 0; //turn_stop_flag = 0,
    int temp = 40;
    int dis = 0, dis1 = 0;
    for (int i = Fir_row; i < 40; ++i)
    {
        if (Lef[i] > 40 && Lef[i + 1] >= 40 && Lef[i + 2] <= 40 && Lef[i + 3] < 40 &&
            Lef[i + 3] - Lef[i + 5] < 7 && Lef[i + 5] - Lef[i + 7] < 7 && Lef[i + 7] - Lef[i + 9] < 7 && Lef[i + 9] - Lef[i + 11] < 7 &&
            Lef[i] - Lef[i + 2] < 10 &&
            Lef[i + 3] - Lef[i + 5] > 0 && Lef[i + 5] - Lef[i + 7] > 0 && Lef[i + 7] - Lef[i + 9] > 0 && Lef[i + 9] - Lef[i + 11] > 0 &&
            Lef[i] - Lef[i + 2] > 0)
        {
            temp = i;
            break;
        }
    }
    if (temp == 40) // && turn_stop_flag == 1)
    {
        Road00_count++;
        if (Road00_count >= 3)
        {
            Road00_count = 0;
            Road0_flag = 0;
            // turn_stop_flag = 0;
            return;
        }
    }
    else
    {
        Road00_count = 0;
    }
    dis = Lef[temp] - Lef[temp + 1];
    for (int i = temp; i > Fir_row; --i)
    {
        if (Lef[i - 1] < 40)
        {
            turn_stop = i + 4;
            break;
        }
        dis1 = Lef[i - 1] - Lef[i];
        if (dis1 < 0)
        {
            turn_stop = i;
            break;
        }
        else if (dis1 < dis)
        {
            continue;
        }
        else if (dis1 <= 2 * dis + 1)
        {
            dis = dis1;
            continue;
        }
        else
        {
            turn_stop = i;
            break;
        }
    }
    // if (turn_stop < 28)
    // {
    //     turn_stop_flag = 1;
    // }
    if (Road0_flag != 5 && Lef[turn_stop] > 45 && dis > 4)
    {
        Road05_count++;
        if (Road05_count == 2)
        {
            Road = 0;
            Road0_flag = 5; //turn left flag
            threshold_offset2 = -5;
            Road05_count = 0; //reset
        }
    }
    else
    {
        Road05_count = 0;
    }
    return;
}

/*************************************************************************
*  �������ƣ�void Road1_zhuangtaiji(void)
*  ����˵������Բ��״̬��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020.06.17
*  ��    ע��
*************************************************************************/

void Road1_zhuangtaiji(void)
{
    static int Road11_count = 0, Road12_count = 0, Road13_count = 0, Road14_count = 0, Road15_count = 0, Road16_count = 0, Road17_count = 0;
    int dis = 0, dis1 = 0;
    if (Road1_flag == 0) //׼������Բ��
    {
        Road0_flag = 0;
        if (EM_Value_2 + EM_Value_3 > 4.3) //����ʶ���������߽���һ�߷�������
        {
            Road11_count++;
            if (Road11_count > 1)
            {
                Road11_count = 0;
                Road1_flag = 1; //��ʾ�Ѿ�������Բ��
            }
        }
        else
        {
            Road11_count = 0;
        }
    }
    else if (Road1_flag == 1) //����Բ��1/4
    {
        if (Lef_circle_point != 0)
        {
            Road12_count++;
            if (Road12_count > 1)
            {
                Road1_flag = 2;
            }
        }
        else
        {
            Road12_count = 0;
        }
        // for (int i = 35; i > Fir_row; i--)
        // {
        //     if (Last_col - Rig[i] < 2 || Last_col - Rig[i - 1] < 2)
        //     {
        //         continue;
        //     }
        //     if (i < 28)
        //     {
        //         Road12_count++;
        //         if (Road12_count > 1) //2֡�� ����Բ����һ���
        //         {
        //             Road1_flag = 2;
        //             Road12_count = 0;
        //         }
        //     }
        //     else
        //     {
        //         Road12_count = 0;
        //     }
        //     break;
        // }
        // if (Lef_circle == 0 || (Lef_circle == 1 && Lef_break_point > 45)) //if(((Lef_circle==0||( Lef_circle ==1 && Lef_break_point>30)))&& Road1_turnin(EM_Value_2,EM_Value_3,3.8))//if(((Lef_circle==0||( Lef_circle ==1 && Lef_break_point>30))) && Rig_slope>=10)/ && Road1_turnin(EM_Value_2,EM_Value_3,3.8))    //Rig_slope<1 && (Lef_leap[0]==0||Lef_slope==999)&& Rig_leap[0]==0)
        // {
        //     Road12_count++;
        //     if (Road12_count > 1) //2֡�� ����Բ����һ���
        //     {
        //         Road1_flag = 2;
        //         Road12_count = 0;
        //     }
        // }
        // else
        // {
        //     Road12_count = 0;
        // }
    }
    else if (Road1_flag == 2) //����Բ��2/4 ��ʼ���߽����
    {
        // Road14_count++;
        for (int i = Last_row; i > Fir_row; i--)
        {
            if (Last_col - Rig[i] < 2 || Last_col - Rig[i - 1] < 2)
            {
                continue;
            }
            if (i > 50 && Rig[i] - Rig[i - 2] < 8 && Rig[i - 2] - Rig[i - 4] < 8 && Rig[i] - Rig[i - 2] > 0 && Rig[i - 2] - Rig[i - 4] > 0)
            {
                Road14_count++;
                //
                if (Road14_count > 1)
                {
                    Road1_flag = 4;
                    Road14_count = 0;
                    break;
                }
            }
            else
            {
                Road14_count = 0;
                break;
            }
        }
        // if (Road14_count > (int)(DIS_IN_CIRCLE * 10000 / (SpeedGoal * CAMERA_FPS)) + 1) //�궨����function.h
        // {
        //     Road1_flag = 4;
        //     Road14_count = 0;
        // }
    }
    else if (Road1_flag == 4) //����Բ���� ��ȡ������
    {
        dis = Rig[40 + 1] - Rig[40];
        for (int i = 40; i > Fir_row; --i)
        {
            dis1 = Rig[i] - Rig[i - 1];
            if (dis1 < 0)
            {
                turn_stop = i;
                break;
            }
            else if (dis1 < dis)
            {
                continue;
            }
            else if (dis1 <= 2 * dis + 1)
            {
                dis = dis1;
                continue;
            }
            else
            {
                turn_stop = i;
                break;
            }
        }
        if (Allwhitestart > 29 && Allwhiteend < 30) //Rig_circlr ���ã���!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        {
            Road13_count++;
            if (Road13_count > 2)
            {
                Road1_flag = 3;
                Road13_count = 0;
            }
        }
        else
        {
            Road13_count = 0;
        }
        if (Allwhitestart >= 42 && Allwhiteend < 30)
        {
            Road15_count++;
            if (Road15_count > 1)
            {
                Road15_count = 0;
                Road1_flag = 5;
            }
        }
        else
        {
            Road15_count = 0;
        }
    }
    else if (Road1_flag == 3) //׼����Բ�����ұ��߲���
    {
        if (Allwhitestart >= 42)
        {
            Road15_count++;
            if (Road15_count > 1)
            {
                Road15_count = 0;
                Road1_flag = 5;
            }
        }
        else
        {
            Road15_count = 0;
        }
    }
    else if (Road1_flag == 5) //�ұ����Ѿ����ܲ��ߣ���ŵȷ�����
    {
        // if ((Rig_slope > -0.02 && Rig_slope < 0) || (Pixle[58][74] == 1 && Pixle[57][74] == 1 && Pixle[56][74] == 1 && Pixle[55][74] == 1 && Pixle[54][74] == 1 && Pixle[53][74] == 1)) //|| Lef_edge < 20))
        if ((Rig_slope < -0.1 || Rig_slope == 998) && (Allwhiteend > 45 || Allwhiteend == Fir_row))
        {
            Road16_count++;
            if (Road16_count > 3)
            {
                Road16_count = 0;
                // Road = 0;
                // Road0_flag = 0;
                Road1_flag = 6;
            }
        }
        else
        {
            Road16_count = 0;
        }
    }
    else if (Road1_flag == 6)
    {
        if (EM_Value_1 < 0.6 && EM_Value_4 < 0.6)
        {
            Road17_count++;
            if (Road17_count > 1)
            {
                Road = 0;
                Road0_flag = 0;
            }
        }
    }
    return;
}

/*************************************************************************
*  �������ƣ�void Road1_zhuangtaiji(void)
*  ����˵������Բ��״̬��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020.06.17
*  ��    ע��
*************************************************************************/

void Road2_zhuangtaiji(void)
{
    static int Road21_count = 0, Road22_count = 0, Road23_count = 0, Road24_count = 0, Road25_count = 0, Road26_count = 0, Road27_count = 0;
    int dis = 0, dis1 = 0;
    if (Road2_flag == 0) //
    {
        if (EM_Value_2 + EM_Value_3 > 4.3) //����ʶ���������߽���һ�߷�������
        {
            Road21_count++;
            if (Road21_count > 1)
            {
                Road21_count = 0;
                Road2_flag = 1; //��ʾ�Ѿ�������Բ��
            }
        }
        else
        {
            Road21_count = 0;
        }
    }
    else if (Road2_flag == 1) //
    {
        if (Rig_circle_point != 0)
        {
            Road22_count++;
            if (Road22_count > 1)
            {
                Road2_flag = 2;
            }
        }
        else
        {
            Road22_count = 0;
        }
        // for (int i = 25; i > Fir_row; i--)
        // {
        //     if (Lef[i] - Fir_col < 2 || Lef[i - 1] - Fir_col < 2)
        //     {
        //         continue;
        //     }
        //     if (i < 28)
        //     {
        //         Road22_count++;
        //         if (Road22_count > 1) //2֡�� ����Բ����һ���
        //         {
        //             Road2_flag = 2;
        //             Road22_count = 0;
        //         }
        //     }
        //     else
        //     {
        //         Road22_count = 0;
        //     }
        //     break;
        // }

        // if ((Rig_circle == 0 || (Rig_circle == 1 && Rig_break_point > 45)) && Rig_slope >= 10) //if(((Rig_circle==0||( Rig_circle ==1 && Rig_break_point>30))) && Rig_slope>=10)/ && Road1_turnin(EM_Value_2,EM_Value_3,3.4))
        // {
        //     Road22_count++;
        //     if (Road22_count > 1) //
        //     {
        //         Road2_flag = 2;
        //         Road22_count = 0;
        //     }
        // }
        // else
        // {
        //     Road22_count = 0;
        // }
    }
    else if (Road2_flag == 2) //
    {
        // Road24_count++;
        for (int i = Last_row; i > Fir_row; i--)
        {
            if (Lef[i] - Fir_col < 2 || Lef[i - 1] - Fir_col < 2)
            {
                continue;
            }
            if (i > 50 && Lef[i - 2] - Lef[i] < 8 && Lef[i - 4] - Lef[i - 2] < 8 && Lef[i - 2] - Lef[i] > 0 && Lef[i - 4] - Lef[i - 2] > 0)
            {
                Road24_count++;
                if (Road24_count > 1)
                {
                    Road24_count = 0;
                    Road2_flag = 4;
                    break;
                }
            }
            else
            {
                Road24_count = 0;
                break;
            }
        }

        // if (Road24_count == (int)(DIS_IN_CIRCLE * 10000 / (SpeedGoal * CAMERA_FPS)) + 1)
        // {
        //     Road24_count = 0;
        //     Road2_flag = 4;
        // }
    }
    else if (Road2_flag == 4)
    {
        dis = Lef[40] - Lef[40 + 1];
        for (int i = 40; i > Fir_row; --i)
        {
            dis1 = Lef[i - 1] - Lef[i];
            if (dis1 < 0)
            {
                turn_stop = i;
                break;
            }
            else if (dis1 < dis)
            {
                continue;
            }
            else if (dis1 <= 2 * dis + 1)
            {
                dis = dis1;
                continue;
            }
            else
            {
                turn_stop = i;
                break;
            }
        }

        if (Allwhitestart > 29 && Allwhiteend < 30)
        {
            Road23_count++;
            if (Road23_count > 2)
            {
                Road23_count = 0;
                Road2_flag = 3;
            }
        }
        else
        {
            Road23_count = 0;
        }
        if (Allwhitestart >= 42 && Allwhiteend < 30)
        {
            Road25_count++;
            if (Road25_count > 1)
            {
                Road25_count = 0;
                Road2_flag = 5;
            }
        }
        else
        {
            Road25_count = 0;
        }
    }
    else if (Road2_flag == 3)
    {
        if (Allwhitestart >= 42)
        {
            Road25_count++;
            if (Road25_count > 1)
            {
                Road25_count = 0;
                Road2_flag = 5;
            }
        }
        else
        {
            Road25_count = 0;
        }
    }
    else if (Road2_flag == 5)
    {
        // Road0_flag = 0;
        // if ((Lef_slope > 0 && Lef_slope < 0.02) || (Pixle[58][5] == 1 && Pixle[57][5] == 1 && Pixle[56][5] == 1 && Pixle[55][5] == 1 && Pixle[54][5] == 1 && Pixle[53][5] == 1)) //|| Lef_edge < 20))
        if (Lef_slope > 0.1 && (Allwhiteend > 45 || Allwhiteend == Fir_row))
        {
            Road26_count++;
            if (Road26_count > 3)
            {
                Road26_count = 0;
                // Road = 0;

                Road2_flag = 6;
            }
        }
        else
        {
            Road26_count = 0;
        }
    }
    else if (Road2_flag == 6)
    {
        if (EM_Value_1 < 0.6 && EM_Value_4 < 0.6)
        {
            Road27_count++;
            if (Road27_count > 1)
            {
                Road = 0;
                Road0_flag = 0;
            }
        }
    }
    return;
}

/*************************************************************************
*  �������ƣ�void Road3_zhuangtaiji()
*  ����˵��������
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2019.3.23
*  ��    ע��Road=3��ʾ����
            Road3_flag=0׼�����⣬=1������

*************************************************************************/

void Road3_zhuangtaiji(void)
{

    static int start_count = 0;
    static int Road31_count = 0;
    static int Road32_count = 0;

#ifdef TL2barn

    if (Road3_flag == 0)
    {
        for (int i = Last_row - 3; i > Fir_row + 1; --i)
        {
            if (abs(Lef[i - 1] - Lef[i]) < 10)
            {
                continue;
            }
            else
            {
                start_line = i;
                break;
            }
        }
        if (start_line > 38)
        {
            Road31_count++;
            if (Road31_count > 1)
            {
                Road31_count = 0;
                Road3_flag = 1;
            }
        }
        else
        {
            Road31_count = 0;
        }
    }
    else if (Road3_flag == 1)
    {
        if (Rig_slope < -0.25)
        {
            Road32_count++;
            if (Road32_count > 2)
            {
                Road = 0;
                Road0_flag = 0;
                Road32_count = 0;
            }
        }
        else
        {
            Road32_count = 0;
        }
    }

#endif
#ifdef TR2barn
    if (Road3_flag == 0)
    {
        for (int i = Last_row - 3; i > Fir_row + 1; --i)
        {
            if (abs(Rig[i - 1] - Rig[i]) < 10)
            {
                continue;
            }
            else
            {
                start_line = i;
                break;
            }
        }
        if (start_line > 38)
        {
            Road31_count++;
            if (Road31_count > 1)
            {
                Road31_count = 0;
                Road3_flag = 0;
            }
        }
        else
        {
            Road31_count = 0;
        }
    }
    else if (Road3_flag == 1)
    {
        if (Lef_slope > 0.25 && Lef_slope != 999)
        {
            Road32_count++;
            if (Road32_count > 2)
            {
                Road32_count = 0;
            }
            Road = 0;
            Road0_flag = 0
        }
        else
        {
            Road32_count = 0;
        }
    }

#endif
}

/*************************************************************************
*  �������ƣ�void Road4_zhuangtaiji(void)
*  ����˵�����µ�״̬��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020.07.02
*  ��    ע��
*************************************************************************/
void Road4_zhuangtaiji(void)
{
    if (Road4_flag == 0)
    {
        if (icm_gyro_y_angle < -8)
        {
            Road4_flag = 1;
        }
    }
    else if (Road4_flag == 1)
    {
        if (icm_gyro_y_angle > -6)
        {
            Road4_flag = 2;
        }
    }
    else if (Road4_flag == 2)
    {
        if (icm_gyro_y_angle > 10)
        {
            Road4_flag = 3;
        }
    }
    else if (Road4_flag == 3)
    {
        if (icm_gyro_y_angle < 2)
        {
            Road = 0;
            Road0_flag = 0;
            icm_gyro_y_angle = 0;
        }
    }
}

/*************************************************************************
*  �������ƣ�void Road7_zhuangtaiji(void)
*  ����˵����������״̬��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020.06.17
*  ��    ע��
*************************************************************************/
void Road7_zhuangtaiji(void)
{
    static int Road73_count = 0;
    static int Road74_count = 0;
    static int stop_line = Fir_row;
    int Black_line = 0;
    if (Road7_flag == 0 || Road7_flag == 1) //�ȴ�ת��
    {
        for (int i = barn_line - 5; i < Last_row; i++)
        {
#ifdef TL2barn
            if (Lef[i] - Fir_col < 5 && Lef[i + 1] - Fir_col < 5 && Lef[i + 2] - Fir_col < 5)
#endif
#ifdef TR2barn
                if (Last_col - Rig[i] < 5 && Last_col - Rig[i + 1] < 5 && Last_col - Rig[i + 2] < 5)
#endif
                {
                    barn_line = ((i - 2) > Fir_row + 5 ? (i - 2) : Fir_row + 5);
                    break;
                }
        }
        if (barn_line < 26)
        {
            Road7_flag = 0; //�ȴ�ת�䣬Զ���в���
        }
        else if (barn_line < 28)
        {
            Road7_flag = 1;
        }
        else
        {
            Road = 7;
            Road7_flag = 2; //��ʼת��
        }
    }
    else if (Road7_flag == 2) //��ʼת��
    {
        //Road73_count++;
        // if (Road73_count >= (int)(10 * 100 / (get_speed() * CAMERA_FPS)) + 1)
        // {
        // Road7_flag = 3;
        // }
        if (Lef_slope == 998 && Rig_slope == 998 && (Lef_edge < 7 && Rig_edge < 7) || (EM_Value_1 < 0.2 && EM_Value_2 < 0.15 && EM_Value_2 < 0.15 && EM_Value_4 < 0.2))
        {
            Road73_count++;
            if (Road73_count > 1)
            {
                Road73_count = 0;
                Road7_flag = 3;
            }
        }
    }
    else if (Road7_flag == 3) //ת�����ͣ��
    {
        Road73_count = 0;
        // Road74_count++;
        for (int i = Last_row; i > stop_line - 5; i--)
        {
            if (Pixle[i][8] + Pixle[i][72] + Pixle[i][30] + Pixle[i][35] + Pixle[i][40] + Pixle[i][45] + Pixle[i][50] < 3)
            {
                Black_line++;
                if (Black_line > 2)
                {
                    stop_line = i - 2;
                    if (stop_line > 30) // || Road74_count >= (int)(30 * 100 / (get_speed() * CAMERA_FPS)) + 1)
                    {
                        Road74_count++;
                        if (Road74_count > 1)
                        {
                            Road7_flag = 4; //ͣ��
                            Road74_count = 0;
                            lib_speed_set(0);
                            stop_line = Fir_row;
                        }
                    }
                    break;
                }
            }
            else
            {
                Black_line = 0;
            }
        }
    }
    return;
}
