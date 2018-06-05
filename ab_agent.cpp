#include "ab_agent.h"
#include <utility>
#include <limits>
#include <algorithm>

AB_Agent::AB_Agent(Piece turn)
: turn(turn),
	g(rd())
{
}

int AB_Agent::heuristic(const Game& game) const
{
	Piece ag_color = turn;
	Piece en_color = flip(turn);
	int X[] = {-1, -1, 0, 1, 1, 1, 0, -1};
	int Y[] = {0, 1, 1, 1, 0, -1, -1, -1};
	int V[8][8] = {
	{20, -3, 11, 8, 8, 11, -3, 20},
	{-3, -7, -4, 1, 1, -4, -7, -3},
	{11, -4, 2, 2, 2, 2, -4, 11},
	{8, 1, 2, -3, -3, 2, 1, 8},
	{8, 1, 2, -3, -3, 2, 1, 8},
	{11, -4, 2, 2, 2, 2, -4, 11},
	{-3, -7, -4, 1, 1, -4, -7, -3},
	{20, -3, 11, 8, 8, 11, -3, 20}};

	// disks value and quantity
	int val = 0;
	int e_quant = 0, a_quant = 0;
	for(int i = 0; i < 8; ++i)
		for(int j = 0; j < 8; ++j)
			if(game.board[i][j] == ag_color)
			{
				val += V[i][j];
				a_quant++;
			}
			else if(game.board[i][j] == en_color)
			{
				val -= V[i][j];
				e_quant++;
			}

	double quant;
	if(a_quant > e_quant)
		quant = (100 * a_quant) / (double)(a_quant + e_quant);
	else if(a_quant < e_quant)
		quant = (-100 * e_quant) / (double)(a_quant + e_quant);
	else
		quant = 0;

	// front disks
	int e_front = 0, a_front = 0;
	for(int i = 0; i < 8; ++i)
		for(int j = 0; j < 8; ++j)
			if((game.board[i][j] & 2) == 0)
			{
				for(int k = 0; k < 8; ++k)
				{
					int x = i + X[k], y = j + Y[k];
					if(x >= 0 and y >= 0 and y < 8 and x < 8 
						and game.board[x][y] & 2)
					{
						if(game.board[i][j] == ag_color) a_front++;
						else e_front++;
						break;
					}
				}	
			}	

	double front;
	if(a_front > e_front)
		front = (-100 * a_front) / (double)(a_front + e_front);
	else if(a_front < e_front)
		front = (100 * e_front) / (double)(a_front + e_front);
	else
		front = 0;

	// corner occupancy
	int e_corner = 0, a_corner = 0;
	if(game.board[0][0] == ag_color) a_corner++;
	else if(game.board[0][0] == en_color) e_corner++;
	if(game.board[0][7] == ag_color) a_corner++;
	else if(game.board[0][7] == en_color) e_corner++;
	if(game.board[7][0] == ag_color) a_corner++;
	else if(game.board[7][0] == en_color) e_corner++;
	if(game.board[7][7] == ag_color) a_corner++;
	else if(game.board[7][7] == en_color) e_corner++;
	double corner = 25 * (a_corner - e_corner);

	// Corner closeness
	int e_close = 0, a_close = 0;
	if(game.board[0][0] & 2)   {
		if(game.board[0][1] == ag_color) a_close++;
		else if(game.board[0][1] == en_color) e_close++;
		if(game.board[1][1] == ag_color) a_close++;
		else if(game.board[1][1] == en_color) e_close++;
		if(game.board[1][0] == ag_color) a_close++;
		else if(game.board[1][0] == en_color) e_close++;
	}
	if(game.board[0][7] & 2)   {
		if(game.board[0][6] == ag_color) a_close++;
		else if(game.board[0][6] == en_color) e_close++;
		if(game.board[1][6] == ag_color) a_close++;
		else if(game.board[1][6] == en_color) e_close++;
		if(game.board[1][7] == ag_color) a_close++;
		else if(game.board[1][7] == en_color) e_close++;
	}
	if(game.board[7][0] & 2)   {
		if(game.board[7][1] == ag_color) a_close++;
		else if(game.board[7][1] == en_color) e_close++;
		if(game.board[6][1] == ag_color) a_close++;
		else if(game.board[6][1] == en_color) e_close++;
		if(game.board[6][0] == ag_color) a_close++;
		else if(game.board[6][0] == en_color) e_close++;
	}
	if(game.board[7][7] & 2)   {
		if(game.board[6][7] == ag_color) a_close++;
		else if(game.board[6][7] == en_color) e_close++;
		if(game.board[6][6] == ag_color) a_close++;
		else if(game.board[6][6] == en_color) e_close++;
		if(game.board[7][6] == ag_color) a_close++;
		else if(game.board[7][6] == en_color) e_close++;
	}
	double close = -12.5 * (a_close - e_close);

	// mobility
	int a_moves = game.possible_moves(ag_color).size();
	int e_moves = game.possible_moves(en_color).size();
	double mobility;
	if(a_moves > e_moves)
		mobility = (100.0 * a_moves) / (double)(a_moves + e_moves);
	else if(a_moves < e_moves)
		mobility = -(100.0 * e_moves) / (double)(a_moves + e_moves);
	else mobility = 0;

	double score = (10 * quant) + 
				   (801.724 * corner) + 
				   (382.026 * close) + 
				   (78.922 * mobility) + 
				   (74.396 * front) + 
				   (10 * val);
	return score;
}

std::vector<Pos> AB_Agent::sort_moves(
	const Game& node, const std::vector<Pos>& moves, bool maxPlayer) const
{
	auto c_piece = maxPlayer?turn:flip(turn);
	std::vector<std::pair<int, Pos>> moves_h;
	moves_h.reserve(moves.size());
	for(auto& m : moves)
		moves_h.emplace_back(heuristic(node.apply_move(m, c_piece)), m);

	auto cmp = [](const auto& l, const auto& r) {return l.first < r.first;};
	if(maxPlayer)
		std::sort(moves_h.rbegin(), moves_h.rend(), cmp);
	else
		std::sort(moves_h.begin(), moves_h.end(), cmp);

	std::vector<Pos> s_moves; 
	s_moves.reserve(moves.size());
	for(auto& p : moves_h)
		s_moves.push_back(p.second);

	return s_moves;
}

int AB_Agent::alfabeta(
	const Game& node, int depth, 
	int alfa, int beta, bool maxPlayer) const
{
	auto c_piece = maxPlayer?turn:flip(turn);
	if(depth == 0)
		return heuristic(node);
	
	auto pos_moves = node.possible_moves(c_piece);
	if(pos_moves.empty())
		return heuristic(node);
	
	int v;
	// pos_moves = sort_moves(node, pos_moves, maxPlayer);
	std::shuffle(std::begin(pos_moves), std::end(pos_moves), g);

	if(maxPlayer)
	{
		v = std::numeric_limits<int>::min();
		for(auto& move : pos_moves)
		{
			auto child = node.apply_move(move, c_piece);
			v = std::max(v, alfabeta(child, depth-1, alfa, beta, false));
			alfa = std::max(alfa, v);
			if (beta <= alfa)
				break;
		}
	}
	else
	{
		v = std::numeric_limits<int>::max();
		for(auto& move : pos_moves)
		{
			auto child = node.apply_move(move, c_piece);
			v = std::min(v, alfabeta(child, depth-1, alfa, beta, true));
			beta = std::min(beta, v);
			if (beta <= alfa)
				break;
		}
	}
	return v;
}

static
int depth = 7;

std::optional<Pos> 
AB_Agent::make_move(const Game& node) const
{
	auto pos_moves = node.possible_moves(turn);
	if(pos_moves.empty())
		return {};
	// pos_moves = sort_moves(node, pos_moves, true);
	std::shuffle(std::begin(pos_moves), std::end(pos_moves), g);
	int alfa = std::numeric_limits<int>::min();
	int beta = std::numeric_limits<int>::max();
	int v = std::numeric_limits<int>::min();
	Pos best_move;
	for(auto& move : pos_moves)
	{
		auto child = node.apply_move(move, turn);
		int cand = alfabeta(child, depth-1, alfa, beta, false);
		if(cand >= v)
		{
			v = cand;
			best_move = move;
		}
		alfa = std::max(alfa, v);
	}
	return best_move;
}