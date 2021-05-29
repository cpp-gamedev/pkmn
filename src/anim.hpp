#pragma once

#include<string>
#include<vector>

#include "models.hpp"

namespace anim
{
std::vector<std::string> gen_healthbar(models::Pokemon& pkmn);

void print_splash_screen(const std::filesystem::path& assets_dir);

void print_frame(models::Pokemon& pkmn1, models::Pokemon& pkmn2);
} // namespace anim
