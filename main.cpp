#include <iostream>
#include "object.h"

using namespace std;
using namespace flame;


class myObj : public object {
public:
	myObj() {
		DERIVED(myObj);
	}
};

int main(int argc, const char *argv[]) {

	system("PAUSE");
	return 0;
}