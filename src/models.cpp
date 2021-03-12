#include "models.hpp"
#include <vector>
#include "utils.hpp"
#include < iostream>

struct Move
{
	std::string move_name;
	Status status_effect;
	float accuracy;
	Move_types mt;
};

struct Pokemon
{
	std::string Name;
	int level;
	int hp = 2 * level * random_range(0.8, 1.2);
	int def;
	int attack;
	Status status = Status::None;
	std::vector<Move> moveset;
	Move next_move;
	void display_stats()
	{
		std::cout << "Name: " << this->Name << std::endl;
		std::cout << "Level: " << this->level << std::endl;
		std::cout << "Health: " << this->hp << std::endl;
		std::cout << "Defense: " << this->def << std::endl;
		std::cout << "Attack: " << this->attack << std::endl;
		for (auto mv : moveset)
		{
			std::cout << mv.move_name << std::endl;
		}
	}
	void select_move()
	{
		int move_number;
		std::cout << "1. display stats\n";

		for (int i = 0; i < moveset.size(); ++i)
		{
			std::cout << i + 2 << moveset[i].move_name << '\n';
		}
		std::cin >> move_number;
		if (move_number == 1)
		{
			display_stats();
		}
		else
		{
			this->next_move = moveset[move_number - 2];
		}
	}
	void makeMove(Pokemon* source, Pokemon* target, Move move)
	{
		int success = (random_range(0, move.accuracy) <= move.accuracy);
		switch (move.mt)
		{
		case Move_types::Attack:

			if (target->hp > 0 && success)
			{

				int damage = source->attack * source->level / target->def;
				target->hp -= damage;
				if ((random_range(0, move.accuracy) <= move.accuracy))
				{
					target->status = move.status_effect;
				}
				std::cout << source->Name << " Inflicted " << damage << " damage to " << target->Name << " with " << move.move_name << std::endl;
			}
			else
			{
				std::cout << "Attack failed\n";
			}
			break;
		case Move_types::Defend:
			target = source;
			if (success)
			{
				int defense_increase = source->level * random_range(target->def * 0.2, target->def * 0.8);
				target->def += defense_increase;
				std::cout << "Defense increased by " << defense_increase << ". Total defense is: " << target->def << std::endl;
			}
			break;

		case Move_types::Heal:
			target = source;
			if (success)
			{
				int health_increase = source->level * random_range(target->hp * 0.1, target->hp * 0.5);
				target->hp += health_increase;
				std::cout << "Health increased by " << health_increase << ". Total Health is: " << target->hp << std::endl;
			}
			break;
		}
	}
};
