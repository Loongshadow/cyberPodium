<?php
//response the course's msg(name time place remark etc) with courseID

include "../helper/sqltest.php";

$jsonString = file_get_contents('php://input');    
$data = json_decode($jsonString);    
if ($data === null) {  
    http_response_code(400);  
    echo "无效的 JSON 数据";  
    exit;  
}  
$courseID = isset($data->courseID) ? (int)$data->courseID : null; 
$debugmsg;


//$sql = "SELECT * FROM 'usertable' WHERE 用户ID = '$userID'";
$sql = "SELECT * FROM `coursetable` WHERE `课程ID` = '$courseID'";
//"SELECT * FROM `usertable` WHERE `用户ID` = '$userID'";
$sqlResult = $mysql->query($sql);

if($sqlResult === false){
     
    $responseData = array(   
            'status' => 'sqlFalse',
            'message' => 'sql command error', 
    );  
    header('Content-Type: application/json');  
    echo json_encode($responseData);
}
else if($sqlResult->num_rows > 0){
    // 获取结果集中的第一行数据  
    $row = $sqlResult->fetch_assoc(); 
    //failure again
    $responseData = array(   
        'status' => 'success',
        'message' => 'getCourseMSG success', 
        'sql' => $sqlResult  ,
        'courseID' => $row['课程ID'] ,
        'courseName' => $row['课程名'] ,
        'place' => $row['地点'] ,
        'time' => $row['时间'] ,
        'remark' => $row['备注']
    );
    header('Content-Type: application/json');  
    echo json_encode($responseData);
        
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
    header('Content-Type: application/json');  
    echo json_encode($responseData);
}


?>