#include "models.hpp"
#include <iostream>
#include <vector>
#include "utils.hpp"

Move::Move(std::string move_name, const MoveTypes mt, const float accuracy, const int power)
	: m_move_name(std::move(move_name)), m_mt(mt), m_accuracy(accuracy), m_power(power)
{
}

Pokemon::Pokemon(std::string name, const std::int32_t level, const std::int32_t def, const std::int32_t attack, const Move next_move)
	: m_name(std::move(name)), m_level(level), m_hp(2 * m_level * random_range<float>(0.8, 1.2)), m_def(def), m_attack(attack), m_next_move(next_move)
{
}

Move Pokemon::make_move(Pokemon& source, Pokemon& target)
{
	if ((random_range<int>(0, 100) <= source.m_next_move.m_accuracy) && source.isAlive && target.isAlive)
	{
		switch (source.m_next_move.m_mt)
		{
		case MoveTypes::ATTACK:
			return attack(source, target);
		case MoveTypes::DEFEND:
			return defend(source);

		case MoveTypes::HEAL:
			return heal(source);

		case MoveTypes::NONE:
			return source.m_next_move;
		}
	}
	Move fail = Move("Failed", MoveTypes::NONE, 100, 0);
	return fail;
};
Move Pokemon::attack(Pokemon& source, Pokemon& target)
{
	/* (2Level/5+2)*Attack*Power)/Defense)/50)+2)*random number(217-255))/255
			damage formula https://www.math.miami.edu/~jam/azure/compendium/battdam.htm*/
	if (target.m_hp > 0)
	{
		int attack = source.m_attack;
		if (random_range<int>(random_range<int>(1, 20), 100))
		{
			attack *= 2; // crit damage
		}
		int damage = ((((((2 * source.m_level / 5 + 2) * attack * source.m_next_move.m_power) / target.m_def) / 50) + 2) * random_range<int>(217, 255)) / 255;
		target.m_hp -= damage;
		if (target.m_hp <= 0)
		{
			target.isAlive = false;
		}
		return source.m_next_move;
	}
	return Move("Failed", MoveTypes::NONE, 100, 0);
};
Move Pokemon::defend(Pokemon& source)
{

	int defense_increase = source.m_next_move.m_power * random_range<float>(0.1, 0.3);
	source.m_def += defense_increase;
	return source.m_next_move;
};
Move Pokemon::heal(Pokemon& source)
{

	int health_increase = source.m_next_move.m_power * random_range<float>(0.1, 0.5);
	if (source.m_hp + health_increase > source.m_max_hp)
	{
		source.m_hp = source.m_max_hp;
	}
	else
	{
		source.m_hp += health_increase;
	}
	return source.m_next_move;
};
