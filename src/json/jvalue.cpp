#include "json/jvalue.h"
#include "json/jobject.h"
#include "json/jarray.h"
#include "json/jnumber.h"
#include "json/json_errors.h"
#include "strext.h"


using namespace std;
using namespace std::json;


jobject jvalue::as_object() {
	jobject result;
	load_other(&result, _json);
	return result;
}
jarray jvalue::as_array() {
	jarray result;
	load_other(&result, _json);
	return result;
}
jnumber jvalue::as_number() {
	jnumber result;
	load_other(&result, _json);
	return result;
}
string jvalue::as_string() {
	string result;
	result = trim(_json);
	if (result[0] != '"') {
		throw json_parse_error("jvalue is not a string\n" + result);
	}
	size_t last_quote = 0, size = result.size();
	bool bracket = false;
	for (size_t c = 1; c < size; c++) {
		if (result[c] == '\\') {
			bracket = !bracket;
		}
		else if (result[c] == '"' && !bracket) {
			last_quote = c;
			break;
		}
		else if (bracket) {
			bracket = false;
		}
	}
	if (last_quote + 1 != size) {
		// The entire value is not part of the string. this is an invalid situation
		result = "";
		throw json_parse_error("jvalue is not a string" + result);
	}
	return result.substr(1, size - 2);

}
string &jvalue::load(string json) {
	return _json = json;
}
string jvalue::to_string() {
	string result;
	result = replace(_json, '"', "\\\"");
	result = replace(_json, '\t', "\\t");
	result = replace(_json, '\n', "\\n");
	result = replace(_json, '\r', "\\r");
	result = replace(_json, '\\', "\\\\");
	return '"' + _json + '"';
}
