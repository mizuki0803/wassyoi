#pragma once
#include <Windows.h>

/// <summary>
/// �E�C���h�E�A�v���P�[�V����
/// </summary>
class WindowApp
{
public:
	//�E�C���h�E�T�C�Y
	static const int window_width = 1280;	//����
	static const int window_height = 720;	//�c��

public:
	//�E�C���h�E�v���V�[�W��
	static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	/// <summary>
	/// �E�C���h�E�쐬
	/// </summary>
	/// <param name="titlename">�^�C�g���o�[��</param>
	void WindowCreate(const wchar_t* titlename = L"DirectXGame");

	/// <summary>
	/// �E�C���h�E���
	/// </summary>
	void WindowRelease();

	/// <summary>
	/// ���b�Z�[�W
	/// </summary>
	/// <returns></returns>
	bool MessageProc();

	//getter
	HINSTANCE GetHInstance() { return wndclass.hInstance; };
	HWND GetHwnd() { return hwnd; };

private:
	WNDCLASSEX wndclass{};	//�E�C���h�E�N���X�̐ݒ�
	HWND hwnd = nullptr;
};