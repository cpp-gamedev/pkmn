#pragma once

#include <random>
#include <vector>
#include "utils.hpp"

enum class MoveTypes
{
	NONE,	// move failed or wasn't initialized
	ATTACK, // deals damange
	DEFEND, // increase pokemon defense
	HEAL,	// Increase pokemon health or remove status effects
};

struct Move
{
	std::string m_move_name;
	MoveTypes m_mt;
	float m_accuracy;
	int m_power;
	Move(std::string move_name, MoveTypes mt, float accuracy, int power);
};

struct Pokemon
{
	std::string m_name;

	std::int32_t m_level = 1;
	std::int32_t m_hp = 4 * m_level * random_range(0.8, 1.2);
	std::int32_t m_max_hp = m_hp;
	std::int32_t m_def{};
	std::int32_t m_attack{};
	bool isAlive = true;

	std::vector<Move> m_moveset;
	Move m_next_move = Move("None", MoveTypes::NONE, 100, 0);

	Pokemon() = default; // default constructor can be compiler generated since you are using default member initialization
	Pokemon(std::string Name, int level, int def, int attack, Move next_move);
	Move make_move(Pokemon& source, Pokemon& target);
	Move attack(Pokemon& source, Pokemon& target);
	Move defend(Pokemon& source);
	Move heal(Pokemon& source);
};