%% ͼ������
% 40�� 80��
% �У�40-80
% �У�15-173 ����
y=40:79;
y=y'*ones(1,80);
x=15:2:173;
x=ones(40,1)*x;

%% ͼ������ϵ->�������ϵ
% x,y�����ȥ��ǰ���� 
% X,Yͼ������ϵ����
X=(x-cameraParams.PrincipalPoint(1))/cameraParams.FocalLength(1);
Y=(y-cameraParams.PrincipalPoint(2))/cameraParams.FocalLength(2);

%% ȥ�����ϵ��
X=X-Y*cameraParams.Skew;

%% ����k1k2p1p2ȥ�������������
% ��ֵ
X_new = X;
Y_new = Y;
% ����
for i =1:20000
    % ����
    temp=X_new.*X_new+Y_new.*Y_new;
    tempKr=1+cameraParams.RadialDistortion(1).*temp ...
            +cameraParams.RadialDistortion(2).*temp.^2 ...
            +cameraParams.RadialDistortion(3).*temp.^3;
    % ����
    deltax=2*cameraParams.TangentialDistortion(1)*X_new.*Y_new ...
            +cameraParams.TangentialDistortion(2).*(temp+2*X_new.*X_new);
    deltay=2*cameraParams.TangentialDistortion(1)*X_new.*Y_new ...
            +cameraParams.TangentialDistortion(2).*(temp+2*Y_new.*Y_new);
    % ����У��
    X_new = (X-deltax)./tempKr;
    Y_new = (Y-deltay)./tempKr;
end

%% ���������ϵ->ͼ������ϵ
X_new=X_new*cameraParams.FocalLength(1)+cameraParams.PrincipalPoint(1);
Y_new=Y_new*cameraParams.FocalLength(2)+cameraParams.PrincipalPoint(2);


