#include "dynamic_array.h"
#include "unrolled_list.h"

#ifndef LIST_REPORT
#ifndef ARRAY_REPORT
#include <iostream>
#endif
#endif

class Type
{
private:
	int* p = nullptr;
	int size_ = 0;
public:
	Type()						// default
		: size_{ 8 }
	{
		p = new int[8];
		for (int i = 0; i < 8; ++i)
		{
			*(p + i) = i;
		}
	}

	Type(int size)				// set size
		: size_{ size + 1 }
	{
		assert(size_ > 0);

		p = new int[size_];
		for (int i = 0; i < size_; ++i)
		{
			*(p + i) = i;
		}
	}

	Type(const Type& test)		// copy
	{
		size_ = test.size_;
		p = new int[size_];

		for (int i = 0; i < size_; ++i)
		{
			*(p + i) = *(test.p + i);
		}
	}

	~Type()
	{
		delete[] p;
	}

	int size() const
	{
		return size_;
	}

	int get(int index) const
	{
		return *(p + index);
	}
};

std::ostream& operator<<(std::ostream& os, const Type& test)
{
	os << "{";
	for (int i = 0; i < test.size() - 1; ++i)
	{
		os << test.get(i) << ", ";
	}
	os << test.get(test.size() - 1) << "}";
	return os;
}

bool operator==(const Type& a, const Type& b)
{
	bool ret = true;
	if (a.size() != b.size())
	{
		return false;
	}
	else
	{
		for (int i = 0; i < a.size(); ++i)
		{
			ret = ret && (a.get(i) == b.get(i));
		}
	}
	return ret;
}

int main()
{
	List<Type> l;

	/*
	for (int i = 0; i < 4; ++i)
	{
		l.insertTail(Type(i));
	}
	*/
	l.insertHead(Type(4));

	auto it = l.iterator();
	it.remove();

	std::cout << l.size();

	/*
	for (int i = 0; i < 4; ++i)
	{
		it.next();
	}
	*/

	return 0;
}