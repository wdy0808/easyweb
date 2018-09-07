#include "stdafx.h"
#include "websocket.h"

WebSocket* WebSocket::m_server = nullptr;

WebSocket::WebSocket()
{

}

WebSocket::~WebSocket()
{

}

void WebSocket::connectSuccessful(WebSocketInfo::web_ptr socket)
{
	for (auto i = m_connectedSockets.begin(); i != m_connectedSockets.end(); i++)
	{
		if ((*i) == socket)
			return;
	}
	m_connectedSockets.push_back(socket);
}

void WebSocket::writeToAll(std::string msg, WebSocketInfo::web_ptr source)
{
	for (int i = 0; i < m_connectedSockets.size(); i++)
	{
		if (m_connectedSockets[i] != source && m_connectedSockets[i]->getState() == connected)
		{
			m_connectedSockets[i]->setOutputMsg(msg);
			m_connectedSockets[i]->doWrite();
			++i;
		}
	}
}

WebSocketInfo::web_ptr WebSocket::getNewSocket(ioService& service)
{
	if (m_stopSockets.empty())
		return WebSocketInfo::getWebPtr(service);

	WebSocketInfo::web_ptr ptr = m_stopSockets[0];
	m_stopSockets.erase(m_stopSockets.begin());
	return ptr;
}

void WebSocket::socketStop(WebSocketInfo::web_ptr socket)
{
	m_stopSockets.push_back(socket);
}

WebSocket* WebSocket::getServer()
{
	if (m_server == nullptr)
		m_server = new WebSocket();

	return m_server;
}