#include "Timer.h"
#include <thread>

Timer::Timer():

	frame_count_(0),             // 経過フレーム初期化
	time_(0),                    // 経過時間初期化

	fixed_delta_time_(0.02),     // 0.02秒ごとにFixedUpdate
	fixed_update_time_(0.0),

	frame_rate_(1.0f / 60.0f)
{

}

void Timer::InstrumentationStart()
{
    // 周波数取得
    QueryPerformanceFrequency(&frequency_);
    // 計測開始
    QueryPerformanceCounter(&time_count_start_);
}

double Timer::InstrumentationEnd()
{
    // 現在時間を取得
    QueryPerformanceCounter(&time_count_end_);
    // 経過時間計測
    delta_time_ = static_cast<double>(time_count_end_.QuadPart - time_count_start_.QuadPart) / static_cast<double>(frequency_.QuadPart);

    // 経過秒数加算
    time_ += delta_time_;
    // 経過フレーム数増加
    ++frame_count_;

    // フレームレート固定
    if (delta_time_ <= frame_rate_)
    {
        DWORD sleep_time = static_cast<DWORD>((frame_rate_ - delta_time_) * 1000);
        timeBeginPeriod(1);
        Sleep(sleep_time);
        timeEndPeriod(1);
    }

    return delta_time_;
}
