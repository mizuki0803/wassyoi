#pragma once
#include <Windows.h>

class FpsCheck
{
public: //�ÓI�����o�֐�	
	/// <summary>
	/// �\��
	/// </summary>
	static void Display();

private: //�ÓI�����o�ϐ�
	//�O��̎���
	static DWORD prevTime;
	//�t���[���J�E���g
	static int frameCount;
};