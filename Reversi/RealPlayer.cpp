#include "RealPlayer.h"

pair<int, int> RealPlayer::Act(const Board& board) {
	char row, col;
	while (1) {
		cout << "please input the coordination (row, col): ";
		cin >> row >> col;
		if (row < '1' || row > '8' || col < 'A' || col > 'H') {
			cout << "input out of range" << endl;
		}
		else {
			break;
		}
	}
	return pair<int, int>(row - '1', col - 'A');
}