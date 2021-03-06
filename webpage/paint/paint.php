﻿<?php
session_start();
$username = $_GET['username'];
$canvasname = $_GET['canvasname'];
?>
<!DOCTYPE html>
<meta charset="utf-8" />
<title>共同绘图</title>
<head>
<style>
    #bg{ 
        position: absolute; 
        top: 0%; 
        left: 0%; 
        width: 100%; 
        height: 100%; 
        background-color: black; 
        z-index:1001; 
        -moz-opacity: 0.3; 
        opacity:.30; 
        filter: alpha(opacity=30);
    }
    #show{
        z-index:1002;
        position: absolute; 
        top: 48%; 
        left: 50%; 
        height: auto;  
    }
</style>
</head>
<body>
<canvas id="canvas" width="600" height="400" style="border:grey 1px solid;">
    <span>您的浏览器不支持</span>
</canvas>
<div id="container_div"><!-- 遮罩 -->
    <div id="bg"></div>
    <div id="show"><p>{{connectstate}}</p></div>
</div>
<script src="http://code.jquery.com/jquery-latest.js"></script>
<script src="https://cdn.bootcss.com/vue/2.4.2/vue.min.js"></script>
<script>
    var connect = new Vue({
        el: '#container_div',
        data: {
            connectstate: '连接服务器中',
            mousestate: 'up'
        }
    })

    var oc = $('#canvas')[0]
    canvas = oc.getContext('2d')
    var lastimgData=canvas.getImageData(0,0,600,400)
    $('#canvas').on('mousedown', function(){
    /*开始绘制*/        
        canvas.beginPath();
        /*设置动画绘制起点坐标*/
        canvas.moveTo(event.pageX, event.pageY);
        $('#canvas').on('mousemove', function(){
            /*设置下一个点坐标*/
            canvas.lineTo(event.pageX , event.pageY);
            /*画线*/
            canvas.stroke();
        });
    }).on('mouseup', function(){
        doSend()
        $('#canvas').off('mousemove');
    });
    
    var ws = new WebSocket("ws://172.18.93.21:8888");
    ws.onopen = function(evt) {
        connect.connectstate = '初始化数据'

        var initInfo = '{"username":' + '"<?php echo $username; ?>"' +
         ',"canvasname":' + '"<?php echo $canvasname; ?>"' +
         ',"selfname":' + '"<?php echo $_SESSION['username']; ?>"' + '}'
        ws.send(initInfo)
    };
    ws.onmessage = function(evt) {
        console.log(evt.data);
        var newImage = JSON.parse(evt.data)
        if (newImage["type"] == "changeData" || newImage["type"] == "init")
        {
            var newData = newImage.data
            var temImageData = canvas.getImageData(0,0,oc.width,oc.height)
            for (var i = 0; i < newData.length; i+=2)
            {
                lastimgData.data[newData[i]] = newData[i + 1]
                temImageData.data[newData[i]] = newData[i + 1]
            }
            canvas.putImageData(temImageData,0,0);
            if (newImage["type"] == "init")
            {
                connect.connectstate = '成功连接服务器'
                $("#container_div").css("display","none");
            }
        }
        else if (newImage["type"] == "ownerquit")
        {
            connect.connectstate = '画布拥有者退出'
            $("#container_div").css("display","block");
        }
        else if (newImage["type"] == "ownerback")
        {
            connect.connectstate = '成功连接服务器'
            $("#container_div").css("display","none");
        }
    };
    ws.onclose = function(evt) {
        connect.connectstate = '连接断开'
        $("#container_div").css("display","block");
    };

    function doSend() {
        if (connect.connectstate == '成功连接服务器')
            ws.send(generationJson())
    }

    function generationJson() {
        var first = false;
        var imageData = canvas.getImageData(0,0,oc.width,oc.height)
        var JsonValue = '{"type":"changeData","data":['
        for (var i = 0; i < imageData.data.length; i++) {
            if (imageData.data[i] != lastimgData.data[i])
            {
                if (first)
                    JsonValue += ','
                else 
                    first = true
                JsonValue = JsonValue + i + ',' + imageData.data[i]
            }
            
        }
        JsonValue += ']}'
        lastimgData = imageData
        return JsonValue
    }

    window.onbeforeunload = function(){ws.close(); }
</script>
</body>
</html>