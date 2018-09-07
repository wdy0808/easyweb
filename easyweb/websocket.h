#pragma once

class WebSocketInfo;
class WebSocket
{
public:
	~WebSocket();

	void connectSuccessful(WebSocketInfo* socket);
	void writeToAll(std::string msg, WebSocketInfo* source);

	static WebSocket* getServer();

private:
	WebSocket();

	static WebSocket* m_server;
	std::vector<WebSocketInfo* > m_connectedSockets;
};

