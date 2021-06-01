#include <algorithm>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <thread>

#include <str_format/str_format.hpp>
#include <dumb_json/json.hpp>

#include "utils.hpp"

namespace utils
{
void clear_screen()
{
	std::cout << "\033[2J\033[1;1H";
}

void sleep(int ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

std::string style(std::string text, Color fore, Color back)
{
	// add fore and background ground color to text
	std::string ansi_text = kt::format_str("\x1B[3{}m", static_cast<int>(fore));

	if (back != Color::BLACK)
	{
		ansi_text.append(kt::format_str("\x1B[4{}m", static_cast<int>(back)));
	}

	return ansi_text.append(kt::format_str("{}\033[0m", text));
}

void upper(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return toupper(c); });
}

void lower(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return tolower(c); });
}

std::filesystem::path find_upwards(std::string dir_name, int max_depth)
{
	auto path = std::filesystem::current_path() / dir_name;

	while (!std::filesystem::exists(path) && max_depth > 0)
	{
		max_depth -= 1;
		path = path.parent_path().parent_path() / path.stem();
	}

	return (max_depth > 0) ? path : std::filesystem::path();
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

void print_enum_table(std::vector<std::string> table, std::string header)
{
	// +----------------------------------------------------------+
	// | HEADER                                                   |
	// | 1. table[0]                                              |
	// | 2. table[1]                                              |
	// | 3. table[2]                                              |
	// +----------------------------------------------------------+

	constexpr int width = 60;
	utils::Color border_color = Color::YELLOW;
	std::string border = utils::style(std::string("|"), border_color);
	std::string horizontal_line = utils::style(std::string("+").append(std::string(width - 2, '-')).append("+"), border_color);

	std::cout << horizontal_line << '\n';
	upper(header);
	std::cout << border << kt::format_str(" {}{}", header, std::string(width - 3 - header.length(), ' ')) << border << '\n';

	// clang-format off
	auto padded_string = [](int i, std::string str, std::string limits) {
		return kt::format_str("{} {}. {}{}{}", limits, i, str, std::string(54 - str.length(), ' '), limits);
	};
	// clang-format on

	for (std::size_t i = 0; i < table.size(); ++i)
	{
		lower(table[i]);
		std::cout << padded_string(i + 1, table[i], border) << '\n';
	}

	std::cout << horizontal_line << '\n';
}

Manifest check_manifest(const std::filesystem::path& path)
{
	Manifest manifest{};

	if (std::filesystem::exists(path))
	{
		auto const file = read_file(path);
		auto const str = std::accumulate(file.begin(), file.end(), std::string(""));
		dj::json_t json;
		if (json.read(str) && json.is_object())
		{
			manifest.game_ready = json["game_ready"].as<bool>();
			manifest.duplicates = json["duplicates"].as<std::vector<int>>();
			manifest.files = json["files"].as<std::vector<std::string>>();
			manifest.asset_dir = path.parent_path() / std::filesystem::path("assets");
		}
	}
	else
	{
		std::cerr << "File not found: " << path.string() << '\n';
	}

	return manifest;
}
} // namespace utils
