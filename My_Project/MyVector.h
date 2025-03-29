#ifndef MYVECTOR_H
#define MYVECTOR_H
#include <utility>
#include <memory>
template <typename T> class MyVector
{
public:

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

	MyVector(const MyVector& v)
	{
		auto new_data = copy_alloc(v.the_first, v.size);
		the_first = new_data.first;
		size = capacity = new_data.second;
		new_data.first = nullptr;
		new_data.second = nullptr;
		std::cout << "Copying Vector..." << std::endl;
	} //constructor for copying the implementation is like std::vector<int> vec2(vec) that vec is also a vector<int>
	MyVector(MyVector&& v) noexcept : //MyVector&& v: rvalue reference means that the parameter should be rvalue
		the_first(v.the_first), size(v.size), capacity(v.capacity)
	{
		v.the_first = v.size = v.capacity = nullptr;
		std::cout << "Moving Vector..." << std::endl;
	} //constructor for moving that we can move the data not copy it
	~MyVector()
	{
		free();
		std::cout << "Destructing Vector..." << std::endl;
	} //the destructor and Will be implemented when we exit the program or call delete



	/****************************************Iterator*****************************************************************/
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/****************************************Iterator*****************************************************************/

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

	//used to insert in anywhere of the container
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

	Iterator erase(Iterator&& it) & noexcept
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


	//we can use index to access the value(data) in the container
	//and we need to know this function is for modified container
	T& operator[](size_t index)
	{
		if (index >= get_size()) //check whether the index is out of range or not
		{
			std::cerr << "Error: in function " << __func__ << " at line " << __LINE__
				<< " Compiled on " << __DATE__ << " at " << __TIME__ << std::endl
				<< "The index " << index << " is too big." << std::endl;
			exit(1);
		}
		else
		{
			return the_first[index];
		}
	}
	//same function but this is for unmodified(const) container
	const T& operator[](size_t index) const
	{
		if (index >= get_size()) //check whether the index is out of range or not
		{
			std::cerr << "Error: in function " << __func__ << " at line " << __LINE__
				<< " Compiled on " << __DATE__ << " at " << __TIME__ << std::endl
				<< "The index " << index << " is too big." << std::endl;
			exit(1);
		}
		else
		{
			return the_first[index];
		}
	}

	T& at(size_t index) //kind iof like index
	{
		if (index >= get_size()) //check whether the index is out of range or not
		{
			std::cerr << "Error: in function " << __func__ << " at line " << __LINE__
				<< " Compiled on " << __DATE__ << " at " << __TIME__ << std::endl
				<< "The index " << index << " is too big." << std::endl;
			exit(1);
		}
		else
		{
			return the_first[index];
		}

	}
	const T& at(size_t index) const
	{
		if (index >= get_size()) //check whether the index is out of range or not
		{
			std::cerr << "Error: in function " << __func__ << " at line " << __LINE__
				<< " Compiled on " << __DATE__ << " at " << __TIME__ << std::endl
				<< "The index " << index << " is too big." << std::endl;
			exit(1);
		}
		else
		{
			return the_first[index];
		}
	}

	//used to assign container
	//and this one is for lvalue
	//e.g. if we have two vectors one is MyVector vec1 = {1,2,3} and the other is MyVector vec2 which is unitialized
	//if we call vec2 = vec1; this function will be implemented
	MyVector& operator=(const MyVector& rhs)&  //going to use copy-and-swap
	{
		if (this != &rhs)
		{
			std::cout << "Assigning vector..." << std::endl;
			MyVector tmp = rhs;
			swap(tmp);
		}
		return *this;
	}

	//used to assign container
	//and this one is for rvalue
	//e.g. if we have two vectors one is MyVector vec1 = {1,2,3} and the other is MyVector vec2 which is unitialized
	//if we call vec2 = std::move(vec1); this function will be implemented
	MyVector& operator=(MyVector&& rhs) & noexcept
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

	bool operator==(const MyVector& rhs) const
	{
		if (get_size() != rhs.get_size())
			return false;
		else
		{
			auto lbeg = begin();
			auto rbeg = rhs.begin();
			while (rbeg != rhs.end())
			{
				if (*lbeg == *rbeg)
					++lbeg;
				++rbeg;
			}
			if (lbeg == end())
				return true;
			else
				return false;
		}
	}

	bool operator!=(const MyVector& rhs) const
	{
		if (get_size() != rhs.get_size())
			return true;
		else
		{
			auto lbeg = begin();
			auto rbeg = rhs.begin();
			while (rbeg != rhs.end())
			{
				if (*lbeg == *rbeg)
					++lbeg;
				++rbeg;
			}
			if (lbeg == end())
				return false;
			else
				return true;
		}
	}

	bool operator<(const MyVector& rhs) const
	{
		auto lbeg = begin();
		auto rbeg = rhs.begin();

		while (rbeg != rhs.end())
		{
			if (*lbeg < *rbeg)
				return true;
			else if (*lbeg++ == *rbeg++)
				;
			else
				return false;
		}
	}

	bool operator>(const MyVector& rhs) const
	{
		auto lbeg = begin();
		auto rbeg = rhs.begin();

		while (lbeg != end())
		{
			if (*lbeg < *rbeg)
				return false;
			else if (*lbeg++ == *rbeg++)
				;
			else
				return true;
		}
	}

	//const size_t get_size() { return size - the_first; }
	//const size_t get_capacity() { return capacity - the_first; }

	//used to access the size(from the first element to the positon after the last element)
	const size_t get_size() const { return size - the_first; }
	//used to access the capactiy(from the first element to end)
	const size_t get_capacity() const { return capacity - the_first; }

	//the functions to access begin and end, we also need to realize that these 2 are pointers which type are Iterator
	//and these 2 are for modified container
	Iterator begin() { return Iterator(the_first); }
	Iterator end() { return Iterator(the_first + get_size()); }


	//the functions to access begin and end, we also need to realize that these 2 are pointers which type are Iterator
	//and these 2 are for unmodified container
	const Iterator begin() const { return Iterator(the_first); }
	const Iterator end() const { return Iterator(the_first + get_size()); }

	//the functions to access the first element and last element
	//and we need to realize that these 2 are different from begin() and end() since this is used to access the value(data) not pointer
	//so that the type for these two are T
	//they're also be defined in 2 types 1 for unmodified(const) and 1 for modified
	T& front()
	{

		if (empty())
		{
			std::cerr << "Error: in function " << __func__ << " at line " << __LINE__
				<< " Compiled on " << __DATE__ << " at " << __TIME__ << std::endl
				<< "The container is empty." << std::endl;
			exit(1);
		}
		return *the_first;
	}
	const T& front() const
	{
		if (empty())
		{
			std::cerr << "Error: in function " << __func__ << " at line " << __LINE__
				<< " Compiled on " << __DATE__ << " at " << __TIME__ << std::endl
				<< "The container is empty." << std::endl;
			exit(1);
		}
		return *the_first;
	} //return first element

	T& back()
	{
		if (empty())
		{
			std::cerr << "Error: in function " << __func__ << " at line " << __LINE__
				<< " Compiled on " << __DATE__ << " at " << __TIME__ << std::endl
				<< "The container is empty." << std::endl;
			exit(1);
		}
		return *(size - 1);
	}
	const T& back() const
	{
		if (empty())
		{
			std::cerr << "Error: in function " << __func__ << " at line " << __LINE__
				<< " Compiled on " << __DATE__ << " at " << __TIME__ << std::endl
				<< "The container is empty." << std::endl;
			exit(1);
		}
		return *(size - 1);
	} //return last element

	//usd to check there are any elemnets or not
	//since if there are any elements that the size will be moved or we say the size we be resized
	bool empty() const { return the_first == size; }

	//pop all elements
	void clear()
	{
		if (the_first != size)
		{
			while (size != the_first)
				alloc.destroy(--size);
		}
	}
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

private:

	T* the_first; //pointer point to the position where the first_element at
	T* size; //pointer point to the position where is after the last element or we can also call it end
	T* capacity; //pointer point to the position where the last space(last null) for the container
	static std::allocator<T> alloc; //an object which type is allocator and can be used to allocate the container

	void swap(MyVector<T>& rhs)
	{
		using std::swap;
		swap(the_first, rhs.the_first);
		swap(size, rhs.size);
		swap(capacity, rhs.capacity);
	}

	//the function used to allocate the memory space for the container
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

	//use to destructor
	void free()
	{
		if (the_first) //which means that the_first is not point to null
		{
			for (auto pe = size; pe != the_first;) //delete the element from the last element
				alloc.destroy(--pe);
		}
		alloc.deallocate(the_first, capacity - the_first); //free the memory for the whole container
	}

	//the function used to copy the vector
	std::pair<T*, T*> copy_alloc(const T* b, const T* e)
	{
		auto new_one = alloc.allocate(e - b); //allocate the memory space whice size is from b to e
		return { new_one, std::uninitialized_copy(b, e, new_one) }; //copy the value(data) from b to e
	}
	//return a pair of 2 pointers which is the_first point to the first element 
	//and the other is size point to the position where is after the last element

};

template <typename T> class MyVector;
template <typename T>
std::allocator<T> MyVector<T>::alloc;
void Marks();
int print_blocks();
void test_vector();
void test_iterator();

#endif
