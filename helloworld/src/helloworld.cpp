//============================================================================
// Name        : helloworld.cpp
// Author      : josco
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>
using namespace std;

void td1(void)
{
	cout << "task1" << endl;
}

int main() {
	thread t1(td1);

	sleep(1);

	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	t1.join();

	return 0;
}
