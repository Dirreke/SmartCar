function []=Pic_undistort(Pic_L,Pic_R,L,R,flag_canshu)
global ImageH MIDMAP New_Lef New_Rig;
% if nargin==4
    flag_canshu=0;
% end
% % % %% 参数
% % % startpoint=12;%此参数小为远景。，从此开始取
% % % D0=10cm
% % % % RoadWidth=40.4cm
% % % % H0=10cm%17.5cm
% % % %n=68;
% % % %theta=60du
% % % % /40.4*300
% H0=129.9504950495050;
% H0=70.3;
% D0=67;
% % % D0=60;
% n0=68*2;
n0=77*2;
% % % K0=300/n0;
% % % d=sqrt((H0^2+D0^2)/K0^2-30^2);
% % % h=(K0-1)*d;
% % % % theta=-0.389;
% % % theta=-(atan(D0/H0)+atan(30/d));
% % % % theta=-0.2;
% % % % K0=300/n0;
% % % % d=sqrt((H0^2+D0^2)/K0^2-30^2);
% % % % h=(K0-1)*d;
% % % c=cos(theta);
% % % s=sin(theta);
% % % % clear Rig_New Lef_New;
%%
K=1;%0.71;
startpoint=25;
% H0=75;
% D0=50;
% n0=77*2*K;
% K1=300/n0;
% theta1=0;
% d1=0;
% while 1
%     d0=d1;
%     theta0=theta1;
%     ah=H0-28.5*K*sin(theta0);
%     ad=D0+28.5*K*cos(theta0);
%     d1=sqrt((ah^2+ad^2)/(K1-1)^2-(28.5*K)^2);
%     theta1=(atan(ad/ah)+atan(28.5*K/d1));
%     if d1-d0<1e-12 && theta1-theta0<1e-6
%         break
%     end
% end
% c=cos(-theta1);
% s=sin(-theta1);
% h=H0;
% d=d1;
  
h=55.115450159981592;d=40.979835145934850;theta1=atan(d*(71/8-1)/(71*5.5/8+28.5));
c=cos(-theta1);
s=sin(-theta1);


% clear theta0 ad ah
%%
% K=-0.6;
% startpoint=20;
% H0=75;
% D0=82;
% n0=77*2*K;
% K1=300/n0;
% theta1=0;
% d1=0;
% while 1
%     d0=d1;
%     theta0=theta1;
%     ah=H0+28.5*K*sin(theta0);
%     ad=D0-28.5*K*cos(theta0);
%     d1=sqrt((ah^2+ad^2)/(K1+1)^2-(28.5*K)^2);
%     theta1=(atan(ad/ah)+atan(-28.5*K/d1));
%     if d1-d0<1e-12 && theta1-theta0<1e-6
%         break
%     end
% end
% c=cos(-theta1);
% s=sin(-theta1);
% h=H0;
% d=-d1;
% clear theta0 ad ah
%% 去畸变
tempNewy=zeros(ImageH,1);
% 此段程序中在C中为表
for k=1:ImageH
    
    tempy=k-1;
    tempNewy(k)=round(((d*c+h)*(29.5-tempy)*K-h*d*s)/(s*(29.5-tempy)*K+d*c));
    % tempNewy(k)=((d*c+h)*(29.5-tempy)-h*d*s)/(s*(29.5-tempy)+d*c);
end
tempNewy=tempNewy-tempNewy(60);
% 代码
tempNewxR=zeros(ImageH,1);
if R
    for k=startpoint:ImageH
        tempy=k-1;
        tempxR=Pic_R(k);
        if tempxR==78||tempxR==79
            tempNewxR(k)=MIDMAP;
        else
            tempNewxR(k)=round((d*c+h)*2*(tempxR-40.5)*K/(s*(29.5-tempy)*K+d*c));
        end
        if tempNewxR(k)>MIDMAP
            tempNewxR(k)=MIDMAP;
        end
        
    end
end
tempNewxL=zeros(ImageH,1);
if L
for k=startpoint:ImageH
    tempy=k-1;
    tempxL=Pic_L(k);
    if tempxL==1||tempxL==2
        tempNewxL(k)=-MIDMAP;
    else
        tempNewxL(k)=round((d*c+h)*2*(tempxL-40.5)*K/(s*(29.5-tempy)*K+d*c));
    end
    if tempNewxL(k)<-MIDMAP
        tempNewxL(k)=-MIDMAP;
    end
end
end
% figure
% plot(tempNewxL,tempNewy)
% hold on
% plot(tempNewxR,tempNewy)
% figure
% plot(Pic_L,60:-1:1)
% hold on
% plot(Pic_R,60:-1:1)

%% 直接提取=插值+压缩
% clear Rig_New
% clear Lef_New
k=startpoint;%寻找起点
k2=59;%Bili=5
Bili=round(tempNewy(k)/59);
Rig_New=zeros(ImageH,1);
Lef_New=zeros(ImageH,1);
tempNewy(k)=k2*Bili;
while k2>=0
    temp=k2*Bili;
    if tempNewy(k)>=temp && tempNewy(k+1)<=temp
        step=tempNewy(k)-tempNewy(k+1);
        if R
        if tempNewxR(k)~=MIDMAP && tempNewxR(k+1)~=MIDMAP
            Rig_New(59-k2+1)=(tempNewxR(k)-tempNewxR(k+1))/step*(temp-tempNewy(k+1))+tempNewxR(k+1);
        else
            Rig_New(59-k2+1)=MIDMAP;
        end
        end
        if L
        if tempNewxL(k)~=-MIDMAP && tempNewxL(k+1)~=-MIDMAP
            Lef_New(59-k2+1)=(tempNewxL(k)-tempNewxL(k+1))/step*(temp-tempNewy(k+1))+tempNewxL(k+1);
        else
            Lef_New(59-k2+1)=-MIDMAP;
        end
        end
        k2=k2-1;
    else
        k=k+1;
        continue
    end
end
% Rig_New(1)=tempNewxR(1);
% Lef_New(1)=tempNewxL(1);%用来补超出行，对应k2=58.
%% 滤波
if L
k=1;
New_Lef=Lef_New;
while k<=57
    if Lef_New(k+2)==-MIDMAP
        k=k+3;
        continue;
    elseif Lef_New(k+1)==-MIDMAP
        k=k+2;
        continue;
    elseif Lef_New(k)==-MIDMAP
        k=k+1;
        continue
    end
    New_Lef(k+1)=(Lef_New(k)+Lef_New(k+1)+Lef_New(k+2))/3;
    if Lef_New(k+3)==-MIDMAP
        k=k+4;
        continue;
    end
    while  k<=57 && Lef_New(k+3)~=-MIDMAP
        New_Lef(k+2)=(Lef_New(k)+Lef_New(k+1)+Lef_New(k+2)+Lef_New(k+3))/4;
        k=k+1;
    end
    k=k+4;
end
end
if R
k=1;
New_Rig=Rig_New;
while k<=57
    if Rig_New(k+2)==MIDMAP
        k=k+3;
        continue;
    elseif Rig_New(k+1)==MIDMAP
        k=k+2;
        continue;
    elseif Rig_New(k)==MIDMAP
        k=k+1;
        continue
    end
    New_Rig(k+1)=(Rig_New(k)+Rig_New(k+1)+Rig_New(k+2))/3;
    if Rig_New(k+3)==MIDMAP
        k=k+4;
        continue;
    end
    while k<=57 && Rig_New(k+3)~=MIDMAP
        New_Rig(k+2)=(Rig_New(k)+Rig_New(k+1)+Rig_New(k+2)+Rig_New(k+3))/4;
        k=k+1;
    end
    k=k+4;
end
end


%% canshu
if flag_canshu==1
    Bili
end
%% 插值
% for k=1:ImageH-1
%     step=tempNewy(k)-tempNewy(k+1);
%     if tempNewxR(k)~=450 && tempNewxR(k+1)~=MIDMAP
%         for k2=1:step
%         Rig_New(tempNewy(k)-k2+1)=(tempNewxR(k)-tempNewxR(k+1))/step*(tempNewy(k)-k2-tempNewy(k+1))+tempNewxR(k+1);
%         end
%
%     else
%         for k2=1:step
%         Rig_New(tempNewy(k)-k2+1)=MIDMAP;
%         end
%     end
% end
% for k=1:ImageH-1
%     step=tempNewy(k)-tempNewy(k+1);
%     if tempNewxL(k)~=-MIDMAP && tempNewxL(k+1)~=-MIDMAP
%         for k2=1:step
%         Lef_New(tempNewy(k)-k2+1)=(tempNewxL(k)-tempNewxL(k+1))/step*(tempNewy(k)-k2-tempNewy(k+1))+tempNewxL(k+1);
%         end
%
%     else
%         for k2=1:step
%         Lef_New(tempNewy(k)-k2+1)=-MIDMAP;
%         end
%     end
% end

%% 压缩
% Bili=round(tempNewy(1)/59);
% Rig_New_New=Rig_New(1:Bili:end);
% Lef_New_New=Lef_New(1:Bili:end);
% Lef_New_New(60)=Lef_New(end);
% Rig_New_New(60)=Rig_New(end);
% clear Rig_New
% clear Lef_New
% Rig_New=Rig_New_New;
% Lef_New=Lef_New_New;
% clear Lef_New_New
% clear Rig_New_New





%% 去畸变
% Pic_new=zeros(60,601);
% Pic_new_edge=zeros(60,601);
% Lef_New=zeros(1,ImageH);
% Rig_New=zeros(1,ImageH);
%
% for k=1:ImageH
%     for k2=2:601
%         a=pixel_undistort2(k2,k);
%         if a>0 &&a<=ImageW
%             Pic_new(k,k2)=Pic(k,a);
%         end
%     end
% end
% for k=1:ImageH
%         a=pixel_undistort(Pic_L(k),k,0)+301;
%         b=pixel_undistort(Pic_R(k),k,1)+301;
%         Lef_New(k)=a;
%         Rig_New(k)=b;
%         Pic_new_edge(k,a)=Pic(k,Pic_L(k));
% end
%
% figure
% imshow(Pic_new)


%% 畸变标定，对于直行，需人工
% D=Pic_R-Pic_L;
% D(Pic_L==1)=0;
% D(Pic_R==78)=0;

% end