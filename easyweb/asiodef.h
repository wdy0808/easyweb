#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#define MAX_SIZE 8096

typedef boost::asio::ip::tcp::socket Socket;
typedef boost::asio::ip::tcp::endpoint EndPoint;
typedef boost::system::error_code ErrorCode;
typedef boost::asio::io_service ioService;
typedef boost::asio::ip::tcp::acceptor Acceptror;

using boost::bind;
using boost::asio::buffer;
