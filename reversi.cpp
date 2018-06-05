#include "game.h"
#include "match.h"
#include "random_agent.h"
#include "ab_agent.h"
#include "mcts_agent.h"


int main()
{

	while(true)
	{
		Match m;
		if(m.duel<AB_Agent>() == 0)
			break;
	}
}