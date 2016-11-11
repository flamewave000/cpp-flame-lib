#pragma once
#ifndef _CLI_
#define _CLI_ 1

#ifndef _STRING_
#include <string>
#endif
#ifndef _VECTOR_
#include <vector>
#endif
#ifndef _TUPLE_
#include <tuple>
#endif
#ifndef _MEMORY_
#include <memory>
#endif
#ifndef _TYPEINFO
#include <typeinfo>
#endif
#ifndef _GLIBCXX_FUNCTIONAL
#include <functional>
#endif


typedef enum _data
{
	NoData = 0,
	Optional = 1,
	Required = 2
} Data;

class Base
{
public:
	virtual std::string toString() { return typeid(*this).name(); }
};

class _flag : public Base
{
#pragma region Instance Variables
public:
	std::string shortName; // Short name of flag(i.e. - i, -u john.doe).
	std::string longName; // Long name of flag (i.e. --interactive, --username=john.doe).
	std::string description; // Description of what the argument does.
	bool required; // True if the argument is required.
	Data data; // Takes the next argument in the argument list as data.
	std::function<void(std::string)> call; // Method to be called, must take one parameter. Will be the paired data, or None if data is False. 
#pragma endregion


#pragma region Constructors
public:
	/*Initializes the Flag class*/
	_flag(std::function<void(std::string)> call, std::string shortName, std::string longName, std::string description, Data data = Data::NoData, bool required = false);
#pragma endregion


#pragma region Public Methods
public:
	virtual std::string toString();
#pragma endregion
};

class Flag : public std::shared_ptr<_flag>
{
public:
	Flag();
	Flag(_flag * _Px);
	Flag(std::function<void(std::string)> call, std::string shortName, std::string longName, std::string description, Data data = Data::NoData, bool required = false);
};

class CLI : public Base
{
#pragma region Instance Variables
private:
	std::vector<Flag> _flags;
	std::string _program_name;
	std::string _description;
	std::function<int(std::vector<std::string>)> _startDelegate;
#pragma endregion


#pragma region Constructors
public:
	CLI(const char * program_name, std::function<int(std::vector<std::string>)> start, const std::vector<Flag> &flags, const std::string &description = "");
	virtual ~CLI() { }
#pragma endregion


#pragma region Public Methods
public:
	int run(const int &argc, const char* argv[]);
	int run(const std::vector<std::string> &argv);
	void showHelp(std::string error = "");
	std::string getProgramName();
	std::string getDescription();
#pragma endregion


#pragma region Helper Methods
private:
	std::string getArg(std::string arg);
	Flag getFlag(std::string name);
#pragma endregion
};

#endif
