学生移动端代码
代码量：1040 行（不计入ui）
以下为文件分布

--------------------------
登录注册(code: 190 line)
LoginRegister.kt
activity_loginregister.xml
--------------------------
主界面(code: 340 line)
实现：显示用户信息和课程列表，支持用户选课退课、进入课程、开始上课
MainActivity.kt
activity_main.xml
--------------------------
进入课程(code: 100 line)
实现：显示个人历史考勤记录
inClass.kt
inclass.xml
--------------------------
上课(code: 370 line)
实现：定位打卡考勤、编辑发送弹幕
CyberClass.kt
activity_cyberclass.xml
--------------------------
websocket支持(code: 40 line)
实现：提供websocket连接支持
MyWebSocket.java