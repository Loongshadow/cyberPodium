<?php

include "../helper/sqltest.php";

/**
 * User: LMR
 * Date: 2024/3/5
 * Time: 上午1:05
 * 实现功能: 修改用户信息
 * 待实现功能：
 *    错误提示信息/或由客户端实现
 */ 
$jsonString = file_get_contents('php://input');   
$data = json_decode($jsonString);   
if ($data === null) {  
    http_response_code(400);  
    echo "无效的 JSON 数据";  
    exit;  
} 
$ID = isset($data->ID) ? (int)$data->ID : null; 
$name = isset($data->Name) ? $data->Name : null; 
$wechat = isset($data->Wechat) ? $data->Wechat : null; 
$qq = isset($data->QQ) ? $data->QQ : null;
$phone = isset($data->Phone) ? $data->Phone : null;
$office = isset($data->Office) ? $data->Office : null;
$email = isset($data->EMail) ? $data->EMail : null;
$notes = isset($data->Notes) ? $data->Notes : null;

$debugmsg;


$sql = "
        UPDATE `usertable`
        SET `用户名` = '$name',
            `微信号` = '$wechat',
            `QQ` = '$qq',
            `邮箱` = '$email',
            `电话` = '$phone',
            `办公室` = '$office',
            `备注` = '$notes'
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
        //failure again
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