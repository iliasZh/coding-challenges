#pragma once

#include <stdlib.h>
#include <utility>

class TestArraysBase
{
protected:
	void Fill(int* arr, size_t size)
	{
		for (int i = 0; i < (int)size; ++i)
		{
			arr[i] = i;
		}
	}
	std::pair<size_t, size_t> GenerateIndices(size_t arr_size)
	{
		size_t first = rand() % arr_size;
		size_t second;
		do
		{
			second = rand() % arr_size;
		} while (second == first);
		return std::make_pair(first, second);
	}
	void Shuffle(int* arr, size_t size)
	{
		for (size_t i = 0; i < size / 2; ++i)
		{
			auto pair = GenerateIndices(size);
			std::swap(arr[pair.first], arr[pair.second]);
		}
	}
};

class SmallTestArrays : public TestArraysBase
{
public:
	SmallTestArrays()
	{
		srand(NULL);

		Fill(tenElems, 10);
		Shuffle(tenElems, 10);
		Fill(hundredElems, 100);
		Shuffle(hundredElems, 100);
		Fill(thousandElems, 1000);
		Shuffle(thousandElems, 1000);
	}
	const int* TenElements()
	{
		return tenElems;
	}
	const int* HundredElements()
	{
		return hundredElems;
	}
	const int* ThousandElements()
	{
		return thousandElems;
	}
private:
	int tenElems[10];
	int hundredElems[100];
	int thousandElems[1000];
};

class LargeTestArrays : public TestArraysBase
{
public:
	LargeTestArrays()
	{
		srand(NULL);
		tenThousandElems		= new int[10000];
		hundredThousandElems	= new int[100000];
		millionElems			= new int[1000000];

		Fill(tenThousandElems,			10000);
		Shuffle(tenThousandElems,		10000);
		Fill(hundredThousandElems,		100000);
		Shuffle(hundredThousandElems,	100000);
		Fill(millionElems,				1000000);
		Shuffle(millionElems,			1000000);
	}
	~LargeTestArrays()
	{
		delete[] tenThousandElems;
		delete[] hundredThousandElems;
		delete[] millionElems;
	}
	const int* TenThousandElements()
	{
		return tenThousandElems;
	}
	const int* HundredThousandElements()
	{
		return hundredThousandElems;
	}
	const int* MillionElements()
	{
		return millionElems;
	}
private:
	int* tenThousandElems;
	int* hundredThousandElems;
	int* millionElems;
};