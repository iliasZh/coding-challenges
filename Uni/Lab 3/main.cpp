#include "dictionary.h"
#include <iostream>
#include <map>

class temp
{
	int a;
public:
	temp(int a) : a{ a } {}
	int get() const { return a; }
};

bool operator==(const temp& x, const temp& y)
{
	return x.get() == y.get();
}

bool operator<(const temp& x, const temp& y)
{
	return x.get() < y.get();
}


int main()
{
	Dictionary<std::string, int> d;
	
	d.put(std::make_pair("a", 1));
	std::cout << std::endl;
	d.put(std::make_pair("b", 2));
	std::cout << std::endl;
	d.put(std::make_pair(NULL, 3));

	std::map<std::string, int> m;
	
	m.insert(std::make_pair("a", 1));
	m.insert(std::make_pair("b", 2));
	m.insert(std::make_pair(NULL, 3));

	return 0;
}