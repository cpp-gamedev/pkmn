#pragma once

#include <random>
#include <vector>
#include "utils.hpp"

enum class MoveType
{
	NONE,
	ATTACK,
	DEFEND,
	HEAL,
};

struct Move
{
	std::string name{};
	MoveType move_type{MoveType::NONE};
	int accuracy{};
	int damage{};
	Move();
};

struct Pokemon
{
  private:
	std::vector<std::string> read_asset(std::string ext);
	
	std::string json{};
	std::vector<std::string> sprite{};

  public:
	std::filesystem::path assets_dir{};

	const int id{};
	std::string name{};
	int level{};
	int hp{};
	int max_hp{};
	int atk{};
	int def{};
	std::vector<Move> move_set{};

	Pokemon(int id, std::filesystem::path assets_dir);
};
