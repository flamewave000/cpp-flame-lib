#include "json\jarray.h"
#include "json\json_errors.h"
#include "strext.h"
#include <sstream>

using namespace std;
using namespace std::json;


string jarray::to_string() {
	ostringstream out;
	out << '[';
	for (jvalue v : _elements) {
		out << v.to_string();
	}
	out << ']';
	return out.str();
}
string &jarray::load(string json) {
	json = trim(json);
	size_t size = json.size(), first_bracket = json.find_first_of('[');
	if (first_bracket > json.find_first_not_of('[')) {
		auto i = json.find_first_not_of('[');
		throw json_parse_error(format("not a valid json array. Expecting '[' but found '{0}' at index {1}")
			% json[i] % i % format::end);
		return json;
	}
	_elements.clear();
	bool in_quote = false;
	bool backslash = false;
	char curr = 0;
	size_t last_comma = first_bracket;
	int bracket_count = 0;
	for (size_t c = 0; c < size; c++) {
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
		// If we meet another array, make sure we track it
		else if (curr == '[' && !in_quote) {
			bracket_count++;
			continue;
		}
		// Decrement the array stack if we see a closing bracket
		else if (curr == ']' && !in_quote) {
			bracket_count--;
			// if we are on the last one, complete the loop
			if (bracket_count == 0) {
				return _json = json.erase(0, c + 1);
			}
			continue;
		}
		// If we had a backslash but the following character is not one, clear the backslash.
		else if (backslash) {
			backslash = false;
		}
		// We are at the end of the value
		if (curr == ',' && !in_quote) {
			_elements.push_back(jvalue().parse(json.substr(last_comma + 1, (c - last_comma) - 1)));
			last_comma = c;
		}
	}
	// Should never get here unless json string is badly formed
	throw json_parse_error("could not find the end of the array. expected ']'");
	return json;
}