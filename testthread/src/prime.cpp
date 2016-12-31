/*
 * prime.cpp
 *
 *  Created on: 2016. 12. 31.
 *      Author: josco92
 */
#include "prime.h"

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

int count_prime(int lo, int hi)
{
	int result = 0;
	for (int i=lo; i <hi; ++i)
	{
		if (is_prime(i))
		{
			++result;
		}
	}
	return result;
}
