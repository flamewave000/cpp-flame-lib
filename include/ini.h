#ifndef _INI_
#define _INI_
#include <unordered_map>
#include <string>
#include <iostream>

namespace std
{
	typedef unordered_map<string, string> ini_kvp_collection;
	typedef unordered_map<string, ini_kvp_collection> ini_section_collection;

	ini_section_collection ini_readf(const char *filename, ostream &_log = clog);
	//void ini_writef(const char *filename, ini_section_collection collection);

	//ini_section_collection ini_reads(const char *str);
	//string ini_writes(ini_section_collection collection);
}
#endif
