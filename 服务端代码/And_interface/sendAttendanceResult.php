<?php

include "../helper/sqltest.php";

//功能：
//接收 安卓学生用户上课时的打卡结果：成功/失败
//查询临时考勤表，若存在，将考勤结果写入
//临时考勤表不在，说明教师未上课，通知学生端下线


$jsonString = file_get_contents('php://input');   
$data = json_decode($jsonString);   
if ($data === null) {  
    http_response_code(400);  
    echo "无效的 JSON 数据";  
    exit;  
}
$userID = isset($data->userID) ? (int)$data->userID : null; 
$courseID = isset($data->courseID) ? (int)$data->courseID : null; 
$attResult = isset($data->attResult) ? (int)$data->attResult : null;


//查询课程对应临时考勤表是否存在
$dbName = "cyberpodium";
$tableName = "A" . $courseID;
$tablesql = "SELECT * FROM information_schema.tables WHERE table_schema = '" . $dbName . "' AND table_name = '" . $tableName . "';";  
$tableresult = $mysql->query($tablesql);  
if ($tableresult->num_rows > 0) {   //数据表存在
    //attResult == 0/1
    $sql2 = "INSERT INTO $tableName (`用户ID`) VALUES ( $clientID)";
    $mysql->query($sql2);
    $sql = "
        UPDATE $tableName
        SET `记录` = '$attResult'
        WHERE `用户ID` = '$userID'";
    $sqlResult = $mysql->query($sql);
    
    $responseData = array(   
            'status' => 'OK',
            'message' => 'OK to update table', 
            'sql' => $sqlResult
        );
    header("Content-Type: application/json");
    echo json_encode($responseData);
} else {                            //数据表不存在
    $responseData = array(   
            'status' => 'NO',
            'message' => 'teacher no online'
        );
    header("Content-Type: application/json");
    echo json_encode($responseData);
}





?>