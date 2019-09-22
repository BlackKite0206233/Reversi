#pragma once

#include <utility>
#include <vector>
#include <map>
#include <windows.h>

using namespace std;

enum class Color {
	Black = 1,
	White = -1,
	Empty = 0
};
enum class State {
	Ready,
	ChoosingColor,
	Playing,
	Ending
};
enum class PlayerType {
	Real,
	AI,
};

typedef vector<vector<Color>> Board;
typedef pair<int, int> Direction;
typedef map<int, vector<Direction>> Move;

class Player;

class Reversi
{
public:
	static bool ChangePlayer(const Board&, Color);
	static bool CheckAct(Move&, int, int);
	static int ExecAct(Board&, int, int, Color, vector<Direction>, bool);
	static Move GetAvaliableAct(const Board&, Color);
	static int CheckEnd(const Board&);

	Player* players[2];
	Board board;
	int currentPlayer;
	State state;

	Reversi(void);
	~Reversi(void);

	void Start();
	void ChooseLeft();
	void ChooseRight();
	void ExecSelect();
	void MoveNext();
	void MovePrev();
	void Undo();
	void Redo();
	void Restart();
	void ExecMove();

private:
	static bool canMove(int, int);
	static vector<Direction> getDirection(const Board&, int, int, Color);

	int mode;
	int choosedColor;
	int wantExit;
	int cursor;
	int winner;
	bool isMove;
	bool isInit;

	Move avaliableMoves;
	vector<int> moveList;
	vector<pair<int, Board>> history;
	vector<pair<int, Board>>::iterator it;

	void Init();
	void NextState();
	void Print();

	void execAct(Move&, int, int);
	int checkEnd();
};

class Player {
public:
	PlayerType playerType;
	int num;
	Color color;
	void Init();
	virtual pair<int, int> Act(const Board&) = 0;
};