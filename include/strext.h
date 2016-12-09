#pragma once
#ifndef _STREXT_H
#define _STREXT_H 1

#ifndef _STRING_
#include <string>
#endif
#ifndef _VECTOR_
#include <vector>
#endif


namespace std
{
#pragma region globals
	extern const std::string whitespace;

	inline bool contains(const std::string &str, const char &c)
	{
		return str.find_first_of(c) != std::string::npos;
	}
	inline bool contains(const std::string &target, const std::string &query)
	{
		return target.find(query) != std::string::npos;
	}

	size_t find_first_of_pat(const std::string &str, const std::string &pattern);

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
	inline std::string trim(const std::string &str)
	{
		return rtrim(ltrim(str));
	}
#pragma endregion


	/// <summary>
	/// Formatter for <seealso cref="std::string"/> which provides simplified string formatting
	/// </summary>
	/// <example>
	/// // Simple
	/// std::string str = format("{0} {1}")
	///     % "Hello"
	///     % "World"
	///     % format::end;
	/// // Custom
	/// std::string str = format("Average {0}")
	///     % format::spec("%#.3f", (1.0 + 2.3 + 4.7) / 3)
	///     % format::end;
	/// </example>
	class format {
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
		std::string _buffer;
		std::vector<std::string> _params;
#pragma endregion


#pragma region constructors
	public:
		/// <summary>
		/// Constructs the formatter using the provided <paramref name="format"/> string.
		/// </summary>
		/// <param name="format">Format string</param>
		format(std::string format) : _buffer(format) {}
#pragma endregion


#pragma region operator overloads
	public:
		inline std::string &operator[](size_t index) {
			return _params[index];
		}
		inline const std::string &operator[](size_t index) const {
			return _params[index];
		}
		inline format& operator%(const char &c) {
			_params.push_back("" + c);
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
		inline format& operator%(const std::string &str) {
			_params.push_back(str);
			return *this;
		}
		inline std::string operator%(const __end &e) const {
			return this->str();
		}
		inline std::string operator%(const __endclr &e) {
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
		/// Replaces the formatted string parameter at the given index in the parameter list.
		/// </summary>
		/// <param name="index">Index of parameter to be replaced.</param>
		/// <param name="arg">New value that is to replace the old value.</param>
		inline void replace(size_t index, std::string arg) {
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
		std::string str() const;
		/// <summary>
		/// Builds the formatted string using the previously provided parameters, then clears the parameters.
		/// </summary>
		/// <returns>Formatted string.</returns>
		std::string strclr();
		/// <summary>
		/// Generates a specialized string representation of the provided value <paramref name="arg"/>.
		/// </summary>
		/// <param name="fmt">The specialized format the value is to be formatted to. (i.e "%#.2f")</param>
		/// <param name="arg">The value to be formatted into the resulting string.</param>
		/// <param name="buff_size">The size of buffer to use for the underlying <seealso cref="snprintf"/> call.</param>
		/// <returns>The formatted string.</returns>
		template<typename T>
		static std::string spec(const char * fmt, const T &arg, const size_t &buff_size = 32) {
			std::string buff;
			buff.resize(buff_size + 1);
			int written = snprintf(const_cast<char*>(buff.data()), buff.size(), fmt, arg);
			buff = buff.substr(0, written);
			return buff;
		}
#pragma endregion
	};
}
#endif