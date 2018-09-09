#include "stdafx.h"
#include "httpsocket.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <sstream>
#include "HttpString.h"
#include "HttpFile.h"


HttpSocket::HttpSocket(ioService& service)
	: m_sock(service), m_service(service)
{
}


HttpSocket::~HttpSocket()
{
}

HttpSocket::http_ptr HttpSocket::getHttpPtr(ioService& service)
{
	HttpSocket::http_ptr ptr(new HttpSocket(service));
	return ptr;
}

void HttpSocket::step(const ErrorCode & err, size_t bytes)
{
	reenter(this)
	{
		for (;;) {
			yield boost::asio::async_read(m_sock, buffer(m_input), BIND2(step, _1, _2));
			yield m_service.post(BIND(parseHeader));
			yield m_service.post(BIND(handleResponse));
			yield boost::asio::async_write(m_sock, buffer(m_output, bytes), BIND2(step, _1, _2));
			if (m_timer == 1)
			{
				boost::asio::deadline_timer timer(m_service, boost::posix_time::seconds(60));
				timer.async_wait(&HttpSocket::stop);
				m_timer == 2;
			}
			else if (m_timer == 0)
			{
				stop();
				break;
			}
		}
	}
}

void HttpSocket::parseHeader()
{
	m_headers.clear();

	std::istringstream stream(m_input);
	std::string value;
	std::vector<std::string> sto;
	std::getline(stream, value);
	Http::split(value, ' ', sto);
	m_headers["way"] = sto[0];
	m_headers["url"] = sto[1];
	m_headers["protocol"] = sto[2];

	std::getline(stream, value);
	while (!value.empty())
	{
		sto.clear();
		Http::split(value, ':', sto);
		m_headers[sto[0]] = sto[1];
		std::getline(stream, value);
	}
	stream >> value;
	if (!value.empty())
		m_headers["data"] = value;

	if (m_headers["protocol"] == "HTTP/1.0")
	{
		m_timer = 0;
		if (m_headers.find("Connection") != m_headers.end() && m_headers["Connection"] == "Keep-Alive")
			m_timer = 1;
	}
	else if (m_headers["protocol"] == "HTTP/1.1")
	{
		m_timer = 1;
		if (m_headers.find("Connection") != m_headers.end() && m_headers["Connection"] == "close")
			m_timer = 0;
	}
}

void HttpSocket::handleResponse()
{
	m_response.clear();

	if (m_headers["way"] == "GET")
		getHandler();

	m_output = m_headers["protocol"] + " " + m_response["code"] + "\r\n";
	generateResonse();
	for (auto i = m_response.begin(); i != m_response.end(); i++)
	{
		if (i->first == "code" || i->first == "data")
			continue;
		m_output += i->first + ": " + i->second + "\r\n";
	}
	m_output += "\r\n";
	m_output += m_headers["data"] + "\r\n";
}

void HttpSocket::stop()
{
	std::lock_guard<std::mutex> lock(m_mtx);
	if (m_sock.is_open())
		m_sock.close();
}

void HttpSocket::getHandler()
{
	if (Http::fileExist(m_headers["url"]))
	{
		if (Http::fileReadable(m_headers["url"]))
		{
			m_response["code"] = "200 OK";
			m_response["data"] = Http::getFile(m_headers["url"]);
			m_response["Content-Type"] = "text/html";
			m_response["Conent-Length"] = std::to_string(m_headers["data"].size());
		}
	}
}

void HttpSocket::generateResonse()
{
	m_response["Date"] = Http::getGMTstring();
}