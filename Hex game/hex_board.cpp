
#include <hex_board.h>
#include <queue>
#include <utility>
using namespace std;

bool HexBoard::placePiece(int x, int y , Player player)
{
	if (inBoard(x, y) == false)
		return false;

	if (board[x][y] != Blank)
		return false;

	if (player == Player::BLUE)
		board[x][y] = Blue;
	else if (player == Player::RED)
		board[x][y] = Red;

	return true;
}

bool HexBoard::undo(int x, int y)
{
	if (inBoard(x, y) == false)
		return false;

	board[x][y] = Blank;

	return true;
}
const int HexBoard::dirs[6][2] = 
{
	{-1, 0}, {-1, 1}, // top left, top right
	{0, -1}, {0, 1},  // left, right
	{1, -1}, {1, 0}   // buttom left, buttom right
};



// check from the last move via BFS

void HexBoard::printBoard(ostream &out) const
{
	if (size <= 0)
		return;

	// print the first line
	out << board[0][0];
	for (int j=1; j<size; j++)
		out << " - " << board[0][j];
	out << endl;

	string spaces = "";
	for (int i=1; i<size; i++)
	{
		spaces += ' ';
		out << spaces << connectedLine << endl;
		spaces += ' ';
		out << spaces << board[i][0];
		for (int j=1; j<size; j++)
			out << " - " << board[i][j];
		out << endl;
	}
}