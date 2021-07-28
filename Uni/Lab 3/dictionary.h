#pragma once
#include <assert.h>
#include <iostream>
#include <initializer_list>

template<class K>
int compare(const K& k1, const K& k2)
{
	if (k1 == k2) return 0;
	if (k1 < k2) return -1;
	else return 1;
}

template<class K, class V>
class Dictionary final
{
private:
	static const bool RED = false;
	static const bool BLACK = true;

	struct Node
	{
		bool color;
		K key;
		V value;
		Node* parent = nullptr;
		Node* left = nullptr;
		Node* right = nullptr;
		Node() = delete;
		void toggleColor() { color = !color; }
		Node(const K& key, const V& value, bool color) :key{ key }, value{ value }, color{ color } { }
	};

	class Iterator
	{
	private:
		Node* ptr;
		Dictionary& dict;
		int state;										// -1: beyond begin(), 0: valid, +1: beyond end() 
	public:
		Iterator(Dictionary& dict) : dict{ dict }
		{
			ptr = dict.root;
			while (ptr->left)
			{
				ptr = ptr->left;
			}
			state = 0;
		}
		const K& key() const
		{
			return ptr->key;
		}
		const V& get() const
		{
			return ptr->value;
		}
		void set(const V& value)
		{
			ptr->value = value;
		}
		void next()
		{
			assert(state != 1);					// cannot go further if state is 1

			if (state == -1)
			{
				ptr = dict.root;
				while (ptr->left)
				{
					ptr = ptr->left;
				}
				state = 0;
				return;
			}

			if (ptr->right)						// if has a right child
			{
				ptr = ptr->right;				// go to right child

				while (ptr->left)				// search for the left-est child
				{
					ptr = ptr->left;
				}
				return;
			}
			else								// if does not have a right child
			{
				Node* buff = ptr;
				while (true)
				{
					if (ptr->parent)			// if has a parent, go up till parent with a left child
					{
						buff = ptr;
						ptr = ptr->parent;
						if (ptr->left == buff) return;
					}
					else						// gone up to the root -> this is the last element
					{
						ptr = nullptr;
						state = 1;				// is out of bounds now
						return;
					}
				}
			}
		}
		void prev()
		{
			assert(state != -1);			// cannot go backwards if state is -1

			if (state == 1)
			{
				ptr = dict.root;
				while (ptr->right)
				{
					ptr = ptr->right;
				}
				state = 0;					// is normal again
				return;
			}


			if (ptr->left)						// if has a left child
			{
				ptr = ptr->left;				// go to left child

				while (ptr->right)				// search for the right-est child
				{
					ptr = ptr->right;
				}
				return;
			}
			else								// if does not have a left child
			{
				Node* buff = ptr;
				while (true)
				{
					if (ptr->parent)			// if has a parent, go up till parent with a right child
					{
						buff = ptr;
						ptr = ptr->parent;
						if (ptr->right == buff) return;
					}
					else						// gone up to the root -> this is the first element
					{
						ptr = nullptr;
						state = -1;				// is out of bounds now
						return;
					}
				}
			}
		}
		bool hasNext() const
		{
			return state != 1;
		}
		bool hasPrev() const
		{
			return state != -1;
		}
	};

	Node* root = nullptr;
	int size_ = 0;
private:
	// helper methods
	void flipColor(Node* node);
	Node* rotateLeft(Node* x);
	Node* rotateRight(Node* x);
	Node* fixUp(Node* x);
	Node* put(Node* node, const K& key, const V& value);
	void recursiveDelete(Node* node);
	Node* moveRedRight(Node* node);
	Node* moveRedLeft(Node* node);
	//Node* deleteMax(Node* node);
	Node* deleteMin(Node* node);
	Node* deleteAny(Node* node, const K& key);
	
public:
	Dictionary() { }
	Dictionary(const Dictionary&) = delete;
	~Dictionary();
	void put(std::pair<K, V> p);
	bool contains(const K& key);
	V& operator[](const K& key);
	const V& operator[](const K& key) const;
	int size() const { return size_; }
	void remove(const K& key);

	Iterator iterator();
	const Iterator iterator() const;
};

template<class K, class V>
void Dictionary<K, V>::flipColor(Node* node)
{
	assert(node->left && node->right);					// check if has kids
	if (node->parent) node->toggleColor();				// root should remain black
	node->left->toggleColor();							// flip 'em
	node->right->toggleColor();
}

template<class K, class V>
typename Dictionary<K, V>::Node* Dictionary<K, V>::rotateLeft(Node* x)
{
	Node* y = x->right;
	assert(y && y->color == RED);
	
	x->right = y->left;
	y->parent = x->parent;
	
	if (y->left)
	{
		y->left->parent = x;	
	}

	y->left = x;
	x->parent = y;

	y->color = x->color;
	x->color = RED;
	return y;
}

template<class K, class V>
typename Dictionary<K, V>::Node* Dictionary<K, V>::rotateRight(Node* x)
{
	Node* y = x->left;
	assert(y && y->color == RED);

	x->left = y->right;
	y->parent = x->parent;

	if (y->right)
	{
		y->right->parent = x;
	}

	y->right = x;
	x->parent = y;

	y->color = x->color;
	x->color = RED;

	return y;
}

template<class K, class V>
typename Dictionary<K, V>::Node* Dictionary<K, V>::fixUp(Node* x)
{
	assert(x);

	if (x->right &&
		x->right->color == RED)
	{
		x = rotateLeft(x);
	}

	if (x->left && x->left->left &&
		x->left->color == RED && x->left->left->color == RED)
	{
		x = rotateRight(x);
	}

	if (x->left && x->right &&
		x->left->color == RED && x->right->color == RED)
	{
		flipColor(x);
	}

	return x;
}

template<class K, class V>
typename Dictionary<K, V>::Node* Dictionary<K, V>::put(Node* node, const K& key, const V& value)
{
	if (!node)
	{
		++size_;
		return new Node(key, value, node == root);		// if node is root -> true == BLACK
	}

	int cmp = compare(key, node->key);

	if (cmp == 0)
	{
		node->value = value;
	}
	else if (cmp < 0)
	{
		node->left = put(node->left, key, value);
		node->left->parent = node;
	}
	else
	{
		node->right = put(node->right, key, value);
		node->right->parent = node;
	}

	return fixUp(node);
}

template<class K, class V>
void Dictionary<K, V>::recursiveDelete(Node* node)
{
	if (node)
	{
		if (!(node->left) && !(node->right))
			delete node;
		else
		{
			if (node->left) recursiveDelete(node->left);
			if (node->right) recursiveDelete(node->right);
		}
	}
}

template<class K, class V>
typename Dictionary<K, V>::Node* Dictionary<K, V>::moveRedRight(Node* node)
{
	assert(node);
	flipColor(node);
	if (node->left->left &&
		node->left->left->color == RED)
	{
		node = rotateRight(node);
		flipColor(node);
	}
	return node;
}

template<class K, class V>
typename Dictionary<K, V>::Node* Dictionary<K, V>::moveRedLeft(Node* node)
{
	assert(node);
	flipColor(node);

	if (node->right->left &&
		node->right->left->color == RED)
	{
		node->right = rotateRight(node->right);
		node = rotateLeft(node);
		flipColor(node);
	}

	return node;
}

/*
template<class K, class V>
typename LLRB<K, V>::Node* LLRB<K, V>::deleteMax(Node* node)
{
	if (node->left &&
		node->left->color == RED)
	{
		node = rotateRight(node);
	}

	if (!node->right)
	{
		delete node;
		return nullptr;
	}

	if (node->right && node->right->color == BLACK &&
		(!node->right->left || (node->right->left && node->right->left->color == BLACK)))
	{
		node = moveRedRight(node);
	}

	node->right = deleteMax(node->right);

	return fixUp(node);
}
*/

template<class K, class V>
typename Dictionary<K, V>::Node* Dictionary<K, V>::deleteMin(Node* node)
{
	if (!node->left)
	{
		delete node;
		return nullptr;
	}

	if (node->left && node->left->color == BLACK &&
		(!node->left->left || (node->left->left && node->left->left->color == BLACK)))
	{
		node = moveRedLeft(node);
	}

	node->left = deleteMin(node->left);
	
	return fixUp(node);
}

template<class K, class V>
typename Dictionary<K, V>::Node* Dictionary<K, V>::deleteAny(Node* node, const K& key)
{
	int cmp = compare(key, node->key);

	if (cmp < 0)														// L
	{
		if (node->left && node->left->color == BLACK &&					// push red link to left
			(!node->left->left || (node->left->left && node->left->left->color == BLACK)))
		{
			node = moveRedLeft(node);
		}

		node->left = deleteAny(node->left, key);						// move to the next
	}
	else																// EQ or R
	{
		if (node->left && node->left->color == RED)						// rotate red link to right if necessary
		{
			node = rotateRight(node);
		}

		cmp = compare(key, node->key);									// update because node might have changed

		if (cmp == 0 && !node->right)									// if found needed node at bottom
		{
			delete node;
			return nullptr;
		}

		if (node->right && node->right->color == BLACK &&				// push red right if necessary
			(!node->right->left || (node->right->left && node->right->left->color == BLACK)))
		{
			node = moveRedRight(node);
		}

		if (cmp == 0)													// not at the bottom
		{
			Node* minRight = node;
			assert(minRight->right);
			minRight = minRight->right;
			while (minRight->left)
			{
				minRight = minRight->left;									// find min in right subtree
			}

			node->key = minRight->key;										// current node takes
			node->value = minRight->value;									// its successor's state

			node->right = deleteMin(node->right);							// delete successor
		}
		else																// R
		{
			node->right = deleteAny(node->right, key);						// move to the next
		}
	}

	return fixUp(node);														// fix everything on the way up
}



template<class K, class V>
Dictionary<K, V>::~Dictionary()
{
	recursiveDelete(root);
}

template<class K, class V>
void Dictionary<K, V>::put(std::pair<K, V> p)
{
	root = put(root, p.first, p.second);
}

template<class K, class V>
bool Dictionary<K, V>::contains(const K& key)
{
	Node* search = root;

	while (search)
	{
		int cmp = compare(key, search->key);

		if (cmp == 0)
			return true;
		else if (cmp < 0)
			search = search->left;
		else
			search = search->right;
	}
	return false;
}

template<class K, class V>
V& Dictionary<K, V>::operator[](const K& key)
{
	Node* search = root;

	if (!contains(key))
	{
		put(std::make_pair(key, V()));
	}

	while (search)
	{
		int cmp = compare(key, search->key);

		if (cmp == 0)
			return search->value;
		else if (cmp < 0)
			search = search->left;
		else
			search = search->right;
	}
}

template<class K, class V>
const V& Dictionary<K, V>::operator[](const K& key) const
{
	Node* search = root;

	while (search)
	{
		int cmp = compare(key, search->key);

		if (cmp == 0)
			return search->value;
		else if (cmp < 0)
			search = search->left;
		else
			search = search->right;
	}
	return V();
}

template<class K, class V>
void Dictionary<K, V>::remove(const K& key)
{
	if (contains(key))
	{
		root = deleteAny(root, key);
		--size_;
	}
}

template<class K, class V>
typename Dictionary<K, V>::Iterator Dictionary<K, V>::iterator()
{
	return Iterator(*this);
}

template<class K, class V>
const typename Dictionary<K, V>::Iterator Dictionary<K, V>::iterator() const
{
	return Iterator(*this);
}
