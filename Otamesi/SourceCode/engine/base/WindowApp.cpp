#include "WindowApp.h"

LRESULT WindowApp::windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//メッセージを分岐
	switch (msg) {
	case WM_DESTROY:		//ウインドウが破棄された
		PostQuitMessage(0);	//OSに対して、アプリの終了を伝える
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);	//標準の処理を行う
}

void WindowApp::WindowCreate(const wchar_t* titlename)
{
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.lpfnWndProc = (WNDPROC)windowProc;			//ウインドウブロージャを設定
	wndclass.lpszClassName = titlename;					//ウインドウクラス名
	wndclass.hInstance = GetModuleHandle(nullptr);		//ウインドウハンドル
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);		//カーソル指定

	//ウインドウクラスをOSに設定
	RegisterClassEx(&wndclass);

	//ウインドウサイズ{X座標,Y座標, 横幅, 縦幅}
	RECT wrc = { 0, 0, window_width, window_height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//ウインドウオブジェクトの生成
	hwnd = CreateWindow(wndclass.lpszClassName,	//クラス名
		titlename,				//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	//標準的なウインドウスタイル
		CW_USEDEFAULT,			//表示X座標（OSに任せる）
		CW_USEDEFAULT,			//表示Y座標（OSに任せる）
		wrc.right - wrc.left,	//ウインドウ横幅
		wrc.bottom - wrc.top,	//ウインドウ縦幅
		nullptr,				//親ウインドウハンドル
		nullptr,				//メニューハンドル
		wndclass.hInstance,			//呼び出しアプリケーションハンドル
		nullptr					//オプション
	);

	//ウインドウ表示
	ShowWindow(hwnd, SW_SHOW);
}

void WindowApp::WindowRelease()
{
	UnregisterClass(wndclass.lpszClassName, wndclass.hInstance);
}

bool WindowApp::MessageProc()
{
	MSG msg{};	//メッセージ

	//メッセージがある？
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);	//キー入力メッセージの処理
		DispatchMessage(&msg);	//プロシージャにメッセージを送る
	}

	//×ボタンで終了メッセージが来たらゲームループを抜ける
	if (msg.message == WM_QUIT) {
		return true;
	}

	return false;
}
