#pragma once
#ifndef _STREXT_H
#define _STREXT_H 1

#ifndef _STRING_
#include <string>
#endif
#ifndef _STRING_H
#include <string.h>
#endif
#ifndef _VECTOR_
#include <vector>
#endif
#ifndef _STDIO_H
#include <stdio.h>
#endif


namespace std
{
#pragma region globals
	extern const string whitespace;

	inline bool contains(const string &str, const char &c) { return str.find_first_of(c) != string::npos; }
	inline bool contains(const string &target, const string &query) { return target.find(query) != string::npos; }
	inline bool starts_with(const string & str, const string &pattern) { return str.size() >= pattern.size() && str.find_first_of(pattern) == 0; }

	size_t find_first_of_pat(const string &str, const string &pattern);

	vector<string> split(string str, const char &delim);

	string join(const vector<string> &tokens, const char &delim);
	string join(const vector<string> &tokens, const string &delim);

	string replace(const string &str, const string *patterns, size_t length, const string &replacement);
	string replace(const string &str, const string &pattern, const string &replacement);
	string replace(const string &str, const char &c, const string &replacement);
	string replace(const string &str, const string &pattern, const char &replacement);
	string replace(const string &str, const char &pattern, const char &replacement);

	inline string lpad(string str, size_t width, const char pad = ' ')
	{
		width -= str.size();
		return width > 0 ? str.insert(0, width, pad) : str;
	}
	inline string rpad(string str, size_t width, const char pad = ' ')
	{
		size_t size = str.size();
		width -= size;
		return width > 0 ? str.insert(size, width, pad) : str;
	}

	inline string ltrim(string str)
	{
		if (str.size() == 0) return str;
		str.erase(str.begin(), str.begin() + str.find_first_not_of(whitespace));
		return str;
	}
	inline string rtrim(string str)
	{
		if (str.size() == 0) return str;
		str.erase(str.begin() + str.find_last_not_of(whitespace) + 1, str.end());
		return str;
	}
	inline string trim(const string &str)
	{
		return rtrim(ltrim(str));
	}

	inline bool s_empty(const string &str) { return str.size() == 0; }
	inline bool s_whitespace(const string &str) { return str.size() == 0 || str.find_first_not_of(whitespace) != string::npos; }

	string str2lower(string str);
	string str2upper(string str);

	inline bool stob(string str) {
		str = trim(str);
		return str == "true" || str == "True" || str == "1";
	}
	inline int8_t stobytec(string str) { return (int8_t)stol(str); }
	inline uint8_t stoubyte(string str) { return (uint8_t)stoul(str); }

	string ftos(string path);
	string ftos(FILE* file);
#pragma endregion


	/// <summary>
	/// Formatter for <seealso cref="string"/> which provides simplified string formatting
	/// </summary>
	/// <example>
	/// // Simple
	/// string str = "{0} {1}"_f
	///     % "Hello"
	///     % "World"
	///     % format::end;
	/// // Custom
	/// string str = "Average {0}"_f
	///     % format::spec("%#.3f", (1.0 + 2.3 + 4.7) / 3)
	///     % format::end;
	/// </example>
	class format
	{
#pragma region internal classes
	private:
		struct __end {};
		struct __endclr {};
#pragma endregion


#pragma region class variables
	public:
		/// <summary>
		/// Signifies the end of a string format. Will cause the formatter to return a string at the end.
		/// </summary>
		static __end end;
		/// <summary>
		/// Signifies the end of a string format. Will cause the formatter to return a string at the end, then clear the provided parameters.
		/// </summary>
		static __endclr endclr;
#pragma endregion


#pragma region instance variables
	private:
		string _buffer;
		vector<string> _params;
#pragma endregion


#pragma region constructors
	public:
		/// <summary>
		/// Constructs the formatter using the provided <paramref name="format"/> string.
		/// </summary>
		/// <param name="format">Format string</param>
		format(const string &format) : _buffer(format) {}
		format(const char *format) : _buffer(format) {}
#pragma endregion


#pragma region operator overloads
	public:
		inline string &operator[](const size_t &index) {
			return _params[index];
		}
		inline const string &operator[](const size_t &index) const {
			return _params[index];
		}
		inline format& operator%(const char &c) {
			_params.push_back(string(1, c));
			return *this;
		}
		inline format& operator%(const int16_t &s) {
			_params.push_back(spec("%hd", s, 6));
			return *this;
		}
		inline format& operator%(const uint16_t &s) {
			_params.push_back(spec("%hu", s, 6));
			return *this;
		}
		inline format& operator%(const int32_t &i) {
			_params.push_back(spec("%d", i, 11));
			return *this;
		}
		inline format& operator%(const uint32_t &i) {
			_params.push_back(spec("%du", i, 11));
			return *this;
		}
		inline format& operator%(const int64_t &l) {
			_params.push_back(spec("%ll", l, 20));
			return *this;
		}
		inline format& operator%(const uint64_t &l) {
			_params.push_back(spec("%llu", l, 21));
			return *this;
		}
		inline format& operator%(const float &f) {
			_params.push_back(spec("%f", f, 30));
			return *this;
		}
		inline format& operator%(const double &d) {
			_params.push_back(spec("%f", d, 30));
			return *this;
		}
		inline format& operator%(const char* c_str) {
			_params.push_back(c_str);
			return *this;
		}
		inline format& operator%(const string &str) {
			_params.push_back(str);
			return *this;
		}
		inline string operator%(const __end &e) const {
			return this->str();
		}
		inline string operator%(const __endclr &e) {
			return this->strclr();
		}
#pragma endregion


#pragma region public methods
	public:
		/// <summary>
		/// Provides the number of parameters which have been provided so far.
		/// </summary>
		/// <returns>Number of parameters provided so far.</returns>
		inline size_t count() {
			return _params.size();
		}
		/// <summary>
		/// Appends the provided string onto the end of the argument list.
		/// </summary>
		/// <param name="arg">string to be added to list.</param>
		inline void append(const string &arg) {
			_params.push_back(arg);
		}
		/// <summary>
		/// Inserts the provided string into the provided index of the argument list.
		/// </summary>
		/// <param name="arg">string to be inserted in the list</param>
		inline void insert(const size_t &index, const string &arg) {
			_params.insert(_params.begin() + index, arg);
		}
		/// <summary>
		/// Replaces the formatted string parameter at the given index in the parameter list.
		/// </summary>
		/// <param name="index">Index of parameter to be replaced.</param>
		/// <param name="arg">New value that is to replace the old value.</param>
		inline void replace(size_t index, string arg) {
			_params[index] = arg;
		}
		/// <summary>
		/// Clears all parameters provided up until now.
		/// </summary>
		inline void clear() {
			_params.clear();
		}
		/// <summary>
		/// Builds the formatted string using the previously provided parameters.
		/// </summary>
		/// <returns>Formatted string.</returns>
		string str() const;
		/// <summary>
		/// Builds the formatted string using the previously provided parameters, then clears the parameters.
		/// </summary>
		/// <returns>Formatted string.</returns>
		string strclr();
		/// <summary>
		/// Generates a specialized string representation of the provided value <paramref name="arg"/>.
		/// </summary>
		/// <param name="fmt">The specialized format the value is to be formatted to. (i.e "%#.2f")</param>
		/// <param name="arg">The value to be formatted into the resulting string.</param>
		/// <param name="buff_size">The size of buffer to use for the underlying <seealso cref="snprintf"/> call.</param>
		/// <returns>The formatted string.</returns>
		template<typename T>
		static string spec(const char * fmt, const T &arg, const size_t &buff_size = 32) {
			string buff;
			buff.resize(buff_size + 1);
			int written = snprintf(const_cast<char*>(buff.data()), buff.size(), fmt, arg);
			buff = buff.substr(0, written);
			return buff;
		}
#pragma endregion
	};

#pragma region Formatter User Defined Literals
	inline format operator "" _f(const char *c_str, size_t length) { return move(format(string(move(c_str), move(length)))); }
#pragma endregion


#pragma region Global Operator Overloads
	inline format operator%(const string &str, const char &val) { return format(str) % val; }
	inline format operator%(const string &str, const int16_t &val) { return format(str) % val; }
	inline format operator%(const string &str, const uint16_t &val) { return format(str) % val; }
	inline format operator%(const string &str, const int32_t &val) { return format(str) % val; }
	inline format operator%(const string &str, const uint32_t &val) { return format(str) % val; }
	inline format operator%(const string &str, const int64_t &val) { return format(str) % val; }
	inline format operator%(const string &str, const uint64_t &val) { return format(str) % val; }
	inline format operator%(const string &str, const float &val) { return format(str) % val; }
	inline format operator%(const string &str, const double &val) { return format(str) % val; }
	inline format operator%(const string &str, const char* val) { return format(str) % val; }
	inline format operator%(const string &str, const string &val) { return format(str) % val; }
#pragma endregion
}
#endif