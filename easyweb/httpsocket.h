#pragma once
#include "asiodef.h"
#include <boost/asio/yield.hpp>
#include <boost/coroutine/coroutine.hpp>
#include <atomic>

#define BIND(func) bind(&HttpSocket::func, shared_from_this())
#define BIND2(func, x, y) bind(&HttpSocket::func, shared_from_this(), x, y)

class HttpSocket : public boost::enable_shared_from_this<HttpSocket>, boost::noncopyable, boost::asio::coroutine
{
public:
	typedef boost::shared_ptr<HttpSocket> http_ptr;

	HttpSocket(ioService& service);
	~HttpSocket();

	static http_ptr getHttpPtr(ioService& service);

	void step(const ErrorCode & err = ErrorCode(), size_t bytes = 0);
	void parseHeader();
	void handleResponse();
	void stop();
	void getHandler();
	void generateResonse();

private:
	Socket m_sock;
	std::map<std::string, std::string> m_headers;
	std::map<std::string, std::string> m_response;
	std::atomic<int> m_timer;
	std::string m_input;
	std::string m_output;
	ioService& m_service;
	std::mutex m_mtx;
};

