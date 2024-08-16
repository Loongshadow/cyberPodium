教师桌面端代码
代码量：2040 行（ui不计入）
以下是文件分布与简介

--------------------------
【主函数(code: 10 line)
main.cpp 
--------------------------
【登录注册(code: 260 line)
logwidget.cpp 
logwidget.h
logwidget.ui
--------------------------
【主界面(code: 500 line)
实现：显示教师信息、任课课表，能进行添加课程、删除课程、进入课程、开始上课
mainwindow.cpp 
mainwindow.h
mainwindow.ui
--------------------------
【进入课程(code: 470 line)
实现：查看与修改课程信息&查看与修改学生考勤记录
inclass.cpp 
inclass.h
inclass.ui
--------------------------
【上课 (code: 560 line)
实现：弹幕播放与管理、悬浮球、查看考勤、工具箱
classing.cpp
classing.h
classing.ui
--------------------------
【实时考勤(code: 280 line)
上课时查看本课程当前考勤记录
attendance.cpp
attendance.h 
attendance.ui
--------------------------
【工具箱(code: 60 line)
实现：随机抽签点名
toolbox.cpp 
toolbox.h
toolbox.ui
--------------------------
【其他文件
imgPool文件夹：		软件图片资源
CyberPodium_PC.pro:	项目文件
img.qrc			xml格式的资源配置文件
logo.rc			软件logo
--------------------------