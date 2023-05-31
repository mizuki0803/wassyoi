#pragma once
#include <Windows.h>
#include "Singleton.h"

class Timer final
	: public Singleton<Timer>
{
public:
	friend class Singleton<Timer>; // インスタンス作成を許可

	void InstrumentationStart();
	double InstrumentationEnd();
	bool CheckFixedUpdate()
	{
		return fixed_update_time_ >= fixed_delta_time_;
	}
	void AddFixedTimer()
	{
		// 固定長更新用タイマーに加算
		fixed_update_time_ += delta_time_;
	}
	void SubFixedTimer()
	{
		fixed_update_time_ -= fixed_delta_time_;
	}
	void ClearFixedTimer()
	{
		fixed_update_time_ = 0.0f;
	}
	

private:

	Timer();

	int frame_count_;					// 最初のシーンが起動してからの経過フレーム
	double time_;						// 最初のシーンが起動してからの経過時間（秒）

	double frame_rate_;					// 1フレームにかける時間
	double fixed_delta_time_;			// 固定長更新にかける時間
	LARGE_INTEGER frequency_;			// 周波数
	LARGE_INTEGER time_count_start_;	// 計測開始時間
	LARGE_INTEGER time_count_end_;		// 計測終了時間
	double delta_time_{0.0};					// 1フレームでの経過時間
	double fixed_update_time_;			// 固定長更新用タイマー
};

