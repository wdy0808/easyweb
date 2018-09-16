<?php
session_start();
$_SESSION['lasturl'] = '../../paint/newpaint.php';
if(!isset($_SESSION['username']))
{
    header('Location: ../user/login/login.php');
}
?>
<html>
<head>
<title>新建画布</title>
<style>
body {
    background-color: rgb(241, 233, 233);
}
#main {
    margin: 200px auto;
    width: 240px;
    padding: 15px;
}
#inputname {
    background-color:transparent;
}
form:last-child {
    display: block;
}
</style>
</head>
<body>
    <div id="main">
        <form action="./addpaint.php" method="post">
            <p>画布名字：<input type="text" name="canvasname" id="inputname"></p>
            <input type="radio" name="authority" value="public" checked="checked"/>公开访问
            <input type="radio" name="authority" value="protected" />限制访问
            <input type="radio" name="authority" value="private" />私密
            <input type="submit" value='添加画布'>
        </form>
    </div>
</body>
</html>