#include "stdafx.h"
#include "websocket.h"
#include "socketinfo.h"

WebSocket::WebSocket()
{
	
}


WebSocket::~WebSocket()
{
}

void WebSocket::running(ioService & service, EndPoint & endpoint)
{
	while (true)
	{
		WebSocketInfo::web_ptr ptr = WebSocketInfo::getWebPtr(service, this);
		Acceptror accept(service, endpoint);
		accept.accept(ptr->getSocket());
		ptr->doRead(service);
	}
}

void WebSocket::test(WebSocketInfo * onesocket)
{
	//onesocket->doRead();
}
