<?php

include "../helper/sqltest.php";

/**
 * User: LMR
 * Date: 2024/3/5
 * Time: 上午1:05
 * 实现功能: 学生选课、退课
 *          同步数据库中的考勤记录表
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
$courseID = isset($data->courseID) ? (int)$data->courseID : null; 
$option = isset($data->option) ? $data->option : null; 
$userName = isset($data->userName) ? $data->userName : null; 

//退选课程
if($option == "deletecourse"){
    $sql = "
        DELETE FROM `studentcoursetable`
        WHERE `课程ID` = '$courseID'
        AND `用户ID`='$userID'";
    $sql2 = "
         DELETE FROM `attendance`
         WHERE `课程ID` = '$courseID'
         AND `用户ID`='$userID'";
    $sqlResult = $mysql->query($sql);
    if($sqlResult === false){ 
        $responseData = array(   
            'status' => 'sqlFalse',
            'message' => 'sql command error', 
            'sql' => $sqlResult
        );
    }
    else{
        $mysql->query($sql2); 
        $responseData = array(   
            'status' => 'success',
            'message' => 'delete course success', 
            'sql' => $sqlResult 
        );  
        
    }
}else if($option=="addcourse"){
//选课
    $sql = "
        SELECT * FROM `coursetable`
        WHERE `课程ID` = '$courseID'";
    $sqlResult = $mysql->query($sql);
    if($sqlResult === false){
        $responseData = array(   
            'status' => 'sqlFalse',
            'message' => 'sql command error', 
            'sql' => $sqlResult
        );
    }else if($sqlResult->num_rows > 0){
        $sql ="INSERT INTO `studentcoursetable`(`用户ID`, `课程ID`) VALUES ('$userID','$courseID')";
        $sqlResult = $mysql->query($sql);
        //$sql2 = "INSERT INTO `attendance`(`用户ID`, `课程ID`,`昵称`,`最近更新时间`) VALUES ('$userID','$courseID','$userName','$currentDateTime')";
        if($sqlResult === false){
            $responseData = array(   
                'status' => 'sqlFalse',
                'message' => 'sql command error', 
                'sql' => $sqlResult
            );
        }else{  
            $currentDateTime = date('Y-m-d H:i:s');
            $sql1 = "SELECT * FROM `attendance` WHERE `课程ID` = '$courseID'";
            $myresult = $mysql->query($sql1);
            if($myrow = mysqli_fetch_array($myresult)){
                //$myrow = mysqli_fetch_array($myresult);
                $updateDate = $myrow['最近更新时间'];
                $sql2 = "INSERT INTO `attendance`(`用户ID`, `课程ID`,`昵称`,`最近更新时间`) VALUES ('$userID','$courseID','$userName','$updateDate')";
                $mysql->query($sql2);
            }else{
                $sql2 = "INSERT INTO `attendance`(`用户ID`, `课程ID`,`昵称`,`最近更新时间`) VALUES ('$userID','$courseID','$userName','$currentDateTime')";
                $mysql->query($sql2);
            }
            $responseData = array(   
                'status' => 'success',
                'message' => 'add course already', 
                'sql' => $sqlResult
            );
            
        }
        
        
    }else{
        $responseData = array(   
            'status' => 'failure',
            'message' => 'courseID not found', 
            'sql' => $sqlResult
        );
        
    }
    
}
header("Content-Type: application/json");
echo json_encode($responseData);

?>