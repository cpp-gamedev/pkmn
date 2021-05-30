#pragma once

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <random>
#include <type_traits>
#include <vector>

namespace utils
{
///
/// \brief Return a random value in range of [min, max]
///
template <typename T>
T random_range(T min, T max)
{
	// in range of [min, max]
	static std::default_random_engine eng{std::random_device{}()};
	if constexpr (std::is_integral_v<T>)
	{
		std::uniform_int_distribution<T> dist(min, max);
		return dist(eng);
	}
	else
	{
		std::uniform_real_distribution<T> dist(min, max);
		return dist(eng);
	}
}

///
/// \brief Return k random values in range of [min, max] (with repitition)
///
template <typename T>
std::vector<T> random_ranges(T min, T max, std::size_t k)
{
	std::vector<T> vector(k);
	std::generate(vector.begin(), vector.end(), [min, max]() { return random_range(min, max); });
	return vector;
}

///
/// \brief Return a random element from vector
///
template <typename T>
T random_choice(const std::vector<T>& vector)
{
	return vector[random_range<std::size_t>(0, vector.size() - 1)];
}

///
/// \brief Return k random elements from vector (with repitition)
///
template <typename T>
std::vector<T> random_choices(const std::vector<T>& vector, std::size_t k)
{
	std::vector<T> choices(k);
	std::generate(choices.begin(), choices.end(), [&]() { return random_choice(vector); });
	return choices;
}

template <typename T>
T get_user_input(std::string prompt)
{
	std::cout << prompt;
	T input{};
	std::cin >> input;
	return input;
}

void clear_screen();

void sleep(int ms);

enum class Color
{
	BLACK,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE
};

std::string style(std::string text, Color fore, Color back = Color::BLACK);

std::string upper(std::string& str);

std::string lower(std::string& str);

std::filesystem::path find_upwards(std::string dir_name, int max_depth = 10);

std::vector<std::string> read_file(const std::filesystem::path& path);

void print_enum_table(std::vector<std::string> table, std::string header);

struct Manifest
{
	Manifest() = default;
	std::vector<std::string> files{};
	std::vector<int> duplicates{};
	bool game_ready{};
	std::filesystem::path asset_dir{};
};

Manifest check_manifest(const std::filesystem::path& path);
} // namespace utils
