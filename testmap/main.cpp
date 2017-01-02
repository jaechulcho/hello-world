/*
 * main.cc
 *
 *  Created on: 2016. 11. 21.
 *      Author: josco
 */


#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

#include <iostream>
#include <vector>
#include <map>
#include <memory>

typedef boost::thread Thread;
typedef boost::mutex Mutex;

using namespace std;

map<int, string> m;
map<int, string>::iterator i;

int testmap(int argc)
{
	m.insert(map<int, string>::value_type(1, "Hello"));
	m.insert(map<int, string>::value_type(2, "World"));
	m.insert(map<int, string>::value_type(3, "Diary"));

	auto entry = m.find(argc);

	if (entry == m.end())
	{
		cout << "찾는 원소가 없습니다." << endl;
		return 1;
	}

	cout << entry->second << endl;

#if not defined(_GLIBCXX_HAS_GTHREAD)
	cout << "glib에는 gthread가 없습니다." << endl;
#endif

	return 0;
}

// static_pointer_cast example

struct A {
  static const char* static_type;
  const char* dynamic_type;
  A() { dynamic_type = static_type; }
};
struct B: A {
  static const char* static_type;
  B() { dynamic_type = static_type; }
};

const char* A::static_type = "class A";
const char* B::static_type = "class B";

int testdynamic_cast(int argc) {
  shared_ptr<A> foo;
  shared_ptr<B> bar;

  bar = make_shared<B>();

  foo = dynamic_pointer_cast<A>(bar);

  cout << "foo's static  type: " << foo->static_type << '\n';
  cout << "foo's dynamic type: " << foo->dynamic_type << '\n';
  cout << "bar's static  type: " << bar->static_type << '\n';
  cout << "bar's dynamic type: " << bar->dynamic_type << '\n';

  return 0;
}

struct TestFunc {
	int (*func)(int argc);
	int arg;
};

class C
{
protected:
	int _val;
public:
	C(int a = 0): _val(a) {}
	int GetVal(void) { return _val; }
};

int testclass(int argc) {
	C _(5);

	cout << _.GetVal() << endl;

	return 0;
}

class Box {
public:
	int num_things;
	Mutex m;
	explicit Box(int num) : num_things(num) {}
};

void transfer(Box &from, Box &to, int num)
{
	// don't actually take the locks yet
	boost::unique_lock<Mutex> lock1(from.m, boost::defer_lock);
	boost::unique_lock<Mutex> lock2(to.m, boost::defer_lock);

	// lock both unique_locks without deadlock
	boost::lock(lock1, lock2);

	from.num_things -= num;
	to.num_things += num;

	cout << from.num_things << "," << to.num_things << endl;
}


int testlock(int argc) {
	Box acc1(100);
	Box acc2(50);

	Thread t1(transfer, ref(acc1), ref(acc2), 10);
	Thread t2(transfer, ref(acc2), ref(acc1), 5);

	t1.join();
	t2.join();

	return 0;
}

int main(void)
{
	vector<TestFunc> tf;
	C _(5);


	tf.push_back({testmap, 2});
	tf.push_back({testdynamic_cast, 0});

	tf.push_back({testclass, 0});
	tf.push_back({testlock, 0});


	for (auto i:tf)
	{
		cout << "=====================" << endl;
		i.func(i.arg);
	}
}



