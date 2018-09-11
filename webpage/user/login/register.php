<?php
session_start();
if (isset($_POST['username']))
{
    $con = mysqli_connect("localhost","root","0101,wdy.","easyweb");
    $errmsg = '';
    $username = $_POST['username'];
    $password = $_POST['password'];
    if (mysqli_connect_errno($con))
    {
        $errmsg = mysqli_connect_error();
    }
    else 
    {
        $sql="INSERT INTO user (username, password) VALUES ('$_POST[username]','$_POST[password]')";
        if (!mysqli_query($con,$sql)) {
            $errmsg = '用户名已注册';
        }
        else 
        {
            $_SESSION['username'] = $_POST['username'];
            $_SESSION['login'] = true;
            header('Location:'.$_SESSION['lasturl']);
        }
    }
}
?>
<html>
<head>
<title>注册</title>
<style>
body {
    background-image: url('../../resource/login_background.jpg');
    background-size: 100% auto;
}
#main {
    border-radius: 10px;
    width: 250px;
    height: 200px;
    margin: 200px auto;
    background-color: white;
    padding: 10px;
}
#error {
    font-size: 10px;
    color: red;
}
</style>
</head>
<body>
    <div id='main'>
        <form action="./register.php" method="POST">
            <p>用户名：<input type="text" name="username" <?php 
            if(isset($username)) {
                echo 'value='.$username;
            } ?> /> </p>
            <p>&nbsp; 密码：<input type="password" name="password" <?php
            if(isset($password)) {
                echo 'value='.$password;
            } ?> /> </p>
            <input type="submit" value="注册" />
        </form>
        <span id="error"><?php if (isset($_POST['username'])) {
             echo $errmsg; 
        }?></span>
    </div>
</body>
</html>