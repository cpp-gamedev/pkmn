#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <sstream>
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
	BLACK = 0,
	RED = 1,
	GREEN = 2,
	YELLOW = 3,
	BLUE = 4,
	MAGENTA = 5,
	CYAN = 6,
	WHITE = 7
};

std::string style(std::string text, Color fore, Color back = Color::BLACK)
{
	// add fore and background ground color to text
	std::stringstream ansi_text;
	ansi_text << "\x1B[3" << static_cast<int>(fore) << 'm';
	if (back != Color::BLACK)
	{
		ansi_text << "\x1B[4" << static_cast<int>(back) << 'm';
	}
	ansi_text << text << "\033[0m";
	return ansi_text.str();
}
