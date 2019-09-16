#include <cstdlib>
#include <ctime>
#include <string>
#include "Game.h"

int x[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
int y[] = { -1, -1, 0, 1, 1, 1, 0, -1 };

bool Game::ChangePlayer(const Board& board, Color color) {
	return Game::GetAvaliableAct(board, color).size() != 0;
}

bool Game::CheckAct(Move& avaliableMoves, int row, int col) {
	return avaliableMoves.find(row * 8 + col) != avaliableMoves.end();
}

int Game::ExecAct(Board& board, int row, int col, Color color, vector<Direction> directions, bool reverse) {
	int revCnt = 0;
	if (reverse) {
		board[row][col] = Color::Empty;
	} else {
		board[row][col] = color;
	}
	for (auto& direction : directions) {
		int startY = row + y[direction.first];
		int startX = col + x[direction.first];
		do {
			if (reverse) {
				board[startY][startX] = Color(-static_cast<int>(color));
			} else {
				board[startY][startX] = color;
			}
			startY += y[direction.first];
			startX += x[direction.first];
			revCnt++;
		} while(startY != direction.second / 8 || startX != direction.second % 8);
	}
	return revCnt;
}

Move Game::GetAvaliableAct(const Board& board, Color color) {
	Move avaliableMoves;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			vector<Direction> directions = Game::getDirection(board, i, j, color);
			if (directions.size()) {
				avaliableMoves[i * 8 + j] = directions;
			}
		}
	}
	return avaliableMoves;
}	

vector<Direction> Game::getDirection(const Board& board, int row, int col, Color color) {
	vector<Direction> directions;
	if (board[row][col] != Color::Empty) {
		return directions;
	}
	for (int w = 0; w < 8; w++) {
		int startY = row;
		int startX = col;
		if (Game::canMove(startX + x[w], startY + y[w]) &&
			static_cast<int>(board[startY + y[w]][startX + x[w]]) == -static_cast<int>(color)) {
			startY += y[w];
			startX += x[w];
			while (Game::canMove(startX + x[w], startY + y[w])) {
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
	if (!Game::GetAvaliableAct(board, Color::Black).size() && !Game::GetAvaliableAct(board, Color::White).size()) {
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
		if (this->isMove) {
			while (this->history.size() > 0 && this->it != this->history.end() - 1) {
				this->history.pop_back();
			}
			this->history.push_back(pair<int, Board>(this->currentPlayer, this->board));
			this->it = this->history.end() - 1;
		}
		this->isMove = false;
		Move avaliableMoves = this->GetAvaliableAct(this->board, this->players[this->currentPlayer]->color);
		this->Print(avaliableMoves);

		pair<int, int> act;
		while (1) {
			act = this->players[this->currentPlayer]->Act(board);
			if (act.first == -1 || act.first == -2 || act.first == -3) {
				break;
			}

			if (this->CheckAct(avaliableMoves, act.first, act.second)) {
				break;
			}
		}
		if (act.first == -1) {
			vector<pair<int, Board>>::iterator it = this->it;
			while (it > this->history.begin()) {
				it--;
				if (it->first == this->currentPlayer) {
					this->board = it->second;
					this->it = it;
					break;
				}
			}
		} else if (act.first == -2) {
			vector<pair<int, Board>>::iterator it = this->it;
			it++;
			while (it < this->history.end()) {
				if (it->first == this->currentPlayer) {
					this->board = it->second;
					this->it = it;
					break;
				}
				it++;
			}
		} else if (act.first == -3) {
			break;
		} else {
			this->execAct(avaliableMoves, act.first, act.second);
			this->isMove = true;

			if (this->checkEnd()) {
				this->winner = this->players[0]->num < this->players[1]->num;
				this->printWinner();
				break;
			}

			if (this->ChangePlayer(this->board, this->players[1 - this->currentPlayer]->color)) {
				this->currentPlayer = 1 - this->currentPlayer;
			}
		}
	}
}

void Game::Print(const Move& avaliableMoves) {
	system("cls");

	string row = "   +---+---+---+---+---+---+---+---+";
	cout << "\n     A   B   C   D   E   F   G   H" << endl;
	cout << row << endl;
	for (int i = 0; i < 8; i++) {
		cout << " " << i + 1 << " | ";
		for (int j = 0; j < 8; j++) {
			if (avaliableMoves.find(i * 8 + j) != avaliableMoves.end()) {
				cout << "x" << " | ";
			}
			else {
				cout << (this->board[i][j] == Color::Black ? "@" : (this->board[i][j] == Color::White ? "O" : " ")) << " | ";
			}
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
	cout << "Turn: " << (this->players[this->currentPlayer]->color == Color::Black ? "@" : "O") << endl;
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

	this->history.clear();
	this->it = this->history.end();
	this->isMove = true;
}

void Game::printWinner() {
	cout << "winner is player" << this->winner << " !!!" << endl;
}

void Game::execAct(Move& avaliableMoves, int row, int col) {
	int revCnt = this->ExecAct(this->board, row, col, this->players[this->currentPlayer]->color, avaliableMoves[row * 8 + col], false);
	this->players[this->currentPlayer]->num += revCnt + 1;
	this->players[1 - this->currentPlayer]->num -= revCnt;
}

int Game::checkEnd() {
	return this->CheckEnd(this->board);
}