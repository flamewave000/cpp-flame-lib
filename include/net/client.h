#ifndef _CLIENT_
#define _CLIENT_ 1

#ifndef _GLIBCXX_FUNCTIONAL
#include <functional>
#endif
#ifndef _GLIBCXX_STRING
#include <string>
#endif

#ifndef _CONNECTION_
#include "connection.h"
#endif


namespace net
{
	class client
	{
#pragma region instance variables
	private:
		int _sfd;
#pragma endregion


#pragma region constructors
	public:
		client();
		virtual ~client();
#pragma endregion


#pragma region properties
	public:
		inline bool is_open() { return _sfd > 0; }
#pragma endregion


#pragma region public methods
	public:
		int connect(std::string addr_path, std::function<void(connection*)> handler);
#pragma endregion
	};
}

#endif
