# Tiny_STL
### Create a tiny STL in C++11/14 for myself.

# Overview
### This is a project based on C++11/14, I'm gonna to make a tiny STL(Include [Vector](https://github.com/littleyu0820/Tiny_STL/blob/main/README.md#completed-functions-for-vector), [List](https://github.com/littleyu0820/Tiny_STL/blob/main/README.md#completed-functions-for-list), [BRT](), [Map](https://github.com/littleyu0820/Tiny_STL/blob/main/README.md#completed-functions-for-map)). Keep updating until complete the functions.
### If you don't really understand the how does the function do or its principle, I comment most of the functions. After that if you still don't understand feel free to ask me. [here](https://www.youtube.com/@%E6%B5%81%E9%9B%B2%E7%9A%84%E7%A8%8B%E5%BC%8F%E7%AD%86%E8%A8%98)


# Language
* ### C++ [here](https://zh.wikipedia.org/zh-tw/C%2B%2B)

# System
* ### Windows 10/11, Linux

# Compiler
* ### Visual Studio 2022 [here](https://visualstudio.microsoft.com/zh-hant/downloads/)


# Completed Functions for Vector
* ### Create Constructor(Rule Of Five)
* ### Create push_back() and pop_back()
* ### Create realloacate()
* ### Create empty() and clear()
* ### Create free()
* ### Create inset() and erase()
* ### Create index search
* ### Create at()
* ### Create Vector Comparing( > , == , != , <)
* ### Create resize(), shrink_to_fit() and reserve()
### ---------------------------------------------
* ### Support Iterator
* ### Support Copy-and-Swap
* ### Support Dynamic Allocate Memory



# Details for Important Functions in Vector
## 1. Default Constructor:
```c++
MyVector() : the_first(nullptr), size(nullptr), capacity(nullptr)
{
	std::cout << "Constructing Vector..." << std::endl;
} //constructor and there is no memory(data) in the container now

MyVector(std::initializer_list<T> il)
{
	auto new_data = copy_alloc(il.begin(), il.end());
	the_first = new_data.first;
	size = capacity = new_data.second;
	new_data.first = nullptr;
	new_data.second = nullptr;
	std::cout << "Copying Vector..." << std::endl;
} //constructor but this is used for initializer_list the implementation is like std::vector<int> vec = {0,1,2,3}
```
## 2. Copy Constructor:
```c++
MyVector(const MyVector& v)
{
	auto new_data = copy_alloc(v.the_first, v.size);
	the_first = new_data.first;
	size = capacity = new_data.second;
	new_data.first = nullptr;
	new_data.second = nullptr;
	std::cout << "Copying Vector..." << std::endl;
} //constructor for copying the implementation is like std::vector<int> vec2(vec) that vec is also a vector<int>
```
## 3. Move Constructor:
```c++
MyVector(MyVector&& v) noexcept : //MyVector&& v: rvalue reference means that the parameter should be rvalue
	the_first(v.the_first), size(v.size), capacity(v.capacity)
{
	v.the_first = v.size = v.capacity = nullptr;
	std::cout << "Moving Vector..." << std::endl;
} //constructor for moving that we can move the data not copy it
```
## 4. Reallocate:
```c++
void reallocate(const size_t new_capacity)
{
	auto first = alloc.allocate(new_capacity); //how much space the new container needs and return the positon where the first element at
	auto last = first;
	try
	{
		last = std::uninitialized_copy(std::make_move_iterator(the_first), std::make_move_iterator(size), first);
	}
	catch (...) //chech there are any errors or not
	{
		while (last != first) //free the memory if we used
			alloc.destroy(--last);
		alloc.deallocate(first, new_capacity); //destructor
		throw;
	}
	//copy the old value(data) into to the new container and return the positon where is after the last element or we call it end
	free(); //free the memory space where the old container is
	the_first = first; //reassign
	size = last; //reassign
	capacity = the_first + new_capacity; //reassign
}
```
## 5. uninitialized_copy(b,e,data): Copy the element from b to e into data.
## 6. Remove Elemnet:
```c++
void pop_back() //remove the last element
{
	if (empty())
	{
		std::cerr << "Error: in function " << __func__ << " at line " << __LINE__
			<< " Compiled on " << __DATE__ << " at " << __TIME__ << std::endl
			<< "The container is empty." << std::endl;
		exit(1);
	}
	alloc.destroy(--size);
}
```
```c++
Iterator erase(Iterator& it) //used to erase the specific element
{
	int ctr = 0;
	while (it-- != begin())
		++ctr;
	if (ctr >= get_size())
	{
		std::cerr << "Error: in function " << __func__ << " at line " << __LINE__
			<< " Compiled on " << __DATE__ << " at " << __TIME__ << std::endl
			<< "The iterator is out of range." << std::endl;
		return it;
	}
	else
	{
		auto deleted = the_first + ctr;
		auto temp = deleted;
		while (deleted != size)
		{
			*(deleted) = *(deleted + 1);
			++deleted;
		}
		size -= 1;
		return Iterator(temp);

	}
}
```
## 7. destroy(p): Remove the element from position p but we need to know the memory is not freed.
## 8. Add Element:
```c++
void push_back(const T& value) //bind to lvalue we construct by copying
//we'll have two push_back function for the lvalue and the rvalue
//bind to lvalue we construct by copying
void push_back(const T& value)
{
	if (get_size() == get_capacity()) //only when we insert the element that the capacity will not be enough
	{
		reallocate(get_capacity() == 0 ? 1 : get_capacity() * 2);
	}
	alloc.construct(size++, value);
	//construct the container(or we say we construct the value at the positon) 
	//and we need to realize that the_first and size are all at 0 or we say they all point to null at first
	//so that we can just copy the value into the position where the size was at and after that we move it
}

/*
* the example for lvaue
* std::string str = "hello";
* vec.push_back(str); //lvalue call const T&
*/

//bind to rvalue and we construct by moving
void push_back(T&& value) //bind to rvalue so that we can construct by moving	
{
	if (get_size() == get_capacity()) //only when we insert the element that the capacity will not be enough
	{
		reallocate(get_capacity() == 0 ? 1 : get_capacity() * 2);
	}
	alloc.construct(size++, std::move(value));
	//in fact, we can't just construct the container(or we say we construct the value at the positon) by rvalue 
	//so we need to use std::move to do that
}

/*
* the exaple for rvalue
* vec.push_back(std::string("wrold")); //rvalue call T&&
* vec.push_back(std::move(str)); //rvalue call T&& because std::move convert the str(lvalue) into rvalue
*/
```
```c++
Iterator insert(Iterator& it, const T& val)
{

	int ctr = 0;
	while (it-- != begin()) //used to record how many steps we need to move
		++ctr;
	if (ctr > get_size()) //check whether the iterator(position) is legal or not
	{
		std::cerr << "Error: in function " << __func__ << " at line " << __LINE__
			<< " Compiled on " << __DATE__ << " at " << __TIME__ << std::endl
			<< "The iterator is out of range." << std::endl;
		return begin();
	}

	size_t reserved_space = get_capacity() - get_size(); //how much space we can use

	if (reserved_space <= ctr) //if the space is not enough reallocate
	{
		reallocate(get_capacity() == 0 ? 1 : get_capacity() * 2);
	}

	auto start = begin() + ctr; //from here to insert
	auto tmp = *start;
	auto next_tmp = *(start + 1);
	*start = val;
	while (start != end())
	{
		*(++start) = tmp;
		tmp = next_tmp;
		next_tmp = *(start + 1);
	}
	size += 1;
	return begin() + ctr;
}

Iterator insert(Iterator&& it, T&& val)
{
	int ctr = 0;
	while (it-- != begin())
		++ctr;

	if (ctr > get_size())
	{
		std::cerr << "Error: in function " << __func__ << " at line " << __LINE__
			<< " Compiled on " << __DATE__ << " at " << __TIME__ << std::endl
			<< "The iterator is out of range." << std::endl;
		return begin();
	}

	size_t check_space = get_capacity() - get_size();

	if (check_space <= ctr)
	{
		reallocate(get_capacity() == 0 ? 1 : get_capacity() * 2);
	}

	auto start = begin() + ctr;
	auto tmp = *start;
	auto next_tmp = *(start + 1);
	*start = val;
	while (start != end())
	{
		*(++start) = tmp;
		tmp = next_tmp;
		next_tmp = *(start + 1);
	}

	size += 1;
	return begin() + ctr;
}
```
## 9. (Memory)Space Management:
```c++
void resize(size_t input) //modify the size
{
	if (get_size() > input) //check the input is greater than the old size or not
	{
		while (get_size() != input)
			alloc.destroy(--size);
	}
	else if (get_size() < input)
	{
		reserve(input);
		while (get_size() != input)
			push_back(T());
	}
	else
	{
		std::cerr << "Warning: in function " << __func__ << " at line " << __LINE__
			<< " Compiled on " << __DATE__ << " at " << __TIME__ << std::endl
			<< "The input size you want to modify is equal to the old one." << std::endl;
		return;
	}
}
void shrink_to_fit() //modify the capacity to fit size
{
	if (get_capacity() > 2 * get_size())
		reallocate(get_size());
	else
	{
		std::cerr << "Warning: in function " << __func__ << " at line " << __LINE__
			<< " Compiled on " << __DATE__ << " at " << __TIME__ << std::endl
			<< "The capacity doesn't need to be reallocated now." << std::endl;
		return;
	}
}
```
```c++
void resize(size_t input, const T& element) //modify the size
{
	if (get_size() > input) //check the input is greater than the old size or not
	{
		std::cerr << "Warning: in function " << __func__ << " at line " << __LINE__
			<< " Compiled on " << __DATE__ << " at " << __TIME__ << std::endl
			<< "The input size you want to modify is equal to or less than the old one." << std::endl;
		return;
	}
	else
	{
		reserve(input);
		while (get_size() != input)
			push_back(element);
	}

}
void resize(size_t input, T&& element) //modify the size
{
	if (get_size() >= input) //check the input is greater than the old size or not
	{
		std::cerr << "Warning: in function " << __func__ << " at line " << __LINE__
			<< " Compiled on " << __DATE__ << " at " << __TIME__ << std::endl
			<< "The input size you want to modify is equal to or less than the old one." << std::endl;
		return;
	}
	else
	{
		reserve(input);
		while (get_size() != input)
			push_back(element);
	}
}
```
```c++
	void reserve(size_t input) //give the container enough space to store
	{
		if (get_capacity() >= input)
		{
			std::cerr << "Warning: in function " << __func__ << " at line " << __LINE__
				<< " Compiled on " << __DATE__ << " at " << __TIME__ << std::endl
				<< "The input is less tha the capacity now." << std::endl;
			return;
		}
		else
			reallocate(input);
	}
```
```c++
	void free()
	{
		if (the_first) //which means that the_first is not point to null
		{
			for (auto pe = size; pe != the_first;) //delete the element from the last element
				alloc.destroy(--pe);
		}
		alloc.deallocate(the_first, capacity - the_first); //free the memory for the whole container
	}
```
```c++
//the function used to copy the vector
std::pair<T*, T*> copy_alloc(const T* b, const T* e)
{
	auto new_one = alloc.allocate(e - b); //allocate the memory space whice size is from b to e
	return { new_one, std::uninitialized_copy(b, e, new_one) }; //copy the value(data) from b to e
}
//return a pair of 2 pointers which is the_first point to the first element 
//and the other is size point to the position where is after the last element
```
## 10. Iterator:
```c++
	class Iterator //the important thing is that we can see Iterator as a pointer
	{
	public:
		Iterator() : t_ptr(nullptr) {} //constructor with no pointer
		Iterator(T* ptr) : t_ptr(ptr) {} //constructor

		T& operator*() //we can use * to dereference to get the value where iterator point to
		{
			return *t_ptr;
		}

		T* operator->() //used to access member
		{
			return t_ptr;
		}

		T& operator[](std::ptrdiff_t pos) const
		{
			//if (pos >= (*this).get_size())
			//{
			//	std::cerr << "Error: in function " << __func__ << " at line " << __LINE__
			//		<< " Compiled on " << __DATE__ << " at " << __TIME__ << std::endl
			//		<< "The index is out of range." << std::endl;
			//	return *t_ptr;
			//}
			return *(t_ptr + pos);
		}

		//the important thing is that "this" is a pointer and "*this" means that we dereference pointer "this" to get the object (or we say value)
		//and here "*this" is an object which type is Iterator and "this" is a pointer point to "T"
		Iterator operator++()
		{
			++t_ptr;
			return *this;
		}
		Iterator operator++(int)
		{
			Iterator tmp = *this;
			++(*this);
			return tmp;
		}
		Iterator operator--()
		{
			--t_ptr;
			return *this;
		}
		Iterator operator--(int)
		{
			Iterator tmp = *this;
			--(*this);
			return tmp;
		}

		//use const to make sure that *this will not be modified which means that the lhs object will not be modified
		//e.g. if we define beg is an iterator which point to the first element for the container 
		//and we call beg+1 so that const will make sure that this will not be modified
		//why should we make it be unmodified, be const since that we just move the pointer not modify the container, 
		//if we modify the begin it means that the container will also be modify because the begin is not at the same positon
		Iterator operator+(std::ptrdiff_t pos) const&
		{
			Iterator tmp = *this;
			tmp.t_ptr += pos;
			return tmp;
		}
		Iterator operator-(std::ptrdiff_t pos) const&
		{
			Iterator tmp = *this;
			tmp.t_ptr -= pos;
			return tmp;
		}

		//the function to compare 2 iterator(pointers) equal or not equal
		//if we call beg == end; which beg is the iterator point to the first element in the container 
		//and end is the iterator point to the position where is after the last element
		//this function will be implemented like, beg.operator==(end)
		//and move into the function body the t_ptr is for beg(since it's the object who calls this function) and rhs.t_ptr as its name is for end
		//so that, we can get true or false up to they are at the same positon or not
		bool operator==(const Iterator& rhs) const
		{
			return t_ptr == rhs.t_ptr;
		}

		bool operator!=(const Iterator& rhs) const
		{
			return !(*this == rhs);
		}

		//the main member for Iterator or we can just say the box(Iterator) is made to store the pointer t_ptr
	private:
		T* t_ptr;
	};
```

# Completed Functions for List
* ### Create Constructor(Rule of Five)
* ### Create front() and back()
* ### Create push_back() and pop_back()
* ### Create push_front() and pop_front()
* ### Create get_size() and reszie()
* ### Create insert() and erase()
* ### Create sort() and merge()
* ### Create splice()
* ### Create unique()
* ### Create swap() and reverse()
* ### Create empty()
* ### Create clear()
* ### Create print()  
### ---------------------------------------------
* ### Support Iterator
* ### Support Merge-Sort
* ### Support Slice

# Details for Important Functions in Vector
## 1. Default Constructor:
### Used to create the node.
```c++
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
```
### Used to create the list.
```c++
	MyList() : virtual_tail(new ListNode<T>(T())), head(nullptr), tail(nullptr), size(0)
	{
		std::cout << "Default Constructor..." << std::endl;
	}
```
```c++
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
```
```c++
	//Construct by Iterator
	MyList(List_Iterator b, List_Iterator e) :
		virtual_tail(new ListNode<T>(T())), head(nullptr), tail(nullptr), size(0)
	{
		std::cout << "Iterator Constructor..." << std::endl;
		while (b != e)
			push_back(*b++);
	}
```
```c++
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
```
```c++
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
```
```c++
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
```
```c++
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
			if (!empty())
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
```

```c++
	//Destructor
	~MyList() noexcept
	{
		if (head)
		{
			std::cout << "Destructor..." << std::endl;
			free();
		}
	}
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
```
## 2. Unique:
### Used to remove the repeat element.
```c++
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
```
## 3. Slice:
### Used to copy the element in a limited range.
```c++
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
```
## 4. Splice:
### Used to cut the element in a limited range.
### Note: We cut the list, so the old one will be modified.
```c++
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

```
## 5. Bubble Sort:
```c++
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
```
## 6. Merge:
### Used to combie two sorted list.
```c++
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

	while (!list1.empty())
	{
		result.push_back(list1.front());
		list1.pop_front();
	}
	while (!list2.empty())
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

```
## 7. 🌟🌟🌟Merge-Sort:
```c++
template<typename T>
MyList<T> MyList<T>::mergesort(MyList<T>& list)
{
	if (list.size <= 1)
		return list;

	auto slow_ptr = list.begin(), fast_ptr = list.begin();
	while (fast_ptr != list.end() && fast_ptr.next(fast_ptr, 1) != list.end())
	{
		++slow_ptr;
		fast_ptr = fast_ptr.next(fast_ptr, 2);
	} 

	MyList<T> left_side(list.begin(), slow_ptr);
	MyList<T> right_side(slow_ptr, list.end());

	left_side = mergesort(left_side);
	right_side = mergesort(right_side);

	return merge(left_side, right_side);

}
```
## 8. Reverse:
```c++
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
```

# Completed Functions for Map
* ### Create Constructor(Rule Of Five)
* ### Create push_back() and pop_back()
* ### Create realloacate()
* ### Create empty() and clear()
* ### Create free()
* ### Create inset() and erase()
* ### Create index search
* ### Create at()
* ### Create Vector Comparing( > , == , != , <)
* ### Create resize(), shrink_to_fit() and reserve()
### ---------------------------------------------
* ### Support Iterator
* ### Support Copy-and-Swap
* ### Support Dynamic Allocate Memory
