#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

void clear_screen()
{
	std::cout << "\033[2J\033[1;1H";
}

void sleep(int ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

enum class Color
{
	RED = 1,
	GREEN = 2,
	YELLOW = 3,
	BLUE = 4,
	MAGENTA = 5,
	CYAN = 6,
	WHITE = 7
};

std::string color(std::string text, Color color)
{
	// modify fore ground color of text
	std::string code = "\x1B[3?m";
	code.replace(code.find('?'), 1, std::to_string(static_cast<int>(color)+1));
	return code + text + "\033[0m";
}
