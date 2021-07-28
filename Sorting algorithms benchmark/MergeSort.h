#pragma once

#include <cassert>
#include "InsertionSort.h"

#define MERGE_SWITCH_TO_INSERTION
#define MERGE_SWITCH_THRESHOLD 120 // experimentally determined value

template <class T, class Comparator>
void merge(T* begin, T* split, T* end, T* buffer, const Comparator& cmp)
{
	size_t lsize = split - begin;
	size_t rsize = end - split;

	T* l = buffer;
	T* r = buffer + lsize;

	// copying
	for (int i = 0; i < (int)lsize; ++i)
	{
		l[i] = begin[i];
	}
	for (int i = 0; i < (int)rsize; ++i)
	{
		r[i] = split[i];
	}

	T* lp = l;
	T* lEnd = l + lsize;
	T* rp = r;
	T* rEnd = r + rsize;
	T* ptr = begin; // lp/rp - scanning ptrs, ptr - overwrite ptr
	for (; lp != lEnd && rp != rEnd; ++ptr)
	{
		if (cmp(*lp, *rp))
		{
			*ptr = *lp; ++lp;
		}
		else
		{
			*ptr = *rp; ++rp;
		}
	}

	if (lp == lEnd)
	{
		for (; rp != rEnd; ++ptr, ++rp)
		{
			*ptr = *rp;
		}
	}
	else if (rp == rEnd)
	{
		for (; lp != lEnd; ++ptr, ++lp)
		{
			*ptr = *lp;
		}
	}

	assert(lp == lEnd);
	assert(rp == rEnd);
	assert(ptr == end);
}

template <class T, class Comparator>
void mergesort_implementation(T* begin, T* end, T* buffer, const Comparator& cmp)
{
	int size = end - begin;
	assert(size > 0);

#ifdef MERGE_SWITCH_TO_INSERTION
	if (size <= MERGE_SWITCH_THRESHOLD)
	{
		insertionsort(begin, end, cmp);
		return;
	}
#else
	if (size == 1)
	{
		return;
	}
#endif
	else
	{
		T* split = begin + size / 2;

		mergesort_implementation(begin, split, buffer, cmp);
		mergesort_implementation(split, end, buffer, cmp);
		merge(begin, split, end, buffer, cmp);
	}
}

template <class T, class Comparator>
void mergesort(T* begin, T* end, const Comparator& cmp)
{
	assert(end - begin > 0);
	T* buffer = new T[end - begin];
	mergesort_implementation(begin, end, buffer, cmp);
	delete[] buffer;
}

