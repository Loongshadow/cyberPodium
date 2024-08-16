<?php
//杀死websocket_server.php进程
//在DB connection表删除ws连接记录

include "../helper/sqltest.php";

// 执行系统命令
exec("ps aux | grep websocket_server.php", $output);

// 遍历输出数组
foreach ($output as $line) {
    // 使用正则表达式匹配进程号
    if (preg_match("/\w+\s+(\d+).+websocket_server\.php/", $line, $matches)) {
        // 获取进程号
        $pid = $matches[1];
        // 终止进程
        exec("kill $pid");
        echo "Process with PID $pid killed.\n";
    }
}


    
$sql = "DELETE FROM connection";  
$sqlResult = $mysql->query($sql);
if ($sqlResult !== false) {  
    echo "All connection records removed from DB successful\n";  
} else {  
    echo "Error removing connection records: " . $mysql->error . "\n";  
}


?>
