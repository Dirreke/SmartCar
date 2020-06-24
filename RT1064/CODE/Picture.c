#include "Picture.h"
// #include "math.h"
uint8 Image_Use[CAMERA_H][CAMERA_W];
uint16 Pixle[CAMERA_H][CAMERA_W]; //二值化后用于OLED显uint16氖???//u16

int threshold_offset = 0;
int threshold_offset2 = 0;
int threshold_offset3 = -10;

int whitecnt = 0;

int Lef[LCDH]; //道路左分离点的纵坐标
int Rig[LCDH]; //道路右分离点的纵坐标
int Mid[LCDH]; //道路中心点的纵坐标

int New_Lef[60];
int New_Rig[60]; //用于存储逆透视变换后的横坐标
int New_Mid[60];

int Lef_circle_point = 0;
int Rig_circle_point = 0;

int Lef_edge = 0, Rig_edge = 0;

float Lef_slope = 0, Rig_slope = 0;

__ramfunc void Get_Use_Image(void)
{
    int i = 0, j = 0;
    int row = 0;

    for (i = 20; i < 80; i += 1) //60行
    {
        for (j = 15; j < 174; j += 2) //188，取94列
        {
            Image_Use[row][(j - 15) >> 1] = mt9v03x_csi_image[i][j];
        }
        row += 1;
    }
}

/***************************************************************
*
* 函数名称：uint8_t GetOSTU(void)
* 功能说明：求阈值大小
* 参数说明：
* 函数返回：阈值大小
* 修改时间：2020年6月21日
* 备 注：
参考：https://blog.csdn.net/zyzhangyue/article/details/45841255
      https://www.cnblogs.com/moon1992/p/5092726.html
      https://www.cnblogs.com/zhonghuasong/p/7250540.html
Ostu方法又名最大类间差方法，通过统计整个图像的直方图特性来实现全局阈值T的自动选取，其算法步骤为：
1) 先计算图像的直方图，即将图像所有的像素点按照0~255共256个bin，统计落在每个bin的像素点数量
2) 归一化直方图，也即将每个bin中像素点数量除以总的像素点
3) i表示分类的阈值，也即一个灰度级，从0开始迭代
4) 通过归一化的直方图，统计0~i 灰度级的像素(假设像素值在此范围的像素叫做前景像素) 所占整幅图像的比例w0，并统计前景像素的平均灰度u0；统计i~255灰度级的像素(假设像素值在此范围的像素叫做背景像素) 所占整幅图像的比例w1，并统计背景像素的平均灰度u1；
5) 计算前景像素和背景像素的方差 g = w0*w1*(u0-u1) (u0-u1)
6) i++；转到4)，直到i为256时结束迭代
7）将最大g相应的i值作为图像的全局阈值
缺陷:OSTU算法在处理光照不均匀的图像的时候，效果会明显不好，因为利用的是全局像素信息。

改：
1. 计算直方图时滤掉最小、最大灰度像素较少的类别，更加均匀
2. 过亮的点（反光点）置为1中搜索到的像素点较多的最大灰度-offset（若其灰度更大），避免sobel将其周边置黑
3. TODO 反光点密集且多时失效
4. TODO 反光点不亮但旁边较暗时失效
***************************************************************/
uint8_t GetOSTU(void)
{
    int16_t i, j;
    uint32_t Amount = 0;
    uint32_t PixelBack = 0;
    uint32_t PixelIntegralBack = 0;
    uint32_t PixelIntegral = 0;
    int32_t PixelIntegralFore = 0;
    int32_t PixelFore = 0;
    double OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // 类间方差;
    int16_t MinValue, MaxValue;
    uint8_t Threshold = 0;
    uint16 HistoGram[256]; //
    int16_t Mincount = 0, Maxcount = 0;

    for (j = 0; j < 256; j++)
    {
        HistoGram[j] = 0; //初始化灰度直方图
    }

    for (j = START_LINE; j < CAMERA_H; j++)
    {
        for (i = 0; i < CAMERA_W; i++)
        {
            HistoGram[Image_Use[j][i]]++; //统计灰度级中每个像素在整幅图像中的个数
        }
    }

    //获取最小灰度的值
    for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] <= 5; MinValue++)
    {
        Mincount += HistoGram[MinValue];
        HistoGram[MinValue] = 0;
    }
    //获取最大灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistoGram[MaxValue] <= 15; MaxValue--)
    {
        Maxcount += HistoGram[MaxValue];
        HistoGram[MaxValue] = 0;
    }
    //滤除反光点
    for (j = START_LINE; j < CAMERA_H; j++)
    {
        for (i = 1; i < CAMERA_W; i++)
        {
            if (Image_Use[j][i] > MaxValue - 8)
            {
                Image_Use[j][i] = MaxValue - 8;
            }
        }
    }

    HistoGram[MaxValue] += Maxcount;
    HistoGram[MinValue] += Mincount;
    if (MaxValue == MinValue)
    {
        return MaxValue; // 图像中只有一个颜色
    }

    if (MinValue + 1 == MaxValue)
    {
        return MinValue; // 图像中只有二个颜色
    }

    for (j = MinValue; j <= MaxValue; j++)
    {
        Amount += HistoGram[j]; //  像素总数
    }

    PixelIntegral = 0;
    for (j = MinValue; j <= MaxValue; j++)
    {
        PixelIntegral += HistoGram[j] * j; //灰度值总数
    }

    SigmaB = -1;

    for (j = MinValue; j < MaxValue; j++)
    {
        PixelBack = PixelBack + HistoGram[j];                                              //前景像素点数
        PixelFore = Amount - PixelBack;                                                    //背景像素点数
        OmegaBack = (double)PixelBack / Amount;                                            //前景像素百分比
        OmegaFore = (double)PixelFore / Amount;                                            //背景像素百分比
        PixelIntegralBack += HistoGram[j] * j;                                             //前景灰度值
        PixelIntegralFore = PixelIntegral - PixelIntegralBack;                             //背景灰度值
        MicroBack = (double)PixelIntegralBack / PixelBack;                                 //前景灰度百分比
        MicroFore = (double)PixelIntegralFore / PixelFore;                                 //背景灰度百分比
        Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore); //计算类间方差
        if (Sigma > SigmaB)                                                                //遍历最大的类间方差g //找出最大类间方差以及对应的阈值
        {
            SigmaB = Sigma;
            Threshold = j;
        }
    }
    return Threshold; //返回最佳阈值;
}

void sobel(void) //Sobel边沿检测
{

    int tempx = 0, tempy = 0, i = 0, j = 0;
    double tempsqrt = 0;
    uint8 threshold;
    uint8 Sobel_Threshold;
    int threshold_temp;
    threshold = GetOSTU();
    for (i = Fir_row; i < LCDH - 1; i++)
    {
        for (j = 1; j < LCDW - 1; j++)
        {
            if (j < 11 || j > 69)
            {
                threshold_temp = threshold + threshold_offset + threshold_offset2 + threshold_offset3;
            }
            else
            {
                threshold_temp = threshold + threshold_offset + threshold_offset2;
            }
            if (Image_Use[i][j] < threshold_temp)
            {
                Pixle[i][j] = 0;
                continue;
            }

            tempx = -Image_Use[i - 1][j - 1] - 2 * Image_Use[i][j - 1] - Image_Use[i + 1][j - 1] + Image_Use[i - 1][j + 1] + 2 * Image_Use[i][j + 1] + Image_Use[i + 1][j + 1];

            tempy = Image_Use[i + 1][j - 1] + 2 * Image_Use[i + 1][j] + Image_Use[i + 1][j + 1] - Image_Use[i - 1][j - 1] - 2 * Image_Use[i - 1][j] - Image_Use[i - 1][j + 1];

            tempsqrt = sqrt(tempx * tempx + tempy * tempy);

            if (i < Sobel_Far_FarFar_Line)
            {
                Sobel_Threshold = Sobel_Threshold_FarFar;
            }
            else if (i < Sobel_Near_Far_Line)
            {
                Sobel_Threshold = Sobel_Threshold_Far;
            }
            else
            {
                Sobel_Threshold = Sobel_Threshold_Near;
            }
            if (tempsqrt > Sobel_Threshold && Image_Use[i][j] < 200) //i < Sobel_Near_Far_Line &&
            {
                Pixle[i][j] = 0;
                // if (j == 40)
                // FINAL[i] = 0;
            }
            else
            {
                Pixle[i][j] = 1;
                // if (j == 40)
                // FINAL[i] = 1;
            }
        }
    }
}

/*************************************************************************
*  函数名称：void Pic_noi_elim()
*  功能说明：照片噪点消除
*  参数说明：无
*  函数返回：无
*  修改时间：2019.2.20
*  备    注：某点三面相反视为噪点
*************************************************************************/
void Pic_noi_elim(void)
{
    int nr; //行
    int nc; //列
    whitecnt = 0;
    for (nr = Fir_row; nr < LCDH - 1; nr++)
    {
        for (nc = 1; nc < LCDW - 1; nc = nc + 1)
        {
            if ((Pixle[nr][nc] == 0) && (Pixle[nr - 1][nc] + Pixle[nr + 1][nc] + Pixle[nr][nc + 1] + Pixle[nr][nc - 1] > 2))
            {
                Pixle[nr][nc] = 1;
            }
            else if ((Pixle[nr][nc] == 1) && (Pixle[nr - 1][nc] + Pixle[nr + 1][nc] + Pixle[nr][nc + 1] + Pixle[nr][nc - 1] < 2))
            {
                Pixle[nr][nc] = 0;
            }
            if (Pixle[nr][nc] == 1)
                whitecnt += 1;
        }
    }
}

/*************************************************************************
*  函数名称：void Pic_DrawLRside(void)
*  功能说明：绘制左右边线线
*  参数说明：返回符号数，正表示应右转，负表示应左转
*  函数返回：中心线前置区域内的均值与预设值的偏差
*  修改时间：2020.06.19
*  备    注：寻找底层黑白跳变点，逐层向上搜索每行的跳变点。(向两边搜点、范围搜点两种方法）
             将整幅图的左右5列置黑
             每一行只检测两个跳变点。
             ////然后利用求平均值绘制中心线，单写
*************************************************************************/

void Pic_DrawLRside(void)
{
    const int Middle = 41;
    int i = 0, j = 0;
    bool search_flag1 = 0, search_flag2 = 0;
    bool Side_flag;
    bool Side_true = 0;
    Lef_circle_point = 0;
    Rig_circle_point = 0;
    for (i = Fir_row; i < LCDH; i++)
    {
        Rig[i] = 78;
        Lef[i] = 1;
        for (j = 0; j < Fir_col; j++)
        {
            Pixle[i][j] = 0;
        }
        for (j = Last_col + 1; j < LCDW; j++)
        {
            Pixle[i][j] = 0;
        }
    }

    for (i = Last_row; i > Last_row - 5; i--)
    {

        for (j = Middle; j < Last_col; j++)
        {
            if (Pixle[i][j] == 1 && Pixle[i][j - 1] == 1 && Pixle[i][j - 2] == 1 && Pixle[i][j - 3] == 1 && Pixle[i][j - 4] == 1 && Pixle[i][j - 5] == 1 && Pixle[i][j - 6] == 1 && Pixle[i][j + 1] == 0 && Pixle[i][j + 2] == 0 && Pixle[i][j + 3] == 0)
            {
                Rig[i] = j;
                break;
            }
        }
        for (j = Middle; j > Fir_col; j--)
        {
            if (Pixle[i][j] == 1 && Pixle[i][j + 1] == 1 && Pixle[i][j + 2] == 1 && Pixle[i][j + 3] == 1 && Pixle[i][j + 4] == 1 && Pixle[i][j + 5] == 1 && Pixle[i][j + 6] == 1 && Pixle[i][j - 1] == 0 && Pixle[i][j - 2] == 0 && Pixle[i][j - 3] == 0)
            {
                Lef[i] = j;
                break;
            }
        }
        if (Rig[i] != 78 && Lef[i] != 1)
        {
            --i;
            break;
        }
    }

    for (; i > Fir_row - 1; i--) //从底层向上绘线
    {
        search_flag1 = 0;
        search_flag2 = 0;
        Side_flag = 0;

        if (Rig[i + 1] < 77)
        {
            if (Pixle[i][Rig[i + 1]] == 0 || (Pixle[i][Rig[i + 1]] == 1 && Pixle[i][Rig[i + 1] + 1] == 0)) //向内查找10个
            {
                for (j = Rig[i + 1]; j > Rig[i + 1] - 10 && j > Lef[i + 1] + 5; j--)
                {
                    if (Pixle[i][j] == 1 && Pixle[i][j - 1] == 1) //两白
                    {
                        Rig[i] = j;
                        Side_flag = 1;
                        break;
                    }
                }
                search_flag1 = 1;
            }
            else if (Pixle[i][Rig[i + 1] + 1] == 1) //向外查找8个
            {
                for (j = Rig[i + 1] + 1; j < Rig[i + 1] + 8 && j < Last_col + 1; j++)
                {
                    if (Pixle[i][j + 1] == 0 && Pixle[i][j + 2] == 0) //两黑
                    {
                        Rig[i] = j;
                        Side_flag = 1;
                        break;
                    }
                }
                search_flag2 = 1;
            }
        }
        else if (Rig[i + 2] < 77) //更严格的条件
        {
            for (j = Rig[i + 2]; j > Rig[i + 1] - 10 && j > Lef[i + 1] + 5; j--) //先搜内10个
            {
                if (Pixle[i][j + 1] = 0 && Pixle[i][j] == 1 && Pixle[i][j - 1] == 1 && Pixle[i][j - 2] == 1 && Pixle[i][j - 3] == 1)
                {
                    Rig[i] = j;
                    Side_flag = 1;
                    break;
                }
                search_flag1 = 1;
            }
            if (Side_flag == 0 && Pixle[i][Rig[i + 2] - 2] == 1 && Pixle[i][Rig[i + 2] - 1] == 1 && Pixle[i][Rig[i + 2]] == 1 && Pixle[i][Rig[i + 2] + 1] == 1) //搜外8个
            {
                for (j = Rig[i + 2] + 1; j < Rig[i + 2] + 8 && j < Last_col + 1; j++)
                {
                    if (Pixle[i][j + 1] == 0 && Pixle[i][j + 2] == 0 && Pixle[i][j + 3] == 0)
                    {
                        Rig[i] = j;
                        Side_flag = 1;
                        break;
                    }
                }
                search_flag2 = 1;
            }
        }
        if (Side_flag == 0) //若没有找到跳变点，则放宽范围进行搜索
        {

            for (j = Lef[i + 1] + 5; j <= Rig[i + 1] - 10; j++)
            {
                if (Pixle[i][j] == 1 && Pixle[i][j - 1] == 1 && Pixle[i][j - 2] == 1 && Pixle[i][j - 3] == 1 && Pixle[i][j - 4] == 1 && Pixle[i][j - 5] == 1 && Pixle[i][j - 6] == 1 && Pixle[i][j + 1] == 0 && Pixle[i][j + 2] == 0 && Pixle[i][j + 3] == 0)
                {
                    Rig[i] = j;
                    Side_flag = 1;
                    break;
                }
            }
        }
        if (Side_flag == 0) //若没有找到跳变点，则放宽范围进行搜索
        {
            if (search_flag1 == 0)
            {
                for (j = Rig[i + 1] - 9; j < Rig[i + 1] && j < Last_col; j++)
                {
                    if (Pixle[i][j] == 1 && Pixle[i][j - 1] == 1 && Pixle[i][j - 2] == 1 && Pixle[i][j - 3] == 1 && Pixle[i][j - 4] == 1 && Pixle[i][j - 5] == 1 && Pixle[i][j - 6] == 1 && Pixle[i][j + 1] == 0 && Pixle[i][j + 2] == 0 && Pixle[i][j + 3] == 0)
                    {
                        Rig[i] = j;
                        Side_flag = 1;
                        break;
                    }
                }
            }
        }
        if (Side_flag == 0) //若没有找到跳变点，则放宽范围进行搜索
        {
            if (search_flag2 == 0)
            {
                for (j = Rig[i + 1]; j < Rig[i + 1] + 8 && j < Last_col; j++)
                {
                    if (Pixle[i][j] == 1 && Pixle[i][j - 1] == 1 && Pixle[i][j - 2] == 1 && Pixle[i][j - 3] == 1 && Pixle[i][j - 4] == 1 && Pixle[i][j - 5] == 1 && Pixle[i][j - 6] == 1 && Pixle[i][j + 1] == 0 && Pixle[i][j + 2] == 0 && Pixle[i][j + 3] == 0)
                    {
                        Rig[i] = j;
                        Side_flag = 1;
                        break;
                    }
                }
            }
        }
        if (Side_flag == 0) //若没有找到跳变点，则放宽范围进行搜索
        {
            for (j = Rig[i + 1] + 8; j < Last_col; j++)
            {
                if (Pixle[i][j] == 1 && Pixle[i][j - 1] == 1 && Pixle[i][j - 2] == 1 && Pixle[i][j - 3] == 1 && Pixle[i][j - 4] == 1 && Pixle[i][j - 5] == 1 && Pixle[i][j - 6] == 1 && Pixle[i][j + 1] == 0 && Pixle[i][j + 2] == 0 && Pixle[i][j + 3] == 0)
                {
                    Rig[i] = j;
                    Side_flag = 1;
                    break;
                }
            }
        }

        search_flag1 = 0;
        search_flag2 = 0;
        Side_flag = 0;

        if (Lef[i + 1] > 2)
        {
            if (Pixle[i][Lef[i + 1]] == 0 || (Pixle[i][Lef[i + 1]] == 1 && Pixle[i][Lef[i + 1] - 1] == 0))
            {
                for (j = Lef[i + 1]; j < Lef[i + 1] + 10 && j < Rig[i + 1] - 5; j++)
                {
                    if (Pixle[i][j] == 1 && Pixle[i][j + 1] == 1)
                    {
                        Lef[i] = j;
                        Side_flag = 1;
                        break;
                    }
                }
                search_flag1 = 1;
            }
            else if (Pixle[i][Lef[i + 1] - 1] == 1)
            {
                for (j = Lef[i + 1] - 1; j > Lef[i + 1] - 8 && j > Fir_col - 1; j--)
                {
                    if (Pixle[i][j - 1] == 0 && Pixle[i][j - 2] == 0)
                    {
                        Lef[i] = j;
                        Side_flag = 1;
                        break;
                    }
                }
                search_flag2 = 1;
            }
        }
        else if (Lef[i + 2] > 2)
        {
            for (j = Lef[i + 2]; j < Lef[i + 1] + 10 && j < Rig[i + 1] + 5; j++)
            {
                if (Pixle[i][j - 1] == 0 && Pixle[i][j] == 1 && Pixle[i][j + 1] == 1 && Pixle[i][j + 2] == 1 && Pixle[i][j + 3] == 1)
                {
                    Lef[i] = j;
                    Side_flag = 1;
                    break;
                }
                search_flag1 = 1;
            }
            if (Side_flag == 0 && Pixle[i][Lef[i + 2] + 2] == 1 && Pixle[i][Lef[i + 2] + 1] == 1 && Pixle[i][Lef[i + 2]] == 1 && Pixle[i][Lef[i + 2] - 1] == 1)
            {
                for (j = Lef[i + 2] - 1; j > Lef[i + 2] - 8 && j > Fir_col - 1; j--)
                {
                    if (Pixle[i][j - 1] == 0 && Pixle[i][j - 2] == 0 && Pixle[i][j - 3] == 0)
                    {
                        Lef[i] = j;
                        Side_flag = 1;
                        break;
                    }
                }
                search_flag2 = 1;
            }
        }

        if (Side_flag == 0) //若没有找到跳变点，则放宽范围进行搜索
        {
            for (j = Rig[i + 1] - 5; j >= Lef[i + 1] + 10; j--)
            {
                if (Pixle[i][j] == 1 && Pixle[i][j + 1] == 1 && Pixle[i][j + 2] == 1 && Pixle[i][j + 3] == 1 && Pixle[i][j + 4] == 1 && Pixle[i][j + 5] == 1 && Pixle[i][j + 6] == 1 && Pixle[i][j - 1] == 0 && Pixle[i][j - 2] == 0 && Pixle[i][j - 3] == 0)
                {
                    Lef[i] = j;
                    Side_flag = 1;
                    break;
                }
            }
        }
        if (Side_flag == 0) //若没有找到跳变点，则放宽范围进行搜索
        {
            if (search_flag1 == 0)
            {
                for (j = Lef[i + 1] + 9; j > Lef[i + 1] && j > Fir_col; j--)
                {
                    if (Pixle[i][j] == 1 && Pixle[i][j + 1] == 1 && Pixle[i][j + 2] == 1 && Pixle[i][j + 3] == 1 && Pixle[i][j + 4] == 1 && Pixle[i][j + 5] == 1 && Pixle[i][j + 6] == 1 && Pixle[i][j - 1] == 0 && Pixle[i][j - 2] == 0 && Pixle[i][j - 3] == 0)
                    {
                        Lef[i] = j;
                        Side_flag = 1;
                        break;
                    }
                }
            }
        }
        if (Side_flag == 0) //若没有找到跳变点，则放宽范围进行搜索
        {
            if (search_flag2 == 0)
            {
                for (j = Lef[i + 1]; j > Lef[i + 1] - 8 && j > Fir_col; j--)
                {
                    if (Pixle[i][j] == 1 && Pixle[i][j + 1] == 1 && Pixle[i][j + 2] == 1 && Pixle[i][j + 3] == 1 && Pixle[i][j + 4] == 1 && Pixle[i][j + 5] == 1 && Pixle[i][j + 6] == 1 && Pixle[i][j - 1] == 0 && Pixle[i][j - 2] == 0 && Pixle[i][j - 3] == 0)
                    {
                        Lef[i] = j;
                        Side_flag = 1;
                        break;
                    }
                }
            }
        }
        if (Side_flag == 0) //若没有找到跳变点，则放宽范围进行搜索
        {
            for (j = Lef[i + 1] - 8; j > Fir_col; j--)
            {
                if (Pixle[i][j] == 1 && Pixle[i][j + 1] == 1 && Pixle[i][j + 2] == 1 && Pixle[i][j + 3] == 1 && Pixle[i][j + 4] == 1 && Pixle[i][j + 5] == 1 && Pixle[i][j + 6] == 1 && Pixle[i][j - 1] == 0 && Pixle[i][j - 2] == 0 && Pixle[i][j - 3] == 0)
                {
                    Lef[i] = j;
                    Side_flag = 1;
                    break;
                }
            }
        }

        if (Lef[i] >= Rig[i])
        {
            Lef[i] = 1;
            Rig[i] = 78;
        }
    }

    if (Road1_flag == 1 || Road1_flag == 2)
    {
        // for (i = Fir_row; i < Last_row - 4 && Lef[i] - Fir_col < 2 && Lef[i] - Lef[i + 2] < 5 && Lef[i + 2] - Lef[i + 4] < 5; ++i)
        // {
        //     ;
        // }
        // if (i == Last_row - 4)
        // {
        //     return;
        // }
        // ++i;
        // for (; i < Last_row; ++i)
        // {
        //     if (Lef[i - 1] > 2)
        //     {

        //     }
        // }
        for (i = Fir_row; i < Last_row - 28; ++i)
        {
            if (Lef[i] - Fir_col > 10 || Lef[i + 1] - Fir_col > 10)
            {
                continue;
            }
            for (j = Lef[i]; j < 40; j++)
            {
                if (Pixle[i][j] == 1 && Pixle[i][j - 1] == 1 && Pixle[i][j - 2] == 1 && Pixle[i][j - 3] == 1 && Pixle[i][j - 4] == 1 && Pixle[i][j - 5] == 1 && Pixle[i][j - 6] == 1 && Pixle[i][j + 1] == 0 && Pixle[i][j + 2] == 0 && Pixle[i][j + 3] == 0)
                {
                    Rig[i] = j;
                    Side_true = 1;
                    break;
                }
            }
            if (Side_true == 1)
            {
                Side_true = 0;
                for (j = Lef[i + 1]; j < 40; j++)
                {
                    if (Pixle[i + 1][j] == 1 && Pixle[i + 1][j - 1] == 1 && Pixle[i + 1][j - 2] == 1 && Pixle[i + 1][j - 3] == 1 && Pixle[i + 1][j - 4] == 1 && Pixle[i + 1][j - 5] == 1 && Pixle[i + 1][j - 6] == 1 && Pixle[i + 1][j + 1] == 0 && Pixle[i + 1][j + 2] == 0 && Pixle[i + 1][j + 3] == 0)
                    {
                        Rig[i + 1] = j;
                        Side_true = 1;
                        break;
                    }
                }
            }

            if (Side_true && Rig[i + 1] - Rig[i] < 10)
            {
                Side_true = 1;
                break;
            }
            else
            {
                Side_true = 0;
            }
        }
        if (Side_true == 1)
        {
            i += 2;
            for (; i < Last_row; ++i)
            {
                for (j = Rig[i - 1]; j < Rig[i - 1] + 10; ++j)
                {
                    if (Pixle[i][j + 1] == 0 && Pixle[i][j + 2] == 0) //两Hei
                    {
                        Rig[i] = j;
                        Lef_circle_point = i;
                        break;
                    }
                }
                if (j == Rig[i - 1] + 10)
                {
                    break;
                }
            }
        }
    }
    Side_true = 0;
    if (Road2_flag == 1 || Road2_flag == 2)
    {
        for (i = Fir_row; i < Last_row - 28; ++i)
        {
            if (Last_col - Rig[i] > 10 || Last_col - Rig[i + 1] > 10)
            {
                continue;
            }
            for (j = Rig[i]; j > 40; j--)
            {
                if (Pixle[i][j] == 1 && Pixle[i][j + 1] == 1 && Pixle[i][j + 2] == 1 && Pixle[i][j + 3] == 1 && Pixle[i][j + 4] == 1 && Pixle[i][j + 5] == 1 && Pixle[i][j + 6] == 1 && Pixle[i][j - 1] == 0 && Pixle[i][j - 2] == 0 && Pixle[i][j - 3] == 0)
                {
                    Lef[i] = j;
                    Side_true = 1;
                    break;
                }
            }
            if (Side_true == 1)
            {
                Side_true = 0;
                for (j = Rig[i + 1]; j > 40; j--)
                {
                    if (Pixle[i + 1][j] == 1 && Pixle[i + 1][j + 1] == 1 && Pixle[i + 1][j + 2] == 1 && Pixle[i + 1][j + 3] == 1 && Pixle[i + 1][j + 4] == 1 && Pixle[i + 1][j + 5] == 1 && Pixle[i + 1][j + 6] == 1 && Pixle[i + 1][j - 1] == 0 && Pixle[i + 1][j - 2] == 0 && Pixle[i + 1][j - 3] == 0)
                    {
                        Lef[i + 1] = j;
                        Side_true = 1;
                        break;
                    }
                }
            }

            if (Side_true && Lef[i] - Lef[i + 1] < 10)
            {
                Side_true = 1;
                break;
            }
            else
            {
                Side_true = 0;
            }
        }
        if (Side_true == 1)
        {
            i += 2;
            for (; i < Last_row; ++i)
            {
                for (j = Lef[i - 1]; j > Lef[i - 1] - 10; --j)
                {
                    if (Pixle[i][j - 1] == 0 && Pixle[i][j - 2] == 0) //两Hei
                    {
                        Lef[i] = j;
                        Rig_circle_point = i;
                        break;
                    }
                }
                if (j == Lef[i - 1] - 10)
                {
                    break;
                }
            }
        }
    }
}
/*************************************************************************
 *  函数名称：void Pic_undistort(int L, int R)
 *  功能说明：图像去畸变
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020.05.30
 *  备    注：对Lef、Rig进行映射处理
 * **********************************************************************/
void Pic_undistort(int L, int R)
{
    int i = 0;
    int j = 0;
    int const startpoint = 29;
    int tempy;
    int tempx;
    int tempNewxR[60];
    int tempNewxL[60];
    float temp;
    int step;
    int Rig_New[60];
    int Lef_New[60];
    static const int tempNewy[60] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 261, 217, 184, 158, 138, 121, 107, 96, 86, 77, 70, 63, 57, 52, 47, 43, 39, 35, 32, 29, 26, 24, 21, 19, 17, 15, 13, 12, 10, 8, 7, 6, 4, 3, 2, 1, 0, 0};

    /*************************映射*******************************/
    if (R)
    {
        for (i = startpoint; i < 59; i++)
        {
            tempy = i;
            tempx = Rig[i];
            if (tempx >= Last_col)
                tempNewxR[i] = MIDMAP;
            else
                tempNewxR[i] = (int)(UNDISTORT_XPK * (UNDISTORT_D * UNDISTORT_C + UNDISTORT_H) * 2 * (tempx - 39.5) / (-UNDISTORT_S * (29.5 - tempy) + UNDISTORT_D * UNDISTORT_C) + 0.5);

            if (tempNewxR[i] > MIDMAP)
                tempNewxR[i] = MIDMAP;
        }
    }
    if (L)
    {
        for (i = startpoint; i < 59; i++)
        {
            tempy = i;
            tempx = Lef[i];
            if (tempx <= Fir_col)
                tempNewxL[i] = -MIDMAP;
            else
                tempNewxL[i] = (int)(UNDISTORT_XPK * (UNDISTORT_D * UNDISTORT_C + UNDISTORT_H) * 2 * (tempx - 39.5) / (-UNDISTORT_S * (29.5 - tempy) + UNDISTORT_D * UNDISTORT_C) + 0.5);

            if (tempNewxL[i] < -MIDMAP)
                tempNewxL[i] = -MIDMAP;
        }
    }
    /************************插值+压缩+倒序*************************/
    i = startpoint;
    j = 59; //59，不补最远行，58，补最远行需+下面两行代码
    // Rig_New[0] = tempNewxR[0];
    // Lef_New[0] = tempNewxL[0];
    while (j >= 0)
    {
        temp = UNDISTORT_PYK * j;
        if (tempNewy[i] >= temp && tempNewy[i + 1] <= temp)
        {
            step = tempNewy[i] - tempNewy[i + 1];
            if (R)
            {
                if (tempNewxR[i] != MIDMAP && tempNewxR[i + 1] != MIDMAP)
                {
                    Rig_New[59 - j] = (int)((tempNewxR[i] - tempNewxR[i + 1]) * (temp - tempNewy[i + 1]) / step + tempNewxR[i + 1] + 0.5);
                }
                else
                {
                    Rig_New[59 - j] = MIDMAP;
                }
            }
            if (L)
            {
                if (tempNewxL[i] != -MIDMAP && tempNewxL[i + 1] != -MIDMAP)
                {
                    Lef_New[59 - j] = (int)((tempNewxL[i] - tempNewxL[i + 1]) * (temp - tempNewy[i + 1]) / step + tempNewxL[i + 1] + 0.5);
                }
                else
                {
                    Lef_New[59 - j] = -MIDMAP;
                }
            }
            j--;
        }
        else
        {
            i++;
        }
    }
    /************************滤波*************************/

    if (L)
    {

        for (i = 0; i < 60; i++)
        {
            New_Lef[i] = Lef_New[i];
        }
        i = 0;
        while (i < 57)
        {
            if (Lef_New[i + 2] == -MIDMAP)
            {
                i = i + 3;
                continue;
            }
            else if (Lef_New[i + 1] == -MIDMAP)
            {
                i = i + 2;
                continue;
            }
            else if (Lef_New[i] == -MIDMAP)
            {
                i = i + 1;
                continue;
            }

            New_Lef[i + 1] = (Lef_New[i] + Lef_New[i + 1] + Lef_New[i + 2]) / 3;
            if (Lef_New[i + 3] == -MIDMAP)
            {
                i = i + 4;
                continue;
            }

            while (i < 57 && Lef_New[i + 3] != -MIDMAP)
            {

                New_Lef[i + 2] = (Lef_New[i] + Lef_New[i + 1] + Lef_New[i + 2] + Lef_New[i + 3]) / 4;
                i = i + 1;
            }
            i = i + 4;
        }
    }
    if (R)
    {
        for (i = 0; i < 60; i++)
        {
            New_Rig[i] = Rig_New[i];
        }
        i = 0;
        while (i < 57)
        {
            if (Rig_New[i + 2] == MIDMAP)
            {
                i = i + 3;
                continue;
            }
            else if (Rig_New[i + 1] == MIDMAP)
            {
                i = i + 2;
                continue;
            }
            else if (Rig_New[i] == MIDMAP)
            {
                i = i + 1;
                continue;
            }

            New_Rig[i + 1] = (Rig_New[i] + Rig_New[i + 1] + Rig_New[i + 2]) / 3;
            if (Rig_New[i + 3] == MIDMAP)
            {
                i = i + 4;
                continue;
            }

            while (i < 57 && Rig_New[i + 3] != MIDMAP)
            {

                New_Rig[i + 2] = (Rig_New[i] + Rig_New[i + 1] + Rig_New[i + 2] + Rig_New[i + 3]) / 4;
                i = i + 1;
            }
            i = i + 4;
        }
    }
}

/*************************************************************************
*  函数名称：void Pic_particular()
*  功能说明：图像特殊处理
*  参数说明：无
*  函数返回：无
*  修改时间：2020.06.17
*  备    注：
*************************************************************************/

void Pic_particular(void)
{
    int i;
    Lef_edge = 0;
    Rig_edge = 0;
    for (i = 59; i > Fir_row; i--)
    {
        if (Lef[i] - Fir_col < 3)
            Lef_edge += 1;
        if (Last_col - Rig[i] < 3)
            Rig_edge += 1;
    }
    // for (i = 59; i > 0; i--) //平均值法中心线绘制
    // {
    //   Mid[i] = (int)((New_Lef[i] + New_Rig[i]) / 20) + 40;
    // }
}

/*************************************************************************
*  函数名称：void Slope_fig()
*  功能说明：赛道左右斜率计算
*  参数说明：无
*  函数返回：无
*  修改时间：2019.3.21
*  备    注：最小二乘法拟合FIG_AREA区域内不为0的斜率

*************************************************************************/

void LR_Slope_fig()
{
    int i;
    float xsum = 0, ysum = 0, xysum = 0, x2sum = 0, count = 0;
    Lef_slope = 0;
    Rig_slope = 0;
    int max = -800, min = 0;
    for (i = 0; i < 60; i++)
    {
        if (i <= FIG_AREA_NEAR && i >= FIG_AREA_FAR && New_Lef[i] != -MIDMAP)
        {
            if (New_Lef[i] > max)
            {
                max = New_Lef[i];
            }
            if (New_Lef[i] < min)
            {
                min = New_Lef[i];
            }
            xsum += New_Lef[i];
            ysum += i;
            xysum += New_Lef[i] * i;
            x2sum += New_Lef[i] * New_Lef[i];
            count++;
        }
    }
    if (abs(max - min) > 25)
    {
        if (count * x2sum - xsum * xsum)
        {
            Lef_slope = -(count * xysum - xsum * ysum) / (count * x2sum - xsum * xsum);
        }
        else
        {
            Lef_slope = 999;
        }
    }
    else
    {
        Lef_slope = 998;
    }
    max = 0;
    min = 800;
    xsum = 0;
    ysum = 0;
    xysum = 0;
    x2sum = 0;
    count = 0;
    for (i = 0; i < 60; i++)
    {
        if (i <= FIG_AREA_NEAR && i >= FIG_AREA_FAR && New_Rig[i] != MIDMAP)
        {
            if (New_Rig[i] > max)
            {
                max = New_Rig[i];
            }
            if (New_Rig[i] < min)
            {
                min = New_Rig[i];
            }
            xsum += New_Rig[i];
            ysum += i;
            xysum += New_Rig[i] * i;
            x2sum += New_Rig[i] * New_Rig[i];
            count++;
        }
    }
    if (abs(max - min) > 30)
    {
        if (count * x2sum - xsum * xsum)
        {
            Rig_slope = -(count * xysum - xsum * ysum) / (count * x2sum - xsum * xsum);
        }
        else
        {
            Rig_slope = 999;
        }
    }
    else
    {
        Rig_slope = 998;
    }
    //   LR_slope = L_R_Slope(1) + L_R_Slope(2);
}

/*************************************************************************
*  函数名称：float Slope(int F1x,int F1y,int F2x,int F2y)
*  功能说明：斜率函数
*  参数说明：无
*  函数返回：无
*  修改时间：2019.3.21
*  备    注：△y/△x

*************************************************************************/

float Slope(int F1x, int F1y, int F2x, int F2y)
{
    float slope = 0;
    if (F2y == F1y)
    {
        return 999;
    }
    slope = (F2y - F1y) * 1.0 / (F2x - F1x);
    return slope;
}

/*************************************************************************
*  函数名称：void Threshold_change(void)
*  功能说明：在特殊状态改变阈值偏置
*  参数说明：无
*  函数返回：无
*  修改时间：2020.05.31
*  备    注：
*************************************************************************/
void Threshold_change(void)
{
    if (Road == 0)
    {
        switch (Road0_flag)
        {
        case 1:
            threshold_offset2 = -8;
            break;
        case 2:
            threshold_offset2 = -15;
            break;
        case 4:
            threshold_offset2 = -5;
            break;
        case 5:
            threshold_offset2 = -5;
            break;
        default:
            threshold_offset2 = 0;
            break;
        }
    }
    else if (Road == 1)
    {
        // switch (Road1_flag)
        // {
        // case 4:
        //     threshold_offset2 = -5;
        //     break;
        //     // case 5:

        // case 3:
        //     threshold_offset2 = -5;
        //     break;
        // default:
        //     threshold_offset2 = 0;
        //     break;
        // }
        threshold_offset2 = -5;
    }
    else if (Road == 2)
    {
        // switch (Road2_flag)
        // {
        // case 4:
        //     threshold_offset2 = -5;
        //     break;
        // case 3:
        //     threshold_offset2 = -5;
        //     break;
        // default:
        //     threshold_offset2 = 0;
        //     break;
        // }
        threshold_offset2 = -5;
    }
}
#if 1
/*************************************************************************
*  函数名称：void Fix_line()
*  功能说明：岔路补线处理
*  参数说明：无
*  函数返回：无
*  修改时间：2020.06.23
*  备    注：十字：近远景布线，远景补线
            弯道：圆环弯道部分：将无用信息行（弯道以外）置1和78
            入环，分别按1/2和2/3计算斜率补线
            出环，按2计算斜率补线

*************************************************************************/

void Pic_Fix_Line(void)
{
    float slope;
    // float slope2;
    static float slope_static;
    // int i;
    // int j;
    // int k;
    int xtemp, ytemp, get_flag = 0;
    static int xtemp_static, ytemp_static;
    //static float stat_slope;
    //static float stat_slope2;
    //static char road1_flag1 = 1; //0表示已计算完进圆环斜率，1表示已经出圆环，再次进圆环时计算补线斜率
    //static char road2_flag1 = 1;
    //bool road1_flag2 = 0, road2_flag2 = 0;
    //static bool road1_flag3 = 0, road2_flag3 = 0;
    // static bool road1_flag4, road2_flag4;
    static bool Road_flag1 = 0, Road_flag2 = 0;
    if (Road == 0)
    {
        if (Road0_flag == 1)
        {
            for (int i = Fir_row; i < Allwhiteend; ++i)
            {
                if (Lef[i] <= Fir_col + 15)
                {
                    continue;
                }
                if (Lef[i] - Lef[i + 2] < 5 && Lef[i + 1] - Lef[i + 3] < 5 && Lef[i] - Lef[i + 2] > 0 && Lef[i + 1] - Lef[i + 3] > 0)
                {
                    xtemp = Lef[i];
                    ytemp = i;
                    get_flag = 1;
                    break;
                }
            }
            if (get_flag == 1)
            {
                for (int i = 55; i > Allwhitestart; i--)
                {
                    if (Lef[i] <= Fir_col)
                    {
                        continue;
                    }

                    if (Lef[i - 2] - Lef[i] < 5 && Lef[i - 3] - Lef[i - 1] < 5 && Lef[i - 2] - Lef[i] > 0 && Lef[i - 3] - Lef[i - 1] > 0)
                    {
                        slope = Slope(Lef[i], i, xtemp, ytemp); //Slope(int F1x,int F1y,int F2x,int F2y)
                        if (slope != 999)
                        {
                            for (int j = ytemp; j < 55; j++)
                            {
                                Lef[j] = (int)(Lef[i] - (i - j) / slope);
                            }
                            Pic_undistort(1, 0);
                            break;
                        }
                    }
                }
            }
            else
            {
                for (int i = 55; i > Allwhitestart; i--)
                {
                    if (abs(Lef[i] - Fir_col) < 5)
                    {
                        continue;
                    }
                    if (Lef[i - 3] - Lef[i - 1] < 5 && Lef[i - 5] - Lef[i - 3] < 5 && Lef[i - 3] - Lef[i - 1] > 0 && Lef[i - 5] - Lef[i - 3] > 0)
                    {
                        slope = Slope(Lef[i], i, Lef[i - 5], i - 5); //Slope(int F1x,int F1y,int F2x,int F2y)
                        if (slope != 999)
                        {
                            for (int j = i + 1; j > Fir_row + 5; j--)
                            {
                                Lef[j] = (int)(Lef[i] - (i - j) / slope);
                            }
                            Pic_undistort(1, 0);
                            break;
                        }
                    }
                }
            }
            get_flag = 0;
            for (int i = Fir_row; i < Allwhiteend; ++i)
            {
                if (Rig[i] >= Last_col - 15)
                {
                    continue;
                }
                if (Rig[i + 2] - Rig[i] < 5 && Rig[i + 3] - Rig[i + 1] < 5 && Rig[i + 2] - Rig[i] > 0 && Rig[i + 3] - Rig[i + 1] > 0)
                {
                    xtemp = Rig[i];
                    ytemp = i;
                    get_flag = 1;
                    break;
                }
            }
            if (get_flag == 1)
            {
                for (int i = 55; i > Allwhitestart; i--)
                {
                    if (Rig[i] >= Last_col)
                    {
                        continue;
                    }

                    if (Rig[i] - Rig[i - 2] < 5 && Rig[i - 1] - Rig[i - 3] < 5 && Rig[i] - Rig[i - 2] > 0 && Rig[i - 1] - Rig[i - 3] > 0)
                    {
                        slope = Slope(Rig[i], i, xtemp, ytemp); //Slope(int F1x,int F1y,int F2x,int F2y)
                        if (slope != 999)
                        {
                            for (int j = ytemp; j < 55; j++)
                            {
                                Rig[j] = (int)(Rig[i] - (i - j) / slope);
                            }
                            Pic_undistort(0, 1);
                            break;
                        }
                    }
                }
            }
            else
            {
                for (int i = 55; i > Allwhitestart; i--)
                {
                    if (Last_col - Rig[i] < 5)
                    {
                        continue;
                    }
                    if (Rig[i - 1] - Rig[i - 3] < 5 && Rig[i - 3] - Rig[i - 5] < 5 && Rig[i - 1] - Rig[i - 3] > 0 && Rig[i - 3] - Rig[i - 5] > 0)
                    {
                        slope = Slope(Rig[i], i, Rig[i - 5], i - 5); //Slope(int F1x,int F1y,int F2x,int F2y)
                        if (slope != 999)
                        {
                            for (int j = i + 1; j > Fir_row + 5; j--)
                            {
                                Rig[j] = (int)(Rig[i] - (i - j) / slope);
                            }
                            Pic_undistort(0, 1);
                            break;
                        }
                    }
                }
            }
        }
        else if (Road0_flag == 2)
        {
            for (int i = Fir_row; i < Allwhiteend; ++i)
            {
                if (Lef[i] <= Fir_col + 15)
                {
                    continue;
                }
                if (Lef[i] - Lef[i + 2] < 5 && Lef[i + 2] - Lef[i + 4] < 5 && Lef[i] - Lef[i + 2] > 0 && Lef[i + 2] - Lef[i + 4] > 0)
                {
                    xtemp = Lef[i];
                    ytemp = i;
                    get_flag = 1;
                    break;
                }
            }
            if (get_flag == 1)
            {
                for (int i = Allwhiteend - 1; i > ytemp; --i)
                {
                    if (Lef[i] - Fir_col < 5)
                    {
                        continue;
                    }
                    if (Lef[i - 2] - Lef[i] < 5 && Lef[i - 4] - Lef[i - 2] < 5 && Lef[i - 5] - Lef[i - 3] < 5 &&
                        Lef[i - 2] - Lef[i] > 0 && Lef[i - 4] - Lef[i - 2] > 0 && Lef[i - 5] - Lef[i - 3] > 0)
                    {
                        slope = Slope(Lef[i], i, xtemp, ytemp); //Slope(int F1x,int F1y,int F2x,int F2y)
                        if (slope != 999)
                        {
                            for (int j = i + 1; j < 55; j++)
                            {
                                Lef[j] = (int)(Lef[i] - (i - j) / slope);
                            }
                            Pic_undistort(1, 0);
                            break;
                        }
                    }
                }
            }
            // for (int i = Fir_row + 5; i < Allwhiteend; i++)
            // {
            //     if (abs(Lef[i] - Fir_col) < 15)
            //         continue;
            //     slope = Slope(Lef[i], i, 4, 54); //Slope(int F1x,int F1y,int F2x,int F2y)
            //     if (slope != 999)
            //     {
            //         for (int j = i + 1; j < 55; j++)
            //         {
            //             Lef[j] = (int)(Lef[i] - (i - j) / slope);
            //         }
            //         Pic_undistort(1, 0);
            //         break;
            //     }
            // }
            get_flag = 0;
            for (int i = Fir_row; i < Allwhiteend; ++i)
            {
                if (Rig[i] >= Last_col - 15)
                {
                    continue;
                }
                if (Rig[i + 2] - Rig[i] < 5 && Rig[i + 4] - Rig[i + 2] < 5 && Rig[i + 2] - Rig[i] > 0 && Rig[i + 4] - Rig[i + 2] > 0)
                {
                    xtemp = Rig[i];
                    ytemp = i;
                    get_flag = 1;
                    break;
                }
            }
            if (get_flag == 1)
            {
                for (int i = Allwhiteend - 1; i > ytemp; --i)
                {
                    if (Rig[i] >= Last_col - 5)
                    {
                        continue;
                    }
                    if (Rig[i] - Rig[i - 2] < 5 && Rig[i - 2] - Rig[i - 4] < 5 && Rig[i - 3] - Rig[i - 5] < 5 &&
                        Rig[i] - Rig[i - 2] > 0 && Rig[i - 2] - Rig[i - 4] > 0 && Rig[i - 3] - Rig[i - 5] > 0)
                    {
                        slope = Slope(Rig[i], i, xtemp, ytemp); //Slope(int F1x,int F1y,int F2x,int F2y)
                        if (slope != 999)
                        {
                            for (int j = i + 1; j < 55; j++)
                            {
                                Rig[j] = (int)(Rig[i] - (i - j) / slope);
                            }
                            Pic_undistort(0, 1);
                            break;
                        }
                    }
                }
            }

            // for (int i = Fir_row + 5; i < Allwhiteend; i++)
            // {
            //     if (abs(Rig[i] - Last_col) < 15)
            //         continue;
            //     slope = Slope(Rig[i], i, 75, 54); //Slope(int F1x,int F1y,int F2x,int F2y)
            //     if (slope != 999)
            //     {
            //         for (int j = i + 1; j < 55; j++)
            //         {
            //             Rig[j] = (int)(Rig[i] - (i - j) / slope);
            //         }
            //         Pic_undistort(0, 1);
            //         break;
            //     }
            // }
            return;
        }
        // else if (Road0_flag == 3)
        // {
        //     for (int i = 55; i > Fir_row + 15; i--)
        //     {
        //         if (abs(Lef[i] - Fir_col) < 5)
        //             continue;
        //         slope = Slope(Lef[i], i, Lef[Fir_row + 1], Fir_row + 1); //Slope(int F1x,int F1y,int F2x,int F2y)
        //         if (slope != 999 && slope < 0)
        //         {
        //             for (int j = i + 1; j > Fir_row + 1; j--)
        //             {
        //                 Lef[j] = (int)(Lef[i] - (i - j) / slope);
        //             }
        //             Pic_undistort(1, 0);
        //             break;
        //         }
        //     }
        //     for (int i = 55; i > Fir_row + 15; i--)
        //     {
        //         if (abs(Rig[i] - Last_col) < 5)
        //             continue;
        //         slope = Slope(Rig[i], i, Rig[Fir_row + 1], Fir_row + 1); //Slope(int F1x,int F1y,int F2x,int F2y)
        //         if (slope != 999 && slope > 0)
        //         {
        //             for (int j = i + 1; j > Fir_row + 5; j--)
        //             {
        //                 Rig[j] = (int)(Rig[i] - (i - j) / slope);
        //             }
        //             Pic_undistort(0, 1);
        //             break;
        //         }
        //     }

        //     return;
        // }
        else if (Road0_flag == 4)
        {
            for (int i = turn_stop; i >= Fir_row; --i)
            {
                Rig[i] = 78;
            }
            Pic_undistort(0, 1);
            return;
        }
        else if (Road0_flag == 5)
        {
            for (int i = turn_stop; i >= Fir_row; --i)
            {
                Lef[i] = 1;
            }
            Pic_undistort(1, 0);
            return;
        }
    }

    //左圆环补线处理
    else if (Road == 1)
    {
        if (Road1_flag == 1)
        {
            if (Road_flag1 == 0)
            {
                slope_static = 999;
                Road_flag1 = 1;
            }
            // road1_flag2 = 0;
            for (int i = Fir_row; i < Last_row - 20; i++)
            {
                if (Lef[i] - Fir_col < 2)
                {
                    continue;
                }
                if (Lef[i - 4] - Lef[i - 2] < 5 && Lef[i - 2] - Lef[i] < 5 && Lef[i] - Lef[i + 1] > 15 && Pixle[i + 2][Lef[i] - 5] == 1)
                {

                    slope_static = Slope(Lef[i], i, 79, 54);
                    xtemp_static = Lef[i];
                    ytemp_static = i;
                    // road1_flag2 = 1;
                    break;
                }
            }
            if (slope_static != 999)
            {
                for (int k = Fir_row + 3; k < ytemp_static; k++)
                {
                    Rig[k] = (int)(xtemp_static - (ytemp_static - k) / slope_static);
                }
                for (int k = ytemp_static; k < 55; k++)
                {
                    // Rig[k] = (int)((xtemp_static - (ytemp_static - k) / slope_static) / 2) + xtemp_static / 2;
                    Rig[k] = (int)((k - ytemp_static) / slope_static / 3) + xtemp_static;
                }

                Pic_undistort(0, 1);
            }
        }

        else if (Road1_flag == 2)
        {
            Road_flag1 = 0;
            if (Road_flag2 == 0)
            {
                slope_static = 999;
                Road_flag2 = 1;
            }
            if (Lef_circle_point != 0)
            {
                slope_static = Slope(Rig[Lef_circle_point], Lef_circle_point, 79, 54);
                xtemp_static = Rig[Lef_circle_point];
                ytemp_static = Lef_circle_point;
            }
            if (slope_static != 999)
            {
                for (int k = ytemp_static; k < 55; k++)
                {
                    // Rig[k] = (int)((xtemp_static - (ytemp_static - k) / slope_static )* 2 / 3) + xtemp_static / 3;
                    Rig[k] = (int)((k - ytemp_static) * 2 / slope_static / 3) + xtemp_static;
                }
                Pic_undistort(0, 1);
            }

            // for (int i = Last_row - 13; i > Fir_row; i--)
            // {
            //     if (road1_flag1)
            //     {
            //         if (abs(Lef[i] - Fir_col) < 25)
            //             continue;
            //         stat_slope = Slope(Lef[i], i, 75, 54);
            //         road1_flag1 = 0;
            //     }

            //     if (stat_slope != 999)
            //     {
            //         for (int k = Fir_row + 3; k < 55; k++)
            //         {
            //             Rig[k] = (int)(75 - (54 - k) / stat_slope);
            //         }
            //         Pic_undistort(0, 1);
            //         break;
            //     }
            // }
        }
        else if (Road1_flag == 4)
        {
            Road_flag2 = 0;
            //road1_flag3 = 0;
            //road1_flag4 = 0;
            // road1_flag1 = 1; //表示已经出圆环，再次进圆环时计算补线斜率
            for (int i = turn_stop; i >= Fir_row; --i)
            {
                Rig[i] = 78;
            }
            Pic_undistort(0, 1);
        }
        else if (Road1_flag == 3)
        {
            for (int j = Last_row - 3; j > Fir_row; j--)
            {

                if (abs(Rig[j] - Last_col) < 2 || Rig[j] - Rig[j - 2] >= 5 || Rig[j - 2] - Rig[j - 4] >= 5 || Rig[j - 4] - Rig[j - 6] >= 5 || Rig[j] - Rig[j - 2] < 0 || Rig[j - 2] - Rig[j - 4] < 0 || Rig[j - 4] - Rig[j - 6] < 0)
                    continue;
                slope = Slope(Rig[j - 1], j - 1, Rig[j - 6], j - 6);

                if (slope != 999)
                {
                    for (int k = j - 7; k > Fir_row + 5; k--)
                    {
                        // Rig[k] = (int)(Rig[j] - (j - k) / slope);
                        // Rig[k] = (int)((Rig[j - 6] - (j - 6 - k) / slope) * 2) - Rig[j - 6];
                        Rig[k] = (int)((k + 6 - j) * 2 / slope) + Rig[j - 6];
                    }
                    Pic_undistort(0, 1);
                    break;
                }
            }
        }
        // else if (Road1_flag == 5)
        // {
        //   ;
        // }
    }
    else if (Road == 2)
    {
        if (Road2_flag == 1)
        {
            if (Road_flag1 == 0)
            {
                slope_static = 999;
                Road_flag1 = 1;
            }
            // road2_flag2 = 0;
            for (int i = Fir_row; i < Last_row - 20; i++)
            {
                if (Last_col - Rig[i] < 2)
                {
                    continue;
                }
                if (Rig[i] - Rig[i - 2] < 5 && Rig[i - 2] - Rig[i - 4] < 5 && Rig[i + 1] - Rig[i] > 15 && Pixle[i + 2][Rig[i] + 5] == 1)
                {
                    slope_static = Slope(Rig[i], i, 0, 54);
                    xtemp_static = Rig[i];
                    ytemp_static = i;
                    // road2_flag2 = 1;
                    // road2_flag3 = 1;
                    break;
                }
            }
            // if (Rig_circle_point == 0)
            // {
            //     road2_flag2 = 1;
            // }
            if (slope_static != 999)
            {
                for (int k = Fir_row + 3; k < ytemp_static; k++)
                {
                    Lef[k] = (int)(xtemp_static - (ytemp_static - k) / slope_static);
                }
                for (int k = ytemp_static; k < 55; k++)
                {

                    Lef[k] = (int)((k - ytemp_static) / slope_static / 2) + xtemp_static;
                }
                Pic_undistort(1, 0);
            }
        }
        else if (Road2_flag == 2)
        {
            Road_flag1 = 0;
            if (Road_flag2 == 0)
            {
                slope_static = 999;
                Road_flag2 = 1;
            }
            if (Rig_circle_point != 0)
            {
                slope_static = Slope(Lef[Rig_circle_point], Rig_circle_point, 1, 54);
                xtemp_static = Lef[Rig_circle_point];
                ytemp_static = Rig_circle_point;
            }

            if (slope_static != 999)
            {
                for (int k = ytemp_static; k < 55; k++)
                {
                    // Lef[k] = (int)(4 - (54 - k) / slope_static);
                    Lef[k] = (int)((k - ytemp_static) * 2 / slope_static / 3) + xtemp_static;
                }
                Pic_undistort(1, 0);
            }

            // for (int i = Last_row - 13; i > Fir_row; i--)
            // {
            //     if (road2_flag1)
            //     {
            //         if (abs(Rig[i] - Last_col) < 25)
            //             continue;
            //         stat_slope2 = Slope(Rig[i], i, 4, 54);
            //         road2_flag1 = 0;
            //     }

            //     if (stat_slope2 != 999)
            //     {
            //         for (int k = Fir_row + 3; k < 55; k++)
            //         {
            //             Lef[k] = (int)(15 - (57 - k) / stat_slope2);
            //         }
            //         Pic_undistort(1, 0);
            //         break;
            //     }
            // }
        }
        else if (Road2_flag == 4)
        {
            Road_flag2 = 0;
            // road2_flag3 = 0;
            // road2_flag4 = 0;
            // road2_flag1 = 1;
            for (int i = turn_stop; i >= Fir_row; --i)
            {
                Lef[i] = 1;
            }
            Pic_undistort(1, 0);
        }
        else if (Road2_flag == 3)
        {
            for (int j = Last_row - 3; j > Fir_row; j--)
            {

                if (abs(Lef[j] - Fir_col) < 2 || Lef[j - 2] - Lef[j] >= 5 || Lef[j - 4] - Lef[j - 2] >= 5 || Lef[j - 6] - Lef[j - 4] >= 5 || Lef[j - 2] - Lef[j] < 0 || Lef[j - 4] - Lef[j - 2] < 0 || Lef[j - 6] - Lef[j - 4] < 0)
                    continue;
                slope = Slope(Lef[j - 1], j - 1, Lef[j - 6], j - 6);

                if (slope != 999)
                {
                    for (int k = j - 6; k > Fir_row + 5; k--)
                    {
                        // Lef[k] = (int)(Lef[j] - (j - k) / slope2);
                        Lef[k] = (int)((k + 6 - j) * 2 / slope) - Lef[j - 6];
                    }
                    Pic_undistort(1, 0);
                    break;
                }
            }
        }
        // else if (Road2_flag == 5)
        // {
        //   ;
        // }
    }
    else if (Road == 7)
    {
        if (Road7_flag == 0)
        {
            get_flag = 0;
            for (int i = Fir_row + 2; i < start_stop_line; i++)
            {
                if (abs(Lef[i] - Fir_col) < 15 || Lef[i] - Lef[i + 2] > 5 || Lef[i] - Lef[i + 2] < 0)
                    continue;
                xtemp = Lef[i];
                ytemp = i;
                get_flag = 1;
                break;
            }
            if (get_flag == 1)
            {

                for (int i = start_stop_line + 5; i < Last_row; i++)
                {
                    if (abs(Lef[i] - Fir_col) < 4 || Lef[i] - Lef[i + 2] > 5 || Lef[i] - Lef[i + 2] < 0)
                        continue;

                    slope = Slope(Lef[i], i, xtemp, ytemp); //Slope(int F1x,int F1y,int F2x,int F2y)
                    if (slope != 999)
                    {
                        for (int j = ytemp; j < 55; j++)
                        {
                            Lef[j] = (int)(Lef[i] - (i - j) / slope);
                        }
                        Pic_undistort(1, 0);
                        break;
                    }
                }
            }

            get_flag = 0;
            for (int i = Fir_row + 2; i < start_stop_line; i++)
            {
                if (abs(Rig[i] - Last_col) < 15 || Rig[i + 2] - Rig[i] > 5 || Rig[i + 2] - Rig[i] < 0)
                    continue;
                if (Rig[i] < 40)
                    break;
                xtemp = Rig[i];
                ytemp = i;
                get_flag = 1;
                break;
            }
            if (get_flag == 1)
            {
                for (int i = start_stop_line + 5; i < Last_row; i++)
                {
                    if (abs(Rig[i] - Fir_col) < 4 || Rig[i] < 40 || Rig[i + 2] - Rig[i] > 5 || Rig[i + 2] - Rig[i] < 0)
                        continue;

                    slope = Slope(Rig[i], i, xtemp, ytemp); //Slope(int F1x,int F1y,int F2x,int F2y)
                    if (slope != 999)
                    {
                        for (int j = ytemp; j < 55; j++)
                        {
                            Rig[j] = (int)(Rig[i] - (i - j) / slope);
                        }
                        Pic_undistort(0, 1);
                        break;
                    }
                }
            }
        }
        else if (Road7_flag == 1)
        {

            // get_flag = 0;
            for (int i = Fir_row + 5; i < start_stop_line; i++)
            {
                if (abs(Lef[i] - Fir_col) < 15 || Lef[i] - Lef[i + 2] > 5 || Lef[i] - Lef[i + 2] < 0)
                    continue;
                // xtemp = Lef[i];
                // ytemp = i;
                slope = Slope(4, 54, Lef[i], i); //Slope(int F1x,int F1y,int F2x,int F2y)
                if (slope != 999)
                {
                    for (int j = ytemp; j < 55; j++)
                    {
                        Lef[j] = (int)(Lef[i] - (i - j) / slope);
                    }
                    Pic_undistort(1, 0);
                    break;
                }
                // get_flag = 1;
            }
            // if (get_flag == 1)
            // {

            //   for (; i < start_stop_line; i++)
            //   {
            //     if (abs(Lef[i] - Fir_col) > 15 && Lef[i] - Lef[i + 2] < 5 && Lef[i] - Lef[i + 2] > 0)
            //       continue;

            // slope = Slope(Lef[i-1], i-1, xtemp, ytemp); //Slope(int F1x,int F1y,int F2x,int F2y)
            //     break;
            //   }
            // }

            // get_flag = 0;
            for (int i = Fir_row + 5; i < start_stop_line; i++)
            {
                if (abs(Rig[i] - Last_col) < 15 || Rig[i + 2] - Rig[i] > 5 || Rig[i + 2] - Rig[i] < 0)
                    continue;
                if (Rig[i] < 40)
                    break;
                // xtemp = Rig[i];
                // ytemp = i;
                slope = Slope(75, 54, Rig[i], i); //Slope(int F1x,int F1y,int F2x,int F2y)
                if (slope != 999)
                {
                    for (int j = ytemp; j < 55; j++)
                    {
                        Rig[j] = (int)(Rig[i] - (i - j) / slope);
                    }
                    Pic_undistort(0, 1);
                    break;
                }
                // get_flag = 1;
            }

            // if (get_flag == 1)
            // {
            //   for (; i < start_stop_line; i++)
            //   {
            //     if (abs(Rig[i] - Fir_col) > 15 && Rig[i] > 40 && Rig[i+2] - Rig[i] < 5 && Rig[i+2] - Rig[i] > 0)
            // continue;

            //     slope = Slope(Rig[i-1], i-1, xtemp, ytemp); //Slope(int F1x,int F1y,int F2x,int F2y)
            //     break;
            //   }
            // }
            //
        }
    }
    fangyuejie();
}

#endif
#if 0
/*************************************************************************
*  函数名称：void Fix_line()
*  功能说明：岔路补线处理
*  参数说明：无
*  函数返回：无
*  修改时间：2020.06.20
*  备    注：3.6日：主要补线体现在十字路口偏离中心道路时将左右两边路补线，
             在普通直道与弯道不进行补线（通过平滑与否与十字区分开）

*************************************************************************/

void Pic_Fix_Line(void)
{
    float slope;
    float slope2;
    static float slope_static;
    // int i;
    // int j;
    // int k;
    int xtemp, ytemp, get_flag = 0;
    //static float stat_slope;
    //static float stat_slope2;
    //static char road1_flag1 = 1; //0表示已计算完进圆环斜率，1表示已经出圆环，再次进圆环时计算补线斜率
    //static char road2_flag1 = 1;
    //bool road1_flag2 = 0, road2_flag2 = 0;
    //static bool road1_flag3 = 0, road2_flag3 = 0;
    static bool road1_flag4, road2_flag4;
    if (Road == 0)
    {
        if (Road0_flag == 1)
        {
            for (int i = Fir_row; i < Allwhiteend; ++i)
            {
                if (Lef[i] <= Fir_col)
                {
                    continue;
                }
                if (40 - Lef[i] < 20 && Lef[i] - Lef[i + 2] < 5 && Lef[i + 1] - Lef[i + 3] < 5 && Lef[i] - Lef[i + 2] > 0 && Lef[i + 1] - Lef[i + 3] > 0)
                {
                    xtemp = Lef[i];
                    ytemp = i;
                    get_flag = 1;
                    break;
                }
            }
            if (get_flag == 1)
            {
                for (int i = 55; i > Allwhitestart; i--)
                {
                    if (Lef[i] <= Fir_col)
                    {
                        continue;
                    }

                    if (Lef[i - 2] - Lef[i] < 5 && Lef[i - 3] - Lef[i - 1] < 5 && Lef[i - 2] - Lef[i] > 0 && Lef[i - 3] - Lef[i - 1] > 0)
                    {
                        slope = Slope(Lef[i], i, xtemp, ytemp); //Slope(int F1x,int F1y,int F2x,int F2y)
                        if (slope != 999)
                        {
                            for (int j = ytemp; j < 55; j++)
                            {
                                Lef[j] = (int)(Lef[i] - (i - j) / slope);
                            }
                            Pic_undistort(1, 0);
                            break;
                        }
                    }
                }
            }
            else
            {
                for (int i = 55; i > Allwhitestart; i--)
                {
                    if (abs(Lef[i] - Fir_col) < 5)
                    {
                        continue;
                    }
                    if (Lef[i - 3] - Lef[i - 1] < 5 && Lef[i - 5] - Lef[i - 3] < 5 && Lef[i - 3] - Lef[i - 1] > 0 && Lef[i - 5] - Lef[i - 3] > 0)
                    {
                        slope = Slope(Lef[i], i, Lef[i - 5], i - 5); //Slope(int F1x,int F1y,int F2x,int F2y)
                        if (slope != 999)
                        {
                            for (int j = i + 1; j > Fir_row + 5; j--)
                            {
                                Lef[j] = (int)(Lef[i] - (i - j) / slope);
                            }
                            Pic_undistort(1, 0);
                            break;
                        }
                    }
                }
            }
            get_flag = 0;
            for (int i = Fir_row; i < Allwhiteend; ++i)
            {
                if (Rig[i] >= Last_col)
                {
                    continue;
                }
                if (Rig[i] - 40 < 20 && Rig[i + 2] - Rig[i] < 5 && Rig[i + 3] - Rig[i + 1] < 5 && Rig[i + 2] - Rig[i] > 0 && Rig[i + 3] - Rig[i + 1] > 0)
                {
                    xtemp = Rig[i];
                    ytemp = i;
                    get_flag = 1;
                    break;
                }
            }
            if (get_flag == 1)
            {
                for (int i = 55; i > Allwhitestart; i--)
                {
                    if (Rig[i] >= Last_col)
                    {
                        continue;
                    }

                    if (Rig[i] - Rig[i - 2] < 5 && Rig[i - 1] - Rig[i - 3] < 5 && Rig[i] - Rig[i - 2] > 0 && Rig[i - 1] - Rig[i - 3] > 0)
                    {
                        slope = Slope(Rig[i], i, xtemp, ytemp); //Slope(int F1x,int F1y,int F2x,int F2y)
                        if (slope != 999)
                        {
                            for (int j = ytemp; j < 55; j++)
                            {
                                Rig[j] = (int)(Rig[i] - (i - j) / slope);
                            }
                            Pic_undistort(0, 1);
                            break;
                        }
                    }
                }
            }
            else
            {
                for (int i = 55; i > Allwhitestart; i--)
                {
                    if (Last_col - Rig[i] < 5)
                    {
                        continue;
                    }
                    if (Rig[i - 1] - Rig[i - 3] < 5 && Rig[i - 3] - Rig[i - 5] < 5 && Rig[i - 1] - Rig[i - 3] > 0 && Rig[i - 3] - Rig[i - 5] > 0)
                    {
                        slope = Slope(Rig[i], i, Rig[i - 5], i - 5); //Slope(int F1x,int F1y,int F2x,int F2y)
                        if (slope != 999)
                        {
                            for (int j = i + 1; j > Fir_row + 5; j--)
                            {
                                Rig[j] = (int)(Rig[i] - (i - j) / slope);
                            }
                            Pic_undistort(0, 1);
                            break;
                        }
                    }
                }
            }
        }
        else if (Road0_flag == 2)
        {
            for (int i = Fir_row + 5; i < Allwhiteend; i++)
            {
                if (abs(Lef[i] - Fir_col) < 15)
                    continue;
                slope = Slope(Lef[i], i, 4, 54); //Slope(int F1x,int F1y,int F2x,int F2y)
                if (slope != 999)
                {
                    for (int j = i + 1; j < 55; j++)
                    {
                        Lef[j] = (int)(Lef[i] - (i - j) / slope);
                    }
                    Pic_undistort(1, 0);
                    break;
                }
            }
            for (int i = Fir_row + 5; i < Allwhiteend; i++)
            {
                if (abs(Rig[i] - Last_col) < 15)
                    continue;
                slope = Slope(Rig[i], i, 75, 54); //Slope(int F1x,int F1y,int F2x,int F2y)
                if (slope != 999)
                {
                    for (int j = i + 1; j < 55; j++)
                    {
                        Rig[j] = (int)(Rig[i] - (i - j) / slope);
                    }
                    Pic_undistort(0, 1);
                    break;
                }
            }
            return;
        }
        else if (Road0_flag == 3)
        {
            for (int i = 55; i > Fir_row + 15; i--)
            {
                if (abs(Lef[i] - Fir_col) < 5)
                    continue;
                slope = Slope(Lef[i], i, Lef[Fir_row + 1], Fir_row + 1); //Slope(int F1x,int F1y,int F2x,int F2y)
                if (slope != 999 && slope < 0)
                {
                    for (int j = i + 1; j > Fir_row + 1; j--)
                    {
                        Lef[j] = (int)(Lef[i] - (i - j) / slope);
                    }
                    Pic_undistort(1, 0);
                    break;
                }
            }
            for (int i = 55; i > Fir_row + 15; i--)
            {
                if (abs(Rig[i] - Last_col) < 5)
                    continue;
                slope = Slope(Rig[i], i, Rig[Fir_row + 1], Fir_row + 1); //Slope(int F1x,int F1y,int F2x,int F2y)
                if (slope != 999 && slope > 0)
                {
                    for (int j = i + 1; j > Fir_row + 5; j--)
                    {
                        Rig[j] = (int)(Rig[i] - (i - j) / slope);
                    }
                    Pic_undistort(0, 1);
                    break;
                }
            }

            return;
        }
        else if (Road0_flag == 4)
        {
            for (int i = turn_stop; i >= Fir_row; --i)
            {
                Rig[i] = 78;
            }
            Pic_undistort(0, 1);
            return;
        }
        else if (Road0_flag == 5)
        {
            for (int i = turn_stop; i >= Fir_row; --i)
            {
                Lef[i] = 1;
            }
            Pic_undistort(1, 0);
            return;
        }
    }

    //左圆环补线处理
    else if (Road == 1)
    {
        if (Road1_flag == 1)
        {
            if (road1_flag4 == 0)
            {
                slope_static = 999;
                road1_flag4 = 1;
            }
            // road1_flag2 = 0;
            for (int i = Fir_row; i < Last_row - 20; i++)
            {
                if (Lef[i] - Fir_col < 2)
                {
                    continue;
                }
                if (Lef[i - 4] - Lef[i - 2] < 5 && Lef[i - 2] - Lef[i] < 5 && Lef[i] - Lef[i + 1] > 15 && Pixle[i + 2][Lef[i] - 5] == 1)
                {

                    slope_static = Slope(Lef[i], i, 75, 54);
                    // road1_flag2 = 1;
                    break;
                }
            }
            if (slope_static != 999)
            {
                for (int k = Fir_row + 3; k < 55; k++)
                {
                    Rig[k] = (int)(75 - (54 - k) / slope_static);
                }
                Pic_undistort(0, 1);
            }
        }

        else if (Road1_flag == 2)
        {
            road1_flag4 = 0;
            slope = Slope(Rig[Lef_circle_point], Lef_circle_point, 75, 54);
            if (slope != 999)
            {
                for (int k = Lef_circle_point; k < 55; k++)
                {
                    Rig[k] = (int)(75 - (54 - k) / slope);
                }
                Pic_undistort(0, 1);
            }

            // for (int i = Last_row - 13; i > Fir_row; i--)
            // {
            //     if (road1_flag1)
            //     {
            //         if (abs(Lef[i] - Fir_col) < 25)
            //             continue;
            //         stat_slope = Slope(Lef[i], i, 75, 54);
            //         road1_flag1 = 0;
            //     }

            //     if (stat_slope != 999)
            //     {
            //         for (int k = Fir_row + 3; k < 55; k++)
            //         {
            //             Rig[k] = (int)(75 - (54 - k) / stat_slope);
            //         }
            //         Pic_undistort(0, 1);
            //         break;
            //     }
            // }
        }
        else if (Road1_flag == 4)
        {
            //road1_flag3 = 0;
            //road1_flag4 = 0;
            // road1_flag1 = 1; //表示已经出圆环，再次进圆环时计算补线斜率
            for (int i = turn_stop; i >= Fir_row; --i)
            {
                Rig[i] = 78;
            }
            Pic_undistort(0, 1);
        }
        else if (Road1_flag == 3)
        {
            for (int j = Last_row - 3; j > Fir_row; j--)
            {

                if (abs(Rig[j] - Last_col) < 2 || Rig[j] - Rig[j - 2] >= 5 || Rig[j - 2] - Rig[j - 4] >= 5 || Rig[j - 4] - Rig[j - 6] >= 5 || Rig[j] - Rig[j - 2] < 0 || Rig[j - 2] - Rig[j - 4] < 0 || Rig[j - 4] - Rig[j - 6] < 0)
                    continue;
                slope = Slope(Rig[j - 1], j - 1, Rig[j - 6], j - 6);

                if (slope != 999)
                {
                    for (int k = j + 1; k > Fir_row + 5; k--)
                    {
                        Rig[k] = (int)(Rig[j] - (j - k) / slope);
                    }
                    Pic_undistort(0, 1);
                    break;
                }
            }
        }
        // else if (Road1_flag == 5)
        // {
        //   ;
        // }
    }
    else if (Road == 2)
    {
        if (Road2_flag == 1)
        {
            if (road2_flag4 == 0)
            {
                slope_static = 999;
                road2_flag4 = 1;
            }
            // road2_flag2 = 0;
            for (int i = Fir_row; i < Last_row - 20; i++)
            {
                if (Last_col - Rig[i] < 2)
                {
                    continue;
                }
                if (Rig[i] - Rig[i - 2] < 5 && Rig[i - 2] - Rig[i - 4] < 5 && Rig[i + 1] - Rig[i] > 15 && Pixle[i + 2][Rig[i] + 5] == 1)
                {
                    slope_static = Slope(Rig[i], i, 4, 54);
                    // road2_flag2 = 1;
                    // road2_flag3 = 1;
                    break;
                }
            }
            // if (Rig_circle_point == 0)
            // {
            //     road2_flag2 = 1;
            // }
            if (slope_static != 999)
            {
                for (int k = Fir_row + 3; k < 55; k++)
                {
                    Lef[k] = (int)(4 - (54 - k) / slope_static);
                }
                Pic_undistort(1, 0);
            }
        }
        else if (Road2_flag == 2)
        {
            road2_flag4 = 0;
            slope = Slope(Lef[Rig_circle_point], Rig_circle_point, 4, 54);
            if (slope != 999)
            {
                for (int k = Rig_circle_point; k < 55; k++)
                {
                    Lef[k] = (int)(4 - (54 - k) / slope);
                }
                Pic_undistort(1, 0);
            }

            // for (int i = Last_row - 13; i > Fir_row; i--)
            // {
            //     if (road2_flag1)
            //     {
            //         if (abs(Rig[i] - Last_col) < 25)
            //             continue;
            //         stat_slope2 = Slope(Rig[i], i, 4, 54);
            //         road2_flag1 = 0;
            //     }

            //     if (stat_slope2 != 999)
            //     {
            //         for (int k = Fir_row + 3; k < 55; k++)
            //         {
            //             Lef[k] = (int)(15 - (57 - k) / stat_slope2);
            //         }
            //         Pic_undistort(1, 0);
            //         break;
            //     }
            // }
        }
        else if (Road2_flag == 4)
        {
            // road2_flag3 = 0;
            // road2_flag4 = 0;
            // road2_flag1 = 1;
            for (int i = turn_stop; i >= Fir_row; --i)
            {
                Lef[i] = 1;
            }
            Pic_undistort(1, 0);
        }
        else if (Road2_flag == 3)
        {
            for (int j = Last_row - 3; j > Fir_row; j--)
            {

                if (abs(Lef[j] - Fir_col) < 2 || Lef[j - 2] - Lef[j] >= 5 || Lef[j - 4] - Lef[j - 2] >= 5 || Lef[j - 6] - Lef[j - 4] >= 5 || Lef[j - 2] - Lef[j] < 0 || Lef[j - 4] - Lef[j - 2] < 0 || Lef[j - 6] - Lef[j - 4] < 0)
                    continue;
                slope2 = Slope(Lef[j - 1], j - 1, Lef[j - 6], j - 6);

                if (slope2 != 999)
                {
                    for (int k = j + 1; k > Fir_row + 5; k--)
                    {
                        Lef[k] = (int)(Lef[j] - (j - k) / slope2);
                    }
                    Pic_undistort(1, 0);
                    break;
                }
            }
        }
        // else if (Road2_flag == 5)
        // {
        //   ;
        // }
    }
    else if (Road == 7)
    {
        if (Road7_flag == 0)
        {
            get_flag = 0;
            for (int i = Fir_row + 2; i < start_stop_line; i++)
            {
                if (abs(Lef[i] - Fir_col) < 15 || Lef[i] - Lef[i + 2] > 5 || Lef[i] - Lef[i + 2] < 0)
                    continue;
                xtemp = Lef[i];
                ytemp = i;
                get_flag = 1;
                break;
            }
            if (get_flag == 1)
            {

                for (int i = start_stop_line + 5; i < Last_row; i++)
                {
                    if (abs(Lef[i] - Fir_col) < 4 || Lef[i] - Lef[i + 2] > 5 || Lef[i] - Lef[i + 2] < 0)
                        continue;

                    slope = Slope(Lef[i], i, xtemp, ytemp); //Slope(int F1x,int F1y,int F2x,int F2y)
                    if (slope != 999)
                    {
                        for (int j = ytemp; j < 55; j++)
                        {
                            Lef[j] = (int)(Lef[i] - (i - j) / slope);
                        }
                        Pic_undistort(1, 0);
                        break;
                    }
                }
            }

            get_flag = 0;
            for (int i = Fir_row + 2; i < start_stop_line; i++)
            {
                if (abs(Rig[i] - Last_col) < 15 || Rig[i + 2] - Rig[i] > 5 || Rig[i + 2] - Rig[i] < 0)
                    continue;
                if (Rig[i] < 40)
                    break;
                xtemp = Rig[i];
                ytemp = i;
                get_flag = 1;
                break;
            }
            if (get_flag == 1)
            {
                for (int i = start_stop_line + 5; i < Last_row; i++)
                {
                    if (abs(Rig[i] - Fir_col) < 4 || Rig[i] < 40 || Rig[i + 2] - Rig[i] > 5 || Rig[i + 2] - Rig[i] < 0)
                        continue;

                    slope = Slope(Rig[i], i, xtemp, ytemp); //Slope(int F1x,int F1y,int F2x,int F2y)
                    if (slope != 999)
                    {
                        for (int j = ytemp; j < 55; j++)
                        {
                            Rig[j] = (int)(Rig[i] - (i - j) / slope);
                        }
                        Pic_undistort(0, 1);
                        break;
                    }
                }
            }
        }
        else if (Road7_flag == 1)
        {

            // get_flag = 0;
            for (int i = Fir_row + 5; i < start_stop_line; i++)
            {
                if (abs(Lef[i] - Fir_col) < 15 || Lef[i] - Lef[i + 2] > 5 || Lef[i] - Lef[i + 2] < 0)
                    continue;
                // xtemp = Lef[i];
                // ytemp = i;
                slope = Slope(4, 54, Lef[i], i); //Slope(int F1x,int F1y,int F2x,int F2y)
                if (slope != 999)
                {
                    for (int j = ytemp; j < 55; j++)
                    {
                        Lef[j] = (int)(Lef[i] - (i - j) / slope);
                    }
                    Pic_undistort(1, 0);
                    break;
                }
                // get_flag = 1;
            }
            // if (get_flag == 1)
            // {

            //   for (; i < start_stop_line; i++)
            //   {
            //     if (abs(Lef[i] - Fir_col) > 15 && Lef[i] - Lef[i + 2] < 5 && Lef[i] - Lef[i + 2] > 0)
            //       continue;

            // slope = Slope(Lef[i-1], i-1, xtemp, ytemp); //Slope(int F1x,int F1y,int F2x,int F2y)
            //     break;
            //   }
            // }

            // get_flag = 0;
            for (int i = Fir_row + 5; i < start_stop_line; i++)
            {
                if (abs(Rig[i] - Last_col) < 15 || Rig[i + 2] - Rig[i] > 5 || Rig[i + 2] - Rig[i] < 0)
                    continue;
                if (Rig[i] < 40)
                    break;
                // xtemp = Rig[i];
                // ytemp = i;
                slope = Slope(75, 54, Rig[i], i); //Slope(int F1x,int F1y,int F2x,int F2y)
                if (slope != 999)
                {
                    for (int j = ytemp; j < 55; j++)
                    {
                        Rig[j] = (int)(Rig[i] - (i - j) / slope);
                    }
                    Pic_undistort(0, 1);
                    break;
                }
                // get_flag = 1;
            }

            // if (get_flag == 1)
            // {
            //   for (; i < start_stop_line; i++)
            //   {
            //     if (abs(Rig[i] - Fir_col) > 15 && Rig[i] > 40 && Rig[i+2] - Rig[i] < 5 && Rig[i+2] - Rig[i] > 0)
            // continue;

            //     slope = Slope(Rig[i-1], i-1, xtemp, ytemp); //Slope(int F1x,int F1y,int F2x,int F2y)
            //     break;
            //   }
            // }
            //
        }
    }
    fangyuejie();
}
#endif
void fangyuejie(void)
{
    for (int i = Fir_row; i < Last_row; ++i)
    {
        if (Lef[i] < 1)
        {
            Lef[i] = 1;
        }
        if (Rig[i] < 1)
        {
            Rig[i] = 1;
        }
        if (Lef[i] > 78)
        {
            Lef[i] = 78;
        }
        if (Rig[i] > 78)
        {
            Rig[i] = 78;
        }
    }
}

/*************************************************************************
*  函数名称：void Pic_DrawMid(void)
*  功能说明：绘制左右边线线
*  参数说明：返回符号数，正表示应右转，负表示应左转
*  函数返回：中心线前置区域内的均值与预设值的偏差
*  修改时间：2020.05.31
*  备    注：寻找底层黑白跳变点，逐层向上搜索每行的跳变点。(向两边搜点、范围搜点两种方法）
             将整幅图的左右5列置黑
             每一行只检测两个跳变点。
             ////然后利用求平均值绘制中心线，单写
*************************************************************************/

void Pic_DrawMid(void)
{
    int i = 0;
    int road_half_width_original[40] = {40, 40, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4};
    if ((Road0_flag == 4 && Road == 0) || Road == 1)
    {
        for (i = Fir_row; i < Last_row + 1; i++)
        {

            if (Rig[i] < Last_col)
            {
                Mid[i] = Rig[i] - road_half_width_original[Last_row - i];
            }
            else
            {
                Mid[i] = 40;
            }
        }
    }
    else if ((Road0_flag == 5 && Road == 0) || Road == 2)
    {
        for (i = Fir_row; i < Last_row + 1; i++)
        {

            if (Lef[i] > Fir_col)
            {
                Mid[i] = Lef[i] + road_half_width_original[Last_row - i];
            }
            else
            {
                Mid[i] = 40;
            }
        }
    }
    else
    {
        for (i = Fir_row; i < Last_row + 1; i++)
        {

            if (Lef[i] > Fir_col && Rig[i] < Last_col) //Mid Calculaing
            {
                Mid[i] = (int)((Lef[i] + Rig[i]) / 2.0 + 0.5);
            }
            else if (Lef[i] <= Fir_col && Rig[i] < Last_col)
            {
                Mid[i] = Rig[i] - road_half_width_original[Last_row - i];
            }
            else if (Lef[i] > Fir_col && Rig[i] >= Last_col)
            {
                Mid[i] = Lef[i] + road_half_width_original[Last_row - i];
            }
            else
            {
                Mid[i] = 40;
            }
        }
    }
    return;
}
/*************************************************************************
 *  函数名称：void Pic_DrawMid_und(void)
 *  功能说明：计算去畸变后中线无插值
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020.5.31
 *  备    注：
 * ************************************************************************/

void Pic_DrawMid_und(void)
{
    int i;

    if ((Road0_flag == 4 && Road == 0) || Road == 1)
    {
        for (i = 0; i < 60; i++)
        {

            if (New_Rig[i] != MIDMAP)
            {
                New_Mid[i] = New_Rig[i] - ROAD_HALF_WIDTH;
            }
            else
            {
                New_Mid[i] = 999;
            }
        }
    }
    else if ((Road0_flag == 5 && Road == 0) || Road == 2)
    {
        for (i = 0; i < 60; i++)
        {

            if (New_Lef[i] != -MIDMAP)
            {
                New_Mid[i] = New_Lef[i] + ROAD_HALF_WIDTH;
            }
            else
            {
                New_Mid[i] = 999;
            }
        }
    }
    else
    {
        for (i = 0; i < 60; i++)
        {

            if (New_Lef[i] != -MIDMAP && New_Rig[i] != MIDMAP) //Mid Calculaing
            {
                New_Mid[i] = (int)((New_Lef[i] + New_Rig[i]) / 2.0 + 0.5);
            }
            else if (New_Lef[i] == -MIDMAP && New_Rig[i] != MIDMAP)
            {
                New_Mid[i] = New_Rig[i] - ROAD_HALF_WIDTH;
            }
            else if (New_Lef[i] != -MIDMAP && New_Rig[i] == MIDMAP)
            {
                New_Mid[i] = New_Lef[i] + ROAD_HALF_WIDTH;
            }
            else
            {
                New_Mid[i] = 999;
            }
        }
    }
    return;
}

/*************************************************************************
*  函数名称：void Get_pic_with_edge()
*  功能说明：带边线灰度图
*  参数说明：无
*  函数返回：无
*  修改时间：2019.12.12
*  备    注：
*************************************************************************/
void Get_pic_with_edge()
{
    int i = 0;
    for (i = 0; i < 60; i++)
    {
        Image_Use[i][Lef[i]] = 0xFF;
        Image_Use[i][Mid[i]] = 0xFF;
        Image_Use[i][Rig[i]] = 0xFF;
    }
}

#if 0
/*************************************************************************
*  函数名称：void Pic_seedfill(void)
*  功能说明：DFS搜索种子填充算法
*  参数说明：无
*  函数返回：将Sobel边缘提取后的图像进行填充以及干扰的去除
*  修改时间：2019.5.27
*  备    注：子函数grow()为递归函数。
*************************************************************************/

void Pic_seedfill(void)
{
  uint8 flag[CAMERA_H][CAMERA_W] = {0};
  int i, j;
  int seed_flag = 0, seed_i = -1, seed_j = -1;

  //设定基准团(点)
  for (i = seed_down; i >= seed_up; i = i - 3)
  {
    for (j = seed_left; j <= seed_right; j = j + 3)
    {
      if (Pixle[i][j] == 1)
      {
        seed_flag = 1;
        seed_i = i;
        seed_j = j;
        break;
      }
    }
    if (seed_flag == 1)
      break;
  }
  if (seed_flag == 0) //没找到基准团,返回-1
    return;
  if (Lef_slope == 999 && Rig_slope < 900 && Road != 2)
  {
    seed_i = 55;
    seed_j = 10;
  }
  if (Rig_slope == 999 && Lef_slope < 900 && Road == 1)
  {
    seed_i = 55;
    seed_j = 10;
  }
  //生长填充
  Pic_seedfill_grow(flag, seed_i, seed_j);

  for (i = CAMERA_H - 1; i >= 0; i--)
    for (j = 0; j <= CAMERA_W - 1; j++)
    {
      Pixle[i][j] = flag[i][j];
      if (flag[i][j] == 2)
        Pixle[i][j] = 0;
      // 	Bondary[i][j]=1;
    }

  return;
}

void Pic_seedfill_grow(uint8 flag[CAMERA_H][CAMERA_W], int i, int j)
{
  //判定 深度优先搜索 方向下左右上 带边界条件
  int grow_i, grow_j;

  for (grow_i = i - 1; grow_i <= i + 1; grow_i++)
    for (grow_j = j - 1; grow_j <= j + 1; grow_j++)
      if (Pixle[grow_i][grow_j] == 1)
        flag[grow_i][grow_j] = 1;
      else
        flag[grow_i][grow_j] = 2; //2表明原图像为黑色0，且已搜过

  // if(Pixle[i][j]==1)
  // 	flag[i][j]=1;

  //down
  if (Pixle[i + 1][j] == 1 && Pixle[i + 2][j] == 1 && flag[i + 3][j] == 0 && i < BOTTOM - 3) //条件：1、九宫格下中心下一下二均为白 2、下三没搜过 3、下三小于边界  //会忽略掉最后一行或两行
    Pic_seedfill_grow(flag, i + 3, j);

  //left
  if (Pixle[i][j - 1] == 1 && Pixle[i][j - 2] == 1 && flag[i][j - 3] == 0 && j > LEFT + 3)
    Pic_seedfill_grow(flag, i, j - 3);
  //right
  if (Pixle[i][j + 1] == 1 && Pixle[i][j + 2] == 1 && flag[i][j + 3] == 0 && j < RIGHT - 3)
    Pic_seedfill_grow(flag, i, j + 3);
  //up
  if (Pixle[i - 1][j] == 1 && Pixle[i - 2][j] == 1 && flag[i - 3][j] == 0 && i > TOP + 3)
    Pic_seedfill_grow(flag, i - 3, j);

  return;
}

#endif