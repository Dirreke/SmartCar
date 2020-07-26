#include "headfile.h"

int Allwhitestart; //ȫ���п�ʼ��
int Allwhiteend;   //ȫ���н�����

int Lef_circle;
int Rig_circle; //���ұ���Ѱ�һ�״���߱�־λ
int Lef_break_point;
int Rig_break_point; //���һ�״���߹����
bool Lef_circle_pre_flag = 0;
bool Rig_circle_pre_flag = 0;
// int Lef_break_point_und = 0;
// int Rig_break_point_und = 0;

bool barn_reset_flag = 0; //�ɻɹܼ������������ش���ʱ
// bool ramp_reset_flag = 1;
bool crossing_flag;

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

int stop_line = Fir_row;

volatile int turn_stop = 0; //ת����

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
    bool Allwhiterow[LCDH]; //ȫ���У�1��ʾȫ�ף�����Ϊ0
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
    // Allwhitestart = Last_row;
    // i = Last_row - 8;
    // while (Allwhiterow[i] != 1 && i > Fir_row)
    // {
    //     i = i - 1;
    // }
    // Allwhitestart = i;
    // Allwhiteend = Fir_row;
    // while (Allwhiterow[i] == 1 && i > Fir_row)
    // {
    //     i = i - 1;
    // }
    // Allwhiteend = i;
    Allwhiteend = Fir_row;
    for (i = Fir_row; i < Last_row - 6; ++i)
    {
        if (Allwhiterow[i])
        {
            Allwhiteend = i - 1; //ȫ���е�Զ��һ�У�����Ϊ��ȫ���У�
            break;
        }
    }
    Allwhitestart = Fir_row;
    for (i = Allwhiteend + 2; i < Last_row - 6; ++i)
    {
        if (!Allwhiterow[i])
        {
            Allwhitestart = i - 1; //������Ϊȫ���У�
            break;
        }
    }
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
#if 0
//�ѷ��벹����
/*************************************************************************
*  �������ƣ�void break_point_find_und(void)
*  ����˵����Բ���ڻ�ȥ����ĵ�
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020.07.11
*  ��    ע��

*************************************************************************/

void break_point_find_und(bool L, bool R)
{
    if (Lef_circle == 1 && L)
    {
        Lef_break_point_und = 0;
        for (int i = FIG_AREA_NEAR; i < FIG_AREA_FAR; --i)
        {
            if (New_Lef[i] != -MIDMAP && New_Lef[i] < -50 && New_Lef[i] > Lef_break_point_und)
            {
                Lef_break_point_und = New_Lef[i];
            }
        }
    }
    if (Rig_circle == 1 && R)
    {
        Rig_break_point_und = 0;
        for (int i = FIG_AREA_NEAR; i < FIG_AREA_FAR; --i)
        {
            if (New_Rig[i] != MIDMAP && New_Rig[i] > 50 && New_Rig[i] < Rig_break_point_und)
            {
                Rig_break_point_und = New_Rig[i];
            }
        }
    }
}
#endif
/*************************************************************************
*  �������ƣ�void crossing_find(void)
*  ����˵����ʮ��ʶ��
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020.07.13
*  ��    ע��
*************************************************************************/
void crossing_find(void)
{
    crossing_flag = 0;
    int Lef_none_edge[LCDH], Rig_none_edge[LCDH];
    int jl = 0, jr = 0;
    int Lmin = 0, Lmax = 0, Rmin = 0, Rmax = 0;
    int temp1 = 0, temp2 = 0;
    bool set_flag1 = 1, set_flag2 = 1, find_flag1 = 0, find_flag2 = 0;
    float slopetemp;
    int y, sum = 0;
    for (int i = 0; i < LCDH; ++i)
    {
        Lef_none_edge[i] = -1;
        Rig_none_edge[i] = -1;
    }
    for (int i = 45; i > Fir_row + 5; --i)
    {
        if (Lef[i] < Fir_col + 5)
        {
            Lef_none_edge[jl] = i;
            ++jl;
        }
        if (Rig[i] > Last_col - 5)
        {
            Rig_none_edge[jr] = i;
            ++jr;
        }
    }
    for (int i = 0; Lef_none_edge[i] != -1; ++i)
    {
        if (set_flag1)
        {
            Lmax = Lef_none_edge[i];
            set_flag1 = !set_flag1;
        }
        if (Lef_none_edge[i + 1] - Lef_none_edge[i] > 1 || Lef_none_edge[i + 1] == -1)
        {
            Lmin = Lef_none_edge[i];
            if (Lmax - Lmin < 3)
            {
                set_flag1 = 1;
            }
            else
            {
                find_flag1 = !find_flag1;
                break;
            }
        }
    }
    for (int i = 0; Rig_none_edge[i] != -1; ++i)
    {
        if (set_flag2)
        {
            Rmax = Rig_none_edge[i];
            set_flag2 = !set_flag2;
        }

        if (Rig_none_edge[i + 1] - Rig_none_edge[i] > 1 || Rig_none_edge[i + 1] == -1)
        {
            Rmin = Rig_none_edge[i];
            if (Rmax - Rmin < 3)
            {
                set_flag2 = 1;
            }
            else
            {
                find_flag2 = !find_flag2;
                break;
            }
        }
    }
    if (!find_flag1 || !find_flag2)
    {
        return;
    }

    if (Lmax > Rmax && Lmin > Rmin)
    {
        if (Lmin - Rmax < 3)
        {
            temp1 = (Lmin + Lmax) / 2;
            temp2 = (Rmin + Rmax) / 2;
        }
        else
        {
            return;
        }
    }
    else if (Rmax > Lmax && Rmin > Lmin)
    {
        if (Rmin - Lmax < 3)
        {
            temp1 = (Lmin + Lmax) / 2;
            temp2 = (Rmin + Rmax) / 2;
        }
        else
        {
            return;
        }
    }
    else
    {
        return;
    }

    slopetemp = Slope(1, temp1, 78, temp2);
    crossing_flag = 0;
    sum = 0;
    for (int x = 10; x < 23; x += 4)
    {
        y = (int)(slopetemp * (x - 1) + temp1);
        sum += Pixle[y][x];
        if (sum > 2)
        {
            crossing_flag = !crossing_flag;
            break;
        }
    }
    if (!crossing_flag)
    {
        return;
    }
    crossing_flag = !crossing_flag;
    sum = 0;
    for (int x = 58; x < 71; x += 4)
    {
        y = (int)(slopetemp * (x - 1) + temp1);
        sum += Pixle[y][x];
        if (sum > 2)
        {
            crossing_flag = !crossing_flag;
            break;
        }
    }

    if (!crossing_flag)
    {
        return;
    }
    crossing_flag = !crossing_flag;
    sum = 0;
    for (int x = 34; x < 47; x += 4)
    {
        y = (int)(slopetemp * (x - 1) + temp1);
        sum += Pixle[y][x];
        if (sum > 2)
        {
            crossing_flag = !crossing_flag;
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
// bool DEBUGDEBUG;

void start_stop_find(void)
{
    start_stop_line_flag = 0;

    int tiaobian1 = 0;
    bool Method3_flag = 0;
    int Method3_sum2 = 0;
    int Method3_sum = 0;
    int Method2_sum = 0;
    int Method2_sum2 = 0;
    int Method3_k = 0;

    int R_STOP = 0, L_STOP = 0;
    float STOP_slope;
    int Rig_end = 60, Lef_end = 60, Road_end = 60;

    // if (Road == 0 && Road0_flag == 0)
    // {
    if (barn_reset_flag == 0)
    {
        return;
    }
    for (int i = Fir_row; i < Last_row - 4; ++i)
    {
        if (Rig[i] >= Last_col)
        {
            continue;
        }
        if (Rig[i + 2] - Rig[i] < 5 && Rig[i + 3] - Rig[i + 1] < 5 &&
            Rig[i + 2] - Rig[i] >= 0 && Rig[i + 3] - Rig[i + 1] >= 0 && Rig[i] < 78)
        {
            Rig_end = i;
            break;
        }
    }
    for (int i = Fir_row; i < Last_row - 4; ++i)
    {
        if (Lef[i] <= Fir_col)
        {
            continue;
        }
        if (Lef[i] - Lef[i + 2] < 5 && Lef[i + 1] - Lef[i + 3] < 5 &&
            Lef[i] - Lef[i + 2] >= 0 && Lef[i + 1] - Lef[i + 3] >= 0 && Lef[i] > 2)
        {
            Lef_end = i;
            break;
        }
    }
    Road_end = (Lef_end < Rig_end ? Lef_end : Rig_end);
    if (Road_end > 30)
    {
        return;
    }
    // Method 1
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
//Method 2
#ifdef TL2barn
    // Rig_end = (Rig_end < (Fir_row + 3) ? (Fir_row + 3) : Rig_end);
    for (int i = Rig_end; i < Last_row - 7; ++i)
    {
        if (Rig[i + 2] - Rig[i] < 5 && Rig[i + 3] - Rig[i + 1] < 5 &&
            Rig[i + 2] - Rig[i] > 0 && Rig[i + 3] - Rig[i + 1] > 0 &&
            Rig[i + 3] - Rig[i + 7] > 18)
        {
            tiaobian1 = i + 4;
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
            if (((Rig[i] - Rig[i - 4] > 18) || (Rig[i] - Rig[i - 3] > 18)) && Rig[i + 2] - Rig[i] < 5 && Rig[i + 3] - Rig[i + 1] < 5 && Rig[i + 4] - Rig[i + 2] < 5)
            {
                for (int j = i - 1; j > i - 4; --j)
                {
                    for (int k = Rig[j]; k < Rig[i]; ++k) //��΢�o�һ�c
                    {
                        Method2_sum += Pixle[j][k];
                        Method2_sum2++;
                    }
                }
                if (Method2_sum * 1.0 / Method2_sum2 < 0.3)
                {
                    start_stop_line_flag = 1;
                    start_stop_line = tiaobian1;
                }
                return;
            }
        }
    }
#endif
#ifdef TR2barn
    // Lef_end = (Lef_end < (Fir_row + 3) ? (Fir_row + 3) : Lef_end);
    for (int i = Lef_end; i < Last_row - 7; ++i)
    {
        if (Lef[i] - Lef[i + 2] < 5 && Lef[i + 1] - Lef[i + 3] < 5 &&
            Lef[i] - Lef[i + 2] > 0 && Lef[i + 1] - Lef[i + 3] > 0 &&
            Lef[i + 7] - Lef[i + 3] > 18)
        {
            tiaobian1 = i + 4;
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
            if (((Lef[i - 3] - Lef[i] > 18) || (Lef[i - 4] - Lef[i] > 18)) && Lef[i] - Lef[i + 2] < 5 && Lef[i + 1] - Lef[i + 3] < 5 && Lef[i + 2] - Lef[i + 4] < 5)
            {
                for (int j = i - 1; j > i - 4; --j)
                {
                    for (int k = Lef[j]; k > Lef[i]; --k) //��΢�o�һ�c
                    {
                        Method2_sum += Pixle[j][k];
                        Method2_sum2++;
                    }
                }
                if (Method2_sum * 1.0 / Method2_sum2 < 0.3)
                {
                    start_stop_line_flag = 1;
                    start_stop_line = tiaobian1;
                }
                return;
            }

            else
            {
                continue;
            }
        }
    }
#endif
    ///Method 3
#if 1
    if (Rig_end > 25 && Lef_end > 25)
        return;
    for (int i = 50; i > Rig_end; i--)
    {
        if (Rig[i + 6] - Rig[i + 4] < 5 && Rig[i + 4] - Rig[i + 2] < 5 && Rig[i + 3] - Rig[i + 1] < 5 &&
            (Rig[i + 2] - Rig[i] >= 5 || Rig[i + 2] - Rig[i] < 0) &&
            Rig[i + 6] - Rig[i + 4] >= 0 && Rig[i + 4] - Rig[i + 2] >= 0 && Rig[i + 3] - Rig[i + 1] >= 0 && Rig[i + 1] > 20)
        {

            // if ((Pixle[i][Rig[i + 1] - 5] == 0 || Pixle[i - 2][Rig[i + 1] - 5] == 0) && Pixle[i - 1][Rig[i + 2] - 5] == 0)
            if ((Pixle[i][Rig[i + 1] - 5] == 0 && Pixle[i - 1][Rig[i + 1] - 5] == 0) ||
                (Pixle[i - 1][Rig[i + 1] - 5] == 0 && Pixle[i - 2][Rig[i + 1] - 5] == 0) ||
                (Pixle[i - 2][Rig[i + 1] - 5] == 0 && Pixle[i - 3][Rig[i + 1] - 5] == 0))
            {
                R_STOP = i + 1;
                break;
            }
        }
    }
    for (int i = Rig_end; i < R_STOP; ++i)
    {
        if (Rig[i + 2] - Rig[i] < 5 && Rig[i + 3] - Rig[i + 1] < 5 &&
            (Rig[i + 4] - Rig[i + 2] >= 5 || Rig[i + 4] - Rig[i + 2] <= 0) &&
            Rig[i + 2] - Rig[i] > 0 && Rig[i + 3] - Rig[i + 1] > 0)
        {
            if (Rig[R_STOP] - Rig[i + 3] > -5 && Rig[R_STOP] - Rig[i + 3] < 15)
            {
#ifdef TL2barn
                start_stop_line = R_STOP;
#endif
                Method3_flag = 1;
                break;
            }
        }
    }
    if (Method3_flag)
    {
        Method3_flag = 0;
        for (int i = 50; i > Lef_end; i--)
        {
            if (Lef[i + 4] - Lef[i + 6] < 5 && Lef[i + 2] - Lef[i + 4] < 5 && Lef[i + 1] - Lef[i + 3] < 5 &&
                (Lef[i] - Lef[i + 2] >= 5 || Lef[i] - Lef[i + 2] < 0) &&
                Lef[i + 4] - Lef[i + 6] >= 0 && Lef[i + 2] - Lef[i + 4] >= 0 && Lef[i + 1] - Lef[i + 3] >= 0 && Lef[i + 1] < 60)
            {

                if ((Pixle[i][Lef[i + 1] + 5] == 0 && Pixle[i - 1][Lef[i + 1] + 5] == 0) ||
                    (Pixle[i - 1][Lef[i + 1] + 5] == 0 && Pixle[i - 2][Lef[i + 1] + 5] == 0) ||
                    (Pixle[i - 2][Lef[i + 1] + 5] == 0 && Pixle[i - 3][Lef[i + 1] + 5] == 0))
                {
                    L_STOP = i + 1;
                    break;
                }
            }
        }
        for (int i = Lef_end; i < L_STOP; ++i)
        {
            if (Lef[i] - Lef[i + 2] < 5 && Lef[i + 1] - Lef[i + 3] < 5 &&
                (Lef[i + 2] - Lef[i + 4] >= 5 || Lef[i + 2] - Lef[i + 4] <= 0) &&
                Lef[i] - Lef[i + 2] > 0 && Lef[i + 1] - Lef[i + 3] > 0)
            {
                if (Lef[i + 3] - Lef[L_STOP] > -5 && Lef[i + 3] - Lef[L_STOP] < 15)
                {
                    Method3_flag = 1;
#ifdef TR2barn
                    start_stop_line = L_STOP;
#endif
                    break;
                }
            }
        }
    }
    if (Method3_flag)
    {
        STOP_slope = Slope(Lef[L_STOP], L_STOP, Rig[R_STOP], R_STOP);
        for (int j = 1; j < 5; ++j)
        {
            for (int i = Lef[L_STOP]; i < Rig[R_STOP]; ++i)
            {
                Method3_k = (int)(STOP_slope * (i - Rig[R_STOP]) + R_STOP - j);
                Method3_k = Method3_k < 1 ? 1 : (Method3_k > 78 ? 78 : Method3_k);
                Method3_sum += Pixle[Method3_k][i];
                Method3_sum2 += 1;
            }
        }
        if (Method3_sum2 != 0)
        {
            if (Method3_sum * 1.0 / Method3_sum2 < 0.4)
            {
                start_stop_line_flag = 1;
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
    // static int start_waited = 0;
    // if (barn_reset_flag == 0)
    // {
    //     start_waited++;
    // }
    if (loop_time > 6000) //(start_waited >= 600)
    {
        gpio_interrupt_init(C25, FALLING, GPIO_INT_CONFIG);
        // start_waited = 0;
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
    static int Road100_count = 0;
    static int Road200_count = 0;
    //��ֱ·
    if (Road != 0)
    {
        if (Road == 4 && Lef_slope == 998 && Rig_slope == 998)
        {
            Road0_count++;
            if (Road0_count >= 3)
            {
                Road = 0;
                Road0_flag = 0;
            }
        }
        if (fabs(Lef_slope) > 1.5 && fabs(Rig_slope) > 1.5 && Road7_flag != 2 &&
            ((Lef_edge < 10 && Rig_edge < 10) || (Lef_edge < 4 && Rig_edge < 12) || (Rig_edge < 4 && Lef_edge < 12)))
        {
            if (Lef[21] - Lef[23] < 5 && Lef[23] - Lef[25] < 5 && Rig[23] - Lef[21] < 5 && Rig[25] - Rig[23] < 5 && Lef[21] - Lef[23] >= 0 && Lef[23] - Lef[25] >= 0 && Rig[23] - Lef[21] >= 0 && Rig[25] - Rig[23] >= 0)
                Road0_count++;
            if (Road == 4 && Road4_flag > 1)
                Road0_count = 0;
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
            if (Road70_count > 1)
            {
                Road70_count = 0;
                Road = 7;
                Road7_flag = 0;
#ifdef TL2barn

                for (int i = ((start_stop_line - 10) > Fir_row ? (start_stop_line - 10) : Fir_row); i < Last_row - 2; ++i)
                {
                    if (Lef[i - 4] - Lef[i - 2] < 5 && Lef[i - 3] - Lef[i - 1] < 5 && Lef[i - 2] - Lef[i] < 5)
                    {
                        continue;
                    }
                    if ((Lef[i] - Fir_col < 5 && (Lef[i + 1] - Fir_col < 5 || Lef[i + 2] - Fir_col < 5)) ||
                        (Lef[i - 1] - Lef[i + 2] > 15 && Lef[i - 1] - Lef[i + 1] > 15) ||
                        (Pixle[i][3] + Pixle[i][4] + Pixle[i][5] == 3 && Pixle[i + 1][3] + Pixle[i + 1][4] + Pixle[i + 1][5] == 3 && Pixle[i + 2][3] + Pixle[i + 2][4] + Pixle[i + 2][5] == 3))
                    {
                        barn_line = i - 1;
                        break;
                    }
                    barn_line = start_stop_line - 5;
                }
#endif
#ifdef TR2barn

                for (int i = ((start_stop_line - 10) > Fir_row ? (start_stop_line - 10) : Fir_row); i < Last_row - 2; ++i)
                {
                    if (Rig[i - 2] - Rig[i - 4] < 5 && Rig[i - 1] - Rig[i - 3] < 5 && Rig[i] - Rig[i - 2] < 5)
                    {
                        continue;
                    }
                    if ((Last_col - Rig[i] < 5 && (Last_col - Rig[i + 1] < 5 || Last_col - Rig[i + 2] < 5)) ||
                        (Rig[i + 2] - Rig[i - 1] > 15 && Rig[i + 1] - Rig[i - 1] > 15) ||
                        (Pixle[i][76] + Pixle[i][75] + Pixle[i][74] == 3 && Pixle[i + 1][76] + Pixle[i + 1][75] + Pixle[i + 1][74] == 3 && Pixle[i + 2][76] + Pixle[i + 2][75] + Pixle[i + 2][74] == 3))
                    {
                        barn_line = i - 1;
                        break;
                    }
                    barn_line = start_stop_line - 5;
                }
#endif

                barn_line = (barn_line > (Fir_row + 5) ? barn_line : (Fir_row + 5));
            }
        }
        else
        {
            Road70_count = 0;
        }
    }

    if (Road == 0)
    {
        /* ����ͷ���� */
        // if (New_Lef[22] > New_Lef[20] && New_Lef[20] > New_Lef[18] && New_Lef[18] > New_Lef[16] && New_Lef[16] > New_Lef[14] && New_Lef[15] > New_Lef[13] && New_Lef[14] > New_Lef[12] && New_Lef[13] > New_Lef[11] && New_Lef[12] > New_Lef[10] &&
        //     New_Rig[22] < New_Rig[20] && New_Rig[20] < New_Rig[18] && New_Rig[18] < New_Rig[16] && New_Rig[16] < New_Rig[14] && New_Rig[15] < New_Rig[13] && New_Rig[14] < New_Rig[12] && New_Rig[13] < New_Rig[11] && New_Rig[12] < New_Rig[10] &&
        //     New_Lef[6] > -300 && New_Rig[6] < 300 && New_Lef[8] > -300 && New_Rig[8] < 300)
        // {
        //     Road40_count++;
        //     if (Road40_count > 2)
        //     {
        //         Road = 4;
        //         Road4_flag = 0;
        //         return;
        //     }
        // }
        // else
        // {
        //     Road40_count = 0;
        // }

        /* ICM���� */
        if (icm_gyro_y_w < -30 * (CarSpeed1 + CarSpeed2) && icm_gyro_y_w < -60 && loop_time - ramp_out_time > 500)
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
        //���ж��Ƿ���������������������ΪRoad1_flag0����λ����������������else if�ж�ֱ�ӽ�Road1_flag=1������
        if (Rig[39] - Rig[37] < 5 && Rig[37] - Rig[35] < 5 && Rig[35] - Rig[33] < 5 && Rig[33] - Rig[31] < 5 && Rig[31] - Rig[29] < 5 && Rig[29] - Rig[27] < 5 && Rig[27] - Rig[25] < 5 && Rig[25] - Rig[23] < 5 &&
            Rig[39] - Rig[37] > 0 && Rig[37] - Rig[35] > 0 && Rig[35] - Rig[33] > 0 && Rig[33] - Rig[31] > 0 && Rig[31] - Rig[29] > 0 && Rig[29] - Rig[27] > 0 && Rig[27] - Rig[25] > 0 && Rig[25] - Rig[23] > 0)

        {
            if (Lef_circle_pre_flag == 1 && Rig_circle_pre_flag == 0 &&
                Rig[45] - Rig[43] < 5 && Rig[43] - Rig[41] < 5 && Rig[41] - Rig[39] < 5 &&
                Rig[45] - Rig[43] >= 0 && Rig[43] - Rig[41] >= 0 && Rig[41] - Rig[39] >= 0)
            {

                Road100_count++;
                if (Road100_count >= 2)
                {
                    Road100_count = 0;
                    Road = 1;
                    Road1_flag = 0;
                }

                return;
            }
            else
            {
                Road100_count = 0;
            }

            if (Rig_circle == 0 && Lef_circle == 1 && Lef_slope != 998 && Rig_slope >= 0 &&
                Lef_break_point < 45 && EM_Value_2 + EM_Value_3 > 4.2)
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
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////��ͨ������Բ��
        //���ж��Ƿ���������������������ΪRoad2_flag0����λ����������������else if�ж�ֱ�ӽ�Road2_flag=1������
        if (Lef[25] - Lef[27] < 5 && Lef[27] - Lef[29] < 5 && Lef[29] - Lef[31] < 5 && Lef[31] - Lef[33] < 5 && Lef[33] - Lef[35] < 5 && Lef[35] - Lef[37] < 5 && Lef[37] - Lef[39] < 5 && Lef[23] - Lef[25] < 5 &&
            Lef[37] - Lef[39] > 0 && Lef[35] - Lef[37] > 0 && Lef[33] - Lef[35] > 0 && Lef[31] - Lef[33] > 0 && Lef[29] - Lef[31] > 0 && Lef[27] - Lef[29] > 0 && Lef[25] - Lef[27] > 0 && Lef[23] - Lef[25] > 0)
        {
            if (Lef_circle_pre_flag == 0 && Rig_circle_pre_flag == 1 &&
                Lef[43] - Lef[45] < 5 && Lef[41] - Lef[43] < 5 && Lef[39] - Lef[41] < 5 &&
                Lef[43] - Lef[45] >= 0 && Lef[41] - Lef[43] >= 0 && Lef[39] - Lef[41] >= 0)
            {
                Road200_count++;
                if (Road200_count >= 2)
                {
                    Road200_count = 0;
                    Road = 2;
                    Road2_flag = 0;
                }

                return;
            }
            else
            {
                Road200_count = 0;
            }

            if (Lef_circle == 0 && Rig_circle == 1 && Rig_slope != 998 && (Lef_slope <= 0 || Lef_slope == 998) &&
                Rig_break_point < 45 && EM_Value_2 + EM_Value_3 > 4.2)
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
        }
        //���״̬��
        // else if (((Rig_slope > -0.5 && Rig_slope != 998 && Rig_slope != 999) || Road0_flag == 4) && Rig_slope != 998) //��ת��//(Lef_break_point > 35 && Lef_circle == 1 && Rig_circle == 0)
        if ((Rig_slope != 998 && Rig_slope != 999) || Road0_flag == 4) //DEBUG chongxintiao
        {
            TurnLeft_Process();
            if (Road0_flag == 4)
            {
                return;
            }
        }
        // else if ((Lef_slope < 0.5 || Road0_flag == 5) && Lef_slope != 998) //��ת��//(Rig_break_point > 35 && Rig_circle == 1 && Lef_circle == 0)
        if ((Lef_slope != 998 && Lef_slope != 999) || Road0_flag == 5)
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
        else if (Allwhitestart > 45 && Allwhiteend != 20 && (Allwhiteend < 40 || (Allwhiteend < 52 && Road0_flag == 2)) && Allwhitestart - Allwhiteend > 3 && whitecnt > 2000)
        {
            Road0_flag = 2;
        }
        else if (crossing_flag_flag)
        {
            Road0_flag = 1;
            Allwhitestart = whiteline_end;
            Allwhiteend = whiteline_start;
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
    else if (Road == 4)
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
    int temp = 0;
    int dis = 0, dis1 = 0;
    for (int i = Fir_row; i < 48; ++i)
    {
        if (Rig[i] < 40 && Rig[i + 1] <= 40 && Rig[i + 2] >= 40 && Rig[i + 3] - Rig[i + 1] < 7 && //Rig[i + 3] > 40 &&
            Rig[i + 5] -
                    Rig[i + 3] <
                7 &&
            Rig[i + 7] - Rig[i + 5] < 7 && Rig[i + 9] - Rig[i + 7] < 7 && Rig[i + 11] - Rig[i + 9] < 7 &&
            Rig[i + 5] - Rig[i + 3] > 0 && Rig[i + 7] - Rig[i + 5] > 0 && Rig[i + 9] - Rig[i + 7] > 0 && Rig[i + 11] - Rig[i + 9] > 0 &&
            Rig[i + 1] - Rig[i] > 0 && Rig[i + 2] - Rig[i + 1] > 0)
        //���ܽ��ϣ���������������Ϊ40��
        {
            temp = i + 2;
            break;
        }
    }

    if (temp == 0 && Road0_flag != 5) //&& turn_stop_flag == 1)
    {
        Road00_count++;
        if (Road00_count == 2)
        {
            Road00_count = 0;
            Road0_flag = 0;
            // turn_stop_flag = 0;
        }
        return;
    }
    else if (temp == 0)
    {
        Road00_count = 0;
        return;
    }
    else
    {
        Road00_count = 0;
    }
    dis = Rig[temp + 1] - Rig[temp];
    if (dis == 0)
    {
        dis = 1;
    }
    for (int i = temp; i > Fir_row; --i)
    {
        if (Rig[i - 1] > 40)
        {
            turn_stop = i;
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

            if (i == Fir_row + 1)
            {
                turn_stop = i - 1;
            }
            continue;
        }
        else if (dis1 <= 2 * dis + 1)
        {
            dis = dis1;

            if (i == Fir_row + 1)
            {
                turn_stop = i - 1;
            }
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
    if (Road0_flag != 4 && Rig[turn_stop] < 36 && dis > 2)
    {
        Road04_count++;
        if (Road04_count == 2)
        {
            Road = 0;
            Road0_flag = 4; //turn left flag
            // road_change_flag = 1;
            // threshold_offset2 = -5;
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
    int temp = 0;
    int dis = 0, dis1 = 0;
    for (int i = Fir_row; i < 48; ++i)
    {
        if (Lef[i] > 40 && Lef[i + 1] >= 40 && Lef[i + 2] <= 40 && Lef[i + 1] - Lef[i + 3] < 7 && //Lef[i + 3] < 40 &&
            Lef[i + 3] - Lef[i + 5] < 7 && Lef[i + 5] - Lef[i + 7] < 7 && Lef[i + 7] - Lef[i + 9] < 7 && Lef[i + 9] - Lef[i + 11] < 7 &&
            Lef[i + 3] - Lef[i + 5] > 0 && Lef[i + 5] - Lef[i + 7] > 0 && Lef[i + 7] - Lef[i + 9] > 0 && Lef[i + 9] - Lef[i + 11] > 0 &&
            Lef[i] - Lef[i + 1] > 0 && Lef[i + 1] - Lef[i + 2] > 0)
        {
            temp = i + 2;
            break;
        }
    }

    if (temp == 0 && Road0_flag != 4) // && turn_stop_flag == 1)
    {
        Road00_count++;
        if (Road00_count >= 3)
        {
            Road00_count = 0;
            Road0_flag = 0;
            // road_change_flag = 1;
            // turn_stop_flag = 0;
        }

        return;
    }
    else if (temp == 0)
    {
        Road00_count = 0;
        return;
    }
    else
    {
        Road00_count = 0;
    }
    dis = Lef[temp] - Lef[temp + 1];
    if (dis == 0)
    {
        dis = 1;
    }
    for (int i = temp; i > Fir_row; --i)
    {
        if (Lef[i - 1] < 40)
        {
            turn_stop = i;
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
            if (i == Fir_row + 1)
            {
                turn_stop = i - 1;
            }
            continue;
        }
        else if (dis1 <= 2 * dis + 1)
        {
            dis = dis1;
            if (i == Fir_row + 1)
            {
                turn_stop = i - 1;
            }
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
    if (Road0_flag != 5 && Lef[turn_stop] > 43 && dis > 2)
    {
        Road05_count++;
        if (Road05_count == 2)
        {
            Road = 0;
            Road0_flag = 5; //turn left flag
            // road_change_flag = 1;
            // threshold_offset2 = -5;
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
*  �������ƣ�void TurnLeft_Process(void)
*  ����˵������ת����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020.06.17
*  ��    ע��
*************************************************************************/
void TurnLeftCircle_Process(void)
{
    static int Road14_count = 0; //, Road00_count = 0; //turn_stop_flag = 0,
    int temp = 0;
    int dis = 0, dis1 = 0;
    for (int i = Last_row - 8; i > Fir_row; --i)
    {
        if (Rig[i] >= Last_col)
        {
            continue;
        }
        if (Rig[i + 2] - Rig[i] < 8 && Rig[i + 2] - Rig[i] > 0)
        {
            if (Rig[i] >= 40)
            {
                continue;
            }
            else
            {
                temp = i;
                break;
            }
        }
        else
        {
            if (Rig[i + 2] - Rig[i] < 10 && Rig[i] < 40)
            {
                temp = i;
            }
            else
            {
                break;
            }
        }
    }

    // if (temp == 40 && Road1_flag != 4) //&& turn_stop_flag == 1)
    // {
    //     Road00_count++;
    //     if (Road00_count == 2)
    //     {
    //         Road00_count = 0;
    //         Road0_flag = 0;
    //         // turn_stop_flag = 0;
    //     }

    //     return;
    // }
    // else
    // {
    //     Road00_count = 0;
    // }
    if (temp != 0)
    {
        dis = Rig[temp + 1] - Rig[temp];
        if (dis == 0)
        {
            dis = 1;
        }
        for (int i = temp; i > Fir_row; --i)
        {
            if (Rig[i - 1] > 40)
            {
                turn_stop = i;
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

                if (i == Fir_row + 1)
                {
                    turn_stop = i - 1;
                }
                continue;
            }
            else if (dis1 <= 2 * dis + 1)
            {
                dis = dis1;

                if (i == Fir_row + 1)
                {
                    turn_stop = i - 1;
                }
                continue;
            }
            else
            {
                turn_stop = i;
                break;
            }
        }
    }
    // if (turn_stop < 28)
    // {
    //     turn_stop_flag = 1;
    // }
    if (Road1_flag != 4 && Rig[turn_stop] < 36 && dis > 2)
    {
        Road14_count++;
        if (Road14_count == 2)
        {
            Road1_flag = 4;   //turn left flag
            Road14_count = 0; //reset
        }
    }
    else
    {
        Road14_count = 0;
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
void TurnRightCircle_Process(void)
{
    static int Road24_count = 0; //, Road00_count = 0; //turn_stop_flag = 0,
    int temp = 0;
    int dis = 0, dis1 = 0;
    for (int i = Last_row - 8; i > Fir_row; --i)
    {
        if (Lef[i] <= Fir_col)
        {
            continue;
        }
        if (Lef[i] - Lef[i + 2] < 8 && Lef[i] - Lef[i + 2] > 0)
        {
            if (Lef[i] <= 40)
            {
                continue;
            }
            else
            {
                temp = i;
                break;
            }
        }
        else
        {
            if (Lef[i] - Lef[i + 2] < 10 && Lef[i] > 40)
            {
                temp = i;
            }
            else
            {
                break;
            }
        }
    }

    // if (temp == 40 && Road0_flag != 4) // && turn_stop_flag == 1)
    // {
    //     Road00_count++;
    //     if (Road00_count >= 3)
    //     {
    //         Road00_count = 0;
    //         Road0_flag = 0;
    //         road_change_flag = 1;
    //         // turn_stop_flag = 0;
    //     }

    //     return;
    // }
    // else
    // {
    //     Road00_count = 0;
    // }

    if (temp != 0)
    {
        dis = Lef[temp] - Lef[temp + 1];
        if (dis == 0)
        {
            dis = 1;
        }
        for (int i = temp; i > Fir_row; --i)
        {
            if (Lef[i - 1] < 40)
            {
                turn_stop = i;
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
                if (i == Fir_row + 1)
                {
                    turn_stop = i - 1;
                }
                continue;
            }
            else if (dis1 <= 2 * dis + 1)
            {
                dis = dis1;
                if (i == Fir_row + 1)
                {
                    turn_stop = i - 1;
                }
                continue;
            }
            else
            {
                turn_stop = i;
                break;
            }
        }
    }
    if (Road2_flag != 4 && Lef[turn_stop] > 43 && dis > 2)
    {
        Road24_count++;
        if (Road24_count == 2)
        {
            Road2_flag = 4;   //turn flag
            Road24_count = 0; //reset
        }
    }
    else
    {
        Road24_count = 0;
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
    static int Road0_count = 0, Road11_count = 0, Road12_count = 0, Road13_count = 0, Road14_count = 0, Road15_count = 0, Road16_count = 0, Road17_count = 0;
    int dis = 0, dis1 = 0;
    if (Road1_flag == 0) //׼������Բ��
    {
        Road0_flag = 0;
        if (Rig_circle == 0 &&
            // Lef_circle == 1 &&
            Lef_slope != 998 &&
            Rig[39] - Rig[37] < 5 && Rig[37] - Rig[35] < 5 && Rig[35] - Rig[33] < 5 && Rig[33] - Rig[31] < 5 && Rig[31] - Rig[29] < 5 && Rig[29] - Rig[27] < 5 && Rig[27] - Rig[25] < 5 && Rig[25] - Rig[23] < 5 && Rig_slope >= 0 && ((EM_Value_2 + EM_Value_3 > 4.0) || (EM_Value_2 > 2.6 && Lef_circle == 1)))
        //if (EM_Value_2 + EM_Value_3 > 4.3) //����ʶ���������߽���һ�߷�������
        {
            Road11_count++;
            if (Road11_count > 1)
            {
                Road11_count = 0;
                Road1_flag = 1; //��ʾ�Ѿ�������Բ��
                mean_turn_out = 0;
            }
        }
        else
        {
            Road11_count = 0;
        }
        if (Allwhiteend < 35 && Allwhiteend != 20)
        {
            Road0_count++;
            if (Road0_count > 1)
            {
                Road = 0;
                Road0_flag = 0;
                Road0_count = 0;
            }
        }
        else
        {
            Road0_count = 0;
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
                // road_change_flag = 1;
            }
        }
        else
        {
            Road12_count = 0;
        }
        TurnLeftCircle_Process();
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
        /////////////////////////////////////
        TurnLeftCircle_Process();
        /////////////////////////////////////////////
        // for (int i = Last_row; i > Fir_row; i--)
        // {
        //     if (Last_col - Rig[i] < 2 || Last_col - Rig[i - 1] < 2)
        //     {
        //         continue;
        //     }
        // if (i > 40 && Rig[i] - Rig[i - 2] < 8 && Rig[i - 2] - Rig[i - 4] < 8 && Rig[i] - Rig[i - 2] > 0 && Rig[i - 2] - Rig[i - 4] > 0)
        // {
        //     Road14_count++;
        //     //
        //     if (Road14_count > 1)
        //     {
        //         Road1_flag = 4;
        //         Road14_count = 0;
        //         break;
        //     }
        // }
        // else
        // {
        //     Road14_count = 0;
        //     break;
        // }
        //}
        // if (Road14_count > (int)(DIS_IN_CIRCLE * 10000 / (SpeedGoal * CAMERA_FPS)) + 1) //�궨����function.h
        // {
        //     Road1_flag = 4;
        //     Road14_count = 0;
        // }
    }
    else if (Road1_flag == 4) //����Բ���� ��ȡ������
    {
        for (int i = 40; i > 20 && dis == 0; --i)
        {
            dis = Rig[i + 1] - Rig[i];
        }

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
                if (i == Fir_row + 1)
                {
                    turn_stop = i - 1;
                }
                continue;
            }
            else if (dis1 <= 2 * dis + 1)
            {
                dis = dis1;
                if (i == Fir_row + 1)
                {
                    turn_stop = i - 1;
                }
                continue;
            }
            else
            {
                turn_stop = i;
                break;
            }
        }
        if (Allwhitestart > 25 && Allwhitestart < 40 && Allwhitestart - Allwhiteend > 2) //Rig_circlr ���ã���!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
        if (Allwhitestart >= 40 && Allwhiteend < 32)
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
        if (Allwhitestart >= 40)
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
        if (Rig_slope < -0.2) // -0.15 || Rig_slope == 998) || (Rig_slope < -0.1 && (Allwhiteend > 43 || Allwhiteend == Fir_row)))
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
        // if (EM_Value_1 < 0.6 && EM_Value_4 < 0.6 && Lef_edge < 10)
        if (Lef_edge < 10)
        {
            Road17_count++;
            if (Road17_count > 1)
            {
                Road = 0;
                Road0_flag = 0;
                Road1_flag = 0;
                // road_change_flag = 1;
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
    static int Road0_count = 0, Road21_count = 0, Road22_count = 0, Road23_count = 0, Road24_count = 0, Road25_count = 0, Road26_count = 0, Road27_count = 0;
    int dis = 0, dis1 = 0;
    if (Road2_flag == 0) //
    {
        if (Lef_circle == 0 &&
            // Rig_circle == 1 &&
            Rig_slope != 998 &&
            Lef[25] - Lef[27] < 5 && Lef[27] - Lef[29] < 5 && Lef[29] - Lef[31] < 5 && Lef[31] - Lef[33] < 5 && Lef[33] - Lef[35] < 5 && Lef[35] - Lef[37] < 5 && Lef[37] - Lef[39] < 5 && Lef[23] - Lef[25] < 5 && (Lef_slope <= 0 || Lef_slope == 998) && ((EM_Value_2 + EM_Value_3 > 4.0) || (EM_Value_3 > 2.6 && Rig_circle == 1)))
        //if (EM_Value_2 + EM_Value_3 > 4.3) //����ʶ���������߽���һ�߷�������
        {
            Road21_count++;
            if (Road21_count > 1)
            {
                Road21_count = 0;
                Road2_flag = 1; //��ʾ�Ѿ�������Բ��
                mean_turn_out = 0;
            }
        }
        else
        {
            Road21_count = 0;
        }
        if (Allwhiteend < 35 && Allwhiteend != 20)
        {
            Road0_count++;
            if (Road0_flag > 1)
            {
                Road0_count = 0;
                Road = 0;
                Road0_flag = 0;
            }
        }
        else
        {
            Road0_count = 0;
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
                // road_change_flag = 1;
            }
        }
        else
        {
            Road22_count = 0;
        }
        TurnRightCircle_Process();
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
        TurnRightCircle_Process();

        ////////////////////////////////////////
        // for (int i = Last_row; i > Fir_row; i--)
        // {
        //     if (Lef[i] - Fir_col < 2 || Lef[i - 1] - Fir_col < 2)
        //     {
        //         continue;
        //     }
        //     if (i > 40 && Lef[i - 2] - Lef[i] < 8 && Lef[i - 4] - Lef[i - 2] < 8 && Lef[i - 2] - Lef[i] > 0 && Lef[i - 4] - Lef[i - 2] > 0)
        //     {
        //         Road24_count++;
        //         if (Road24_count > 1)
        //         {
        //             Road24_count = 0;
        //             Road2_flag = 4;
        //             break;
        //         }
        //     }
        //     else
        //     {
        //         Road24_count = 0;
        //         break;
        //     }
        // }
        /////////////////////////////
        // if (Road24_count == (int)(DIS_IN_CIRCLE * 10000 / (SpeedGoal * CAMERA_FPS)) + 1)
        // {
        //     Road24_count = 0;
        //     Road2_flag = 4;
        // }
    }
    else if (Road2_flag == 4)
    {
        for (int i = 40; i > 20 && dis == 0; --i)
        {
            dis = Lef[i] - Lef[i + 1];
        }
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
                if (i == Fir_row + 1)
                {
                    turn_stop = i - 1;
                }
                continue;
            }
            else if (dis1 <= 2 * dis + 1)
            {
                dis = dis1;
                if (i == Fir_row + 1)
                {
                    turn_stop = i - 1;
                }
                continue;
            }
            else
            {
                turn_stop = i;
                break;
            }
        }

        if (Allwhitestart > 25 && Allwhitestart < 40 && Allwhitestart - Allwhiteend > 2)
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
        if (Allwhitestart >= 40 && Allwhiteend < 30)
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
        if (Allwhitestart >= 40)
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
        if (Lef_slope > 0.2) //0.15 || (Lef_slope > 0.1 && (Allwhiteend > 43 || Allwhiteend == Fir_row))) //DEBUG chongxintiao
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
        // if (EM_Value_1 < 0.6 && EM_Value_4 < 0.6 && Rig_edge < 10)
        if (Rig_edge < 10)
        {
            Road27_count++;
            if (Road27_count > 1)
            {
                Road = 0;
                Road0_flag = 0;
                Road2_flag = 0;
                // road_change_flag = 1;
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

    //static int start_count = 0;
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
                Road3_flag = 0;
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
        if (Lef_slope > 0.25 && Lef_slope != 999)
        {
            Road32_count++;
            if (Road32_count > 2)
            {
                Road32_count = 0;
                Road = 0;
                Road0_flag = 0;
            }
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
    static uint8 Road4_count0 = 0;
    static uint8 Road4_count3 = 0;

    if (Road4_flag == 0)
    {
        //lib_speed_set(2.0);
        if (icm_gyro_y_angle < -8)
        {
            Road4_count0++;
            if (Road4_count0 > 2)
            {
                Road4_flag = 1;
                Road4_count0 = 0;
            }
        }
        else
        {
            Road4_count0 = 0;
        }
    }
    else if (Road4_flag == 1)
    {
        if (icm_gyro_y_angle > 9)
        {
            Road4_flag = 3;
        }
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
        if (icm_gyro_y_angle < 6)
        {
            Road4_count3++;
            if (Road4_count3 > 1)
            {
                Road4_count3 = 0;
                Road = 0;
                Road0_flag = 0;
                Road4_flag = 0;
                // road_change_flag = 1;
                icm_gyro_y_angle = 0;
            }
        }
    }
    else
    {
        Road4_count3 = 0;
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
    static int Road74_count = 0, Road75_count = 0;

    int Black_line = 0;
    if (Road7_flag == 0 || Road7_flag == 1) //�ȴ�ת��
    {
        for (int i = barn_line - 5; i < Last_row; i++)
        {
#ifdef TL2barn
            if ((Lef[i] - Fir_col < 5 && (Lef[i + 1] - Fir_col < 5 || Lef[i + 2] - Fir_col < 5)) ||
                (Lef[i - 1] - Lef[i + 2] > 15 && Lef[i - 1] - Lef[i + 1] > 15) ||
                (Pixle[i][3] + Pixle[i][4] + Pixle[i][5] == 3 && Pixle[i + 1][3] + Pixle[i + 1][4] + Pixle[i + 1][5] == 3 && Pixle[i + 2][3] + Pixle[i + 2][4] + Pixle[i + 2][5] == 3))
#endif
#ifdef TR2barn
                if ((Last_col - Rig[i] < 5 && (Last_col - Rig[i + 1] < 5 || Last_col - Rig[i + 2] < 5)) ||
                    (Rig[i + 2] - Rig[i - 1] > 15 && Rig[i + 1] - Rig[i - 1] > 15) ||
                    (Pixle[i][76] + Pixle[i][75] + Pixle[i][74] == 3 && Pixle[i + 1][76] + Pixle[i + 1][75] + Pixle[i + 1][74] == 3 && Pixle[i + 2][76] + Pixle[i + 2][75] + Pixle[i + 2][74] == 3))
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
        else if (barn_line < 25)
        {
            Road7_flag = 1;
        }
        else if (barn_line < 30)
        {
            Road7_flag = 2;
        }
        else
        {
            Road7_flag = 6; //��ʼת��
        }
    }
    else if (Road7_flag == 2 || Road7_flag == 6) //��ʼת��
    {
        //Road73_count++;
        // if (Road73_count >= (int)(10 * 100 / (get_speed() * CAMERA_FPS)) + 1)
        // {
        // Road7_flag = 3;
        // }
        // if (Lef_slope == 998 && Rig_slope == 998 && (Lef_edge < 7 && Rig_edge < 7) || (EM_Value_1 < 0.2 && EM_Value_2 < 0.15 && EM_Value_2 < 0.15 && EM_Value_4 < 0.2))
        if (Rig_slope < -0.2 && (EM_Value_1 < 0.2 && EM_Value_2 < 0.15 && EM_Value_2 < 0.15 && EM_Value_4 < 0.2))
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
        for (int i = Last_row - 2; i > Fir_row; i--)
        {
            if (Pixle[i][8] + Pixle[i][72] + Pixle[i][30] + Pixle[i][35] + Pixle[i][40] + Pixle[i][45] + Pixle[i][50] < 3)
            {
                Black_line++;
                if (Black_line > 2)
                {
                    stop_line = i + 2;
                    if (stop_line > 27) // || Road74_count >= (int)(30 * 100 / (get_speed() * CAMERA_FPS)) + 1)
                    {
                        Road74_count++;
                        if (Road74_count > 1)
                        {
                            Road7_flag = 4; //ͣ��
                            Road74_count = 0;
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
    else if (Road7_flag == 4)
    {
        if (fabs(CarSpeed1) < 0.1 && fabs(CarSpeed2) < 0.1)
        {
            Road75_count++;
            if (Road75_count > 9)
            {
                Road = 7;
                Road7_flag = 5;
                Road75_count = 0;
                return;
            }
        }
        else
        {
            Road75_count = 0;
        }
    }
    return;
}

void Pic_find_circle_pre(void)
{
    int break_sum = 0;
    Lef_circle_pre_flag = 0;
    Rig_circle_pre_flag = 0;

    for (int i = Fir_row; i < Last_row - 30; ++i) //30=20+10
    {
        if (Lef[i + 1] > 5 && Lef[i + 3] > 5 && Lef[i + 5] > 5 && Lef[i + 7] < 30 &&
            Lef[i + 1] - Lef[i + 3] < 5 && Lef[i + 3] - Lef[i + 5] < 10 && Lef[i + 5] - Lef[i + 7] < 15 &&
            // Lef[i + 5] - Lef[i + 7] > 5 &&
            Lef[i + 5] - Lef[i + 6] > 2 && Lef[i + 6] - Lef[i + 7] > 2 &&
            Lef[i + 1] - Lef[i + 3] >= 0 && Lef[i + 3] - Lef[i + 5] > 0 && Lef[i + 5] - Lef[i + 7] > 0)
        {
            //31-39>=6���Ѳ��������
            for (int j = i + 8; j < i + 18; j++)
            {
                if (Lef[j] < Fir_col + 5)
                {
                    break_sum++;
                }
            }
            if (break_sum >= 5) //�����������������������ж����������������Ƿ���Ѳ������ߣ��ұ����Ƿ�ֱ
            {
                break_sum = 0;
                Lef_circle_pre_flag = 1;
            }
            return;
        }
    }
    break_sum = 0;
    for (int i = Fir_row; i < Last_row - 30; ++i) //30=20+10
    {
        if (Rig[i + 3] < 75 && Rig[i + 1] < 75 && Rig[i + 5] < 75 && Rig[i + 7] > 50 &&
            Rig[i + 3] - Rig[i + 1] < 5 && Rig[i + 5] - Rig[i + 3] < 10 && Rig[i + 7] - Rig[i + 5] < 15 &&
            // Rig[i + 7] - Rig[i + 5] > 5 &&
            Rig[i + 7] - Rig[i + 6] > 2 && Rig[i + 6] - Rig[i + 5] > 2 &&
            Rig[i + 3] - Rig[i + 1] >= 0 && Rig[i + 5] - Rig[i + 3] > 0 && Rig[i + 7] - Rig[i + 5] > 0)
        {
            //31-39>=6���Ѳ��������
            for (int j = i + 8; j < i + 18; j++)
            {
                if (Rig[j] > Last_col - 5)
                {
                    break_sum++;
                }
            }
            if (break_sum >= 5) //�����������������������ж����������������Ƿ���Ѳ������ߣ��ұ����Ƿ�ֱ
            {
                break_sum = 0;
                Rig_circle_pre_flag = 1;
            }
            return;
        }
    }
}
bool crossing_flag_flag;
uint8 whiteline_start;
uint8 whiteline_end; //�����start��end��֮ǰ���Ƿ��ģ�����endԶ��start
void crossing_find2(void)
{
    whiteline_start = 0;
    whiteline_end = 0;
    crossing_flag_flag = 0;
    int white_line_threshold[40] = {70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 58, 56, 54, 52, 50, 48, 46, 44, 42, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40};
    // int road_half_width_original[40] = {40, 40, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4};

    uint8 count;
    uint8 count2; //��ÿ���������ȫ�׵�
    uint8 whiteline_start_temp = 0;
    uint8 whitecol_start_temp = 0;
    uint8 whitecol_start = 0;
    uint8 whitecol_end = 0;

    uint8 whiteline_count = 0;
    uint8 whiteline_count2 = 0; //���������ȫ����
    uint8 whitecol_count = 0;
    uint8 whitecol_count2 = 0; //���������ȫ����
    bool whiteline_flag;
    for (int i = Fir_row; i < Last_row - 12; ++i)
    {
        count2 = 0;
        count = 0;
        for (int j = Fir_col; j < Last_col; ++j)
        {
            if (Pixle[i][j] || Pixle[i][j + 1])
            {
                count++;
            }
            else
            {
                if (count > count2)
                {
                    count2 = count;
                }
                count = 0;
            }
        }
        if (count > count2)
        {
            count2 = count;
        }
        if (count2 > white_line_threshold[59 - i])
        {
            if (whiteline_start_temp == 0)
            {
                whiteline_start_temp = i;
            }
            whiteline_count++;
        }
        else
        {
            if (whiteline_count > whiteline_count2)
            {
                whiteline_count2 = whiteline_count;
                whiteline_count = 0;
                whiteline_start = whiteline_start_temp;
                whiteline_end = i;
                whiteline_start_temp = 0;
            }
        }
    }
    if (whiteline_count > whiteline_count2)
    {
        whiteline_count2 = whiteline_count;
        whiteline_count = 0;
        whiteline_start = whiteline_start_temp;
        whiteline_end = Last_row - 12;
        whiteline_start_temp = 0;
    }
    // if (whiteline_start < 45)
    // {
    //     ;
    // }
    // else
    // {
    //     return;
    // }

    for (int j = Fir_col; j < Last_col; ++j)
    {
        count2 = 0;
        count = 0;
        for (int i = Fir_row; i < Last_row; ++i)
        {
            if (Pixle[i][j] || Pixle[i + 1][j])
            {
                count++;
            }
            else
            {
                if (count > count2)
                {
                    count2 = count;
                }
                count = 0;
            }
        }
        if (count > count2)
        {
            count2 = count;
        }
        if (count2 > 35)
        {
            whitecol_start_temp = j;
            whitecol_count++;
        }
        else
        {
            if (whitecol_count > whitecol_count2)
            {
                whitecol_count2 = whitecol_count;
                whitecol_count = 0;
                // whitecol_start = whitecol_start_temp;
                // whitecol_end = i;
            }
        }
    }
    if (whitecol_count2 > 4 && whiteline_count2 > 4 && whiteline_end < 40)
    {
        crossing_flag_flag = 1;
    }
}
