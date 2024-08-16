<?php
//学生端获取本课程本人所有历史考勤记录

include "../helper/sqltest.php";

$jsonString = file_get_contents('php://input');  
$data = json_decode($jsonString);   
if ($data === null) {  
    http_response_code(400);  
    echo "无效的 JSON 数据";  
    exit;  
}
// 获取 courseID
$courseID = isset($data->courseID) ? (int)$data->courseID : null;
$userID = isset($data->userID) ? (int)$data->userID : null; 

$debugmsg;

//$sql = "SELECT * FROM 'usertable' WHERE 用户ID = '$userID'";
$sql = "SELECT * FROM `attendance` WHERE `用户ID` = '$userID' AND `课程ID` = '$courseID'";
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
    header('Content-Type: application/json');  
    echo json_encode($responseData);
}
else if($sqlResult->num_rows > 0){
    // 获取结果集中的第一行数据  
    $row = $sqlResult->fetch_assoc(); 
     
    $responseData = array(   
        'status' => 'success',
        'message' => 'get attendance MSG success', 
        'sql' => $sqlResult  ,
        'record' => $row['记录'] 
    );
    header('Content-Type: application/json');  
    echo json_encode($responseData);
        
}else{
//success
//$sql = "INSERT INTO 'usertable' (用户ID, 密码) VALUES ('$userID', '$passwd')";
    
    $responseData = array(   
        'status' => 'failure',
        'message' => 'sth wrong', 
        'userID' => $userID ,  
        'sql' => $sqlResult  ,
        'debugmsg' => $debugmsg
    );  
    header('Content-Type: application/json');  
    echo json_encode($responseData);
}
?>

