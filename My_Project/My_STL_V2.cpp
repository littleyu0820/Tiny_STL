#include <iostream>
#include <memory>

//struct Iterator //We can think iterator as a pointer since it represents the position
//{
//
//	Iterator(int* ptr) :m_ptr(ptr) {} //constructor
//
//	int& operator*() const { return *m_ptr; } //get the value from the pointer
//	int* operator->() { return m_ptr; } //make the object be used like a pointer
//
//	Iterator& operator++() //prefix increment
//	{ 
//		++m_ptr; //m_ptr move back(1)
//		return *this; //return the object
//	}
//	Iterator& operator++(int) //postfix increment
//	{
//		Iterator tmp = *this; //copy the object
//		++(*this); //m_ptr move back(1)
//		return tmp; //return the old object(we already copy it)
//	}
//
//	Iterator& operator--() //prefix decrement
//	{
//		--m_ptr;
//		return *this;
//	}
//	Iterator& operator--(int) //postfix decrement
//	{
//		Iterator tmp = *this;
//		--(*this);
//		return tmp;
//	}
//
//	//In order to print the whole vector compare begin and end
//	friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
//	friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };
//
//private:
//
//	int* m_ptr; //In order to point to a memory address where the container is
//
//};


template <typename T> 
class MyVector
{
public:

	MyVector() : data(nullptr), size(0), capacity(0) {} //constructor and there is no memory for data now
	~MyVector() { delete[] data; } //Will be implemented when we leave the scope or use delete
								  // avoid memory leak
	
	const size_t pos_b = 0; //the position of begin

	/****************************************Iterator*****************************************************************/
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Iterator//only vector can access
	{
	public:

		Iterator(int* ptr) :m_ptr(ptr) {} //constructor

		int& operator*() const { return *m_ptr; } //get the value from the pointer
		int* operator->() { return m_ptr; } //make the object be used like a pointer

		Iterator& operator++() //prefix increment
		{
			++m_ptr; //m_ptr move back(1)
			return *this; //return the object
		}
		Iterator& operator++(int) //postfix increment
		{
			Iterator tmp = *this; //copy the object
			++(*this); //m_ptr move back(1)
			return tmp; //return the old object(we already copy it)
		}

		Iterator& operator--() //prefix decrement
		{
			--m_ptr;
			return *this;
		}
		Iterator& operator--(int) //postfix decrement
		{
			Iterator tmp = *this;
			--(*this);
			return tmp;
		}

		//In order to print the whole vector compare begin and end
		friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
		friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

	private:
		int* m_ptr; //In order to point to a memory address where the container is
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/****************************************Iterator*****************************************************************/

	/*
	* a rvalue reference: 
	* that a new object can be initialized by moving the contents if an old object out of an old object
	*  and into a new object.
	*/

	void push_back(const T& value) //Access lvalue in order to avoid "copy" because it return reference
	{
		if (size == capacity) //only when we insert the element that the capacity will not be enough
		{
			reallocate(capacity == 0 ? 1 : capacity * 2);
		}
		data[size++] = value;
	}

	/*
	* std::string str = "hello";
	* vec.push_back(str); //lvalue call const T&
	*/

	void push_back(T&& value) //Access rvalue in order to avoid "copy" because rvalue can use std::move()						
	{
		if (size == capacity) //only when we insert the element that the capacity will not be enough
		{
			reallocate(capacity == 0 ? 1 : capacity * 2);
		}
		data[size++] = std::move(value);
	}

	/*
	* vec.push_back(std::string("wrold")); //rvalue call T&&
	* vec.push_back(std::move(str)); //rvalue call T&&
	*/

	void pop_back() //remove the last element
	{
		if (size > 0)
			--size;
	}

	T& operator[](size_t index) { return data[index]; } //The function support as to use index(lavalue) access vector
	const T& operator[](size_t index) const { return data[index]; } //The function support as to use index(rvalue) access vector

	size_t get_size() const { return size; }
	size_t get_capacity() const { return capacity; }

	MyVector::Iterator begin() { return MyVector::Iterator(&data[pos_b]); } //*m_ptr = &data[pos_b];
	MyVector::Iterator end() { return MyVector::Iterator(&data[size]); } //*m_ptr = &data[size];
	
	T& front() { return data[pos_b]; } //return first element
	T& back() { return data[size-1]; } //return last element

	bool empty() { return &data[pos_b] == &data[size] ? 1 : 0; }

	void clear() //pop all elements
	{
		if(size > 0)
			size -= size;
	}


	
	

private:

	T* data; //declare data as a ponter in order to manage the memory
	size_t size;
	size_t capacity;

	void reallocate(size_t new_capacity)
	{
		/*
		* We need to rememebr that new will call construct function.
		* So constructor is needed when we use new
		*/
		T* new_data = new T[new_capacity]; //create a new object(array) that capacity is new capacity and store into new_data

		for (size_t i = 0; i < size; ++i)
		{
			new_data[i] = std::move(data[i]); //move the old data into new_data
		}
		delete[] data; //delete old_data
		data = new_data; //copy
		capacity = new_capacity;
	}


	
	
};



int main()
{

	MyVector<int> vec;

	vec.push_back(10);
	vec.push_back(20);
	vec.push_back(30);
	vec.push_back(40);
	vec.push_back(50);

	vec.pop_back();
	


	////test data
	for (size_t i = 0; i < vec.get_size(); ++i)
	{
		std::cout << vec[i] << " ";
	}

	std::cout << std::endl;
	//test capacity realloacate
	std::cout << "The size is " << vec.get_size() << ".\n" << "The capacity is " << vec.get_capacity() << "." << std::endl;

	std::cout << "----------------------------" << std::endl;
	std::cout << "----------------------------" << std::endl;
	std::cout << "----------------------------" << std::endl;


	auto beg = vec.begin();
	auto end = vec.end();


	std::cout << "The address of begin: " << & beg << std::endl;


	std::cout << "Begin: " << *beg << std::endl; //begin
	std::cout << "Begin move back(1): " << *(++beg) << std::endl; //++ prefix
	std::cout << "Gonna do posfix increment: " << *(beg++) << std::endl; //++ postfix
	std::cout << "After postfix increment: " << *beg << std::endl; //++ postfix


	std::cout << "----------------------------" << std::endl;
	std::cout << "----------------------------" << std::endl;
	std::cout << "----------------------------" << std::endl;

	std::cout << "The address of end: " << &end << std::endl;

	std::cout << "End: " << *end << std::endl; //end null or whatever the value it was
	std::cout << "End move front(1): " << *(--end) << std::endl; //-- prefix
	std::cout << "Gonna do postfix decrement: " << *(beg--) << std::endl; //-- postfix
	std::cout << "After postfix decrement: " << *beg << std::endl; //-- postfix

	std::cout << "----------------------------" << std::endl;
	std::cout << "----------------------------" << std::endl;
	std::cout << "----------------------------" << std::endl;


	beg = vec.begin();
	end = vec.end();

	while (beg != end)
	{
		std::cout << *(beg++) << " " << std::flush;
	}

	std::cout << "\n" << "----------------------------" << std::endl;
	std::cout << "----------------------------" << std::endl;
	std::cout << "----------------------------" << std::endl;


	std::cout << "The first element is: " << vec.front() << std::endl;
	std::cout << "The last element is: " << vec.back() << std::endl;
	
	if (!vec.empty())
		std::cout << "The vector is not empty!" << std::endl;
	else
		std::cout << "The vector is empty!" << std::endl;

	vec.clear();
	

	if (!vec.empty())
		std::cout << "After clear the vector is not empty!" << std::endl;
	else
		std::cout << "After clear the vector is empty!"  << "And the size is: " << vec.get_size() << std::endl;




	return 0;
}
