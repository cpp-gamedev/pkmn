#include <algorithm>
#include <filesystem>
#include <iostream>
#include <numeric>
#include <string>

#include "anim.hpp"
#include "models.hpp"
#include "utils.hpp"

using namespace anim;
using namespace models;
using namespace utils;

int main()
{
	const auto assets_dir = find_upwards("assets");
	Manifest manifest = check_manifest(assets_dir.parent_path() / "manifest.json");

	if (std::filesystem::exists(assets_dir) && manifest.game_ready)
	{
		print_splash_screen(assets_dir);
		clear_screen();

		auto pkmns = load_main_menu(manifest);
		sleep(1000);
		clear_screen();

		std::cout << "TODO: init game loop" << '\n';
		sleep(1000);
		clear_screen();

		print_frame(pkmns[0], pkmns[1]);

		return EXIT_SUCCESS;
	}
	else
	{
		std::cerr << "Error: The assets directory is in an invalid state. Consult the README for further instructions." << '\n';
		return EXIT_FAILURE;
	}
}