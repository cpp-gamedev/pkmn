#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include "str_format.hpp"
#include "utils.hpp"

// TODO: update args to Pokemon object
std::vector<std::string> gen_healthbar(std::string name, int level, int hp, int max_hp)
{
	/*
	 * Bulbasaur :L30	// label
	 * HP [********* ]	// progressbar
	 *         58 / 60	// hitpoints
	 */
	std::string label = kt::format_str("{} :L{}\n", name, level);

	double percentage = ceil(static_cast<double>(hp) / max_hp * 100);
	int digits = static_cast<int>(log10(percentage));
	int first_digit = static_cast<int>(percentage / pow(10, digits));

	std::string stars = std::string(first_digit, '*');
	stars = stars.append(std::string(10LL - first_digit, ' '));
	std::string progressbar = kt::format_str("HP [{}]\n", style(stars, Color::GREEN));

	std::string hitpoints = kt::format_str("{} / {}\n", hp, max_hp);
	hitpoints = std::string(16 - hitpoints.length(), ' ').append(hitpoints);

	return {label, progressbar, hitpoints};
}

std::vector<std::string> load_sprite(int id, const std::filesystem::path& assets)
{
	for (const auto& file : std::filesystem::directory_iterator(assets))
	{
		if (file.path().extension() == ".txt" && std::stoi(file.path().stem().string()) == id)
		{
			return read_file(file);
		}
	}
	return {};
}
