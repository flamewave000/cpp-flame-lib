#include "json\jobject.h"
#include "json\json_errors.h"
#include "strext.h"
#include <sstream>

using namespace std;
using namespace std::json;

bool jobject::is_null() {
	return _json == "null";
}
bool jobject::contains(const string &key) {
	return _fields.find(key) != _fields.end();
}
void jobject::add(const string &key, const jvalue &value) {
	_fields[key] = value;
}
void jobject::remove(const string &key) {
	_fields.erase(key);
}

bool jobject::operator==(const nullptr_t &) {
	return is_null();
}
bool jobject::operator!=(const nullptr_t &) {
	return !is_null();
}
jvalue &jobject::operator[](const string &key) {
	return _fields[key];
}

string getFieldName(const string &json) {
	string name = ltrim(json);
	bool in_quote = false;
	bool backslash = false;
	char curr = 0;
	for (size_t c = 0, size = json.size(); c < size; c++) {
		curr = json[c];
		// Check if we are a quote and if this quote should be ignored
		if (curr == '"' && !backslash) {
			in_quote = !in_quote;
			// if we found the last quote
			if (!in_quote) {
				// return the json parsed string
				return jvalue().parse(json.substr(0, c + 1)).as_string();
			}
			continue;
		}
		// Check if we are a backslash
		else if (curr == '\\') {
			backslash = !backslash;
			continue;
		}
		// If we had a backslash but the following character is not one, clear the backslash.
		else if (backslash) {
			backslash = false;
		}
	}
	throw json_parse_error(format("Expected '\"' not {0}") % curr % format::end);
	return json;
}
string getFieldValue(const string &json) {
	string name = ltrim(json);
	bool in_quote = false;
	bool backslash = false;
	char curr = 0;
	for (size_t c = 0, size = json.size(); c < size; c++) {
		curr = json[c];
		// Check if we are a quote and if this quote should be ignored
		if (curr == '"' && !backslash) {
			in_quote = !in_quote;
			continue;
		}
		// Check if we are a backslash
		else if (curr == '\\') {
			backslash = !backslash;
			continue;
		}
		// If we had a backslash but the following character is not one, clear the backslash.
		else if (backslash) {
			backslash = false;
		}
		if (curr == ':' && !in_quote) {
			return json.substr(c + 1);
		}
	}
	throw json_parse_error(format("Expected '\"' not {0}") % curr % format::end);
	return json;
}
string getNextField(string &json) {
	bool in_quote = false;
	bool backslash = false;
	int bracket_count = 0;
	char curr = 0;
	for (size_t c = 0, size = json.size(); c < size; c++) {
		curr = json[c];
		// Check if we are a quote and if this quote should be ignored
		if (curr == '"' && !backslash) {
			in_quote = !in_quote;
			continue;
		}
		// Check if we are a backslash
		else if (curr == '\\') {
			backslash = !backslash;
			continue;
		}
		// If we had a backslash but the following character is not one, clear the backslash.
		else if (backslash) {
			backslash = false;
		}
		// Keep track of object/array brackets in case our field value is an object/array
		if ((curr == '{' || curr == '[') && !in_quote) {
			bracket_count++;
			continue;
		}
		else if ((curr == '}' || curr == ']') && !in_quote) {
			bracket_count--;
			if (bracket_count >= 0) {
				continue;
			}
		}
		// If we have hit a comma or a close bracket that is not a part of an internal object/array
		if ((curr == ',' || curr == '}') && bracket_count <= 0 && !in_quote) {
			string result = json.substr(0, c);
			json = json.substr(c + 1);
			return result;
		}
	}
	throw json_parse_error(format("Expected ',' or '}' not {0}") % curr % format::end);
	return json;
}

string &jobject::load(string json) {
	json = trim(json);
	if (json == "null") {
		_json = json;
		return _json;
	}

	size_t size = json.size(), first_bracket = json.find_first_of('{');
	if (first_bracket > json.find_first_not_of('{')) {
		size_t i = json.find_first_not_of('{');
		throw json_parse_error(format("not a valid json object. Expecting '{' but found '{0}' at index {1}") % json[i] % i % format::end);
		return json;
	}
	// Remove the first bracket
	json = json.substr(first_bracket + 1);
	_fields.clear();
	string kvp;
	while (json.size() > 0) {
		kvp = getNextField(json);
		_fields[getFieldName(kvp)] = jvalue().parse(getFieldValue(kvp));
		if (json.size() <= 0) {
			break;
		}
	}
}
string jobject::to_string() {
	if (is_null()) {
		return "null";
	}
	format fmt("\"{0}\": {1},");
	ostringstream out;
	out << '{';
	for (auto kvp : _fields) {
		out << (fmt % kvp.first % kvp.second.to_string() % format::endclr);
	}
	out << '}';
	return out.str();
}