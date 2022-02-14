#ifndef HEX_AI_H
#define HEX_AI_H

#include <utility>

#include <hex_Board.h>
#include <MonteCarlo.h>

class HexAI
{
public:
	virtual std::pair<int, int> getNextMove(HexBoard &board, Player player);
};

#endif