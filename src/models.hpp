#pragma once

#include <random>
#include <vector>
#include "utils.hpp"

enum class MoveType
{
	NONE,
	ATTACK,
	HEAL,
	BOOST_ATK,
	BOOST_DEF
};

struct Move
{
	std::string name{};
	MoveType type{MoveType::NONE};
	int accuracy{};
	int power{};
	int effect_chance{};
	std::string flavor_text{};
	Move() = default;
	Move(std::string name, MoveType move_type, int accuracy, int effect_chance, int power, std::string flavor_text);
};

struct Pokemon
{
  private:
	std::vector<std::string> read_asset(std::string ext);
	void configure_move_set();
	
	std::string json_str{};
	std::vector<Move> all_moves{};

  public:
	std::filesystem::path assets_dir{};

	std::vector<std::string> sprite{};
	int id{};
	std::string name{};
	int level{};
	int hp{};
	int max_hp{};
	int atk{};
	int def{};
	std::vector<Move> move_set{};

	Pokemon(int id, std::filesystem::path assets_dir);
};
