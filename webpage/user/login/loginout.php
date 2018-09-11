<?php
session_start();
if (isset($_SESSION['login']))
{
    unset($_SESSION['login']);
}
if (isset($_SESSION['username']))
{
    unset($_SESSION['username']);
}
header('Location:'.$_SESSION['lasturl']);