#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <iostream>

#include "Reversi.h"

using namespace std;

Reversi game;

LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


void KeyDownEvent( WPARAM wParam )
{
}

void KeyUpEvent( WPARAM wParam )
{

	switch (game.state) {
	case State::Ready:
	case State::ChoosingColor:
	case State::Ending:
		if (wParam == VK_LEFT) {
			game.ChooseLeft();
		}
		else if (wParam == VK_RIGHT) {
			game.ChooseRight();
		}
		else if (wParam == VK_RETURN) {
			game.ExecSelect();
		}
		break;
	case State::Playing:
		if (game.players[game.currentPlayer]->playerType == PlayerType::Real) {
			if (wParam == VK_LEFT) {
				game.MoveNext();
			}
			else if (wParam == VK_RIGHT) {
				game.MovePrev();
			}
			else if (wParam == VK_SUBTRACT) {
				game.Undo();
			}
			else if (wParam == VK_ADD) {
				game.Redo();
			}
			else if (wParam == 'R') {
				game.Restart();
			}
			else if (wParam == VK_RETURN) {
				game.ExecMove();
			}
		}
		break;
	default:
		break;
	}
}

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow )
{
	//==== 註冊視窗類別 ====//
	const wchar_t CLASS_NAME[]  = L"Sample Window Class";

	WNDCLASS wc = {};

	wc.lpfnWndProc   = WindowProc;
	wc.hInstance     = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass( &wc );

	//==== 創建視窗 ====//
	HWND hwnd = CreateWindowEx(
		0,                      // Optional window styles.
		CLASS_NAME,             // 視窗類別
		L"Reversi",				// 視窗標題
		WS_OVERLAPPEDWINDOW,    // 視窗風格
		0, 0, 5, 5,				// 視窗大小及位置
		NULL,					// Parent 視窗    
		NULL,					// 選單
		hInstance,				// Instance handle
		NULL					// Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	//==== 創建 console 視窗 ====//
	AllocConsole();
	freopen( "CONOUT$", "wb", stdout );

	ShowWindow( hwnd, nCmdShow );

	game.Start();

	//==== 執行訊息迴圈 ====//
	MSG msg = { };
	while ( GetMessage(&msg, NULL, 0, 0) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	return 0;
}

LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint( hwnd, &ps );
			FillRect( hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
			EndPaint( hwnd, &ps );
		}
		return 0;

	case WM_KEYDOWN:
		KeyDownEvent( wParam );
		break;

	case WM_KEYUP:
		KeyUpEvent( wParam );
		break;
	}

	return DefWindowProc( hwnd, uMsg, wParam, lParam );
}
