#include <cstdlib>
#include <ctime>
#include <string>
#include "Game.h"

int x[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
int y[] = { -1, -1, 0, 1, 1, 1, 0, -1 };

bool Game::ChangePlayer(const Board& board, Color color) {
	return this->GetAvaliableAct(board, color).size() != 0;
}

bool Game::CheckAct(Move avaliableMoves, int row, int col) {
	return avaliableMoves.find(row * 8 + col) != avaliableMoves.end();
}

int Game::ExecAct(Board& board, Move avaliableMoves, int row, int col, Color color) {
	int revCnt = 0;
	board[row][col] = color;
	vector<Direction> directions = avaliableMoves[row * 8 + col];
	for (auto& direction : directions) {
		int startY = row + y[direction.first];
		int startX = col + x[direction.first];
		do {
			board[startY][startX] = color;
			startY += y[direction.first];
			startX += x[direction.first];
			revCnt++;
		} while(startY != direction.second / 8 && startX != direction.second % 8);
	}
	return revCnt;
}

int Game::UndoAct(Board& board, Move avaliableMoves, int row, int col, Color color) {
	int revCnt = 0;
	board[row][col] = Color::Empty;
	vector<Direction> directions = avaliableMoves[row * 8 + col];
	for (auto& direction : directions) {
		int startY = row + y[direction.first];
		int startX = col + x[direction.first];
		do {
			board[startY][startX] = -static_cast<int>(color);
			startY += y[direction.first];
			startX += x[direction.first];
			revCnt++;
		} while(startY != direction.second / 8 && startX != direction.second % 8);
	}
	return revCnt;
}

Move Game::GetAvaliableAct(const Board& board, Color color) {
	Move avaliableMoves;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			vector<Direction> directions = this->getDirection(board, i, j, color);
			if (directions.size()) {
				avaliableMoves[i * 8 + j] = directions;
			}
		}
	}
	return avaliableMoves;
}	

vector<Direction> Game::getDirection(const Board& board, int row, int col, Color color) {
	vector<Direction> directions;
	if (board[row][col] == Color::Empty) {
		return directions;
	}
	for (int w = 0; w < 8; w++) {
		int startY = row;
		int startX = col;
		if (this->canMove(startX + x[w], startY + y[w]) &&
			static_cast<int>(board[startY + y[w]][startX + x[w]]) == -static_cast<int>(color)) {
			startY += y[w];
			startX += x[w];
			while (this->canMove(startX + x[w], startY + y[w])) {
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

int Game::CheckEnd(const Board& board) {
	if (!this->GetAvaliableAct(board, Color::Black).size() && !this->GetAvaliableAct(board, Color::White).size()) {
		this->winner = this->players[0]->num < this->players[1]->num;
		return 1;
	}
	return 0;
}

Game::Game() {
	srand(time(NULL));
}

void Game::Start() {
	this->init();

	int first = rand() % 2;
	this->currentPlayer = first;
	this->players[this->currentPlayer]->color = Color::Black;
	this->players[1 - this->currentPlayer]->color = Color::White;

	while (1) {
		this->Print();

		pair<int, int> act;
		Move avaliableMoves = this->GetAvaliableAct(this->board, this->players[this->currentPlayer]->color);
		while (1) {
			act = this->players[this->currentPlayer]->Move();

			if (this->CheckAct(avaliableMoves, act.first, act.second)) {
				break;
			}
		}
		this->execAct(avaliableMoves, act.first, act.second);

		if (this->checkEnd()) {
			this->printWinner();
			break;
		}

		if (this->ChangePlayer(this->board, this->players[1 - this->currentPlayer]->color)) {
			this->currentPlayer = 1 - this->currentPlayer;
		}

		this->round++;
	}
}

void Game::Print() {
	system("cls");

	string row = "   +---+---+---+---+---+---+---+---+";
	cout << "\n     A   B   C   D   E   F   G   H" << endl;
	cout << row << endl;
	for (int i = 0; i < 8; i++) {
		cout << " " << i + 1 << " | ";
		for (int j = 0; j < 8; j++) {
			cout << (this->board[i][j] == Color::Black ? "@" : (this->board[i][j] == Color::White ? "O" : " ")) << " | ";
		}
		if (i == 3) {
			cout << "   player0 (" << (this->players[0]->color == Color::Black ? "@" : "O") << ": " << this->players[0]->num << ")";
		}
		if (i == 4) {
			cout << "   player1 (" << (this->players[1]->color == Color::Black ? "@" : "O") << ": " << this->players[1]->num << ")";
		}
		cout << endl << row << endl;
	}
	cout << endl;
	cout << "Turn: " << static_cast<char>(this->players[this->currentPlayer]->color) << endl;
}

bool Game::canMove(int x, int y) {
	return x >= 0 && x < 8 && y >= 0 && y < 8;
}

void Game::init() {
	this->players[0]->Init();
	this->players[1]->Init();

	this->board = vector<vector<Color>>(8, vector<Color>(8, Color::Empty));
	this->board[3][3] = this->board[4][4] = Color::White;
	this->board[3][4] = this->board[4][3] = Color::Black;

	this->round = 0;
}

void Game::printWinner() {
	cout << "winner is player" << this->winner << " !!!" << endl;
}

void Game::execAct(Move avaliableMoves, int row, int col) {
	int revCnt = this->ExecAct(this->board, avaliableMoves, row, col, this->players[this->currentPlayer]->color);
	this->players[this->currentPlayer]->num += revCnt + 1;
	this->players[1 - this->currentPlayer]->num -= revCnt;
}

int Game::checkEnd() {
	return this->CheckEnd(this->board);
}