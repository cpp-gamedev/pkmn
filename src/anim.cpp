#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include "models.hpp"
#include "utils.hpp"
#include "str_format.hpp"

std::vector<std::string> gen_healthbar(Pokemon& pkmn)
{
	/*
	 * bulbasaur :L30	// label
	 * HP [********* ]	// progressbar
	 *         58 / 60	// hitpoints
	 */
	std::string label = kt::format_str("{} :L{}", pkmn.name, pkmn.level);
	std::size_t max_width = pkmn.name.length() > 10 ? label.length() : 15;

	label = std::string(max_width - label.length(), ' ').append(label);

	double percentage = ceil(static_cast<double>(pkmn.hp) / pkmn.max_hp * 100);
	int digits = static_cast<int>(log10(percentage));
	int first_digit = (pkmn.max_hp == pkmn.hp) ? 10 : static_cast<int>(percentage / pow(10, digits));

	std::string stars = std::string(first_digit, '*');
	stars = stars.append(std::string(10LL - first_digit, ' '));
	std::string progressbar = kt::format_str("HP [{}]", style(stars, Color::GREEN));
	progressbar = std::string(max_width - 15, ' ').append(progressbar);

	std::string hitpoints = kt::format_str("{} / {}", pkmn.hp, pkmn.max_hp);
	hitpoints = std::string(max_width - hitpoints.length(), ' ').append(hitpoints);

	return {label, progressbar, hitpoints};
}

void print_frame(Pokemon& pkmn1, Pokemon& pkmn2)
{
	std::string healthbars{};
	std::string sprites{};

	std::vector<std::string> healthbar1 = gen_healthbar(pkmn1);
	std::vector<std::string> healthbar2 = gen_healthbar(pkmn2);

	std::size_t padding = healthbar1[0].length() + healthbar2[0].length();

	if (padding > 30)
		padding -= 2*(padding - 30);

	for (std::size_t i = 0; i < 3; i++)
		healthbars.append(healthbar1[i]).append(std::string(padding, ' ')).append(healthbar2[i]).append("\n");

	std::cout << healthbars;

	for (std::size_t i = 0; i < 20; i++)
	{
		std::string tmp = pkmn1.sprite[i]; // strip new line
		tmp.erase(std::remove(tmp.begin(), tmp.end(), '\n'), tmp.end());
		sprites.append(tmp).append(std::string(20, ' ')).append(pkmn2.sprite[i]);
	}

	std::cout << sprites;

	return;
}
