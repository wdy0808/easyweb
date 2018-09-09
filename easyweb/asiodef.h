#pragma once
#include "stdafx.h"
#define MAX_SIZE 6554500

typedef boost::asio::ip::tcp::socket Socket;
typedef boost::asio::ip::tcp::endpoint EndPoint;
typedef boost::system::error_code ErrorCode;
typedef boost::asio::io_service ioService;
typedef boost::asio::ip::tcp::acceptor Acceptror;

using boost::bind;
using boost::asio::buffer;
