#include <iostream>
#include <string>

void clear_screen()
{
	std::cout << "\033[2J\033[1;1H";
}