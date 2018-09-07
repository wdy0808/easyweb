#pragma once
#include "socketinfo.h"

class WebSocketInfo;
class WebSocket
{
public:
	~WebSocket();

	void connectSuccessful(WebSocketInfo::web_ptr socket);
	void writeToAll(std::string msg, WebSocketInfo::web_ptr source);
	WebSocketInfo::web_ptr getNewSocket(ioService& service);
	void socketStop(WebSocketInfo::web_ptr socket);

	static WebSocket* getServer();

private:
	WebSocket();

	static WebSocket* m_server;
	std::vector<WebSocketInfo::web_ptr> m_connectedSockets;
	std::vector<WebSocketInfo::web_ptr> m_stopSockets;
};

