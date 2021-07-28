#pragma once

#include <cassert>
#include "InsertionSort.h"

#define QUICK_SWITCH_TO_INSERTION
#define QUICK_SWITCH_THRESHOLD 50	// experimentally determined value

template <class T, class Comparator>
T* get_pivot(T* begin, T* end, const Comparator& cmp)
{
	// picks a pivot for partitioning 
	// returns a median of first, middle and last element

	T* first = begin;
	T* middle = begin + (end - begin) / 2;
	T* last = end - 1;
	T* tmp = nullptr;

	if (!cmp(*first, *middle))
	{
		tmp = first;
		first = middle;
		middle = tmp;
	}

	if (!cmp(*middle, *last))
	{
		tmp = middle;
		middle = last;
		last = tmp;
	}

	if (!cmp(*first, *middle))
	{
		tmp = first;
		first = middle;
		middle = tmp;
	}

	return middle;
}

template <class T, class Comparator>
T* partition_border(T* begin, T* end, const Comparator& cmp)
{
	// splits an array that is LARGER THAN 2 ELEMENTS (CRUCIAL) into two NON-EMPTY parts:
	// first part is [begin, return ptr), second is [return ptr, end) so that
	// for all elements in the first part  cmp(elem, pivot) == true 
	// for all elements in the second part cmp(elem, pivot) == false

	// suppose cmp = [](auto a, auto b) { return a < b; }
	// at the end things look like that:

	// 				array
	// ---------------------------------
	// |   < pivot    |    >= pivot    |
	// ---------------------------------
	//               ^ ^
	//               | |
	//           right left

	// thus function always returns the left ptr

	assert(end - begin > 1);

	T pivot = *get_pivot(begin, end, cmp);

	T* left = begin;
	T* right = end - 1;

	while (right - left > 0)
	{
		while (cmp(*left, pivot))
			++left;

		while (!cmp(*right, pivot))
			--right;

		if (right - left > 0)
		{
			T tmp = *left;
			*left = *right;
			*right = tmp;
		}
	}

	return left;
}

template <class T, class Comparator>
void quicksort(T* begin, T* end, const Comparator& cmp)
{
#ifdef QUICK_SWITCH_TO_INSERTION
	while (end - begin > QUICK_SWITCH_THRESHOLD)
#else
	while (end - begin > 2)
#endif
	{
		// partition the array so that:

		//  				array
		//  ---------------------------------
		//  |   < pivot    |    >= pivot    |
		//  ---------------------------------
		//   ^              ^                ^
		//   |              |                |
		// begin          border            end
		T* border = partition_border(begin, end, cmp);

		// sort smaller part recursively, bigger part - iteratively
		if (border - begin < end - border)
		{
			quicksort(begin, border, cmp);
			begin = border;
		}
		else
		{
			quicksort(border, end, cmp);
			end = border;
		}
	}

#ifdef QUICK_SWITCH_TO_INSERTION
	if (end - begin > 0 && end - begin <= QUICK_SWITCH_THRESHOLD)
	{
		insertionsort(begin, end, cmp);
		return;
	}
#else
	if (end - begin == 1)
		return;
	else if (end - begin == 2)
	{
		// this quicksort implementation cannot correctly sort two element arrays
		// so I do it manually

		T* first = begin;
		T* second = end - 1;
		if (!cmp(*first, *second))
		{
			T tmp = *first;
			*first = *second;
			*second = tmp;
		}
		return;
	}
#endif
	else if (end - begin < 1)
		return;
}