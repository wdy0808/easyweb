<?php
session_start();
$con = mysqli_connect("localhost","root","0101,wdy.","easyweb");
$sql="SELECT name from painting where username = '$_SESSION[username]'";
$result = mysqli_query($con,$sql);
?>
<!DOCTYPE html>
<html>
<head>
<title>个人界面</title>
<style>
div {border: paleturquoise 1px solid;}
#main {width: 500px;height: 500px;margin: 50px auto}
</style>
</head>
<body>
    <div id="main">
        <div id="info">
            <table>
                <tr>
                    <th>名字</th>
                </tr>
                <?php
                while ($row = mysqli_fetch_array($result))
                {
                    echo '<tr>';
                    echo '<td>'.$row['name'].'</td>';
                    echo '<td>'.'<a href="../paint/paint.php?username='.$_SESSION['username'].'&canvasname='.$row['name'].'">进入</a></td>';
                    echo '</tr>';
                }
                ?>
            </table>
        </div>
    </div>
</body>
</html>