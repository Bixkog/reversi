#include "mcts_agent.h"
#include "match.h"
#include "game.h"
#include <utility>
#include <limits>
#include <algorithm>

MCTS_Agent::MCTS_Agent(Piece turn)
: root(new g_node(Game(), Pos(), Piece::white, nullptr)),
	turn(turn),
	g(rd()),
	w_player(Piece::white, 12),
	b_player(Piece::black, 13)
{
	root->expand();
}

int K = 1000;

std::optional<Pos> 
MCTS_Agent::make_move(const Game& game) const
{
	auto pos_moves = game.possible_moves(turn);
	if(pos_moves.empty())
		return {};
	// get new root
	auto game_node = root->find_game(game);
	if(game_node != root)
	{
		if(!game_node)
		{
			delete root;
			root = new g_node(game, Pos(), turn, nullptr);
		}
		else
		{
			game_node->cut_off();
			delete root;
			root = game_node;
		}
	}
	// get children
	root->expand();
	// calculate games
	for(int i = 0; i < K;)
	{
		for(auto& succ : root->succs)
		{
			// selection
			auto leaf = succ->get_leaf(root->denominator);
			// expansion
			if(leaf->denominator > 0)
			{
				leaf->expand();
				leaf = leaf->get_leaf(root->denominator);
			}
			// playout
			Match random_match{leaf->game};
			int res;
			if(leaf->turn == Piece::white)
				res = random_match.play(w_player, b_player);
			else
				res = random_match.play(b_player, w_player);
			// backpropagation
			if(res == 0)
				leaf->backprop(rand()%2);
			else if(res == 1?
					leaf->turn==Piece::white : 
					leaf->turn==Piece::black)
				leaf->backprop(true);
			else
				leaf->backprop(false);

			i++;
		}
	}
	auto best_succ = root->best_succ();
	best_succ->cut_off();
	delete root;
	root = best_succ;
	root->expand();
	return root->move;
}