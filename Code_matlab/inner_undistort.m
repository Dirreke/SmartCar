%% 图像设置
% 40行 80列
% 行：40-80
% 列：15-173 隔行
y=40:79;
y=y'*ones(1,80);
x=15:2:173;
x=ones(40,1)*x;

%% 图像坐标系->相机坐标系
% x,y摄像机去畸前坐标 
% X,Y图像坐标系坐标
X=(x-cameraParams.PrincipalPoint(1))/cameraParams.FocalLength(1);
Y=(y-cameraParams.PrincipalPoint(2))/cameraParams.FocalLength(2);

%% 去除耦合系数
X=X-Y*cameraParams.Skew;

%% 利用k1k2p1p2去除径向切向畸变
% 初值
X_new = X;
Y_new = Y;
% 迭代
for i =1:20000
    % 径向
    temp=X_new.*X_new+Y_new.*Y_new;
    tempKr=1+cameraParams.RadialDistortion(1).*temp ...
            +cameraParams.RadialDistortion(2).*temp.^2 ...
            +cameraParams.RadialDistortion(3).*temp.^3;
    % 切向
    deltax=2*cameraParams.TangentialDistortion(1)*X_new.*Y_new ...
            +cameraParams.TangentialDistortion(2).*(temp+2*X_new.*X_new);
    deltay=2*cameraParams.TangentialDistortion(1)*X_new.*Y_new ...
            +cameraParams.TangentialDistortion(2).*(temp+2*Y_new.*Y_new);
    % 畸变校正
    X_new = (X-deltax)./tempKr;
    Y_new = (Y-deltay)./tempKr;
end

%% 摄像机坐标系->图像坐标系
X_new=X_new*cameraParams.FocalLength(1)+cameraParams.PrincipalPoint(1);
Y_new=Y_new*cameraParams.FocalLength(2)+cameraParams.PrincipalPoint(2);


