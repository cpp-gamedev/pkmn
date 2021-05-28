#include "models.hpp"
#include <algorithm>
#include <numeric>
#include <vector>
#include "utils.hpp"
#include <str_format/str_format.hpp>
#include <dumb_json/json.hpp>

Move::Move(std::string name, MoveType type, int accuracy, int effect_chance, int power, std::string flavor_text)
	: name(name), type(type), accuracy(accuracy), power(power), effect_chance(effect_chance), flavor_text(flavor_text)
{
}

std::vector<std::string> Pokemon::read_asset(std::string ext)
{
	auto path = assets_dir / std::filesystem::path(kt::format_str("{}.{}", std::to_string(this->id), ext));
	return read_file(path);
}

void Pokemon::configure_move_set()
{
	this->move_set = random_choices(this->all_moves, 4);

	for (Move& move : this->move_set)
	{
		if (move.power > 20 && move.accuracy > 50)
		{
			move.type = MoveType::ATTACK;
			move.flavor_text = kt::format_str("{} deals {} points in damage.", move.name, move.power);
		}
		else if (move.power <= 20 && move.effect_chance >= 0)
		{
			move.type = random_choice(std::vector<MoveType>{MoveType::BOOST_ATK, MoveType::BOOST_DEF});
			move.flavor_text = kt::format_str("Boost your {} by 10%.", move.type == MoveType::BOOST_ATK ? "ATK" : "DEF");
		}
		else
		{
			move.type = MoveType::HEAL;
			move.accuracy = 100;
			move.power = random_range<int>(4, 9) * 10;
			move.flavor_text = kt::format_str("Increase your HP by {} points.", move.power);
		}
	}
}

Pokemon::Pokemon(int id, std::filesystem::path assets_dir) : assets_dir{assets_dir}, id{id}
{
	this->sprite = read_asset("txt");
	auto lines = read_asset("json");
	this->json_str = std::accumulate(lines.begin(), lines.end(), std::string(""));
	dj::json_t json;
	if (json.read(this->json_str) && json.is_object())
	{
		// pkmn data
		this->id = id;
		this->name = json["name"].as<std::string>();
		this->level = json["level"].as<int>();
		this->hp = json["hp"].as<int>();
		this->max_hp = this->hp;
		this->atk = json["atk"].as<int>();
		this->def = json["def"].as<int>();
		// move data
		if (auto moves = json.find("moves"))
		{
			for (auto const& [num, m] : moves->as<dj::map_t>())
			{
				dj::json_t& move = *m;

				this->all_moves.push_back(
					Move(
						move["name"].as<std::string>(),
						MoveType::NONE,
						move["accuracy"].as<int>(),
						move["effect_chance"].as<int>(),
						move["power"].as<int>(),
						random_choice(move["flavor_text_entries"].as<std::vector<std::string>>())
					)
				);
			}
		}
		this->configure_move_set();
	}
}
