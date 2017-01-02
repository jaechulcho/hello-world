/*
 * main.cpp
 *
 *  Created on: 2016. 12. 24.
 *      Author: josco
 */

#include <iostream>
#include <memory>

using namespace std;

class Parent {
public:
	virtual void func() { cout << "부모 클래스의 func 함수 호출!" << endl; }
};

class Child : public Parent {
public:
	virtual void func() { cout << "자식 클래스의 func 함수 호출!" << endl; }
};

int main()
{
	shared_ptr<Parent> pP(new Parent);
	Child C;

	pP = &P;

	pP->func();

	pP = &C;

	pP->func();

	return 0;
}
