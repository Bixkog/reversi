#pragma once

#include "game.h"
#include <optional>
#include <unordered_map>
#include <random>

struct AB_Agent
{
	AB_Agent(Piece turn);
	std::optional<Pos> make_move(const Game& game) const;

	int heuristic(const Game& game) const;
	std::vector<Pos> sort_moves(const Game& node, 
			const std::vector<Pos>& moves, bool maxPlayer) const;
	int alfabeta(const Game& node, int depth, 
						int alfa, int beta, bool maxPlayer) const;
	Piece turn;
	std::random_device rd;
    mutable std::mt19937 g;
};