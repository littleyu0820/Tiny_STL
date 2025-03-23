#ifndef MYVECTOR_H
#define MYVECTOR_H
#include <utility>
#include <memory>
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
		Iterator(T* ptr) : t_ptr(ptr) {}
		T& operator*()
		{
			return *t_ptr;
		}

		T* operator&()
		{
			return t_ptr;
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


		Iterator operator+(int pos)
		{
			while (pos-- > 0)
				++t_ptr;
			return *this;
		}

		Iterator operator-(int pos)
		{
			while (pos-- > 0)
				--t_ptr;
			return *this;
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
		auto last = std::uninitialized_copy(the_first, size, first);
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

template <typename T>
std::allocator<T> MyVector<T>::alloc;
void Marks();
int print_blocks();
void test_vector();
void test_iterator();

#endif
