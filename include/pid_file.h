#ifndef _PID_
#define _PID_

#ifndef _GLIBCXX_STRING
#include <string>
#endif


class pid_file
{
private:
	pid_file() {}
public:
	static bool exists();
	static int get();
	static void set(int pid);
	static int clear();
	static void set_file_path(const std::string &file_path);
	static std::string get_file_path();
};

#endif
