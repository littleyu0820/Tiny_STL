# Tiny_STL
### Create a tiny STL in C++11 for myself.

# Overview
### This is a project based on C++11, I'm gonna to make a tiny STL. Keep update until complete most functions.

# Language
* ### C++

# System
* ### Windows 10

# Compiler
* ### Visual Studio 2022

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
## Create the constructor
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

