<?php

include "../helper/sqltest.php";

/**
 * User: LMR
 * Date: 2024/3/5
 * Time: 上午1:05
 * 实现功能: 注册
 *    1、进入注册界面生成可用用户ID
 *    2、输入账号密码正常注册
 *    3、已存在账号注册失败
 * 待实现功能：
 *    错误提示信息/或由客户端实现
 */


//$userID = $_POST['userID'];
//$passwd = $_POST['password'];
$jsonString = file_get_contents('php://input');  
$data = json_decode($jsonString);  
if ($data === null) {  
    http_response_code(400);  
    echo "无效的 JSON 数据";  
    exit;  
}
$userID = isset($data->userID) ? (int)$data->userID : null; 
$passwd = isset($data->password) ? (int)$data->password : null; 

$debugmsg;
if (filter_var($userID, FILTER_VALIDATE_INT)) {  
        // $userID 是整数  
        $denugmsg = "The userID is an integer.";  
        // 在这里可以继续处理 $userID  
} else {  
        // $userID 不是整数  
        $debugmsg =  "The userID is not an integer.";  
}  

//自动生成合法的新账号

if($userID == 100000000){
    // 获取当前年份  
    $currentYear = date('y'); // 'y' 返回年份的后两位  
    // 获取当前月份，并确保它是两位数（如果需要的话在前面补零）  
    $currentMonth = sprintf("%02d", date('m')); // 使用sprintf补零  
    // 构造九位数ID，头两位是年份后两位，第三第四位是月份  
    $userID = $currentYear . $currentMonth . '00000';
  
    $result;
    $loop = true;
    $sql = "SELECT * FROM usertable";
    $num = 0;
    while($loop){
        $sql = "SELECT * FROM `usertable` WHERE 用户ID = '$userID'";
        //$sql = "SELECT * FROM usertable";
        $sqlResult = $mysql->query($sql);
        if ($sqlResult->num_rows > 0) {
            //$userID++;
            $userID+=1;
        }else {
            $loop = false;
            $result = $sqlResult;
            break;
             
        }
    }
    header('Content-Type: application/json');  
    $responseData = array(   
        'status' => 'success',
        'message' => 'request newID success', 
        'userID' => $userID ,  
        'sql' => $sqlResult ,
        'num' => $num ,
        'debugmsg' => $debugmsg
    );  
    echo json_encode($responseData);
}else{

//正常注册流程

    //判断密码空否
    //$sql = "SELECT * FROM 'usertable' WHERE 用户ID = '$userID'";
    $sql = "SELECT * FROM `usertable` WHERE `用户ID` = '$userID'";
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
    else if($sqlResult->num_rows > 0){
        //注册失败：账号已被占用
        //failure again
        header('Content-Type: application/json');  
        $responseData = array(   
            'status' => 'failure',
            'message' => 'ID exist already', 
            'userID' => $userID ,  
            'sql' => $sqlResult  ,
            'debugmsg' => $debugmsg
        );  
        echo json_encode($responseData);
        
    }else{
        //注册成功
        //success
        //$sql = "INSERT INTO 'usertable' (用户ID, 密码) VALUES ('$userID', '$passwd')";
        $sql = "INSERT INTO `usertable`(`用户ID`,`密码`) VALUES ('$userID','$passwd')";
        $sqlResult = $mysql->query($sql);
        
        header('Content-Type: application/json'); 
        $responseData = array(   
            'status' => 'success',
            'message' => 'register success', 
            'userID' => $userID ,  
            'sql' => $sqlResult  ,
        'debugmsg' => $debugmsg
        );  
        echo json_encode($responseData);
        //success or not
    }
}

?>