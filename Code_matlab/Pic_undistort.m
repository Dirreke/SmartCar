function []=Pic_undistort(Pic_L,Pic_R,L,R,flag_canshu)
global ImageH;
if nargin==4
    flag_canshu=0;
end
%% 参数
% D0=10cm
% RoadWidth=40.4cm
% H0=17.5cm
%n=68;
%theta=60du
% /40.4*300
H0=129.9504950495050;
D0=74.257425742574260;
n0=68*2;
K0=300/n0;
d=sqrt((H0^2+D0^2)/K0^2-30^2);
h=(K0-1)*d;
% theta=-0.389;
theta=-(atan(D0/H0)+atan(30/d));
% theta=-0.2;
K0=300/n0;
d=sqrt((H0^2+D0^2)/K0^2-30^2);
h=(K0-1)*d;
c=cos(theta);
s=sin(theta);
% clear Rig_New Lef_New;

%% 去畸变
tempNewy=zeros(ImageH,1);
for k=1:ImageH
    
    tempy=k-1;
    tempNewy(k)=round(((d*c+h)*(29.5-tempy)-h*d*s)/(s*(29.5-tempy)+d*c));
    % tempNewy(k)=((d*c+h)*(29.5-tempy)-h*d*s)/(s*(29.5-tempy)+d*c);
end
tempNewy=tempNewy-tempNewy(60);
tempNewxR=zeros(ImageH,1);
if R
    for k=1:ImageH
        tempy=k-1;
        tempxR=Pic_R(k);
        if tempxR==78||tempxR==79
            tempNewxR(k)=860;
        else
            tempNewxR(k)=round((d*c+h)*2*(tempxR-39.5)/(s*(29.5-tempy)+d*c));
        end
        if tempNewxR(k)>860
            tempNewxR(k)=860;
        end
        
    end
end
tempNewxL=zeros(ImageH,1);
if L
for k=1:ImageH
    tempy=k-1;
    tempxL=Pic_L(k);
    if tempxL==1||tempxL==2
        tempNewxL(k)=-860;
    else
        tempNewxL(k)=round((d*c+h)*2*(tempxL-39.5)/(s*(29.5-tempy)+d*c));
    end
    if tempNewxL(k)<-860
        tempNewxL(k)=-860;
    end
end
end
figure
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
k=1;
k2=58;
Bili=round(tempNewy(1)/59);
Rig_New=zeros(ImageH,1);
Lef_New=zeros(ImageH,1);
while k2>=0
    temp=k2*Bili;
    if tempNewy(k)>=temp && tempNewy(k+1)<=temp
        step=tempNewy(k)-tempNewy(k+1);
        if R
        if tempNewxR(k)~=860 && tempNewxR(k+1)~=860
            Rig_New(59-k2+1)=(tempNewxR(k)-tempNewxR(k+1))/step*(temp-tempNewy(k+1))+tempNewxR(k+1);
        else
            Rig_New(59-k2+1)=860;
        end
        end
        if L
        if tempNewxL(k)~=-860 && tempNewxL(k+1)~=-860
            Lef_New(59-k2+1)=(tempNewxL(k)-tempNewxL(k+1))/step*(temp-tempNewy(k+1))+tempNewxL(k+1);
        else
            Lef_New(59-k2+1)=-860;
        end
        end
        k2=k2-1;
    else
        k=k+1;
        continue
    end
end
Rig_New(1)=tempNewxR(1);
Lef_New(1)=tempNewxL(1);
%% 滤波
if L
k=1;
New_Lef=Lef_New;
while k<=57
    if Lef_New(k+2)==-860
        k=k+3;
        continue;
    elseif Lef_New(k+1)==-860
        k=k+2;
        continue;
    elseif Lef_New(k)==-860
        k=k+1;
        continue
    end
    New_Lef(k+1)=(Lef_New(k)+Lef_New(k+1)+Lef_New(k+2))/3;
    if Lef_New(k+3)==-860
        k=k+4;
        continue;
    end
    while  k<=57 && Lef_New(k+3)~=-860
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
    if Rig_New(k+2)==860
        k=k+3;
        continue;
    elseif Rig_New(k+1)==860
        k=k+2;
        continue;
    elseif Rig_New(k)==860
        k=k+1;
        continue
    end
    New_Rig(k+1)=(Rig_New(k)+Rig_New(k+1)+Rig_New(k+2))/3;
    if Rig_New(k+3)==860
        k=k+4;
        continue;
    end
    while k<=57 && Rig_New(k+3)~=860
        New_Rig(k+2)=(Rig_New(k)+Rig_New(k+1)+Rig_New(k+2)+Rig_New(k+3))/4;
        k=k+1;
    end
    k=k+4;
end
end

figure
plot(New_Lef,60:-1:1)
hold on
plot(New_Rig,60:-1:1)
%% canshu
if flag_canshu==1
    printf(Bili);
end
%% 插值
% for k=1:ImageH-1
%     step=tempNewy(k)-tempNewy(k+1);
%     if tempNewxR(k)~=450 && tempNewxR(k+1)~=860
%         for k2=1:step
%         Rig_New(tempNewy(k)-k2+1)=(tempNewxR(k)-tempNewxR(k+1))/step*(tempNewy(k)-k2-tempNewy(k+1))+tempNewxR(k+1);
%         end
%
%     else
%         for k2=1:step
%         Rig_New(tempNewy(k)-k2+1)=860;
%         end
%     end
% end
% for k=1:ImageH-1
%     step=tempNewy(k)-tempNewy(k+1);
%     if tempNewxL(k)~=-860 && tempNewxL(k+1)~=-860
%         for k2=1:step
%         Lef_New(tempNewy(k)-k2+1)=(tempNewxL(k)-tempNewxL(k+1))/step*(tempNewy(k)-k2-tempNewy(k+1))+tempNewxL(k+1);
%         end
%
%     else
%         for k2=1:step
%         Lef_New(tempNewy(k)-k2+1)=-860;
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

end