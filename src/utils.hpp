#pragma once

#include <algorithm>
#include <random>
#include <type_traits>
#include <vector>

template <typename T>
T random_range(T min, T max)
{
	// in range of [min, max]
	static std::default_random_engine eng{ std::random_device{}() };
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

template <typename T>
T random_choice(const std::vector<T>& vector)
{
	return vector[random_range<std::size_t>(0, vector.size() - 1)];
}

template <typename T>
std::vector<T> random_choices(const std::vector<T>& vector, T k)
{
	// select k random values in vector
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
int random_range(int min, int max);