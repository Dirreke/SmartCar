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
注释searchc第907行，因为未使用
注释search.c第1635，1637行，因为未使用