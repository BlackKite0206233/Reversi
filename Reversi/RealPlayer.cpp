#include "RealPlayer.h"

pair<int, int> RealPlayer::Move() {
	char row, col;
	while (1) {
		cout << "�п�J�y�� (row, col): ";
		cin >> row >> col;
		if (row < '1' || row > '8' || col < 'A' || col > 'H') {
			cout << "��J�W�X�d��" << endl;
		}
		else {
			break;
		}
	}
	return pair<int, int>(row - '1', col - 'A');
}