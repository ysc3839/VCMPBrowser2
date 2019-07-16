#pragma once

using fnAdjustWindowRectExForDpi = decltype(AdjustWindowRectExForDpi)*;
using fnGetDpiForWindow = decltype(GetDpiForWindow)*;
using fnGetSystemMetricsForDpi = decltype(GetSystemMetricsForDpi)*;
using fnSystemParametersInfoForDpi = decltype(SystemParametersInfoForDpi)*;

fnAdjustWindowRectExForDpi _fnAdjustWindowRectExForDpi = nullptr;
fnGetDpiForWindow _fnGetDpiForWindow = nullptr;
fnGetSystemMetricsForDpi _fnGetSystemMetricsForDpi = nullptr;
fnSystemParametersInfoForDpi _fnSystemParametersInfoForDpi = nullptr;

uint32_t g_dpi = 0;

// https://stackoverflow.com/a/25065519
int muldiv(int number, int numerator, int denominator)
{
	return (int)(((long)number * numerator + (denominator >> 1)) / denominator);
}

void InitDPIAPI()
{
	auto hUser = GetModuleHandleW(L"user32.dll");
	if (hUser)
	{
		_fnAdjustWindowRectExForDpi = GetProcAddressByFunctionDeclaration(hUser, AdjustWindowRectExForDpi);
		_fnGetDpiForWindow = GetProcAddressByFunctionDeclaration(hUser, GetDpiForWindow);
		//_fnGetSystemMetricsForDpi = GetProcAddressByFunctionDeclaration(hUser, GetSystemMetricsForDpi);
		_fnSystemParametersInfoForDpi = GetProcAddressByFunctionDeclaration(hUser, SystemParametersInfoForDpi);
	}
}

UINT _GetDpiForWindow(HWND hWnd)
{
	if (_fnGetDpiForWindow)
		return _fnGetDpiForWindow(hWnd);

	UINT dpi = 0;

	auto hdc = wil::GetDC(hWnd);
	if (hdc)
		dpi = GetDeviceCaps(hdc.get(), LOGPIXELSX);

	return dpi;
}

BOOL _AdjustWindowRectExForDpi(LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle, UINT dpi)
{
	if (_fnAdjustWindowRectExForDpi)
		return _fnAdjustWindowRectExForDpi(lpRect, dwStyle, bMenu, dwExStyle, dpi);
	return AdjustWindowRectEx(lpRect, dwStyle, bMenu, dwExStyle);
}

int _GetSystemMetricsForDpi(int nIndex, UINT dpi)
{
	if (_fnGetSystemMetricsForDpi)
		return _fnGetSystemMetricsForDpi(nIndex, dpi);
	return GetSystemMetrics(nIndex);
}

BOOL _SystemParametersInfoForDpi(UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni, UINT dpi)
{
	if (_fnSystemParametersInfoForDpi)
		return _fnSystemParametersInfoForDpi(uiAction, uiParam, pvParam, fWinIni, dpi);
	return SystemParametersInfoW(uiAction, uiParam, pvParam, fWinIni);
}

int Scale(int i)
{
	if (g_dpi == 0)
		return i;
	else
		return muldiv(i, g_dpi, USER_DEFAULT_SCREEN_DPI);
}

// user-defined literal for dpi scale
int operator""_s(unsigned long long i)
{
	return Scale(static_cast<int>(i));
}
