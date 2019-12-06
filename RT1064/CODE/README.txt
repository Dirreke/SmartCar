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
