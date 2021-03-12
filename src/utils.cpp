#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <random>
#include "str_format.hpp"

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
	std::string ansi_text = kt::format_str("\x1B[3{}m", static_cast<int>(fore));

	if (back != Color::BLACK)
	{
		ansi_text.append(kt::format_str("\x1B[4{}m", static_cast<int>(back)));
	}

	return ansi_text.append(kt::format_str("{}\033[0m", text));
}

int random_range(int min, int max)
{
	// randomInteger in [min, max]
	static std::default_random_engine eng{std::random_device{}()};
	std::uniform_int_distribution<int> dist(min, max);
	return dist(eng);
}