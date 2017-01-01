//============================================================================
// Name        : testthread.cpp
// Author      : josco92
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include "prime.h"
#include "divrange.h"

using namespace std;


void single(int M)
{
	/* 측정 시작 시간을 저장 한다. */
	chrono::system_clock::time_point start = chrono::system_clock::now();
	int result = count_prime(2, M);
	chrono::duration<double> sec = chrono::system_clock::now() - start;

	cout << result << ' ' << sec.count() << "[sec]" << endl;
}

void multi(int M, int nthr)
{
	vector<thread> thr(nthr);
	vector<int> count(nthr);
	div_range<> rng(2, M, nthr);

	chrono::system_clock::time_point start = chrono::system_clock::now();
	for (int i=0; i<nthr; ++i)
	{
		thr[i] = thread([&, i](int lo, int hi) {count[i] = count_prime(lo, hi);}, rng.lo(i), rng.hi(i));
	}

	int result = 0;
	for (int i =0; i < nthr; ++i)
	{
		thr[i].join();
		result += count[i];
	}
	chrono::duration<double> sec = chrono::system_clock::now() - start;

	cout << result << ' ' << sec.count() << "[sec]: " << nthr << endl;
}

int main() {
	const int M = 100000;
	for (int i = 1; i < 10; ++i) multi(M, i);
	return 0;
}
