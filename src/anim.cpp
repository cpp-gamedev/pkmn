#include <str_format/str_format.hpp>

#include <array>

#include "models.hpp"
#include "utils.hpp"

namespace anim
{
std::vector<std::string> gen_healthbar(const models::Pokemon& pkmn)
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
	auto logo = utils::read_file(assets_dir / "splashscreen.txt");
	std::cout << utils::style(std::accumulate(logo.begin(), logo.end(), std::string("")), utils::Color::YELLOW) << '\n';

	std::cout << '\n' << std::setfill(' ') << std::setw(19);

	for (const char& c : "copyright (c) 2021 cpp-gamedev")
	{
		std::cout << c;
		utils::sleep(100);
	}
}

void print_move_table(const models::Pokemon& pkmn)
{
	// +----------------------------------------------------------+
	// |    MOVE                                POWER    ACCURACY |
	// | 1. move1                                 100          90 |
	// |    first move description                                |
	// | 2. move2                                  90          50 |
	// |    second move description                               |
	// | 3. move3                                 120          10 |
	// |    third move description                                |
	// | 4. move4                                  30          70 |
	// |    fourth move description                               |
	// +----------------------------------------------------------+

	constexpr int width = 60;
	utils::Color border_color = utils::Color::YELLOW;
	std::string border = utils::style(std::string("|"), border_color);
	std::string horizontal_line = utils::style(std::string("+").append(std::string(width - 2, '-')).append("+"), border_color);

	std::cout << horizontal_line << '\n';
	std::cout << border << std::string(4, ' ') << "MOVE" << std::string(32, ' ') << "POWER" << std::string(4, ' ') << "ACCURACY" << ' ' << border << '\n';

	for (std::size_t i = 0; i < pkmn.move_set.size(); ++i)
	{
		// move name, power and accuracy
		std::string name = kt::format_str("{}. {}", i + 1, pkmn.move_set[i].name);
		std::cout << border << ' ' << utils::style(name, utils::Color::GREEN) << std::setfill(' ') << std::setw(width - 7 - name.length())
				  << utils::style(std::to_string(pkmn.move_set[i].power), utils::Color::RED) << std::setfill(' ') << std::setw(21)
				  << utils::style(std::to_string(pkmn.move_set[i].accuracy), utils::Color::CYAN) << ' ' << border << '\n';
		// flavor text
		std::cout << border << std::string(4, ' ') << pkmn.move_set[i].flavor_text << std::setfill(' ') << std::setw(width + 4 - pkmn.move_set[i].flavor_text.length())
				  << border << '\n';
	}

	std::cout << horizontal_line << '\n';
}

std::array<models::Pokemon, 2> load_main_menu(const utils::Manifest& manifest)
{
	// 1. set difficulty
	int selection{utils::validate_user_input({"easy", "moderate", "hard"}, "(1/2) set difficulty")};
	auto difficulty = (selection == 1) ? models::Difficulty::EASY : (selection == 2) ? models::Difficulty::MODERATE : models::Difficulty::HARD;

	// 2. instantiate all available pokemons
	std::vector<models::Pokemon> pkmns{};
	pkmns.reserve(manifest.duplicates.size());
	std::for_each(manifest.duplicates.begin(), manifest.duplicates.end(),
				  [&manifest, &pkmns, &difficulty](int id) { pkmns.push_back(models::Pokemon(id, manifest.asset_dir, difficulty)); });

	// 3. select pokemon
	std::vector<std::string> names{};
	names.reserve(pkmns.size());
	std::for_each(pkmns.begin(), pkmns.end(), [&names](models::Pokemon& pkmn) { names.push_back(pkmn.name); });
	selection = utils::validate_user_input(names, "(2/2) choose a pokemon");
	models::Pokemon player = pkmns[selection - 1];

	// 4. remove selection from pkmns, so that player won't fight against his doppelganger
	pkmns.erase(std::remove_if(pkmns.begin(), pkmns.end(), [player](models::Pokemon pkmn) { return player.id == pkmn.id; }), pkmns.end());
	return {player, pkmns.size() > 1 ? utils::random_choice(pkmns) : pkmns[0]};
}

int print_frame(const models::Pokemon& pkmn1, const models::Pokemon& pkmn2)
{
	std::string healthbars{};
	std::string sprites{};

	std::vector<std::string> healthbar1 = gen_healthbar(pkmn1);
	std::vector<std::string> healthbar2 = gen_healthbar(pkmn2);

	std::size_t padding = healthbar1[0].length() + healthbar2[0].length();

	if (padding > 30)
		padding -= 2 * (padding - 30);

	// gather healthbars
	for (std::size_t i = 0; i < 3; i++)
		healthbars.append(healthbar1[i]).append(std::string(padding, ' ')).append(healthbar2[i]).append("\n");

	// gather sprites
	for (std::size_t i = 0; i < 20; i++)
	{
		std::string tmp = pkmn1.sprite[i]; // strip new line
		tmp.erase(std::remove(tmp.begin(), tmp.end(), '\n'), tmp.end());
		sprites.append(tmp).append(std::string(20, ' ')).append(pkmn2.sprite[i]);
	}

	int answer{};
	std::vector<int> choices(pkmn1.move_set.size());
	std::iota(choices.begin(), choices.end(), 1);

	// read and return user-selected move
	while (std::find(choices.begin(), choices.end(), answer) == choices.end())
	{
		utils::clear_screen();
		std::cout << healthbars;
		std::cout << sprites;
		print_move_table(pkmn1);
		answer = utils::get_user_input<int>(">>> ");
		std::cin.clear();
		std::cin.ignore(utils::str_max, '\n');
	}

	return answer;
}
} // namespace anim
