#pragma once
#include "MapData.h"
#include "StageBlock.h"

/// <summary>
/// �X�e�[�W�p�}�b�v�f�[�^
/// </summary>
class MapDataStage : public MapData
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="selectStageNum">�I�𒆂̃X�e�[�W�ԍ�</param>
	/// <returns>�X�e�[�W�p�}�b�v�f�[�^</returns>
	static MapDataStage* Create(const int selectStageNum);


public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	//getter
	const XMINT3& GetPlayerCreateMapChipNum() { return playerCreateMapChipNum; }

protected: //�����o�֐�
	/// <summary>
	/// �}�b�v�u���b�N����
	/// </summary>
	void CreateMapBlock() override;

	/// <summary>
	/// �}�b�v�u���b�N�Đ���
	/// </summary>
	void ReCreateMapBlock();

	/// <summary>
	/// �u���b�N���グ��ԍ��̐���
	/// </summary>
	void BlockCountCreate();

protected: //�����o�ϐ�
	//�X�e�[�W�p�u���b�N
	std::vector<std::unique_ptr<StageBlock>> blocks;

	//�v���C���[�𐶐�����}�b�v�ԍ�
	XMINT3 playerCreateMapChipNum;

	// �グ��u���b�N�̔ԍ��̕ۑ�
	std::vector<int> rndcount;
	// �u���b�N���グ��^�C�}�[
	int blockActTimer_ = 100;
	// �u���b�N���グ�I�������
	bool isBolckUp_ = false;
};