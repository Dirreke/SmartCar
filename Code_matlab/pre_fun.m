function []=pre_fun(name)
% name='mykc20191217(11)';
% No=455;%429


%% 
index=strcat('../SWJ/Data/',name,'.txt');
index1=strcat('./Data/',name,'.mat');
dos('Recode');
data=textread(index);
data=data';
global MAX_VARIAVLE ImageH ImageW;
WORD_LENS=4;


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
                temp=data(k+5:k+4*num1+4);
                Variable(frame0,1:length(temp)+1)=[temp num1];
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
clear name MAX_VARIABLE;
%% 图像处理

[m,n]=size(Image_unpcsImage);
Images=cell(1,m);
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

% clear Image Image_unpcsImage Image_others frame frame0 flag data a temp ki kj n num1 num2 i k p Pi Pj P MAX_VARIABLE WORD_LENS index;
%% 数据处理
Var=zeros(m,Variable(1,end));
Variable=Variable(:,1:end-1);

for i=1:m
    Var(i,:)=typecast(uint8(Variable(i,:)),'single');
end
save(index1,'Image_L','Image_M','Image_R','Images','Var')

end