
float  EM_Offset_Table[15] = {-350,     -200,      -150,     -100,       -60,      -40,     -20,       0,       20,          40,       60,       100,       150,    200,     350 };
float Turn_P_EM_Table0[15] = {0.56,   0.86,   0.76,     0.66,   0.58,   0.48,   0.45,    0.03,     0.45,  0.48,  0.58,   0.66,   0.76, 0.86 ,0.56};
float Turn_D_EM_Table0[15] = {0.6,       0.45,     0.44,    0.64,      0.82,      0.9,   1.2,   0.35,    1.20,    0.9,   0.82,    0.64,    0.45,    0.25,    0.6};
                               //弯道
float Turn_P_EM_Table1[15] = {1.5,   1.06,   0.86,     0.76,   0.68,   0.48,   0.45,    0.03,     0.45,  0.48,  0.68,   0.76,   0.86, 1.06 ,0.56};
float Turn_D_EM_Table1[15] = {0.4,       0.32,     0.5,    0.7,      0.9,      1.0,   1.40,   0.40,    1.40,       1.0,    0.9,       0.7,      0.5,    0.3,    0.1};









/*********************************************
***程序，根据中心偏移量计算左右轮输出速度
***输入参数：
***输出参数：
***说明：偏移量offset为负说明车身相对赛道中心偏左
         偏移量offset为正说明车身相对赛道中心偏右
        说明不对着，不用看注释
*********************************************/
//float KP = 17;
void Turn_EM0(void)
{

  float mid_err;
  float mid_length;
  float mid_length_err;
  int length_EM_23=0.17;
  mid_err = EM_err_cal(EM_Value_2,EM_Value_3,EM_Value_1,EM_Value_4); //左平行，右平行，左垂直，右垂直 得到电感偏差值
  mid_length= -0.000572*mid_err*mid_err*mid_err*mid_err*mid_err+0.008346*mid_err*mid_err*mid_err-0.05759*mid_err+0.085;//二值化拟合距离x(0-17) 也许需要重新拟合  h=2.8cm L=17cm 
  mid_length_err=mid_length-length_EM_23/2;//此值为距离  为负时偏右 为正时偏左
  EM_offset= mid_length_err/8.5*350;   //根据偏移距离分直线和弯道求出对应的偏移 映射到offset_table表
  TurnFuzzyPD_EM();  //根据偏移写模糊PD 
  Em_offset *=Turn_P_EM;
  EM_Turn_Control= PD_section(EM_offset);//先用P*offset求出打角，再乘一个PD作为打角的控制，还未加限幅
  

  Servo_Duty((uint32)(EM_Turn_Control)); //舵机控制
}

void Turn_EM1(void){
  float mid_err;
  float mid_length;
  float mid_length_err;
  float ctl_out;
  int length_EM_23=0.17;
  mid_err= EM_Value_2-Em_value_3;
  mid_length =  -0.000572*mid_err*mid_err*mid_err*mid_err*mid_err+0.008346*mid_err*mid_err*mid_err-0.05759*mid_err+0.085;
  mid_length_err = mid_length-length_EM_23/2;
  EM_offset1 = mid_length_err/8.5*350; 
  TurnFuzzyPD_EM();  //根据偏移写模糊PD 
  Em_offset1 *=Turn_P_EM;
  EM_Turn_Control1 = PD_section(EM_offset);
  EM_angle=acos(cos_angle);
  EM_Turn_Control2 = PD_section1(EM_angle);
  EM_Turn_Control = 1.0*EM_Turn_Control1+1.0*EM_Turn_Control2;

}


float EM_err_cal(float l,float r,float pl, float pr){

  //now calculate the actuall distance dA
  float lm, rm;    //l and r 's magnitude  //将全局变量改为局部变量，，部分与EM_angle_get 相同，但EM_angle_get使用值似乎没有用到；——GMY注
  float cos_angle; //cos of angle
  int K1=1.5;   //垂直电感和平行电感的系数不同，垂直电感偏小，因此加系数
  int K2=1.5;
  float err;

  lm = (float)sqrt(l * l + K1*pl * pl);
  rm = (float)sqrt(K2*pr * pr + r * r);

  if (lm > rm)
  {
    cos_angle = l / lm;
    err = (lm-rm);
  }
  else
  {
    cos_angle = r / rm;
    err = (rm-lm);
  }
/* bh进环之后按左边的电感出环
  if (Road1_flag == 3)
  {
    err = -1.21 * (2.0 - l);
    return err;
  }
*/
  if (l>r) //car is near right side
    return err;
  else //car is near left side
    return -err;
  
}






/*********************************
转弯PD模糊函数------电磁控制
输入参数：电磁计算偏差值
输出参数：电磁控制转弯PD
***********************************/ 

void TurnFuzzyPD_EM(void)
{
  int i=0;
  /***********fuzzy***********/
         if(EM_Road==4)  //弯道
        {
            if(EM_offset <= EM_Offset_Table[0])
              {
                Turn_P_EM = Turn_P_EM_Table0[0];
                Turn_D_EM = Turn_D_EM_Table0[0];
                return;
              }
              else if(EM_offset >= EM_Offset_Table[14])
              {
                Turn_P_EM = Turn_P_EM_Table0[14];
                Turn_D_EM = Turn_D_EM_Table0[14];
                return;
              }
            
        }
        else
        {
          if(EM_offset <= EM_Offset_Table[0])
              {
                Turn_P_EM = Turn_P_EM_Table1[0];
                Turn_D_EM = Turn_D_EM_Table1[0];
                return;
              }
              else if(EM_offset >= EM_Offset_Table[14])
              {
                Turn_P_EM = Turn_P_EM_Table1[14];
                Turn_D_EM = Turn_D_EM_Table1[14];
                return;
              }                                  
      for(i=0;i<14;i++)
      {
       
        }
          if(EM_offset >= EM_Offset_Table[i] && EM_offset < EM_Offset_Table[i+1])
          {
            if(EM_Road==4){  //弯道
              Turn_P_EM=Turn_P_EM_Table0[i]+(EM_offset-EM_Offset_Table[i])
                        *(Turn_P_EM_Table0[i+1]-Turn_P_EM_Table0[i])
                         /(EM_Offset_Table[i+1]-EM_Offset_Table[i]);
              Turn_D_EM=Turn_D_EM_Table0[i]+(EM_offset - EM_Offset_Table[i])
                      *(Turn_D_EM_Table0[i+1] - Turn_D_EM_Table0[i])
                       /(EM_Offset_Table[i+1] - EM_Offset_Table[i]);
            
              } 
            
            else{               //直道
              Turn_P_EM=Turn_P_EM_Table1[i]+(EM_offset-EM_Offset_Table[i])
                        *(Turn_P_EM_Table1[i+1]-Turn_P_EM_Table1[i])
                         /(EM_Offset_Table[i+1]-EM_Offset_Table[i]);
              Turn_D_EM=Turn_D_EM_Table1[i]+(EM_offset - EM_Offset_Table[i])
                      *(Turn_D_EM_Table1[i+1] - Turn_D_EM_Table1[i])
                       /(EM_Offset_Table[i+1] - EM_Offset_Table[i]);
              
            
            }
            
          }
      }
      

float PD_section(float err)
{
  static float last;
  float sub;
  float p;
  
  float out;
  sub = err - last;
  last = err;
  out=p* err + Turn_D_EM * sub;
//加限幅

  return (out);
}

float PD_section1(float err)
{
  static float last;
  float sub;
  float p;
  float d;
  float out;
  p=1.0;
  d=1.0;
  sub = err - last;
  last = err;
  out=p* err + d * sub;
//加限幅

  return (out);
}