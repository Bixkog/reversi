#include "random_agent.h"

R_Agent::R_Agent(Piece turn, size_t seed)
: turn(turn),
	seed(seed),
	gen(seed)
{
}

std::optional<Pos> R_Agent::make_move(const Game& game)
{
	auto possible_moves = game.possible_moves(turn);
	if(possible_moves.empty()) return {};
	std::uniform_int_distribution<int> distr(0, possible_moves.size()-1);
	return possible_moves[distr(gen)];
}