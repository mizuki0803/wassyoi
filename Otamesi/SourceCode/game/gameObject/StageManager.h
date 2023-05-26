#pragma once
#include <array>

/// <summary>
/// �X�e�[�W�Ǘ�
/// </summary>
class StageManager
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// �w�肵���X�e�[�W���N���A���Ă��邩
	/// </summary>
	/// <param name="stageName">�X�e�[�W��</param>
	/// <returns>�X�e�[�W���N���A���Ă��邩</returns>
	static bool GetIsClear(const int selectStage);

	/// <summary>
	/// ���ݑI�𒆂̃X�e�[�W���N���A��Ԃɂ���
	/// </summary>
	static void StageClear();

	/// <summary>
	/// ���ݑI�𒆂̃X�e�[�W��getter
	/// </summary>
	/// <returns>���ݑI�𒆂̃X�e�[�W</returns>
	static int GetSelectStage() { return StageManager::selectStage; }

	/// <summary>
	/// ���ݑI�𒆂̃X�e�[�W��setter
	/// </summary>
	static void SetSelectStage(const int selectStage);

	/// <summary>
	/// �I�𒆂̃X�e�[�W�����Ɉړ�������
	/// </summary>
	static bool NextStageSelect();

	/// <summary>
	/// �I�𒆂̃X�e�[�W��O�Ɉړ�������
	/// </summary>
	static bool PrevStageSelect();

private: //�ÓI�����o�ϐ�(�萔)
	static const int stageNum = 6;

private: //�ÓI�����o�ϐ�
	//�I�𒆂̃X�e�[�W
	static int selectStage;
	//�X�e�[�W�N���A���Ă��邩
	static std::array<bool, stageNum> isClear;
};