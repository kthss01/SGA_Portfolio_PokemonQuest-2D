// Direct2D.cpp: 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "./System/Window.h"

HWND g_hWnd;
HINSTANCE g_hInstance;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	g_hInstance = hInstance;

	Window* window = new Window();
	WPARAM wParam = window->Run();
	SAFE_DELETE(window);

    return (int)wParam;
}