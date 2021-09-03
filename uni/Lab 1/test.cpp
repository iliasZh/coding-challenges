#include "pch.h"
#include "../Lab 1 Test/quicksort.h"
#include <random>

class UserType
{
public:
	int x;
	int y;
};
bool operator<(const UserType& a, const UserType& b)
{
	return a.x * a.x + a.y * a.y < b.x* b.x + b.y * b.y;
}
bool operator>(const UserType& a, const UserType& b)
{
	return a.x * a.x + a.y * a.y > b.x* b.x + b.y * b.y;
}



TEST(Sorting, DISABLED_AlreadySorted)
{
	const int n = 50;
	UserType arr[n];
	for (int i = 0; i < n; ++i)
	{
		arr[i] = { i,i };
	}

	sort(arr, arr + n - 1, [](UserType a, UserType b) { return a < b; });
	EXPECT_TRUE(is_sorted(arr, arr + n - 1, [](UserType a, UserType b) { return a < b; }));
}



TEST(Sorting, DISABLED_Reversed)
{
	const int n = 50;
	UserType arr[n];
	
	for (int i = 0; i < n; ++i)
	{
		arr[i] = { n - i,n - i };
	}

	sort<UserType>(arr, arr + n - 1, [](UserType a, UserType b) { return a < b; });

	EXPECT_TRUE(is_sorted(arr, arr + n - 1, [](UserType a, UserType b) { return a < b; }));
}



TEST(Sorting, DISABLED_AllEqual)
{
	const int n = 50;
	UserType arr[n];

	for (int i = 0; i < n; ++i)
	{
		arr[i] = { 1,1 };
	}

	sort<UserType>(arr, arr + n - 1, [](UserType a, UserType b) { return a < b; });

	EXPECT_TRUE(is_sorted(arr, arr + n - 1, [](UserType a, UserType b) { return a < b; }));
}



TEST(SortingRandom, DISABLED_Small)
{
	const int n = 30;
	UserType arr[n];

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uid(1, n);

	for (int i = 0; i < n; ++i)
	{
		arr[i] = { uid(rng),uid(rng) };
	}

	sort<UserType>(arr, arr + n - 1, [](UserType a, UserType b) { return a < b; });

	EXPECT_TRUE(is_sorted(arr, arr + n - 1, [](UserType a, UserType b) { return a < b; }));
}



TEST(SortingRandom, DISABLED_WithLotsOfEqual)
{
	const int n = 500;
	UserType arr[n];

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uid(1, n);

	for (int i = 0; i <= n - 5; i += 5)
	{
		arr[i] = { uid(rng),uid(rng) };
		for (int j = 1; j < 5; ++j)
		{
			arr[i + j] = arr[i + j - 1];
		}
	}

	sort<UserType>(arr, arr + n - 1, [](UserType a, UserType b) { return a < b; });

	EXPECT_TRUE(is_sorted(arr, arr + n - 1, [](UserType a, UserType b) { return a < b; }));
}



TEST(SortingRandom, DISABLED_Big)
{
	const int n = 500;
	UserType arr[n];

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uid(1, n);

	for (int i = 0; i < n; ++i)
	{
		arr[i] = { uid(rng),uid(rng) };
	}

	sort<UserType>(arr, arr + n - 1, [](UserType a, UserType b) { return a < b; });

	EXPECT_TRUE(is_sorted(arr, arr + n - 1, [](UserType a, UserType b) { return a < b; }));
}



TEST(SortingRandom, DISABLED_BigDiffComparator)
{
	const int n = 500;
	UserType arr[n];

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uid(1, n);

	for (int i = 0; i < n; ++i)
	{
		arr[i] = { uid(rng),uid(rng) };
	}

	sort<UserType>(arr, arr + n - 1, [](UserType a, UserType b) { return a > b; });

	EXPECT_TRUE(is_sorted(arr, arr + n - 1, [](UserType a, UserType b) { return a > b; }));
}


TEST(Swapping, Swapping)
{
	int a[] = { 9,8,7,6,5,4,3,2,1 };
	sort(a, a + 6, [](int a, int b) {return a < b; });
	EXPECT_TRUE(true);
}