clear all;clc;close all;

name='mykc20200513_10cm';
% mykc20200513_10cm
Initial();
global New_Lef New_Rig New_Mid;
try load(strcat('./Data/',name,'.mat'))
catch
    pre_fun(name);
    load(strcat('./Data/',name,'.mat'))
end
No=455;

%% ȡͼ

No=9272;%429
Pic=Images{No};
Pic_L=Image_L(:,No);
Pic_M=Image_M(:,No);
Pic_R=Image_R(:,No);
figure
imshow(Pic)
clear Images Image_L Image_R Image_M No
%% Matlab �Դ�������Ե��ȡ
% Picedge=edge(Pic);

%% ȥ����2
Pic_undistort(Pic_L,Pic_R,1,1);

%% ȥ������������
Pic_DrawMid_und();

%% ��ͼ
Create_Fig();