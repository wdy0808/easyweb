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

void WebSocket::writeToAll(std::string msg, WebSocketInfo* source)
{
	for (int i = 0; i < m_connectedSockets.size(); i++)
	{
		if (m_connectedSockets[i] != source && m_connectedSockets[i]->getState() == connected)
		{
			m_connectedSockets[i]->setOutputMsg(msg);
			m_connectedSockets[i]->doWrite();
			++i;
		}
		//else if (m_connectedSockets[i]->getState() == stop)
			//i = m_connectedSockets.erase(i);
	}
}

WebSocket* WebSocket::getServer()
{
	if (m_server == nullptr)
		m_server = new WebSocket();

	return m_server;
}