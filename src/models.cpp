#include "models.hpp"
#include <algorithm>
#include <numeric>
#include <vector>
#include "utils.hpp"
#include "str_format.hpp"
#include <dumb_json/djson.hpp>

Move::Move()
{

}

std::vector<std::string> Pokemon::read_asset(std::string ext)
{
	auto path = assets_dir / std::filesystem::path(kt::format_str("{}.{}", std::to_string(this->id), ext));
	return read_file(path);
}

Pokemon::Pokemon(int id, std::filesystem::path assets_dir) : id{id}, assets_dir{assets_dir}
{
	this->sprite = read_asset("txt");
	auto lines = read_asset("json");
	this->json = std::accumulate(lines.begin(), lines.end(), std::string(""));
	if (auto n = dj::node_t::make(this->json))
	{
		// pkmn data
		dj::node_t& node = *n;
		this->name = node["name"].as<std::string>();
		this->hp = node["hp"].as<int>();
		this->max_hp = this->hp;
		this->atk = node["atk"].as<int>();
		this->def = node["def"].as<int>();
		// move data
		// TODO
	}
}
