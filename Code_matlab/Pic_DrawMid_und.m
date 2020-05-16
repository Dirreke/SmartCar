function []=Pic_DrawMid_und()
global ImageH New_Lef New_Rig New_Mid MIDMAP ROAD_HALF_WIDTH;
New_Mid=zeros(ImageH,1);
flag = 0;
inter_point = 0;
flag = 0;
inter_count = 0;
LT=0;
RT=0;

if LT%(Road0_flag == 3 && Road == 0) || Road == 1
    for i=1:60
        if New_Rig(i) ~= MIDMAP
            New_Mid(i) = New_Rig(i) - ROAD_HALF_WIDTH;
            if flag == 0 || inter_count == 0
                inter_count = 1; %第一个点直接continue，flag==0意味着每断点直接continue
                continue;
            end
        else
            inter_point=inter_point+1; %for 插值
            if flag==1
                if i==60
                    for j=1:60
                        New_Mid(j) = 999;
                    end
                end
                continue;
            end
            if i == 1   %开头断点，下一个有值点后插值
                flag = 1;
                continue;
            elseif i ~= 60  %结尾断点，直接进插值
                flag = 2;
                continue;
            else %中间断点
                flag = 3;
            end
        end
        if flag == 2 %插值 内插
            slope_temp = (New_Mid(i) - New_Mid(i - inter_point - 1) )/ (inter_point + 1);
            for j = inter_point:-1:1
                New_Mid(i-j) = round(New_Mid(i) - slope_temp * j);
            end
            flag = 0;
        elseif flag == 3 %外推
            slope_temp = New_Mid(i - inter_point) - New_Mid(i - inter_point - 1);
            for j = inter_point - 1:-1:0
                New_Mid(i-j) = round(New_Mid(i - inter_point) + slope_temp * (inter_point - j));
            end
            flag = 0;
        elseif flag == 1 %外推
            slope_temp = New_Mid(i) - New_Mid(i - 1);
            for j = inter_point + 1:-1:2
                New_Mid(i-j) = round(New_Mid(i) - slope_temp * j );
            end
            flag = 0;
        end
    end
elseif RT%(Road0_flag == 4 && Road == 0) || Road == 2
    for i=1:60
        if New_Lef(i) ~= MIDMAP
            New_Mid(i) = New_Lef(i) + ROAD_HALF_WIDTH;
            if flag == 0 || inter_count == 0
                inter_count = 1; %第一个点直接continue，flag==0意味着每断点直接continue
                continue;
            end
        else
            inter_point=inter_point+1; %for 插值
            if flag==1
                if i==60
                    for j=1:60
                        New_Mid(j) = 999;
                    end
                end
                continue;
            end
            if i == 1   %开头断点，下一个有值点后插值
                flag = 1;
                continue;
            elseif i ~= 60  %结尾断点，直接进插值
                flag = 2;
                continue;
            else %中间断点
                flag = 3;
            end
        end
        
        if flag == 2 %插值 内插
            slope_temp = (New_Mid(i) - New_Mid(i - inter_point - 1) )/ (inter_point + 1);
            for j = inter_point:-1:1
                New_Mid(i-j) = round(New_Mid(i) - slope_temp * j);
            end
            flag = 0;
        elseif flag == 3 %外推
            slope_temp = New_Mid(i - inter_point) - New_Mid(i - inter_point - 1);
            for j = inter_point - 1:-1:0
                New_Mid(i-j) = round(New_Mid(i - inter_point) + slope_temp * (inter_point - j));
            end
            flag = 0;
        elseif flag == 1 %外推
            slope_temp = New_Mid(i) - New_Mid(i - 1);
            for j = inter_point + 1:-1:2
                New_Mid(i-j) = round(New_Mid(i) - slope_temp * j );
            end
            flag = 0;
        end
    end
else
    for i=1:60
        if New_Lef(i) ~= -MIDMAP && New_Rig(i) ~= MIDMAP %Mid Calculaing
            New_Mid(i) = round((New_Lef(i) + New_Rig(i)) / 2.0);
            if flag == 0 || inter_count == 0
                inter_count = 1; %第一个点直接continue，flag==0意味着每断点直接continue
                continue;
            end
        elseif New_Lef(i) == -MIDMAP && New_Rig(i) ~= MIDMAP
            New_Mid(i) = New_Rig(i) - ROAD_HALF_WIDTH;
            if flag == 0 || inter_count == 0
                inter_count = 1; %第一个点直接continue，flag==0意味着每断点直接continue
                continue;
            end
        elseif New_Lef(i) ~= -MIDMAP && New_Rig(i) == MIDMAP
            New_Mid(i) = New_Lef(i) + ROAD_HALF_WIDTH;
            if flag == 0 || inter_count == 0
                inter_count = 1; %第一个点直接continue，flag==0意味着每断点直接continue
                continue;
            end
        else
            inter_point=inter_point+1; %for 插值
            if flag==1
                if i==60
                    for j=1:60
                        New_Mid(j) = 999;
                    end
                end
                continue;
            end
            if i == 1   %开头断点，下一个有值点后插值
                flag = 1;
                continue;
            elseif i ~= 60  %结尾断点，直接进插值
                flag = 2;
                continue;
            else %中间断点
                flag = 3;
            end
        end
        if flag == 2 %插值 内插
            slope_temp = (New_Mid(i) - New_Mid(i - inter_point - 1) )/ (inter_point + 1);
            for j = inter_point:-1:1
                New_Mid(i-j) = round(New_Mid(i) - slope_temp * j);
            end
            flag = 0;
        elseif flag == 3 %外推
            slope_temp = New_Mid(i - inter_point) - New_Mid(i - inter_point - 1);
            for j = inter_point - 1:-1:0
                New_Mid(i-j) = round(New_Mid(i - inter_point) + slope_temp * (inter_point - j));
            end
            flag = 0;
        elseif flag == 1 %外推
            slope_temp = New_Mid(i) - New_Mid(i - 1);
            for j = inter_point + 1:-1:2
                New_Mid(i-j) = round(New_Mid(i) - slope_temp * j );
            end
            flag = 0;
        end
    end
end
end


