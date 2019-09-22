#include "Reversi.h"
#include "AIPlayer.h"
#include "RealPlayer.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

Reversi::Reversi(void)
{
	srand(time(NULL));
}

Reversi::~Reversi(void)
{
}


char* asciiArtStr[] = {
	"\t\t     ***** ***                                                                  ",
	"\t\t  ******  * **                                                             *    ",
	"\t\t **   *  *  **            **                                              ***   ",
	"\t\t*    *  *   **            **                                               *    ",
	"\t\t    *  *    *              **    ***             ***  ****       ****           ",
	"\t\t   ** **   *       ***      **    ***     ***     **** **** *   * **** * ***    ",
	"\t\t   ** **  *       * ***     **     ***   * ***     **   ****   **  ****   ***   ",
	"\t\t   ** ****       *   ***    **      **  *   ***    **         ****         **   ",
	"\t\t   ** **  ***   **    ***   **      ** **    ***   **           ***        **   ",
	"\t\t   ** **    **  ********    **      ** ********    **             ***      **   ",
	"\t\t   *  **    **  *******     **      ** *******     **               ***    **   ",
	"\t\t      *     **  **          **      *  **          **          ****  **    **   ",
	"\t\t  ****      *** ****    *    *******   ****    *   ***        * **** *     **   ",
	"\t\t *  ****    **   *******      *****     *******     ***          ****      *** *",
	"\t\t*    **     *     *****                  *****                              *** ",
	"\t\t*                                                                               ",
	"\t\t **                                                                             "
};

void printAsciiArt() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int i = 0; i < 17; i++) {
		for (int j = 0; j < 83; j++) {
			if (j == 0) SetConsoleTextAttribute(hConsole, 4);
			else if (j == 18) SetConsoleTextAttribute(hConsole, 6);
			else if (j == 28) SetConsoleTextAttribute(hConsole, 2);
			else if (j == 41) SetConsoleTextAttribute(hConsole, 3);
			else if (j == 51) SetConsoleTextAttribute(hConsole, 9);
			else if (j == 64) SetConsoleTextAttribute(hConsole, 1);
			else if (j == 75) SetConsoleTextAttribute(hConsole, 5);
			putchar(asciiArtStr[i][j]);
		}
		putchar('\n');
	}
	SetConsoleTextAttribute(hConsole, 7);
}

void ClearScreen(int row = 0, int col = 0)
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { col, row };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X * csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR) ' ',
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}

int x[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
int y[] = { -1, -1, 0, 1, 1, 1, 0, -1 };

bool Reversi::ChangePlayer(const Board& board, Color color) {
	return Reversi::GetAvaliableAct(board, color).size() != 0;
}

bool Reversi::CheckAct(Move& avaliableMoves, int row, int col) {
	return avaliableMoves.find(row * 8 + col) != avaliableMoves.end();
}

int Reversi::ExecAct(Board& board, int row, int col, Color color, vector<Direction> directions, bool reverse) {
	int revCnt = 0;
	if (reverse) {
		board[row][col] = Color::Empty;
	}
	else {
		board[row][col] = color;
	}
	for (auto& direction : directions) {
		int startY = row + y[direction.first];
		int startX = col + x[direction.first];
		do {
			if (reverse) {
				board[startY][startX] = Color(-static_cast<int>(color));
			}
			else {
				board[startY][startX] = color;
			}
			startY += y[direction.first];
			startX += x[direction.first];
			revCnt++;
		} while (startY != direction.second / 8 || startX != direction.second % 8);
	}
	return revCnt;
}

Move Reversi::GetAvaliableAct(const Board& board, Color color) {
	Move avaliableMoves;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			vector<Direction> directions = Reversi::getDirection(board, i, j, color);
			if (directions.size()) {
				avaliableMoves[i * 8 + j] = directions;
			}
		}
	}
	return avaliableMoves;
}

int Reversi::CheckEnd(const Board& board) {
	if (!Reversi::GetAvaliableAct(board, Color::Black).size() && !Reversi::GetAvaliableAct(board, Color::White).size()) {
		return 1;
	}
	return 0;
}

vector<Direction> Reversi::getDirection(const Board& board, int row, int col, Color color) {
	vector<Direction> directions;
	if (board[row][col] != Color::Empty) {
		return directions;
	}
	for (int w = 0; w < 8; w++) {
		int startY = row;
		int startX = col;
		if (Reversi::canMove(startX + x[w], startY + y[w]) &&
			static_cast<int>(board[startY + y[w]][startX + x[w]]) == -static_cast<int>(color)) {
			startY += y[w];
			startX += x[w];
			while (Reversi::canMove(startX + x[w], startY + y[w])) {
				startY += y[w];
				startX += x[w];
				if (board[startY][startX] == color) {
					directions.push_back(pair<int, int>(w, startY * 8 + startX));
					break;
				}
			}
		}
	}
	return directions;
}

bool Reversi::canMove(int x, int y) {
	return x >= 0 && x < 8 && y >= 0 && y < 8;
}



void Reversi::Start() {
	ClearScreen();
	state = State::Ready;
	mode = 0;
	choosedColor = 0;
	wantExit = 0;
	cout << endl;
	cout << endl;
	cout << endl;
	printAsciiArt();
	cout << endl;
	cout << endl;
	cout << endl;
	cout << "		                                   遊戲模式                                       " << endl;
	cout << endl;
	cout << endl;
	Print();
}

void Reversi::ChooseLeft() {
	switch (state)
	{
	case State::Ready:
		mode = 0;
		break;
	case State::ChoosingColor:
		choosedColor = 0;
		break;
	case State::Ending:
		wantExit = 0;
		break;
	default:
		break;
	}
	Print();
}

void Reversi::ChooseRight() {
	switch (state)
	{
	case State::Ready:
		mode = 1;
		break;
	case State::ChoosingColor:
		choosedColor = 1;
		break;
	case State::Ending:
		wantExit = 1;
		break;
	default:
		break;
	}
	Print();
}

void Reversi::ExecSelect() {
	if (state == State::Ending) {
		if (wantExit) {
			PostQuitMessage(0);
		}
		else {
			Start();
		}
	}
	else {
		NextState();
	}
}

void Reversi::MoveNext() {
	cursor += 1;
	cursor %= avaliableMoves.size();
	Print();
}

void Reversi::MovePrev() {
	cursor -= 1;
	if (cursor < 0) cursor = avaliableMoves.size() - 1;
	Print();
}

void Reversi::Undo() {
	vector<pair<int, Board>>::iterator it_ = it;
	while (it_ > history.begin()) {
		it_--;
		if (it_->first == currentPlayer) {
			board = it_->second;
			it = it_;
			break;
		}
	}
	cursor = 0;
	NextState();
}

void Reversi::Redo() {
	vector<pair<int, Board>>::iterator it_ = it;
	it_++;
	while (it_ < history.end()) {
		if (it_->first == currentPlayer) {
			board = it_->second;
			it = it_;
			break;
		}
		it_++;
	}
	cursor = 0;
	NextState();
}

void Reversi::Restart() {
	isInit = false;
	NextState();
}

void Reversi::ExecMove() {
	execAct(avaliableMoves, moveList[cursor] / 8, moveList[cursor] % 8);
	isMove = true;
	if (checkEnd()) {
		winner = players[0]->num < players[1]->num;
		state = State::Ending;
	}

	if (ChangePlayer(board, players[1 - currentPlayer]->color)) {
		currentPlayer = 1 - currentPlayer;
	}
	cursor = 0;
	NextState();
}

void Reversi::Init() {
	players[0]->Init();
	players[1]->Init();

	if (!choosedColor) {
		players[0]->color = Color::Black;
		players[1]->color = Color::White;
		currentPlayer = 0;
	}
	else {
		players[0]->color = Color::White;
		players[1]->color = Color::Black;
		currentPlayer = 1;
	}

	board = vector<vector<Color>>(8, vector<Color>(8, Color::Empty));
	board[3][3] = board[4][4] = Color::White;
	board[3][4] = board[4][3] = Color::Black;

	history.clear();
	it = history.end();
	isMove = true;
	isInit = true;
}

void Reversi::NextState() {
	if (state == State::Playing) {
		if (!isInit) {
			Init();
		}
		if (isMove) {
			while (history.size() > 0 && it != history.end() - 1) {
				history.pop_back();
			}
			history.push_back(pair<int, Board>(currentPlayer, board));
			it = history.end() - 1;
		}
		isMove = false;
		avaliableMoves = GetAvaliableAct(board, players[currentPlayer]->color);
		moveList = vector<int>();
		for (auto& i : avaliableMoves) {
			moveList.push_back(i.first);
		}
		Print();
		if (!mode && players[currentPlayer]->playerType == PlayerType::AI) {
			pair<int, int> act = players[currentPlayer]->Act(board);
			execAct(avaliableMoves, act.first, act.second);
			isMove = true;
			if (checkEnd()) {
				winner = players[0]->num < players[1]->num;
				state = State::Ending;
			}

			if (ChangePlayer(board, players[1 - currentPlayer]->color)) {
				currentPlayer = 1 - currentPlayer;
			}
			NextState();
		}
	}
	else {
		switch (state)
		{
		case State::Ready:
			if (!mode) { // single
				state = State::ChoosingColor;
				players[0] = new RealPlayer();
				players[1] = new AIPlayer(200);
				ClearScreen(23);
				cout << "		                                   先下/後下                                       " << endl;
				cout << endl;
				cout << endl;
				Print();
			}
			else { // double
				state = State::Playing;
				choosedColor = rand() & 1;
				players[0] = new RealPlayer();
				players[1] = new RealPlayer();
				isInit = false;
				NextState();
			}
			break;
		case State::ChoosingColor:
			state = State::Playing;
			isInit = false;
			NextState();
			break;
		case State::Ending:
			cout << "		                                   player" << winner << " win!" << endl;
			cout << endl;
			cout << endl;
			Print();
			break;
		default:
			break;
		}
	}
}

void Reversi::Print() {
	if (state == State::Ready) {
		if (!mode) {
			cout << "\r		                       > 單人                     雙人                            ";
		}
		else {
			cout << "\r		                         單人                   > 雙人                            ";
		}
	}
	else if (state == State::ChoosingColor) {
		if (!choosedColor) {
			cout << "\r		                   > 先下(黑)                     後下(白)                            ";
		}
		else {
			cout << "\r		                     先下(黑)                   > 後下(白)                            ";
		}
	}
	else if (state == State::Playing) {
		ClearScreen();

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		string row = "   +---+---+---+---+---+---+---+---+";
		cout << "\n     A   B   C   D   E   F   G   H" << endl;
		cout << row << endl;
		for (int i = 0; i < 8; i++) {
			cout << " " << i + 1 << " | ";
			for (int j = 0; j < 8; j++) {
				if (avaliableMoves.find(i * 8 + j) != avaliableMoves.end() && players[currentPlayer]->playerType == PlayerType::Real) {
					if (i * 8 + j == moveList[cursor]) {
						SetConsoleTextAttribute(hConsole, 4);
					}
					cout << "x";
					SetConsoleTextAttribute(hConsole, 7);
					cout << " | ";
				}
				else {
					cout << (board[i][j] == Color::Black ? "@" : (board[i][j] == Color::White ? "O" : " ")) << " | ";
				}
			}
			if (i == 3) {
				cout << "   player0 (" << (players[0]->color == Color::Black ? "@" : "O") << ": " << players[0]->num << ")";
			}
			if (i == 4) {
				cout << "   player1 (" << (players[1]->color == Color::Black ? "@" : "O") << ": " << players[1]->num << ")";
			}
			cout << endl << row << endl;
		}
		cout << endl;
		cout << "Turn: " << (players[currentPlayer]->color == Color::Black ? "@" : "O") << endl;
	}
	else if (state == State::Ending) {
		if (!wantExit) {
			cout << "\r		                   > 再來一局                          離開                            ";
		}
		else {
			cout << "\r		                     再來一局                        > 離開                            ";
		}
	}
}

void Reversi::execAct(Move& avaliableMoves, int row, int col) {
	int revCnt = ExecAct(board, row, col, players[currentPlayer]->color, avaliableMoves[row * 8 + col], false);
	players[currentPlayer]->num += revCnt + 1;
	players[1 - currentPlayer]->num -= revCnt;
}

int Reversi::checkEnd() {
	return CheckEnd(board);
}

void Player::Init() {
	num = 2;
}