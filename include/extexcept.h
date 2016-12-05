#ifndef _EXTEXCEPT_H
#define _EXTEXCEPT_H 1
#ifndef _GLIBCXX_STDEXCEPT
#include <stdexcept>
#endif

namespace std {
	class nullptr_error : public runtime_error {
	public:
		/** Takes a character string describing the error.  */
		explicit nullptr_error(const string& __arg) : runtime_error(__arg) {}
#if __cplusplus >= 201103L
		explicit nullptr_error(const char* __arg) : runtime_error(__arg) {}
#endif
	};
	class not_implemented : public runtime_error {
	public:
		/** Takes a character string describing the error.  */
		explicit not_implemented(const string& __arg) : runtime_error(__arg) {}
#if __cplusplus >= 201103L
		explicit not_implemented(const char* __arg) : runtime_error(__arg) {}
#endif
	};
}

#endif