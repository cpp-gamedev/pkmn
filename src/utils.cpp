#include <algorithm>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <random>
#include <string>
#include <thread>
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

std::filesystem::path find_upwards(std::string dir_name, int max_depth = 10)
{
	auto path = std::filesystem::current_path() / std::filesystem::path(dir_name);

	while (!std::filesystem::exists(path) && max_depth > 0)
	{
		max_depth -= 1;
		path = path.parent_path().parent_path() / path.stem();
	}

	return (max_depth > 0) ? path : std::filesystem::path();
}

bool validate_asset_dir(const std::filesystem::path& asset_dir)
{
	// check manifest here
	return true;
}

std::vector<std::string> read_file(const std::filesystem::path& path)
{
	std::string line{};
	std::vector<std::string> lines{};
	std::ifstream file{path.string()};

	if (std::filesystem::exists(path))
	{
		while (getline(file, line))
		{
			lines.push_back(line + '\n');
		}
	}
	else
	{
		std::cerr << "File not found: " << path.string() << '\n';
	}

	return lines;
}
