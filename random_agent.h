#pragma once
#include "game.h"
#include <random>
#include <optional>

struct R_Agent
{
	R_Agent(Piece turn, size_t seed);
	std::optional<Pos> make_move(const Game& game);

	Piece turn;
	size_t seed;
	std::default_random_engine gen;
};