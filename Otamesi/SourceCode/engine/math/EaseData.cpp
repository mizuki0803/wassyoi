#include "EaseData.h"

EaseData::EaseData(int count)
{
	countNum_ = count;
}

void EaseData::Update()
{
	if (!endFlag_ && actFlag_)
	{
		timeRate_ = timer_ / countNum_;
		timer_++;

		if (timer_ > countNum_)
		{
			endFlag_ = true;
			timer_ = 0.0f;
		}
	}
}

void EaseData::Reset()
{
	timer_ = 0.0f;
	timeRate_ = 0.0f;
	endFlag_ = false;
}
