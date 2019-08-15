#pragma once

#include "ListViewUtil.hpp"

HWND g_hMainWnd;
HWND g_hWndLVServers;
HWND g_hWndLVHistory;
HWND g_hWndEditName;
HWND g_hWndTab;
HWND g_hWndLVPlayers;
HWND g_hWndGB1; // Players
HWND g_hWndGB2; // Server Info
// Server Info Labels and Edits
#define SERVER_INFOS 5
HWND g_hWndStaticList[SERVER_INFOS];
HWND g_hWndEditList[SERVER_INFOS];
HWND g_hWndSB;

// UI sizes
#define UI_PLAYERLIST_WIDTH 200
#define UI_SERVERINFO_HEIGHT 140
#define UI_NAMEEDIT_WIDTH 150

wil::unique_hfont g_hFont;
SIZE maxTextSize = {}; // cached for WM_SIZE
const wchar_t* infoTexts[SERVER_INFOS];

void UpdateDpiDependentFontsAndResources()
{
	HFONT hFont = 0;
	NONCLIENTMETRICSW ncm = { sizeof(ncm) };
	if (_SystemParametersInfoForDpi(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0, g_dpi))
	{
		hFont = CreateFontIndirectW(&ncm.lfMessageFont);
		g_hFont.reset(hFont);
	}
	if (!hFont)
		hFont = reinterpret_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT));

	SetWindowFont(g_hWndTab, hFont, FALSE);
	SetWindowFont(g_hWndEditName, hFont, FALSE);
	SetWindowFont(g_hWndGB1, hFont, FALSE);
	SetWindowFont(g_hWndGB2, hFont, FALSE);

	auto hdc = wil::GetDC(g_hMainWnd);
	auto so = wil::SelectObject(hdc.get(), hFont);

	maxTextSize = {};
	for (size_t i = 0; i < SERVER_INFOS; ++i)
	{
		SetWindowFont(g_hWndStaticList[i], hFont, FALSE);
		SetWindowFont(g_hWndEditList[i], hFont, FALSE);

		SIZE size;
		if (GetTextExtentPoint32W(hdc.get(), infoTexts[i], static_cast<int>(wcslen(infoTexts[i])), &size))
		{
			if (size.cx > maxTextSize.cx)
				maxTextSize = size;
		}
	}
}

void CreateControls(HWND hWnd)
{
	auto hImlLocks = ImageList_LoadImageW(g_hInst, MAKEINTRESOURCEW(IDB_LOCKICONS), 16, 0, CLR_NONE, IMAGE_BITMAP, LR_CREATEDIBSECTION);

	g_hWndLVServers = CreateWindowW(WC_LISTVIEWW, nullptr, WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_OWNERDATA, 0, 0, 0, 0, hWnd, nullptr, g_hInst, nullptr);
	if (g_hWndLVServers)
	{
		LVEnableVisualStyles(g_hWndLVServers, LVS_EX_HEADERDRAGDROP);

		ListView_SetImageList(g_hWndLVServers, hImlLocks, LVSIL_SMALL);

		LVCOLUMNW lvc;
		lvc.mask = LVCF_WIDTH;
		lvc.cx = 24_s;
		ListView_InsertColumn(g_hWndLVServers, 0, &lvc);

		lvc.mask = LVCF_WIDTH | LVCF_TEXT;
		lvc.cx = 280_s;
		lvc.pszText = const_cast<LPWSTR>(_(L"Server Name"));
		ListView_InsertColumn(g_hWndLVServers, 1, &lvc);

		lvc.cx = 60_s;
		lvc.pszText = const_cast<LPWSTR>(_(L"Ping"));
		ListView_InsertColumn(g_hWndLVServers, 2, &lvc);

		lvc.cx = 80_s;
		lvc.pszText = const_cast<LPWSTR>(_(L"Players"));
		ListView_InsertColumn(g_hWndLVServers, 3, &lvc);

		lvc.cx = 70_s;
		lvc.pszText = const_cast<LPWSTR>(_(L"Version"));
		ListView_InsertColumn(g_hWndLVServers, 4, &lvc);

		lvc.cx = 140_s;
		lvc.pszText = const_cast<LPWSTR>(_(L"Gamemode"));
		ListView_InsertColumn(g_hWndLVServers, 5, &lvc);

		lvc.cx = 100_s;
		lvc.pszText = const_cast<LPWSTR>(_(L"Map Name"));
		ListView_InsertColumn(g_hWndLVServers, 6, &lvc);

		// FIXME
		//LoadFavorites();
		//ListView_SetItemCount(g_hWndListViewServers, g_favoriteList.size());
	}

	g_hWndLVHistory = CreateWindowW(WC_LISTVIEWW, nullptr, WS_CHILD | WS_CLIPSIBLINGS | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_OWNERDATA, 0, 0, 0, 0, hWnd, nullptr, g_hInst, nullptr);
	if (g_hWndLVHistory)
	{
		LVEnableVisualStyles(g_hWndLVHistory, LVS_EX_HEADERDRAGDROP);

		ListView_SetImageList(g_hWndLVHistory, hImlLocks, LVSIL_SMALL);

		LVCOLUMNW lvc;
		lvc.mask = LVCF_WIDTH;
		lvc.cx = 24_s;
		ListView_InsertColumn(g_hWndLVHistory, 0, &lvc);

		lvc.mask = LVCF_WIDTH | LVCF_TEXT;
		lvc.cx = 240_s;
		lvc.pszText = const_cast<LPWSTR>(_(L"Server Name"));
		ListView_InsertColumn(g_hWndLVHistory, 1, &lvc);

		lvc.cx = 60_s;
		lvc.pszText = const_cast<LPWSTR>(_(L"Ping"));
		ListView_InsertColumn(g_hWndLVHistory, 2, &lvc);

		lvc.cx = 80_s;
		lvc.pszText = const_cast<LPWSTR>(_(L"Players"));
		ListView_InsertColumn(g_hWndLVHistory, 3, &lvc);

		lvc.cx = 70_s;
		lvc.pszText = const_cast<LPWSTR>(_(L"Version"));
		ListView_InsertColumn(g_hWndLVHistory, 4, &lvc);

		lvc.cx = 120_s;
		lvc.pszText = const_cast<LPWSTR>(_(L"Gamemode"));
		ListView_InsertColumn(g_hWndLVHistory, 5, &lvc);

		lvc.cx = 160_s;
		lvc.pszText = const_cast<LPWSTR>(_(L"Last Played"));
		ListView_InsertColumn(g_hWndLVHistory, 6, &lvc);

		// FIXME
		//LoadHistory();
		//ListView_SetItemCount(g_hWndListViewHistory, g_historyList.size());
	}

	g_hWndEditName = CreateWindowW(WC_EDITW, nullptr, WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 0, 0, 0, 0, hWnd, nullptr, g_hInst, nullptr);
	if (g_hWndEditName)
	{
		Edit_SetCueBannerText(g_hWndEditName, _(L"Name"));
		Edit_LimitText(g_hWndEditName, PLAYER_NAME_MAX_LEN);
	}

	g_hWndTab = CreateWindowW(WC_TABCONTROLW, nullptr, WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE, 0, 0, 0, 0, hWnd, nullptr, g_hInst, nullptr);
	if (g_hWndTab)
	{
		auto hTabIml = ImageList_LoadImageW(g_hInst, MAKEINTRESOURCEW(IDB_TABICONS), 16, 0, CLR_NONE, IMAGE_BITMAP, LR_CREATEDIBSECTION);
		TabCtrl_SetImageList(g_hWndTab, hTabIml);

		TCITEMW ti;
		ti.mask = TCIF_TEXT | TCIF_IMAGE;
		ti.iImage = 0;
		ti.pszText = const_cast<LPWSTR>(_(L"Favorites"));
		TabCtrl_InsertItem(g_hWndTab, 0, &ti);

		ti.iImage = 1;
		ti.pszText = const_cast<LPWSTR>(_(L"Internet"));
		TabCtrl_InsertItem(g_hWndTab, 1, &ti);

		ti.iImage = 1;
		ti.pszText = const_cast<LPWSTR>(_(L"Official"));
		TabCtrl_InsertItem(g_hWndTab, 2, &ti);

		ti.iImage = 2;
		ti.pszText = const_cast<LPWSTR>(_(L"Lan"));
		TabCtrl_InsertItem(g_hWndTab, 3, &ti);

		ti.iImage = 3;
		ti.pszText = const_cast<LPWSTR>(_(L"History"));
		TabCtrl_InsertItem(g_hWndTab, 4, &ti);
	}

	g_hWndLVPlayers = CreateWindowW(WC_LISTVIEW, nullptr, WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_NOCOLUMNHEADER | LVS_SINGLESEL | LVS_OWNERDATA, 0, 0, 0, 0, hWnd, nullptr, g_hInst, nullptr);
	if (g_hWndLVPlayers)
	{
		LVEnableVisualStyles(g_hWndLVPlayers, LVS_EX_AUTOSIZECOLUMNS);

		LVCOLUMNW lvc;
		lvc.mask = LVCF_WIDTH;
		lvc.cx = INT_MAX;
		ListView_InsertColumn(g_hWndLVPlayers, 0, &lvc);
	}

	g_hWndGB1 = CreateWindowW(WC_BUTTONW, _(L"Players"), WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | BS_GROUPBOX, 0, 0, 0, 0, hWnd, nullptr, g_hInst, nullptr);

	g_hWndGB2 = CreateWindowW(WC_BUTTONW, _(L"Server Info"), WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | BS_GROUPBOX, 0, 0, 0, 0, hWnd, nullptr, g_hInst, nullptr);
	if (g_hWndGB2)
	{
		infoTexts[0] = _(L"Name:");
		infoTexts[1] = _(L"IP:");
		infoTexts[2] = _(L"Players:");
		infoTexts[3] = _(L"Ping:");
		infoTexts[4] = _(L"Gamemode:");

		for (size_t i = 0; i < SERVER_INFOS; ++i)
		{
			g_hWndStaticList[i] = CreateWindowW(WC_STATICW, infoTexts[i], WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | SS_RIGHT, 0, 0, 0, 0, g_hWndGB2, nullptr, g_hInst, nullptr);
			g_hWndEditList[i] = CreateWindowW(WC_EDITW, nullptr, WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | ES_READONLY | ES_AUTOHSCROLL, 0, 0, 0, 0, g_hWndGB2, nullptr, g_hInst, nullptr);
		}
	}

	g_hWndSB = CreateWindowW(STATUSCLASSNAMEW, nullptr, WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | SBARS_SIZEGRIP, 0, 0, 0, 0, hWnd, nullptr, g_hInst, nullptr);

	UpdateDpiDependentFontsAndResources();
}

void OnSize(LPARAM lParam)
{
	const int width = GET_X_LPARAM(lParam), height = GET_Y_LPARAM(lParam);
	auto hDWP = BeginDeferWindowPos(7);
	if (hDWP)
	{
		RECT rc = { 0, 0, width - Scale(UI_PLAYERLIST_WIDTH), height - Scale(UI_SERVERINFO_HEIGHT) };
		DeferWindowPos(hDWP, g_hWndTab, 0, 0, 0, rc.right, rc.bottom, SWP_NOZORDER | SWP_NOACTIVATE);
		TabCtrl_AdjustRect(g_hWndTab, FALSE, &rc);
		DeferWindowPos(hDWP, g_hWndLVServers, 0, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
		DeferWindowPos(hDWP, g_hWndLVHistory, 0, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
		DeferWindowPos(hDWP, g_hWndEditName, 0, width - Scale(UI_PLAYERLIST_WIDTH) - Scale(UI_NAMEEDIT_WIDTH) - 2, 0, Scale(UI_NAMEEDIT_WIDTH), rc.top - 2_s, SWP_NOZORDER | SWP_NOACTIVATE);
		DeferWindowPos(hDWP, g_hWndGB1, 0, width - Scale(UI_PLAYERLIST_WIDTH), 0, Scale(UI_PLAYERLIST_WIDTH), height - Scale(UI_SERVERINFO_HEIGHT), SWP_NOZORDER | SWP_NOACTIVATE);
		DeferWindowPos(hDWP, g_hWndLVPlayers, 0, width - Scale(UI_PLAYERLIST_WIDTH - 1), 18_s, Scale(UI_PLAYERLIST_WIDTH - 2), height - Scale(UI_SERVERINFO_HEIGHT + 18 + 2), SWP_NOZORDER | SWP_NOACTIVATE);
		DeferWindowPos(hDWP, g_hWndGB2, 0, 0, height - Scale(UI_SERVERINFO_HEIGHT), width, 118_s, SWP_NOZORDER | SWP_NOACTIVATE);
		EndDeferWindowPos(hDWP);
	}

	hDWP = BeginDeferWindowPos(SERVER_INFOS * 2);
	if (hDWP)
	{
		const int lineGap = 4_s,
			labelX = 8_s,
			labelWidth = maxTextSize.cx,
			editX = labelX + labelWidth + 2_s,
			editWidth = 300_s,
			height = maxTextSize.cy;
		int y = 18_s;

		for (int i = 0; i < SERVER_INFOS; ++i)
		{
			DeferWindowPos(hDWP, g_hWndStaticList[i], 0, labelX, y, labelWidth, height, SWP_NOZORDER | SWP_NOACTIVATE);
			DeferWindowPos(hDWP, g_hWndEditList[i], 0, editX, y, editWidth, height, SWP_NOZORDER | SWP_NOACTIVATE);
			y += height + lineGap;
		}
		EndDeferWindowPos(hDWP);
	}

	SendMessageW(g_hWndSB, WM_SIZE, 0, 0);
}

void OnDpiChanged(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	g_dpi = LOWORD(wParam);
	UpdateDpiDependentFontsAndResources();
	auto rc = reinterpret_cast<LPCRECT>(lParam);
	SetWindowPos(hWnd, nullptr, rc->left, rc->top, rc->right - rc->left, rc->bottom - rc->top, SWP_NOZORDER | SWP_NOACTIVATE);
}
