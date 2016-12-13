#pragma once
#ifndef _JOBJECT_H
#define _JOBJECT_H 1

#ifndef _GLIBCXX_UNORDERED_MAP
#ifndef _UNORDERED_MAP_
#include <unordered_map>
#endif
#endif

#ifndef _JVALUE_H
#include "jvalue.h"
#endif
#ifndef _JBASE_H
#include "jbase.h"
#endif

namespace std
{
	namespace json
	{
		class jobject : public jbase
		{
		private:
			unordered_map<string, jvalue> _fields;
		public:
			bool is_null();
			bool contains(const string &key);
			void add(const string &key, const jvalue &value);
			void remove(const string &key);

			bool operator==(const nullptr_t &);
			bool operator!=(const nullptr_t &);
			jvalue &operator[](const string &key);
		protected:
			virtual string &load(string json);
		public:
			virtual string to_string();
		};
	}
}


#endif