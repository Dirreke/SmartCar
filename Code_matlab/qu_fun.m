function result=qu_fun(X)
global ImageH MIDMAP New_Lef New_Rig;
ImageH=60;
%%
% theta1=X(1);
h=X(1);
d=X(2);
% K=1;.

% theta1=atan(d*(70/8-1)/(70*5.5/8+28.5));
theta1=atan(d*(80/15-1)/(80*5.5/15+28.5));

c=cos(-theta1);
s=sin(-theta1);
% K=(dx*c+h)*71/(s*-28.5+dx*c)/150;
% d=77.3125/7.875*tan(theta1);

%% ȥ����
% tempNewy=zeros(ImageH,1);
tempy=[25,26,27,28,29,30,31,32,33,35,39,45,50];
tempNewy=((d*c+h).*(29.5-tempy)-h*d*s)./(s.*(29.5-tempy)+d*c);
%  [70.5 63 59 52.5 48 46.5 42.5 38 36 32 25.5 21.5 18]
[66.75 61 55.75 50.25 47 44.5 41 38 35 32 25.5 21 18]
tempNewy=tempNewy-tempNewy(end);
tempNewy(end)=[];
data=[66.75 61 55.75 50.25 47 44.5 41 38 35 32 25.5 21 18];
data=data-data(end);
data(end)=[];
quanjubili=tempNewy./data;
M=mean(quanjubili);
M22=(d*c+h)*2*(35.5)/(s*-28.5+d*c)/20;
test=(quanjubili-M)./M;
% result=sum(((quanjubili-M)./M).^2);
% result=(max(quanjubili)-min(quanjubili))/M
result=abs(M-M22)^2;
end



% ss=double(vpa(solve((dx*c+h)*(s*-28.5+dx*c)*6/((s*5.5+dx*c)*(dx*c+h)*71)-1==0)));




% ����
% 
% 
% tempNewxR=zeros(ImageH,1);
%     for k=startpoint:ImageH
%         tempy=k-1;
%         tempxR=Pic_R(k);
%         if tempxR==78||tempxR==79
%             tempNewxR(k)=MIDMAP;
%         else
%             tempNewxR(k)=round((d*c+h)*2*(tempxR-40.5)/(s*(29.5-tempy)+d*c));
%         end
%         if tempNewxR(k)>MIDMAP
%             tempNewxR(k)=MIDMAP;
%         end
%         
%     end
% tempNewxL=zeros(ImageH,1);
% for k=startpoint:ImageH
%     tempy=k-1;
%     tempxL=Pic_L(k);
%     if tempxL==1||tempxL==2
%         tempNewxL(k)=-MIDMAP;
%     else
%         tempNewxL(k)=round((d*c+h)*2*(tempxL-40.5)/(s*(29.5-tempy)+d*c));
%     end
%     if tempNewxL(k)<-MIDMAP
%         tempNewxL(k)=-MIDMAP;
%     end
% end