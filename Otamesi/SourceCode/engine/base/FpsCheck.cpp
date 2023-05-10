#include <stdio.h>
#include "FpsCheck.h"

#pragma comment(lib, "Winmm.lib")

DWORD FpsCheck::prevTime = timeGetTime();	// 前回の時間
int FpsCheck::frameCount = 0;	// フレームカウント

void FpsCheck::Display()
{
	DWORD now_time = timeGetTime();	//今回のフレームの時間

	frameCount++;	//フレーム数をカウントする

	// 経過時間が１秒を超えたらカウントと時間をリセット
	if (now_time - prevTime >= 1000)
	{
		char str[128];
		sprintf_s(str, "%f\n", (float)frameCount);
		OutputDebugStringA(str);
		prevTime = now_time;
		frameCount = 0;
	}
}
