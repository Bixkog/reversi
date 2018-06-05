#include "g_node.h"
#include <cmath>
#include <limits>

g_node::g_node(const Game& game, Pos move, Piece turn, g_node* pred)
	: game(game),
		move(move),
		turn(turn),
		pred(pred),
		numerator(0),
		denominator(0)
	{}

void g_node::backprop(bool win)
{
	numerator += win;
	denominator++;
	if(pred)
		pred->backprop(!win);
}

g_node* pick_succ(const std::vector<std::unique_ptr<g_node>>& succs, int N)
{
	if(succs.empty()) return nullptr; 
	double c = std::sqrt(2);
	double log_N = std::log(N);
	double best_res = 0;
	g_node* best_succ = &(*succs[0]);
	for(auto& succ : succs)
	{
		double res;
		if(succ->denominator == 0)
			res = std::numeric_limits<double>::max();
		else
			res = (double)succ->numerator / (double)succ->denominator +
					 c * std::sqrt(log_N / (double)succ->denominator);
		if(res >= best_res)
			best_succ = &(*succ);
	}
	return best_succ;
}

g_node* g_node::get_leaf(int N)
{
	if(succs.empty())
		return this;
	return pick_succ(succs, N)->get_leaf(N);
}

void g_node::expand()
{
	if(!succs.empty())
		return;
	auto pos_moves = game.possible_moves(turn);
	std::vector<Game> pos_games;
	pos_games.reserve(pos_moves.size());
	for(auto& pos_move : pos_moves)
		pos_games.push_back(game.apply_move(pos_move, turn));
	for(size_t i = 0; i < pos_games.size(); ++i)
		succs.emplace_back(new g_node(pos_games[i], pos_moves[i], flip(turn), this));
}

g_node* g_node::find_game(const Game& target)
{
	if(game == target)
		return this;
	for(auto& succ : succs)
		if(auto res = succ->find_game(target); res)
			return res;
	return nullptr;
}

void g_node::cut_off()
{
	if(!pred) return;
	for(auto& cand : pred->succs)
		if(&(*cand)==this)
		{
			cand.release();
			break;
		}
	pred = nullptr;
}

g_node* g_node::best_succ()
{
	if(succs.empty()) return nullptr;
	g_node* best = &(*succs[0]);
	for(auto& succ : succs)
		if(succ->numerator * best->denominator > best->numerator * succ->denominator)
			best = &(*succ);
	return &(*best);
}