#pragma once
#include <array>

/// <summary>
/// �X�e�[�W�Ǘ�
/// </summary>
class StageManager
{
public: //enum
	/// <summary>
	/// �X�e�[�W��
	/// </summary>
	enum StageName
	{
		Stage1,
		Stage2,
		Stage3,

		StageNum,	//�X�e�[�W��
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// �w�肵���X�e�[�W���N���A���Ă��邩
	/// </summary>
	/// <param name="stageName">�X�e�[�W��</param>
	/// <returns>�X�e�[�W���N���A���Ă��邩</returns>
	static bool GetIsClear(const StageName stageName);

	/// <summary>
	/// �w�肵���X�e�[�W���N���A��Ԃɂ���
	/// </summary>
	/// <param name="stageName">�X�e�[�W��</param>
	static void StageClear(const StageName stageName);

	//getter
	static StageName GetSelectStage() { return StageManager::selectStage; }

	//setter
	static void SetSelectStage(const StageName stageName) { StageManager::selectStage = selectStage; }

private: //�ÓI�����o�ϐ�
	//�I�𒆂̃X�e�[�W��
	static StageName selectStage;
	//�X�e�[�W�N���A���Ă��邩
	static std::array<bool, StageNum> isClear;
};