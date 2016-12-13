#include "json\jnumber.h"
#include "json\json_errors.h"
#include "strext.h"

using namespace std;
using namespace std::json;

string &jnumber::load(string json) {
	static const char * ALL_NUM_CHARS = "-+0123456789.eE";
	static const char * NUM_CHARS = "0123456789";
	json = trim(json);
	if (find_first_of_pat(json, "true") == 0) {
		return _json = json.substr(0, 4);
	}
	else if (find_first_of_pat(json, "false") == 0) {
		return _json = json.substr(0, 5);
	}
	json = trim(json);
	size_t start = json.find_first_of(NUM_CHARS);
	size_t end = json.find_first_not_of(NUM_CHARS, start);
	json = json.substr(start, end);
	if (json.find_first_of(NUM_CHARS) != string::npos) {
		return _json = json;
	}
	throw json_parse_error("json string not a valid number");
	return json;
}