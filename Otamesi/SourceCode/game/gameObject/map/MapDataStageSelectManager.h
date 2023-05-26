#pragma once
#include "MapDataStageSelect.h"
#include "StageSelectBlockManager.h"

/// <summary>
/// �X�e�[�W�I���V�[���S�u���b�N�֘A�̊Ǘ�
/// </summary>
class MapDataStageSelectManager
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�X�e�[�W�I���V�[���S�u���b�N�֘A�̊Ǘ�</returns>
	static MapDataStageSelectManager* Create();

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private: //�����o�֐�
	/// <summary>
	/// �I������X�e�[�W��ύX����
	/// </summary>
	void ChangeStage();

private: //�����o�ϐ�
	//1�X�e�[�W���̃}�b�v�f�[�^�Q
	std::list<std::unique_ptr<MapDataStageSelect>> mapDatas;
};