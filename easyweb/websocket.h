#pragma once
#include "socketinfo.h"
#include <map>

class WebSocketInfo;
class WebSocket
{
public:
	~WebSocket();

	void connectSuccessful(WebSocketInfo::web_ptr socket);
	void writeToAll(std::string msg, WebSocketInfo::web_ptr source);
	WebSocketInfo::web_ptr getNewSocket(ioService& service);
	void socketStop(WebSocketInfo::web_ptr socket);
	void ownerQuit(WebSocketInfo::web_ptr owner);
	std::string generateJson(std::string username);

	static WebSocket* getServer();

private:
	WebSocket();

	static WebSocket* m_server;
	static std::map<std::string, std::map<std::string, std::list<WebSocketInfo::web_ptr> > > m_connectedSockets;
	std::vector<WebSocketInfo::web_ptr> m_stopSockets;
};

