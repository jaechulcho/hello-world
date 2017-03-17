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

#define TimePoint	chrono::system_clock::time_point
#define ChronoNow	chrono::system_clock::now
#define dfDuration	chrono::duration<double>

void single(int M)
{
<<<<<<< HEAD
	/* saving start time */
	TimePoint start = ChronoNow();
=======
	chrono::system_clock::time_point start = chrono::system_clock::now();
>>>>>>> 92731d30607db09b74a323e994cb27d5b8db95b0
	int result = count_prime(2, M);
	dfDuration sec = ChronoNow() - start;

	cout << result << ' ' << sec.count() << "[sec]" << endl;
}

void multi(int M, int nthr)
{
	vector<thread> thr(nthr);
	vector<int> count(nthr);
	div_range<> rng(2, M, nthr);

	TimePoint start = ChronoNow();
	for (int i=0; i<nthr; ++i)
	{
		thr[i] = thread([&, i](int lo, int hi) {
							count[i] = count_prime(lo, hi);
						},
						rng.lo(i),
						rng.hi(i));
	}

	int result = 0;
	for (int i =0; i < nthr; ++i)
	{
		thr[i].join();
		result += count[i];
	}
	dfDuration sec = ChronoNow() - start;

	cout << result << ' ' << sec.count() << "[sec]: " << nthr << endl;
}

int main() {
	const int M = 100000;
	for (int i = 1; i < 10; ++i) multi(M, i);
	cout << "== END ==" << endl;
	return 0;
}
