#include <iostream>
#include <algorithm>
#include <memory>
#include <cassert>


//V4
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
		virtual_tail(list.virtual_tail), head(list.head), tail(list.tail), size(list.size)
	{
		std::cout << "Move Cosntructor..." << std::endl;
		list.virtual_tail = list.head = list.tail = nullptr;
	}
	//Move-Assing and it will call Move-Constructor
	MyList& operator=(MyList&& list) noexcept = default;

	//Destructor
	~MyList() noexcept
	{
		if(head)
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
		List_Iterator() : it_ptr(nullptr){}
		List_Iterator(ListNode<T>* node) :
			it_ptr(node) {}

		T& operator*()
		{
			assert(it_ptr && it_ptr->nextptr && "Error: Dereferencing an illegal iterator!");
			return it_ptr->data;
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
			auto tmp = *this;
			it_ptr = it_ptr->preptr;
			return tmp;
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


	List_Iterator begin() const { return List_Iterator(head); }
	List_Iterator end()
	{ 
		return List_Iterator(virtual_tail);
	}

	void push_front(const T&);
	void pop_front();
	void push_back(const T&); //use to add the element at the tail
	void pop_back();
	size_t get_size();
	bool empty();

	void clear()
	{
		
		assert(!empty() && "Error: Clearing an empty list!");
		std::cout << "Clearing the list..." << std::endl;
		head = tail = virtual_tail = nullptr;
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


};


template<typename T>
size_t MyList<T>::get_size()
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

	tail->nextptr = virtual_tail;
	virtual_tail->preptr = tail;
	--size;
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
	

	

	return 0;
}
