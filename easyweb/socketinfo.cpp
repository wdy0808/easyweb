#include "stdafx.h"
#include "socketinfo.h"
#include "websocket.h"
#include <sstream>
#include "WSHelper.h"
#include <cstring>


WebSocketInfo::WebSocketInfo(ioService& service)
	: m_socket(service), m_state(shakehand)
{
}

WebSocketInfo::~WebSocketInfo()
{
}

void WebSocketInfo::onRead(const ErrorCode& code, size_t bytes)
{
	std::cout << bytes << std::endl;
	if (code)
		stop();
	m_inputBytes = bytes;
	if (m_state == shakehand)
	{
		parseHeader();
		buildResponse();
		doWrite();
	}
	else if (m_state == connected)
		WebSocket::getServer()->writeToAll(WS::getReceiveData(std::string(m_input, bytes)));
	doRead();
}

void WebSocketInfo::onWrite(const ErrorCode& code, size_t bytes)
{
	if (code)
		stop();
	if (m_state == shakehand)
	{
		WebSocket::getServer()->connectSuccessful(this);
		m_state = connected;
	}
	//std::cout << "Response: " << std::string(m_output, bytes) << std::endl;
}

WebSocketInfo::web_ptr WebSocketInfo::getWebPtr(ioService& service)
{
	WebSocketInfo::web_ptr new_ptr(new WebSocketInfo(service));
	return new_ptr;
}

void WebSocketInfo::doWrite()
{
	if (m_state != WebSocketState::stop)
		m_socket.async_write_some(buffer(m_output, m_outputBytes), BIND2(onWrite, _1, _2));
}

void WebSocketInfo::doRead()
{
	if (m_state != WebSocketState::stop)
		m_socket.async_read_some(buffer(m_input), BIND2(onRead, _1, _2));
}

Socket & WebSocketInfo::getSocket()
{
	return m_socket;
}

void WebSocketInfo::stop()
{
	if (m_state != WebSocketState::stop)
	{
		m_state = WebSocketState::stop;
		m_socket.close();
	}
}

WebSocketState WebSocketInfo::getState()
{
	return m_state;
}

void WebSocketInfo::buildResponse()
{
	m_output[0] = '\0';
	strcat_s(m_output, "HTTP/1.1 101 Switching Protocols\r\n");
	strcat_s(m_output, "Upgrade: websocket\r\n");
	strcat_s(m_output, "Connection: Upgrade\r\n");
	strcat_s(m_output, "Sec-WebSocket-Accept: ");
	strcat_s(m_output, m_header["Sec-WebSocket-Accept"].c_str());
	strcat_s(m_output, "\r\nWebSocket-Location: ws://");
	strcat_s(m_output, m_header["Host"].c_str());
	strcat_s(m_output, m_header["url"].c_str());
	strcat_s(m_output, "\r\n\r\n");
	m_outputBytes = strlen(m_output);
}

void WebSocketInfo::setOutputMsg(std::string msg)
{
	m_output[0] = '\0';
	strcat_s(m_output, WS::generateSendData(msg).c_str());
	m_outputBytes = strlen(m_output);
}

void WebSocketInfo::parseHeader()
{
	std::istringstream stream(m_input);
	std::string header;
	std::getline(stream, header);
	size_t index1 = header.find(' ');
	size_t index2 = header.find(' ', index1 + 1);
	m_header["url"] = header.substr(index1 + 1, index2 - index1 - 1);

	size_t index;
	while (getline(stream, header))
	{
		header = header.substr(0, header.size() - 1);
		index = header.find(":");
		if (index != std::string::npos)
			m_header[header.substr(0, index)] = header.substr(index + 2);
	}

	char tem[100];
	WS::getResponseKey(m_header["Sec-WebSocket-Key"].c_str(), tem);
	m_header["Sec-WebSocket-Accept"] = tem;
}