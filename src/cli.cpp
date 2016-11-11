#include "../util/str.h"
#include "cli.h"
#include <queue>
#include <iostream>
#include <sstream>


using namespace std;
using namespace str;


#pragma region _flag Definition
#pragma region Constructors
_flag::_flag(function<void(string)> call, string shortName, string longName, string description, Data data, bool required)
	: shortName(shortName),
	longName(longName),
	description(description),
	required(required),
	data(data),
	call(call) { }
#pragma endregion


#pragma region Public Methods
string _flag::toString()
{
	return shortName.size() > 0 ? shortName : longName;
}
#pragma endregion
#pragma endregion


#pragma region Flag Definition
#pragma region Constructors
Flag::Flag() : shared_ptr<_flag>() { }
Flag::Flag(_flag * _Px) : shared_ptr<_flag>(_Px) { }
Flag::Flag(function<void(string)> call, string shortName, string longName, string description, Data data, bool required)
	: shared_ptr<_flag>(new _flag(call, shortName, longName, description, data, required)) { }
#pragma endregion
#pragma endregion


#pragma region CLI Definition
#pragma region Constructors
CLI::CLI(const char * program_name,
	function<int(vector<string>)> start,
	const vector<Flag> &flags,
	const string &description)
{
	this->_program_name = program_name;
	this->_startDelegate = start;
	this->_flags = flags;
	this->_description = description;
}

#pragma endregion


#pragma region Public Methods
int CLI::run(const int &argc, const char* argv[])
{
	vector<string> args = vector<string>(argc);
	for (int c = 0; c < argc; c++)
	{
		args[c] = string(argv[c]);
	}
	return run(args);
}
int CLI::run(const vector<string> &argv)
{
	queue<string> args;
	queue<Flag> flags;
	vector<Flag> reqs;
	for (auto req : this->_flags)
	{
		if (req->required)
		{
			reqs.push_back(req);
		}
	}
	vector<string> unknown;
	if (argv.size() > 1 && argv[1] == "--help")
	{
		showHelp();
		return EXIT_SUCCESS;
	}
	for (size_t c = 1, size = argv.size(); c < size; c++)
	{
		args.push(argv[c]);
	}
	Flag flag;
	while (!args.empty())
	{
		string rawArg = args.front();
		args.pop();
		string arg = getArg(rawArg);
		flag = getFlag(arg);
		if (flag == nullptr)
		{
			unknown.push_back(rawArg);
			continue;
		}
		for (size_t c = 0, size = reqs.size(); c < size; c++)
		{
			if (flag == reqs[c])
			{
				reqs.erase(reqs.begin() + c);
				break;
			}
		}
		string data = "";
		if (flag->data != Data::NoData)
		{
			if (arg == flag->shortName)
			{
				string tmp = !args.empty() ? getArg(args.front()) : string();
				bool isFlag = getFlag(tmp) != nullptr;
				if (flag->data == Data::Required && (args.empty() || isFlag))
				{
					showHelp("Expected data for flag '" + arg + "'");
					return EXIT_FAILURE;
				}
				else if (flag->data == Data::Required || (flag->data == Data::Optional && !args.empty() && !isFlag))
				{
					data = args.front();
					args.pop();
				}
			}
			else
			{
				if (contains(rawArg, '='))
				{
					data = join(split(rawArg, '='), '=');
				}
				else
				{
					showHelp("Expected data for flag '" + arg + "'");
					return EXIT_FAILURE;
				}
			}
		}
		flag->call(data);
	}
	if (reqs.size() > 0)
	{
		vector<string> strs;
		for (auto req : reqs)
		{
			strs.push_back(req->toString());
		}
		showHelp("Missing required arguments [" + join(strs, ", ") + ']');
		return EXIT_FAILURE;
	}
	return _startDelegate(unknown);
}
void CLI::showHelp(string error)
{
	if (error.size() != 0)
	{
		cerr << error << '\n';
	}
	if (_description.size() == 0)
		printf("usage: %s [options]\n", _program_name.c_str());
	else
		cout << _description << '\n';
	size_t shortLen = 0, longLen = 0, snameLen = 0, lnameLen = 0;
	for (auto flag : _flags)
	{
		if (flag->shortName.size() != 0)
		{
			snameLen = flag->shortName.size();
			shortLen = snameLen > shortLen ? snameLen : shortLen;
		}
		if (flag->longName.size() != 0)
		{
			lnameLen = flag->longName.size();
			longLen = lnameLen > longLen ? lnameLen : longLen;
		}
	}
	string shortName;
	string longName;
	string desc;
	string cmdStr;
	vector<size_t> newlineIndecies;
	size_t count, max;
	for (auto flag : _flags)
	{
		shortName = flag->shortName.size() != 0 && trim(flag->shortName).size() ? flag->shortName : "";
		longName = flag->longName.size() != 0 && trim(flag->longName).size() ? flag->longName : "    ";
		desc = flag->description.size() != 0 ? flag->description : "";
		ostringstream sstream;
		sstream
			<< rpad(shortName, shortLen + 2)
			<< rpad(longName, longLen + 2);
		cmdStr = sstream.str();

		newlineIndecies.clear();
		count = 0;
		max = 80 - cmdStr.size();
		for (size_t c = 0, size = desc.size(); c < size; c++)
		{
			if (desc[c] == '\n')
				count = 0;
			else
				count += 1;
			if (count == max)
			{
				newlineIndecies.push_back(c);
				count = 0;
			}
		}
		count = 0;
		for (auto index : newlineIndecies)
		{
			desc.insert(desc.begin() + index, '\n');
			count += 1;
		}
		vector<string> lines = split(desc, '\n');
		desc = join(lines, rpad("\n", cmdStr.size() + 1));
		cerr << cmdStr << desc << endl;
	}
}
std::string CLI::getProgramName() {
	return _program_name;
}
std::string CLI::getDescription() {
	return _description;
}
#pragma endregion


#pragma region Helper Methods
string CLI::getArg(string arg)
{
	if (arg.size() == 0)
	{
		return string(nullptr);
	}
	return str::contains(arg, '=') ? str::split(arg, '=')[0] : arg;
}
Flag CLI::getFlag(string name)
{
	if (name.size() == 0)
	{
		return nullptr;
	}
	for (Flag flag : _flags)
	{
		if (flag->shortName == name)
		{
			return flag;
		}
		else if (flag->longName == name)
		{
			return flag;
		}
	}
	return nullptr;
}
#pragma endregion  
#pragma endregion