#include "quicksort.h"
#include <random>

int main()
{
	const int arraySize = 100000;

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uid(1, 10*arraySize);

	int* a = new int[arraySize];

	for (int i = 0; i < arraySize; ++i)
	{
		*(a + i) = uid(rng);
	}

	sort(a, a + arraySize - 1, [](int a, int b) {return a < b; });

	//assert(is_sorted(a, a + arraySize - 1, [](int a, int b) {return a < b; }));

	for (int i = 0; i < 100; ++i)
	{
		std::cout << *(a + i) << std::endl;
	}

	delete[] a;
}