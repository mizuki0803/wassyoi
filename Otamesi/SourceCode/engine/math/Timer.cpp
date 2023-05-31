#include "Timer.h"
#include <thread>

Timer::Timer():

	frame_count_(0),             // �o�߃t���[��������
	time_(0),                    // �o�ߎ��ԏ�����

	fixed_delta_time_(0.02),     // 0.02�b���Ƃ�FixedUpdate
	fixed_update_time_(0.0),

	frame_rate_(1.0f / 60.0f)
{

}

void Timer::InstrumentationStart()
{
    // ���g���擾
    QueryPerformanceFrequency(&frequency_);
    // �v���J�n
    QueryPerformanceCounter(&time_count_start_);
}

double Timer::InstrumentationEnd()
{
    // ���ݎ��Ԃ��擾
    QueryPerformanceCounter(&time_count_end_);
    // �o�ߎ��Ԍv��
    delta_time_ = static_cast<double>(time_count_end_.QuadPart - time_count_start_.QuadPart) / static_cast<double>(frequency_.QuadPart);

    // �o�ߕb�����Z
    time_ += delta_time_;
    // �o�߃t���[��������
    ++frame_count_;

    // �t���[�����[�g�Œ�
    if (delta_time_ <= frame_rate_)
    {
        DWORD sleep_time = static_cast<DWORD>((frame_rate_ - delta_time_) * 1000);
        timeBeginPeriod(1);
        Sleep(sleep_time);
        timeEndPeriod(1);
    }

    return delta_time_;
}
