#pragma once
#include <stdlib.h>
#include <assert.h>

//#define LIST_REPORT

#ifdef LIST_REPORT
#include <iostream>
#endif

template <class T>
class List final
{
public:
	static const int chunk_capacity = 4;
private:
	class Chunk
	{
	private:
		T* items = nullptr;
		int size_ = 0;
	public:
		Chunk* prev = nullptr;
		Chunk* next = nullptr;
	public:
		Chunk() {}												// dummy chunk
		Chunk(Chunk* prev_, Chunk* next_);						// empty chunk
		Chunk(const T& item_, Chunk* prev_, Chunk* next_);		// chunk starter pack
		~Chunk();

		int size() const { return size_; }
		bool isFull() const { return size_ == chunk_capacity; }
		bool isEmpty() const { return size_ == 0; }
		const T& at(int index) const;
		T& at(int index);

		void insertAt(int index, const T& item_);
		void removeAt(int index);
		void squeeze(int index, const T& item_);				// deletes last item to squeeze item_ in
																// !LOSES DATA! BUFFER REQUIRED
																// used ONLY for Iterator::insert()
	};

	class Iterator
	{
	private:
		T* ptr;
		Chunk* curr_chunk;
		int chunk_pos = 0;
		List& l;
	public:
		explicit Iterator(List& l)
			: ptr{ &l.headDummy->next->at(0) }
			, curr_chunk{ l.headDummy->next }
			, chunk_pos{ 0 }
			, l{ l }
		{}
		const T& get() const;
		void set(const T& value);
		void insert(const T& value);
		void remove();
		void next();
		void prev();
		bool hasNext() const;
		bool hasPrev() const;
	};
private:
	Chunk* headDummy;
	Chunk* tailDummy;
	int size_ = 0;
public:
	List();
	~List();
	List(List&) = delete;							// no copying!

	void insertHead(const T& value);
	void insertTail(const T& value);
	
	void removeHead();
	void removeTail();

	const T& head() const;
	const T& tail() const;

	int size() const;

	Iterator iterator();
	const Iterator iterator() const;
};





//------------------------------ITERATOR DEFINITIONS BEGIN------------------------------//
//------------------------------ITERATOR DEFINITIONS BEGIN------------------------------//

template<class T>
const T& List<T>::Iterator::get() const
{
	return *ptr;
}

template<class T>
void List<T>::Iterator::set(const T& value)
{
	ptr -> ~T();
	new(ptr) T(value);
}


template <class T>
void List<T>::Iterator::insert(const T& value)
{
	if (curr_chunk->isFull())
	{
		T buff = curr_chunk->at(chunk_capacity - 1);			// remember last item

		curr_chunk->squeeze(chunk_pos, value);					// squeeze value in
		ptr = &curr_chunk->at(chunk_pos);

		if (curr_chunk->next->isFull() ||
			curr_chunk->next == l.tailDummy)					// if next chunk is full or is tailDummy
		{
			Chunk* new_chunk = new Chunk(buff, curr_chunk, curr_chunk->next);
			curr_chunk->next = new_chunk;
			new_chunk->next->prev = new_chunk;
		}
		else													// if next chunk has room				
		{
			curr_chunk->next->insertAt(0, buff);
		}

	}
	else														// current chunk NOT full
	{
		assert(chunk_pos < curr_chunk->size());
		curr_chunk->insertAt(chunk_pos, value);					// just insert value
		ptr = &curr_chunk->at(chunk_pos);
	}


	++l.size_;
}


template <class T>
void List<T>::Iterator::remove()
{
#ifdef LIST_REPORT
	std::cout << std::endl << "iterator: item removed" << std::endl;
#endif

	assert(curr_chunk->next != nullptr);				// be sure it's not a not a dummy chunk
	assert(curr_chunk->prev != nullptr);
	
	if (curr_chunk->size() > 1)							// curr_chunk has more than 1 item and will NOT be deleted
	{
		curr_chunk->removeAt(chunk_pos);				// remove the thing

		if (chunk_pos == curr_chunk->size())			// if removed item was last in the chunk
		{
			if (!(curr_chunk->next->isEmpty()))			// if the next chunk is NOT empty
			{
				curr_chunk = curr_chunk->next;			
				chunk_pos = 0;							// set ptr to the first item in the next chunk
				ptr = &curr_chunk->at(chunk_pos);
			}
			else										// if the next chunk IS empty == tail was removed
			{
				ptr = &curr_chunk->at(chunk_pos);		// points to item after the last
			}
		}
		else											// if removed item was NOT last in the chunk
		{
			ptr = &curr_chunk->at(chunk_pos);			// if ptr remains in the same chunk
		}
	}
	else												// curr_chunk has 1 item and WILL be deleted if there is another
	{
		assert(curr_chunk->size() == 1);
		assert(chunk_pos == 0);
		curr_chunk->removeAt(chunk_pos);				// remove the thing

		if (!(curr_chunk->next->isEmpty()))				// if the next chunk is NOT empty
		{
			Chunk* todelete = curr_chunk;
			curr_chunk = curr_chunk->next;
			chunk_pos = 0;
			ptr = &curr_chunk->at(chunk_pos);

			curr_chunk->prev = todelete->prev;			// reestablish connections
			todelete->prev->next = curr_chunk;

			delete todelete;							// delete empty chunk
		}
														// if the next chunk IS empty, do nothing
	}
	--l.size_;
}

template<class T>
void List<T>::Iterator::next()
{
	if (chunk_pos < curr_chunk->size() - 1)
	{
		++chunk_pos;
		ptr = &curr_chunk->at(chunk_pos);
	}
	else
	{
		assert(curr_chunk->next != nullptr);			// do not go beyond tailDummy
		curr_chunk = curr_chunk->next;
		chunk_pos = 0;
		if (curr_chunk != l.tailDummy)
			ptr = &curr_chunk->at(chunk_pos);
		else ptr = nullptr;
	}
}

template<class T>
void List<T>::Iterator::prev()
{
	if (chunk_pos > 0)
	{
		--chunk_pos;
		ptr = &curr_chunk->at(chunk_pos);
	}
	else
	{
		assert(curr_chunk->prev != nullptr);			// do not go beyond headDummy
		curr_chunk = curr_chunk->prev;
		if (curr_chunk == l.headDummy)
		{
			chunk_pos = 0;
			ptr = nullptr;
		}
		else
		{
			chunk_pos = curr_chunk->size() - 1;
			assert(chunk_pos >= 0);
			ptr = &curr_chunk->at(chunk_pos);
		}
		
	}
}

template<class T>
bool List<T>::Iterator::hasNext() const
{
	return curr_chunk != l.tailDummy;
}

template<class T>
bool List<T>::Iterator::hasPrev() const
{
	return curr_chunk != l.headDummy;
}

//------------------------------ITERATOR DEFINITIONS END------------------------------//
//------------------------------ITERATOR DEFINITIONS END------------------------------//





//------------------------------CHUNK DEFINITIONS BEGIN------------------------------//
//------------------------------CHUNK DEFINITIONS BEGIN------------------------------//

template <class T>
List<T>::Chunk::Chunk(Chunk* prev_, Chunk* next_)						// empty data chunk
	: prev{ prev_ }
	, next{ next_ }
{
	items = (T*)malloc(chunk_capacity * sizeof(T));
}

template <class T>
List<T>::Chunk::Chunk(const T& item_, Chunk* prev_, Chunk* next_)		// chunk starter pack
	: prev{ prev_ }
	, next{ next_ }
	, size_{ 1 }
{
	items = (T*)malloc(chunk_capacity * sizeof(T));
	new(items) T(item_);
}

template<class T>
List<T>::Chunk::~Chunk()
{
	if (items != nullptr && size_ > 0)								// if not a dummy chunk, delete content
	{
		for (int i = 0; i < size_; ++i)
		{
			(items + i) -> ~T();
		}

		free(items);
	}
}

template<class T>
void List<T>::Chunk::insertAt(int index, const T& item_)
{
	assert(items != nullptr);
	assert(index >= 0 && index <= size_);
	assert(size_ < chunk_capacity);

	for (int i = size_ - 1; i >= index; --i)
	{
		new(items + i + 1) T(*(items + i));
		(items + i) -> ~T();
	}

	new(items + index) T(item_);
	++size_;
}

template<class T>
void List<T>::Chunk::removeAt(int index)
{
	assert(items != nullptr);
	assert(index >= 0 && index < size_);
	assert(size_ > 0);

	(items + index) -> ~T();

	for (int i = index + 1; i < size_; ++i)
	{
		new(items + i - 1) T(*(items + i));
		(items + i) -> ~T();
	}

	--size_;
}

template<class T>
void List<T>::Chunk::squeeze(int index, const T& item_)
{
	assert(isFull());
	assert(index >= 0 && index < chunk_capacity);

	(items + chunk_capacity - 1) -> ~T();						// destruct last item

	for (int i = chunk_capacity - 1; i > index; --i)			// make room for value being inserted
	{
		new (items + i) T(*(items + i - 1));
		(items + i - 1) -> ~T();
	}

	new(items + index) T(item_);								// insert item_
}


template<class T>
const T& List<T>::Chunk::at(int index) const
{
	assert(index >= 0 && index < size_);
	return *(items + index);
}

template<class T>
T& List<T>::Chunk::at(int index)
{
	assert(index >= 0);
	assert(index < size_);
	return *(items + index);
}

//------------------------------CHUNK DEFINITIONS END------------------------------//
//------------------------------CHUNK DEFINITIONS END------------------------------//





//------------------------------LIST DEFINITIONS BEGIN------------------------------//
//------------------------------LIST DEFINITIONS BEGIN------------------------------//

template<class T>
List<T>::List()
{
#ifdef LIST_REPORT
	std::cout << std::endl << "default constructor called" << std::endl;
#endif

	headDummy = new Chunk();
	tailDummy = new Chunk();
	Chunk* first = new Chunk(headDummy, tailDummy);
	headDummy->next = first;
	tailDummy->prev = first;
}

template<class T>
List<T>::~List()
{
#ifdef LIST_REPORT
	std::cout << std::endl << "default destructor called" << std::endl;
#endif

	Chunk* c = tailDummy->prev;
	
	while (c != headDummy)
	{
		assert(c->next != nullptr);
		delete (c->next);
		c = c->prev;
	}

	delete c;
}

template<class T>
void List<T>::insertHead(const T& value)
{
#ifdef LIST_REPORT
	std::cout << std::endl << "insertHead called:" << std::endl << value << std::endl;
#endif

	Chunk* tmp = headDummy->next;

	if (tmp->isFull())
	{
#ifdef LIST_REPORT
		std::cout << "new chunk created" << std::endl;
#endif

		Chunk* newchunk = new Chunk(value, headDummy, tmp);
		headDummy->next = newchunk;
		tmp->prev = newchunk;
	}
	else
	{
#ifdef LIST_REPORT
		std::cout << "used existing chunk" << std::endl;
#endif

		tmp->insertAt(0, value);										// insert right in the head
	}

	++size_;
}

template<class T>
inline void List<T>::insertTail(const T& value)
{
#ifdef LIST_REPORT
	std::cout << std::endl << "insertTail called:" << std::endl << value << std::endl;
#endif
	
	Chunk* tmp = tailDummy->prev;

	if (tmp->isFull())
	{
#ifdef LIST_REPORT
		std::cout << "new chunk created" << std::endl;
#endif

		Chunk* newchunk = new Chunk(value, tmp, tailDummy);
		tmp->next = newchunk;
		tailDummy->prev = newchunk;
	}
	else
	{
#ifdef LIST_REPORT
		std::cout << "used existing chunk" << std::endl;
#endif

		tmp->insertAt(tmp->size(), value);
	}

	++size_;
}

template<class T>
void List<T>::removeHead()
{
#ifdef LIST_REPORT
	std::cout << std::endl << "removeHead called"  << std::endl;
#endif

	Chunk* tmp = headDummy->next;
	
	assert(!(tmp->isEmpty()));									// so that there's something to remove

	tmp->removeAt(0);											// remove the thing from the chunk

	if (tmp->size() == 0 && tmp->next != tailDummy)				// if chunk is empty AND is not the last one
	{
#ifdef LIST_REPORT
		std::cout << "deleted empty chunk" << std::endl;
#endif

		headDummy->next = tmp->next;							// reestablish connections
		tmp->next->prev = headDummy;


		delete tmp;												// delete it
	}

	--size_;
}

template<class T>
void List<T>::removeTail()
{
#ifdef LIST_REPORT
	std::cout << std::endl << "removeTail called" << std::endl;
#endif

	Chunk* tmp = tailDummy->prev;								// found the tail!

	assert(!(tmp->isEmpty()));									// so that there's something to remove

	tmp->removeAt(tmp->size() - 1);

	if (tmp->size() == 0 && tmp->prev != headDummy)
	{
#ifdef LIST_REPORT
		std::cout << "deleted empty chunk" << std::endl;
#endif

		tailDummy->prev = tmp->prev;
		tmp->prev->next = tailDummy;

		delete tmp;
	}

	--size_;
}

template<class T>
const T& List<T>::head() const
{
	assert(!(headDummy->next->isEmpty()));

	return headDummy->next->at(0);
}

template<class T>
const T& List<T>::tail() const
{
	assert(!(tailDummy->prev->isEmpty()));

	return tailDummy->prev->at(tailDummy->prev->size() - 1);
}

template<class T>
int List<T>::size() const
{
	return size_;
}

template<class T>
typename List<T>::Iterator List<T>::iterator()
{
	return Iterator(*this);
}

template<class T>
typename const List<T>::Iterator List<T>::iterator() const
{
	return Iterator(*this);
}

//------------------------------LIST DEFINITIONS END------------------------------//
//------------------------------LIST DEFINITIONS END------------------------------//