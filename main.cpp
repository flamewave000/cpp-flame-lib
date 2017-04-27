#include <iostream>
#include <string>
#include <object.h>
#include <event.h>
#include "json\json.h"
#include "safevar.h"
#define LINQ_USE_MACROS 1
#include "linq.h"

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
		so.myEvent(2);// will print once
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


	json::jvalue value;
	value.parse("[\"[hello, \", \"world]\\\\\", 42,\n true, null]");
	json::jarray array = value.as_array();
	cout << array[0].as_string() << '\n';
	cout << array[1].as_string() << '\n';
	cout << array[2].as_number().to_int32() << '\n';
	cout << (array[3].as_number().to_bool() ? "true" : "false") << '\n';

	value.parse("{\"hello\": \"world\", \"I am\" : 42, \"wassup?\": [ 1, 2, 3, 4, 5 ], \"obj!\" : { \"yo\" : true} }");
	json::jobject obj = value.as_object();
	cout << obj["hello"].as_string() << '\n';
	cout << obj["I am"].as_number().to_int32() << '\n';
	cout << obj["wassup?"].as_array()[3].as_number().to_int32() << '\n';
	cout << (obj["obj!"].as_object()["yo"].as_number().to_bool() ? "true" : "false") << '\n';


	safe_var<myObj> svi;
	auto ref = svi.lock();
	//var_ref<int> vri;// = *ref;
	(*ref)->func(42);
	(**ref).func(42);


	typedef struct{ int i; string desc; } description;
	int nums[10] = { 1,2,3,4,5,6,7,8,9,10 };
	// Select the even numbers from the list
	auto evens = FROM(nums, 10) WHERE { return item % 2 == 0; } ORDERBY(DESCENDING) END;
	// Select the odd numbers from the list
	auto odds = linq::from(nums, 10).where([](auto item) { return item % 2 == 1; }).orderby(linq::descending);
	// Change the data to a new type
	auto even_descs = FROM(evens) SELECT(description) { return { item, "even" }; } END;
	auto odd_descs = FROM(odds) SELECT(description) { return { item, "odd" }; } END;


	typedef struct { int id, proj_id; string name; } Employee;
	typedef struct { int id; string name; } Project;
	typedef struct { struct { int id; string name; } employee; string proj_name; } EmployeeProject;
	Employee employees[3] = { { 0, 0, "Joe" }, { 1, 1, "Jane" }, { 2, 1, "Alex" } };
	vector<Project> projects = { { 0, "Financial Inc." }, { 1, "Contracts R Us" } };
	// First method of joining using the MERGE_JOIN
	auto empProjs = FROM(employees, 3)
		MERGE_JOIN(Project, EmployeeProject) projects
		INTO(EmployeeProject) { return { { left.id, left.name }, right.name }; } // required
		ON { return left.proj_id == right.id; } // left/right condition
		END;
	// Second method of joining using the JOIN to get an array of pairs and then using SELECT to construct the new array
	empProjs = FROM(employees, 3)
		JOIN(Project) projects
		ON { return left.proj_id == right.id; } // left/right condition
		SELECT(EmployeeProject) { return { { item.left.id, item.left.name }, item.right.name }; }
		END;
	// First option as before but without macros
	empProjs = linq::from(employees, 3)
		.join<Project, EmployeeProject>(projects,
			[](auto left, auto right) -> EmployeeProject { return { { left.id, left.name }, right.name }; },
			[](auto left, auto right) -> bool { return left.proj_id == right.id; }); // left/right condition
	// Second option as before but without macros
	empProjs = linq::from(employees, 3)
		.join<Project>(projects, [](auto left, auto right) -> bool { return left.proj_id == right.id; }) // left/right condition
		.select<EmployeeProject>([](auto item) ->EmployeeProject { return { { item.left.id, item.left.name }, item.right.name }; });

	system("PAUSE");
	return 0;
}