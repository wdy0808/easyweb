// easyweb.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "websocket.h"
#include <boost/thread.hpp>

ioService io;
EndPoint endpoint(boost::asio::ip::tcp::v4(), 8888);
Acceptror acc(io, endpoint);

void handle_accept(WebSocketInfo::web_ptr client, const ErrorCode& err)
{
	client->doRead();
	WebSocketInfo::web_ptr ptrs = WebSocket::getServer()->getNewSocket(io);
	acc.async_accept(ptrs->getSocket(), bind(handle_accept, ptrs, _1));
}

int main() {
	WebSocketInfo::web_ptr ptrs = WebSocket::getServer()->getNewSocket(io);
	acc.async_accept(ptrs->getSocket(), bind(handle_accept, ptrs, _1));
	boost::thread_group threads;
	for (int i = 0; i < 5; ++i)
		threads.create_thread([]() {io.run(); });
	boost::this_thread::sleep(boost::posix_time::millisec(500));
	threads.join_all();
	return 0;
}