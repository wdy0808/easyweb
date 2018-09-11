<?php
session_start();
$_SESSION['lasturl'] = '../../paint/newpaint.php';
if(!isset($_SESSION['login']))
{
    header('Location: ../user/login/login.php');
}
?>
<html>
<head>
<title>新建画布</title>
<style>
body {
    margin: 250px auto;
    width: 200px;
}
</style>
</head>
<body>
    <form action="./paint.php" method="post">
        <p>画布名字：<input type="text" name="canvasname"></p>
        <input type="radio" name="authority" value="public" checked="checked"/>public
        <input type="radio" name="authority" value="private" />private
        <input type="submit" value='添加'>
    </form>
</body>
</html>