#pragma once

void LVEnableVisualStyles(HWND hWnd, int exStyle = 0)
{
	SetWindowTheme(hWnd, L"Explorer", nullptr);
	ListView_SetExtendedListViewStyle(hWnd, LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | exStyle);
}
