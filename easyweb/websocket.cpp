#include "stdafx.h"
#include "websocket.h"
#include "socketinfo.h"

WebSocket* WebSocket::m_server = nullptr;

WebSocket::WebSocket()
{

}

WebSocket::~WebSocket()
{

}

void WebSocket::connectSuccessful(WebSocketInfo* socket)
{
	m_connectedSockets.push_back(socket);
}

void WebSocket::writeToAll(std::string msg)
{
	auto i = m_connectedSockets.begin();
	while (i != m_connectedSockets.end()) 
	{
		if ((*i)->getState() == connected)
		{
			(*i)->setOutputMsg(msg);
			(*i)->doWrite();
			++i;
		}
		else
			i = m_connectedSockets.erase(i);
	}
}

WebSocket* WebSocket::getServer()
{
	if (m_server == nullptr)
		m_server = new WebSocket();

	return m_server;
}