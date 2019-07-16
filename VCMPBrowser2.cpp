#include "pch.h"
#include "VCMPBrowser2.h"

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	g_hInst = hInstance;

	WNDCLASSEXW wcex = { sizeof(WNDCLASSEX) };
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIconW(hInstance, MAKEINTRESOURCEW(IDI_VCMPBROWSER2));
	wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_3DFACE + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_VCMPBROWSER2);
	wcex.lpszClassName = L"VCMPBrowser2";
	wcex.hIconSm = LoadIconW(wcex.hInstance, MAKEINTRESOURCEW(IDI_VCMPBROWSER2));

	RegisterClassExW(&wcex);

	g_hMainWnd = CreateWindowW(L"VCMPBrowser2", L"VCMPBrowser2", WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, nullptr, nullptr, hInstance, nullptr);
	if (!g_hMainWnd)
		return FALSE;

	ShowWindow(g_hMainWnd, nCmdShow);
	UpdateWindow(g_hMainWnd);

	auto hAccelTable = LoadAcceleratorsW(hInstance, MAKEINTRESOURCEW(IDC_VCMPBROWSER2));

	MSG msg;
	while (GetMessageW(&msg, nullptr, 0, 0))
	{
		if (!TranslateAcceleratorW(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		InitDPIAPI();
		g_dpi = _GetDpiForWindow(hWnd);

		const int width = Scale(1000), height = Scale(600);
		const int cxScreen = GetSystemMetrics(SM_CXSCREEN), cyScreen = GetSystemMetrics(SM_CYSCREEN); // not influenced by dpi

		RECT rc = { (cxScreen - width) / 2, (cyScreen - height) / 2 };
		rc.right = rc.left + width;
		rc.bottom = rc.top + height;

		_AdjustWindowRectExForDpi(&rc, WS_OVERLAPPEDWINDOW, TRUE, 0, g_dpi);
		SetWindowPos(hWnd, nullptr, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);

		CreateControls(hWnd);
	}
	break;
	case WM_SIZE:
		OnSize(lParam);
		break;
	case WM_DPICHANGED:
		OnDpiChanged(hWnd, wParam, lParam);
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_DESTROY:
		g_hFont.reset();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
