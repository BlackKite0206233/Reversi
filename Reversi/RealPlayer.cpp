#include "RealPlayer.h"

#include <cctype>

pair<int, int> RealPlayer::Act(const Board& board) {
	char row, col;
	while (1) {
		cout << "please input the coordination (row, col): ";
		cin >> row;
		if (row == '-') {
			return pair<int, int>(-1, 0);
		}
		if (row == '+') {
			return pair<int, int>(-2, 0);
		}
		if (row == 'r') {
			return pair<int, int>(-3, 0);
		}
		cin >> col;
		col = toupper(col);
		if (row < '1' || row > '8' || col < 'A' || col > 'H') {
			cout << "input out of range" << endl;
		}
		else {
			break;
		}
	}
	return pair<int, int>(row - '1', col - 'A');
}