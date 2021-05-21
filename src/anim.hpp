#pragma once

#include<string>
#include<vector>

std::vector<std::string> gen_healthbar(std::string name, int level, int hp, int max_hp);

std::vector<std::string> load_sprite(int id, const std::filesystem::path& assets);