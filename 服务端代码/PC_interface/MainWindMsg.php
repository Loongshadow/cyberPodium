<?php

include "../helper/sqltest.php";

/**
 * User: LMR
 * Date: 2024/3/5
 * Time: 上午1:05
 * 实现功能: 获取用户信息和用户课程信息
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
$userID = isset($data->userID) ? (int)$data->userID : null; 


$debugmsg;

//获取用户信息

//$sql = "SELECT * FROM 'usertable' WHERE 用户ID = '$userID'";
$sql = "SELECT * FROM `usertable` WHERE `用户ID` = '$userID'";
//"SELECT * FROM `usertable` WHERE `用户ID` = '$userID'";
$sqlResult = $mysql->query($sql);
if($sqlResult === false){
    
    $responseData = array(   
            'status' => 'sqlFalse',
            'message' => 'sql command error', 
            'userID' => $userID ,  
            'sql' => $sqlResult  ,
            'debugmsg' => $debugmsg
    );  
    //echo json_encode($responseData);
}
else if($sqlResult->num_rows > 0){
    // 获取结果集中的第一行数据  
    $row = $sqlResult->fetch_assoc(); 
    //failure again
      
    $responseData = array(   
        'status' => 'success',
        'message' => 'getMSG success', 
        'sql' => $sqlResult  ,
        'userID' => $row['用户ID'] ,
        'username' => $row['用户名'] ,
        'wechat' => $row['微信号'] ,
        'QQ' => $row['QQ'] ,
        'mail' => $row['邮箱'] ,
        'telenumber' => $row['电话'] ,
        'office' => $row['办公室'] ,
        'notes' => $row['备注']
    );
    //echo json_encode($responseData);
        
}else{
//success
//$sql = "INSERT INTO 'usertable' (用户ID, 密码) VALUES ('$userID', '$passwd')";
    
        $responseData = array(   
        'status' => 'failure',
        'message' => 'ID wrong', 
        'userID' => $userID ,  
        'sql' => $sqlResult  ,
        'debugmsg' => $debugmsg
    );  
    //echo json_encode($responseData);        //success or not
}

//继续查用户所拥有课程

$sql = "
        SELECT `coursetable`.*  
        FROM `coursetable`  
        JOIN `teachercoursetable` ON `coursetable`.`课程ID` = `teachercoursetable`.`课程ID`
        JOIN `usertable` ON `teachercoursetable`.`用户ID` = `usertable`.`用户ID`  
        WHERE `usertable`.`用户ID` = '$userID'  ";
$sqlResult = $mysql->query($sql);
  

if($sqlResult === false){
    header('Content-Type: application/json');  
    $responseData['status2'] = 'sqlFalse';
    $responseData['message2'] = 'sql command error';
    $responseData['userID2'] = $userID;
    $responseData['sql2'] =  $sqlResult;
    $responseData['debugmsg2'] = $debugmsg;
    echo json_encode($responseData);
}else if($sqlResult){
    // 遍历结果集  
    //if($sqlResult->num_rows == 1||$sqlResult->num_rows == 0){
    if($sqlResult->num_rows == 0){
        $responseData["sqlresult"] = $sqlResult->fetch_assoc();
        header('Content-Type: application/json');  
        echo json_encode($responseData);
    }else if($sqlResult->num_rows == 1){
        $responseData[0] = mysqli_fetch_array($sqlResult);
        header('Content-Type: application/json');  
        echo json_encode($responseData);   
    }else{
        $i=-1;
        while($row = mysqli_fetch_array($sqlResult)){
            $i++;
            $responseData[$i] = $row;
        }
        header('Content-Type: application/json');  
        echo json_encode($responseData);
    }
}
?>