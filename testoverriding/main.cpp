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
	virtual void func() { cout << "�θ� Ŭ������ func �Լ� ȣ��!" << endl; }
};

class Child : public Parent {
public:
	virtual void func() { cout << "�ڽ� Ŭ������ func �Լ� ȣ��!" << endl; }
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
