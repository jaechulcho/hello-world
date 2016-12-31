//============================================================================
// Name        : testthread.cpp
// Author      : josco92
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <chrono>
#include "prime.h"
using namespace std;


void single(int M)
{
	chrono::system_clock::time_point start = chrono::system_clock::now();
	int result = count_prime(2, M);
	chrono::duration<double> sec = chrono::system_clock::now() - start;

	cout << result << ' ' << sec.count() << "[sec]" << endl;
}

int main() {
	const int M = 100000;
	single(M);
	return 0;
}
