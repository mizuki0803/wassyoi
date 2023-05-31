#pragma once
#include <Windows.h>
#include "Singleton.h"

class Timer final
	: public Singleton<Timer>
{
public:
	friend class Singleton<Timer>; // �C���X�^���X�쐬������

	void InstrumentationStart();
	double InstrumentationEnd();
	bool CheckFixedUpdate()
	{
		return fixed_update_time_ >= fixed_delta_time_;
	}
	void AddFixedTimer()
	{
		// �Œ蒷�X�V�p�^�C�}�[�ɉ��Z
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

	int frame_count_;					// �ŏ��̃V�[�����N�����Ă���̌o�߃t���[��
	double time_;						// �ŏ��̃V�[�����N�����Ă���̌o�ߎ��ԁi�b�j

	double frame_rate_;					// 1�t���[���ɂ����鎞��
	double fixed_delta_time_;			// �Œ蒷�X�V�ɂ����鎞��
	LARGE_INTEGER frequency_;			// ���g��
	LARGE_INTEGER time_count_start_;	// �v���J�n����
	LARGE_INTEGER time_count_end_;		// �v���I������
	double delta_time_{0.0};					// 1�t���[���ł̌o�ߎ���
	double fixed_update_time_;			// �Œ蒷�X�V�p�^�C�}�[
};

