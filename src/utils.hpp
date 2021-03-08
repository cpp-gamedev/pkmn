#pragma once

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
T get_user_input(std::string prompt)
{
	std::cout << prompt;
	T input{};
	std::cin >> input;
	return input;
}

void clear_screen();