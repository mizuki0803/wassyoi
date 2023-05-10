#include <stdio.h>
#include "FpsCheck.h"

#pragma comment(lib, "Winmm.lib")

DWORD FpsCheck::prevTime = timeGetTime();	// �O��̎���
int FpsCheck::frameCount = 0;	// �t���[���J�E���g

void FpsCheck::Display()
{
	DWORD now_time = timeGetTime();	//����̃t���[���̎���

	frameCount++;	//�t���[�������J�E���g����

	// �o�ߎ��Ԃ��P�b�𒴂�����J�E���g�Ǝ��Ԃ����Z�b�g
	if (now_time - prevTime >= 1000)
	{
		char str[128];
		sprintf_s(str, "%f\n", (float)frameCount);
		OutputDebugStringA(str);
		prevTime = now_time;
		frameCount = 0;
	}
}
