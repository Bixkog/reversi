#include "game.h"
#include <cstring>

Piece flip(Piece p)
{
	return Piece(p ^ 1);
}

Game::Game()
{
	for(size_t i = 0; i < SIZE; ++i)
		for(size_t j = 0; j < SIZE; ++j)
			board[i][j] = Piece::empty;
	board[SIZE/2 - 1][SIZE/2 - 1] = Piece::black;
	board[SIZE/2][SIZE/2] = Piece::black;
	board[SIZE/2 - 1][SIZE/2] = Piece::white;
	board[SIZE/2][SIZE/2 - 1] = Piece::white;
}

bool Game::operator == (const Game& other) const
{
	for(int i = 0; i < 8; ++i)
		for(int j = 0; j < 8; ++j)
			if(board[i][j] != other.board[i][j])
				return false;
	return true;
}

bool Game::is_pos_move(uint i, uint j, Piece turn) const
{
	auto c_piece = board[i][j];
	if((c_piece & 2) == 0)
		return false;

	// right
	auto prev = Piece::empty;
	for(uint k = 1; k < SIZE-j; ++k)
	{
		auto temp = board[i][j+k];
		if(temp & 2)
			break;
		if(temp != turn)
			prev = temp;
		else
		{
			if(prev & 2)
				break;
			else
				return true;
		}
	}
	
	// left
	prev = Piece::empty;
	for(uint k = 1; k <= j; ++k)
	{
		auto temp = board[i][j-k];
		if(temp & 2)
			break;
		if(temp != turn)
			prev = temp;
		else
		{
			if(prev & 2)
				break;
			else
				return true;
		}
	}

	// up
	prev = Piece::empty;
	for(uint k = 1; k < SIZE-i; ++k)
	{
		auto temp = board[i+k][j];
		if(temp & 2)
			break;
		if(temp != turn)
			prev = temp;
		else
		{
			if(prev & 2)
				break;
			else
				return true;
		}
	}

	// down
	prev = Piece::empty;
	for(uint k = 1; k <= i; ++k)
	{
		auto temp = board[i-k][j];
		if(temp & 2)
			break;
		if(temp != turn)
			prev = temp;
		else
		{
			if(prev & 2)
				break;
			else
				return true;
		}
	}

	uint ind_max = std::max(i, j);
	uint ind_min = std::min(i, j);
	// right-down
	prev = Piece::empty;
	for(uint k = 1; k < SIZE - ind_max; ++k)
	{
		auto temp = board[i+k][j+k];
		if(temp & 2)
			break;
		if(temp != turn)
			prev = temp;
		else
		{
			if(prev & 2)
				break;
			else
				return true;
		}
	}

	// right-up
	prev = Piece::empty;
	for(uint k = 1; k < SIZE - j and k <= i; ++k)
	{
		auto temp = board[i-k][j+k];
		if(temp & 2)
			break;
		if(temp != turn)
			prev = temp;
		else
		{
			if(prev & 2)
				break;
			else
				return true;
		}
	}

	// left-up
	prev = Piece::empty;
	for(uint k = 1; k <= ind_min; ++k)
	{
		auto temp = board[i-k][j-k];
		if(temp & 2)
			break;
		if(temp != turn)
			prev = temp;
		else
		{
			if(prev & 2)
				break;
			else
				return true;
		}
	}

	// left-down
	prev = Piece::empty;
	for(uint k = 1; k <= j and k < SIZE - i; ++k)
	{
		auto temp = board[i+k][j-k];
		if(temp & 2)
			break;
		if(temp != turn)
			prev = temp;
		else
		{
			if(prev & 2)
				break;
			else
				return true;
		}
	}
	return false;
}

std::vector<Pos> 
Game::possible_moves(Piece turn) const
{
	std::vector<Pos> moves;
	moves.reserve(SIZE*2);

	for(uint i = 0; i < SIZE; i++)
	{
		for(uint j = 0; j < SIZE; j++)
		{
			if(is_pos_move(i, j, turn))
				moves.push_back({i, j});
		}
	}

	return moves;
}

Game Game::apply_move(Pos p, Piece turn) const
{
	// if(!is_pos_move(p.x, p.y, turn))
	// 	throw std::logic_error("Applying impossible move.");
	Game new_game = *this;
	uint i = p.x;
	uint j = p.y;
	new_game.board[i][j] = turn;

	//right
	Pos e = {-1u, -1u};
	for(uint k = 1; k < SIZE-j; ++k)
	{
		auto temp = new_game.board[i][j+k];
		if(temp & 2) break;
		else if(temp != turn) continue;
		else
		{
			if(temp == turn)
				e = {i, j+k};
			break;
		}
	}
	if(e.x != -1u and e.y != -1u)
	{
		for(uint k = 1;; ++k)
		{
			auto& temp = new_game.board[e.x][e.y-k];
			if(temp != turn) temp = turn;
			else break;
		}
	}

	
	// left
	e = {-1u, -1u};
	for(uint k = 1; k <= j; ++k)
	{
		auto temp = new_game.board[i][j-k];
		if(temp & 2) break;
		else if(temp != turn) continue;
		else
		{
			if(temp == turn)
				e = {i, j-k};
			break;
		}
	}
	if(e.x != -1u and e.y != -1u)
	{
		for(uint k = 1;; ++k)
		{
			auto& temp = new_game.board[e.x][e.y+k];
			if(temp != turn) temp = turn;
			else break;
		}
	}

	// down
	e = {-1u, -1u};
	for(uint k = 1; k < SIZE-i; ++k)
	{
		auto temp = new_game.board[i+k][j];
		if(temp & 2) break;
		else if(temp != turn) continue;
		else
		{
			if(temp == turn)
				e = {i+k, j};
			break;
		}
	}
	if(e.x != -1u and e.y != -1u)
	{
		for(uint k = 1;; ++k)
		{
			auto& temp = new_game.board[e.x-k][e.y];
			if(temp != turn) temp = turn;
			else break;
		}
	}

	// up
	e = {-1u, -1u};
	for(uint k = 1; k <= i; ++k)
	{
		auto temp = new_game.board[i-k][j];
		if(temp & 2) break;
		else if(temp != turn) continue;
		else
		{
			if(temp == turn)
				e = {i-k, j};
			break;
		}
	}
	if(e.x != -1u and e.y != -1u)
	{
		for(uint k = 1;; ++k)
		{
			auto& temp = new_game.board[e.x+k][e.y];
			if(temp != turn) temp = turn;
			else break;
		}
	}

	uint ind_max = std::max(i, j);
	uint ind_min = std::min(i, j);
	// right-down
	e = {-1u, -1u};
	for(uint k = 1; k < SIZE-ind_max; ++k)
	{
		auto temp = new_game.board[i+k][j+k];
		if(temp & 2) break;
		else if(temp != turn) continue;
		else
		{
			if(temp == turn)
				e = {i+k, j+k};
			break;
		}
	}
	if(e.x != -1u and e.y != -1u)
	{
		for(uint k = 1;; ++k)
		{
			auto& temp = new_game.board[e.x-k][e.y-k];
			if(temp != turn) temp = turn;
			else break;
		}
	}

	// right-up
	e = {-1u, -1u};
	for(uint k = 1; k < SIZE-j and k <= i; ++k)
	{
		auto temp = new_game.board[i-k][j+k];
		if(temp & 2) break;
		else if(temp != turn) continue;
		else
		{
			if(temp == turn)
				e = {i-k, j+k};
			break;
		}
	}
	if(e.x != -1u and e.y != -1u)
	{
		for(uint k = 1;; ++k)
		{
			auto& temp = new_game.board[e.x+k][e.y-k];
			if(temp != turn) temp = turn;
			else break;
		}
	}

	// left-up
	e = {-1u, -1u};
	for(uint k = 1; k <= ind_min; ++k)
	{
		auto temp = new_game.board[i-k][j-k];
		if(temp & 2) break;
		else if(temp != turn) continue;
		else
		{
			if(temp == turn)
				e = {i-k, j-k};
			break;
		}
	}
	if(e.x != -1u and e.y != -1u)
	{
		for(uint k = 1;; ++k)
		{
			auto& temp = new_game.board[e.x+k][e.y+k];
			if(temp != turn) temp = turn;
			else break;
		}
	}

	// left-down
	e = {-1u, -1u};
	for(uint k = 1; k < SIZE-i and k <= j; ++k)
	{
		auto temp = new_game.board[i+k][j-k];
		if(temp & 2) break;
		else if(temp != turn) continue;
		else
		{
			if(temp == turn)
				e = {i+k, j-k};
			break;
		}
	}
	if(e.x != -1u and e.y != -1u)
	{
		for(uint k = 1;; ++k)
		{
			auto& temp = new_game.board[e.x-k][e.y+k];
			if(temp != turn) temp = turn;
			else break;
		}
	}

	return new_game;
}

int Game::get_winner() const
{
	int whites = 0;
	int blacks = 0;
	for(uint i = 0; i < SIZE; ++i)
		for(uint j = 0; j < SIZE; ++j)
			if(board[i][j] == Piece::white)
				whites++;
			else if(board[i][j] == Piece::black)
				blacks++;
	if(whites > blacks)
		return 1;
	else if(whites < blacks)
		return -1;
	else
		return 0;
}