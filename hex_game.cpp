//Code for a hex 10x10 game
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <queue>
#include <utility>
#include <vector>
#include <string>
using namespace std;

class HexAI
{
public:
	virtual std::pair<int, int> getNextMove(HexBoard &board, Player player);
};

class MonteCarlo
{
public:
	// get MonteCarlo score
	static double getScore(HexBoard &board, Player player);

private:
	// generate a increased permutation
	// e.g n = 5, return a permutation, say, 0,1,2,3,4 
	static std::vector<int> generatePermutation(int n);

	// adapt the order of input permutation to make it in
	// random order.
	static void adaptPermutation(std::vector<int> &perm);

	const static int TrialNum;
};

const int MonteCarlo::TrialNum = 1000;

double MonteCarlo::getScore(HexBoard &board, Player player)
{
	auto unoccupied = board.getUnoccupied();
	auto perm = generatePermutation(unoccupied.size());

	int count = 0; // count the player's winning times
	for (int n=0; n<TrialNum; n++)
	{
		int turn = (player == Player::BLUE ? 0 : 1);
		adaptPermutation(perm);
		for (int i=0; i<perm.size(); i++)
		{
			turn = !turn;
			int x = unoccupied[perm[i]].first;
			int y = unoccupied[perm[i]].second;
			if (turn) // RED's turn
			{
				board.placePiece(x, y, Player::RED);
			}
			else      // BLUE's turn
			{
				board.placePiece(x, y, Player::BLUE);
			}
		}

		if (board.win() == player)
			count++;

		// retract random moves
		for (auto itr = unoccupied.begin(); itr != unoccupied.end(); ++itr)
		{
			board.undo(itr->first, itr->second);
		}
	}

	return static_cast<double>(count) / TrialNum;
}

vector<int> MonteCarlo::generatePermutation(int n)
{
	vector<int> perm(n);

	for (int i=0; i<n; i++)
		perm[i] = i;

	return perm;
}

void MonteCarlo::adaptPermutation(vector<int> &perm)
{
	for (int i=perm.size(); i>1; i--)
	{
		int choose = rand() % i;
		int temp = perm[i-1];
		perm[i-1] = perm[choose];
		perm[choose] = temp;
	}
}

void HexGame::playGame()
{
	while (true)
	{
		initGame();
		choosePlayer();

		cout << "Game starting ..." << endl << endl;
		int size = board.getBoardSize();
		bool terminal = false;
		/*if (robot == Player::BLUE)
		{
			cout << "Robot's turn: " << endl;
			board.placePiece(size/2, size/2, robot);
			terminal = board.win(size/2, size/2);
			cout << board;
		}*/

		int turn = (robot == Player::BLUE ? 0 : 1);
		while (terminal == false)
		{
			turn = !turn;
			if (turn)
			{
				terminal = robotTurn();
			}
			else
			{
				terminal = humanTurn();
			}
		}

		if (turn == 1)
			cout << "Robot wins!!!" << endl;
		else
			cout << "You win! Congratulations." << endl;

		cout << "Do you want a new game (y/n)? ";
		char flag;
		cin >> flag;
		if (flag != 'y' && flag != 'Y')
			break;
		cin.clear();
	}

	cout << "Thank you for playing HEX GAME! Bye-bye." << endl;
}

void HexGame::initGame()
{
	string guard(30, '*');
	cout << '\n' << guard << endl;
	cout << "*  Welcome to the HEX world  *" << endl;
	cout << guard << endl << endl;

	int boardSize;
	while (true)
	{
		cout << "Please choose the HexBoard size: ";
		cin >> boardSize;
		if (boardSize > 0)
			break;
		
		cout << "The HexBoard size should be positive." << endl;
		cin.clear();
	}

	board = HexBoard(boardSize);
	cout << "The HexBoard has been set up." << endl;
	cout << board;
}

void HexGame::choosePlayer()
{
	char side = '.';
	while (true)
	{
		cout << "Please choose your side (B/R): ";
		cin >> side;
		if (side == 'b' || side == 'B')
		{
			human = Player::BLUE;
			robot = Player::RED;
			break;
		}
		else if (side == 'r' || side == 'R')
		{
			human = Player::RED;
			robot = Player::BLUE;
			break;
		}

		cout << "Illegal side!" << endl;
		cin.clear();
	}
}

bool HexGame::robotTurn()
{
	cout << "Robot's turn: ";
	auto move = hexAI.getNextMove(board, robot);
	board.placePiece(move.first, move.second, robot);
	cout << move.first << " " << move.second << endl;
	cout << board;

	return board.win(move.first, move.second);
}

bool HexGame::humanTurn()
{
	int x, y;
	cout << "Your turn: " << endl;
	while (true)
	{
		cout << "Input your move: x y = ";
		cin >> x >> y;
		if (board.placePiece(x, y, human))
			break;

		cout << "Illegal move!" << endl;
	}

	cout << board;

	return board.win(x, y);
}
enum class Player { BLUE, RED };

class HexBoard
{
public:
	HexBoard() {}
	HexBoard(int size): 
		size(size), board(size, std::vector<char>(size, '.')) 
	{
		connectedLine = "\\";
		for (int i=1; i<size; i++)
		{
			connectedLine += " / \\";
		}
	}

	// place a piece on the board, and if no conflicts, return ture
	bool placePiece(int x, int y, Player player);

	// retact a false move
	bool undo(int x, int y);

	// check whehter one side win or not
	bool win(int x, int y) const;

	// print out the board
	void printBoard(std::ostream &out) const;

private:
	inline bool inBoard(int x, int y) const;
	inline void checkBorders(int x, int y, 
			std::vector<bool> &flags, char side) const;

	const static int dirs[6][2]; // six directions
	const static char Blue = 'B';
	const static char Red = 'R';
	const static char Blank = '.';
	int size;
	std::string connectedLine;
	std::vector<std::vector<char> > board;
};

bool HexBoard::inBoard(int x, int y) const
{
	return x>=0 && x<size && y>=0 && y<size;
}
void HexBoard::checkBorders(int x, int y, 
		std::vector<bool> &flags, char side) const
{
	if (side == Blue)
	{
		if (y == 0)
			flags[0] = true;
		if (y == size - 1)
			flags[1] = true;
	}
	else if (side == Red)
	{
		if (x == 0)
			flags[0] = true;
		if (x == size - 1)
			flags[1] = true;
	}
}
void HexGame::playGame()
{
	while (true)
	{
		initGame();
		choosePlayer();

		cout << "Game starting ..." << endl << endl;
		int size = board.getBoardSize();
		bool terminal = false;
		/*if (robot == Player::BLUE)
		{
			cout << "Robot's turn: " << endl;
			board.placePiece(size/2, size/2, robot);
			terminal = board.win(size/2, size/2);
			cout << board;
		}*/

		int turn = (robot == Player::BLUE ? 0 : 1);
		while (terminal == false)
		{
			turn = !turn;
			if (turn)
			{
				terminal = robotTurn();
			}
			else
			{
				terminal = humanTurn();
			}
		}

		if (turn == 1)
			cout << "Robot wins!!!" << endl;
		else
			cout << "You win! Congratulations." << endl;

		cout << "Do you want a new game (y/n)? ";
		char flag;
		cin >> flag;
		if (flag != 'y' && flag != 'Y')
			break;
		cin.clear();
	}

	cout << "Thank you for playing HEX GAME! Bye-bye." << endl;
}

void HexGame::initGame()
{
	string guard(30, '*');
	cout << '\n' << guard << endl;
	cout << "*  Welcome to the HEX world  *" << endl;
	cout << guard << endl << endl;

	int boardSize;
	while (true)
	{
		cout << "Please choose the HexBoard size: ";
		cin >> boardSize;
		if (boardSize > 0)
			break;
		
		cout << "The HexBoard size should be positive." << endl;
		cin.clear();
	}

	board = HexBoard(boardSize);
	cout << "The HexBoard has been set up." << endl;
	cout << board;
}

void HexGame::choosePlayer()
{
	char side = '.';
	while (true)
	{
		cout << "Please choose your side (B/R): ";
		cin >> side;
		if (side == 'b' || side == 'B')
		{
			human = Player::BLUE;
			robot = Player::RED;
			break;
		}
		else if (side == 'r' || side == 'R')
		{
			human = Player::RED;
			robot = Player::BLUE;
			break;
		}

		cout << "Illegal side!" << endl;
		cin.clear();
	}
}

bool HexGame::robotTurn()
{
	cout << "Robot's turn: ";
	auto move = hexAI.getNextMove(board, robot);
	board.placePiece(move.first, move.second, robot);
	cout << move.first << " " << move.second << endl;
	cout << board;

	return board.win(move.first, move.second);
}

bool HexGame::humanTurn()
{
	int x, y;
	cout << "Your turn: " << endl;
	while (true)
	{
		cout << "Input your move: x y = ";
		cin >> x >> y;
		if (board.placePiece(x, y, human))
			break;

		cout << "Illegal move!" << endl;
	}

	cout << board;

	return board.win(x, y);
}
class HexGame
{
public:
	HexGame() {}
	HexGame(const HexAI &hexAI): hexAI(hexAI) {}

	// start playing HEX GAME
	void playGame();

private:
	// initialize the hex board size
	void initGame();

	// the player choose side
	void choosePlayer();

	// if robot or human wins, return true;
	bool robotTurn();
	bool humanTurn();

	Player robot;
	Player human;
	HexAI hexAI;
	HexBoard board;
};

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
int main()
{
	const int SIZE = 11;
	HexBoard board(SIZE);
	cout << "Initialize Hex board" << endl;
	board.printBoard(cout);

	int turn = 0;
	int steps = 0;
	int x, y;
	srand(time(0));
	while (true)
	{
		steps++;
		turn = !turn;
		if (turn == 1)
		{
			do
			{
				x = rand() % SIZE;
				y = rand() % SIZE;
			}while (!board.placePiece(x, y, Player::BLUE));

			cout << "Player Blue: " << x << ", " << y << endl;
		}
		else
		{
			do
			{
				x = rand() % SIZE;
				y = rand() % SIZE;
			}while (!board.placePiece(x, y, Player::RED));

			cout << "Player Red: " << x << ", " << y << endl;
		}

		if (board.win(x, y))
		{
			cout << (turn ? "Blue" : "Red") << " win" << endl;
			cout << "Total steps = " << steps << endl;
			board.printBoard(cout);
			break;
		}
	}

	return 0;
}