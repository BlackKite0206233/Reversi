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
	//==== ���U�������O ====//
	const wchar_t CLASS_NAME[]  = L"Sample Window Class";

	WNDCLASS wc = {};

	wc.lpfnWndProc   = WindowProc;
	wc.hInstance     = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass( &wc );

	//==== �Ыص��� ====//
	HWND hwnd = CreateWindowEx(
		0,                      // Optional window styles.
		CLASS_NAME,             // �������O
		L"Reversi",				// �������D
		WS_OVERLAPPEDWINDOW,    // ��������
		0, 0, 5, 5,				// �����j�p�Φ�m
		NULL,					// Parent ����    
		NULL,					// ���
		hInstance,				// Instance handle
		NULL					// Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	//==== �Ы� console ���� ====//
	AllocConsole();
	freopen( "CONOUT$", "wb", stdout );

	ShowWindow( hwnd, nCmdShow );

	game.Start();

	//==== ����T���j�� ====//
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
