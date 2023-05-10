#pragma once
#include "ObjObject3d.h"
#include "GameCamera.h"

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
	/// <param name="mapChipNumberPos">�v���C���[�ʒu��\���}�b�v�ԍ�</param>
	/// <param name="gameCamera">�Q�[���J����</param>
	/// <returns>�v���C���[</returns>
	static Player* Create(ObjModel* model, const XMINT3& mapChipNumberPos, GameCamera* gameCamera);

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	//getter
	const bool GetIsGoal() { return isGoal; }

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

private: //�ÓI�����o�ϐ�
	//�v���C���[�̑傫��
	static const float playerSize;

private: //�����o�ϐ�
	//�v���C���[�ʒu��\���}�b�v�ԍ�(Z,Y,X�̏��ԂɂȂ��Ă���)
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
};