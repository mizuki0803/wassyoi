#pragma once
#include <Windows.h>

class FpsCheck
{
public: //静的メンバ関数	
	/// <summary>
	/// 表示
	/// </summary>
	static void Display();

private: //静的メンバ変数
	//前回の時間
	static DWORD prevTime;
	//フレームカウント
	static int frameCount;
};