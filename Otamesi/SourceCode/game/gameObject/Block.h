#pragma once
#include "ObjObject3d.h"

/// <summary>
/// �}�b�v�p�u���b�N
/// </summary>
class Block : public ObjObject3d
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�u���b�N</returns>
	static Block* Create(ObjModel* model, const Vector3& position);

	//getter
	static float GetBlockSize() { return Block::blockSize; }

private: //�ÓI�����o�ϐ�
	//�u���b�N��̑傫��
	static const float blockSize;
};