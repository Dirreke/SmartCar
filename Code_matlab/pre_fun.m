clear all; 
clc;
close all;
name='mykc20191217(1)';
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
%% ��ͼ
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
%% ͼ����

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
% ��,ÿһ��ʱ��1ͼ
Image_L=Image_others(:,1:ImageH)';
Image_M=Image_others(:,1+ImageH:ImageH*2)';
Image_R=Image_others(:,1+ImageH*2:ImageH*3)';

clear Image Image_unpcsImage Image_others frame frame0 flag data a temp ki kj n num1 num2 i k m p Pi Pj P MAX_VARIABLE WORD_LENS ;


%% 
No=316;
Pic=Images{No};
Pic_L=Image_L(:,No);
Pic_M=Image_M(:,No);
Pic_R=Image_R(:,No);
clear Images Image_L Image_R Image_M No 
%%
Picedge=edge(Pic);



%% ȥ����
Pic_new=zeros(60,601);

test=[];

for k=1:ImageH
    for k2=2:601
        a=pixel_undistort2(k2,k);
        if a>0 &&a<=ImageW
            Pic_new(k,k2)=Pic(k,a);
        end
    end
end
% for k=1:60
%         a=pixel_undistort(Pic_L(k),k,0)+301;
% %         test=[test,a];
%         Pic_new(k,a)=Pic(k,Pic_L(k));
% end

figure
imshow(Pic)
figure
imshow(Pic_new)

    
%% ����궨������ֱ�У����˹�
D=Pic_R-Pic_L;
D(Pic_L==1)=0;
D(Pic_R==78)=0;





%% ȥ���亯��
function a=pixel_undistort(x,y,LR)
MIDMAP=300;
offset=300/(y+10);
table=[63 63 62 62 62];
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
  if LR==0 %�����
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




