<?php
session_start();
if (isset($_POST['canvasname']))
{
    $con = mysqli_connect("localhost","root","0101,wdy.","easyweb");
    $nowtime = date("Y-m-d H:i:s");
    $sql="INSERT INTO painting (username, name, authority, ctime) VALUES ('$_SESSION[username]','$_POST[canvasname]','$_POST[authority]', '$nowtime')";
    if (mysqli_query($con,$sql))
    {
        header('Location: ./paint.php?username='.$_SESSION['username'].'&canvasname='.$_POST['canvasname']);
    }
    else 
    {
        echo mysqli_error($con);
    }
}