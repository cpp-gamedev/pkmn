# PKMN

The purpose of this document is to outline the project goals and user requirements.
Not everything described here needs to be implemented, and you may also add new
features you don't find here. However, it is recommended to add the core mechanics
first, and minor details later to polish the finished project.

## Intro

PKMN is intended to be a turn-based CLI fighting game based on the original pokemon
franchise. The scope of the project is narrowed down to a 1vs1 encounter between
the player and an AI. Before the game starts, the user should be able to pick a
pokemon and select the level of difficulty.

![reference image](https://i.ytimg.com/vi/xKy1e4uUe7Q/hqdefault.jpg)

---

## Concept

### Models

For reasons of consistency, this game will use the same terminology as the original
game that you should keep in mind when you create these structures.

#### Move

A `move` in pokemon is a general term that describes in-battle actions which can
either change

- one's `hp`
- the enemy's `hp`
- one's `status`
- then enemy's `status`
- a combination of any of those, but never more than 2 at the same time

There're a few more things one could consider, but for this project these parameters
should suffice. In addition to that, each `move` has a `accuracy` between `30%` and
`100%`. Stronger moves are less likely to succeed, and vice versa.

#### Pokemon

- `name`
- `level` (between `1` and `100`)
- `hp` (short for hit points [[1](#hp)])
- `status` (poison, sleep, etc.)
- `moveset` (collection of all moves, at most 4)

In the early stages, pokemon don't know many moves. To replicate that, choose at
random

- 1 move if `level<5`
- 2 moves if `level<10`
- 3 moves if `level<20`
- else 4

An important method of this struct will be `select_move` which will take a user
input and invoke the selected move.

### UI

One of the major challenges while developing this game will be the fact that it's
not trivial to draw images in a terminal. For the sake of simplicity, convert the
original sprites [[2](#sprites)] into ASCII characters based on density. Because
of space limitations (small ASCII images generally look like a ball of
characters), use ANSI escape codes to color each "pixel" accordingly [[3](#ansii)].

### Animations

While not mandatory, it would be interesting to explore how to embed animations
into the game. Here are a few ideas from which you can draw inspiration:

- when the battle starts, fade in both characters
- the healthbar could change its state slowly rather than instantly
- the pokemon could move a little up and down when they take damage
- add text from the original game to annotate the players actions (e.g. *It's
  super effective!* when a pokemon receives critical damage)

### Fight Mechanism

Because this will be a turn-based fighting game, there will be a main event loop
that terminates when one of the two pokemon's life points were reduced to zero.
Because the player can initially set the level of difficulty, the following
parameters should be taken into account:

- add a transition to indicate the start of a battle [[4](#trans)]
- difference in level between both pokemons
- setting a stronger move set
- likelihood in with which moves succeed
- how much health an enemy can restore with a berry (if any)

---

## Coding Conventions

This code base will have a pre-made `.clang-format` to take care of most formatting
issues automatically. Use `snake_case` for function and variable names, and `CamelCase` for classes, structs, and so on.

## System Requirements

The game will be developed with os-independent dependencies in C++17. For starters
work without any external dependencies. This is not a hard rule, any may change in
the future if deemed necessary.

## References

- [<a id="hp">1</a>] In the original game the HP of a pokemon was influenced by
  various factors. For the sake of simplicity, assume for all pokemon that `hp=2*level*random_range(0.8,1.2)`
- [<a id="sprites">2</a>] <https://pokemondb.net/sprites>
- [<a id="ansi">3</a>] <https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797>
- [<a id="trans">4</a>] <https://youtu.be/YbDCXJ0xH2g>
