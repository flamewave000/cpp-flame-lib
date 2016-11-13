#include <iostream>
#include <object.h>
#include <event.h>

using namespace std;


class myObj : ROOT_BASE {
public:
	myObj() {
		DERIVED(myObj);
	}
	void func(int a) {
		cout << a << endl;
	}
};

PROTOCOL(proto):
	virtual void AB(int a, int b) = 0;
PROTOEND

class someObject : ROOT_BASE {
public:
	someObject() {
		DERIVED(someObject);
	}
};

void test(int a) {
	cout << a << endl;
}

int main(int argc, const char *argv[]) {
	myObj mo;
	method_delegate<myObj, int> mdel(&mo, &myObj::func);
	mdel.invoke(42);

	static_delegate<int> sdel(test);
	sdel.invoke(24);


	cout << "sizeof std::object          " << sizeof(std::object) << endl;
	cout << "sizeof std::type            " << sizeof(std::type) << endl;
	cout << "sizeof std::type::info      " << sizeof(std::type::info) << endl;
	cout << "sizeof std::type::info::uid " << sizeof(std::type::info::uid) << endl;

	someObject *so = new someObject();

	system("PAUSE");
	return 0;
}