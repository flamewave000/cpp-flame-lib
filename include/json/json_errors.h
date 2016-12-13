#pragma once
#ifndef _JSON_ERRORS_H
#define _JSON_ERRORS_H

#ifndef _STDEXCEPT_
#include <stdexcept>
#endif

namespace std
{
	namespace json
	{
		class json_parse_error : public std::runtime_error
		{
		public:
			explicit json_parse_error(string _Message) : runtime_error(_Message) {}
			explicit json_parse_error(const char *_Message) : runtime_error(_Message) {}
		};
	}
}


#endif