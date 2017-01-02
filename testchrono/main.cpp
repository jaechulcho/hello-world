/*
 * main.cpp
 *
 *  Created on: 2016. 12. 23.
 *      Author: josco
 */
#include <chrono>
#include <iostream>
#include <cmath>

using namespace std;

void Test()
{
	for (long i = 0; i < 320*240*4*3; ++i)
	{
		sqrt(123.456L);
	}
}

int main()
{
	chrono::system_clock::time_point start = chrono::system_clock::now();

	Test();

	chrono::duration<double> sec = chrono::system_clock::now() - start;

	chrono::milliseconds mill = chrono::duration_cast<chrono::milliseconds>(sec);

	cout << "Test() function time ellapses(sec): " << sec.count() << " sec" << endl;

	cout << "Test() function time ellapses(ms) : " << mill.count() << " ms" << endl;

	return 0;
}

