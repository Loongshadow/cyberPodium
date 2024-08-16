<?php

include "../helper/sqltest.php";

/**
 * User: LMR
 * Date: 2024/3/5
 * Time: 上午1:05
 * 实现功能: 登录
 * 待实现功能：
 *    错误提示信息/或由客户端实现
 */
// 获取原始请求体  
$jsonString = file_get_contents('php://input');  
$data = json_decode($jsonString);  
// 确保解析成功  
if ($data === null) {  
    http_response_code(400);  
    echo "无效的 JSON 数据";  
    exit;  
}
// 获取 userID，并确保它是一个整数  
$userID = isset($data->userID) ? (int)$data->userID : null; 
$passwd = isset($data->password) ? (int)$data->password : null; 

$debugmsg;


    //$sql = "SELECT * FROM 'usertable' WHERE 用户ID = '$userID'";
    $sql = "SELECT * FROM `usertable` WHERE `用户ID` = '$userID' AND `密码` = '$passwd'";
    //"SELECT * FROM `usertable` WHERE `用户ID` = '$userID'";
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
        // 将响应数据转换为 JSON 格式并输出给客户端
        echo json_encode($responseData);
    }
    else if($sqlResult->num_rows > 0){
        //failure again
        header('Content-Type: application/json');  
        $responseData = array(   
            'status' => 'success',
            'message' => 'login success', 
            'userID' => $userID ,  
            'sql' => $sqlResult  ,
            'debugmsg' => $debugmsg
        );  
        echo json_encode($responseData);
        
    }else{
        //success
        //$sql = "INSERT INTO 'usertable' (用户ID, 密码) VALUES ('$userID', '$passwd')";
        header('Content-Type: application/json'); 
        $responseData = array(   
            'status' => 'failure',
            'message' => 'ID or passwd wrong', 
            'userID' => $userID ,  
            'sql' => $sqlResult  ,
        'debugmsg' => $debugmsg
        );  
        echo json_encode($responseData);
        //success or not
    }

?>