#include "log.h"

#include <ctime>
#include <stdarg.h>
#include <stdio.h>
#include <iostream>

using namespace std;

std::string log::timestamp()
{
	time_t t = time(0);// get time now
	tm * now = localtime(&t);
	char buff[20];
	sprintf(buff, "%04d-%02d-%02d %02d:%02d:%02d", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
	return buff;
}
void log::write(const log::level &l, std::string fmt, ...)
{
	va_list args;
	va_start(args, &fmt);
	vsyslog(l, fmt.c_str(), args);
#ifdef DEBUG
	vfprintf(stderr, (timestamp() + ' ' + fmt + '\n').c_str(), args);
	fflush(stderr);
#endif
	va_end(args);
}

#ifdef DEBUG
void log::va_write(const level &l, std::string fmt, va_list args) {
	va_list args2;
	va_copy(args2, args);
	vsyslog(l, (timestamp() + fmt).c_str(), args);
	vfprintf(stderr, (timestamp() + ' ' + fmt + '\n').c_str(), args2);
	fflush(stderr);
}
#endif

#define VA_WRITE_CALL(log_level, level_str) \
va_list args;\
va_start(args, &fmt);\
va_write((log_level), "[" level_str "] " + fmt, args);\
va_end(args)

void log::d(std::string fmt, ...) { VA_WRITE_CALL(level::debug, "DEBUG"); }
void log::i(std::string fmt, ...) { VA_WRITE_CALL(level::info, "INFO"); }
void log::n(std::string fmt, ...) { VA_WRITE_CALL(level::notice, "NOTICE"); }
void log::w(std::string fmt, ...) { VA_WRITE_CALL(level::warning, "WARNING"); }
void log::e(std::string fmt, ...) { VA_WRITE_CALL(level::err, "ERROR"); }

#undef VA_WRITE_CALL