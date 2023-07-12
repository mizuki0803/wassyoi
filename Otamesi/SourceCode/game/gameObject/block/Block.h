#pragma once
#include "BaseBlock.h"
#include "ObjObject3d.h"
#include "EaseData.h"

/// <summary>
/// �}�b�v�p�u���b�N
/// </summary>
class Block
{
public: //enum
	/// <summary>
	/// �u���b�N�̎��
	/// </summary>
	enum class BROCK_TYPE {
		block,
		plane = 10,
		goal,
		player,
		size,
	};

public: //�ÓI�����o�֐�

	/// <summary>
	/// ���݂̂̏�����
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// ��������
	/// </summary>	
	/// <param name="_blockType">���f���ԍ�</param>
	/// <param name="mapChipNum">�}�b�v�`�b�v�̔ԍ�</param>
	/// <returns>�u���b�N</returns>
	static Block* Create(const int _blockType, const XMINT3& mapChipNum);

	/// <summary>
	/// �`��
	/// </summary>
	static void Draw();

	/// <summary>
/// frame���Ƃ̏�����
/// </summary>
	void FrameReset();

	//setter
	void SetRotation(const XMFLOAT3& _rotation) { rotation = _rotation; }
	void SetColor(const XMFLOAT4& _color) { color = _color; }

	//getter
	static float GetBlockSize() { return Block::blockSize; }

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="_blockType">���f��</param>
	/// <param name="mapChipNum">�}�b�v�`�b�v�̔ԍ�</param>
	/// <returns>����</returns>
	bool Initialize(const int _blockType, const XMINT3& mapChipNum);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �u���b�N�^�C�v�̎擾
	/// </summary>
	/// <returns>�u���b�N�^�C�v</returns>
	BROCK_TYPE GetBlockType() { return blockType; }

protected: //�ÓI�����o�ϐ�
	//�u���b�N��̑傫��
	static const float blockSize;
	//�u���b�N�`��p
	static std::unique_ptr<BaseBlock> instanceBlock;

protected: // �����o�ϐ�
	//�u���b�N�̎��
	BROCK_TYPE blockType;
	//�傫��
	XMFLOAT3 scale = { 1, 1, 1 };
	//��]�p
	XMFLOAT3 rotation = { 0, 0, 0 };
	//���W
	Vector3 position = { 0, 0, 0 };
	//�F
	XMFLOAT4 color = { 1, 1, 1, 1 };
	//�e�I�u�W�F�N�g
	ObjObject3d* parent;
};