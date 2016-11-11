#pragma once
#ifndef _STR_
#define _STR_

#ifndef _STRING_
#include <string>
#endif
#ifndef _VECTOR_
#include <vector>
#endif


namespace str
{
	extern const std::string whitespace;

	inline bool contains(const std::string &str, const char &c)
	{
		return str.find_first_of(c) != std::string::npos;
	}
	inline bool contains(const std::string &target, const std::string &query)
	{
		return target.find(query) != std::string::npos;
	}

	std::vector<std::string> split(std::string str, const char &delim);

	std::string join(const std::vector<std::string> &tokens, const char &delim);
	std::string join(const std::vector<std::string> &tokens, const std::string &delim);

	std::string replace(const std::string &str, const std::string *patterns, size_t length, const std::string &replacement);
	std::string replace(const std::string &str, const std::string &pattern, const std::string &replacement);
	std::string replace(const std::string &str, const char &c, const std::string &replacement);
	std::string replace(const std::string &str, const std::string &pattern, const char &replacement);
	std::string replace(const std::string &str, const char &pattern, const char &replacement);

	inline std::string lpad(std::string str, size_t width, const char pad = ' ')
	{
		width -= str.size();
		return width > 0 ? str.insert(0, width, pad) : str;
	}
	inline std::string rpad(std::string str, size_t width, const char pad = ' ')
	{
		size_t size = str.size();
		width -= size;
		return width > 0 ? str.insert(size, width, pad) : str;
	}

	inline std::string ltrim(std::string str)
	{
		if (str.size() == 0) return str;
		str.erase(str.begin(), str.begin() + str.find_first_not_of(whitespace));
		return str;
	}
	inline std::string rtrim(std::string str)
	{
		if (str.size() == 0) return str;
		str.erase(str.begin() + str.find_last_not_of(whitespace) + 1, str.end());
		return str;
	}
	inline std::string trim(std::string str)
	{
		return rtrim(ltrim(str));
	}
}
#endif