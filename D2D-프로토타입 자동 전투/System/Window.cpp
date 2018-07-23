#include "stdafx.h"
#include "Window.h"
#include "Program.h"

Program* Window::program = NULL;

WPARAM Window::Run()
{
	MSG msg = { 0 };

	D2D::Get();

#ifdef IMGUI_USE
	ImGui::CreateContext();
	ImGui_ImplDX9_Init(g_hWnd, D2D::GetDevice());
	ImGui::StyleColorsDark();
#endif // IMGUI_USE

	// ���ӿ� ���� �̷��� ���ִµ�
	// ���߿� FPS ���� �Ҳ�
	FRAME->Init();
	program = new Program();

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
#ifdef IMGUI_USE
			ImGui_ImplDX9_NewFrame();
			// ���м��� ����ϰ� ���ַ��°�
			{
				Frame::Get()->UpdateTime(60.0f);
				program->Update();
				// ImGui �ѱ��� �ȵ� �� ��� ���ľ���
				ImGui::Text("Hello, ImGui");
				ImGui::ColorEdit3("Clear Color", color);
			}
			ImGui::EndFrame();
#else
			FRAME->Update(60.0f);
			program->Update();
#endif // IMGUI_USE

			// DWORD�� �÷����� ARGB
			D2D::Get()->BeginScene(D3DXCOLOR(color));
			{
#ifdef IMGUI_USE
				ImGui::Render();
				ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
#endif // IMGUI_USE

				program->Render();
			}
			D2D::Get()->EndScene();
		}
	}

#ifdef IMGUI_USE
	ImGui_ImplDX9_Shutdown();
#endif // IMGUI_USE

	FRAME->Release();
	SAFE_DELETE(program);

	// static�̶� �ٷ� �Լ� ����
	D2D::Delete();

	return msg.wParam;
}

Window::Window()
{
	WNDCLASSEX wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hIconSm = wndClass.hIcon;
	wndClass.hInstance = g_hInstance;
	wndClass.lpfnWndProc = (WNDPROC)WndProc;
	wndClass.lpszClassName = appName.c_str();
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.cbSize = sizeof(WNDCLASSEX);

	WORD wHr = RegisterClassEx(&wndClass);
	assert(wHr != 0);

	if (bFullScreen == true)
	{
		DEVMODE devMode = { 0 };
		devMode.dmSize = sizeof(DEVMODE);
		devMode.dmPelsWidth = (DWORD)WINSIZE_X;
		devMode.dmPelsHeight = (DWORD)WINSIZE_Y;
		devMode.dmBitsPerPel = 32;
		devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
	}

	g_hWnd = CreateWindowEx
	(
		WS_EX_APPWINDOW
		, appName.c_str()
		, appName.c_str()
		, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, NULL
		, (HMENU)NULL
		, g_hInstance
		, NULL
	);
	assert(g_hWnd != NULL);


	RECT rect = { 0, 0, (LONG)WINSIZE_X, (LONG)WINSIZE_Y };

	UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - (UINT)WINSIZE_X) / 2;
	UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - (UINT)WINSIZE_Y) / 2;

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	MoveWindow
	(
		g_hWnd
		, centerX, centerY - 30
		, rect.right - rect.left, rect.bottom - rect.top
		, TRUE
	);
	ShowWindow(g_hWnd, SW_SHOWNORMAL);
	SetForegroundWindow(g_hWnd);
	SetFocus(g_hWnd);

	ShowCursor(true);

	// rgba ��
	color[0] = 0.25f;
	color[1] = 0.35f;
	color[2] = 0.45f;
	color[3] = 1.00f;

	//color[0] = 1.00f;
	//color[1] = 1.00f;
	//color[2] = 1.00f;
	//color[3] = 1.00f;
}

Window::~Window()
{
	INPUT->Delete();
	FRAME->Delete();
	SOUND->Delete();
	SCENE->Delete();
	TEXTURE->Delete();

	DestroyWindow(g_hWnd);
	UnregisterClass(appName.c_str(), g_hInstance);
}

LRESULT CALLBACK Window::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
#ifdef IMGUI_USE
	ImGui_ImplWin32_WndProcHandler(handle, message, wParam, lParam);
#endif // IMGUI_USE

	if (message == WM_SIZE)
	{

		if (program != NULL)
		{
			float width = (float)LOWORD(lParam);
			float height = (float)HIWORD(lParam);
		}
	}

	if (message == WM_CLOSE || message == WM_DESTROY)
	{
		PostQuitMessage(0);

		return 0;
	}

	return DefWindowProc(handle, message, wParam, lParam);
}

