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
    top: 5px;
    margin-right: 40px;
}
#login {
    float:right;
    height: 50px;
    position: relative;
    top: 10px;
}
#portait {
    vertical-align: middle;
    position: relative;
    bottom: 5px;
    height: 40px;
    width: 40px;
}
#loginname {
    color: #606060;
    font-size: 18px;
    font-weight: 100;
}
#catagory {
    position: absolute;
    top: 50px;
    right: 190px;
    display: none;
    border: 1px solid #b4a9a9;
    padding: 10px;
}
a:link {text-decoration:none;}
a:visited {text-decoration:none;} 
a.top {font-weight: bolder}
a.top:link {color: #EEEEEE;}
a.top:visited {color:#EEEEEE;} 
a.hide:link {color: black;}
a.hide:visited {color:black;}
#main {
    width: 400px;
    margin: 50px auto;
}
</style>
</head>
<body>
    <div id="navigator">
        <div id="navigatorshow">
            <span id="name">EasyWeb</span>
            <a class="top" href="<?php
            if(isset($_SESSION['username'])) {
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
                <a class="top" id="loginname" href="javascript:void(0)">
                <?php 
                if(isset($_SESSION['username'])) {
                    echo $_SESSION['username'];
                }
                else 
                {
                    echo '登陆';
                }
                ?>
                </a>
                <?php if(isset($_SESSION['username'])) { ?>
                <img v-bind:src = arrowresource>
                <?php } ?>
            </div>
        </div>
        <div id="catagory">
        <a class="hide" href="./user/login/loginout.php" onclick="header('Location: ./user/login/loginout.php')">退出</a><br><br>
            <a class="hide" href="./user/person.php">个人中心</a>
        </div>
    </div>
    <div id="main">
        <p>{{connectstate}}</p>
        <table>
            <tr>
                <th>用户名</th>
                <th>绘图名</th>
                <th>创建时间</th>
                <th>修改时间</th>
            </tr>
            <paint v-for="post in paintingInfo" v-bind:post="post"></paint>
        </table>
    </div>
    <script src="https://cdn.bootcss.com/vue/2.4.2/vue.min.js"></script>
    <script type="text/javascript">
        var navi = new Vue({
            el: '#navigator',
            data: {
                arrowresource: "./resource/arrowdown.png"
            }
        })
        <?php if(isset($_SESSION['username'])) { ?>
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
        <?php } 
        else {
        ?>
        document.getElementById("loginname").onclick = function () {
            window.location.href='./user/login/login.php'
        }
        <?php } ?>

        Vue.component('paint', {
                        props: ['post'],
                        template: `
                            <tr>
                                <td>{{post.username}}</td>
                                <td>{{post.canvasname}}</td>
                                <td>{{post.ctime}}</td>
                                <td>{{post.mtime}}</td>
                                <td><a href="./paint/paint.php?username={{post.username}}&canvasname={{post.canvasname}}">进入</a></td>
                            </tr>
                        `        
        })
        var m = new Vue({
            el: '#main',
            data: {
                paintingInfo: '',
                connectstate: ''
            }
        })

        var ws = new WebSocket("ws://172.18.93.21:9999");
        ws.onopen = function(evt) {
            var initInfo = '{"username":' + '"<?php echo $_SESSION['username']; ?>"}'
            ws.send(initInfo)
        };
        ws.onmessage = function(evt) {
            console.log(evt.data);
            m.paintingInfo = JSON.parse(evt.data).data
        };
        ws.onclose = function(evt) {
            m.connectstate = '连接断开'
        };
    </script>
</body>
</html>