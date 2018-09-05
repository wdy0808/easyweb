#include "stdafx.h"
#include "socketinfo.h"
#include "websocket.h"
#include <sstream>
#include "WSHelper.h"
#include <time.h>
#include <cstdlib>
#include <websocketpp/server.hpp>


WebSocketInfo::WebSocketInfo(ioService& service, WebSocket* web)
	: m_socket(service), m_web(web)
{
}

void WebSocketInfo::onRead(const ErrorCode& code, size_t bytes)
{
	if (code)
		stop();
	std::cout << "Receive: " << std::string(m_input, bytes) << std::endl;
	m_inputBytes = bytes;
	parseHeader();
	m_input[0] = '\0';
	char tem[100];
	WS::getResponseKey(m_header["Sec-WebSocket-Key"].c_str(), tem);
	m_header["Sec-WebSocket-Accept"] = tem;
	buildResponse();
	doWrite();
	//doRead();
}

void WebSocketInfo::onWrite(const ErrorCode& code, size_t bytes)
{
	if (code)
		stop();
	std::cout << "Response: " << std::string(m_output, bytes) << std::endl;

}


WebSocketInfo::~WebSocketInfo()
{
}

WebSocketInfo::web_ptr WebSocketInfo::getWebPtr(ioService& service, WebSocket* web)
{
	WebSocketInfo::web_ptr new_ptr(new WebSocketInfo(service, web));
	return new_ptr;
}

void WebSocketInfo::doWrite()
{
	m_socket.write_some(buffer(m_output));
	std::cout << "o1" << m_output << "o1";
}

void WebSocketInfo::doRead(ioService & service)
{
	int m_inputBytes = m_socket.read_some(buffer(m_input));
	std::cout << "Receive: " << std::string(m_input, m_inputBytes) << std::endl;
	parseHeader();
	char tem[100];
	WS::getResponseKey(m_header["Sec-WebSocket-Key"].c_str(), tem);
	m_header["Sec-WebSocket-Accept"] = tem;
	buildResponse();
	doWrite();
	doRead(service);
}

Socket & WebSocketInfo::getSocket()
{
	return m_socket;
}

void WebSocketInfo::stop()
{
	if (m_state)
	{
		m_state = false;
		m_socket.close();
	}
}

void WebSocketInfo::buildResponse()
{
	m_output.clear();
	m_output = "HTTP/1.1 101 Switching Protocols\r\n";
	m_output += "Upgrade: websocket\r\n";
	m_output += "Connection: Upgrade\r\n";
	m_output += "Sec-WebSocket-Accept: ";
	m_output += m_header["Sec-WebSocket-Accept"];
	m_output += "\r\nWebSocket-Location:ws://";
	m_output += m_header["Host"] + m_header["url"];
	m_output += "\r\n\r\n";
	//strcat_s(m_output, "HTTP/1.1 101 Switching Protocols\n");
	//strcat_s(m_output, "Access-Control-Allow-Credentials: true\r\n");
	//strcat_s(m_output, "Access-Control-Allow-Headers: content-type, authorization, x-websocket-extensions, x-websocket-version, x-websocket-protocol\r\n");
	//strcat_s(m_output, "Access-Control-Allow-Headers: authorization\r\n");
	//strcat_s(m_output, "Access-Control-Allow-Headers: x-websocket-extensions\r\n");
	//strcat_s(m_output, "Access-Control-Allow-Headers: x-websocket-version\r\n");
	//strcat_s(m_output, "Access-Control-Allow-Headers: x-websocket-protocol\r\n");
	//strcat_s(m_output, "Access-Control-Allow-Origin: null\r\n");
	//strcat_s(m_output, "Upgrade: websocket\n");
	//strcat_s(m_output, "Connection: Upgrade\n");
	char gmt[31];
	time_t rawtime;
	tm timeInfo;
	char szTemp[30] = { 0 };
	time(&rawtime);
	gmtime_s(&timeInfo, &rawtime);
	strftime(szTemp, sizeof(szTemp), "%a, %d %b %Y %H:%M:%S GMT", &timeInfo);
	strcpy_s(gmt, strlen(szTemp) + 1, szTemp);
	//strcat_s(m_output, "Date: ");
	//strcat_s(m_output, gmt);
	//strcat_s(m_output, "Sec-WebSocket-Accept: ");
	//strcat_s(m_output, m_header["Sec-WebSocket-Accept"].c_str());
	//strcat_s(m_output, "\r\nServer: Kaazing Gateway\r\n");
	//strcat_s(m_output, "\n\n");
	//strcat_s(m_output, "Sec-WebSocket-Protocol: ");
	
}

void WebSocketInfo::parseHeader()
{
	std::istringstream stream(m_input);
	std::string header;
	std::getline(stream, header);
	int index1 = header.find(' ');
	int index2 = header.find(' ', index1 + 1);
	m_header["url"] = header.substr(index1 + 1, index2 - index1 - 1);

	size_t index;
	while (getline(stream, header))
	{
		header = header.substr(0, header.size() - 1);
		index = header.find(":");
		if (index != std::string::npos)
			m_header[header.substr(0, index)] = header.substr(index + 2);
	}
}