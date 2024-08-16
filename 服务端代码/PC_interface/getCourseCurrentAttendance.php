<?php
//教师端获取临时考勤表本课所有同学历史考勤记录
//教师端获取临时考勤表本课所有同学名单

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

$debugmsg;

//$sql = "SELECT * FROM 'usertable' WHERE 用户ID = '$userID'";
$sql = "SELECT * FROM A{$courseID}";
//"SELECT * FROM `usertable` WHERE `用户ID` = '$userID'";
$sqlResult = $mysql->query($sql);
if($sqlResult === false){ 
    $responseData = array(   
            'status' => 'sqlFalse',
            'message' => 'sql command error', 
            'courseID' => $courseID ,  
            'sql' => $sqlResult  ,
            'debugmsg' => $debugmsg
    );
    header('Content-Type: application/json');  
    echo json_encode($responseData);
}else if($sqlResult){
    $responseData = array(   
            'status' => 'success',
            'message' => 'get course current attendance successfull', 
            'courseID' => $courseID ,  
            'sql' => $sqlResult  ,
            'debugmsg' => $debugmsg
    ); 
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

