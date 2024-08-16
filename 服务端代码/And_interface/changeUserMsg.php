<?php
//功能：
//实现安卓学生用户修改用户昵称
//同步实现DB 用户表userTbale 和考勤表attendance
include "../helper/sqltest.php";

$jsonString = file_get_contents('php://input');  
$data = json_decode($jsonString);  
if ($data === null) {  
    http_response_code(400);  
    echo "无效的 JSON 数据";  
    exit;  
} 
$ID = isset($data->ID) ? (int)$data->ID : null; 
$name = isset($data->Name) ? $data->Name : null; 

$sql = "
        UPDATE `usertable`
        SET `用户名` = '$name'
        WHERE `用户ID` = '$ID'  ";
$sqlResult = $mysql->query($sql);

if($sqlResult === false){
        header('Content-Type: application/json');  
        $responseData = array(   
            'status' => 'sqlFalse',
            'message' => 'sql command error', 
            'userID' => $userID ,  
            'sql' => $sqlResult  ,
            'debugmsg' => $debugmsg
        );  
        echo json_encode($responseData);
}
else{
        header('Content-Type: application/json');  
        $responseData = array(   
            'status' => 'success',
            'message' => 'change userMSG success', 
            'userID' => $userID ,  
            'sql' => $sqlResult  ,
            'debugmsg' => $debugmsg
        );  
        echo json_encode($responseData);
        
        $attsql = "
            UPDATE `attendance`
            SET `昵称` = '$name'
            WHERE `用户ID` = '$ID'";
        $mysql->query($attsql);
}

?>