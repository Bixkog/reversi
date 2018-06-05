#pragma once
#include <iostream>
#include <vector>

constexpr size_t SIZE = 8;

enum Piece : int
{
	white = 0,
	black = 1,
	empty = 2
};

struct Pos
{
	uint x;
	uint y;
	
	friend 
	std::ostream& operator << (std::ostream& out, const Pos& p)
	{
		out << "(" << p.x << ", " << p.y << ")";
		return out;
	}
};

struct Game
{
	Game();

	friend
	std::ostream& operator << (std::ostream& out, const Game& game)
	{
		for(size_t i = 0; i < SIZE; ++i)
		{
			for(size_t j = 0; j < SIZE; ++j)
			{
				switch(game.board[i][j])
				{
					case Piece::white: out << 'O'; break;
					case Piece::black: out << 'B'; break;
					default: out << '.'; break;
				}
			}
			out << std::endl;
		}
		return out;
	}

	bool operator == (const Game& other) const;

	bool is_pos_move(uint i, uint j, Piece turn) const;
	std::vector<Pos> possible_moves(Piece turn) const;

	Game apply_move(Pos p, Piece turn) const;
	int get_winner() const;

	Piece board[SIZE][SIZE];
};

struct GameHash
{
	int primes[64] = {2 ,3 ,5 ,7 ,11 ,13 ,17 ,19 ,23 ,29 
				 ,31 ,37 ,41 ,43 ,47 ,53 ,59 ,61 ,67
				 ,71 ,73 ,79 ,83 ,89 ,97 ,101 ,103 
				 ,107 ,109 ,113 ,127 ,131 ,137 ,139 
				 ,149 ,151 ,157 ,163 ,167 ,173 ,179 
				 ,181 ,191 ,193 ,197 ,199 ,211 ,223 
				 ,227 ,229 ,233 ,239 ,241 ,251 ,257 
				 ,263 ,269 ,271 ,277 ,281 ,283 ,293 
				 ,307 ,311};
	size_t operator () (const Game& game) const
	{
		size_t hash = 0;
		for(int i = 0; i < 8; ++i)
			for(int j = 0; j < 8; ++j)
				hash += game.board[i][j] * primes[i*8 + j];
		return hash;
	}
};

Piece flip(Piece p);