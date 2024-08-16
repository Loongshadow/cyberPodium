服务端代码
架构为LNMP(Linux+Nginx+MySQL+PHP)
代码量：1800 行
以下为文件分布与简介
--------------------------------------------------------------
【文件夹helper】
》公用脚本
sqlHelper.php			用于数据库连接(code: 10 line)
--------------------------------------------------------------
【文件夹And_interface】
》实现学生移动端应用接口
Login.php				实现登录功能(code: 70 line)
Register.php			实现注册功能(code: 120 line)
MainWindMsg.php			获取学生信息和学生所选课程列表(code: 110 line)
sendAttendanceResult.php		判断对应课程是否正在上课，将学生定位打卡结果写入临时考勤表(code: 50 line)
getMyAttendance.php		获取某学生某门课程所有历史考勤记录(code: 60 line)
changeUserMsg,php		实现学生用户修改用户昵称(code: 50 line)
changeCourse.php			实现学生选课、退课(code: 110 line)
--------------------------------------------------------------
【文件夹PC_interface】		
》实现教师桌面端应用接口
Login.php				实现登录功能(code: 70 line)
Register.php			实现注册功能(code: 130 line)
MainWindMsg.php			获取教师信息和教师任课课程列表(code: 120 line)
changeUserMsg.php		实现教师用户修改用户昵称(code: 70 line)
changeCourse.php			实现教师创建课程、删除课程、修改课程(code: 160 line)
changeAttendance.php		实现教师对某课程考勤记录的修改(code: 50 line)
getCourseAttendance.php		获取某门课程所有同学历史考勤记录(code: 60 line)
getCourseCurrentAttendance.php	课程上课时获取当前所有同学考勤记录(code: 60 line)
getCourseMsg.php			获取某门课程的课程信息(code: 60 line)
--------------------------------------------------------------
【文件夹websocket】
》实现websocket连接与管理	
readme_instruction.txt		ws服务端使用说明(打开与关闭)
websocket_server.php		服务器websocket服务端运行脚本（code: 410 line）
clearConnectionKillServer.php		websocket_server.php结束时的工具脚本，杀死websocket_server.php进程，清除数据库ws连接记录(code: 30 line)	
---------------------------------------------------------------
cyberpodium.sql			数据库文件
