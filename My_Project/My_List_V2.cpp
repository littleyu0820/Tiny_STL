#include <iostream>
#include <algorithm>
#include <memory>

//V2

int print_blocks();
void Marks();


template<typename T>
struct ListNode //the struct of the node
{
	T data; //store the element in the node
	ListNode<T>* preptr; //the normal pointer to help us access the last node
	std::unique_ptr<ListNode<T>> nextptr;
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
	MyList() : head(nullptr), tail(nullptr), size(0) 
	{
		std::cout << "Default Constructor..." << std::endl;
	}

	//Direct-Initialization
	MyList(std::initializer_list<T> il) : 
		head(nullptr), tail(nullptr), size(0)
	{
		auto beg = il.begin();
		while (beg != il.end())
		{
			this->push_back(*beg++);
		}

	}

	//No Copy-Constructor since we use unique pointer
	MyList(const MyList&) = delete;
	MyList& operator=(const MyList&) = delete;

	//Move-Constructor
	MyList(MyList&& list) noexcept
	{
		std::cout << "Moving Constructor..." << std::endl;
		this->head = std::move(list.head);
		this->tail = list.tail;
		this->size = list.size;
		list.tail = nullptr;
		list.size = 0;
	}
	//Move-Assing and it will call Move-Constructor
	MyList& operator=(MyList&& list) noexcept = default;

	//Destructor
	~MyList() 
	{
		std::cout << "Destructor..." << std::endl;
	}


	void print() const;
	void push_front(const T&);
	void pop_front();
	void push_back(const T&); //use to add the element at the tail
	void pop_back();

	bool empty();


private:

	std::unique_ptr<ListNode<T>> head;
	//the unique_ptr point to the head which means that this pointer(head) will have the possession of the whole list
	ListNode<T>* tail; //the normal pointer point to the tail which can help us access the last node
	size_t size;
};

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
	auto newnode = std::make_unique<ListNode<T>>(val);
	//create a dynamic object which is type(ListNode<T>) and be constructed by val, we can see it as "ListNode x(10);"
	//and the unique pointer newnode point to it(x)

	if (!head) //if there are no nodes in the list
	{
		head = std::move(newnode); //since unique_ptr can no be copied; thus, we use move to transfer the possession
		tail = head.get(); //get() is a function that can return the normal pointer points to the ListNode
	}
	else
	{
		head->preptr = newnode.get();
		newnode->nextptr.reset(head.release()); 
		//release the possession for the old head so that the newnode can point and now head point to nothing
		head = std::move(newnode); //make head point to the newnode and it has the possession
	}
	++size;
}

template<typename T>
void MyList<T>::pop_front()
{
	if (empty()) //if there are no nodes in the list
	{
		std::cerr << "Error: in function " << __func__ << " at line " << __LINE__
			<< " Compiled on " << __DATE__ << " at " << __TIME__ << std::endl
			<< "The list is empty." << std::endl;
		return;
	}
	else
	{
		if(size == 1)
		{
			head.reset();
			tail = nullptr;
		}
		else 
			head.reset(head->nextptr.release()); //destroy the node(head) and release the possession for the next node
	}
	--size;
}

template<typename T>
void MyList<T>::push_back(const T& val)
{
	auto newnode = std::make_unique<ListNode<T>>(val);
	//create a dynamic object which is type(ListNode<T>) and be constructed by val, we can see it as "ListNode x(10);"
	//and the unique pointer newnode point to it(x)

	if (!head) //if there are no nodes in the list
	{
		head = std::move(newnode); //since unique_ptr can no be copied; thus, we use move to transfer the possession
		tail = head.get(); //get() is a function that can return the normal pointer points to the ListNode
	}
	else
	{
		tail->nextptr = std::move(newnode);
		//tail is a normal pointer points to the listnode(node 1) so we use -> to access the member in the listnode
		//then, the member in the listnode(node 1), nextptr is a unique pointer so we use move to transfer the possession
		//so now, the the nextptr in "node 1" is pointing to the newnode
		tail->nextptr->preptr = tail;
		//tail->nextptr is newnode, and its preptr points to "node 1"
		tail = tail->nextptr.get();
		//finally, we change the pointer tail to point the newnode
	}
	++size;
}


template<typename T>
void MyList<T>::pop_back()
{
	if (empty())
	{
		std::cerr << "Error: in function " << __func__ << " at line " << __LINE__
			<< " Compiled on " << __DATE__ << " at " << __TIME__ << std::endl
			<< "The list is empty." << std::endl;
		return;
	}
	else
	{
		if (size == 1)
		{
			head.reset();
			tail = nullptr;
		}
		else
		{
			tail = tail->preptr;
			tail->nextptr.reset();
		}
		--size;
	}
}

template<typename T>
void MyList<T>::print() const
{
	if (size == 0)
	{
		std::cout << "The list is empty." << std::endl;
		return;
	}
	auto tmp = head.get();
	while (tmp)
	{
		std::cout << tmp->data << " " << std::flush;
		tmp = tmp->nextptr.get();
	}
	std::cout << std::endl;
}


int main()
{

	MyList<int> _list3{0};

	_list3.print();
	_list3.pop_back();
	_list3.print();

	return 0;
}
