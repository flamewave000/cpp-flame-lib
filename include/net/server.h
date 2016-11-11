#ifndef _NET_SERVER_
#define _NET_SERVER_ 1

#ifndef _CONNECTION_
#include "connection.h"
#endif

#ifndef _GLIBCXX_MUTEX
#include <mutex>
#endif

#ifndef _GLIBCXX_STRING
#include <string>
#endif


namespace std
{
	class thread;
}

namespace net
{
	class server
	{
#pragma region instance variables
	private:
		int _sfd;
		std::recursive_mutex _listening_mutex;
		std::thread * _listen_thread;
		bool _listening;
#pragma endregion


#pragma region constructors
	protected:
		server();
	public:
		virtual ~server();
#pragma endregion


#pragma region properties
	public:
		inline bool is_open() {
			std::lock_guard<std::recursive_mutex> lock(_listening_mutex);
			return _listen_thread != nullptr && _listening;
		}
#pragma endregion


#pragma region public methods
	public:
		int open(std::string addr_path);
		void kill();
#pragma endregion


#pragma region protected methods
	protected:
		virtual void connected(connection *conn) = 0;
		void stop_listening();
#pragma endregion


#pragma region thread methods
	private:
		static void listen(server* self);
#pragma endregion
	};
}

#endif