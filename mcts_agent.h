#pragma once

#include "game.h"
#include "g_node.h"
#include "random_agent.h"
#include <optional>
#include <unordered_map>
#include <random>

struct MCTS_Agent
{
	MCTS_Agent(Piece turn);
	std::optional<Pos> make_move(const Game& game) const;

	mutable g_node* root;

	Piece turn;
	std::random_device rd;
    mutable std::mt19937 g;
    mutable R_Agent w_player;
    mutable R_Agent b_player;
};
