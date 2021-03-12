#pragma once

#include <random>
#include <vector>

struct Pokemon;
struct Move;

enum class Status
{
	None, // no status, default value
	Heal, // heal the user. Can only be used on oneself
	/* A paralyzed Pokémon has a 25% chance of not being able to attack every turn and their Speed is decreased by 50% */
	Paralyzed,
	Poisoned,		/* A poisoned Pokémon loses a set amount of HP every turn. */
	Badly_Poisoned, /* A badly poisoned Pokémon is damaged every turn, but this damage increases over time. The first turn, the poisoned Pokémon takes 1/16 of
					   the maximum HP in damage. That damage increases by an additional 1/16 of the max HP each turn. */
	Burned,			/* Burned Pokémon loose 1/8 of their maximum HP at the end of each turn in addition having their Attack stat reduced by 50%.  */
	Frozen,			/* A frozen Pokémon cannot attack, but it can thaw out in a few turns. Fire type attacks can thaw out a frozen Pokémon more easily. */
	Flinch,			/* A condition where the affected Pokémon is unable to use moves or attack during the turn it is affected. */
	Confused, /* Once confused, a Pokémon will remain so for 1 to 4 turns, and during those turns have a 50% chance of damaging themselves and being unable to
				 perform any other action for that turn.  */
	Infatuation, /* The Pokémon becomes infatuated with the opponent (who must be of an opposing gender) and is under a condition similar to confusion. */
	Leech_Seed	 /* The Pokémon loses 1/16th of his/her HP each turn, and the opponent gains what was lost */
};
enum class Move_types
{
	Attack, // deals damange
	Defend, // increase pokemon defense
	Heal,	// Increase pokemon health or remove status effects
};
void makeMove(Pokemon* source, Pokemon* target, Move move);
void select_move();
void display_stats();