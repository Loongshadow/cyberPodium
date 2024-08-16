<?php
//保存教师对考勤数据的编辑
include "../helper/sqltest.php";

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

if($option == "updateRemark"){//保存教师编辑的考勤备注
    $i = 0;  
    while(1){
        if(isset($data->{$i})){
            $sID = 0;
            $sRemark = "";
            if($i%2==0) 
                $sID=$data->{$i};
            $i++;
            if(isset($data->{$i})){
               $sRemark = $data->{$i}; 
               $sql = "
                    UPDATE `attendance`
                    SET `教师备注` = '$sRemark'
                    WHERE `用户ID` = '$sID' AND `课程ID` = '$courseID'";
                $sqlResult = $mysql->query($sql);
                    
            }else{
                break;
            }
        }else{
            break;
        }
        $i++;
    }
    $responseData = array(   
            'status' => 'default',
            'message' => 'default',  
            'lastsql' => $sqlResult
    );
    header("Content-Type: application/json");
    echo json_encode($responseData);
}

?>