#include "../str.h"
#include "ini.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

#define INI_KVP_DELIM '='
#define INI_SECTION_START '['
#define INI_SECTION_END ']'
#define EMPTY_STR ""


string extract_section(const string &line) throw(invalid_argument) {
	if (line[0] == INI_SECTION_START && line[line.size() - 1] == INI_SECTION_END) {
		return line.substr(1, line.size() - 2);
	}
	else {
		throw invalid_argument("line not a section header");
	}
	return EMPTY_STR;
}
pair<string, string> get_kvp(const string &line) throw(invalid_argument) {
	pair<string, string> kvp;
	size_t delim_index = line.find_first_of(INI_KVP_DELIM);
	if (delim_index != string::npos) {
		// If there's text before the delimeter, grab it; otherwise set as empty
		kvp.first = delim_index > 0 ? line.substr(0, delim_index) : EMPTY_STR;
		// If there's text after the delimeter, grab it; otherwise set as empty
		kvp.second = delim_index < line.size() - 1 ? line.substr(delim_index + 1) : EMPTY_STR;
	}
	else {
		throw invalid_argument("line is not a valid Key-Value_pair");
	}
	return kvp;
}


ini_section_collection std::ini_readf(const char *filename, ostream &_log)
{
	ini_section_collection result;
	ifstream stream(filename, ios::in);
	if (!stream.is_open()) {
		_log << "could not open file: " << filename << endl;
		return result;
	}
	pair<string, ini_kvp_collection> cur_section;
	string line;
	for (int line_num = 1; getline(stream, line); line_num++)
	{
		str::trim(line);
		if (line.size() <= 0) {
			continue;
		}
		try {
			string section = extract_section(line);
			if (cur_section.second.size() > 0) {
				if (result.find(cur_section.first) != result.end()) {
					_log << "duplicate section '"
						<< cur_section.first
						<< "' found on line "
						<< line_num
						<< ", overriding previous"
						<< endl;
				}
				result[cur_section.first] = cur_section.second;
				cur_section.second.clear();
			}
			cur_section.first = section;
		}
		catch (invalid_argument &e) {
			try {
				pair<string, string> kvp = get_kvp(line);
				if (cur_section.second.find(kvp.first) != cur_section.second.end())
				{
					_log << "duplicate key '"
						<< kvp.first
						<< "' found on line "
						<< line_num
						<< ", overriding previous"
						<< endl;
				}
				cur_section.second[kvp.first] = kvp.second;
			}
			catch (invalid_argument &e) {
				stringstream str;
				str << "Error at line " << line_num << ": invalid line is not a section header or a key-value pair";
				throw invalid_argument(str.str());
			}
		}
	}
	result[cur_section.first] = cur_section.second;
	return result;
}
//void ini_writef(const char *filename, ini_section_collection collection) {
//
//}
//
//ini_section_collection ini_reads(const char *str) {
//	return ini_section_collection();
//}
//string ini_writes(ini_section_collection collection) {
//	return "";
//}
