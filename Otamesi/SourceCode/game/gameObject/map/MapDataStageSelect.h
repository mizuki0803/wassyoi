#pragma once
#include "MapData.h"
#include "StageSelectBlock.h"
#include "StageSelectBlockManager.h"

/// <summary>
/// �X�e�[�W�I��p�}�b�v�f�[�^
/// </summary>
class MapDataStageSelect : public MapData
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="selectStageNum">�I�𒆂̃X�e�[�W�ԍ�</param>
	/// <param name="positionPhase">�u���b�N�Ǘ��̏ꏊ�t�F�[�Y</param>
	/// <returns>�X�e�[�W�I��p�}�b�v�f�[�^</returns>
	static MapDataStageSelect* Create(const int selectStageNum, const StageSelectBlockManager::BlockManagerPositionPhase& positionPhase);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="selectStageNum">�I�𒆂̃X�e�[�W�ԍ�</param>
	/// <param name="positionPhase">�u���b�N�Ǘ��̏ꏊ�t�F�[�Y</param>
	/// <returns>����</returns>
	bool Initialize(const int selectStageNum, const StageSelectBlockManager::BlockManagerPositionPhase& positionPhase);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	//getter
	StageSelectBlockManager* GetStageSelectBlockManager() { return blockManager.get(); }

private: //�����o�֐�
	/// <summary>
	/// �}�b�v�u���b�N����
	/// </summary>
	void CreateMapBlock() override;

private: //�����o�ϐ�
	//�v���C���[���f��
	std::unique_ptr<ObjModel> modelPlayer;

	//�X�e�[�W�I��p�u���b�N
	std::vector<std::unique_ptr<StageSelectBlock>> blocks;
	//�X�e�[�W�I��p�u���b�N�Ǘ�
	std::unique_ptr<StageSelectBlockManager> blockManager;
};