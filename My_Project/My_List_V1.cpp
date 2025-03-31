#include <iostream>
#include <algorithm>
#include <memory>

//V1

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
	MyList() : head(nullptr), tail(nullptr), size(0) {}
	~MyList(){}


	void print() const;
	void push_front(const T&);
	void push_back(const T&); //use to add the element at the tail

private:

	std::unique_ptr<ListNode<T>> head; 
	//the unique_ptr point to the head which means that this pointer(head) will have the possession of the whole list
	ListNode<T>* tail; //the normal pointer point to the tail which can help us access the last node
	size_t size;
};

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
void MyList<T>::print() const
{
	auto tmp = head.get();
	while (tmp)
	{
		std::cout << tmp->data << " " << std::flush;
		tmp = tmp->nextptr.get();
	}
}


int main()
{
	

	MyList<int> _list;

	_list.push_back(10);
	_list.push_back(20);
	_list.push_back(30);
	_list.push_back(40);
	_list.push_back(50);
	_list.push_back(60);
	
	_list.print();

	

	return 0;
}
