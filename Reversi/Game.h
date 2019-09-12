#pragma once
#include "Player.h"
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

typedef vector<vector<Color>> Board;
typedef pair<int, int> Direction;
typedef pair<int, vector<Direction>> Move;

class Game {
public:
	bool ChangePlayer(const Board&, Color);
	bool CheckAct(vector<Move>, int, int);
	int ExecAct(Board&, int, int, Color);
	int UndoAct(Board&, int, int, Color);
	vector<Move> GetAvaliableAct(const Board&, Color);
	int CheckEnd(const Board&);

	Player* players[2];
	Board board;
	Game();
	void Print();
	void Start();
private:
	static bool canMove(int, int);
	int round;
	int currentPlayer;
	int winner;
	void init();
	void printWinner();
	void execAct(int, int);
	int checkEnd();
};

