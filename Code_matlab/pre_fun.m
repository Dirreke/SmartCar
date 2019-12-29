clear all;
clc;
close all;
name='mykc20191217(11)';
index=strcat('../SWJ/Data/',name,'.txt');
dos('Recode');
data=textread(index);
data=data';

WORD_LENS=4;
MAX_VARIAVLE=30;

i=1;
k=1;
flag=0;
frame=0;
frame0=1;
%% 读图
while 1
    if k>=numel(data)
        break
    end
    if flag==0
        if data(k)~=85
            k=k+1;
            continue;
        elseif data(k+1)~=170 % 85+~170
            k=k+1;
            continue;
        elseif data(k+2)~=255 % 85+170+~255
            k=k+2;
            continue;
        elseif data(k+3)~=1 % 85+170+255+~1
            k=k+3;
            continue;
        else                %  85 +170 +255+1
            num1=data(k+4);
            frame=frame+1;
            if num1>MAX_VARIAVLE
                warning('The number of Variables in No.%d frame is too much!',frame);
                
                k=k+MAX_VARIAVLE*WORD_LENS;
                continue
            end
            
            if data(k+4*num1+5)==1
                temp=data(k+5:k+4*num1+5);
                Variable(frame0,1:length(temp))=temp;
                flag=1;
            else
                warning('The Variable data is more or less than it is declared');
                flag=0;
            end
            k=k+4*num1+6;
            continue;
        end
    elseif flag==1
        if data(k)~=85
            k=k+1;
            continue;
        elseif data(k+1)~=170 % 85+~170
            k=k+1;
            continue;
        elseif data(k+2)~=255 % 85+170+~255
            k=k+2;
            continue;
        elseif data(k+3)~=162 % 85+170+255+~162
            k=k+3;
            continue;
        else                %  85 +170 +255+162
            num2=data(k+6)*256+data(k+5);
            if data(k+6+num2)==242
                temp=data(k+7:k+6+num2);
                Image_unpcsImage(frame0,1:length(temp))=temp;
                frame0=frame0+1;
                flag=2;
            else
                warning('The Image data is more or less than it is declared');
                flag=0;
            end
            k=k+num2+7;
            continue;
        end
    elseif flag==2
        if data(k)~=85
            k=k+1;
            continue;
        elseif data(k+1)~=170 % 85+~170
            k=k+1;
            continue;
        elseif data(k+2)~=17 % 85+170+~17
            k=k+2;
            continue;
        else
            flag=0;
            continue
        end
    end
    if k==numel(data)
        break
    end
end
%% 图像处理

[m,n]=size(Image_unpcsImage);
Images=cell(1,m);
ImageH=60;
ImageW=80;
for ki=1:m
    P=0;
    Pi=1;
    Pj=1;
    Image=zeros(ImageH,ImageW);
    for kj=2:n
        if Image_unpcsImage(ki,kj)==240
            temp=Image_unpcsImage(ki,kj+1:end);
            Image_others(ki,1:length(temp))=temp;
            break
        end
        if Image_unpcsImage(ki,kj)==255
            Pi=Pi+1;
            Image(Pi,Pj:ImageW)=P;
        else
            Image(Pi,Pj:Image_unpcsImage(ki,kj)-1)=P;
            Pj=Image_unpcsImage(ki,kj);
            P=~P;
        end
        
    end
    Images{ki}=Image;
end
% 边,每一列时是1图
Image_L=Image_others(:,1:ImageH)';
Image_M=Image_others(:,1+ImageH:ImageH*2)';
Image_R=Image_others(:,1+ImageH*2:ImageH*3)';

clear Image Image_unpcsImage Image_others frame frame0 flag data a temp ki kj n num1 num2 i k m p Pi Pj P MAX_VARIABLE WORD_LENS ;


%%
No=455;%429
Pic=Images{No};
Pic_L=Image_L(:,No);
Pic_M=Image_M(:,No);
Pic_R=Image_R(:,No);
figure
imshow(Pic)
clear Images Image_L Image_R Image_M No
%% Matlab 自带函数边缘提取
% Picedge=edge(Pic);



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
%% 去畸变2
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
clear Rig_New Lef_New;

for k=1:ImageH
    
    tempy=k-1;
    tempNewy(k)=round(((d*c+h)*(29.5-tempy)-h*d*s)/(s*(29.5-tempy)+d*c));
    % tempNewy(k)=((d*c+h)*(29.5-tempy)-h*d*s)/(s*(29.5-tempy)+d*c);
    
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
tempNewy=tempNewy-tempNewy(60);

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
figure
plot(tempNewxL,tempNewy)
hold on
plot(tempNewxR,tempNewy)
figure
plot(Pic_L,60:-1:1)
hold on
plot(Pic_R,60:-1:1)
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

%% 直接提取=插值+压缩
clear Rig_New
clear Lef_New
k=1;
k2=58;
Bili=round(tempNewy(1)/59);
while k2>=0
    temp=k2*Bili;
    if tempNewy(k)>=temp && tempNewy(k+1)<=temp
        step=tempNewy(k)-tempNewy(k+1);
        if tempNewxR(k)~=860 && tempNewxR(k+1)~=860
            Rig_New(59-k2+1)=(tempNewxR(k)-tempNewxR(k+1))/step*(temp-tempNewy(k+1))+tempNewxR(k+1);
        else
            Rig_New(59-k2+1)=860;
        end
        if tempNewxL(k)~=-860 && tempNewxL(k+1)~=-860
            Lef_New(59-k2+1)=(tempNewxL(k)-tempNewxL(k+1))/step*(temp-tempNewy(k+1))+tempNewxL(k+1);
        else
            Lef_New(59-k2+1)=-860;
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

k=1;
Lef_New_New=Lef_New;
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
    Lef_New_New(k+1)=(Lef_New(k)+Lef_New(k+1)+Lef_New(k+2))/3;
    if Lef_New(k+3)==-860
        k=k+4;
        continue;
    end
    while  k<=57 && Lef_New(k+3)~=-860
        Lef_New_New(k+2)=(Lef_New(k)+Lef_New(k+1)+Lef_New(k+2)+Lef_New(k+3))/4;
        k=k+1;
    end
    k=k+4;
end
k=1;
Rig_New_New=Rig_New;
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
    Rig_New_New(k+1)=(Rig_New(k)+Rig_New(k+1)+Rig_New(k+2))/3;
    if Rig_New(k+3)==860
        k=k+4;
        continue;
    end
    while k<=57 && Rig_New(k+3)~=860
        Rig_New_New(k+2)=(Rig_New(k)+Rig_New(k+1)+Rig_New(k+2)+Rig_New(k+3))/4;
        k=k+1;
    end
    k=k+4;
end


% figure
% plot(Lef_New)
% hold on
% plot(Lef_New_New);
% figure
% plot(Rig_New)
% hold on
% plot(Rig_New_New);
figure
plot(Lef_New_New,60:-1:1)
hold on
plot(Rig_New_New,60:-1:1)

%% 去畸变函数
function a=pixel_undistort(x,y,LR)
MIDMAP=300;
offset=300/(y+10);
table=[66 67 68 69 90];
temx=x;
if temx==2
    temx=0;
end
if temx==78
    temx=79;
end
if y>55
    offset=300/table(y-55);
end
if LR==0 %左边线
    New_Lef=round( (temx-39)*offset);
    if New_Lef < -MIDMAP
        New_Lef = -MIDMAP;
    end
    a=New_Lef;
else
    New_Rig=round((temx-39)*offset);
    if New_Rig> MIDMAP
        New_Rig = MIDMAP;
    end
    a=New_Rig;
end
end
function a=pixel_undistort2(x,y)
% offset_map=[31/77,31/77,31/9,31/9,31/10,31/11,31/12,31/13,31/14,31/15,31/16,31/17,31/17,31/19,31/19,31/21,31/21,31/23,31/24,31/25,31/25,31/27,31/28,31/29,31/30,31/31,31/31	,31/33,31/34,31/34,31/36,31/36	,31/36	,31/38	,31/40,31/40,31/42,31/42,31/42	,31/44	 ,31/46	 ,31/46	 ,31/48	  ,31/48	,31/48	,31/50	,31/51,31/52,31/53,31/54,31/54,31/56,31/56,31/58,31/58,31/58,31/60,31/60,31/61,31/77];
table=[63 63 62 62 62];
New_Lef=x-301;
offset=300/(y+10);
if y>55
    offset=300/table(y-55);
end
a=round(New_Lef/offset+39);
end




