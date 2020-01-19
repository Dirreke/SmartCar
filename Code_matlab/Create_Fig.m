function []=Create_Fig()
global New_Lef New_Rig New_Mid;
figure
plot(New_Lef,60:-1:1)
hold on
plot(New_Rig,60:-1:1)
plot(New_Mid,60:-1:1)
end