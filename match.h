#pragma once
#include "game.h"
#include <optional>
#include "mcts_agent.h"
#include "ab_agent.h"

struct Match
{
	Match(const Game& game = Game())
	: game(game)
	{}
	template<typename P1, typename P2>
	int play(P1& player1, P2& player2);

	template<typename P>
	int duel();

	Game game;
};

template<typename P1, typename P2>
int Match::play(P1& player1, P2& player2)
{
	int last_player = -1;
	while(true)
	{
		auto p1_move = player1.make_move(game);
		if(p1_move)
		{
			game = game.apply_move(p1_move.value(), player1.turn);
			last_player = 1;
		}
		else
		{
			if(last_player == 1)
				break;
			last_player = 2;
		}
		auto p2_move = player2.make_move(game);
		if(p2_move)
		{
			game = game.apply_move(p2_move.value(), player2.turn);
			last_player = 2;
		}
		else
		{
			if(last_player == 2)
				break;
			last_player = 1;
		}
	}
	return game.get_winner();
}



template<typename P>
int Match::duel()
{
	std::cout << "RDY" << std::endl;

	std::string command;
	std::unique_ptr<P> player;
	Piece opp_turn;
	float time, time2;
	int move_x, move_y;
	// first move
	std::cin >> command;
	if(command == "HEDID")
	{
		player = std::make_unique<P>(Piece::black);
		opp_turn = Piece::white;
		std::cin >> time >> time2;
		std::cin >> move_x >> move_y;
		game = game.apply_move({(uint)move_y, (uint)move_x}, opp_turn);
	}
	else
	{
		player = std::make_unique<P>(Piece::white);
		opp_turn = Piece::black;
	}

	while(true)
	{
		// my move
		auto p_move = player->make_move(game);
		if(p_move)
		{
			auto move = p_move.value();
			game = game.apply_move(move, player->turn);
			std::cout << "IDO " << move.y << " " << move.x << std::endl;
		}
		else
			std::cout << "IDO -1 -1" << std::endl;

		// opponent move
		std::cin >> command;
		if(command == "HEDID")
		{
			std::cin >> time >> time2;
			std::cin >> move_x >> move_y;
			if(move_x == -1 and move_y == -1)
				continue;
			game = game.apply_move({(uint)move_y, (uint)move_x}, opp_turn);
		}
		else if(command == "ONEMORE")
		{
			return 1;
		}
		else if(command == "BYE")
		{
			return 0;
		}
	}
}
