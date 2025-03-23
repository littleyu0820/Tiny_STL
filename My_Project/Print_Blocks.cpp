#include <iostream>
int print_blocks()
{
	std::string up_down_line(36, '*');
	//std::string the_space(34, ' ');

	std::cout << "Welcome to My_STL!" << std::endl;
	std::cout << "What container do you want to test?" << std::endl;

	std::cout << up_down_line << std::endl;
	for (int i = 6; i > 0; --i)
	{
		switch (i)
		{
		case 6:
			std::cout << "1. Test Vector" << std::endl;
			break;
		case 5:
			std::cout << "2. Test Map" << std::endl;
			break;
		case 4:
			std::cout << "3. Test Set" << std::endl;
			break;
		case 3:
			std::cout << "4. Test Iterator" << std::endl;
			break;
		default:
			break;
		}
	}

	std::cout << "\n" << up_down_line << std::endl;

	std::cout << "Please choose: " << std::flush;
	int choose = 0;
	std::cin >> choose;
	switch (choose)
	{
	case 1:
		std::cout << "1. We're going to test Vector!\n" << std::endl;
		break;
	case 2:
		std::cout << "2. We're going to test Map!\n" << std::endl;
		break;
	case 3:
		std::cout << "3. We're going to test Set!\n" << std::endl;
		break;
	case 4:
		std::cout << "4. We're going to test Iterator!\n" << std::endl;
		break;
	default:
		break;
	}
	return choose;
}
