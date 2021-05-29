#include <algorithm>
#include <filesystem>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include <str_format/str_format.hpp>

#include "models.hpp"
#include "utils.hpp"

namespace anim
{
std::vector<std::string> gen_healthbar(models::Pokemon& pkmn)
{
	/*
	 * bulbasaur :L30	// label
	 * HP [********* ]	// progressbar
	 *         58 / 60	// hitpoints
	 */
	std::string label = kt::format_str("{} :L{}", pkmn.name, pkmn.level);
	std::size_t max_width = pkmn.name.length() > 10 ? label.length() : 15;

	label = std::string(max_width - label.length(), ' ').append(label);

	double linear_map = (10.0 / pkmn.max_hp) * pkmn.hp;
	int hp_scaled = (0 < linear_map && linear_map < 1) ? 1 : std::floor(linear_map);

	std::string stars = std::string(hp_scaled, '*');
	stars = stars.append(std::string(10 - hp_scaled, ' '));
	utils::Color star_color = (hp_scaled >= 5) ? utils::Color::GREEN : (3 < hp_scaled && hp_scaled < 5) ? utils::Color::YELLOW : utils::Color::RED;
	std::string progressbar = kt::format_str("HP [{}]", style(stars, star_color));
	progressbar = std::string(max_width - 15, ' ').append(progressbar);

	std::string hitpoints = kt::format_str("{} / {}", pkmn.hp, pkmn.max_hp);
	hitpoints = std::string(max_width - hitpoints.length(), ' ').append(hitpoints);

	return {label, progressbar, hitpoints};
}

void print_splash_screen(const std::filesystem::path& assets_dir)
{
	auto logo = utils::read_file(assets_dir / std::filesystem::path("splashscreen.txt"));
	std::cout << utils::style(std::accumulate(logo.begin(), logo.end(), std::string("")), utils::Color::YELLOW) << '\n';

	std::cout << '\n' << std::string(19, ' ');

	for (const char& c : "copyright (c) 2021 cpp-gamedev")
	{
		std::cout << c;
		utils::sleep(100);
	}
}

void print_frame(models::Pokemon& pkmn1, models::Pokemon& pkmn2)
{
	std::string healthbars{};
	std::string sprites{};

	std::vector<std::string> healthbar1 = gen_healthbar(pkmn1);
	std::vector<std::string> healthbar2 = gen_healthbar(pkmn2);

	std::size_t padding = healthbar1[0].length() + healthbar2[0].length();

	if (padding > 30)
		padding -= 2 * (padding - 30);

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
} // namespace anim
