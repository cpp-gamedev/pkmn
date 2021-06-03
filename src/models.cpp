#include <algorithm>
#include <numeric>
#include <vector>

#include <str_format/str_format.hpp>
#include <dumb_json/json.hpp>

#include "models.hpp"
#include "utils.hpp"


namespace models
{
std::vector<std::string> Pokemon::read_asset(std::string ext)
{
	return utils::read_file(assets_dir / ((ext == "txt") ? "textures" : "data") / kt::format_str("{}.{}", std::to_string(this->id), ext));
}

void Pokemon::configure_move_set()
{
	this->move_set = utils::random_choices(this->all_moves, 4);

	for (Move& move : this->move_set)
	{
		// NOTE: flavor_text can never be longer than 54 chars, or else it breaks the table formatting in print_move_table @anim.cpp

		if (move.power >= 20 && move.accuracy > 0)
		{
			move.type = MoveType::ATTACK;
			move.power += this->difficulty == Difficulty::EASY ? -20 : (this->difficulty == Difficulty::MODERATE) ? 0 : 20;
			move.flavor_text = kt::format_str("{} deals {} points in damage.", move.name, move.power);
		}
		else if (move.accuracy == 0)
		{
			move.type = utils::random_choice(std::vector<MoveType>{MoveType::BOOST_ATK, MoveType::BOOST_DEF});
			move.accuracy = 100;
			move.power = (this->difficulty <= Difficulty::MODERATE) ? 10 : 20;
			move.flavor_text = kt::format_str("Boost your {} by {}%.", move.type == MoveType::BOOST_ATK ? "ATK" : "DEF", move.power);
		}
		else
		{
			move.type = MoveType::HEAL;
			move.accuracy = 100;
			move.power = utils::random_range<int>(4, 9) * 10;
			move.flavor_text = kt::format_str("Increase your HP by {} points.", move.power);
		}
	}
}

Pokemon::Pokemon(int id, std::filesystem::path assets_dir, Difficulty difficulty) : assets_dir{assets_dir}, id{id}, difficulty{difficulty}
{
	this->sprite = read_asset("txt");
	auto lines = read_asset("json");
	this->json_str = std::accumulate(lines.begin(), lines.end(), std::string(""));
	dj::json_t json;
	if (json.read(this->json_str) && json.is_object())
	{
		this->id = id;
		this->name = json["name"].as<std::string>();
		this->level = json["level"].as<int>();
		this->hp = json["hp"].as<int>();
		this->max_hp = this->hp;
		this->atk = json["atk"].as<int>();
		this->def = json["def"].as<int>();

		if (auto moves = json.find("moves"))
		{
			for (auto const& [num, m] : moves->as<dj::map_t>())
			{
				dj::json_t& move = *m;

				// clang-format off
				this->all_moves.push_back(
					Move{
						move["name"].as<std::string>(),
						MoveType::NONE, move["accuracy"].as<int>(),
						move["effect_chance"].as<int>(),
						move["power"].as<int>(),
						utils::random_choice(move["flavor_text_entries"].as<std::vector<std::string>>())
					}
				);
				// clang-format on
			}
		}
		this->configure_move_set();
	}
}
} // namespace models
