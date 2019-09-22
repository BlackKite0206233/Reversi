#include "AIPlayer.h"

AIPlayer::AIPlayer(int depth) : depth(depth) {
	playerType = PlayerType::AI;
}

pair<int, int> AIPlayer::Act(const Board& board) {
	Board b = board;
	int coord = negamax(b, color);
	return pair<int, int>(coord / 8, coord % 8);
}

int AIPlayer::negamax(Board& board, Color color) {
	int coord;
	int alpha = -100;
	int beta = 100;
	Move avaliableMoves = Reversi::GetAvaliableAct(board, color);
	coord = avaliableMoves.begin()->first;
	for (auto& [key, value] : avaliableMoves) {
		Reversi::ExecAct(board, key / 8, key % 8, color, value, false);
		int val = -alphabeta(board, Color(-static_cast<int>(color)), depth, -beta, -alpha);
		Reversi::ExecAct(board, key / 8, key % 8, color, value, true);
		if (val >= beta) {
			return key;
		}
		if (val > alpha) {
			alpha = val;
			coord = key;
		}
	}
	return coord;
}

int AIPlayer::alphabeta(Board& board, Color color, int depth, int alpha, int beta) {
	if (depth == 0 || Reversi::CheckEnd(board)) {
		return score(board, color);
	}
	Move avaliableMoves = Reversi::GetAvaliableAct(board, color);
	if (!avaliableMoves.size()) {
		int val = -alphabeta(board, Color(-static_cast<int>(color)), depth - 1, -beta, -alpha);
		if (val >= beta) {
			return val;
		}
		if (val > alpha) {
			alpha = val;
		}
	}
	else {
		for (auto& [key, value] : avaliableMoves) {
			Reversi::ExecAct(board, key / 8, key % 8, color, value, false);
			int val = -alphabeta(board, Color(-static_cast<int>(color)), depth - 1, -beta, -alpha);
			Reversi::ExecAct(board, key / 8, key % 8, color, value, true);
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
	int num = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			num += static_cast<int>(board[i][j]);
		}
	}
	return num * static_cast<int>(color);
}