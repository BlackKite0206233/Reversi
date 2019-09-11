#pragma once
#include "Player.h"
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

class Game {
public:
	bool ChangePlayer(vector<vector<Color>>&, Color);
	int ExecAct(vector<vector<Color>>&, int, int, Color, bool);
	int CheckEnd(vector<vector<Color>>&);

	Player* players[2];
	vector<vector<Color>> board;
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
	bool checkAction(int, int);
	int checkEnd();
};

