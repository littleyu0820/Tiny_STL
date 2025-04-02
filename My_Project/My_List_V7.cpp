#include <iostream>
#include <algorithm>
#include <memory>
#include <cassert>


//v7


int print_blocks();
void Marks();


template<typename T>
struct ListNode //the struct of the node
{
	T data; //store the element in the node
	ListNode<T>* preptr; //the normal pointer to help us access the last node
	ListNode<T>* nextptr;
	//the unique_ptr point to the next node which means that this pointer has the possession of the next node

	//Constructor
	ListNode(const T& val) : data(val), preptr(nullptr), nextptr(nullptr) {}
	ListNode(T&& val) : data(val), preptr(nullptr), nextptr(nullptr) {}

};

template<typename T>
class MyList
{
public:
	//Constructor and Destructor
	MyList() : virtual_tail(new ListNode<T>(T())), head(nullptr), tail(nullptr), size(0)
	{
		std::cout << "Default Constructor..." << std::endl;
	}

	//Direct-Initialization
	MyList(std::initializer_list<T> il) :
		virtual_tail(new ListNode<T>(T())), head(nullptr), tail(nullptr), size(0)
	{
		std::cout << "Initialize List Constructor..." << std::endl;
		auto beg = il.begin();
		while (beg != il.end())
		{
			push_back(*beg++);
		}

	}


	MyList(const MyList& list) : virtual_tail(new ListNode<T>(T()))
	{
		std::cout << "Copy Constructor..." << std::endl;
		auto temp = list.head;
		while (temp->nextptr)
		{
			push_back(temp->data);
			temp = temp->nextptr;
		}

	}
	MyList& operator=(const MyList&) = default;

	//Move-Constructor
	MyList(MyList&& list) noexcept :
		head(list.head), tail(list.tail), size(list.size)
	{
		std::cout << "Move Cosntructor..." << std::endl;
		if (list.empty())
			virtual_tail = new ListNode<T>(T());
		else
			virtual_tail = list.virtual_tail;

		list.virtual_tail = list.head = list.tail = nullptr;
		list.size = 0;
	}
	//Move-Assing and it will call Move-Constructor
	MyList& operator=(MyList&& list) noexcept = default;

	//Destructor
	~MyList() noexcept
	{
		if (head)
		{
			std::cout << "Destructor..." << std::endl;
			free();
		}
	}

	/****************************************Iterator*****************************************************************/
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class List_Iterator
	{
	public:
		List_Iterator() : it_ptr(nullptr) {}
		List_Iterator(ListNode<T>* node) :
			it_ptr(node) {
		}

		T& operator*()
		{
			assert(it_ptr && it_ptr->nextptr && "Error: Dereferencing an illegal iterator!");
			return it_ptr->data;
		}

		ListNode<T>* operator->()
		{
			return it_ptr;
		}

		List_Iterator next(const List_Iterator& it, std::ptrdiff_t pos) const
		{
			return it + pos;
		}

		List_Iterator operator++()
		{
			it_ptr = it_ptr->nextptr;
			return *this;
		}

		List_Iterator operator++(int)
		{
			auto tmp = *this;
			it_ptr = it_ptr->nextptr;
			return tmp;
		}

		List_Iterator operator--()
		{

			assert(it_ptr && it_ptr->preptr && "Error: Decrementing an illegal iterator");
			it_ptr = it_ptr->preptr;
			return *this;
		}

		List_Iterator operator--(int)
		{
			assert(it_ptr && it_ptr->preptr && "Error: Decrementing an illegal iterator");
			auto tmp = *this;
			it_ptr = it_ptr->preptr;
			return tmp;
		}

		List_Iterator operator+(std::ptrdiff_t pos) const
		{

			auto temp = *this;
			while (temp.it_ptr && temp.it_ptr->nextptr && pos != 0)
			{
				temp.it_ptr = temp.it_ptr->nextptr;
				--pos;
			}

			assert(pos == 0 && "Error: Incrementing an illegal iterator!");

			return temp;
		}
		List_Iterator operator-(std::ptrdiff_t pos) const
		{
			auto temp = *this;
			while (temp.it_ptr && temp.it_ptr->preptr && pos != 0)
			{
				temp.it_ptr = temp.it_ptr->preptr;
				--pos;
			}
			assert(pos == 0 && "Error: Decrementing an illegal iterator!");

			return temp;
		}

		size_t operator-(List_Iterator& rhs)
		{
			auto lhs = *this;
			assert(lhs->preptr && "Error: Illegal iterator!");
			size_t offset = 0;
			while (lhs.it_ptr->preptr)
			{
				if(rhs.it_ptr != lhs.it_ptr)
					++offset;
				lhs.it_ptr = lhs.it_ptr->preptr;
			}
			
			return offset;
		}



		bool operator==(const List_Iterator& rhs) const
		{
			return it_ptr == rhs.it_ptr;
		}

		bool operator!=(const List_Iterator& rhs) const
		{
			return !(*this == rhs);
		}

	private:
		ListNode<T>* it_ptr;

	};



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   /****************************************Iterator*****************************************************************/


	T& operator[](size_t index)
	{
		assert(index >= 0 && index < size && "Error: Out of range!");
		if (index == 0)
			return head->data;
		else if (index == size - 1)
			return tail->data;
		else
		{
			auto tmp = head;
			while (index != 0)
			{
				tmp = tmp->nextptr;
				--index;
			}
			return tmp->data;
		}

	}

	List_Iterator begin() const { return List_Iterator(head); }
	List_Iterator end()
	{
		return List_Iterator(virtual_tail);
	}

	void insert(const List_Iterator&, const T&);
	void erase(const List_Iterator&);

	void reverse();
	MyList<T>& sort();
	MyList<T>& merge(MyList<T>&, MyList<T>& list);
	MyList<T>& mergesort(MyList<T>&);

	void slice(size_t, size_t, MyList<T>&);
	void splice(List_Iterator&, MyList<T>&, List_Iterator&, List_Iterator&);

	void push_front(const T&);
	void pop_front();
	void push_back(const T&); //use to add the element at the tail
	void pop_back();
	size_t get_size() const ;
	bool empty();

	void clear()
	{

		assert(!empty() && "Error: Clearing an empty list!");
		std::cout << "Clearing the list..." << std::endl;
		while (head)
		{
			auto tmp = head;
			head = head->nextptr;
			delete tmp;
		}
		tail = nullptr;
		virtual_tail = new ListNode<T>(T());
		size = 0;
	}



	void print() const;


private:

	ListNode<T>* head;
	//the unique_ptr point to the head which means that this pointer(head) will have the possession of the whole list
	ListNode<T>* tail; //the normal pointer point to the tail which can help us access the last node
	ListNode<T>* virtual_tail;
	size_t size;

	void free()
	{
		while (head->nextptr)
		{
			ListNode<T>* tmp = head;
			head = head->nextptr;
			std::cout << "Deleting " << tmp->data << "..." << std::endl;
			delete tmp;
		}
		delete virtual_tail;
	}

	ListNode<T>* get_mid(MyList<T>& list)
	{
		assert(size > 1 && "Error: Spliting an empty or a single-node list!");

		
		ListNode<T>* slow_ptr = head;
		ListNode<T>* fast_ptr = head->nextptr;

		while (fast_ptr->nextptr)
		{
			slow_ptr = slow_ptr->nextptr;
			fast_ptr = fast_ptr->nextptr->nextptr;
		}

		return slow_ptr;
	}


};


template<typename T>
size_t MyList<T>::get_size() const 
{
	if (!head)
	{
		std::cout << "The list is empty." << std::endl;
		return -1;
	}
	else
		std::cout << "The size is: " << size << std::endl;
	return size;
}

template<typename T>
bool MyList<T>::empty()
{
	if (!head)
		return true;
	else
		return false;
}

template<typename T>
void MyList<T>::insert(const List_Iterator& pos, const T& val)
{
	assert(!empty() && "Error: Inserting an empty list!");
	if (pos == begin())
		push_front(val);
	else if (pos == end())
		push_back(val);
	else
	{
		auto beg = begin();
		auto ctr = 0;
		while (beg != pos)
		{
			beg = beg + 1;
		}
		auto newnode = new ListNode<T>(val);
		auto temp = beg->preptr;
		newnode->nextptr = temp->nextptr;
		temp->nextptr = newnode;
		newnode->preptr = temp;
		beg->preptr = newnode;
		++size;
	}
}

template<typename T>
void MyList<T>::erase(const List_Iterator& pos)
{
	auto e = end();
	if (pos == begin())
		pop_front();
	else if (pos == e - 1)
		pop_back();
	else
	{
		auto beg = begin();
		auto ctr = 0;
		while (beg != pos)
		{
			beg = beg + 1;
			++ctr;
		}
		assert(ctr < size && "Error: Out of range!");
		auto temp = beg->nextptr;
		beg->preptr->nextptr = temp;
		temp->preptr = beg->preptr;
		beg = nullptr;
		--size;
	}
}

template<typename T>
void MyList<T>::push_front(const T& val)
{
	ListNode<T>* newnode = new ListNode<T>(val);
	if (!head)
		head = tail = newnode;
	else
	{
		head->preptr = newnode;
		newnode->nextptr = head;
		head = newnode;
	}
	++size;
}

template<typename T>
void MyList<T>::pop_front()
{
	assert(!empty() && "Error: Popping an empty list!");
	auto tmp = head;
	head = head->nextptr;
	if (head)
		head->preptr = nullptr;
	else
		tail = nullptr;
	delete tmp;
	--size;
}

template<typename T>
void MyList<T>::push_back(const T& val)
{
	ListNode<T>* newnode = new ListNode<T>(val);
	if (!head)
		head = tail = newnode;
	else
	{
		tail->nextptr = newnode;
		newnode->preptr = tail;
		tail = newnode;
	}
	tail->nextptr = virtual_tail;
	virtual_tail->preptr = tail;
	++size;
}


template<typename T>
void MyList<T>::pop_back()
{
	assert(!empty() && "Error: Popping an empty list!");
	auto tmp = tail;
	tail = tail->preptr;
	if (tail)
		tail->nextptr = nullptr;
	else
		head = nullptr;;
	delete tmp;

	if (tail)
	{
		tail->nextptr = virtual_tail;
		virtual_tail->preptr = tail;
	}
	else
		virtual_tail->preptr = nullptr;

	--size;
}


template<typename T>
void MyList<T>::slice(size_t left, size_t right, MyList<T>& newlist)
{
	assert(size > 1 && left >= 0 && right <= size && left < right && "Error: Slicing an empty or a single-node list!");
	std::cout << "Slicing the list from index 0 to index 2..." << std::endl;
	for (size_t index = 0; index < size; ++index)
	{
		if (index >= left && index < right)
			newlist.push_back((*this)[index]);
	}
	
}


template<typename T>
void MyList<T>::splice(List_Iterator& target_pos, MyList<T>& list, List_Iterator& left, List_Iterator& right)
{
	auto offset = right - left;
	auto beg = list.begin();
	auto temp_h = list.head;
	while (beg++ != left)
		temp_h = temp_h->nextptr;
	auto temp_t = temp_h;
	while (offset-- > 0)
		temp_t = temp_t->nextptr;
	if (temp_h == list.head && temp_t == list.virtual_tail)
	{
		if (empty())
		{
			head = temp_h;
			tail = temp_t->preptr;
			tail->nextptr = virtual_tail;
			virtual_tail->preptr = tail;
			size = list.size;
		}
		else
		{
			while (temp_h->nextptr)
			{
				(*this).insert(target_pos, temp_h->data);
				temp_h = temp_h->nextptr;
			}
		}
		std::cout << "test" << std::endl;
		list.head = list.tail = nullptr;
		list.virtual_tail = new ListNode<T>(T());
		list.size = 0;
		return;
	}
	else
	{
		if (temp_h == list.head)
		{
			list.head = temp_t;
			list.head->preptr = nullptr;
		}
		if (temp_t == list.virtual_tail)
		{
			list.virtual_tail->preptr = temp_h->preptr;
			temp_h->preptr->nextptr = list.virtual_tail;
		}
		list.size = list.size - offset;
		if (empty())
		{
			head = temp_h;
			tail = temp_t->preptr;
			tail->nextptr = virtual_tail;
			virtual_tail->preptr = tail;
			size = list.size;
		}
		else
		{
			while (temp_h->nextptr)
			{
				(*this).insert(target_pos, temp_h->data);
				temp_h = temp_h->nextptr;
			}
		}
		return;
	}
	



	

}

template<typename T>
MyList<T>& MyList<T>::sort() //Bubble sort
{
	MyList<T> newlist;
	auto beg = begin();
	for (int i = 0; i < size - 1; ++i)
	{
		for (int j = 0; j < (size - i - 1); ++j)
		{
			if ((*this)[j] > (*this)[j + 1])
			{
				T temp = (*this)[j + 1];
				(*this)[j + 1] = (*this)[j];
				(*this)[j] = temp;
			}
		}
	}
	return *this;
}

template<typename T>
MyList<T>& MyList<T>::merge(MyList<T>& list1, MyList<T>& list2)
{
	auto beg1 = list1.begin(), beg2 = list2.begin();
	
	while (beg1 != list1.end() && beg2 != list2.end())
	{
		if (*beg1 < *beg2)
			(*this).push_back(*beg1++);
	
		else
			(*this).push_back(*beg2++);
		
	}
	while(beg1 != list1.end())
		(*this).push_back(*beg1++);

	while(beg2 != list2.end())
		(*this).push_back(*beg2++);

	
	return *this;
}

template<typename T>
MyList<T>& MyList<T>::mergesort(MyList<T>& list)
{
	
	

}

template<typename T>
void MyList<T>::reverse()
{
	auto cur_ptr = head->nextptr;
	head->nextptr = virtual_tail;
	head->preptr = cur_ptr; 
	virtual_tail->preptr = head; 
	tail->nextptr = nullptr;


	while (cur_ptr)
	{
		auto tmp = cur_ptr->nextptr;
		cur_ptr->nextptr = head;
		cur_ptr->preptr = tmp;
		head = head->preptr;
		cur_ptr = tmp;
	}

	tail = virtual_tail->preptr;
	head->preptr = nullptr;

}


template<typename T>
void MyList<T>::print() const
{
	if (size == 0)
	{
		std::cout << "The list is empty." << std::endl;
		return;
	}
	auto tmp = head;
	std::cout << "The list is: " << std::flush;
	while (tmp->nextptr)
	{
		std::cout << tmp->data << " " << std::flush;
		tmp = tmp->nextptr;
	}
	std::cout << std::endl;
}


int main()
{


	MyList<int> _list, _list2, _list3, _list4;
	_list.push_back(15);
	_list.push_back(24);
	_list.push_back(-53);
	_list.push_back(9);
	_list.push_back(910);
	_list.sort();


	_list2.push_back(10);
	_list2.push_back(20);
	_list2.push_back(30);
	_list2.push_back(40);

	auto beg = _list.begin();
	auto left = beg.next(beg, 0);
	auto right = beg.next(beg, 2);

	auto target = _list2.begin();
	_list.print();
	_list2.print();
	//std::cout << *beg << " " << *left << " " << *right << std::endl;
	_list2.splice(target, _list, left, right);

	_list.print();
	_list2.print();
	
	


	return 0;
}
