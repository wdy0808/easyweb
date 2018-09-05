#pragma once
#include "asiodef.h"
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <map>

#define BIND1(func, x) bind(&WebSocketInfo::func, shared_from_this(), x)
#define BIND2(func, x, y) bind(&WebSocketInfo::func, shared_from_this(), x, y)

class WebSocket;
class WebSocketInfo : public boost::enable_shared_from_this<WebSocketInfo>, boost::noncopyable
{
public:
	typedef boost::shared_ptr<WebSocketInfo> web_ptr;

	~WebSocketInfo();

	static web_ptr getWebPtr(ioService& service, WebSocket* web);

	void doWrite();
	void doRead(ioService & service);
	Socket& getSocket();
	void stop();

private:
	WebSocketInfo(ioService& service, WebSocket* web);

	void onRead(const ErrorCode& code, size_t bytes);
	void onWrite(const ErrorCode& code, size_t bytes);

	void buildResponse();
	void parseHeader();

	Socket m_socket;
	WebSocket* m_web;
	std::map<std::string, std::string> m_header;
	char m_input[1024];
	std::string m_output;
	size_t m_inputBytes;
	size_t m_outputBytes;
	bool m_state;
};

