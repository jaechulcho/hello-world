/*
 * divrange.h
 *
 *  Created on: 2016. 12. 31.
 *      Author: josco92
 */

#ifndef DIVRANGE_H_
#define DIVRANGE_H_

template<typename T=int>
class div_range {
private:
	T _lo;
	T _hi;
	T _stride;
	int _n;
public:
	div_range(T lo, T hi, int n) : _lo(lo), _hi(hi), _n(n) {
		_stride  = (hi - lo)/n;
	};
	T lo(int n) const { return _lo + _stride * n; };
	T hi(int n) const { return (++n < _n) ? _lo + _stride * n : _hi; };
};

#endif /* DIVRANGE_H_ */
