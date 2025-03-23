#include <iostream>
#include <string>
#include <algorithm>
#include "MyVector.h"
void test_vector()
{
	Test consturct
	MyVector<int> vec, vec2, vec3;

	Test push
	vec.push_back(10);
	vec.push_back(20);
	vec.push_back(30);
	vec.push_back(40);
	vec.push_back(50);

	vec2.push_back(10);
	vec2.push_back(20);
	vec2.push_back(30);



	Marks();

	std::cout << "Test index. " << std::endl;
	std::cout << "Vec: " << std::flush;
	Test array
	for (size_t i = 0; i < vec.get_size(); ++i)
	{
		std::cout << vec[i] << " " << std::flush;
	}

	Marks();


	Test copy
	MyVector<int> vec4(vec);
	std::cout << "Copy Vec into Vec4. " << std::endl;
	std::cout << "Vec4: " << std::flush;
	for (auto v : vec4)
	{
		std::cout << v << " ";
	}

	Marks();

	Test assign
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

	Test moving
	Before moving
	std::cout << "Move Vec2 into Vec5. " << std::endl;
	std::cout << "Vec2: " << std::flush;
	for (auto v : vec2)
	{
		std::cout << v << " ";
	}


	Marks();

	MyVector<int> vec5 = std::move(vec2);//convert vec2 to rvalue and use moving
	After moving
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

	Test pop
	std::cout << "Test pop." << std::endl;
	std::cout << "Vec: " << std::flush;
	vec.pop_back();

	for (auto v : vec)
	{
		std::cout << v << " ";
	}

	test capacity realloacate
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


	Test the other type and access the value by user

	MyVector<std::string> test_vec;
	std::string input;
	unsigned ctr = 0;

	std::cout << "This is a test please enter any numbers or words.(Max:5)" << std::endl;
	std::cout << ": " << std::flush;
	std::cin.get();
	while (ctr++ < 5 && getline(std::cin, input))
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
}
