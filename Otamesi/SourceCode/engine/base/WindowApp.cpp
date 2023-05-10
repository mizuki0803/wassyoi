#include "WindowApp.h"

LRESULT WindowApp::windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//���b�Z�[�W�𕪊�
	switch (msg) {
	case WM_DESTROY:		//�E�C���h�E���j�����ꂽ
		PostQuitMessage(0);	//OS�ɑ΂��āA�A�v���̏I����`����
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);	//�W���̏������s��
}

void WindowApp::WindowCreate(const wchar_t* titlename)
{
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.lpfnWndProc = (WNDPROC)windowProc;			//�E�C���h�E�u���[�W����ݒ�
	wndclass.lpszClassName = titlename;					//�E�C���h�E�N���X��
	wndclass.hInstance = GetModuleHandle(nullptr);		//�E�C���h�E�n���h��
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);		//�J�[�\���w��

	//�E�C���h�E�N���X��OS�ɐݒ�
	RegisterClassEx(&wndclass);

	//�E�C���h�E�T�C�Y{X���W,Y���W, ����, �c��}
	RECT wrc = { 0, 0, window_width, window_height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//�E�C���h�E�I�u�W�F�N�g�̐���
	hwnd = CreateWindow(wndclass.lpszClassName,	//�N���X��
		titlename,				//�^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,	//�W���I�ȃE�C���h�E�X�^�C��
		CW_USEDEFAULT,			//�\��X���W�iOS�ɔC����j
		CW_USEDEFAULT,			//�\��Y���W�iOS�ɔC����j
		wrc.right - wrc.left,	//�E�C���h�E����
		wrc.bottom - wrc.top,	//�E�C���h�E�c��
		nullptr,				//�e�E�C���h�E�n���h��
		nullptr,				//���j���[�n���h��
		wndclass.hInstance,			//�Ăяo���A�v���P�[�V�����n���h��
		nullptr					//�I�v�V����
	);

	//�E�C���h�E�\��
	ShowWindow(hwnd, SW_SHOW);
}

void WindowApp::WindowRelease()
{
	UnregisterClass(wndclass.lpszClassName, wndclass.hInstance);
}

bool WindowApp::MessageProc()
{
	MSG msg{};	//���b�Z�[�W

	//���b�Z�[�W������H
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);	//�L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg);	//�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
	}

	//�~�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
	if (msg.message == WM_QUIT) {
		return true;
	}

	return false;
}
