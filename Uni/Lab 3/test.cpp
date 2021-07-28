#include "pch.h"
#include "../Lab 3/dictionary.h"

#include <string>

class Type
{
private:
	int a;
	int* p = nullptr;
	int size_ = 0;
public:
	Type() = delete;						// default
	/*	: size_{ 8 }
	{
		p = new int[8];
		for (int i = 0; i < 8; ++i)
		{
			*(p + i) = i;
		}
	}
	*/

	Type(int a)				// set size
		: a{ a }
		, size_ { 10 }
	{
		assert(size_ > 0);

		p = new int[size_];
		for (int i = 0; i < size_; ++i)
		{
			*(p + i) = i;
		}
	}

	Type(const Type& test) : a{ test.a }		// copy
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

	int get() const
	{
		return a;
	}

	int get(int index) const
	{
		return *(p + index);
	}
};

bool operator==(const Type& x, const Type& y)
{
	return x.get() == y.get();
}

bool operator<(const Type& x, const Type& y)
{
	return x.get() < y.get();
}

TEST(Dictionary, ClassInsertion)
{
	Dictionary<Type, int> d;

	d.put(std::make_pair(Type(1), 1));
	d.put(std::make_pair(Type(2), 100));
	d.put(std::make_pair(Type(3), 300));

	d.put(std::make_pair(Type(3), 500));

	EXPECT_EQ(d[Type(1)], 1);
	EXPECT_EQ(d[Type(2)], 100);
	EXPECT_EQ(d[Type(3)], 500);

	EXPECT_EQ(d.size(), 3);
}

TEST(Dictionary, ClassCheckIfContains)
{
	Dictionary<Type, int> d;

	d.put(std::make_pair(Type(1), 1));
	d.put(std::make_pair(Type(2), 100));
	d.put(std::make_pair(Type(3), 500));

	EXPECT_TRUE(d.contains(Type(1)));
	EXPECT_TRUE(d.contains(Type(2)));
	EXPECT_TRUE(d.contains(Type(3)));

	EXPECT_FALSE(d.contains(Type(4)));
	
	EXPECT_EQ(d.size(), 3);
}

TEST(Dictionary, ClassConstBrackets)
{
	Dictionary<Type, int> d;

	d.put(std::make_pair(Type(1), 1));
	d.put(std::make_pair(Type(2), 100));
	d.put(std::make_pair(Type(3), 500));

	int i = const_cast<const Dictionary<Type, int>&>(d)[Type(4)];
	EXPECT_EQ(i, 0);

	EXPECT_EQ(d[Type(1)], 1);
	EXPECT_EQ(d[Type(2)], 100);
	EXPECT_EQ(d[Type(3)], 500);

	EXPECT_EQ(d.size(), 3);
}

TEST(Dictionary, ClassNonConstBrackets)
{
	Dictionary<Type, int> d;

	d.put(std::make_pair(Type(1), 1));
	d.put(std::make_pair(Type(2), 100));
	d.put(std::make_pair(Type(3), 500));

	d[Type(1)] = 2;
	EXPECT_EQ(d[Type(1)], 2);

	d[Type(4)];
	EXPECT_TRUE(d.contains(Type(4)));

	EXPECT_EQ(d.size(), 4);
}


TEST(Dictionary, ClassDeleteMax)
{
	Dictionary<Type, std::string> d;

	d.put(std::make_pair(Type(0), "a"));
	d.put(std::make_pair(Type(1), "b"));
	d.put(std::make_pair(Type(2), "c"));
	d.put(std::make_pair(Type(3), "d"));
	d.put(std::make_pair(Type(4), "e"));
	d.put(std::make_pair(Type(5), "f"));

	d.remove(Type(5));
	EXPECT_EQ(d.size(), 5);

	std::string t;
	for (int i = 0; i < d.size(); ++i)
	{
		t += d[Type(i)];
	}

	EXPECT_EQ(t, "abcde");
}



TEST(Dictionary, ClassDeleteMin)
{
	Dictionary<Type, std::string> d;

	d.put(std::make_pair(Type(0), "a"));
	d.put(std::make_pair(Type(1), "b"));
	d.put(std::make_pair(Type(2), "c"));
	d.put(std::make_pair(Type(3), "d"));
	d.put(std::make_pair(Type(4), "e"));
	d.put(std::make_pair(Type(5), "f"));

	d.remove(Type(0));
	EXPECT_EQ(d.size(), 5);

	std::string t;
	for (int i = 0; i < d.size(); ++i)
	{
		t += d[Type(i + 1)];
	}

	EXPECT_EQ(t, "bcdef");
}


TEST(Dictionary, ClassDeleteLeaf)
{
	Dictionary<Type, std::string> d;

	d.put(std::make_pair(Type(0), "a"));
	d.put(std::make_pair(Type(1), "b"));
	d.put(std::make_pair(Type(2), "c"));
	d.put(std::make_pair(Type(3), "d"));
	d.put(std::make_pair(Type(4), "e"));
	d.put(std::make_pair(Type(5), "f"));

	d.remove(Type(2));
	EXPECT_EQ(d.size(), 5);


	std::string t;

	t += d[Type(0)];
	t += d[Type(1)];
	// t += d[Type(2)];
	t += d[Type(3)];
	t += d[Type(4)];
	t += d[Type(5)];

	EXPECT_EQ(t, "abdef"); // ab[c]def
}

TEST(Dictionary, ClassDeleteMiddle)
{
	Dictionary<Type, std::string> d;

	d.put(std::make_pair(Type(0), "a"));
	d.put(std::make_pair(Type(1), "b"));
	d.put(std::make_pair(Type(2), "c"));
	d.put(std::make_pair(Type(3), "d"));
	d.put(std::make_pair(Type(4), "e"));
	d.put(std::make_pair(Type(5), "f"));

	d.remove(Type(1));
	EXPECT_EQ(d.size(), 5);

	std::string t;

	t += d[Type(0)];
	//t += d[Type(1)];
	t += d[Type(2)];
	t += d[Type(3)];
	t += d[Type(4)];
	t += d[Type(5)];

	EXPECT_EQ(t, "acdef"); // a[b]cdef
}

TEST(Dictionary, ClassDeleteRoot)
{
	Dictionary<Type, std::string> d;

	d.put(std::make_pair(Type(0), "a"));
	d.put(std::make_pair(Type(1), "b"));
	d.put(std::make_pair(Type(2), "c"));
	d.put(std::make_pair(Type(3), "d"));
	d.put(std::make_pair(Type(4), "e"));
	d.put(std::make_pair(Type(5), "f"));

	d.remove(Type(3));

	EXPECT_EQ(d.size(), 5);

	std::string t;

	t += d[Type(0)];
	t += d[Type(1)];
	t += d[Type(2)];
	//t += d[Type(3)];
	t += d[Type(4)];
	t += d[Type(5)];

	EXPECT_EQ(t, "abcef"); // abc[d]ef
}




//------------------------------ITERATOR------------------------------
//------------------------------ITERATOR------------------------------

TEST(DictionaryIterator, GetKeyAndValue)
{
	Dictionary<Type, std::string> d;

	d.put(std::make_pair(Type(4), "f"));

	auto it = d.iterator();

	EXPECT_EQ(it.key(), Type(4));
	EXPECT_EQ(it.get(), "f");
}

TEST(DictionaryIterator, SetValue)
{
	Dictionary<Type, std::string> d;

	d.put(std::make_pair(Type(4), "f"));

	auto it = d.iterator();

	it.set("kek");

	EXPECT_EQ(it.key(), Type(4));
	EXPECT_EQ(it.get(), "kek");
}

TEST(DictionaryIterator, NextLoop)
{
	Dictionary<Type, char> d;

	d.put(std::make_pair(Type(4), 'e'));
	d.put(std::make_pair(Type(6), 'g'));
	d.put(std::make_pair(Type(1), 'b'));
	d.put(std::make_pair(Type(3), 'd'));
	d.put(std::make_pair(Type(2), 'c'));
	d.put(std::make_pair(Type(7), 'h'));
	d.put(std::make_pair(Type(5), 'f'));
	d.put(std::make_pair(Type(8), 'i'));
	d.put(std::make_pair(Type(0), 'a'));


	std::string t;
	for (auto it = d.iterator(); it.hasNext(); it.next())
	{
		t += it.get();
	}

	EXPECT_EQ(t, "abcdefghi");
}

TEST(DictionaryIterator, NextPrevLoop)
{
	Dictionary<Type, char> d;

	d.put(std::make_pair(Type(4), 'e'));
	d.put(std::make_pair(Type(6), 'g'));
	d.put(std::make_pair(Type(1), 'b'));
	d.put(std::make_pair(Type(3), 'd'));
	d.put(std::make_pair(Type(2), 'c'));
	d.put(std::make_pair(Type(7), 'h'));
	d.put(std::make_pair(Type(5), 'f'));
	d.put(std::make_pair(Type(8), 'i'));
	d.put(std::make_pair(Type(0), 'a'));


	std::string t;
	auto it = d.iterator();

	for (; it.hasNext(); it.next())
	{
		t += it.get();
	}

	it.prev();

	for (; it.hasPrev(); it.prev())
	{
		t += it.get();
	}

	EXPECT_EQ(t, "abcdefghiihgfedcba");
}