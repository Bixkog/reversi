#pragma once

#include "game.h"
#include <vector>
#include <memory>

struct g_node
{
	g_node(const Game& game, Pos move, Piece turn, g_node* pred);

	void backprop(bool win);
	g_node* get_leaf(int N);
	void expand();
	g_node* find_game(const Game& target);
	void cut_off();
	g_node* best_succ();

	const Game game;
	const Pos move;
	const Piece turn;
	g_node* pred;
	std::vector<std::unique_ptr<g_node>> succs;
	int numerator;
	int denominator;
};