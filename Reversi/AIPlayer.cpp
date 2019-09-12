#include "AIPlayer.h"

pair<int, int> AIPlayer::Move(const Board& board) {
	Board b = board;
	return this->negamax(b, this->color, 8);
}

pair<int, int> AIPlayer::negamax(Board& board, Color color, int depth) {
	int row, col;
	int alpha = -100;
	int beta = 100;
	Move avaliableMoves = Game::GetAvaliableAct(board, color);
	row = avaliableMoves.begin()->first / 8;
	col = avaliableMoves.begin()->first % 8;
	for (auto& [key, value] : avaliableMoves) {
		Game::ExecAct(board, key / 8, key % 8, value, color, false);
		int val = -this->alphabeta(board, Color(-static_cast<int>(color), depth, -beta, -alpha);
		Game::ExecAct(board, key / 8, key % 8, value, color, true);
		if (val >= beta) {
			return pair<int, int>(key / 8, key % 8);
		}
		if (val > alpha) {
			alpha = val;
			row = key / 8;
			col = key % 8;
		}
	}
	return pair<int, int>(row, col);
}

int AIPlayer::alphabeta(Board& board, Color color, int depth, int alpha, int beta) {
	if (depth == 0 || Game::CheckEnd(board)) {
		return this->score(board, color);
	}
	Move avaliableMoves = Game::GetAvaliableAct(board, color);
	if (!avaliableMoves.size()) {
		int val = -alphabeta(board, Color(-static_cast<int>(color)), depth - 1, -beta, -alpha);
		if (val >= beta) {
			return val;
		}
		if (val > alpha) {
			alpha = val;
		}
	} else {
		for (auto& [key, value] : avaliableMoves) {
			Game::ExecAct(board, key / 8, key % 8, value, color, false);
			int val = -this->alphabeta(board, Color(-static_cast<int>(color), depth - 1, -beta, -alpha);
			Game::ExecAct(board, key / 8, key % 8, value, color, true);
			if (val >= beta) {
				return val;
			}
			if (val > alpha) {
				alpha = val;
			}
		}
	}
	return alpha;
}

int AIPlayer::score(Board& board, Color color) {

}