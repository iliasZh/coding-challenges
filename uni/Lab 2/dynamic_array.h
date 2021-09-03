#pragma once
#include <stdlib.h>
#include <assert.h>
#include <new>

//#define ARRAY_REPORT

#ifdef ARRAY_REPORT
#include <iostream>
#endif

template<class T>
class Array final											// final - cannot be inherited from
{
public:
	static constexpr int default_capacity = 8;				// size for the default constructor
private:
	static constexpr float allocation_multiplier = 2.0f;
	
	T* items = nullptr;							// Array
	int capacity_ = 0;							// actual capacity of a dynamically *allocated* array
	int size_ = 0;								// size of the actually *occupied* part of an Array
	
	class Iterator
	{
	private:
		T* ptr_;
		Array<T>& arr_;
	public:
		Iterator() = delete;
		Iterator(Array<T>& a)
			: ptr_{ a.items }
			, arr_{ a }
		{}

		const T& get() const;
		void set(const T& value);
		void insert(const T& value);
		void remove();
		void next();
		void prev();
		void toIndex(int index);
		bool hasNext() const;
		bool hasPrev() const;
	};

public:
	Array();
	Array(int capacity);
	Array(Array&) = delete;								// copy constructor deleted
	~Array();
	void insert(const T& value);
	void insert(int index, const T& value);
	void remove(int index);
	const T& operator[](int index) const;
	T& operator[](int index);
	int size() const;
	Iterator iterator();
	const Iterator iterator() const;
};


//---------------------ITERATOR DEFINITIONS BEGIN---------------------//
//---------------------ITERATOR DEFINITIONS BEGIN---------------------//

template<class T>
bool Array<T>::Iterator::hasNext() const
{
	return ptr_ != (arr_.items + arr_.size());
}

template<class T>
bool Array<T>::Iterator::hasPrev() const
{
	return ptr_ != arr_.items - 1;
}

template<class T>
const T& Array<T>::Iterator::get() const
{
	return *ptr_;
}

template<class T>
void Array<T>::Iterator::set(const T& value)
{
	ptr_ -> ~T();
	new(ptr_) T(value);
}

template<class T>
void Array<T>::Iterator::insert(const T& value)
{
	const int index = ptr_ - arr_.items;
	arr_.insert(index, value);
	this->toIndex(index);
}

template<class T>
void Array<T>::Iterator::remove()
{
	assert(arr_.size() > 0);
	const int index = ptr_ - arr_.items;
	arr_.remove(index);
	ptr_ = arr_.items + index;				// if deleting last item in array,
											// ptr_ will point to item after the last
}

template<class T>
void Array<T>::Iterator::next()
{
	assert(hasNext());
	++ptr_;
}

template<class T>
void Array<T>::Iterator::prev()
{
	assert(hasPrev());
	--ptr_;
}

template<class T>
void Array<T>::Iterator::toIndex(int index)
{
	assert(index >= 0 && index < arr_.size());
	ptr_ = arr_.items + index;
}

//---------------------ITERATOR DEFINITIONS END---------------------//
//---------------------ITERATOR DEFINITIONS END---------------------//



//---------------------ARRAY DEFINITIONS BEGIN---------------------//
//---------------------ARRAY DEFINITIONS BEGIN---------------------//

template<class T>
Array<T>::Array()
{
#ifdef ARRAY_REPORT
	std::cout << std::endl << "default constructor called" << std::endl;
#endif

	items = (T*)malloc(default_capacity * sizeof(T));

	capacity_ = default_capacity;
	size_ = 0;
}

template<class T>
Array<T>::Array(int capacity)
{
	assert(capacity > 0);

#ifdef ARRAY_REPORT
	std::cout << std::endl << "constructor called, capacity = " << capacity << std::endl;
#endif
	
	items = (T*)malloc(capacity * sizeof(T));

	capacity_ = capacity;
	size_ = 0;
}

template<class T>
Array<T>::~Array()
{
#ifdef ARRAY_REPORT
	std::cout << std::endl << "destructor called" << std::endl;
#endif

	for (int i = 0; i < size_; ++i)
	{
		(items + i)->~T();								// destruct items
	}

	free(items);										// deallocate memory
}

template<class T>
void Array<T>::insert(const T& value)					// inserts to back by default
{
	if (size_ < capacity_)
	{
		new(items + size_) T(value);					// placement new: call copy constructor at (items + size_)
		++size_;


#ifdef ARRAY_REPORT
		std::cout << std::endl << "value" << std::endl << value << std::endl <<
			"added with no reallocation" << std::endl;
#endif
	}
	else
	{
		const int new_capacity = int(float(capacity_) * allocation_multiplier);
		
		T* tmp = (T*)malloc(new_capacity * sizeof(T));	// allocate a new array

		for (int i = 0; i < size_; ++i)					// copy from the old array
		{
			new(tmp + i) T(*(items + i));				// placement new: call copy contructor at (tmp + i)
			(items + i) -> ~T();						// destruct copied (old) items
		}

		free(items);									// deallocate memory

		items = tmp;									// redirect items ptr to new array
		new(items + size_) T(value);					// place new value
		

#ifdef ARRAY_REPORT
		std::cout << std::endl << "value" << std::endl << value << std::endl <<
			"added with reallocation: old capacity = " << capacity_ <<
			", new capacity = " << new_capacity << std::endl;
#endif


		capacity_ = new_capacity;						// update Array parameters
		++size_;
	}
}

template<class T>										// no move semantics :(
void Array<T>::insert(int index, const T& value)
{
	assert(index >= 0 && index <= size_);

	if (size_ < capacity_)
	{
		for (int i = size_ - 1; i >= index; --i)
		{
			new(items + i + 1) T(*(items + i));			// copy item to next cell
			(items + i) -> ~T();						// destruct prev item
		}

		new (items + index) T(value);					// place value in the freed spot

		++size_;										// update size


#ifdef ARRAY_REPORT
		std::cout << std::endl << "value" << std::endl << value << std::endl << "inserted at index " <<
			index << " with no reallocation" << std::endl;
#endif
	}
	else
	{
		const int new_capacity = int(float(capacity_) * allocation_multiplier);

		T* tmp = (T*)malloc(new_capacity * sizeof(T));	// allocate new array

		for (int i = 0; i < index; ++i)					// copy up to insertion index
		{
			new(tmp + i) T(*(items + i));				// placement new
			(items + i) -> ~T();						// destruct old items
		}

		new (tmp + index) T(value);						// copy inserted value

		for (int i = index + 1; i < size_ + 1; ++i)		// copy rest
		{
			new(tmp + i) T(*(items + i - 1));			// placement new
			(items + i - 1) -> ~T();					// destruct old items
		}

		free(items);									// deallocate memory

		items = tmp;									// redirect items to new array


#ifdef ARRAY_REPORT
		std::cout << std::endl << "value" << std::endl << value << std::endl <<
			"inserted at index " << index <<
			" with reallocation: old capacity = " << capacity_ <<
			", new capacity = " << new_capacity << std::endl;
#endif


		capacity_ = new_capacity;						// update parameters
		++size_;
	}

}

template<class T>
void Array<T>::remove(int index)
{
	assert(size_ > 0);
	assert(index >= 0 && index < size_);

#ifdef ARRAY_REPORT
	std::cout << std::endl << "value" << std::endl << *(items + index)
		<< std::endl << "removed at index " << index << std::endl;
#endif

	(items + index) -> ~T();							// destruct the item being removed

	for (int i = index + 1; i < size_; ++i)
	{
		new (items + i - 1) T(*(items + i));			// shift items + i to the left
		(items + i) -> ~T();							// destruct old copy
	}

	--size_;											// update size_
}

template<class T>
const T& Array<T>::operator[](int index) const
{
	assert(index >= 0 && index < size_);
	return *(items + index);
}

template<class T>
T& Array<T>::operator[](int index)
{
	assert(index >= 0 && index < size_);
	return *(items + index);
}

template<class T>
int Array<T>::size() const
{
	assert(size_ >= 0);
	return size_;
}

template<class T>
typename Array<T>::Iterator Array<T>::iterator()
{
	return Iterator(*this);
}

template<class T>
typename const Array<T>::Iterator Array<T>::iterator() const
{
	return Iterator(*this);
}

//---------------------ARRAY DEFINITIONS END---------------------//
//---------------------ARRAY DEFINITIONS END---------------------//
