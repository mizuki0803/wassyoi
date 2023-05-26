#pragma once
#include "ObjObject3d.h"
#include "EaseData.h"

/// <summary>
/// �}�b�v�p�u���b�N
/// </summary>
class Block : public ObjObject3d
{
public: //enum
	/// <summary>
	/// �u���b�N�̎��
	/// </summary>
	enum class BlockType
	{
		Block,
		Plane,
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>	
	/// <param name="model">���f��</param>
	/// <param name="mapChipNum">�}�b�v�`�b�v�̔ԍ�</param>
	/// <returns>�u���b�N</returns>
	static Block* Create(ObjModel* model, const XMINT3& mapChipNum);

	//getter
	static float GetBlockSize() { return Block::blockSize; }

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="mapChipNum">�}�b�v�`�b�v�̔ԍ�</param>
	/// <returns>����</returns>
	bool Initialize(ObjModel* model, const XMINT3& mapChipNum);

protected: //�ÓI�����o�ϐ�
	//�u���b�N��̑傫��
	static const float blockSize;

protected: // �����o�ϐ�
	//�u���b�N�̎��
	BlockType blockType;
};