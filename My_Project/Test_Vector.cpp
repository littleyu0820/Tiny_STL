#include <iostream>
#include <string>
#include <algorithm>
#include "MyVector.h"
void test_vector()
{

	std::cout << "Test push_back()..." << std::endl;

	MyVector<int> Vec1;
	std::cout << "Vec1(int) is created." << std::endl;
	std::cout << "Please enter the numbers you want to push_back(MAX:5)\n" << ":" << std::flush;
	int input = 0, counter = 0;
	while (counter++ != 5 && std::cin >> input)
	{
		Vec1.push_back(input);
		std::cout << ":" << std::flush;
	}

	std::cout << "Vec1(int) is: " << std::flush;
	for (auto val : Vec1)
	{
		std::cout << val << " " << std::flush;
	}

	Marks();

	std::cout << "Test pop_back()..." << std::endl;
	Vec1.pop_back();
	std::cout << "Vec1(int) is: " << std::flush;
	for (auto val : Vec1)
	{
		std::cout << val << " " << std::flush;
	}

	Marks();

	std::cout << "Test Copy_Constructor..." << std::endl;
	MyVector<int> Vec2 = Vec1;
	std::cout << "Copied Vec1 to Vec2..." << std::endl;
	std::cout << "Vec2(int) is: " << std::flush;
	for (auto val : Vec2)
	{
		std::cout << val << " " << std::flush;
	}
	Marks();

	std::cout << "Test Move_Constructor..." << std::endl;
	MyVector<int> Vec3 = std::move(Vec2);
	std::cout << "Moved Vec2 to Vec3..." << std::endl;
	std::cout << "Vec2(int) is: " << std::flush;
	for (auto val : Vec2)
	{
		std::cout << val << " " << std::flush;
	}
	std::cout << "\nVec3(int) is: " << std::flush;
	for (auto val : Vec3)
	{
		std::cout << val << " " << std::flush;
	}
	Marks();

	std::cout << "Test Initialize..." << std::endl;
	MyVector<int> Vec4(Vec1);
	std::cout << "Initialized Vec4 by Vec1..." << std::endl;
	std::cout << "Vec4(int) is: " << std::flush;
	for (auto val : Vec4)
	{
		std::cout << val << " " << std::flush;
	}
	Marks();

	std::cout << "Do you want to clear the screen?(Yes or No) " << "\n";
	std::cout << ": " << std::flush;

	std::string YesOrNo;
	std::cin >> YesOrNo;

	std::transform(YesOrNo.begin(), YesOrNo.end(), YesOrNo.begin(), ::tolower);

	if (YesOrNo == "yes")
		std::system("cls");

	std::cout << "Test Inedx for Vec1...\n" << "Please enter what index you want to search\n" << ":" << std::flush;
	std::cout << "Vec1(int) is: " << std::flush;
	for (auto val : Vec1)
	{
		std::cout << val << " " << std::flush;
	}
	std::cout << "\n:" << std::flush;
	std::cin >> input;
	std::cout << "The element at index " << input << " is: " << Vec1[input] << std::flush;
	Marks();

	std::cout << "Test get_size() and get_capacity() for Vec1..." << std::endl;
	std::cout << "The size for Vec1 is: " << Vec1.get_size() << " and the capacity for Vec1 is: " << Vec1.get_capacity() << std::endl;
	Marks();

	std::cout << "Test Iterator for Vec1..." << std::endl;
	auto beg = Vec1.begin();
	std::cout << "Iterator begin for Vec1 is: " << *beg << "."<< std::endl;

	Marks();
	std::cout << "Test Insert() for Vec1..." << std::endl;
	std::cout << "Please enter the value you want to insert\n" << ":" << std::flush;
	int value = 0, step = 0;
	std::cin >> value;
	std::cout << "Please enter the steps you want to move from begin\n" << std::flush;
	std::cin >> step;
	auto move_steps = beg + step;
	Vec1.insert(move_steps, value);
	std::cout << "Vec1(int) is: " << std::flush;
	for (auto val : Vec1)
	{
		std::cout << val << " " << std::flush;
	}
	std::cout << "\nThe size for Vec1 is: " << Vec1.get_size() << " and the capacity for Vec1 is: " << Vec1.get_capacity() << std::endl;
	Marks();
	std::cout << "Test erase() for Vec1..." << std::endl;
	std::cout << "Please enter the steps you want to move from begin\n" << ":" << std::flush;
	std::cin >> step;
	move_steps = beg + step;
	Vec1.erase(move_steps);
	std::cout << "Vec1(int) is: " << std::flush;
	for (auto val : Vec1)
	{
		std::cout << val << " " << std::flush;
	}
	std::cout << "\nThe size for Vec1 is: " << Vec1.get_size() << " and the capacity for Vec1 is: " << Vec1.get_capacity() << std::endl;
	Marks();

	std::cout << "Test Clear() for Vec1... " << std::endl;
	bool emptyornot = Vec1.empty();
	std::cout << ((emptyornot) ? "Vec1 is empty now." : "Vec1 is not empty now.") << std::endl;
	std::cout << "So after clear..." << std::endl;
	Vec1.clear();
	std::cout << "Vec1(int) is: " << std::flush;
	for (auto val : Vec1)
	{
		std::cout << val << " " << std::flush;
	}
	Marks();


}
