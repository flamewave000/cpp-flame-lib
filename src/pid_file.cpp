#include "pid_file.h"
#ifndef _GLIBCXX_FSTREAM
#include <fstream>
#endif
#ifndef _UNISTD_H
#include <unistd.h>
#endif

using namespace std;

string PID_FILE = ".pid";

bool pid_file::exists()
{
	FILE* fpid = ::fopen(PID_FILE.c_str(), "r");
	if (fpid != nullptr)
	{
		fclose(fpid);
		fpid = nullptr;
		return true;
	}
	return false;
}
int pid_file::get()
{
	ifstream fpid(PID_FILE);
	if (fpid) {
		int pid = -1;
		fpid >> pid;
		return pid;
	}
	else {
		__throw_runtime_error("no pid file found");
		return -1;
	}
}
void pid_file::set(int pid)
{
	ofstream fpid(PID_FILE, ofstream::out | ofstream::trunc);
	fpid << pid << endl;
	fpid.close();
}
int pid_file::clear()
{
	return ::remove(PID_FILE.c_str());
}
void pid_file::set_file_path(const std::string &file_path)
{
	PID_FILE = file_path;
}
std::string pid_file::get_file_path()
{
	return PID_FILE;
}
