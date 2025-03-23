# Tiny_STL
### Create a tiny STL in C++11 for myself.

# Overview
### This is a project based on C++11, I'm gonna to make a tiny STL. Keep update until complete most functions.

# Language
 `rgb(9, 105, 218)` C++

# System
* ### Windows 10

# Compiler
* ### Visual Studio 2022 [here](https://visualstudio.microsoft.com/zh-hant/downloads/)

# Completed Functions
### 1. Create Class MyVector
### 2. Create Insert_Element and Remove_Element function: push_back() and pop_back()
### 3. Create Increase Vector Capacity function: realloacate()
### 4. Create Iterator funcion include prefix/postfix increment, prefix/postfix decrement and compare function.
### 5. Create Front and Back function that we can access the first element and the last element:front(), back()
### 6. Create Empty function that we can recognize whether the vecotr is empty or not: empty()
### 7. Create Clear funciton that we can pop all elements in the vector:clear()
### 8. Create Free funciton that we can free out memory: free()


# Details
## 1. Default Constructor:
```c++
MyVector() : data(nullptr), the_first(nullptr), size(nullptr), capacity(nullptr) 
{ 
	std::cout << "Constructing Vector..." << std::endl; 
} //constructor and there is no memory for data now
```
## 2. Copy Constructor:
```c++
MyVector(const MyVector& v) //copy
{
	auto new_data = copy_alloc(v.begin(), v.end());
	the_first = new_data.first;
	size = capacity = new_data.second;
	std::cout << "Copying Vector..." << std::endl;
} 
```
## 3. Move Constructor:
```c++
MyVector(MyVector&& v) noexcept://move
	the_first(v.the_first), size(v.size), capacity(v.capacity)
{
	v.the_first = v.size = v.capacity = nullptr;
	std::cout << "Moving Vector..." << std::endl;
}
```
## 4. Reallocate:
```c++
void reallocate(size_t new_capacity)
{
	auto first = alloc.allocate(new_capacity);
	auto last = std::uninitialized_copy(std::make_move_iterator(begin()), std::make_move_iterator(end()), first);

	clear();
	the_first = first;
	size = last;
	capacity = the_first + new_capacity;
}
```
## 5. uninitialized_copy(b,e,data): Copy the element from b to e into data.
## 6. Remove Elemnet:
```c++
void pop_back() //remove the last element
{
	if (size != the_first)
	{
		alloc.destroy(--size);
	}
}
```
## 7. destroy(p): Remove the element from position p but we need to know the memory is not freed.
## 8. Add Element:
```c++
void push_back(const T& value) //bind to lvalue we construct by copying
{
	if (get_size() == get_capacity()) //only when we insert the element that the capacity will not be enough
	{
		reallocate(get_capacity() == 0 ? 1 : get_capacity() * 2);
	}
	alloc.construct(size++, value);
}
void push_back(T&& value) //bind to rvalue so that we can construct by moving		
{
	if (get_size() == get_capacity()) //only when we insert the element that the capacity will not be enough
	{
		reallocate(get_capacity() == 0 ? 1 : get_capacity() * 2);
	}
	alloc.construct(size++, std::move(value));
}
```
