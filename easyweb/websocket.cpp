#include "stdafx.h"
#include "websocket.h"
#include "data.h"

WebSocket* WebSocket::m_server = nullptr;
std::map<std::string, std::map<std::string, std::list<WebSocketInfo::web_ptr> > > WebSocket::m_connectedSockets = std::map<std::string, std::map<std::string, std::list<WebSocketInfo::web_ptr> > >();

WebSocket::WebSocket()
{

}

WebSocket::~WebSocket()
{

}

void WebSocket::connectSuccessful(WebSocketInfo::web_ptr socket)
{
	WebSocket::m_connectedSockets[socket->getUsername()][socket->getCanvasname()].push_back(socket);
}

void WebSocket::writeToAll(std::string msg, WebSocketInfo::web_ptr source)
{
	std::list<WebSocketInfo::web_ptr>& tem = WebSocket::m_connectedSockets[source->getUsername()][source->getCanvasname()];
	for (auto i = tem.begin(); i != tem.end(); ++i)
	{
		if ((*i) != source)
		{
			(*i)->setOutputMsg(msg);
			(*i)->doWrite();
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
	WebSocket::m_connectedSockets[socket->getUsername()][socket->getCanvasname()].remove(socket);
}

void WebSocket::ownerQuit(WebSocketInfo::web_ptr owner)
{
	std::string pause = "{\"type\":\"ownerquit\"}";
	writeToAll(pause, owner);
}

WebSocket* WebSocket::getServer()
{
	if (m_server == nullptr)
		m_server = new WebSocket();

	return m_server;
}

std::string WebSocket::generateJson(std::string username)
{
	std::string ans = "[";

	for (auto i = WebSocket::m_connectedSockets.begin(); i != WebSocket::m_connectedSockets.end(); ++i)
	{
		if (i->first == username)
			continue;
		for (auto j = i->second.begin(); j != i->second.end(); ++j)
		{
			if (j->second.empty())
				continue;
			if (ans != "[")
				ans += ",";

			ans += (*(j->second.begin()))->getData()->generateJson(true);
		}
	}
	ans += "]";
	return ans;
}