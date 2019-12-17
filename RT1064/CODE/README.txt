改动：
注释search.c第79行开始my_key_debug()函数
注释search.h第25行my_key_debug()声明
注释Data_Send.h第28行extern PID PID_SPEED（在control里，输出参数用，先不迁了）
注释Data_Send.c第173-188行输出变量（都不要了，变量输出我们自己的）
注释search.c中429行开始的Road_rec()函数，因为EM相关变量、函数定义在control里
注释search.c中1197行开始的Pic_offset_fig()函数，因为cam_offset,em_offset在control里定义
注释search.c中1329行开始的Pic_offset_filter()函数，理由同上
注释search.c中1695行开始的避障部分，不迁移ToF.c，ToF.h两个激光测距模块
注释SEEKFREE_MT9V03X_SCI_ME.c中camera_dispose_main()函数第213行-232行，关于road=0~road=2的部分，因为变量Turn_Angle在control里定义
注释SEEKFREE_MT9V03X_SCI_ME.c中camera_dispose_main()函数第260 262 263行，因为上述search.c中对应函数被注释
更改SEEKFREE_MT9V03X_SCI_ME.c中Get_Use_Imge()函数调用的image函数，因为rt1064中该函数为mt9v03x_csi_image
注释SEEKFREE_MT9V03X_SCI_ME.c中image()函数声明，理由同上
注释SEEKFREE_MT9V03X_SCI_ME.c中camera_dispose_main()第225行对Road3_cnt的定义，因为未使用
更改/* 调试参数 */ 从main.c到search.c
注释search.c第793,814行
更改search.c第818行，添加return sum，变多出口为单出口，函数最后返回
注释search.c第907行，因为未使用
注释search.c第1635，1637行，因为未使用

//---------------------------------------------//

/** 191206 **/
改动：
加入 control.c&h
加入 pwm_me.c&h
加入 qtimer.c&h
更改 headfile.h 添加control.h/qtimer.h/pwm_me.h
取消注释Data_Send.h第28行extern PID PID_SPEED
添加search.c中50行对Cam_offset的定义
取消注释search.c中433行开始的Road_rec()函数
注释Road_rec()中oldwhite，Road1_cnt1，Road1_flag1变量，因为没有用到
注释search.c中63-73行，都是EM
注释search.c中482-493行，因为有关EM
注释第499行原先if条件，改为无EM判断
注释search.c中563-574行，因为有关EM
注释第580行原先if条件，改为无EM判断
取消注释search.c中1209行开始的Pic_offset_fig()函数，因为补充了Cam_offset定义
取消注释search.c中1339行开始的Pic_offset_filter()函数，理由同上
重写SEEKFREE_MT9V03X_SCI_ME.c中camera_dispose_main()函数第227行开始对road=0-2的部分，去掉turn_angle
取消注释SEEKFREE_MT9V03X_SCI_ME.c中第292,294,295行对Road_rec，Pic_offset_fig，Pic_offset_filter的声明

未改动:
注释search.c第93行开始my_key_debug()函数
注释search.h第25行my_key_debug()声明
注释Data_Send.c第173-188行输出变量（上位机换了）
注释search.c中1717行开始的避障部分，不迁移ToF.c，ToF.h两个激光测距模块
注释search.c第799,820行的return sum，添加824行的return sum，变多出口为单出口，函数在最后返回
注释search.c第907行static char road1_flag3，因为未使用
注释search.c第1640，1642行的int j，static char flag5 = 1，因为未使用
注释SEEKFREE_MT9V03X_SCI_ME.c中278行Pic_seedfill();这个不知道是哪里的，好像之前就注释掉了
更改SEEKFREE_MT9V03X_SCI_ME.c中Get_Use_Imge()函数调用的image函数，因为rt1064中该函数为mt9v03x_csi_image
注释SEEKFREE_MT9V03X_SCI_ME.c中image()函数声明，理由同上
注释SEEKFREE_MT9V03X_SCI_ME.c中camera_dispose_main()第225行对Road3_cnt的定义，因为未使用
更改/* 调试参数 */ 从main.c到search.c

//------------------------------------//

/**2019.12.07  **/
GMY

修改fsl_pwm.h中PWM_DUTY_MAX值为20000
修改电机频率为13000Hz，初始化后电机转速0；
设定舵机参数SERVO_MIDDLE以及SERVO_RANGE;
舵机Servo_duty中对duty限幅，以防duty溢出;
**Servo_duty输入量中心偏置放入duty函数中**
**Servo_duty输入改为float**
**新建Properties.h用于放置小车属性及其他参数;**

Cam_Turn_Control声明及定义由search.c迁移至control.h&c,
Cam_Turn_Control更名为Turn_Cam_Out
Turn_P_Cam更名为Turn_Cam_P;Turn_D_Cam更名为Turn_Cam_D;*_Table同
删除Cam_offset在control多余的声明和定义
//-------------------------------------//

/** 191208  **/
JMK&LKM

增加main.c的    Para_Init();    //各个变量初始化
增加function.c&h
增加diff.c&h
增加speed.c&h
更改isr.c第30行开始的void PIT_IRQHandler(void)函数
添加control.c第2行对差分函数get_diff_state的声明
取消control.c第5行对PID的注释
取消control.c第9行对motor_out的注释
添加control.c第11行对cam_offset_range的定义
添加control.h第17.18行对：float limit_f(float n, float lower, float higher);float my_limit(float a, float limit)的声明
添加control.h第22行Speed_Control()的声明
添加headfile.h对三个.h的include
properties内容遷移至function.c
#define中断时间
D12-B1
D13-B2
D0-A1
D1-A2
//-------------------------------------//

/** 191213 **/
ALL

修改function.h中舵机限幅为180（190极限）
修改function.h中舵机中位为1335
添加SEEKFREE_MT9V03X_CSI_ME.c第210-226行Get_pic_with_edge()函数定义，用于带边线的灰度图
添加SEEKFREE_MT9V03X_CSI_ME.hGet_pic_with_edge()函数的声明
添加SEEKFREE_MT9V03X_CSI_ME.c第316行Get_pic_with_edge()函数
修改SEEKFREE_MT9V03X_CSI.c中63行 MT9V03X_CFG_CSI[CONFIG_FINISH][2]的FPS为200
修改function.c第28行为diff_on()，开差速
修改function.c初始差速设定值：lib_set_speed(1.0);
取消control.c中第171-177行的注释，将speedTarget1/2替代SetSpeed1/2
A接右轮 B接左轮

//------------------------------//
/** 191215 **/
LKM&GMY

function.h中添加摄像头频率参数，置位100（200微暗）
修复Data_Send
加入串口初始化，波特率1500000
**上位机修复进程：
购置无线模块EPS8266-12F
经检查：
无线模块发射完好
无线模块协议完好（可以发送ICMP、等至上位机）
无线模块接收完好



//-----------------------------//
/** 191217 **/
GMY
更新去畸变方式为直线函数参数，而非查表，函数参数已重新标定
近景5行不准，56-60，用数据表标定，K0_Tabke；

