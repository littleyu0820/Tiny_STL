#include <iostream>
#include <memory>
#include <algorithm>
#include <utility>



//V5




template <typename T>
class MyVector
{
public:




	MyVector() : the_first(nullptr), size(nullptr), capacity(nullptr)
	{
		std::cout << "Constructing Vector..." << std::endl;
	} //constructor and there is no memory for data now

	MyVector(const MyVector& v) //copy
	{
		auto new_data = copy_alloc(v.the_first, v.size);
		the_first = new_data.first;
		size = capacity = new_data.second;
		std::cout << "Copying Vector..." << std::endl;
	}
	MyVector(MyVector&& v) noexcept ://move
		the_first(v.the_first), size(v.size), capacity(v.capacity)
	{
		v.the_first = v.size = v.capacity = nullptr;
		std::cout << "Moving Vector..." << std::endl;
	}
	~MyVector()
	{
		free();
		std::cout << "Destructing Vector..." << std::endl;
	} //Will be implemented when we leave the scope or use delete
	// avoid memory leak


	/****************************************Iterator*****************************************************************/
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class Iterator 
	{
	public:
		Iterator(T* ptr) : t_ptr(ptr){}
		T& operator*() 
		{
			return *t_ptr;
		}

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


		bool operator==(const Iterator& rhs)
		{
			return t_ptr == rhs.t_ptr;
		}

		bool operator!=(const Iterator& rhs)
		{
			return !(*this == rhs);
		}


	private:
		T* t_ptr;
	};


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/****************************************Iterator*****************************************************************/

	/*
	* a rvalue reference:
	* that a new object can be initialized by moving the contents if an old object out of an old object
	*  and into a new object.
	*/

	void push_back(const T& value) //bind to lvalue we construct by copying
	{
		if (get_size() == get_capacity()) //only when we insert the element that the capacity will not be enough
		{
			reallocate(get_capacity() == 0 ? 1 : get_capacity() * 2);
		}
		alloc.construct(size++, value);
	}

	/*
	* std::string str = "hello";
	* vec.push_back(str); //lvalue call const T&
	*/

	void push_back(T&& value) //bind to rvalue so that we can construct by moving	
	{
		if (get_size() == get_capacity()) //only when we insert the element that the capacity will not be enough
		{
			reallocate(get_capacity() == 0 ? 1 : get_capacity() * 2);
		}
		alloc.construct(size++, std::move(value));
	}

	/*
	* vec.push_back(std::string("wrold")); //rvalue call T&&
	* vec.push_back(std::move(str)); //rvalue call T&& because std::move convert the str(lvalue) into rvalue
	*/

	void pop_back() //remove the last element
	{
		if (size != the_first)
		{
			alloc.destroy(--size);
		}
	}

	T& operator[](const size_t& index)
	{


		if (index < get_size())
			return the_first[index];
		else
		{
			std::cerr << "Error: in function " << __func__ << " at line " << __LINE__
				<< " Compiled on " << __DATE__ << " at " << __TIME__ << std::endl
				<< "The index " << index << " is too big." << std::endl;
			exit(1);
		}


	} //The function support us to use index(lavalue) access vector

	MyVector& operator=(const MyVector& rhs)& //lvalue
	{
		if (this != &rhs)
		{
			std::cout << "Assigning vector..." << std::endl;
			auto new_data = copy_alloc(rhs.the_first, rhs.size);
			free();
			the_first = new_data.first;
			size = capacity = new_data.second;

		}
		return *this;
	}
	MyVector& operator=(MyVector&& rhs) & noexcept //rvalue
	{
		if (this != &rhs)
		{
			std::cout << "Assigning vector..." << std::endl;
			free();
			the_first = std::move(rhs.the_first);
			size = std::move(rhs.size);
			capacity = std::move(rhs.capacity);
			rhs.the_first = rhs.size = rhs.capacity = nullptr;
		}
		return *this;
	}


	const size_t get_size() const { return size - the_first; }
	const size_t get_capacity() const { return capacity - the_first; }

	

	
	Iterator begin() { return Iterator(the_first); }
	Iterator end() { return Iterator(size); }
	const Iterator begin() const { return Iterator(the_first); }
	const Iterator end() const { return Iterator(size); }
	/*T* begin() { return the_first; }
	const T* begin() const { return the_first; }
	T* end() { return size; }
	const T* end() const { return size; }*/

	T& front() { return *the_first; }
	const T& front() const { return *the_first; } //return first element
	T& back() { return *(size - 1); }
	const T& back() const { return *(size - 1); } //return last element

	bool empty() { return the_first == size ? 1 : 0; }

	void clear() //pop all elements
	{
		if (the_first != size)
		{
			while (size != the_first)
				alloc.destroy(--size);
		}
	}

	void free()
	{
		if (the_first)
		{
			for (auto pe = size; pe != the_first;)
				alloc.destroy(--pe);
		}
		alloc.deallocate(the_first, capacity - the_first);
	}

private:

	T* the_first;
	T* size;
	T* capacity;
	static std::allocator<T> alloc;

	void reallocate(size_t new_capacity)
	{
		auto first = alloc.allocate(new_capacity);
		//auto last = first;
		//auto elements = the_first;
		auto last = std::uninitialized_copy(the_first, size, first);
		//for (size_t i = 0; i != get_size(); ++i) //ask why we need to use elements
		//	alloc.construct(last++, std::move(*elements++));
		free();
		the_first = first;
		size = last;
		capacity = the_first + new_capacity;
	}

	const std::pair<T*, T*> copy_alloc(const T* b, const T* e)
	{
		auto new_one = alloc.allocate(e - b);
		return { new_one, std::uninitialized_copy(b, e, new_one) };
	}
	//return a pair of the first element and the last element


};





void Marks()
{
	std::cout << "\n" << "----------------------------" << std::endl;
	std::cout << "----------------------------" << std::endl;
	std::cout << "----------------------------" << std::endl;
}


template <typename T>
std::allocator<T> MyVector<T>::alloc;

void Marks();

int main()
{
	//Test consturct
	MyVector<int> vec, vec2, vec3;

	//Test push
	vec.push_back(10);
	vec.push_back(20);
	vec.push_back(30);
	vec.push_back(40);
	vec.push_back(50);

	vec2.push_back(10);
	vec2.push_back(20);
	vec2.push_back(30);

	//Test Iterator
	std::cout << "Test iterator. " << std::endl;
	auto bbegg = vec.begin();
	auto eendd = vec.end();
	std::cout << *bbegg << std::endl;
	std::cout << *++bbegg << std::endl;
	std::cout << *bbegg++ << std::endl;
	std::cout << *bbegg << std::endl;
	std::cout << *eendd << std::endl;
	std::cout << *--eendd << std::endl;
	std::cout << *eendd-- << std::endl;
	std::cout << *eendd << std::endl;
	
	Marks();

	std::cout << "Test index. " << std::endl;
	std::cout << "Vec: " << std::flush;
	//Test array
	for (size_t i = 0; i < vec.get_size(); ++i)
	{
		std::cout << vec[i] << " " << std::flush;
	}

	Marks();


	//Test copy
	MyVector<int> vec4(vec);
	std::cout << "Copy Vec into Vec4. " << std::endl;
	std::cout << "Vec4: " << std::flush;
	for (auto v : vec4)
	{
		std::cout << v << " ";
	}

	Marks();

	//Test assign
	vec3 = vec;
	std::cout << "Assign Vec into Vec3. " << std::endl;
	std::cout << "Vec: " << std::flush;
	for (auto v : vec)
	{
		std::cout << v << " ";
	}

	Marks();

	std::cout << "Vec3: " << std::flush;
	for (auto v : vec3)
	{
		std::cout << v << " ";
	}

	Marks();

	//Test moving
	//Before moving
	std::cout << "Move Vec2 into Vec5. " << std::endl;
	std::cout << "Vec2: " << std::flush;
	for (auto v : vec2)
	{
		std::cout << v << " ";
	}


	Marks();

	MyVector<int> vec5 = std::move(vec2);//convert vec2 to rvalue and use moving
	//After moving
	std::cout << "Vec2: " << std::flush;
	for (auto v : vec2)
	{
		std::cout << v << " ";
	}

	Marks();

	std::cout << "Vec5: " << std::flush;
	for (auto v : vec5)
	{
		std::cout << v << " ";
	}

	Marks();

	//Test pop
	std::cout << "Test pop." << std::endl;
	std::cout << "Vec: " << std::flush;
	vec.pop_back();

	for (auto v : vec)
	{
		std::cout << v << " ";
	}

	//test capacity realloacate
	std::cout << "\n" << "After pop the size for vec is: " << vec.get_size()
		<< ".\n" << "And the capacity for vec is: " << vec.get_capacity() << "." << std::endl;

	Marks();


	auto beg = vec.begin();
	auto end = vec.end();


	std::cout << "The address of begin: " << &beg << std::endl;


	std::cout << "Begin: " << *beg << std::endl; //begin
	std::cout << "Begin move back(1): " << *(++beg) << std::endl; //++ prefix
	std::cout << "Gonna do posfix increment: " << *(beg++) << std::endl; //++ postfix
	std::cout << "After postfix increment: " << *beg << std::endl; //++ postfix


	Marks();

	std::cout << "The address of end: " << &end << std::endl;

	std::cout << "End: " << *end << std::endl; //end null or whatever the value it was
	std::cout << "End move front(1): " << *(--end) << std::endl; //-- prefix
	std::cout << "Gonna do postfix decrement: " << *(beg--) << std::endl; //-- postfix
	std::cout << "After postfix decrement: " << *beg << std::endl; //-- postfix

	Marks();


	beg = vec.begin();
	end = vec.end();

	std::cout << "Vec: " << std::flush;
	while (beg != end)
	{
		std::cout << *(beg++) << " " << std::flush;
	}

	Marks();


	std::cout << "The first element for vec is: " << vec.front() << std::endl;
	std::cout << "The last element for vec is: " << vec.back() << std::endl;

	if (!vec.empty())
		std::cout << "The vector vec is not empty!" << "\nAnd the size for vec is: " << vec.get_size() << std::endl;
	else
		std::cout << "The vector vec is empty!" << std::endl;

	vec.clear();


	if (!vec.empty())
		std::cout << "After clear the vector vec is not empty!" << std::endl;
	else
		std::cout << "After clear the vector vec is empty!" << " \nAnd the size for vec is: " << vec.get_size() << std::endl;



	std::cout << "Do you want to clear the screen?(Yes or No) " << "\n";
	std::cout << ": " << std::flush;

	std::string YesOrNo;
	std::cin >> YesOrNo;

	std::transform(YesOrNo.begin(), YesOrNo.end(), YesOrNo.begin(), ::tolower);

	if (YesOrNo == "yes")
		std::system("cls");


	//Test the other type and access the value by user

	MyVector<std::string> test_vec;
	std::string input;
	unsigned ctr = 0;

	std::cout << "This is a test please enter any numbers or words.(Max:5)" << std::endl;
	std::cout << ": " << std::flush;
	while (ctr++ < 5 && std::cin >> input)
	{
		if (ctr < 5)
			std::cout << ": " << std::flush;
		test_vec.push_back(input);

	}

	std::cout << "The test_vec is: " << std::flush;
	auto begg = test_vec.begin();
	while (begg != test_vec.end())
		std::cout << *(begg++) << " " << std::flush;

	Marks();


	std::cout << "the first element is: " << test_vec.front() << std::endl;
	std::cout << "the last element is: " << test_vec.back() << std::endl;

	if (!test_vec.empty())
		std::cout << "the vector is not empty!" << "\nand the size is: " << test_vec.get_size() << std::endl;
	else
		std::cout << "the vector is empty!" << std::endl;

	test_vec.clear();


	if (!test_vec.empty())
		std::cout << "After clear the vector is not empty!" << std::endl;
	else
		std::cout << "After clear the vector is empty!" << " \nAnd the size is: " << test_vec.get_size() << std::endl;




	return 0;
}
