#pragma once
#include <Windows.h>

/// <summary>
/// ウインドウアプリケーション
/// </summary>
class WindowApp
{
public:
	//ウインドウサイズ
	static const int window_width = 1280;	//横幅
	static const int window_height = 720;	//縦幅

public:
	//ウインドウプロシージャ
	static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	/// <summary>
	/// ウインドウ作成
	/// </summary>
	/// <param name="titlename">タイトルバー名</param>
	void WindowCreate(const wchar_t* titlename = L"DirectXGame");

	/// <summary>
	/// ウインドウ解放
	/// </summary>
	void WindowRelease();

	/// <summary>
	/// メッセージ
	/// </summary>
	/// <returns></returns>
	bool MessageProc();

	//getter
	HINSTANCE GetHInstance() { return wndclass.hInstance; };
	HWND GetHwnd() { return hwnd; };

private:
	WNDCLASSEX wndclass{};	//ウインドウクラスの設定
	HWND hwnd = nullptr;
};