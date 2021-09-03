#pragma once

#include <cassert>

template <class T, class Comparator>
void insertionsort(T* begin, T* end, const Comparator& cmp)
{
	int size = end - begin;
	assert(size > 0);

	for (int i = 1; i < size; ++i)
	{
		int idx = i - 1;
		T tmp = begin[i];
		while (!cmp(begin[idx], tmp))
		{
			begin[idx + 1] = begin[idx];

			if (idx-- == 0) break;
		}
		begin[idx + 1] = tmp;
	}
}