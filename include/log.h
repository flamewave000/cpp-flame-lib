#ifndef _LOG_
#define _LOG_ 1

#ifndef _GLIBCXX_STRING
#include <string>
#endif

#ifndef _GLIBCXX_SYSLOG
#include <syslog.h>
#endif

#ifndef	_STRING_H
#include <string.h>
#endif


class log {
public:
	enum level {
		emerg = LOG_EMERG, 	   // system is unusable
		alert = LOG_ALERT,	   // action must be taken immediately
		crit = LOG_CRIT,   	   // critical conditions
		err = LOG_ERR,	   	   // error conditions
		warning = LOG_WARNING, // warning conditions
		notice = LOG_NOTICE,   // normal, but significant, condition
		info = LOG_INFO,	   // informational message
		debug = LOG_DEBUG	   // debug - level message
	};
	enum options {
		cons = LOG_CONS,	 // Write directly to system console if there is an error while sending to system logger.
		ndelay = LOG_NDELAY, // Open the connection immediately (normally, the connection is opened when the first message is logged).
		nowait = LOG_NOWAIT, // Don't wait for child processes that may have been created while logging the message.  (The GNU C library does not create a child process, so this option has no effect on Linux.)
		odelay = LOG_ODELAY, // The converse of LOG_NDELAY; opening of the connection is delayed until syslog() is called.  (This is the default, and need not be specified.)
		perror = LOG_PERROR, // (Not in POSIX.1-2001 or POSIX.1-2008.)  Print to stderr as well.
		pid = LOG_PID		 // Include PID with each message.
	};
	static std::string timestamp();

	static void write(const level &l, std::string fmt, ...);
#ifndef DEBUG
	static inline void va_write(const level &l, std::string fmt, va_list args) { vsyslog(l, fmt.c_str(), args); }
#else
	static void va_write(const level &l, std::string fmt, va_list args);
#endif

	static void d(std::string fmt, ...);
	static void i(std::string fmt, ...);
	static void n(std::string fmt, ...);
	static void w(std::string fmt, ...);
	static void e(std::string fmt, ...);
	static inline void herrno(std::string msg, int error) {
		const char * errmsg = strerror(error);
		e(msg);
		e("Error Code: %i", error);
		e("Error Message: %s", errmsg);
	}

	static inline void open(const char *program_identifier, int options) { openlog(program_identifier, options, LOG_DAEMON); }
	static inline void close() { closelog(); }
};

#endif