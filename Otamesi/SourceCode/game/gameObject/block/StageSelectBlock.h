#pragma once
#include "Block.h"

/// <summary>
/// �X�e�[�W�I��p�u���b�N
/// </summary>
class StageSelectBlock : public Block
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>	
	/// <param name="_blockType">���f���ԍ�</param>
	/// <param name="mapChipNum">�}�b�v�`�b�v�̔ԍ�</param>
	/// <param name="mapSize">�}�b�v�̑傫��</param>
	/// <param name="parent">���S�_�p�̐e�I�u�W�F�N�g</param>
	/// <returns>�X�e�[�W�I��p�u���b�N</returns>
	static StageSelectBlock* Create(const int _blockType, const XMINT3& mapChipNum, const XMINT3& mapSize, ObjObject3d* parent);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="_blockType">���f���ԍ�</param>
	/// <param name="mapChipNum">�}�b�v�`�b�v�̔ԍ�</param>
	/// <param name="mapSize">�}�b�v�̑傫��</param>
	/// <param name="parent">���S�_�p�̐e�I�u�W�F�N�g</param>
	/// <returns>����</returns>
	bool Initialize(const int _blockType, const XMINT3& mapChipNum, const XMINT3& mapSize, ObjObject3d* parent);
};