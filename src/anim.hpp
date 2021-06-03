#pragma once

#include<string>
#include<vector>

#include "models.hpp"

namespace anim
{
std::vector<std::string> gen_healthbar(const models::Pokemon& pkmn);

void print_splash_screen(const std::filesystem::path& assets_dir);

void print_move_table(const models::Pokemon& pkmn);

std::vector<models::Pokemon> load_main_menu(const utils::Manifest& manifest);

int print_frame(const models::Pokemon& pkmn1, const models::Pokemon& pkmn2);
} // namespace anim
