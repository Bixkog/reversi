cmake_minimum_required(VERSION 2.8)
project( reversi )
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++17 -g -ggdb -Wall -march=native -Wextra -Werror")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++17 -O3 -flto -march=native -Wall -Wextra -Werror")

add_executable( reversi reversi.cpp 
					game.h
					game.cpp
					random_agent.h
					random_agent.cpp
					match.h
					match.cpp
					ab_agent.h
					ab_agent.cpp
					mcts_agent.h
					mcts_agent.cpp
					g_node.h
					g_node.cpp)


