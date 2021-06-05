#include "anim.hpp"
#include "models.hpp"
#include "utils.hpp"

using namespace anim;
using namespace models;
using namespace utils;

int main()
{
	constexpr std::chrono::milliseconds delay{1000};
	const auto assets_dir = find_upwards("assets");
	Manifest manifest = check_manifest(assets_dir.parent_path() / "manifest.json");

	if (std::filesystem::exists(assets_dir) && manifest.game_ready)
	{
		print_splash_screen(assets_dir);
		clear_screen();

		auto pkmns = load_main_menu(manifest);
		auto& [player, ai] = pkmns;
		clear_screen();

		while (player.hp > 0 && ai.hp > 0)
		{
			player.make_move(ai, print_frame(player, ai));

			if (ai.hp > 0)
			{
				sleep(delay);
				ai.make_move(player, random_range<std::size_t>(1, 4));
				clear_screen();
			}

		}

		clear_screen();
		slow_print((ai.hp == 0) ? "You Won :)" : "You Lost :(", delay / 20);

		return EXIT_SUCCESS;
	}
	else
	{
		std::cerr << "Error: The assets directory is in an invalid state. Consult the README for further instructions." << '\n';
		return EXIT_FAILURE;
	}
}