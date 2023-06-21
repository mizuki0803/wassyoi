#pragma once
#include "MapData.h"
#include "StageBlock.h"

/// <summary>
/// �X�e�[�W�p�}�b�v�f�[�^
/// </summary>
class MapDataStage : public MapData
{
public:
	/// <summary>
	/// �Q�[���t�F�[�Y
	/// </summary>
	enum class GamePhase
	{
		GamePlay,	//�Q�[��
		Start,		//�J�n
		ReStart,	//�ĊJ�n
	};

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

	/// <summary>
	/// �Q�[����
	/// </summary>
	void PlayGame();
	/// <summary>
	/// �Q�[���J�n
	/// </summary>
	void GameStart();
	/// <summary>
	/// �Q�[���ĊJ�n
	/// </summary>
	void GameReStart();
	/// <summary>
	/// �֐��̐ݒ�
	/// </summary>
	void CreateAct();

	/// <summary>
	/// �}�b�v�u���b�N�Đ���
	/// </summary>
	void ReCreateMapBlock(const int selectStageNum);

	//getter
	const XMINT3& GetPlayerCreateMapChipNum() { return playerCreateMapChipNum; }
	const bool GetIsReCreateEnd() { return isReCreateEnd; }

	//setter
	void SetIsReCreateEnd(const bool isReCreateEnd) { this->isReCreateEnd = isReCreateEnd; }

protected: //�����o�֐�
	/// <summary>
	/// �}�b�v�u���b�N����
	/// </summary>
	void CreateMapBlock() override;

	/// <summary>
	/// �u���b�N���グ��ԍ��̐���
	/// </summary>
	void BlockCountCreate();

	bool BlockCount();

protected: //�����o�ϐ�
	//�X�e�[�W�p�u���b�N
	std::vector<std::unique_ptr<StageBlock>> blocks;

	//�v���C���[�𐶐�����}�b�v�ԍ�
	XMINT3 playerCreateMapChipNum;
	// �֐��̊Ǘ�
	std::vector<std::function<void()>> func_;
	// �֐��̔ԍ�
	size_t phase_ = static_cast<int>(GamePhase::Start);
	// �グ��u���b�N�̔ԍ��̕ۑ�
	std::vector<int> rndcount;
	// �u���b�N���グ��^�C�}�[
	int blockActTimer_ = 100;
	//�}�b�v�Đ������I��������
	bool isReCreateEnd = false;
};