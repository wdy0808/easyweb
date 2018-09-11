<?php
session_start();
$_SESSION['lasturl'] = '../../index.php';
?>
<!DOCTYPE html>
<html>
<head>
<title>EasyPainting</title>
<style>
#navigator {
    background-color: orange;
    position: fixed;
    width: 100%;
    height: 50px;
    left: 0;
    top: 0;
}
#navigatorshow {
    width: 70%;
    margin: 0 auto;
} 
#name {
    color: white;
    font-weight: bolder;
    font-size: 30px;
    position: relative;
    top: 7px;
    margin-right: 40px;
}
#login {
    float:right;
    height: 50px;
}
#portait {
    vertical-align: middle;
    position: relative;
    bottom: 5px;
}
#loginname {
    display: inline-block;
    color: #606060;
    font-size: 18px;
    margin-bottom: 11px;
    margin-right: -10px;
    margin-left: 10px;
}
#catagory {
    position: absolute;
    top: 70px;
    right: 180px;
    display: none;
}
</style>
</head>
<body>
    <div id="navigator">
        <div id="navigatorshow">
            <span id="name">EasyWeb</span>
            <a href="<?php
            if(isset($_SESSION['login'])) {
                echo './paint/newpaint.php';
            }
            else {
                echo './user/login/login.php';
            }?>
            ">新建画布</a>
            <div id="login">
                <img src=<?php
                    if(isset($_SESSION['portait'])) {
                        echo $_SESSION['portait'];
                    }
                    else 
                    {
                        echo "./resource/user.png";
                    }?>
                     id="portait">
                <p id="loginname">
                <?php 
                if(isset($_SESSION['username'])) {
                    echo $_SESSION['username'];
                }
                else 
                {
                    echo '登陆';
                }
                ?>
                </p>
                <?php if(isset($_SESSION['login'])) { ?>
                <img v-bind:src = arrowresource>
                <?php } ?>
            </div>
        </div>
        <div id="catagory">
            <a href="./user/login/loginout.php">退出</a>
            <p id="persolweb">个人中心</p>
        </div>
    </div>
    <script src="https://cdn.bootcss.com/vue/2.4.2/vue.min.js"></script>
    <script type="text/javascript">
        var navi = new Vue({
            el: '#navigator',
            data: {
                arrowresource: "./resource/arrowdown.png"
            }
        })
        <?php if(isset($_SESSION['login'])) { ?>
        document.getElementById("loginname").onclick = function () {
            if (navi.arrowresource == "./resource/arrowdown.png")
            {
                navi.arrowresource = "./resource/arrowup.png"
                document.getElementById("catagory").style.display = "block"
            }
            else 
            {
                navi.arrowresource = "./resource/arrowdown.png"
                document.getElementById("catagory").style.display = "none"
            }
        }
        document.getElementById("loginout").onclick = function () {
            header('Location: ./user/login/loginout.php');
        }
        <?php } 
        else {
        ?>
        document.getElementById("loginname").onclick = function () {
            window.location.href='./user/login/login.php'
        }
        <?php } ?>
    </script>
</body>
</html>