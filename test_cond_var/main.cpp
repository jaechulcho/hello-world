/*
 * main.cpp
 *
 *  Created on: 2016. 12. 23.
 *      Author: josco
 */

#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <future>
#include <atomic>
#include "div_range.h"

using namespace std;

// n is prime
bool is_prime(int n)
{
	for (int i = 2; i < n; ++i)
	{
		if (0 == (n % i))
		{
			return false;
		}
	}
	return true;
}

// How many primes is between lo and hi?
int count_prime(int lo, int hi)
{
	int result = 0;

	for (int i = lo; i < hi; ++i)
	{
		if (is_prime(i))
		{
			++result;
		}
	}

	return result;
}

void single(int M)
{
	chrono::system_clock::time_point start = chrono::system_clock::now();
	int result = count_prime(2, M);
	chrono::duration<double> sec = chrono::system_clock::now() - start;

	cout << result << ' ' << sec.count() << "[sec]" << endl;
}

void multi(int M, int nthr)
{
	vector<thread> thr(nthr);
	vector<int> count(nthr);
	div_range<> rng(2,M, nthr);

	chrono::system_clock::time_point start = chrono::system_clock::now();

	for (int i = 0; i < nthr; ++i)
	{
		thr[i] = thread([&, i](int lo, int hi) {count[i] = count_prime(lo, hi);}, rng.lo(i), rng.hi(i));
	}

	int result = 0;
	for (int i = 0; i < nthr; ++i)
	{
		thr[i].join();
		result += count[i];
	}

	chrono::duration<double> sec = chrono::system_clock::now() - start;

	cout << result << ' ' << sec.count() << "[sec]:" << nthr << endl;
}

void count_prime_function(int lo, int hi, int& result)
{
	result = count_prime(lo, hi);
}

class count_prime_class
{
	int result;
	int& result_ = ref(result);
public:
	count_prime_class() { result = 0;};
	count_prime_class(int& result) : result_(result) {}
	int operator() (int lo, int hi) { result_ = count_prime(lo, hi); return result_; }
};

void multi(int M)
{
	thread thr[3];
	int count[3];
	div_range<> rng(2, M, 3);

	auto count_prime_lambda = [&](int lo, int hi) { count[2] = count_prime(lo, hi);};

	chrono::system_clock::time_point start = chrono::system_clock::now();

	thr[0] = thread(count_prime_function, rng.lo(0), rng.hi(0), ref(count[0]));
	thr[1] = thread(count_prime_class(count[1]), rng.lo(1), rng.hi(1));
	thr[2] = thread(count_prime_lambda, rng.lo(2), rng.hi(2));

	for (thread& t : thr) t.join();
	chrono::duration<double> sec = chrono::system_clock::now() - start;

	cout << count[0]+count[1]+count[2] << ' ' << sec.count() << "[sec]" << endl;
}

void asyncmulti(int M)
{
	future<int> fut[3];
	div_range<> rng(2, M, 3);

	auto count_prime_lambda = [&](int lo, int hi) { return count_prime(lo, hi); };

	chrono::system_clock::time_point start = chrono::system_clock::now();

	fut[0] = async(count_prime, rng.lo(0), rng.hi(0));
	fut[1] = async(count_prime_class(), rng.lo(1), rng.hi(1));
	fut[2] = async(count_prime_lambda, rng.lo(2), rng.hi(2));

	int result = fut[0].get() + fut[1].get() + fut[2].get();
	chrono::duration<double> sec = chrono::system_clock::now() - start;

	cout << result << ' ' << sec.count() << "[sec]" << endl;
}

void lambdamulti(int M, int nthr)
{
	vector<thread> thr(nthr);
	div_range<> rng(2, M, nthr);

	atomic<int> result(0);

	chrono::system_clock::time_point start = chrono::system_clock::now();

	for (int t = 0; t < nthr; ++t)
	{
		thr[t] = thread([&](int lo, int hi) {
			for (int n = lo; n < hi; ++n) {
				if (is_prime(n)) {
					++result;
				}
			}
		},
		rng.lo(t), rng.hi(t));
	}

	for (thread& th : thr) { th.join(); }

	chrono::duration<double> sec = chrono::system_clock::now() - start;

	cout << result << ' ' << sec.count() << "[sec] : " << nthr << endl;
}

class rendezvous
{
public:
	rendezvous(uint32_t count) : _threshold(count), _count(count), _generation(0) {
		if (0 == count) { throw invalid_argument("count cannot be zero."); }
	}

	bool wait()
	{
		unique_lock<mutex> lock(_mutex);
		uint32_t gen = _generation;
		if (0 == --_count)
		{
			_generation++;
			_count = _threshold;
			_condition.notify_all();
			return true;
		}
		_condition.wait(lock, [&]() { return gen != _generation;});

		return false;
	}

private:
	mutex _mutex;
	condition_variable _condition;
	uint32_t _threshold;
	uint32_t _count;
	uint32_t _generation;
};

void condmulti(int M, int nthr)
{
	vector<thread> thr(nthr);
	div_range<> rng(2, M, nthr);
	atomic<int> result(0);
	rendezvous quit(nthr+1);

	chrono::system_clock::time_point start = chrono::system_clock::now();

	for (int t = 0; t < nthr; ++t)
	{
		thr[t] = thread([&](int lo, int hi) {
			for (int n = lo; n < hi; ++n) {
				if (is_prime(n)) ++result;
			}
			quit.wait();
		},
		rng.lo(t), rng.hi(t));
	}
	quit.wait();
	chrono::duration<double> sec = chrono::system_clock::now() - start;

	cout << result << ' ' << sec.count() << "[sec] : " << nthr << endl;

	for (thread& th : thr) {th.join();}
}

int main()
{
	const int M = 100000;
	//for (int i = 1; i < 10; ++i) multi(M, i);
	//multi(M);
	//asyncmulti(M);
	//lambdamulti(M, 8);
	condmulti(M, 20);
}
