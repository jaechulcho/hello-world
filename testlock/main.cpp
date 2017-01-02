/*
 * main.cpp
 *
 *  Created on: 2016. 12. 23.
 *      Author: josco
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>

using namespace std;


mutex m;


void test()
{
	cout << "run thread" << endl;

	unique_lock<mutex> lt(m);
	cout << "lock..." << endl;
	sleep(10);
	lt.unlock();

	sleep(4);
	lt.lock();

	cout << "exit thread" << endl;
}

int main()
{
	auto xx = thread(test);
	sleep(1);

	m.lock();
	cout << "exit main" << endl;
	m.unlock();

	xx.join();

	return 0;
}


