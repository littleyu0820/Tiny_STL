#include <iostream>
#include <string>
#include <algorithm>
#include "MyVector.h"

int main()
{
	int choose = print_blocks();
	switch (choose)
	{
	case 1:
		test_vector();
		break;
	case 2:

		break;
	case 3:

		break;
	case 4:
		//test_iterator();
		break;
	default:
		break;
	}

	return 0;
}
