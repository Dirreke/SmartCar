function []=Pic_DrawMid_und()
count = 0;
global ImageH New_Lef New_Rig New_Mid MIDMAP ROAD_HALF_WIDTH;
New_Mid=zeros(ImageH,1);
flag=0;
for i = 1:60
    if New_Lef(i) ~= -MIDMAP && New_Rig(i) ~= MIDMAP %Mid Calculaing
        New_Mid(i) = round((New_Lef(i) + New_Rig(i))/2);
        flag = 1;
    elseif New_Lef(i) == -MIDMAP && New_Rig(i) ~= MIDMAP
        New_Mid(i) = New_Rig(i) - ROAD_HALF_WIDTH;
        flag = 1;
    elseif New_Lef(i) ~= -MIDMAP && New_Rig(i) == MIDMAP
        New_Mid(i) = New_Lef(i) + ROAD_HALF_WIDTH;
        flag = 1;
    elseif flag==1
        count=count+1; %for ²åÖµ
        flag = 0;
    end
    if count ~= 0 && flag == 1 %²åÖµ
        slope_temp = (New_Mid(i) - New_Mid(i - count - 1)) / (count + 1);
        for  j = count:1
            New_Mid(i - j) = round(New_Mid(i) - slope_temp * j + 0.5);
        end
    end
end
end
