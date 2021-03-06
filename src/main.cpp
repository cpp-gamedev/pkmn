#include "anim.hpp"
#include "models.hpp"
#include "utils.hpp"

using namespace std::chrono_literals;

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
		auto& [player, ai] = pkmns;

		while (player.hp > 0 && ai.hp > 0)
		{
			clear_screen();
			player.make_move(ai, print_frame(player, ai));

			if (ai.hp > 0)
			{
				sleep(1000ms);
				ai.make_move(player, random_range<std::size_t>(1, 4));
			}
		}

		std::cout << '\n';
		delayed_print((ai.hp == 0) ? "You Won :)" : "You Lost :(", 50ms);

		return EXIT_SUCCESS;
	}
	else
	{
		std::cerr << "Error: The assets directory is in an invalid state. Consult the README for further instructions." << '\n';
		return EXIT_FAILURE;
	}
}