#include <algorithm>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <thread>
#include "utils.hpp"
#include <str_format/str_format.hpp>
#include <dumb_json/json.hpp>

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

std::filesystem::path find_upwards(std::string dir_name, int max_depth)
{
	auto path = std::filesystem::current_path() / std::filesystem::path(dir_name);

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
		}
	}
	else
	{
		std::cerr << "File not found: " << path.string() << '\n';
	}

	return manifest;
}
