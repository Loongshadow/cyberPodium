<?php
/**
 * 数据库连接
 * 为服务端脚本所用
 */
$mysql = mysqli_connect("localhost", "cyberpodium", "sumunian", "cyberpodium");
// 检查连接是否成功
if (!$mysql) {
    die("连接数据库失败：" . mysqli_connect_error());
}
?>