<?php

include "../helper/sqltest.php";

/**
 * User: LMR
 * Date: 2024/3/5
 * Time: 上午1:05
 * 实现功能: 创建课程、删除课程、修改课程
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


$debugmsg;

//删除课程

if($option == "deletecourse"){
    $sql = "
        DELETE FROM coursetable
        WHERE `课程ID` = '$courseID'  ";
    $sqlResult = $mysql->query($sql);
    if($sqlResult === false){
        $responseData = array(   
            'status' => 'sqlFalse',
            'message' => 'sql command error', 
            'userID' => $userID ,  
            'courseID' => $courseID ,
            'sql' => $sqlResult  ,
            'debugmsg' => $debugmsg
        );  
    }
    else{
        $responseData = array(   
            'status' => 'success',
            'message' => 'delete course success', 
            'userID' => $userID ,  
            'sql' => $sqlResult  ,
            'debugmsg' => $debugmsg
        );  
        
    }
}else if($option=="addcourse"){

//创建课程

    // 获取当前年份  
    $currentYear = date('y'); // 'y' 返回年份的后两位
    // 构造九位数ID，头两位是年份后两位，第三第四位是月份  
    $courseID = $currentYear . '0000001';
    $loop = true;
    while($loop){
        $sql = "SELECT * FROM `coursetable` WHERE `课程ID` = '$courseID'";
        //$sql = "SELECT * FROM usertable";
        $sqlResult = $mysql->query($sql);
        if ($sqlResult->num_rows > 0) {
            //$userID++;
            $courseID+=1;
        }else {
            $loop = false;
            $result = $sqlResult;
            break;
        }
    }
    
    
    $cname = isset($data->cname) ? $data->cname : null;
    $ctime = isset($data->ctime) ? $data->ctime : null;
    $clocation = isset($data->clocation) ? $data->clocation : null;
    $cbuilder = isset($data->cbuilder) ? $data->cbuilder : null;
    $cremark = isset($data->cnote) ? $data->cnote : null;
    $zero = "0";
    $debugmsg+=$courseID;
    $sql = "
        INSERT INTO coursetable(`课程ID`,`课程名`,`时间`,`地点`,`创建者`,`备注`)
        VALUES ('$courseID','$cname','$ctime','$clocation','$cbuilder','$cremark')";
    $sqlResult = $mysql->query($sql);
    if($sqlResult === false){ 
        $responseData = array(   
            'status' => 'sqlFalse',
            'message' => 'sql command error/insert coursetable', 
            'userID' => $userID ,  
            'sql' => $sqlResult  ,
            'debugmsg' => $debugmsg
        );  
    }
    else{ 
        $sql ="INSERT INTO `teachercoursetable`(`用户ID`, `课程ID`, `助教备注`) VALUES ('$userID','$courseID','$zero')";
        $sqlResult = $mysql->query($sql);
        if($sqlResult === false){
            $responseData = array(   
            'status' => 'sqlFalse',
            'message' => 'sql command error/insert teachercoursetable', 
            'userID' => $userID ,  
            'sql' => $sqlResult  ,
            'debugmsg' => $debugmsg
            );  
        }
        else{
        $responseData = array(   
            'status' => 'success',
            'message' => 'add course success', 
            'userID' => $userID ,  
            'courseID' => $courseID ,
            'sql' => $sqlResult  ,
            'debugmsg' => $debugmsg
            );  
        
        }
        
    }
    
    
}else if($option=="alertcourse"){

//修改课程信息

    $cname = isset($data->cname) ? $data->cname : null;
    $ctime = isset($data->ctime) ? $data->ctime : null;
    $clocation = isset($data->clocation) ? $data->clocation : null;
    $cremark = isset($data->cnote) ? $data->cnote : null;
    $sql = "
        UPDATE `coursetable`
        SET `课程名` = '$cname',
            `时间` = '$ctime',
            `地点` = '$clocation',
            `备注` = '$cremark'
        WHERE `课程ID` = '$courseID'  ";
    $sqlResult = $mysql->query($sql);
    if($sqlResult === false){ 
        $responseData = array(   
            'status' => 'sqlFalse',
            'message' => 'sql command error', 
            'courseID' => $courseID ,  
            'sql' => $sqlResult  ,
            'debugmsg' => $debugmsg
        );  
    }   
    else{  
        $responseData = array(   
            'status' => 'success',
            'message' => 'change courseMSG success', 
            'courseID' => $courseID ,  
            'sql' => $sqlResult  ,
            'debugmsg' => $debugmsg
        ); 
    }
}
header("Content-Type: application/json");
echo json_encode($responseData);

?>