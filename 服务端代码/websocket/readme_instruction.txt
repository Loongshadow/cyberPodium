CentOS系统下 websocket使用流程：
【方法一】
//启动websocket server
php websocket_server.php 
//CTRL + C 关闭server
//清空数据库表中的连接，杀死所有websocket_server进程
php clearConnectionKillServer.php

【方法二】
//启动websocket server后台运行
nohup websocket_server.php &
//清空数据库表中的连接，杀死所有websocket_server进程
php clearConnectionKillServer.php