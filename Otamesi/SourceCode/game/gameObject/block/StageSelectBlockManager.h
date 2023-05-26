#pragma once
#include "ObjObject3d.h"
#include <array>

/// <summary>
/// �X�e�[�W�I��p�u���b�N�Ǘ��I�u�W�F�N�g
/// </summary>
class StageSelectBlockManager : public ObjObject3d
{
public: //enum
	/// <summary>
	/// �u���b�N�Ǘ��̏ꏊ�t�F�[�Y
	/// </summary>
	enum BlockManagerPositionPhase
	{
		Center,	//���S
		OutscreenRight, //��ʊO(�E)
		OutscreenLeft, //��ʊO(��)

		PositionNum,	//���W�̐�
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="positionPhase">�u���b�N�Ǘ��̏ꏊ�t�F�[�Y</param>
	/// <returns>�X�e�[�W�I��p�u���b�N�Ǘ��I�u�W�F�N�g</returns>
	static StageSelectBlockManager* Create(const BlockManagerPositionPhase& positionPhase);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="positionPhase">�u���b�N�Ǘ��̏ꏊ�t�F�[�Y</param>
	/// <returns>����</returns>
	bool Initialize(const BlockManagerPositionPhase& positionPhase);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �ړ��J�n
	/// </summary>
	/// <param name="nextPositionPhase">�u���b�N�Ǘ��̏ꏊ�t�F�[�Y</param>
	void MoveStart(const BlockManagerPositionPhase& nextPositionPhase);

	//getter
	bool GetIsDelete() { return isDelete; }
	bool GetIsMove() { return isMove; }

private: //�����o�֐�
	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();

private: //�ÓI�����o�ϐ�
	//�u���b�N�Ǘ��̏ꏊ
	static std::array<Vector3, PositionNum> managerPos;
	//��x�̈ړ��ɂ����鎞��
	static const int32_t moveTime = 50;

private: //�����o�ϐ�
	//�ꏊ�t�F�[�Y
	BlockManagerPositionPhase blockManagerPositionPhase;
	//���Ɉړ�����ꏊ�t�F�[�Y
	BlockManagerPositionPhase nextBlockManagerPositionPhase;
	//�폜�t���O
	bool isDelete = false;
	//�ړ�����
	bool isMove = false;
	//�ړ��p�^�C�}�[
	int32_t moveTimer;
};