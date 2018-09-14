<?php
session_start();
if (isset($_POST['canvasname']))
{
    $con = mysqli_connect("localhost","root","0101,wdy.","easyweb");
    $sql="INSERT INTO painting (username, name, authority) VALUES ('$_SESSION[username]','$_POST[canvasname]','$_POST[authority]')";
    mysqli_query($con,$sql);
    header('Location: ./paint.php?username='.$_SESSION['username'].'&canvasname='.$_POST['canvasname']);
}