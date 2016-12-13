#pragma once
#ifndef _JBASE_H
#define _JBASE_H 1
#ifndef _STRING_
#include <string>
#endif

namespace std
{
	namespace json
	{
		class jbase
		{
		protected:
			string _json;
		protected:
			jbase() {}
		protected:
			inline string &load_other(jbase *other, string json) { return other->load(json); }
			virtual string &load(string json) = 0;
		public:
			inline virtual string to_string() { return _json; }
			inline operator string() { return to_string(); }
		};
	}
}

#endif