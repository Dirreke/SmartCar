#include "Picture.h"
#include "math.h"
uint8 Image_Use[CAMERA_H][CAMERA_W];
uint16 Pixle[CAMERA_H][CAMERA_W]; //二值化后用于OLED显uint16氖???//u16

int threshold_offset = -5;
int threshold_offset2 = 0;

int whitecnt = 0;

int Lef[LCDH]; //道路左分离点的纵坐标
int Rig[LCDH]; //道路右分离点的纵坐标
int Mid[LCDH]; //道路中心点的纵坐标

int New_Lef[60];
int New_Rig[60]; //用于存储逆透视变换后的横坐标
int New_Mid[60];

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
* 函数名称：uint8_t GetOSTU(uint8_t tmImage[IMAGEH][IMAGEW])
* 功能说明：求阈值大小
* 参数说明：
* 函数返回：阈值大小
* 修改时间：2018年3月27日
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
***************************************************************/
uint8_t GetOSTU(uint8_t tmImage[CAMERA_H][CAMERA_W])
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

    for (j = 0; j < 256; j++)
    {
        HistoGram[j] = 0; //初始化灰度直方图
    }

    for (j = START_LINE; j < CAMERA_H; j++)
    {
        for (i = 0; i < CAMERA_W; i++)
        {
            HistoGram[tmImage[j][i]]++; //统计灰度级中每个像素在整幅图像中的个数
        }
    }

    //获取最小灰度的值
    for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++)
    {
        ;
    }
    //获取最大灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistoGram[MaxValue] == 0; MaxValue--)
    {
        ;
    }

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
    threshold = GetOSTU(Image_Use);
    for (i = Fir_row; i < LCDH - 1; i++)
    {
        for (j = 1; j < LCDW - 1; j++)
        {

            if (Image_Use[i][j] < threshold + threshold_offset + threshold_offset2)
            {
                Pixle[i][j] = 0;
                // if (j == 40)
                // FINAL[i] = 0;
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
*  修改时间：2020.06.03
*  备    注：寻找底层黑白跳变点，逐层向上搜索每行的跳变点。(向两边搜点、范围搜点两种方法）
             将整幅图的左右5列置黑
             每一行只检测两个跳变点。
             ////然后利用求平均值绘制中心线，单写
*************************************************************************/

void Pic_DrawLRside(void)
{
    const int Middle = 41;
    int i = 0, j = 0;
    int search_flag1 = 0, search_flag2 = 0;
    int Side_flag;
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
                if (Pixle[i][j] == 1 && Pixle[i][j - 1] == 1 && Pixle[i][j - 2] == 1 && Pixle[i][j - 3] == 1 && Pixle[i][j - 4] == 1 && Pixle[i][j - 5] == 1 && Pixle[i][j - 6] == 1 && Pixle[i][j + 1] == 0 && Pixle[i][j + 2] == 0)
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
                for (j = Rig[i + 1] - 9; j < Rig[i + 1]; j++)
                {
                    if (Pixle[i][j] == 1 && Pixle[i][j - 1] == 1 && Pixle[i][j - 2] == 1 && Pixle[i][j - 3] == 1 && Pixle[i][j - 4] == 1 && Pixle[i][j - 5] == 1 && Pixle[i][j - 6] == 1 && Pixle[i][j + 1] == 0 && Pixle[i][j + 2] == 0)
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
                for (j = Rig[i + 1]; j < Rig[i + 1] + 8 && j < Last_col+1 ; j++)
                {
                    if (Pixle[i][j] == 1 && Pixle[i][j - 1] == 1 && Pixle[i][j - 2] == 1 && Pixle[i][j - 3] == 1 && Pixle[i][j - 4] == 1 && Pixle[i][j - 5] == 1 && Pixle[i][j - 6] == 1 && Pixle[i][j + 1] == 0 && Pixle[i][j + 2] == 0)
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
            for (j = Rig[i + 1] + 8; j < Last_col + 1 ; j++)
            {
                if (Pixle[i][j] == 1 && Pixle[i][j - 1] == 1 && Pixle[i][j - 2] == 1 && Pixle[i][j - 3] == 1 && Pixle[i][j - 4] == 1 && Pixle[i][j - 5] == 1 && Pixle[i][j - 6] == 1 && Pixle[i][j + 1] == 0 && Pixle[i][j + 2] == 0)
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
                if (Pixle[i][j] == 1 && Pixle[i][j + 1] == 1 && Pixle[i][j + 2] == 1 && Pixle[i][j + 3] == 1 && Pixle[i][j + 4] == 1 && Pixle[i][j + 5] == 1 && Pixle[i][j + 6] == 1 && Pixle[i][j - 1] == 0 && Pixle[i][j - 2] == 0)
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
                for (j = Lef[i + 1] + 9; j > Lef[i + 1]; j--)
                {
                    if (Pixle[i][j] == 1 && Pixle[i][j + 1] == 1 && Pixle[i][j + 2] == 1 && Pixle[i][j + 3] == 1 && Pixle[i][j + 4] == 1 && Pixle[i][j + 5] == 1 && Pixle[i][j + 6] == 1 && Pixle[i][j - 1] == 0 && Pixle[i][j - 2] == 0)
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
                for (j = Lef[i + 1]; j > Lef[i + 1] - 8 && j > Fir_col-1; j--)
                {
                    if (Pixle[i][j] == 1 && Pixle[i][j + 1] == 1 && Pixle[i][j + 2] == 1 && Pixle[i][j + 3] == 1 && Pixle[i][j + 4] == 1 && Pixle[i][j + 5] == 1 && Pixle[i][j + 6] == 1 && Pixle[i][j - 1] == 0 && Pixle[i][j - 2] == 0)
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
            for (j = Lef[i + 1] - 8; j > Fir_col - 1; j--)
            {
                if (Pixle[i][j] == 1 && Pixle[i][j + 1] == 1 && Pixle[i][j + 2] == 1 && Pixle[i][j + 3] == 1 && Pixle[i][j + 4] == 1 && Pixle[i][j + 5] == 1 && Pixle[i][j + 6] == 1 && Pixle[i][j - 1] == 0 && Pixle[i][j - 2] == 0)
                {
                    Lef[i] = j;
                    Side_flag = 1;
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
*  修改时间：2019.7.7
*  备    注：
*************************************************************************/

void Pic_particular(void)
{
    int i;
    Lef_edge = 0;
    Rig_edge = 0;
    for (i = 59; i > Fir_row; i--)
    {
        if (Lef[i] <= Fir_col)
            Lef_edge += 1;
        if (Rig[i] >= Last_col)
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

            if (New_Lef[i] != MIDMAP)
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