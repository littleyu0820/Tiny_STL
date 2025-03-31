#include <iostream>
#include <memory>
#include <algorithm>
#include <utility>



//V3
/*
* move semantics ok
* 使用 std::uninitialized_move 搬移物件 ok
* 改善iterator
* 安全性增強
* 增加 operator[] 的邊界檢查 //check index
* pop_back() 需要手動銷毀物件
* 功能增強
* std::initializer_list 支援
* reserve() 提前擴展記憶體
* shrink_to_fit() 釋放多餘空間
* erase() 刪除特定元素
* insert() 在特定位置插入元素
*/



template <typename T>
class MyVector
{
public:
	
	MyVector() : data(nullptr), the_first(nullptr), size(nullptr), capacity(nullptr) 
	{ 
		std::cout << "Constructing Vector..." << std::endl; 
	} //constructor and there is no memory for data now

	MyVector(const MyVector& v) //copy
	{
		auto new_data = copy_alloc(v.begin(), v.end());
		the_first = new_data.first;
		size = capacity = new_data.second;
		std::cout << "Copying Vector..." << std::endl;
	} 
	MyVector(MyVector&& v) noexcept://move
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

	//class Iterator//only vector can access
	//{
	//public:

	//	Iterator(int* ptr) :m_ptr(ptr) {} //constructor

	//	int& operator*() const { return *m_ptr; } //get the value from the pointer
	//	int* operator->() { return m_ptr; } //make the object be used like a pointer

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

	//	//In order to print the whole vector compare begin and end
	//	friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
	//	friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

	//private:
	//	int* m_ptr; //In order to point to a memory address where the container is
	//};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/****************************************Iterator*****************************************************************/

	/*
	* a rvalue reference:
	* that a new object can be initialized by moving the contents if an old object out of an old object
	*  and into a new object.
	*/

	void push_back(const T& value) //Access lvalue in order to avoid "copy" because it return reference
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

	void push_back(T&& value) //Access rvalue in order to avoid "copy" because rvalue can use std::move()						
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
			std::cerr << "Error: in function " << __func__<< " at line " << __LINE__ 
				<< " Compiled on "<< __DATE__ << " at " << __TIME__ << std::endl 
				<< "The index "<< index << " is too big." << std::endl;
			exit(1);
		}
	
		
	} //The function support us to use index(lavalue) access vector
	T& operator[](const size_t&& index)
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
		
	} //The function support us to use index(rvalue) access vector

	MyVector& operator=(const MyVector& rhs) &
	{
		auto new_data = copy_alloc(rhs.begin(), rhs.end());
		free();
		the_first = new_data.first;
		size = capacity = new_data.second;
		return *this;	
	}
	MyVector& operator=(MyVector&& rhs) & noexcept
	{
		if (this != &rhs)
		{
			free();
			the_first = std::move(rhs.the_first);
			size = std::move(rhs.size);
			capacity = std::move(rhs.capacity);
			rhs.the_first = rhs.size = rhs.capacity = nullptr;
		}
		return *this;
	}

	size_t get_size() const { return size - the_first; }
	size_t get_capacity() const { return capacity - the_first; }

	T* begin() const { return the_first; }
	T* end() const { return size; }


	const T& front() { return *the_first; } //return first element
	const T& back() { return *(--size); } //return last element

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

	T* data;
	T* the_first;
	T* size;
	T* capacity;
	static std::allocator<T> alloc;

	void reallocate(size_t new_capacity)
	{
		/*
		* We need to rememebr that new will call construct function.
		* So constructor is needed when we use new
		*/
		auto first = alloc.allocate(new_capacity);
		auto last = std::uninitialized_copy(std::make_move_iterator(begin()), std::make_move_iterator(end()), first);

		clear();
		the_first = first;
		size = last;
		capacity = the_first + new_capacity;
	}

	std::pair<T*, T*> copy_alloc(const T* b, const T* e)
	{
		auto new_one = alloc.allocate(e - b);
		return { new_one, std::uninitialized_copy(b, e, new_one)};
	}
	//return a pair of the first element and the last element


};
template <typename T>
std::allocator<T> MyVector<T>::alloc;

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

	//Test array
	//for (size_t i = 0; i < vec.get_size(); ++i)
	//{
	//	std::cout << vec[i] << std::endl;
	//}

	//std::cout << vec[3] << std::endl;


	//Test copy
	MyVector<int> vec4(vec);
	std::cout << "Vec4: " << std::flush;
	for (auto v : vec4)
	{
		std::cout << v << " ";
	}

	std::cout << "\n" << "----------------------------" << std::endl;
	std::cout << "----------------------------" << std::endl;
	std::cout << "----------------------------" << std::endl;

	//Test moving
	//Before moving
	std::cout << "Vec2: " << std::flush;
	for (auto v : vec2)
	{
		std::cout << v << " ";
	}
	
	std::cout << "\n" << "----------------------------" << std::endl;
	std::cout << "----------------------------" << std::endl;
	std::cout << "----------------------------" << std::endl;

	MyVector<int> vec5 = std::move(vec2);//convert vec2 to rvalue and use moving
	//After moving
	std::cout << "Vec2: " << std::flush;
	for (auto v : vec2)
	{
		std::cout << v << " ";
	}

	std::cout << "\n" << "----------------------------" << std::endl;
	std::cout << "----------------------------" << std::endl;
	std::cout << "----------------------------" << std::endl;
	std::cout << "Vec5: " << std::flush;
	for (auto v : vec5)
	{
		std::cout << v << " ";
	}

	std::cout << "\n" << "----------------------------" << std::endl;
	std::cout << "----------------------------" << std::endl;
	std::cout << "----------------------------" << std::endl;

	//Test pop
	vec.pop_back();


	for (auto v : vec)
	{
		std::cout << v << " ";
	}

	//test capacity realloacate
	std::cout << "\n" << "The size is " << vec.get_size() << ".\n" << "The capacity is " << vec.get_capacity() << "." << std::endl;

	std::cout << "----------------------------" << std::endl;
	std::cout << "----------------------------" << std::endl;
	std::cout << "----------------------------" << std::endl;


	auto beg = vec.begin();
	auto end = vec.end();


	std::cout << "The address of begin: " << &beg << std::endl;


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
		std::cout << "After clear the vector is empty!" << " And the size is: " << vec.get_size() << std::endl;

	

	std::cout << "Do you want to clear the screen?(Yes or No) " << "\n";
	std::cout << ": " << std::flush;

	std::string YesOrNo;
	std::cin >> YesOrNo;

	std::transform(YesOrNo.begin(), YesOrNo.end(), YesOrNo.begin(), ::tolower);

	if (YesOrNo == "yes")
		std::system("cls");


	




	return 0;
}
