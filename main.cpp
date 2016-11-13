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

PROTOCOL(proto) :
	virtual void AB(int a, int b) = 0;
PROTOEND

class someObject : ROOT_BASE {
public:
	event<int> myEvent;
	someObject() : myEvent(nullptr) {
		DERIVED(someObject);
	}
};

template<class i>
class test_template : ROOT_BASE {
private:
	i _var;
public:
	test_template() {
		DERIVED(test_template<i>);
	}
	virtual const char* to_string() {
		return "hello world";
	}
};

void test(int a) {
	cout << a << endl;
}

int main(int argc, const char *argv[]) {
	myObj mo;
	someObject so;
	so.myEvent += new method_delegate<myObj, int>(&mo, &myObj::func);
	so.myEvent += new static_delegate<int>(&test);
	if (so.myEvent != nullptr)
		so.myEvent.invoke(1);// will print twice
	so.myEvent -= new static_delegate<int>(&test);
	if (so.myEvent != nullptr)
		so.myEvent.invoke(2);// will print once
	so.myEvent -= new method_delegate<myObj, int>(&mo, &myObj::func);
	if (so.myEvent != nullptr)// will fail as there are no more
		so.myEvent.invoke(3);// will not be printed

	test_template<int> test;
	if (test.instanceof(class_info<object>())) {
		cout << test.to_string() << endl;
	}



	cout << "sizeof std::object          " << sizeof(std::object) << endl;
	cout << "sizeof std::type            " << sizeof(std::type) << endl;
	cout << "sizeof std::type::info      " << sizeof(std::type::info) << endl;
	cout << "sizeof std::type::info::uid " << sizeof(std::type::info::uid) << endl;

	system("PAUSE");
	return 0;
}