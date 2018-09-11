<?php session_start(); ?>
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
    top: 100px;
    right: 30px;
    
}
</style>
</head>
<body>
    <div id="navigator">
        <div id="navigatorshow">
            <span id="name">EasyWeb</span>
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
            <li id="loginout">退出</li>
            <li id="persolweb">个人中心</li>
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
                document.getElementById("loginname").style.display = "block"
            }
            else 
            {
                navi.arrowresource = "./resource/arrowdown.png"
                document.getElementById("loginname").style.display = "none"
            }
        }
        document.getElementById("loginout").onclick = function () {
            $_SESSION['lasturl'] = '../../index.php';
            header('Location: ./user/login/loginout.php');
        }
        <?php } 
        else {
            $_SESSION['lasturl'] = '../../index.php';
        ?>
        document.getElementById("loginname").onclick = function () {
            window.location.href='./user/login/login.php'
        }
        <?php } ?>
    </script>
</body>
</html>