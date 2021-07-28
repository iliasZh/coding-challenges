#pragma once
#include <iostream>
#include <assert.h>
#include <vector>
//#include <Windows.h>
//#include <profileapi.h>

template<class T, class Compare>
bool is_sorted(T* first, T* last, Compare comp)
{
	assert(last - first >= 0);

	bool ret = true;

	for (int i = 0; i < last - first; ++i)
	{
		ret = ret && !comp(*(first + i + 1), *(first + i));
		if (!ret) return ret;
	}

	return ret;
}

template<class T, class Compare>
void insertion_sort(T* first, T* last, Compare comp)
{
	if (last - first > 0)
	{
		for (int i = 1; i < last - first + 1; ++i)
		{
			assert(is_sorted(first, first + i - 1, comp));

			T temp = *(first + i);
			int j = i;

			for (; j > 0 && comp(temp, *(first + j - 1)); --j)
			{
				*(first + j) = *(first + j - 1);
				assert(is_sorted(first + j, first + i, comp) && !comp(*(first + j), temp));
				assert(j <= i);
			}

			*(first + j) = temp;
		}
	}
}

template<class T, class Compare>
T median(T* a, T* b, T* c, Compare comp)
{
	if (comp(*a, *b))
	{
		if (comp(*b, *c))
			return *b;
		else if (comp(*c, *a))
			return *a;
	}
	else
	{
		if (comp(*a, *c))
			return *a;
		else if (comp(*c, *b))
			return *b;
	}
	return *c;
}



template <class T, class Compare>
int old_partition(T* first, T* last, Compare comp, const T& pivot)
{
	int i = 0;
	int j = last - first;
	int swap_counter = 0;
	while (j >= i)
	{
		if (comp(*(first + i), pivot))
			++i;
		else
		{
			if (comp(pivot, *(first + j)))
				--j;
			else
			{
				std::swap(*(first + i), *(first + j));
				++swap_counter;
				++i; --j;
			}
		}
	}
	std::cout << "swaps: " << swap_counter << std::endl;
	return j;
}

template <class T, class Compare>
int partition(T* first, T* last, Compare comp, const T& pivot)
{
	int i = 0;
	int j = last - first;
	while (j >= i)
	{
		while (comp(*(first + i), pivot))
			++i;

		while (comp(pivot, *(first + j)))
			--j;
		
		std::swap(*(first + i), *(first + j));
		
		++i; --j;
	
	}
	return j;
}

constexpr int INSERTION_THRESHOLD = 32;

template<class T, class Compare>
void sort(T* first, T* last, Compare comp)
{
	if (last - first < INSERTION_THRESHOLD)
	{
		insertion_sort(first, last, comp);
		return;
	}

	while (last - first > 0)
	{
		const T pivot = median<T, Compare>(first, first + (last - first) / 2, last, comp);
		
		int j = partition(first, last, comp, pivot);
		
		if (j >= 0)
		{
			if (j < (last - first) / 2) // first part is smaller
			{
				sort(first, first + j, comp);
				first = first + j + 1;
			}
			else						// second part is smaller
			{
				sort(first + j + 1, last, comp);
				last = first + j;
			}
		}
		else
		{
			last = first;
		}
	}
	return;
}