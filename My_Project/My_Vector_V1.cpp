#include <iostream>
#include <memory>

struct Iterator
{

	Iterator(int* ptr) :m_ptr(ptr) {}

	int& operator*() const { return *m_ptr; }
	int* operator->() { return m_ptr; }

	Iterator& operator++() //prefix increment
	{ 
		++m_ptr; 
		return *this; 
	}
	Iterator& operator++(int) //postfix increment
	{
		Iterator tmp = *this;
		++(*this);
		return tmp;
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

	int* m_ptr;

};


template <typename T> 
class MyVector
{
public:

	MyVector() : data(nullptr), size(0), capacity(0) {}
	~MyVector() { delete[] data; }

	const size_t pos_b = 0;

	/*
	* a rvalue reference: 
	* that a new object can be initialized by moving the contents if an old object out of an old object
	*  and into a new object.
	*/

	void push_back(const T& value)
	{
		if (size == capacity)
		{
			realloacate(capacity == 0 ? 1 : capacity * 2);
		}
		data[size++] = value;
	
	}

	void push_back(T&& value) 
								
	{
	
		if (size == capacity)
		{
			realloacate(capacity == 0 ? 1 : capacity * 2);
		}

		data[size++] = std::move(value);
	
	}

	void pop_back() //remove the last elements
	{
		if (size > 0)
			--size;

	}

	T& operator[](size_t index) { return data[index]; }
	const T& operator[](size_t index) const { return data[index]; }
	size_t get_size() const { return size; }
	size_t get_capacity() const { return capacity; }

	Iterator begin() { return Iterator(&data[pos_b]); } //*m_ptr = &data[pos_b];
	Iterator end() { return Iterator(&data[size]); } //*m_ptr = &data[size-1];


	
	

private:

	T* data;
	size_t size;
	size_t capacity;

	void realloacate(size_t new_capacity)
	{
		T* new_data = new T[new_capacity];

		for (size_t i = 0; i < size; ++i)
		{
			new_data[i] = std::move(data[i]);
		}
		delete[] data;
		data = new_data;
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
	



	return 0;
}
