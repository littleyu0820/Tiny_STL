#include <iostream>

void Marks()
{
	std::string line(36, '-');
	std::cout << "\n";
	for (int i = 3; i > 0; --i)
		std::cout << line << std::endl;

	std::cout << "Please enter any words to continue...\n" << ":" << std::flush;
	std::cin.get();
	int input = 0;
	if (1)
		std::cin >> input;
}
