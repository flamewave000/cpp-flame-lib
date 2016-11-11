#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
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
#include "../util/log.h"
#include "client.h"


#define ERROR -1

using namespace std;


namespace net
{
#pragma region constructors
	client::client() : _sfd(0) { }
	client::~client() { if(_sfd) ::close(_sfd); }
#pragma endregion


#pragma region public methods
	int client::connect(string addr_path, std::function<void(connection*)> handler)
	{
		// Check if we are already listening
		if (is_open()) {
			return ERROR;
		}

		sockaddr_un serv_addr;

		// Create the socket connection
		_sfd = ::socket(AF_LOCAL, SOCK_STREAM, 0);
		if (_sfd < 0) {
			log::herrno("Error occurred opening socket", errno);
			return ERROR;
		}
		// Clear structure
		memset(&serv_addr, 0, sizeof(serv_addr));

		// Set the family type to UNIX
		serv_addr.sun_family = AF_UNIX;
		// Set the address path in the structure (max 107 character)
		size_t sun_path_len = sizeof(serv_addr.sun_path) - 1;
		strncpy(serv_addr.sun_path, addr_path.c_str(), addr_path.length() < sun_path_len ? addr_path.length() : sun_path_len);

		if (::connect(_sfd, (sockaddr*)&serv_addr, SUN_LEN(&serv_addr)) < 0) {
			log::herrno("Error occurred connecting to server", errno);
			::close(_sfd);
			return ERROR;
		}
		else {
			connection conn(_sfd);
			handler(&conn);
		}
		if (::close(_sfd) < 0) {
			log::herrno("Error occurred while closing socket", errno);
			return ERROR;
		}
		_sfd = 0;
		return 0;
	}
#pragma endregion
}
