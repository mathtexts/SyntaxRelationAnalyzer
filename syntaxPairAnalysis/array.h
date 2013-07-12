// S______T_____A___________R       A_________G_______________E__S
// Syntax Texts Analysis of Russian Automated Generator of rulEs Systems

#ifndef STAR_AGES_ARRAY_H
#define STAR_AGES_ARRAY_H

#include <Windows.h>
#include "except.h"

template < class T > class Array;

char * Arr2Str(const Array<char> & arr);

char * Arr2Str(const Array<wchar_t> & arr);

template < class T > class Array {
	static const int ARMI = 10;
	T ** data;
	int l, r, OTT;
public:
	Array(const Array <T> & a);
	Array(int o = 1, int range = ARMI);
	~Array();
	const int e() const;
	void Terminate(int o = 0);
	Array & operator += (T * op);
	T * operator -- ();
	int OK(int i) const;
	T *& operator + (int i);
	T * operator - (int i);
	void Compact();
	T *& operator [] (int i);
	T * const operator [] (int i) const;
	void Reset();
	static char * Arr2Str(const Array<char> & arr);
	static char * Arr2Str(const Array<wchar_t> & arr);
};

template < class T > Array<T>::Array(const Array <T> & a)
	: l(a.l), r(a.r), OTT(a.OTT)
{
	int i;
	data = new T * [r];
	if (OTT) for (i = 0; i < l; ++i) data[i] = new T(*a.data[i]);
	else for (i = 0; i < l; ++i) data[i] = a.data[i];
}

template < class T > Array<T>::Array(int o, int range)
	: l(0), r(range), OTT(o)
{
	int i;
	data = new T * [r];
	for (i = 0; i < r; ++i) data[i] = 0;
}

template < class T > Array<T>::~Array() {
	int i;
	if (OTT) for (i = 0; i < l; ++i) delete data[i];
	delete data;
}

template < class T > const int Array<T>::e() const { return l - 1; }

template < class T > void Array<T>::Terminate(int o) { OTT = o; }

template < class T > Array<T> & Array<T>::operator += (T * op) {
	T ** tmp;
	int i;
	if (l == r) {
		r += ARMI;
		tmp = new T * [r];
		for (i = 0; i < l; ++i) tmp[i] = data[i];
		delete [] data;
		data = tmp;
		for (i = l; i < r; ++i) data[i] = 0;
	}
	data[l++] = op;
	return *this;
}

template < class T > T * Array<T>::operator -- () {
	if (l == 0) throw new Exception(32);
	T * tmp = data[--l];
	data[l] = 0;
	return tmp;
}

template < class T > int Array<T>::OK(int i) const {
	return (i >= 0) && (i < l) && (data[i] != 0);
}

template < class T > T *& Array<T>::operator + (int i) {
	T ** tmp;
	int j;
	if (i >= r) {
		r = i + ARMI;
		tmp = new T * [r];
		for (j = 0; j < l; ++j) tmp[j] = data[j];
		delete [] data;
		data = tmp;
		for (j = l; j < r; ++j) data[j] = 0;
	}
	if (i >= l) l = i + 1;
	return data[i];
}

template < class T > T * Array<T>::operator - (int i) {
	if ((i < 0) || (i >= l) || !data[i]) throw new WithShort(9, i);
	T * tmp = data[i];
	data[i] = 0;
	return tmp;
}

template < class T > void Array<T>::Compact() {
	int i, j = 0;
	for (i = 0; i < l; ++i) if (!data[i]) ++j;
		else data[i - j] = data[i];
	l -= j;
}

template < class T > T *& Array<T>::operator [] (int i) {
	if ((i < 0) || (i >= l) || !data[i]) throw new WithShort(9, i);
	return data[i];
}

template < class T > T * const Array<T>::operator [] (int i) const {
	if ((i < 0) || (i >= l) || !data[i]) throw new WithShort(9, i);
	return data[i];
}

template < class T > void Array<T>::Reset() {
	int i;
	if (OTT) for (i = 0; i < l; ++i) delete data[i];
	delete data;
	l = 0;
	r = ARMI;
	data = new T * [r];
	for (i = 0; i < r; ++i) data[i] = 0;
}

template < class T > char * Array<T>::Arr2Str(const Array<char> & arr) {
	int i;
	char * r = new char[arr.l];
	for (i = 0; i < arr.l; ++i) r[i] = *arr[i];
	return r;
}

template < class T > char * Array<T>::Arr2Str(const Array<wchar_t> & arr) {
	int i;
	char * s = new char[arr.l];
	wchar_t * r = new wchar_t[arr.l];
	for (i = 0; i < arr.l; ++i) r[i] = *arr[i];
	if (!WideCharToMultiByte(1251, 0, r, arr.l, s, arr.l, 0, 0))
		throw new WithStr(65, (char *)r, 1);
	delete [] r;
	return s;
}

#endif
