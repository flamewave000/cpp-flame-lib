#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>
#ifndef _UNISTD_H
#include <unistd.h>
#endif
#ifndef _GLIBCXX_THREAD
#include <thread>
#endif
#ifndef _STDIO_H
#include <stdio.h>
#endif
#ifndef	_STDLIB_H
#include <stdlib.h>
#endif
#ifndef	_STRING_H
#include <string.h>
#endif
#ifndef	_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifndef	_ERRNO_H
#include <errno.h>
#endif
#ifndef _TIME_H
#include <time.h>
#endif
#include "server.h"
#include "../util/log.h"


#define LOCK(mutex) std::lock_guard<std::recursive_mutex> __lock(mutex)
#define ERROR -1
#define MAX_CONNECTION_BACKLOG 16
#define POLL_DELAY_SEC 0L // 0s
#define POLL_DELAY_NSEC 100000000L // 100ms

using namespace std;

namespace net
{
#pragma region constructors
	server::server() : _sfd(-1), _listen_thread(nullptr), _listening(false) { }
	server::~server() { kill(); }
#pragma endregion


#pragma region public methods
	int server::open(string addr_path)
	{
		{
			LOCK(_listening_mutex);
			// Check if we are already listening
			if (is_open()) {
				return ERROR;
			}

			// Create the socket connection
			_sfd = socket(AF_LOCAL, SOCK_STREAM, 0);
			if (_sfd < 0) {
				return ERROR;
			}
			// Create socket address structure
			sockaddr_un serv_addr;
			// Clear structure
			memset(&serv_addr, 0, sizeof(serv_addr));

			// Set the family type to UNIX
			serv_addr.sun_family = AF_UNIX;
			// Set the address path in the structure (max 107 character)
			size_t sun_path_len = sizeof(serv_addr.sun_path) - 1;
			strncpy(serv_addr.sun_path, addr_path.c_str(), addr_path.length() < sun_path_len ? addr_path.length() : sun_path_len);

			// Bind the socket to an address
			if (bind(_sfd, (sockaddr*)&serv_addr, SUN_LEN(&serv_addr))) {
				::close(_sfd);
				return ERROR;
			}

			if (::listen(_sfd, MAX_CONNECTION_BACKLOG) < 0) {
				::close(_sfd);
				return ERROR;
			}

			_listening = true;
		}
		if (_listen_thread != nullptr) {
			kill();
		}
		_listen_thread = new std::thread(&server::listen, this);
		return 0;
	}
	/*sets the listen flag to false and then joins the thread to delete it.*/
	void server::kill()
	{
		if (is_open()) {
			stop_listening();
			if (this_thread::get_id() != _listen_thread->get_id()) {
				_listen_thread->join();
			}
			delete _listen_thread;
			_listen_thread = nullptr;
		}
	}
#pragma endregion


#pragma region protected methods
	void server::stop_listening() {
		LOCK(_listening_mutex);
		_listening = false;
	}
#pragma endregion


#pragma region thread methods
	void server::listen(server* self)
	{
		pollfd pfd = { self->_sfd, POLLIN, 0 };
		while (true)
		{
			{
				LOCK(self->_listening_mutex);
				if (!self->_listening) break;
			}
			int res = ::poll(&pfd, 1, 0);
			if (res < 0) {
				log::herrno("Error occurred waiting for connections", errno);
				continue;
			}
			else if (res == 0) {
				timespec ts{ POLL_DELAY_SEC, POLL_DELAY_NSEC };
				nanosleep(&ts, nullptr);
				continue;
			}
			int cfd = ::accept(self->_sfd, /*(struct sockaddr*)*/nullptr, nullptr);
			if (cfd < 0) {
				log::herrno("Error occurred accepting connection", errno);
				continue;
			}
			//int flags = fcntl(cfd, F_GETFL, 0);
			//fcntl(cfd, F_SETFL, flags | O_NONBLOCK);
			connection conn(cfd);
			self->connected(&conn);
			conn.close();
		}
	}
#pragma endregion
}