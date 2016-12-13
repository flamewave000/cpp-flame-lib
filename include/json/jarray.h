#pragma once
#ifndef _JARRAY_H
#define _JARRAY_H 1

#ifndef _GLIBCXX_VECTOR
#ifndef _VECTOR_
#include <vector>
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
		class jarray : public jbase
		{
		private:
			vector<jvalue> _elements;
		public:
			inline void add(const jvalue &value) { _elements.push_back(value); }
			inline void remove(size_t index) { _elements.erase(_elements.begin() + index); }
			inline size_t size() { return _elements.size(); }
			inline jvalue &operator[](size_t index) { return _elements[index]; }
		public:
			virtual string to_string();
		protected:
			virtual string &load(string json);
		};
	}
}


#endif