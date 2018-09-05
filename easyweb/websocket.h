#pragma once
#include "asiodef.h"

class WebSocketInfo;
class WebSocket
{
public:
	WebSocket();
	~WebSocket();

	void running(ioService& service, EndPoint& endpoint);
	void test(WebSocketInfo* onesocket);
};

