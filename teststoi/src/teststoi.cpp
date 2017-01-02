//============================================================================
// Name        : teststoi.cpp
// Author      : jccho@fine-tec.com
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <thread>
#include <iostream>
#include <string>

using namespace std;

void ThreadFunc(int nVal)
{
	cout << nVal << endl;
}

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	int index = stoi("1000");

	cout << index << endl;

	thread th1(ThreadFunc, 100);

	th1.join();


	return 0;
}
