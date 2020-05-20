% H00=70.3;
% D00=67;
H00=75;
D00=75;
H0=H00;
ku1=-.1;
ku11=1;
i=1;
clear jilu
while H0<100
    ku1=ku1+0.1;
    ku11=-ku11;
    H0=H0+ku1*ku11;
    D0=D00;
    ku2=-1;
    ku22=1;
    flag=0;
    while D0<100

        ku2=ku2+1;
        ku22=-ku22;
        D0=D0+ku2*ku22;
        try Pic_undistort
        catch
            continue;
        end
        if abs(New_Lef(1)-New_Lef(58))<10 &&abs(New_Rig(1)-New_Rig(58))<10
            flag=1;
            break;
        end
    end
    if flag==0
        continue;
    else
       
        ttempp=[tempNewy(53),tempNewy(43),tempNewy(22)]+D0;
        ttempp=ttempp./[13.5 19 64];
        if ttempp(3)/ttempp(1)<1.7 && ttempp(3)/ttempp(1)>0.58 
            jilu(1:3,i)=ttempp';
            jilu(4:5,i)=[H0;D0];
            i=i+1;
        end
    end 
end