#pragma once

void EDFixSearchWeb(HWND hWndParent)
{
	static WNDPROC prevClsProc = nullptr;
	if (!prevClsProc)
	{
		HWND hWnd = CreateWindowW(WC_EDITW, nullptr, WS_CHILD, 0, 0, 0, 0, hWndParent, nullptr, nullptr, nullptr);
		if (hWnd)
		{
			prevClsProc = reinterpret_cast<WNDPROC>(SetClassLongPtrW(hWnd, GCLP_WNDPROC, reinterpret_cast<LONG_PTR>(static_cast<WNDPROC>(
			[](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT {
				if (message == WM_CREATE)
				{
					PostMessageW(hWnd, EM_ENABLESEARCHWEB, TRUE, 0);
				}
				else if (message == EM_SEARCHWEB)
				{
					const int len = static_cast<int>(CallWindowProcW(prevClsProc, hWnd, WM_GETTEXTLENGTH, 0, 0));
					if (len == 0)
						return 0;

					DWORD start = 0, end = 0;
					CallWindowProcW(prevClsProc, hWnd, EM_GETSEL, reinterpret_cast<WPARAM>(&start), reinterpret_cast<LPARAM>(&end));

					if (!(start < end))
						CallWindowProcW(prevClsProc, hWnd, EM_SETSEL, 0, -1);
				}
				return CallWindowProcW(prevClsProc, hWnd, message, wParam, lParam);
			}))));
			DestroyWindow(hWnd);
		}
	}
}
