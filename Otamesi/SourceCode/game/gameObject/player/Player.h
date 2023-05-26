#pragma once
#include "ObjObject3d.h"
#include "GameCamera.h"
#include "EaseData.h"

/// <summary>
/// �v���C���[
/// </summary>
class Player : public ObjObject3d
{
public: //enum
	/// <summary>
	/// �s���t�F�[�Y
	/// </summary>
	enum class ActionPhase
	{
		None,	//�������Ȃ�
		MovePos,	//���W�ړ�
	};

	/// <summary>
	/// �㉺���E�O��A�u���b�N�̂ǂ̖ʈړ����邩�t�F�[�Y
	/// </summary>
	enum class MoveSurfacePhase
	{
		Upward,			//�����
		Downward,		//������
		FacingLeft,		//������
		FacingRight,	//�E����
		FacingForward,	//��O����
		FacingAway,		//������
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="mapChipNum">�v���C���[�ʒu��\���}�b�v�ԍ�</param>
	/// <param name="gameCamera">�Q�[���J����</param>
	/// <returns>�v���C���[</returns>
	static Player* Create(ObjModel* model, const XMINT3& mapChipNum, GameCamera* gameCamera);

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	//getter
	const bool GetIsGoal() { return isGoal; }
	const bool GetIsMove() { return isMove; }
	const bool GetNowMove() { return bool(actionPhase); }
	const int GetMoveSurfacePhase() { return int(moveSurfacePhase); }
	const XMINT3 GetMapChipNumberPos() { return mapChipNumberPos; }
	//setter
	void SetMoveSurfacePhase(const int _moveSurfacePhase) { moveSurfacePhase = MoveSurfacePhase(_moveSurfacePhase); }
	void SetMapChipNumberPos(const XMINT3& _mapChipNumberPos) { mapChipNumberPos = _mapChipNumberPos; }

private: //�����o�֐�
	/// <summary>
	/// ���W�ړ��J�n
	/// </summary>
	void MovePosStart();

	/// <summary>
	/// ���W�ړ�
	/// </summary>
	void MovePos();

	/// <summary>
	/// �����؂�ւ��J�n
	/// </summary>
	void ChanegeDimensionStart();

	/// <summary>
	/// �X�e�[�W�N���A�̃`�F�b�N
	/// </summary>
	void StageClearCheck();

	/// <summary>
	/// �}�b�v�ԍ��̍��W���擾
	/// </summary>
	Vector3 GetMapChipPos(const XMINT3& mapChipNumberPos);

	/// <summary>
	/// �C�[�W���O�f�[�^�̐ݒ�
	/// </summary>
	/// <param name="count">�t���[���̃J�E���g</param>
	void SetEaseData(const int count);
	/// <summary>
	/// �C�[�W���O���J�n���鎞�̈ʒu�̕ۑ�
	/// </summary>
	/// <param name="position"></param>
	void SetPlayerStratPos(const Vector3& position) { playerStratPos_ = position; }
	/// <summary>
	/// �C�[�W���O���I�����鎞�̈ʒu�̕ۑ�
	/// </summary>
	/// <param name="position"></param>
	void SetPlayerEndPos(const Vector3& position) { playerEndPos_ = position; }

private: //�ÓI�����o�ϐ�
	//�v���C���[�̑傫��
	static const float playerSize;

private: //�����o�ϐ�
	//�v���C���[�ʒu��\���}�b�v�ԍ�
	XMINT3 mapChipNumberPos;
	//�Q�[���J����
	GameCamera* gameCamera = nullptr;
	//�A�N�V����
	ActionPhase actionPhase = ActionPhase::None;
	//�A�N�V�����p�^�C�}�[
	int32_t actionTimer = 0;
	//�u���b�N�̂ǂ̖ʈړ����邩�t�F�[�Y
	MoveSurfacePhase moveSurfacePhase = MoveSurfacePhase::Upward;
	//�ړ��O���W
	Vector3 moveBeforePos;
	//�ړ��ڕW���W
	Vector3 moveAfterPos;
	//�S�[��������
	bool isGoal = false;
	// �֐��̊Ǘ�
	std::vector<std::function<void()>> func_;
	// �֐��̔ԍ�
	size_t phase_ = 0;
#pragma region
	// �J�n�ʒu
	Vector3 playerStratPos_;
	// �I���ʒu
	Vector3 playerEndPos_;
	// �C�[�W���O�f�[�^
	std::unique_ptr<EaseData> easeData_;
#pragma endregion �C�[�W���O�֌W

	//�ړ��������I������^�C�~���O
	bool isMove;
};