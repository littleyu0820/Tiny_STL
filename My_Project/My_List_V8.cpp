#include <iostream>
#include <algorithm>
#include <memory>
#include <cassert>


//v8


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
	class List_Iterator;

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

	//Construct by Iterator
	MyList(List_Iterator b, List_Iterator e) :
		virtual_tail(new ListNode<T>(T())), head(nullptr), tail(nullptr), size(0)
	{
		std::cout << "Iterator Constructor..." << std::endl;
		while (b != e)
			push_back(*b++);
	}


	MyList(const MyList& list) : virtual_tail(new ListNode<T>(T())), head(nullptr), tail(nullptr), size(0)
	{
		std::cout << "Copy Constructor..." << std::endl;
		auto temp = list.head;
		while (temp->nextptr)
		{
			push_back(temp->data);
			temp = temp->nextptr;
		}
	}

	MyList& operator=(const MyList& list)
	{
		std::cout << "Assign by copying..." << std::endl;
		if (this != &list)
		{
			if (!empty())
				clear();
	
			auto temp = list;
			while (temp->nextptr)
			{
				push_back(temp->data);
				temp = temp->nextptr;
			}
		}

		return *this;
	}

	//Move-Constructor
	MyList(MyList&& list) noexcept :
		head(list.head), tail(list.tail), size(list.size)
	{
		std::cout << "Move Cosntructor..." << std::endl;
		if (list.empty())
			virtual_tail = new ListNode<T>(T());
		else
		{
			virtual_tail = list.virtual_tail;
			list.virtual_tail = nullptr;
		}

		list.virtual_tail = list.head = list.tail = nullptr;
		list.size = 0;
	}
	//Move-Assing and it will call Move-Constructor
	MyList& operator=(MyList&& list) noexcept
	{
		std::cout << "Assign by moving..." << std::endl;
		std::cout << "Moving head: " << list.head->data << std::endl;
		std::cout << "Moving tail: " << list.tail->data << std::endl;
		std::cout << "Moving virtual_tail: " << list.virtual_tail->data << std::endl;
		std::cout << "Moving size: " << list.size << std::endl;

		if (this != &list)
		{
			if(!empty())
				clear();
			head = list.head;
			tail = list.tail;
			size = list.size;
			virtual_tail = list.virtual_tail;
			list.virtual_tail = list.head = list.tail = nullptr;
			list.size = 0;
		}
		return *this;
	}

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
			//used to operate two iterators if we need to manipulate their distance
			//please make sure that rhs is greater than lhs
		{
			auto lhs = *this;
			assert(lhs->preptr && "Error: Illegal iterator!");
			size_t offset = 0;
			while (lhs.it_ptr->preptr)
			{
				if (rhs.it_ptr != lhs.it_ptr)
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


	T& operator[](size_t index) //used to access the element by index and we need to memorize that,
								//the data store in list is not actually constant, they're connected by pointer, in fact
	{
		assert(index >= 0 && index < size && "Error: Out of range!"); //make the rule of the range
		if (index == 0) //the first data
			return head->data;
		else if (index == size - 1) //the last data
			return tail->data;
		else
		{
			auto tmp = head;
			while (index != 0) //move the pointer to access the target
			{
				tmp = tmp->nextptr;
				--index;
			}
			return tmp->data;
		}

	}

	List_Iterator begin() const { return List_Iterator(head); }
	List_Iterator end() const { return List_Iterator(virtual_tail); }

	void insert(const List_Iterator&, const T&); //used to insert the element before the target position, and the size of the list will alse be increased by 1
	void erase(const List_Iterator&); //used to erase the element at target positon, and the size of the list will alse be decreased by 1
	void unique(); //used to find the repeat element and erase them
	void resize(int); //as its name resize, if the parameter is greater than the old size, we'll increase it by the default element

	void swap(MyList<T>&, MyList<T>&); //swap 2 lists

	void reverse(); //reverse the list, head become tail and tail become head, next become pre, pre become next

	MyList<T>& sort(); //bubble-sort
	MyList<T> merge(MyList<T>&, MyList<T>& list); //merge two sorted list
	MyList<T> mergesort(MyList<T>&); //merge-sort

	void slice(size_t, size_t, MyList<T>&); //"copy" the data from the input range into the other list
	void splice(List_Iterator&, MyList<T>&, List_Iterator&, List_Iterator&); //"move" the data from the input range into the other list

	T& front()
	{
		assert(!empty() && "Error: Attempt to access an element from an empty data!");
		return head->data;
	}

	T& back()
	{
		assert(!empty() && "Error: Attempt to access an element from an empty data!");
		return tail->data;
	}
	
	void push_front(const T&); //use to add the element at the head
	void pop_front(); //use to drop the element from head
	void push_back(const T&); //use to add the element at the tail
	void pop_back(); //use to drop the element from tail
	size_t get_size() const; 
	bool empty();

	void clear()
	{

		assert(!empty() && "Error: Clearing an empty list!");
		std::cout << "Clearing the list..." << std::endl;
		while (head) //if the list is not empty
		{
			auto tmp = head; //point to head, gonna be used to delete
			head = head->nextptr; //move to next node
			delete tmp; //delete the element from head, since tmp is a pointer point to head, 
						//so when we manipulate it, it as same as that we manipulate the object what it points
		}

		//like the constructor, we need to make them be the initial one, that is just be constructed
		tail = nullptr; 
		virtual_tail = new ListNode<T>(T());
		size = 0;
	}
	void print() const;

private:

	ListNode<T>* head;
	ListNode<T>* tail;
	ListNode<T>* virtual_tail;
	size_t size;

	void free() //used to destruct
	{
		while (head->nextptr) //if the next node is not exit, it means that it is the end(virtual tail), we need to delete it later
		{
			ListNode<T>* tmp = head;
			head = head->nextptr;
			std::cout << "Deleting " << tmp->data << "..." << std::endl;
			delete tmp;
		}
		delete virtual_tail;
	}

};

template<typename T>
void MyList<T>::resize(int newsize)
{
	assert(newsize >= 0 && newsize != size && "Error: Invalid size!");
	if (newsize == 0)
		clear();
	else if (newsize > size)
	{
		auto offset = newsize - size;
		while (offset-- > 0)
			push_back(T());
	}
	else
	{
		auto offset = size - newsize;
		while (offset-- > 0)
			pop_back();
	}
}


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
void MyList<T>::swap(MyList<T>& lhs, MyList<T>& rhs) //normal swap method
{
	auto temp_h = lhs.head, temp_t = lhs.tail, temp_vt = lhs.virtual_tail;
	size_t temp_sz = lhs.size;
	lhs.head = rhs.head;
	lhs.tail = rhs.tail;
	lhs.virtual_tail = rhs.virtual_tail;
	lhs.size = rhs.size;

	rhs.head = temp_h;
	rhs.tail = temp_t;
	rhs.virtual_tail = temp_vt;
	rhs.size = temp_sz;
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
		while (beg != pos) //move the position
		{
			beg = beg + 1;
		}

		//for instance, 0 13 5 8 9 11, and we're going to insert the element before 8
		//so now, beg is 8
		auto newnode = new ListNode<T>(val); //create the node
		auto temp = beg->preptr; //0 13 (5)-temp------->newnode------->(8)-beg
								// 0 13 (5)-temp<-------newnode<-------(8)-beg

		newnode->nextptr = temp->nextptr; //beg
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
		while (beg != pos) //move to the target positon
		{
			beg = beg + 1;
			++ctr;
		}
		assert(ctr < size && "Error: Out of range!");

		//for instance, 0 13 5 8 9 11, and we're going to erase the element 8
		//beg is 8
		//0 13 (5)------->(8)-beg--------->(9)temp step1 
		//0 13 (5)------->(8)-begxx step4
		//0 13 (5)------------------------->(9)temp step2
		//0 13 (5)<-------------------------(9)temp step3
		auto temp = beg->nextptr; //store the node what the target point to
		beg->preptr->nextptr = temp;
		temp->preptr = beg->preptr;
		beg = nullptr;
		--size;
	}
}

template<typename T>
void MyList<T>::unique()
{
	assert(!empty() && "Error: Attempt to manipulate an empty list!");
	auto slow_ptr = head, fast_ptr = head->nextptr; //fast-and-slow pointers
	auto start = begin() + 1; //the positon we're going to erase the element
	auto ctr = 0;
	while (fast_ptr->nextptr) //if there are 2 more nodes in the list
	{
		std::cout << slow_ptr->data << "  " << fast_ptr->data << std::endl;
		if (slow_ptr->data == fast_ptr->data) //if same
		{
			auto temp = slow_ptr->data; //store the data to compare
			//std::cout << temp << std::endl;
			erase(start); //erase
			std::cout << slow_ptr->data << " 222 " << fast_ptr->data << std::endl;
			start = start + 1; //move the target
			fast_ptr = fast_ptr->nextptr; //move 1 step
			std::cout << slow_ptr->data << " 333 " << fast_ptr->data << std::endl;
			while (fast_ptr && temp == fast_ptr->data) //if still same
			{
				erase(start);
				start = start + 1;
				fast_ptr = fast_ptr->nextptr;
				++ctr;
			}
			
		
			fast_ptr = fast_ptr->nextptr;
			slow_ptr = slow_ptr->nextptr;
			start = start + 1;
	
		
		}
		else
		{
			fast_ptr = fast_ptr->nextptr;
			slow_ptr = slow_ptr->nextptr;
			
			start = start + 1;
		}
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
	if (size == 1)
	{
		clear();
		return;
	}
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
	for (size_t index = 0; index < size; ++index)
	{
		if (index >= left && index < right)
			newlist.push_back((*this)[index]);
	}

}


template<typename T>
void MyList<T>::splice(List_Iterator& target_pos, MyList<T>& list, List_Iterator& left, List_Iterator& right)
{
	auto offset = right - left; //the range
	auto beg = list.begin();
	auto temp_h = list.head;
	while (beg++ != left)
		temp_h = temp_h->nextptr; //move the pointer to find the new head
	auto temp_t = temp_h; 
	while (offset-- > 0)
		temp_t = temp_t->nextptr; //move the pointer to find the new tail
	if (temp_h == list.head && temp_t == list.virtual_tail) //copy the whole list 
	{
		if (empty()) //if the new list is empty just copy
		{
			head = temp_h;
			tail = temp_t->preptr;
			tail->nextptr = virtual_tail;
			virtual_tail->preptr = tail;
			size = list.size;
		}
		else
		{
			while (temp_h->nextptr) //insert
			{
				(*this).insert(target_pos, temp_h->data);
				temp_h = temp_h->nextptr;
			}
		}
		//clear the list
		list.head = list.tail = nullptr;
		list.virtual_tail = new ListNode<T>(T());
		list.size = 0;
		return;
	}
	else
	{
		if (temp_h == list.head) //head
		{
			list.head = temp_t; //since it doesn't include the right node so the new head is temp_t
			list.head->preptr = nullptr; //break the link
		}
		if (temp_t == list.virtual_tail) 
		{
			list.virtual_tail->preptr = temp_h->preptr; //link to the node before the temp_h
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
MyList<T> MyList<T>::merge(MyList<T>& list1, MyList<T>& list2)
{

	MyList<T> result;
	while (!list1.empty() && !list2.empty())
	{
		if (list1.front() < list2.front())
		{
			result.push_back(list1.front());
			list1.pop_front();

		}
		else
		{
			result.push_back(list2.front());
			list2.pop_front();
		}
	}

	while(!list1.empty())
	{
		result.push_back(list1.front());
		list1.pop_front();
	}
	while(!list2.empty())
	{
		result.push_back(list2.front());
		list2.pop_front();
	}
	
	/*result.print();
	auto tmp = result.head;
	size_t sz = result.get_size();
	while (sz-- > 0)
	{
		std::cout << tmp->data << std::endl;
		tmp = tmp->nextptr;
	}*/
	
	return result;
}

template<typename T>
MyList<T> MyList<T>::mergesort(MyList<T>& list)
{
	if (list.size <= 1)
		return list;

	// 1 41 35 9 15 51 41
	auto slow_ptr = list.begin(), fast_ptr = list.begin();
	while (fast_ptr != list.end() && fast_ptr.next(fast_ptr, 1) != list.end())
	{
		++slow_ptr;
		fast_ptr = fast_ptr.next(fast_ptr, 2);
	} //slow 9 fast 41
	
	MyList<T> left_side(list.begin(), slow_ptr);
	MyList<T> right_side(slow_ptr, list.end());
	
	left_side = mergesort(left_side);
	right_side = mergesort(right_side);

	return merge(left_side, right_side);

}

template<typename T>
void MyList<T>::reverse()
{
	auto cur_ptr = head->nextptr; //store the node aftet the head
	head->nextptr = virtual_tail;
	head->preptr = cur_ptr; //change the direction
	virtual_tail->preptr = head;
	tail->nextptr = nullptr; //break


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


	MyList<int> _list, _list2;
	_list.push_back(1);
	_list.push_back(3);
	_list.push_back(8);
	_list.push_back(15);
	_list.push_back(11);
	_list.push_back(11);
	_list.push_back(33);
	_list = _list.mergesort(_list);



	_list.print();
	_list.unique();
	_list.print();

	

	return 0;
}
