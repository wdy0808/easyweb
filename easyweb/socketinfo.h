#pragma once
#include "asiodef.h"
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <mutex>

#define BIND1(func, x) bind(&WebSocketInfo::func, shared_from_this(), x)
#define BIND2(func, x, y) bind(&WebSocketInfo::func, shared_from_this(), x, y)

enum WebSocketState{shakehand, connected, init, indexconfig, stop};

struct CanvasInfo {
	std::string username, canvasname, selfname;
	bool index;
	bool ifOwner()
	{
		return username == selfname;
	}
};

class WebSocket;
class CanvasData;
class WebSocketInfo : public boost::enable_shared_from_this<WebSocketInfo>, boost::noncopyable
{
public:
	typedef boost::shared_ptr<WebSocketInfo> web_ptr;

	WebSocketInfo(ioService& service);
	~WebSocketInfo();

	static web_ptr getWebPtr(ioService& service);

	void doWrite();
	void doRead();
	Socket& getSocket();
	WebSocketState getState();
	void setOutputMsg(std::string msg);
	void stop();

	std::string getUsername();
	std::string getCanvasname();

	CanvasData* getData();

private:
	WebSocketInfo(ioService& service, WebSocket* web);

	void onRead(const ErrorCode& code, size_t bytes);
	void onWrite(const ErrorCode& code, size_t bytes);

	void buildResponse();
	void parseHeader();

	bool normalState();

	Socket m_socket;
	WebSocket* m_web;
	std::map<std::string, std::string> m_header;
	char m_input[MAX_SIZE];
	char m_output[MAX_SIZE];
	size_t m_inputBytes;
	size_t m_outputBytes;
	WebSocketState m_state;
	CanvasData* m_data;
	std::mutex m_mtx;
	CanvasInfo m_info;
};

