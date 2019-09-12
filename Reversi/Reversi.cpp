// Reversi.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include <string>
#include "RealPlayer.h"
#include "AIPlayer.h"

using namespace std;

int main()
{
	Game game = Game();

	game.players[0] = new RealPlayer();
	game.players[1] = new AIPlayer(200);

	while (1) {
		game.Start();
		string isExit;
		cout << "是否離開 [y/n]\n >_ ";
		cin >> isExit;
		if (isExit == "y" || isExit == "Y") {
			break;
		}
	}
}
