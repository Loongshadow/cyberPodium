<?php
//运转websocket服务器
//与教师端、学生端维持websocket连接
//在教师端、学生端、数据库之间交换信息
//服务器结束运行后，运行clear脚本清除connection所有连接记录
//功能：
//1、教师端、学生端接入，并通过读写表维护连接
//2、学生端发送信息转发给对应教师端
//3  教师端退出后，所有学生端断开连接(通过学生ping来触发/服务器主动断掉) 学生端退出则不影响其他连接
//4  教师端不在连接时，学生端不能接入连接
include "../helper/sqltest.php";



$server = new Swoole\Websocket\Server("0.0.0.0", 9501);

//待实现任务：
//教师端发送信息给所有对应学生端/

//问题：
//学生端首次进入，教师端获取不到考勤信息，学生端出来再进去才有
//学生端退出课程，教师端下课，学生端还会被影响
//多个连接时连接不稳定或不持续（多个连接发心跳包）



//监听连接
$server->on('open', function ($server, $request) {
    global $mysql;
    // 解析客户端传递的 clientID
    $clientID = $request->get['clientID'];
    $courseID = $request->get['courseID'];
    $isTeacher = $request->get['isTeacher'];
    // 将 clientID 与连接对象绑定并保存到连接池中
    if($isTeacher==0){//是学生端
        //A{$courseID} exist or not?stop connect&发送信息:connect
        $dbName = "cyberpodium";
        $tableName = "A" . $courseID;
        $sql = "SELECT * FROM information_schema.tables WHERE table_schema = '" . $dbName . "' AND table_name = '" . $tableName . "';";  
        $result = $mysql->query($sql);  
        if ($result->num_rows > 0) {//数据表存在
            echo "表 {$tableName} 存在\n";  
            saveConnection($clientID, $courseID,$request->fd,$isTeacher);
            $sql2 = "INSERT INTO $tableName (`用户ID`) VALUES ( $clientID)";
            $mysql->query($sql2);
        } else {                    //数据表不存在
            echo "表 {$tableName} 不存在\n";  
            saveConnection($clientID, $courseID,$request->fd,$isTeacher);
            $msg = "BYEBYE";//teacher no online student cant connect the server
            sendToClient($clientID, $courseID,$msg,$isTeacher);
            
        }
        
    }else{//教师端
        saveConnection($clientID, $courseID,$request->fd,$isTeacher);
        newAttendanceTable($courseID);
    }
});

//监听信息接收
$server->on('message', function ($server, $frame) {  
    echo "Received message: {$frame->data}\n";  
    $data = json_decode($frame->data, true);  
      
    if ($data && isset($data['type']) && $data['type'] === 'sendToTeacher') {  
        // 确保clientID和message都存在  
        if (isset($data['clientID'],$data['courseID'], $data['message'])) {  
            //$teacherID = getTeacherConnection($data['clientID'],$data['courseID']);
            //sendToClient(, $data['message'],2);  
            sendToClient($data['clientID'], $data['courseID'],$data['message'],1);
        } else {  
            echo "Invalid message format: missing clientID or message.\n";  
        }  
    } else {  
        // 如果不是sendToClient类型，直接回复发送消息的客户端  
        $server->push($frame->fd, "Hello, {$frame->data}!");  
        //判断学生其教师还在线否，否则断开websocket连接
    }  
});

//监听断开连接
$server->on('close', function ($server, $fd)  {
    // 当连接关闭时，从连接池中移除对应的连接信息
    $clientID = getClientID($fd);
    if ($clientID !== false) {
        // 判断是否为教师端，是则段开所有对应学生端的连接
        $courseID = getCourseID($fd);
        $isTeacher = isTeacherConnection($clientID);
        if($isTeacher===false){
            //普通学生端
            removeConnection($clientID);
        }else{
            //教师端
            deleteCourseByID($courseID);
            //writeAttendance2DB($courseID); 写进 deleteCourseByID函数中 由他执行
        }
    }
});



// RESTful API接口来处理外部发送的消息请求  
// if ($_SERVER['REQUEST_METHOD'] === 'POST' && $_SERVER['REQUEST_URI'] === '/send-message') {  
//     $clientId = $_POST['client_id']; // 假设客户端有一个唯一的ID  
//     $message = $_POST['message'];  
//     sendToClient($clientId,$message);
      
//     echo 'Message sent.';  
//     exit;  
// }  

$server->start();


// 函数：保存连接信息到数据库表connection
function saveConnection($clientID,$courseID,$fd,$isTeacher) {
    global $mysql;
    $date = date('Y-m-d');
    $sql = "
            INSERT INTO connection(`userID`,`courseID`,`fd`,`isTeacher`,`date`)
            VALUES ('$clientID','$courseID','$fd','$isTeacher','$date')";
    $sqlResult = $mysql->query($sql);
    if($sqlResult === false){
        echo "Faile to save the Connection:{$clientID},{$courseID},{$fd}\n";
    }else echo "save the Connection:{$clientID},{$courseID},{$fd}\n";
}

// 函数：从 数据库表connection 中获取连接信息
function getConnection($clientID) {
    global $mysql;
    $sql = "SELECT `fd` FROM connection WHERE `userID` = '$clientID'";
    $sqlResult =$mysql->query($sql);
    if ($sqlResult->num_rows > 0) {  
    // 输出数据  
        $row = $sqlResult->fetch_assoc(); // 获取关联数组
        $fd = $row['fd']; // 获取客户端 ID
        return $fd; 
    } else {  
        echo "No found fd with clientID:{$clientID}\n";  
        return false;
    }  
}

//根据学生ID和课程ID获取对应教师端的连接fd
function getTeacherConnection($clientID,$courseID) {
    global $mysql;
    $isTeacher = 1;
    $sql = "SELECT `fd` FROM connection WHERE `courseID` = '$courseID' AND `isTeacher` = '$isTeacher'";
    $sqlResult =$mysql->query($sql);
    if ($sqlResult->num_rows > 0) {  
    // 输出数据  
        $row = $sqlResult->fetch_assoc(); // 获取关联数组
        $fd = $row['fd']; // 获取客户端 ID
        return $fd; 
    } else {  
        echo "No found fd with teacherclientID:{$clientID}\n";  
        return false;
    }  
}
//根据用户ID判断是不是教师
function isTeacherConnection($clientID) {
    global $mysql;
    $sql = "SELECT `isTeacher` FROM connection WHERE `userID` = '$clientID' ";
    $sqlResult =$mysql->query($sql);
    if ($sqlResult->num_rows > 0) {  
    // 输出数据  
        $row = $sqlResult->fetch_assoc(); // 获取关联数组
        if($row['isTeacher']==1){ 
            echo "{$clientID} is teacher\n";
            return true; // 获取客户端 ID
        }else{ 
            echo "{$clientID} is student\n";
            return false;
            
        } 
    } else {  
        echo "No found fd with teacherclientID:{$clientID}\n";  
        return false;
    }  
}

// 函数：从 DB connection表 中移除连接信息
function removeConnection($clientId) {
    global $mysql;
    $sql = "DELETE FROM connection WHERE `userID` = '$clientId'";  
    $sqlResult =$mysql->query($sql);
    if ($sqlResult!==false) {  
        echo "Delete msg from DB for shuatdown the connection with {$clientId} successful\n";  
    } else {  
        echo "Error deleting record: " . $mysql->error."\n";  
    }
}

// 函数：根据 fd 获取客户端 ID
function getClientID($fd) {
    global $mysql;
    $sql = "SELECT `userID` FROM connection WHERE `fd` = '$fd'";
    $sqlResult =$mysql->query($sql);
    if ($sqlResult->num_rows > 0) {  
    // 输出数据  
        $row = $sqlResult->fetch_assoc(); // 获取关联数组
        $clientID = $row['userID']; // 获取客户端 ID
        echo "getClientID {$clientID} successful with {$fd}\n";
        return  $clientID;
    } else {  
        echo "No found clientID with connection_fd:{$fd}\n";  
        return false;
    }
}

// 函数：根据 fd 获取客户端 ID
function getCourseID($fd) {
    global $mysql;
    $sql = "SELECT `courseID` FROM connection WHERE `fd` = '$fd'";
    $sqlResult =$mysql->query($sql);
    if ($sqlResult->num_rows > 0) {  
    // 输出数据  
        $row = $sqlResult->fetch_assoc(); // 获取关联数组
        $courseID = $row['courseID']; // 获取客户端 ID
        echo "getCourseID {$courseID} successful with {$fd}\n";
        return  $courseID;
    } else {  
        echo "No found courseID with connection_fd:{$fd}\n";  
        return false;
    }
}

// 函数：向指定客户端发送消息
function sendToClient($clientID, $courseID,$message,$isTeacher) {
    // 从连接池中获取客户端的 fd
    global $server;
    $fd = 0;
    if($isTeacher==0)
        $fd = getConnection($clientID);
    else if($isTeacher==1)
        $fd = getTeacherConnection($clientID,$courseID);
    // 如果客户端连接存在，则发送消息
    if ($fd !== false) {
        echo "send {$message} to {$clientID} with {$fd}\n";
        $server->push($fd, $message);
    }else echo "send to {$clientID} faile\n";
}

//从数据库删除所有课程ID为xxx的连接
//并断开对应websocket连接
//used by teacherClient leaving
function deleteCourseByID($courseID) {
    global $server,$mysql;
    $sql2 = "SELECT `fd` FROM connection WHERE `courseID` = '$courseID'";
    $sqlResult2 = $mysql->query($sql2);
    $sql = "DELETE FROM connection WHERE `courseID` = '$courseID'";  
    $sqlResult =$mysql->query($sql);
    if ($sqlResult) {  
        echo "Delete all msg from DB where courseID = {$courseID} successful\n";  
    } else {  
        echo "Error deleting record: " . $mysql->error."\n";  
    }
    if($sqlResult2){
        while($row = mysqli_fetch_array($sqlResult2)){
            $server->close($row['fd']);
            echo "server close the connection:{$row}\n";
        }
    }else{
        echo "no connection with courseID {$courseID}\n";
    }
    usleep(500000); 
    writeAttendance2DB($courseID);
}  

function newAttendanceTable($courseID){
    global $mysql;
    //判断是否存在本课程临时考勤表（待实现）
    //建新的本课程临时考勤表
    $sql = "CREATE TABLE A{$courseID} (  
        用户ID INT(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,  
        记录 INT(1) DEFAULT 0    
    )";
    $sqlResult = $mysql->query($sql);
    if ($sqlResult) {  
        echo "build new attendance table with {$courseID}\n";  
    } else {  
        echo "Faile to build new table:" . $mysql->error."\n";  
    }
}

//教师端退出时
//用临时考勤表更新永久临时表
//删除临时考勤表
function writeAttendance2DB($courseID){
    global $mysql;
    //write into DB
    //step1 计算时间差 以决定更新方式
    $currentDate = date("Y-m-d");
    $sql = "SELECT * FROM attendance WHERE `课程ID` = '$courseID'";
    //$sqlResult = $mysql->query($sql);
    $sqlResult = mysqli_query($mysql,$sql);
    if($sqlResult){
        //$row = $sqlResult->fetch_assoc();
        //$updateDate = $row['最近更新时间'];
        //echo "updatedate:{$updateDate}\n";
        //$date1 = DateTime::createFromFormat('Y-m-d', $currentDate);
        //$row = $sqlResult->fetch_assoc(); // 获取关联数组
        
        //$date2 = DateTime::createFromFormat('Y-m-d', $updateDate);
        //$interval = $date1->diff($updateDate);
        //$days = $interval->days;
        //$days = $c - $updateDate;
        //echo "diff:{$days}\n";
        //echo "date:{$updateDate}\n";
    //step2 
    //遍历 attendance
    //对每一项：在临时表中，获取临时表数据,
    //          0天临表数据=0->不改  0天临表数据!=0->attendance表置1
    //          ?0天临表数据append进attendance表格  改最近更新时间
    //该项不在临表中：
    //          0天 不改
    //          ?天 attendance表格append 0  该最近修改时间
        //$sqlvisitAtten = "SELECT * FROM attendance WHERE `课程ID` = '$courseID'";
        //$sqlvisitAttenResult = $mysql->query($sqlvisitAtten);
        //遍历attendance $courseID对应的所有学生考勤记录
            $updateDate;
            $getTime = false;
            $days = 0;
            echo "line 321\n";
            while($myrow = mysqli_fetch_array($sqlResult)){
                if(!$getTime){
                    $getTime = true;
                    $updateDate = $myrow['最近更新时间'];
                    $date1 = DateTime::createFromFormat('Y-m-d', $currentDate);
                    $date2 = DateTime::createFromFormat('Y-m-d', $updateDate);
                    $interval = $date1->diff($date2);
                    $days = $interval->days;
                    echo "diff:{$days}\n";
                    echo "date:{$updateDate}\n";
                }
                echo "line334\n";
                $stuID = $myrow['用户ID'];
                $record = $myrow['记录'];
                $sqlinTable = "SELECT * FROM A{$courseID} WHERE `用户ID` = '$stuID' "; //查找该同学是否有上课/是否在临时考勤表中
                $sqlinTableResult = $mysql->query($sqlinTable);
                echo "line336\n";
                if($sqlinTableResult->num_rows > 0){//该生在临时表中
                    $inrow = $sqlinTableResult->fetch_assoc();
                    $inrecord = $inrow['记录'];
                    echo "line340\n";
                    if($days==0){//【1】学生在表 且0天
                        echo "line342\n";
                        if($inrecord!=0){//$inrecord != 0 改
                            //$record 最后数字质1
                            // 使用逗号分隔字符串，得到数组；将数组的最后一个元素设置为 1；使用逗号将数组元素连接成字符串
                            $recordArray = explode(",", $record);
                            if($recordArray[count($recordArray) - 1] == 0){//需要置1
                                $recordArray[count($recordArray) - 1] = 1;
                                $newRecord = implode(",", $recordArray);
                                echo "updaterecord:{$newRecord}";
                                $sqlUpdate = "
                                    UPDATE attendance
                                    SET `最近更新时间` = '$currentDate', 
                                        `记录` = '$newRecord' 
                                    WHERE `用户ID` = '$stuID' AND `课程ID` = '$courseID'";
                                $mysql->query($sqlUpdate);
                                
                            }//else 无需修改
                        }//else $inrecord == 0 则不改
                    }else{//【2】学生在表 非0天 临表数据append进attendance表格  改最近更新时间
                        //append
                        $newRecord = $record.",".$inrecord;
                        $sqlUpdate = "
                                UPDATE attendance 
                                SET `最近更新时间` = '$currentDate', 
                                    `记录` = '$newRecord' 
                                WHERE `用户ID` = '$stuID' AND `课程ID` = '$courseID'";
                        $mysql->query($sqlUpdate);
                        
                    }
                    
                }else{//该生不在临时表中
                    if($days!=0){//【3】不在表 非0天 attendance表格append 0  该最近修改时间
                        $newRecord = $record.",0";
                        $sqlUpdate = "
                                UPDATE attendance 
                                SET `最近更新时间` = '$currentDate', 
                                    `记录` = '$newRecord' 
                                WHERE `用户ID` = '$stuID' AND `课程ID` = '$courseID'";
                        $mysql->query($sqlUpdate);
                    }//【4】else $days == 0 不改
                }
                
            }
            echo "line385\n";
            
        
        
    }else{
        echo "herehere".$mysql->error."\n";
    }
    
    
    
    //destray the table 
    $sql2 = "
        DROP TABLE A{$courseID};
    ";
    $sqlResult2 = $mysql->query($sql2);
    if ($sqlResult2) {  
        echo "drop table A{$courseID} already\n";  
    } else {  
        echo "Faile to drop table:" . $mysql->error."\n";  
    }
}
  