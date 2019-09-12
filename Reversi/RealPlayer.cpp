#include "RealPlayer.h"

pair<int, int> RealPlayer::Move() {
	char row, col;
	while (1) {
		cout << "請輸入座標 (row, col): ";
		cin >> row >> col;
		if (row < '1' || row > '8' || col < 'A' || col > 'H') {
			cout << "輸入超出範圍" << endl;
		}
		else {
			break;
		}
	}
	return pair<int, int>(row - '1', col - 'A');
}