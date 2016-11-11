#ifndef _CONNECTION_
#define _CONNECTION_ 1

#ifndef _GLIBCXX_VECTOR
#include <vector>
#endif
#ifndef _GLIBCXX_STRING
#include <string>
#endif

#include "status_codes.h"


#define ZERO_MSG(msg_ptr) ::memset(msg_ptr, 0, sizeof(net::message))

namespace net
{
	struct message {
		struct message_header{
			status_code status;
			size_t data_size;
		} header;
		uint8_t *data;
	};

	class server;
	class client;
	class connection
	{
#pragma region friends
		friend server;
		friend client;
#pragma endregion


#pragma region instance variables
	private:
		int _fd;
		bool _open;
#pragma endregion


#pragma region constructors
	private:
		connection(int fd);
#pragma endregion


#pragma region public methods
	public:
		inline bool is_open() { return _fd >= 0 && _open; }

		bool is_valid() const;
		int has_data(const int &timeout = 0) const;

		ssize_t read(uint8_t * buf, const size_t &count) const;
		ssize_t read_all(uint8_t ** buf) const;

		ssize_t sread(std::string *str_out, const size_t &count) const;
		ssize_t sread_all(std::string *str_out) const;

		ssize_t write(const uint8_t * buf, const size_t &count) const;
		ssize_t swrite(const std::string &data) const;

		ssize_t sendmsg(const message *msg) const;
		ssize_t recvmsg(message *msg) const;
#pragma endregion


#pragma region private methods
	private:
		int close();
#pragma endregion
	};
}

#endif