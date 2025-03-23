#include <iostream>
#include "MyVector.h"
void test_iterator()
{

	std::cout << "Test Iterator" << std::endl;
	//Test the function for iterator
	MyVector<int> vec;

	vec.push_back(10);
	vec.push_back(20);
	vec.push_back(30);

	auto beg = vec.begin();
	auto end = vec.end();
	std::cout << *(beg) << std::endl;
	std::cout << *(beg + 1) << std::endl;
	std::cout << *(beg + 1) << std::endl;
	std::cout << *(beg + 1) << std::endl;

	std::cout << *(end) << std::endl;
	std::cout << *(end - 1) << std::endl;
	std::cout << *(end - 1) << std::endl;
	std::cout << *(end - 1) << std::endl;

	beg = vec.begin();
	end = vec.end();

	std::cout << &(beg) << std::endl;
	std::cout << &(beg + 1) << std::endl;
	std::cout << &(beg + 1) << std::endl;
	std::cout << &(beg + 1) << std::endl;

	std::cout << &(end) << std::endl;
	std::cout << &(end - 1) << std::endl;
	std::cout << &(end - 1) << std::endl;
	std::cout << &(end - 1) << std::endl;
}
