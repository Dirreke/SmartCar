clear all;clc;close all;

name='mykc20191217(11)';
global MAX_VARIAVLE ImageH ImageW;
MAX_VARIAVLE=30;
ImageH=60;
ImageW=80;



try load(strcat('./Data/',name,'.mat'))
catch
    pre_fun(name);
    load(strcat('./Data/',name,'.mat'))
end
No=455;
%% 取图

% No=455;%429
Pic=Images{No};
Pic_L=Image_L(:,No);
Pic_M=Image_M(:,No);
Pic_R=Image_R(:,No);
figure
imshow(Pic)
clear Images Image_L Image_R Image_M No
%% Matlab 自带函数边缘提取
% Picedge=edge(Pic);

%% 去畸变2
Pic_undistort(Pic_L,Pic_R,1,1);

%% 
% void Pic_DrawMid_und(void)
% {
%     int i;
%     int count = 0;
%     int flag = 0;
%     float slope_temp;
% 
%     for (i = 1; i < 60; i++)
%     {
%         if (New_Lef[i] != -MIDMAP && New_Rig[i] != MIDMAP) //Mid Calculaing
%         {
%             New_Mid[i] = New_Lef[i] + New_Rig[i];
%             flag = 1;
%         }
%         else if (New_Lef[i] == -MIDMAP && New_Rig[i] != MIDMAP)
%         {
%             New_Mid[i] = New_Rig[i] - ROAD_HALF_WIDTH;
%             flag = 1;
%         }
%         else if (New_Lef[i] != -MIDMAP && New_Rig[i] == MIDMAP)
%         {
%             New_Mid[i] = New_Lef[i] + ROAD_HALF_WIDTH;
%             flag = 1;
%         }
%         else
%         {
%             count++; //for 插值
%             flag = 0;
%         }
% 
%         if (count != 0 && flag == 1) //插值
%         {
%             slope_temp = (New_Mid[i] - New_Mid[i - count - 1]) / (count + 1);
%             for (j = count; j > 0; j--)
%             {
%                 New_Mid[i - j] = (int)New_Mid[i] - slope_temp * j + 0.5;
%             }
%         }
%     }
% }