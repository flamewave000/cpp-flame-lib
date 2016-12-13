#pragma once
#ifndef _JVALUE_H
#define _JVALUE_H 1
#ifndef _JBASE_H
#include "jbase.h"
#endif

namespace std
{
	namespace json
	{
		class jobject;
		class jarray;
		class jnumber;
		class jvalue : public jbase
		{
		public:
			jobject as_object();
			jarray as_array();
			jnumber as_number();
			string as_string();
			inline jvalue& parse(string json) {
				load(json);
				return *this;
			}
		protected:
			virtual string &load(string json);
		public:
			virtual string to_string();
		};
	}
}


#endif