#include "pch.h"
#include "../Lab 2/dynamic_array.h"
#include "../Lab 2/unrolled_list.h"

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

const int def_cap = Array<int>::default_capacity;



//------------------------------ARRAY TESTS BEGIN------------------------------//
//------------------------------ARRAY TESTS BEGIN------------------------------//

TEST(Array, DefaultInsertInts)
{
	Array<int> a;

	for (int i = 0; i < 2 * def_cap; ++i)
	{
		a.insert(i);
	}

	for (int i = 0; i < 2 * def_cap; ++i)
	{
		EXPECT_EQ(a[i], i);
	}
}

TEST(Array, DefaultInsertClasses)
{
	Array<Type> a;
	
	for (int i = 0; i < 2 * def_cap; ++i)
	{
		a.insert(Type(i));
	}

	for (int i = 0; i < 2 * def_cap; ++i)
	{
		EXPECT_EQ(a[i], Type(i));
	}
}

TEST(Array, NonDefaultConstructor)
{
	Array<Type> a(5);

	for (int i = 0; i < 10; ++i)
	{
		a.insert(Type(i));
	}

	for (int i = 0; i < 10; ++i)
	{
		EXPECT_EQ(a[i], Type(i));
	}
}


TEST(Array, InsertAtIndex)
{
	Array<Type> a;

	for (int i = 0; i < def_cap; ++i)
	{
		a.insert(0, Type(i));
	}

	a.insert(def_cap, Type(def_cap));

	// 7 6 5 4 3 2 1 8

	for (int i = 0; i < def_cap; ++i)
	{
		EXPECT_EQ(a[i], Type(def_cap - i - 1));
	}
	EXPECT_EQ(a[def_cap], Type(def_cap));
}

TEST(Array, RemoveAtIndex)
{
	Array<Type> a;

	for (int i = 0; i < def_cap + 1; ++i)
	{
		a.insert(Type(i));
	}

	// 0 1 2 3 4 5 6 7 8

	a.remove(def_cap);
	a.remove(0);

	// 1 2 3 4 5 6 7

	for (int i = 0; i < def_cap - 1; ++i)
	{
		EXPECT_EQ(a[i], Type(i + 1));
	}
}

TEST(Array, Size)
{
	Array<Type> a;

	for (int i = 0; i < 2 * def_cap; ++i)
	{
		a.insert(Type(i));
	}

	EXPECT_EQ(a.size(), 2 * def_cap);
}

TEST(ArrayIterator, Get)
{
	Array<Type> a;
	a.insert(Type(1));

	auto it = a.iterator();

	EXPECT_EQ(it.get(), Type(1));
}

TEST(ArrayIterator, Set)
{
	Array<Type> a;
	a.insert(Type(1));

	auto it = a.iterator();

	it.set(Type(2));

	EXPECT_EQ(it.get(), Type(2));
}

TEST(ArrayIterator, Next)
{
	Array<Type> a;
	for (int i = 0; i < 2 * def_cap; ++i)
	{
		a.insert(Type(i));
	}
	

	auto it = a.iterator();

	for (int i = 0; i < 2 * def_cap; ++i)
	{
		EXPECT_EQ(it.get(), Type(i));
		it.next();
	}
}

TEST(ArrayIterator, NextLoop)
{
	Array<Type> a;
	for (int i = 0; i < 2 * def_cap; ++i)
	{
		a.insert(Type(i));
	}

	int i = 0;
	for (auto it = a.iterator(); it.hasNext(); it.next())
	{
		EXPECT_EQ(it.get(), Type(i));
		++i;
	}
}

TEST(ArrayIterator, PrevLoop)
{
	Array<Type> a;
	for (int i = 0; i < 2 * def_cap; ++i)
	{
		a.insert(Type(i));
	}

	auto it = a.iterator();
	while (it.hasNext())				// go to end
	{
		it.next();
	}

	it.prev();							// go back to last element

	for (int i = 2 * def_cap - 1; it.hasPrev(); --i, it.prev())
	{
		EXPECT_EQ(it.get(), Type(i));
	}
}

TEST(ArrayIterator, Remove)
{
	Array<Type> a;
	for (int i = 0; i < 2 * def_cap; ++i)
	{
		a.insert(Type(i));
	}

	auto it = a.iterator();

	it.remove();	// remove 0
	it.remove();	// remove 1
	it.remove();	// remove 2

	for (int i = 3; i < 2 * def_cap; ++i)
	{
		EXPECT_EQ(it.get(), Type(i));
		it.next();
	}
}

TEST(ArrayIterator, RemoveEnd)
{
	Array<Type> a;
	for (int i = 0; i < 2 * def_cap; ++i)
	{
		a.insert(Type(i));
	}

	auto it = a.iterator();

	for (int i = 1; i < 2 * def_cap; ++i)
	{
		it.next();
	}

	EXPECT_EQ(it.get(), Type(2 * def_cap - 1));

	it.remove();						// now is invalid, points to deleted item
	it.prev();							// go to prev so that it is valid again

	EXPECT_EQ(it.get(), Type(2 * def_cap - 2));
}

TEST(ArrayIterator, ToIndex)
{
	Array<Type> a;
	for (int i = 0; i < 2 * def_cap; ++i)
	{
		a.insert(Type(i));
	}

	auto it = a.iterator();

	it.toIndex(7);
	EXPECT_EQ(it.get(), Type(7));

	it.toIndex(9);
	EXPECT_EQ(it.get(), Type(9));

	it.toIndex(0);
	EXPECT_EQ(it.get(), Type(0));

	it.toIndex(13);
	EXPECT_EQ(it.get(), Type(13));
}

TEST(ArrayIterator, Insert)
{
	Array<Type> a;
	for (int i = 0; i < 2 * def_cap; ++i)
	{
		if(i != 4) a.insert(Type(i));
	}

	auto it = a.iterator();

	it.toIndex(4);
	it.insert(Type(4));

	for (int i = 0; i < 2 * def_cap; ++i)
	{
		EXPECT_EQ(a[i], Type(i));
	}
}

//------------------------------ARRAY TESTS END------------------------------//
//------------------------------ARRAY TESTS END------------------------------//





//------------------------------LIST TESTS BEGIN------------------------------//
//------------------------------LIST TESTS BEGIN------------------------------//

const int chunk_cap = List<int>::chunk_capacity;

TEST(List, InsertHead)
{
	List<Type> l;
	for (int i = 0; i < chunk_cap + 1; ++i)
	{
		l.insertHead(Type(i));
		EXPECT_EQ(l.head(), Type(i));
	}

	EXPECT_EQ(l.size(), chunk_cap + 1);
}

TEST(List, InsertTail)
{
	List<Type> l;
	for (int i = 0; i < chunk_cap + 1; ++i)
	{
		l.insertTail(Type(i));
		EXPECT_EQ(l.tail(), Type(i));
	}

	EXPECT_EQ(l.size(), chunk_cap + 1);
}

TEST(List, RemoveHead)
{
	List<Type> l;
	for (int i = 0; i < chunk_cap + 1; ++i)
	{
		l.insertHead(Type(i));
	}

	l.removeHead();
	EXPECT_EQ(l.head(), Type(chunk_cap - 1));

	l.removeHead();
	EXPECT_EQ(l.head(), Type(chunk_cap - 2));

	EXPECT_EQ(l.size(), chunk_cap - 1);
}

TEST(List, RemoveTail)
{
	List<Type> l;
	for (int i = 0; i < chunk_cap + 1; ++i)
	{
		l.insertTail(Type(i));
	}

	l.removeTail();
	EXPECT_EQ(l.tail(), Type(chunk_cap - 1));

	l.removeTail();
	EXPECT_EQ(l.tail(), Type(chunk_cap - 2));

	EXPECT_EQ(l.size(), chunk_cap - 1);
}

/*
const T& get() const;
void set(const T& value);
void insert(const T& value);
void remove();
void next();
void prev();
bool hasNext() const;
bool hasPrev() const;
*/

TEST(ListIterator, Get)
{
	List<Type> l;
	l.insertHead(Type(0));

	auto it = l.iterator();

	EXPECT_EQ(it.get(), Type(0));
}

TEST(ListIterator, Set)
{
	List<Type> l;
	l.insertHead(Type(0));

	auto it = l.iterator();

	it.set(Type(1));

	EXPECT_EQ(it.get(), Type(1));
}

TEST(ListIterator, Next)
{
	List<Type> l;

	for (int i = 0; i < 2 * chunk_cap; ++i)
	{
		l.insertTail(Type(i));
	}
	
	auto it = l.iterator();
	for (int i = 0; i < 2 * chunk_cap; ++i)
	{
		EXPECT_EQ(it.get(), Type(i));
		it.next();
	}
}

TEST(ListIterator, NextLoop)
{
	List<Type> l;

	for (int i = 0; i < 2 * chunk_cap; ++i)
	{
		l.insertTail(Type(i));
	}

	int i = 0;
	for (auto it = l.iterator(); it.hasNext(); ++i, it.next())
	{
		EXPECT_EQ(it.get(), Type(i));
	}
}

TEST(ListIterator, Prev)
{
	List<Type> l;

	for (int i = 0; i < 2 * chunk_cap; ++i)
	{
		l.insertHead(Type(i));
	}

	auto it = l.iterator();
	while (it.hasNext())
	{
		it.next();
	}

	it.prev();

	for (int i = 0; i < 2 * chunk_cap; ++i, it.prev())
	{
		EXPECT_EQ(it.get(), Type(i));
	}
}

TEST(ListIterator, PrevLoop)
{
	List<Type> l;

	for (int i = 0; i < 2 * chunk_cap; ++i)
	{
		l.insertHead(Type(i));
	}

	auto it = l.iterator();
	while (it.hasNext())
	{
		it.next();
	}

	it.prev();

	for (int i = 0; it.hasPrev(); ++i, it.prev())
	{
		EXPECT_EQ(it.get(), Type(i));
	}
}

TEST(ListIterator, RemoveFromHead)
{
	List<Type> l;
	
	for (int i = 0; i < chunk_cap; ++i)
	{
		l.insertTail(Type(i));
	}

	auto it = l.iterator();
	it.remove();

	for (int i = 1; i < chunk_cap; ++i)
	{
		EXPECT_EQ(it.get(), Type(i));
		it.next();
	}

	EXPECT_EQ(l.size(), chunk_cap - 1);
}

TEST(ListIterator, RemoveFromEndOfChunk)
{
	List<Type> l;

	for (int i = 0; i < chunk_cap; ++i)
	{
		l.insertTail(Type(i));
	}

	l.insertTail(Type(chunk_cap));

	auto it = l.iterator();

	for (int i = 0; i < chunk_cap; ++i)
	{
		it.next();
	}

	it.remove();

	it.prev();
	EXPECT_EQ(it.get(), Type(chunk_cap - 1));

	EXPECT_EQ(l.size(), chunk_cap);
}

TEST(ListIterator, InsertToNotFullChunk)
{
	List<Type> l;

	l.insertTail(Type(0));
	l.insertTail(Type(2));

	auto it = l.iterator();

	it.next();
	it.insert(Type(1));
	it.prev();

	for (int i = 0; it.hasNext(); ++i, it.next())
	{
		EXPECT_EQ(it.get(), Type(i));
	}
}

TEST(ListIterator, InsertToFullChunkWithNoNext)
{
	List<Type> l;

	l.insertTail(Type(1));
	l.insertTail(Type(2));
	l.insertTail(Type(3));
	l.insertTail(Type(4));

	auto it = l.iterator();

	it.insert(Type(0));

	for (int i = 0; it.hasNext(); ++i, it.next())
	{
		EXPECT_EQ(it.get(), Type(i));
	}
}

TEST(ListIterator, InsertToFullChunkWithFullNext)
{
	List<Type> l;

	for (int i = 0; i < 2 * chunk_cap; ++i)
	{
		l.insertTail(Type(i + 1));
	}

	auto it = l.iterator();

	it.insert(Type(0));

	for (int i = 0; it.hasNext(); ++i, it.next())
	{
		EXPECT_EQ(it.get(), Type(i));
	}
}

TEST(ListIterator, InsertToFullChunkWithNotFullNext)
{
	List<Type> l;

	for (int i = 0; i < chunk_cap + 2; ++i)
	{
		l.insertTail(Type(i + 1));
	}

	auto it = l.iterator();

	it.insert(Type(0));

	for (int i = 0; it.hasNext(); ++i, it.next())
	{
		EXPECT_EQ(it.get(), Type(i));
	}
}

//------------------------------LIST TESTS END------------------------------//
//------------------------------LIST TESTS END------------------------------//