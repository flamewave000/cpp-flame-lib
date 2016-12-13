#pragma once
#ifndef _JNUMBER_H
#define _JNUMBER_H 1

#include <stdint.h>

#ifndef _JBASE_H
#include "jbase.h"
#endif
#include "strext.h"

namespace std
{
	namespace json
	{
		class jnumber : public jbase
		{
		protected:
			virtual string &load(string json);
		public:
			inline int32_t  jnumber::to_int32() { return stol(trim(_json)); }
			inline uint32_t jnumber::to_uint32() { return stoul(trim(_json)); }
			inline int64_t  jnumber::to_int64() { return stoll(trim(_json)); }
			inline uint64_t jnumber::to_uint64() { return stoull(trim(_json)); }
			inline bool     jnumber::to_bool() { return stob(trim(_json)); }
			inline float    jnumber::to_float() { return stof(trim(_json)); }
			inline double   jnumber::to_double() { return stod(trim(_json)); }
			inline void set_value(int32_t value)  { _json = format::spec("%d", value); }
			inline void set_value(uint32_t value) { _json = format::spec("%ud", value); }
			inline void set_value(int64_t value)  { _json = format::spec("%ll", value); }
			inline void set_value(uint64_t value) { _json = format::spec("%ull", value); }
			inline void set_value(bool value)     { _json = value ? "true" : "false"; }
			inline void set_value(double value)   { _json = format::spec("%f", value); }
		};
	}
}


#endif