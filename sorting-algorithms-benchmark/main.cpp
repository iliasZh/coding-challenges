#include <iostream>
#include <algorithm>

#include "MergeSort.h"
#include "InsertionSort.h"
#include "QuickSort.h"
#include "SortingBenchmark.h"


void PrintArray(const int* arr, size_t size)
{
	for (size_t i = 0; i < size - 1; ++i)
	{
		std::cout << arr[i] << ", ";
	}
	std::cout << arr[size - 1];
}

class Comparator
{
public:
	bool operator()(int a, int b) const { return a < b; }
};

void stdsort(int* begin, int* end, const Comparator& cmp)
{
	std::sort(begin, end, cmp);
}

int main()
{
	std::cout << "MODE: ";
#ifdef NDEBUG
	std::cout << "RELEASE\n\n";
#else
	std::cout << "DEBUG\n\n";
#endif
	Comparator cmp;
	//Benchmark<int, Comparator> bench{};
	LargeTestArrays lta;
	//
	//bench.EnableLogging();
	//
	//bench.AddSortingMethod("INSERTION SORT", &insertionsort<int, Comparator>);
	//bench.AddSortingMethod("MERGESORT", &mergesort<int, Comparator>);
	//bench.AddSortingMethod("QUICKSORT", &quicksort<int, Comparator>);
	//
	//bench.Run(lta.MillionElements(), lta.MillionElements() + 1000000, cmp);
	mergesort((int*)lta.TenThousandElements(), (int*)lta.TenThousandElements() + 10000, cmp);

	return 0;
}