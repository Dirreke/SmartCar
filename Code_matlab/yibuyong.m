%% 去畸变函数1部分函数
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
